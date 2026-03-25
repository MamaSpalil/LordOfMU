#include "stdafx.h"
#include "MuWindow.h"
#include "AutoClicker.h"
#include "ClickerJobFactory.h"
#include "AsyncKeyQueue.h"
#include "MuWindowUtil.h"
#include "version.h"
#include "ClickerLogger.h"
#include <map>
#include <vector>
#include <string>
#include <TlHelp32.h>
#include <windowsx.h>
#ifdef SubclassWindow
#undef SubclassWindow
#endif
#include <sys/stat.h>

CMuWindow* CMuWindow::s_pInstance = NULL;

CGetAsyncKeyStateTramp CMuWindow::GetAsyncKeyState;
CChangeDisplaySettingsATramp CMuWindow::ChangeDisplaySettingsA;
CGetForegroundWindowTramp CMuWindow::GetForegroundWindowTr;
CGetActiveWindowTramp CMuWindow::GetActiveWindowTr;


/**
 * \brief 
 */
CMuWindow::CMuWindow()
	: m_cSettingsDlg()
{
	m_hEvent = CreateEvent(0, 0, 0, _T("__LordOfMU_Event01__"));
	SetEvent(m_hEvent);

	m_hKbdHook = NULL;
	m_fWasLastActiveInstance = FALSE;

	m_fIsWndActive = TRUE;
	m_fBlockInput = FALSE;

	m_fGuiActive = FALSE;
	m_bPendingShowSettings = FALSE;
	m_bPendingShowHistory = FALSE;
	m_iInstanceNumber = 0;
	m_pClicker = NULL;

	memset(m_buffDevmode, 0, sizeof(m_buffDevmode));
	m_ulVersion = 0;
	m_fWindow = false;

#if defined(__CLICKER_AVANTA__) || defined(__CLICKER_ELITE__)
	CRegKey key;
	key.Create(HKEY_CURRENT_USER, _T("Software"));
	key.Create(key.m_hKey, _T("LordJerec"));
	key.Create(key.m_hKey, _T("MUAutoClicker"));
	key.Create(key.m_hKey, _T("Config"));

	DWORD dwType = 0;
	key.QueryDWORDValue(_T("ClientType"), dwType); // 0 - N/S, 1 - S3, 2 - S4

	if (dwType < 2)
		m_ulVersion = 104;
	else
		m_ulVersion = 105;
#endif

	memset(m_vFnKeys, 0, sizeof(m_vFnKeys));

	m_vFnKeys[0].vk = VK_F5;
	m_vFnKeys[1].vk = VK_F6;
	m_vFnKeys[2].vk = VK_F7;
	m_vFnKeys[3].vk = VK_F8;
	m_vFnKeys[4].vk = VK_F9;
	m_vFnKeys[5].vk = VK_F10;
	m_vFnKeys[6].vk = VK_F11;
	m_vFnKeys[7].vk = VK_F12;
	m_vFnKeys[8].vk = VK_ESCAPE;

	CAsyncKeyQueue::Init();
}


/**
 * \brief 
 */
CMuWindow::~CMuWindow()
{
	CAsyncKeyQueue::Term();

	if (m_hKbdHook != NULL)
		UnhookWindowsHookEx(m_hKbdHook);

	CloseHandle(m_hEvent);
}


/**
 * \brief 
 */
BOOL CMuWindow::Init(HWND hwndMuWindow, int iInstanceNumber)
{
	if (s_pInstance != NULL)
		return FALSE; // Already initialized

	s_pInstance = new CMuWindow();

	if (!s_pInstance)
		return FALSE; // Out of memory

	if (!s_pInstance->SubclassWindow(hwndMuWindow))
	{
		delete s_pInstance;
		s_pInstance = NULL;
		return FALSE;
	}

	s_pInstance->m_fIsWndActive = (::GetForegroundWindow() == hwndMuWindow);
	s_pInstance->m_iInstanceNumber = iInstanceNumber;

	s_pInstance->PostMessage(WM_MUWND_INITIALIZE, 0, 0);
	return TRUE;
}


void CMuWindow::Term()
{
	GetAsyncKeyState.UnPatch();
	ChangeDisplaySettingsA.UnPatch();
	GetForegroundWindowTr.UnPatch();
	GetActiveWindowTr.UnPatch();

	// Shut down ImGui overlay and D3D9 hook
	if (s_pInstance != NULL)
	{
		s_pInstance->m_cOverlay.Shutdown();
		D3D9Hook::Uninstall();

		delete s_pInstance;
		s_pInstance = NULL;
	}
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnInitMuWindow(UINT, WPARAM, LPARAM, BOOL&)
{
	GetAsyncKeyState.Patch(&MyGetAsyncKeyState);
	ChangeDisplaySettingsA.Patch(&MyChangeDisplaySettingsA);
	GetForegroundWindowTr.Patch(&MyGetForegroundWindow);
//	GetActiveWindowTr.Patch(&MyGetActiveWindow);

	if (m_hKbdHook == NULL)
		m_hKbdHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProcLL, _AtlBaseModule.GetModuleInstance(), 0);

	memset(m_buffDevmode, 0, sizeof(m_buffDevmode));
	((LPDEVMODEA)m_buffDevmode)->dmSize = sizeof(DEVMODEA);
	((LPDEVMODEA)m_buffDevmode)->dmDriverExtra = sizeof(m_buffDevmode) - sizeof(DEVMODEA);

	EnumDisplaySettingsA(0, ENUM_CURRENT_SETTINGS, (LPDEVMODEA)m_buffDevmode);


	TCHAR szFileName[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szFileName, _MAX_PATH);

	for (int i=(int)_tcslen(szFileName)-1; i >= 0 && szFileName[i] != L'\\'; --i)
	{
		szFileName[i] = 0;
	}

	TCHAR szSetFile[_MAX_PATH+1] = {0};
	_stprintf(szSetFile, _T("MUAutoClicker_%d.dat"), m_iInstanceNumber);
	_tcscat(szFileName, szSetFile);

	m_cSettingsDlg.LoadSettings(szFileName);
	
	if (::IsWindow(m_cSettingsDlg.m_hWnd))
	{
		m_cSettingsDlg.DestroyWindow();
	}

	// Legacy CSettingsDlg is used only for settings data (GetSettingsObj).
	// Skip creating its window to avoid wasting HWND and GDI resources.
	// m_cSettingsDlg.Create(m_hWnd);
	m_cLaunchMuDlg.Create(m_hWnd);

	// -----------------------------------------------------------------------
	// D3D9 EndScene hook + ImGui overlay.
	// All UI (HUD buttons, settings dialog, history dialog) is now rendered
	// directly inside the game's Direct3D 9 EndScene call via Dear ImGui.
	// No popup windows, no focus/activation/capture conflicts.
	// -----------------------------------------------------------------------
	if (D3D9Hook::Install(m_hWnd))
	{
		D3D9Hook::SetOnEndScene(OnEndSceneCallback);
		D3D9Hook::SetOnPreReset(OnPreResetCallback);
		D3D9Hook::SetOnPostReset(OnPostResetCallback);

		// Configure overlay callbacks so HUD button presses route to CMuWindow.
		m_cOverlay.SetSettings(&m_cSettingsDlg.GetSettingsObj());
		m_cOverlay.SetOnSettingsClicked(OnOverlaySettingsClicked, this);
		m_cOverlay.SetOnStartStopClicked(OnOverlayStartStopClicked, this);
		m_cOverlay.SetOnHistoryClicked(OnOverlayHistoryClicked, this);
		m_cOverlay.SetOnSettingsApply(OnOverlayApplyClicked, this);

		WriteClickerLog("D3D9 EndScene hook installed, ImGui overlay ready");
	}
	else
	{
		WriteClickerLog("D3D9 EndScene hook FAILED - overlay disabled");
	}

	CRegKey cRegKey;
	DWORD dwWndMode = 0;

	if (ERROR_SUCCESS == cRegKey.Open(HKEY_CURRENT_USER, _T("Software"))
				&& ERROR_SUCCESS == cRegKey.Open(cRegKey, _T("Webzen"))
				&& ERROR_SUCCESS == cRegKey.Open(cRegKey, _T("Mu"))
				&& ERROR_SUCCESS == cRegKey.Open(cRegKey, _T("Config"))
				&& ERROR_SUCCESS == cRegKey.QueryDWORDValue(_T("WindowMode"), dwWndMode)
				&& dwWndMode == 1)
	{
		m_fWindow = true;

		RECT rc, rcClient;
		GetWindowRect(&rc);
		GetClientRect(&rcClient);
		ClientToScreen(&rcClient);

		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		int offs = rcClient.top - rc.top;

		if (cy - GetSystemMetrics(SM_CYCAPTION) < rc.bottom - rc.top)
		{
			MoveWindow((cx - rc.right + rc.left)/2,
							-offs, rc.right - rc.left, rc.bottom - rc.top);
		}
	}

	SetTimer(1011, 100, 0);

	WriteClickerLog("DLL successfully hooked into main.exe, clicker initialized");

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnIsClickerInstalled(UINT, WPARAM, LPARAM, BOOL&)
{
	return (LRESULT)m_iInstanceNumber;
}


/**
 * \brief Returns a human-readable name for a virtual key code (F1-F12, Escape).
 */
static const char* GetFKeyName(UINT vk)
{
	switch (vk)
	{
	case VK_F1:     return "F1";
	case VK_F2:     return "F2";
	case VK_F3:     return "F3";
	case VK_F4:     return "F4";
	case VK_F5:     return "F5";
	case VK_F6:     return "F6";
	case VK_F7:     return "F7";
	case VK_F8:     return "F8";
	case VK_F9:     return "F9";
	case VK_F10:    return "F10";
	case VK_F11:    return "F11";
	case VK_F12:    return "F12";
	case VK_ESCAPE: return "ESC";
	default:        return "?";
	}
}


/**
 * \brief Returns a description of what the F-key does in the AVANTA+ELITE build.
 */
static const char* GetFKeyDescription(UINT vk, BOOL fShift)
{
	switch (vk)
	{
	case VK_F1:     return "Game Help (not handled by AutoClicker)";
	case VK_F2:     return "Game Chat Toggle (not handled by AutoClicker)";
	case VK_F3:     return "Game Whisper (not handled by AutoClicker)";
	case VK_F4:     return "Game Command (not handled by AutoClicker)";
	case VK_F5:     return "Toggle AutoClicker Start/Stop";
	case VK_F6:     return "Force Start AutoClicker";
	case VK_F7:     return "Start AutoClicker (no mouse move)";
	case VK_F8:     return "Force Stop AutoClicker";
	case VK_F9:     return fShift ? "Show History / Pickup Log (Shift+F9)"
	                              : "Show Settings GUI (F9)";
	case VK_F10:    return fShift ? "Move Item command (Shift+F10)"
	                              : "Launch another MU instance";
	case VK_F11:    return "Switch between MU windows";
	case VK_F12:    return "Hide/Show all MU windows";
	default:        return "Unknown key";
	}
}


/**
 * \brief 
 */
LRESULT CALLBACK CMuWindow::KeyboardProcLL(int code, WPARAM wParam, LPARAM lParam)
{
	CMuWindow* pThis = CMuWindow::GetInstance();

	if (!pThis)
		return 0;

	if (code < 0)
		return CallNextHookEx(pThis->m_hKbdHook, code, wParam, lParam);

	KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;

	// Debug: log F1-F12 key events arriving at the LL hook
	if (kbd->vkCode >= VK_F1 && kbd->vkCode <= VK_F12)
	{
		BOOL bGameFg = (CMuWindow::GetForegroundWindowTr() == pThis->m_hWnd);
		WriteClickerLogFmt("KEYDBG", "LL-Hook: %s %s | GameForeground=%s | Action=%s",
			GetFKeyName(kbd->vkCode),
			(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? "KEYDOWN" : "KEYUP",
			bGameFg ? "YES" : "NO",
			bGameFg ? "PassToTimer (game foreground, Timer handles it)"
			        : "DispatchToOnKeyboardEvent (game background)");
	}

	if (CMuWindow::GetForegroundWindowTr() != pThis->m_hWnd)
	{
		// When game is NOT foreground, dispatch all F-keys through the LL
		// hook with fCheckFgWnd=FALSE so OnKeyboardEvent's own foreground
		// check does not reject the call.
		if (pThis->OnKeyboardEvent(kbd->vkCode, (UINT)wParam, FALSE))
			return 0;
	}
	// When game IS foreground, do NOT consume F9 here.  Let it pass
	// through to the system like F5 does.  The 100ms Timer polling
	// (GetAsyncKeyState) in OnTimer is the primary handler for all
	// F-keys when the game is in focus — same reliable path that F5 uses.

	return CallNextHookEx(pThis->m_hKbdHook, code, wParam, lParam);
}


/**
 * \brief 
 */
BOOL CMuWindow::OnKeyboardEvent(UINT vkCode, UINT uMsg, BOOL fCheckFgWnd)
{
	const BOOL fIsF1F12 = (vkCode >= VK_F1 && vkCode <= VK_F12);
	const BOOL fShiftDown = (CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	const char* szKeyEvent = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) ? "KEYDOWN" : "KEYUP";

	if (vkCode == VK_F12 && uMsg == WM_KEYDOWN)
	{
		// Show/Hide all MU Windows
		if (CMuWindow::GetForegroundWindowTr() == m_hWnd)
		{
			m_fWasLastActiveInstance = TRUE;
			PostMessage(WM_HIDE_MU, 0, 0);
			WriteClickerLogFmt("KEYDBG", "[F12] %s -> OK: %s | Game is foreground -> Hiding all MU windows",
				szKeyEvent, GetFKeyDescription(VK_F12, FALSE));
		}
		else if (m_fWasLastActiveInstance)
		{
			m_fWasLastActiveInstance = FALSE;
			PostMessage(WM_SHOW_MU, 0, 0);
			WriteClickerLogFmt("KEYDBG", "[F12] %s -> OK: %s | Was last active instance -> Showing all MU windows",
				szKeyEvent, GetFKeyDescription(VK_F12, FALSE));
		}
		else
		{
			m_fWasLastActiveInstance = FALSE;
			WriteClickerLogFmt("KEYDBG", "[F12] %s -> BLOCKED: %s | Reason: Game is NOT foreground AND this is NOT the last active instance",
				szKeyEvent, GetFKeyDescription(VK_F12, FALSE));
			return FALSE;
		}

		return TRUE;
	}

	if (fCheckFgWnd && CMuWindow::GetForegroundWindowTr() != m_hWnd)
	{
		if (fIsF1F12 && uMsg == WM_KEYUP)
		{
			WriteClickerLogFmt("KEYDBG", "[%s] %s -> BLOCKED: %s | Reason: Game window is NOT foreground (fCheckFgWnd=TRUE, foreground=0x%p, game=0x%p)",
				GetFKeyName(vkCode), szKeyEvent, GetFKeyDescription(vkCode, fShiftDown),
				CMuWindow::GetForegroundWindowTr(), m_hWnd);
		}
		return FALSE;
	}

	// While the ImGui overlay (Settings/History) is open, block clicker
	// control keys (F5-F8) to prevent accidental start/stop behind the
	// overlay.  F9 is intentionally allowed through so that the user can
	// toggle (close) the overlay or switch between Settings and History.
	if (m_fGuiActive)
	{
		if (vkCode >= VK_F5 && vkCode <= VK_F8)
		{
			if (uMsg == WM_KEYUP)
			{
				WriteClickerLogFmt("KEYDBG", "[%s] %s -> BLOCKED: %s | Reason: ImGui overlay is open (m_fGuiActive=TRUE), F5-F8 clicker control keys are blocked to prevent accidental start/stop",
					GetFKeyName(vkCode), szKeyEvent, GetFKeyDescription(vkCode, fShiftDown));
			}
			return TRUE;  // Swallow clicker control keys
		}
	}

#if defined(__HACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
	if (vkCode == VK_F5 && uMsg == WM_KEYUP)
	{
		if (fShiftDown)
		{
			WriteClickerLogFmt("KEYDBG", "[Shift+F5] %s -> OK: Sending '//script toggle' to server",
				szKeyEvent);
			SayToServer("//script toggle");
		}
		else
		{
			WriteClickerLogFmt("KEYDBG", "[F5] %s -> OK: Sending '//autokill toggle' to server",
				szKeyEvent);
			SayToServer("//autokill toggle");
		}

		return TRUE;
	}
#else
	if (vkCode == VK_F5 && uMsg == WM_KEYUP)
	{
		if (m_pClicker != NULL)
		{
			WriteClickerLogFmt("KEYDBG", "[F5] %s -> OK: %s | Clicker is RUNNING -> Sending WM_STOP_CLICKER",
				szKeyEvent, GetFKeyDescription(VK_F5, FALSE));
			PostMessage(WM_STOP_CLICKER, 0, 0);
		}
		else
		{
			WriteClickerLogFmt("KEYDBG", "[F5] %s -> OK: %s | Clicker is STOPPED -> Sending WM_START_CLICKER",
				szKeyEvent, GetFKeyDescription(VK_F5, FALSE));
			PostMessage(WM_START_CLICKER, 0, 0);
		}

		return TRUE;
	}
#endif

	if (vkCode == VK_F6 && uMsg == WM_KEYUP)
	{
		WriteClickerLogFmt("KEYDBG", "[F6] %s -> OK: %s | Sending WM_START_CLICKER",
			szKeyEvent, GetFKeyDescription(VK_F6, FALSE));
		PostMessage(WM_START_CLICKER, 0, 0);
		return TRUE;
	}

	if (vkCode == VK_F7 && uMsg == WM_KEYUP)
	{
		WriteClickerLogFmt("KEYDBG", "[F7] %s -> OK: %s | Sending WM_START_CLICKER (no mouse move)",
			szKeyEvent, GetFKeyDescription(VK_F7, FALSE));
		PostMessage(WM_START_CLICKER, 0, TRUE);
		return TRUE;
	}

	if (vkCode == VK_F8 && uMsg == WM_KEYUP)
	{
		WriteClickerLogFmt("KEYDBG", "[F8] %s -> OK: %s | Sending WM_STOP_CLICKER",
			szKeyEvent, GetFKeyDescription(VK_F8, FALSE));
		PostMessage(WM_STOP_CLICKER, 0, 0);
		return TRUE;
	}

	// F9 / Shift+F9: Same pattern as F5 — act on KEYUP only.
	// Timer polls GetAsyncKeyState every 100ms; when the key is released
	// the Timer fires OnKeyboardEvent(VK_F9, WM_KEYUP).  Shift state is
	// checked at this moment — the user is expected to still hold Shift
	// when releasing F9 (same assumption F10+Shift uses below).
	if (vkCode == VK_F9 && uMsg == WM_KEYUP)
	{
		if (fShiftDown)
		{
			WriteClickerLogFmt("KEYDBG", "[Shift+F9] %s -> OK: %s | Sending WM_SHOW_HISTORY | OverlayActive=%s",
				szKeyEvent, GetFKeyDescription(VK_F9, TRUE),
				m_fGuiActive ? "YES" : "NO");
			PostMessage(WM_SHOW_HISTORY, 0, 0);
		}
		else
		{
			WriteClickerLogFmt("KEYDBG", "[F9] %s -> OK: %s | Sending WM_SHOW_SETTINGS_GUI | OverlayActive=%s | ClickerRunning=%s",
				szKeyEvent, GetFKeyDescription(VK_F9, FALSE),
				m_fGuiActive ? "YES" : "NO",
				m_pClicker ? "YES" : "NO");
			PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);
		}

		return TRUE;
	}

	if (vkCode == VK_F10 && uMsg == WM_KEYUP)
	{
		if (fShiftDown)
		{
			WriteClickerLogFmt("KEYDBG", "[Shift+F10] %s -> OK: %s | Sending '//moveitem 12 76' to server",
				szKeyEvent, GetFKeyDescription(VK_F10, TRUE));
			SayToServer("//moveitem 12 76");
		}
		else
		{
			WriteClickerLogFmt("KEYDBG", "[F10] %s -> OK: %s | Sending WM_LAUNCH_MU",
				szKeyEvent, GetFKeyDescription(VK_F10, FALSE));
			// Launch another MU instance
			PostMessage(WM_LAUNCH_MU, 0, 0);
		}

		return TRUE;
	}

	if (vkCode == VK_F11 && uMsg == WM_KEYUP)
	{
		WriteClickerLogFmt("KEYDBG", "[F11] %s -> OK: %s | Sending WM_SWITCH_INSTANCE",
			szKeyEvent, GetFKeyDescription(VK_F11, FALSE));
		// Switch between MU windows
		PostMessage(WM_SWITCH_INSTANCE, 0, 0);
		return TRUE;
	}

	if (vkCode == VK_ESCAPE && uMsg == WM_KEYUP && m_fGuiActive)
	{
		WriteClickerLogFmt("KEYDBG", "[ESC] %s -> OK: Close ImGui overlay | SettingsVisible=%s HistoryVisible=%s",
			szKeyEvent,
			m_cOverlay.IsInitialized() ? (m_cOverlay.IsAnyWindowVisible() ? "YES" : "NO") : "N/A",
			m_fGuiActive ? "YES" : "NO");
		// Close ImGui overlay windows
		m_cOverlay.HideSettings();
		m_cOverlay.HideHistory();
		m_fGuiActive = FALSE;
		return TRUE;
	}

	// Key was not handled by any branch
	if (fIsF1F12 && uMsg == WM_KEYUP)
	{
		WriteClickerLogFmt("KEYDBG", "[%s] %s -> NOT HANDLED: %s | Reason: No handler registered for this key in AutoClicker (key passed to game)",
			GetFKeyName(vkCode), szKeyEvent, GetFKeyDescription(vkCode, fShiftDown));
	}

	return FALSE;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Forward mouse messages to ImGui overlay.  If ImGui wants the mouse
	// (hovering over an overlay window), block the game from processing it.
	if (m_cOverlay.IsInitialized())
	{
		if (m_cOverlay.WndProcHandler(m_hWnd, uMsg, wParam, lParam))
		{
			bHandled = TRUE;
			return 0;
		}
	}

	// When the autoclicker is running and input is blocked, left-click messages
	// are only allowed through if they land on HUD buttons.  This prevents
	// user clicks from conflicting with the autoclicker's synthetic clicks.
	if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDBLCLK)
	{
		if (m_fBlockInput && m_pClicker != NULL)
		{
			// Block user clicks while autoclicker is running
			bHandled = TRUE;
		}
		else
		{
			bHandled = FALSE;
		}
	}
	else
	{
		bHandled = m_fBlockInput;
	}

	return 0;
}


/**
 * \brief  BUG-3 fix: Forward keyboard messages (WM_KEYDOWN, WM_KEYUP, WM_CHAR,
 *         WM_SYSKEYDOWN, WM_SYSKEYUP) to the ImGui overlay so that keyboard
 *         navigation and text input work inside overlay windows.
 *
 *         Previously only mouse messages were forwarded via OnMouseMessage.
 *         The keyboard WM_KEY* path in CImGuiOverlay::WndProcHandler was
 *         dead code because no handler routed these messages to it.
 */
LRESULT CMuWindow::OnKeyboardMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Block F9 from reaching the game's own WndProc (the game might have
	// its own F9 handler).  The actual F9 action is handled by the 100ms
	// Timer polling in OnTimer — same path F5 uses.
	if (wParam == VK_F9 && (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP))
	{
		if (uMsg == WM_KEYDOWN)
		{
			WriteClickerLogFmt("KEYDBG", "WndProc: F9 KEYDOWN blocked from game WndProc (handled by Timer polling)");
		}
		bHandled = TRUE;
		return 0;
	}

	// Forward keyboard messages to ImGui overlay.  If ImGui wants the
	// keyboard (an overlay text field is focused), block the game from
	// processing the keystroke.
	if (m_cOverlay.IsInitialized())
	{
		if (m_cOverlay.WndProcHandler(m_hWnd, uMsg, wParam, lParam))
		{
			bHandled = TRUE;
			return 0;
		}
	}

	// Let the game process the key normally.
	bHandled = FALSE;
	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnActivateApp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_fIsWndActive = (BOOL)wParam;

	// Update ImGui overlay game-active state
	m_cOverlay.SetGameActive(m_fIsWndActive != FALSE);

	if (m_fIsWndActive)
	{
		bHandled = FALSE;
	}
	else
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (LOWORD(wParam) != WA_INACTIVE)
	{
		bHandled = FALSE;
	}
	else
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (IsWindow())
		SendMessage(WM_SWITCH_INSTANCE, 0, 0);

	if (::IsWindow(m_cSettingsDlg.m_hWnd))
		m_cSettingsDlg.DestroyWindow();

	if (::IsWindow(m_cLaunchMuDlg.m_hWnd))
		m_cLaunchMuDlg.DestroyWindow();

	// Shut down ImGui overlay and D3D9 hook
	m_cOverlay.Shutdown();
	D3D9Hook::Uninstall();

	// Stop autoclicker first
	BOOL fHandled = FALSE;
	OnStopClicker(0, 0, 0, fHandled);

	DWORD dwStart = GetTickCount();

	while (m_pClicker != 0)
	{
		MSG msg = {0};
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if ((int)GetTickCount() - (int)dwStart > 5000)
		{
			break;
		}

		Sleep(10);
	}

	// Clicker is now stopped.

	if (IsWindow())
	{
		KillTimer(1011);
	}

	HWND hWnd = m_hWnd;
	WNDPROC proc = m_pfnSuperWindowProc;

	UnsubclassWindow();
	return CallWindowProc(proc, hWnd, uMsg, wParam, lParam);
}


/**
 * \brief Shows the settings overlay via ImGui (F9 hotkey).
 *        Since the overlay renders directly in EndScene, there is no need
 *        for a separate window, local message pump, or capture/activation
 *        workarounds.
 */
LRESULT CMuWindow::OnShowSettingsGUI(UINT, WPARAM, LPARAM, BOOL&)
{
	WriteClickerLogFmt("KEYDBG", ">>> OnShowSettingsGUI: Received WM_SHOW_SETTINGS_GUI | ClickerRunning=%s | OverlayInitialized=%s | SettingsVisible=%s",
		m_pClicker ? "YES" : "NO",
		m_cOverlay.IsInitialized() ? "YES" : "NO",
		(m_cOverlay.IsInitialized() && m_cOverlay.IsAnyWindowVisible()) ? "YES" : "NO");

	// Guard: if the overlay hasn't been lazily initialized yet (first EndScene
	// hasn't fired), defer the request — it will be applied once the overlay
	// initializes in OnEndSceneCallback.
	if (!m_cOverlay.IsInitialized())
	{
		m_bPendingShowSettings = TRUE;
		m_bPendingShowHistory = FALSE;  // Only one panel at a time.
		WriteClickerLogFmt("KEYDBG", ">>> OnShowSettingsGUI: DEFERRED - overlay not initialized yet (will open after first EndScene)");
		return 0;
	}

	// BUG-K fix: If the autoclicker is running, stop it AND open the settings
	// overlay in one action.  Previously this would only stop the clicker and
	// require a second F9 press.  The clicker stop is async (OnClickerJobFinished
	// clears m_pClicker), but this does not affect the ImGui overlay — it simply
	// shows the settings window while the clicker winds down.
	if (m_pClicker != NULL)
	{
		WriteClickerLogFmt("KEYDBG", ">>> OnShowSettingsGUI: Clicker was running -> stopping clicker first (async)");
		PostMessage(WM_STOP_CLICKER, 0, 0);
		MessageBeep(MB_ICONINFORMATION);
	}

	// Toggle settings overlay in ImGui
	m_cOverlay.ToggleSettings();
	m_fGuiActive = m_cOverlay.IsAnyWindowVisible();

	WriteClickerLogFmt("KEYDBG", ">>> OnShowSettingsGUI: Settings overlay toggled | SettingsVisible=%s",
		m_cOverlay.IsInitialized() && m_cOverlay.IsAnyWindowVisible() ? "YES" : "NO");

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnErasebkgnd(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	return 1;
}


/**
 * \brief 
 */
SHORT WINAPI CMuWindow::MyGetAsyncKeyState(int vk)
{
	CAsyncKeyQueue* pQueue = CAsyncKeyQueue::GetInstance();
	if (pQueue && pQueue->GetKey(vk))
	{
		Sleep(0);
		return 0x8000U;
	}

	if (VK_F11 == vk || VK_F12 == vk)
		return 0;

	CMuWindow* pInstance = CMuWindow::GetInstance();

	if (!pInstance || !pInstance->m_fIsWndActive)
		return 0;

	Sleep(0);
	return CMuWindow::GetAsyncKeyState(vk);
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnStartClicker(UINT, WPARAM, LPARAM fNoMouseMove, BOOL&)
{
	if (m_pClicker != NULL)
		return 0;

	BOOL fHealOnly = FALSE;

	if ((CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
	{
		fHealOnly = TRUE;
	}

#if 0 //defined(__CLICKER_AVANTA__)
	std::map<int, HWND> vInstances;
	EnumWindows(MyEnumWndProc, (LPARAM)&vInstances);

	if (vInstances.size() > 1) // Avanta version allows only 1 game instance
	{
		Beep(1000, 70);
		return 0;
	}
#endif


#if defined(__CLICKER_AVANTA__) || defined(__CLICKER_ELITE__)
	CRegKey key;
	key.Create(HKEY_CURRENT_USER, _T("Software"));
	key.Create(key.m_hKey, _T("LordJerec"));
	key.Create(key.m_hKey, _T("MUAutoClicker"));
	key.Create(key.m_hKey, _T("Config"));

	DWORD dwType = 0;
	key.QueryDWORDValue(_T("ClientType"), dwType); // 0 - N/S, 1 - S3, 2 - S4

	if (dwType < 2)
		m_ulVersion = 104;
	else
		m_ulVersion = 105;
#endif

	m_pClicker = CClickerJobFactory::CreateClickerJob(m_hWnd, m_ulVersion, m_cSettingsDlg.GetSettings(), fNoMouseMove != 0, fHealOnly);

	if (m_pClicker)
	{	
		if (m_pClicker->Start())
		{
			m_fBlockInput = (fNoMouseMove == 0);
			m_cOverlay.SetClickerRunning(true);

			// Reset session statistics for this new clicker run
			ResetSessionStatsCall();

			WriteClickerLog("Clicker started");
		}
		else
		{
			delete m_pClicker;
			m_pClicker = NULL;
		}
	}
	else // Failed to start clicker
	{
		m_fBlockInput = FALSE;
	}

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnStopClicker(UINT, WPARAM, LPARAM, BOOL&)
{
	if (m_pClicker != NULL)
	{
		m_pClicker->Stop();
		WriteClickerLog("Clicker stop requested");
	}

	m_fBlockInput = FALSE;
	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnClickerJobFinished(UINT, WPARAM, LPARAM, BOOL&)
{
	if (m_pClicker != NULL)
	{
		delete m_pClicker;
		m_pClicker = NULL;
	}

	// m_cHUDButtons.SetClickerRunning(FALSE);
	m_cOverlay.SetClickerRunning(false);

	// Mark session as stopped (runtime freezes)
	StopSessionCall();

	WriteClickerLog("Clicker stopped");

	m_fBlockInput = FALSE;
	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnClickerMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return CallWindowProc(m_pfnSuperWindowProc, m_hWnd, WM_MOUSEMOVE, wParam, lParam);
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnClickerRButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return CallWindowProc(m_pfnSuperWindowProc, m_hWnd, WM_RBUTTONDOWN, wParam, lParam);
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnClickerRButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	// wParam == 1 is a special "release capture" flag from the clicker thread.
	// ReleaseCapture/ClipCursor must be called from the UI thread.
	if (wParam == 1)
	{
		ReleaseCapture();
		ClipCursor(0);
		return 0;
	}

	return CallWindowProc(m_pfnSuperWindowProc, m_hWnd, WM_RBUTTONUP, wParam, lParam);
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnClickerLButtonDown(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return CallWindowProc(m_pfnSuperWindowProc, m_hWnd, WM_LBUTTONDOWN, wParam, lParam);
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnClickerLButtonUp(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return CallWindowProc(m_pfnSuperWindowProc, m_hWnd, WM_LBUTTONUP, wParam, lParam);
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnLaunchMu(UINT, WPARAM, LPARAM, BOOL&)
{
	if (m_pClicker != NULL)
		return 0;

	if (m_fGuiActive)
		return 0;

	m_fGuiActive = TRUE;

	::ReleaseCapture();

	m_cLaunchMuDlg.ShowWindow(SW_SHOWNORMAL);

	InvalidateRect(0, TRUE);
	UpdateWindow();

	PostMessage(WM_NULL, 0, 0);

	BOOL fOldBlockInput = m_fBlockInput;
	m_fBlockInput = FALSE;

	MSG msg = {0};
	while (true)
	{
		if (GetMessage(&msg, 0, 0, 0) > 0)
		{
			if (msg.message == WM_SYSCOMMAND && msg.wParam == SC_CLOSE)
			{
				m_cLaunchMuDlg.ShowWindow(SW_HIDE);
			}
			else if (!m_cLaunchMuDlg.IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			m_cLaunchMuDlg.ShowWindow(SW_HIDE);
		}

		if (!m_cLaunchMuDlg.IsWindowVisible())
			break;
	}

	while (CMuWindow::GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(10);
	}

	m_fBlockInput = fOldBlockInput;
	m_fGuiActive = FALSE;

	if (m_cLaunchMuDlg.m_fResult)
	{
		LaunchMU();
	}

	return 0;
}


/**
 * \brief 
 */
void CMuWindow::LaunchMU()
{
	std::map<int, HWND> vInstances;
	EnumWindows(MyEnumWndProc, (LPARAM)&vInstances);

	int iCount = 0;
	for (int i=1; i < 10; ++i)
	{
		if (vInstances.find(i) == vInstances.end())
		{
			iCount = i;
			break;
		}
	}

	if (iCount == 0)
		return; //max number instances reached


	STARTUPINFO si = {0};
	si.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION pi = {0};

	if (!CreateProcess(0, GetCommandLine(), 0, 0, FALSE, CREATE_SUSPENDED, 0, 0, &si, &pi))
		return;

	PatchProcessMemory(pi.hProcess, (char)(iCount + 0x30), "MuOnline");

	ResumeThread(pi.hThread);
}


/**
 * \brief 
 */
BOOL CALLBACK CMuWindow::MyEnumWndProc(HWND hWnd, LPARAM pData)
{
	if (!pData)
		return FALSE;

	std::map<int, HWND>* pInstances = (std::map<int, HWND>*)pData;

	if (CMuWindowUtil::IsMuWindow(hWnd))
	{
		DWORD_PTR dwRes = 0;
		if (SendMessageTimeout(hWnd, WM_IS_CLICKER_INSTALLED, 0, 0, SMTO_BLOCK, 500, &dwRes) != 0)
		{
			pInstances->insert(std::pair<int, HWND>((int)dwRes, hWnd));
		}
		else
		{
			int nInstanceNum = 100;
			for (nInstanceNum; pInstances->find(nInstanceNum) != pInstances->end() 
						&& nInstanceNum < 32000; ++nInstanceNum);
			
			pInstances->insert(std::pair<int, HWND>(nInstanceNum, hWnd));
		}
	}

	return TRUE;
}


/**
 * \brief 
 */
void CMuWindow::PatchProcessMemory(HANDLE hProcess, char patchByte, const char* szMutexName)
{
	char buff[2048] = {0};
	SIZE_T cnt = 0;
	int addr = 0x00400000;
	bool fStop = false;

	while (!fStop)
	{
		ReadProcessMemory(hProcess, (LPCVOID)(DWORD_PTR)addr, buff, sizeof(buff), &cnt);
		
		if (cnt == 0)
			break;

		int pos = 0;

		for (int i=0; i < (int)cnt; i++)
		{
			char ch = buff[i];

			if (ch == szMutexName[pos])
			{
				pos++;

				if (szMutexName[pos] == 0)
				{
					// <--- Mutex name string is found
					int waddr = addr + i;
					ch = patchByte;
					WriteProcessMemory(hProcess, (LPVOID)(DWORD_PTR)waddr, &ch, 1, &cnt);
					
					fStop = true;
					break;
				}
			}
			else if (pos != 0)
			{
				pos = 0;
				--i;
			}
		}

		if (cnt < sizeof(buff) || addr > 0x00FFFFFF)
		{
			break;
		}
		else
		{
			addr = addr + sizeof(buff) -(int)sizeof(szMutexName) + 2;
		}
	}
}


/**
 * \brief 
 */
LONG WINAPI CMuWindow::MyChangeDisplaySettingsA(LPDEVMODEA pDevMode, DWORD dwFlags)
{
	CMuWindow* pInstance = CMuWindow::GetInstance();

	if (!pInstance)
		return DISP_CHANGE_SUCCESSFUL;

	std::map<int, HWND> vInstances;
	EnumWindows(MyEnumWndProc, (LPARAM)&vInstances);

	if (vInstances.size() > 1)
		return DISP_CHANGE_SUCCESSFUL;

	return CMuWindow::ChangeDisplaySettingsA(pDevMode, dwFlags);
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnSwitchMuInstance(UINT, WPARAM, LPARAM, BOOL&)
{
	std::map<int, HWND> vInstances;
	EnumWindows(MyEnumWndProc, (LPARAM)&vInstances);

	if (vInstances.size() == 0)
		return 0;


	std::map<int, HWND>::iterator it = vInstances.find(m_iInstanceNumber);

	if (it == vInstances.end())
		return 0;


	++it;

	if (it == vInstances.end())
		it = vInstances.begin();

	::BringWindowToTop(it->second);
	::SetForegroundWindow(it->second);

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnShowMu(UINT, WPARAM, LPARAM, BOOL&)
{
	std::map<int, HWND> vInstances;
	EnumWindows(MyEnumWndProc, (LPARAM)&vInstances);

	if (!m_fWindow)
		CMuWindow::ChangeDisplaySettingsA((LPDEVMODEA)m_buffDevmode, 0);


	if (vInstances.size() != 0)
	{
		for (std::map<int, HWND>::iterator it = vInstances.begin(); it != vInstances.end(); ++it)
		{
			if (::IsWindow(it->second) && it->second != m_hWnd)
				::ShowWindow(it->second, SW_SHOWNOACTIVATE);
		}
	}

	SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	SetForegroundWindow(m_hWnd);

	Sleep(250);

	MSG msg = {0};

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CHUDParams hud(m_hWnd, m_ulVersion);

	POINT ptClick = {hud.x0, hud.y0};
	ClientToScreen(&ptClick);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	ptClick.x = (LONG)(((float)ptClick.x/(float)cx)*65535);
	ptClick.y = (LONG)(((float)ptClick.y/(float)cy)*65535);

	INPUT ins[3] = {0};
	ins[0].type = INPUT_MOUSE;
	ins[0].mi.dx = ptClick.x;
	ins[0].mi.dy = ptClick.y;
	ins[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	ins[1].type = INPUT_MOUSE;
	ins[1].mi.dx = ptClick.x;
	ins[1].mi.dy = ptClick.y;
	ins[1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;

	ins[2].type = INPUT_MOUSE;
	ins[2].mi.dx = ptClick.x;
	ins[2].mi.dy = ptClick.y;
	ins[2].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;


	SendInput(1, ins, sizeof(INPUT));
	Sleep(100);
	SendInput(2, ins+1, sizeof(INPUT));

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnHideMu(UINT, WPARAM, LPARAM, BOOL&)
{
	std::map<int, HWND> vInstances;
	EnumWindows(MyEnumWndProc, (LPARAM)&vInstances);

	if (vInstances.size() == 0)
		return 0;

	for (std::map<int, HWND>::iterator it = vInstances.begin(); it != vInstances.end(); ++it)
	{
		if (::IsWindow(it->second))
			::ShowWindow(it->second, SW_HIDE);
	}
	
	if (!m_fWindow)
		CMuWindow::ChangeDisplaySettingsA(0, 0);

	::SetForegroundWindow(GetDesktopWindow());
	return 0;
}


/**
 * \brief 
 */
HWND WINAPI CMuWindow::MyGetForegroundWindow(VOID)
{
	CMuWindow* pInstance = CMuWindow::GetInstance();

	if (pInstance && pInstance->m_fBlockInput)
		return pInstance->m_hWnd;

	return CMuWindow::GetForegroundWindowTr();

}


/**
 * \brief 
 */
HWND WINAPI CMuWindow::MyGetActiveWindow(VOID)
{
	CMuWindow* pInstance = CMuWindow::GetInstance();

	if (pInstance && pInstance->m_fBlockInput)
		return pInstance->m_hWnd;

	return CMuWindow::GetActiveWindowTr();
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnNCActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_fIsWndActive = (BOOL)wParam;

	if ((BOOL)wParam)
	{
		bHandled = FALSE;
	}
	else
		::DefWindowProc(m_hWnd, uMsg, wParam, lParam);

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnCaptureChanged(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	HWND hwndNewCapture = (HWND)lParam;

	// Do not block if capture is transitioning to the HUD buttons window
	// if (m_cHUDButtons.IsWindow() && hwndNewCapture == m_cHUDButtons.m_hWnd)
	// {
	// 	bHandled = FALSE;
	// 	return 0;
	// }

	if (m_pClicker == 0)
		bHandled = FALSE;
	
	return 0;
}


/**
 * \brief 
 */
void CMuWindow::SayToServer(const char* buf)
{
	// Cache the SendCommand function pointer across calls to avoid repeated module lookups.
	// Only cache on successful lookup; retry on each call if the DLL wasn't found yet
	// (handles timing where SayToServer is called before the DLL finishes loading).
	static bool (*s_pfnSendCommand)(const char*) = NULL;
	static int s_nLookupAttempts = 0;
	static const int MAX_RETRY_WITH_DELAY = 3; // Number of initial attempts with delay

	if (!s_pfnSendCommand)
	{
		// On the first few attempts, add a small delay to handle race conditions
		// where RunDllWndProc hasn't finished loading the LordOfMU DLL yet.
		if (s_nLookupAttempts > 0 && s_nLookupAttempts <= MAX_RETRY_WITH_DELAY)
		{
			Sleep(500); // Give DLL loading time to complete
		}

		HMODULE hMod = NULL;
		const char* pszMethod = NULL;

		// Attempt 0: Get HMODULE from environment variable set by LoaderDll.
		// This is the most reliable method because environment variables are per-process
		// and don't depend on window handles (which can differ if the game recreates windows).
		if (!hMod)
		{
			char szEnvBuf[32] = {0};
			if (GetEnvironmentVariableA("__LordOfMU_HMODULE__", szEnvBuf, sizeof(szEnvBuf)) > 0)
			{
				// Parse "0x<hex>" format - use _strtoui64 for correct 64-bit pointer support
				HMODULE hEnvMod = (HMODULE)(ULONG_PTR)_strtoui64(szEnvBuf, NULL, 16);
				if (hEnvMod)
				{
					// Verify the module handle is still valid by checking for the export
					if (GetProcAddress(hEnvMod, "SendCommand"))
					{
						hMod = hEnvMod;
						pszMethod = "env var";
					}
				}
			}
		}

		// Attempt 1: Get the LordOfMU DLL HMODULE directly from
		// the window property set by the LoaderDll during DLL injection.
		if (!hMod && m_hWnd)
		{
			hMod = (HMODULE)GetProp(m_hWnd, _T("__LordOfMU_Module__"));
			if (hMod)
			{
				// Verify the module handle has the expected export
				if (GetProcAddress(hMod, "SendCommand"))
					pszMethod = "GetProp(m_hWnd)";
				else
					hMod = NULL;
			}
		}

		// Attempt 1b: Try GetProp on the MU window found by EnumWindows
		// (in case m_hWnd differs from the window the LoaderDll set the property on)
		if (!hMod)
		{
			HWND hMuWnd = CMuWindowUtil::FindMuWindow();
			if (hMuWnd && hMuWnd != m_hWnd)
			{
				hMod = (HMODULE)GetProp(hMuWnd, _T("__LordOfMU_Module__"));
				if (hMod)
				{
					if (GetProcAddress(hMod, "SendCommand"))
						pszMethod = "GetProp(FindMuWindow)";
					else
						hMod = NULL;
				}
			}
		}

		// Attempt 2: Derive DLL name from our own module name (stealth naming convention:
		//            Clicker DLL has 2nd char='T', LordOfMU DLL has 2nd char='K')
		if (!hMod)
		{
			TCHAR szPath[_MAX_PATH+1] = {0};
			GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szPath, _MAX_PATH);

			int i = (int)_tcslen(szPath) - 1;
			for (i; i >= 0 && szPath[i] != _T('\\'); --i);
			i++;

			TCHAR* pszFilename = szPath + i;
			pszFilename[1] = _T('K');

			hMod = GetModuleHandle(pszFilename);
			if (hMod) pszMethod = "name transform";
		}

		// Attempt 3: Try well-known DLL names
		if (!hMod)
		{
			hMod = GetModuleHandle(_T("LordOfMU.dll"));
			if (hMod) pszMethod = "LordOfMU.dll";
		}

		if (!hMod)
		{
			hMod = GetModuleHandle(_T(__LORDOFMU_DLL_NAME));
			if (hMod) pszMethod = __LORDOFMU_DLL_NAME;
		}

		// Attempt 4: Enumerate all loaded modules and find the one exporting SendCommand.
		//            This handles cases where stealth DLL renaming makes name-based lookups fail.
		if (!hMod)
		{
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
			if (hSnap != INVALID_HANDLE_VALUE)
			{
				MODULEENTRY32 me32 = {0};
				me32.dwSize = sizeof(MODULEENTRY32);

				if (Module32First(hSnap, &me32))
				{
					do {
						if (GetProcAddress(me32.hModule, "SendCommand"))
						{
							hMod = me32.hModule;
							pszMethod = "Toolhelp32";
							break;
						}
					} while (Module32Next(hSnap, &me32));
				}
				CloseHandle(hSnap);
			}
		}

		// Attempt 5: Direct LoadLibrary fallback - load MUEliteClicker.dll from
		// the Clicker DLL's directory. This handles the case where the LoaderDll
		// mechanism fails and MUEliteClicker.dll was never loaded into the process.
		if (!hMod)
		{
			static bool s_bDirectLoadAttempted = false;
			if (!s_bDirectLoadAttempted)
			{
				s_bDirectLoadAttempted = true;

				TCHAR szDllDir[_MAX_PATH+1] = {0};
				GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szDllDir, _MAX_PATH);
				for (int j = (int)_tcslen(szDllDir) - 1; j >= 0 && szDllDir[j] != _T('\\'); szDllDir[j--] = 0);

				TCHAR szDllPath[_MAX_PATH+1] = {0};
				_tcscpy(szDllPath, szDllDir);
				_tcscat(szDllPath, _T(__LORDOFMU_DLL_NAME));

				WriteClickerLogFmt("CLICKER", "SayToServer Attempt 5: Direct LoadLibrary for '%s'",
					(LPCSTR)CT2A(szDllPath));

				struct _stat stDll = {0};
				if (0 == _tstat(szDllPath, &stDll))
				{
					HMODULE hDirect = LoadLibrary(szDllPath);
					if (hDirect)
					{
						// Initialize proxy hooks via DllGetClassObject
						typedef HRESULT (__stdcall* InitModulePtr)(REFCLSID, REFIID, LPVOID*);
						InitModulePtr pInit = (InitModulePtr)GetProcAddress(hDirect, "DllGetClassObject");
						if (pInit)
						{
							GUID guid = {0};
							pInit(guid, guid, (LPVOID*)szDllDir);
						}

						// Store HMODULE for subsequent lookups
						char szBuf[32] = {0};
						_snprintf_s(szBuf, sizeof(szBuf), _TRUNCATE, "0x%p", (void*)hDirect);
						SetEnvironmentVariableA("__LordOfMU_HMODULE__", szBuf);

						if (m_hWnd)
							SetProp(m_hWnd, _T("__LordOfMU_Module__"), (HANDLE)hDirect);

						// Try to create a command-only proxy if the game is already connected
						// (the connect hook missed the existing connection).
						typedef bool (*ForceInitPtr)();
						ForceInitPtr pForceInit = (ForceInitPtr)GetProcAddress(hDirect, "ForceInitCommandProxy");
						if (pForceInit)
						{
							pForceInit();
							WriteClickerLogFmt("CLICKER", "SayToServer: ForceInitCommandProxy called");
						}

						if (GetProcAddress(hDirect, "SendCommand"))
						{
							hMod = hDirect;
							pszMethod = "direct LoadLibrary";
						}

						WriteClickerLogFmt("CLICKER", "SayToServer: Direct load %s at 0x%p (SendCommand=%s)",
							__LORDOFMU_DLL_NAME, (void*)hDirect, hMod ? "found" : "not found");
					}
					else
					{
						DWORD dwErr = GetLastError();
						WriteClickerLogFmt("CLICKER", "SayToServer: Direct LoadLibrary FAILED for '%s' (error=%d)",
							(LPCSTR)CT2A(szDllPath), (int)dwErr);
					}
				}
				else
				{
					WriteClickerLogFmt("CLICKER", "SayToServer: Direct load skipped - '%s' not found",
						(LPCSTR)CT2A(szDllPath));
				}
			}
		}

		if (hMod)
		{
			s_pfnSendCommand = (bool (*)(const char*))GetProcAddress(hMod, "SendCommand");

			if (s_pfnSendCommand)
			{
				WriteClickerLogFmt("CLICKER", "SayToServer: DLL found at 0x%p via %s (after %d attempts)",
					hMod, pszMethod ? pszMethod : "unknown", s_nLookupAttempts + 1);
			}
			else
			{
				WriteClickerLogFmt("CLICKER", "SayToServer: DLL module found at 0x%p via %s but SendCommand export not found",
					hMod, pszMethod ? pszMethod : "unknown");
			}
		}
		else
		{
			s_nLookupAttempts++;

			// Log detailed diagnostics on first failure and every 10th attempt
			if (s_nLookupAttempts == 1 || (s_nLookupAttempts % 10) == 0)
			{
				WriteClickerLogFmt("CLICKER", "SayToServer: All lookup methods failed - m_hWnd=0x%p, PID=%d, attempt=%d",
					(void*)m_hWnd, (int)GetCurrentProcessId(), s_nLookupAttempts);

				// Log env var status
				char szEnvCheck[32] = {0};
				DWORD dwEnvLen = GetEnvironmentVariableA("__LordOfMU_HMODULE__", szEnvCheck, sizeof(szEnvCheck));
				WriteClickerLogFmt("CLICKER", "SayToServer diag: env __LordOfMU_HMODULE__='%s' (len=%d)",
					dwEnvLen > 0 ? szEnvCheck : "(not set)", (int)dwEnvLen);

				// Log our own module info for name-transform debugging
				TCHAR szOurPath[_MAX_PATH+1] = {0};
				GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szOurPath, _MAX_PATH);
				WriteClickerLogFmt("CLICKER", "SayToServer diag: our module='%s'", (LPCSTR)CT2A(szOurPath));
			}
		}
	}

	if (!s_pfnSendCommand)
	{
		WriteClickerLogFmt("CLICKER", "SayToServer FAILED: DLL module not found for cmd='%s'", buf);
		return;
	}

	if (!s_pfnSendCommand(buf))
	{
		WriteClickerLogFmt("CLICKER", "SayToServer FAILED: SendCommand returned false for cmd='%s' (game proxy not ready?)", buf);
	}
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnSetVersion(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	m_ulVersion = (ULONG)lParam;
	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnTimer(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if (wParam != 1011)
	{
		bHandled = FALSE;
	}
	else
	{
		bHandled = TRUE;

		// Sync m_fGuiActive with ImGui overlay state.
		// When overlay is not yet initialized, force m_fGuiActive to FALSE
		// so it cannot get stuck TRUE from a premature ToggleSettings call.
		if (m_cOverlay.IsInitialized())
			m_fGuiActive = m_cOverlay.IsAnyWindowVisible();
		else
			m_fGuiActive = FALSE;

		// Robust foreground-window tracking
		HWND hwndFg = GetForegroundWindowTr();
		BOOL bGameFg = (hwndFg == m_hWnd);
		if (!bGameFg && m_cLaunchMuDlg.IsWindow() && hwndFg == m_cLaunchMuDlg.m_hWnd)
			bGameFg = TRUE;

		if (bGameFg && !m_fIsWndActive)
		{
			m_fIsWndActive = TRUE;
			m_cOverlay.SetGameActive(true);
		}
		else if (!bGameFg && m_fIsWndActive)
		{
			m_fIsWndActive = FALSE;
			m_cOverlay.SetGameActive(false);
		}

		// Timer polling is the primary F-key detection path when the game
		// window IS in focus.  When the game is NOT foreground, the LL hook
		// already dispatches to OnKeyboardEvent with fCheckFgWnd=FALSE, so
		// we skip Timer dispatch here to avoid duplicate (and always-blocked)
		// processing through the fCheckFgWnd=TRUE default path.
		if (bGameFg)
		{
			for (int i=0; m_vFnKeys[i].vk != 0; ++i)
			{
				bool fOldState = m_vFnKeys[i].fPressed;
				bool fNewState = (CMuWindow::GetAsyncKeyState(m_vFnKeys[i].vk) & 0x8000) != 0;

				if (fOldState != fNewState)
				{
					// Debug: log Timer-detected F-key state changes (the primary
					// keyboard detection path when the game is in focus)
					if (m_vFnKeys[i].vk >= VK_F1 && m_vFnKeys[i].vk <= VK_F12)
					{
						BOOL fShift = (CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
						WriteClickerLogFmt("KEYDBG", "Timer: %s%s %s detected via GetAsyncKeyState polling (100ms) -> dispatching to OnKeyboardEvent",
							fShift ? "Shift+" : "",
							GetFKeyName(m_vFnKeys[i].vk),
							fNewState ? "PRESSED" : "RELEASED");
					}

					OnKeyboardEvent(m_vFnKeys[i].vk, fNewState ? WM_KEYDOWN : WM_KEYUP);
					m_vFnKeys[i].fPressed = fNewState;
				}
			}
		}
	}

	return 0;
}


/**
 * \brief Hide popup dialogs so they don't float on the desktop when the
 *        game loses focus or is minimised.  Remembers which dialogs were
 *        visible so they can be restored later via RestorePopupDialogs().
 */
LRESULT CMuWindow::OnGameWindowChanged(UINT uMsg, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	// On minimize/restore, update overlay visibility
	if (uMsg == WM_SIZE)
	{
		if (wParam == SIZE_MINIMIZED)
			m_cOverlay.SetGameActive(false);
		else if (wParam == SIZE_RESTORED)
			m_cOverlay.SetGameActive(true);
	}

	bHandled = FALSE; // Let the message pass through to the game
	return 0;
}


/**
 * \brief Shows the pickup history overlay via ImGui (Shift+F9 hotkey).
 *        Queries the LordOfMU DLL for pickup history data and toggles
 *        the history panel in the ImGui overlay.
 */
LRESULT CMuWindow::OnShowHistory(UINT, WPARAM, LPARAM, BOOL&)
{
	WriteClickerLogFmt("KEYDBG", ">>> OnShowHistory: Received WM_SHOW_HISTORY (Shift+F9) | OverlayInitialized=%s | HistoryVisible=%s",
		m_cOverlay.IsInitialized() ? "YES" : "NO",
		(m_cOverlay.IsInitialized() && m_cOverlay.IsAnyWindowVisible()) ? "YES" : "NO");

	// Guard: if the overlay hasn't been lazily initialized yet, defer the
	// request — it will be applied once the overlay initializes in
	// OnEndSceneCallback.
	if (!m_cOverlay.IsInitialized())
	{
		m_bPendingShowHistory = TRUE;
		m_bPendingShowSettings = FALSE;  // Only one panel at a time.
		WriteClickerLogFmt("KEYDBG", ">>> OnShowHistory: DEFERRED - overlay not initialized yet (will open after first EndScene)");
		return 0;
	}

	// Query and set history data + session stats
	QueryPickupHistory();
	QuerySessionStats();

	// Toggle history overlay in ImGui
	m_cOverlay.ToggleHistory();
	m_fGuiActive = m_cOverlay.IsAnyWindowVisible();

	WriteClickerLogFmt("KEYDBG", ">>> OnShowHistory: History overlay toggled | HistoryVisible=%s",
		m_cOverlay.IsInitialized() && m_cOverlay.IsAnyWindowVisible() ? "YES" : "NO");

	return 0;
}


/**
 * \brief Character selected in game - show HUD buttons in overlay.
 */
LRESULT CMuWindow::OnCharSelected(UINT, WPARAM, LPARAM, BOOL&)
{
	WriteClickerLog("OnCharSelected: Enabling HUD overlay");
	m_cOverlay.SetCharSelected(true);
	return 0;
}


/**
 * \brief Character deselected (logout/disconnect) - hide HUD and dialogs.
 */
LRESULT CMuWindow::OnCharDeselected(UINT, WPARAM, LPARAM, BOOL&)
{
	WriteClickerLog("OnCharDeselected: Disabling HUD overlay and hiding dialogs");

	m_cOverlay.SetCharSelected(false);
	m_cOverlay.HideSettings();
	m_cOverlay.HideHistory();
	m_fGuiActive = FALSE;

	return 0;
}


// ============================================================================
// D3D9 EndScene/Reset callbacks  (static, called from D3D9Hook)
// ============================================================================

void CMuWindow::OnEndSceneCallback(IDirect3DDevice9* pDevice)
{
	CMuWindow* pThis = s_pInstance;
	if (!pThis)
		return;

	// Lazy-initialize ImGui on the first EndScene call (the device is now valid).
	if (!pThis->m_cOverlay.IsInitialized())
	{
		if (!pThis->m_cOverlay.Initialize(pThis->m_hWnd, pDevice))
			return;

		// Process any F9/Shift+F9 requests that arrived before initialization.
		if (pThis->m_bPendingShowSettings)
		{
			pThis->m_bPendingShowSettings = FALSE;
			pThis->PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);
		}
		else if (pThis->m_bPendingShowHistory)
		{
			pThis->m_bPendingShowHistory = FALSE;
			pThis->PostMessage(WM_SHOW_HISTORY, 0, 0);
		}
	}

	pThis->m_cOverlay.Render(pDevice);
}

void CMuWindow::OnPreResetCallback(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS*)
{
	CMuWindow* pThis = s_pInstance;
	if (pThis)
		pThis->m_cOverlay.OnPreReset();
}

void CMuWindow::OnPostResetCallback(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS*, HRESULT hr)
{
	CMuWindow* pThis = s_pInstance;
	if (pThis)
		pThis->m_cOverlay.OnPostReset(hr);
}


// ============================================================================
// ImGui overlay button callbacks  (static, called from CImGuiOverlay)
// ============================================================================

void CMuWindow::OnOverlaySettingsClicked(void* pData)
{
	CMuWindow* pThis = (CMuWindow*)pData;
	if (pThis)
		pThis->PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);
}

void CMuWindow::OnOverlayStartStopClicked(void* pData)
{
	CMuWindow* pThis = (CMuWindow*)pData;
	if (!pThis)
		return;

	if (pThis->m_pClicker != NULL)
		pThis->PostMessage(WM_STOP_CLICKER, 0, 0);
	else
		pThis->PostMessage(WM_START_CLICKER, 0, 0);
}

void CMuWindow::OnOverlayHistoryClicked(void* pData)
{
	CMuWindow* pThis = (CMuWindow*)pData;
	if (pThis)
		pThis->PostMessage(WM_SHOW_HISTORY, 0, 0);
}

void CMuWindow::OnOverlayApplyClicked(void* pData)
{
	CMuWindow* pThis = (CMuWindow*)pData;
	if (!pThis)
		return;

	// Save settings to disk
	pThis->m_cSettingsDlg.GetSettingsObj().Save();
	WriteClickerLog("Settings applied and saved via ImGui overlay");
}


// ============================================================================
// History data query helper
// ============================================================================

void CMuWindow::QueryPickupHistory()
{
	typedef int (*GetPickupHistoryPtr)(char*, int);
	static GetPickupHistoryPtr s_pfnGetHistory = NULL;
	static bool s_bLookupDone = false;

	if (!s_bLookupDone)
	{
		HMODULE hMod = NULL;

		// Try environment variable first
		char szEnvBuf[32] = {0};
		if (GetEnvironmentVariableA("__LordOfMU_HMODULE__", szEnvBuf, sizeof(szEnvBuf)) > 0)
		{
			hMod = (HMODULE)(ULONG_PTR)_strtoui64(szEnvBuf, NULL, 16);
			if (hMod && !GetProcAddress(hMod, "GetPickupHistory"))
				hMod = NULL;
		}

		// Try window property
		if (!hMod && m_hWnd)
		{
			hMod = (HMODULE)GetProp(m_hWnd, _T("__LordOfMU_Module__"));
			if (hMod && !GetProcAddress(hMod, "GetPickupHistory"))
				hMod = NULL;
		}

		// Try well-known names
		if (!hMod)
		{
			hMod = GetModuleHandle(_T("LordOfMU.dll"));
			if (hMod && !GetProcAddress(hMod, "GetPickupHistory"))
				hMod = NULL;
		}

		if (hMod)
		{
			s_pfnGetHistory = (GetPickupHistoryPtr)GetProcAddress(hMod, "GetPickupHistory");
			WriteClickerLogFmt("CLICKER", "GetPickupHistory export found at 0x%p", s_pfnGetHistory);
		}

		s_bLookupDone = true;
	}

	std::vector<CImGuiOverlay::HistoryEntry> vHistory;

	if (s_pfnGetHistory)
	{
		char szBuffer[32768] = {0};
		s_pfnGetHistory(szBuffer, sizeof(szBuffer));

		// Parse "HH:MM:SS|ItemName\n" entries
		char* pLine = szBuffer;
		char* pBufEnd = szBuffer + sizeof(szBuffer);
		while (pLine && pLine < pBufEnd && *pLine)
		{
			char* pEnd = strchr(pLine, '\n');
			if (pEnd) *pEnd = '\0';

			char* pSep = strchr(pLine, '|');
			if (pSep)
			{
				*pSep = '\0';
				CImGuiOverlay::HistoryEntry entry;
				entry.sTime = pLine;
				entry.sItem = pSep + 1;
				vHistory.push_back(entry);
			}

			if (!pEnd) break;
			pLine = pEnd + 1;
		}
	}

	m_cOverlay.SetHistory(vHistory);
}


// ============================================================================
// Session statistics helpers
// ============================================================================

/**
 * \brief Queries session statistics from LordOfMU.dll and passes them
 *        to the ImGui overlay for the Kill Count tab.
 */
void CMuWindow::QuerySessionStats()
{
	typedef int (*GetSessionStatsPtr)(char*, int);
	static GetSessionStatsPtr s_pfnGetStats = NULL;
	static bool s_bLookupDone = false;

	if (!s_bLookupDone)
	{
		HMODULE hMod = NULL;

		char szEnvBuf[32] = {0};
		if (GetEnvironmentVariableA("__LordOfMU_HMODULE__", szEnvBuf, sizeof(szEnvBuf)) > 0)
		{
			hMod = (HMODULE)(ULONG_PTR)_strtoui64(szEnvBuf, NULL, 16);
			if (hMod && !GetProcAddress(hMod, "GetSessionStats"))
				hMod = NULL;
		}

		if (!hMod && m_hWnd)
		{
			hMod = (HMODULE)GetProp(m_hWnd, _T("__LordOfMU_Module__"));
			if (hMod && !GetProcAddress(hMod, "GetSessionStats"))
				hMod = NULL;
		}

		if (!hMod)
		{
			hMod = GetModuleHandle(_T("LordOfMU.dll"));
			if (hMod && !GetProcAddress(hMod, "GetSessionStats"))
				hMod = NULL;
		}

		if (hMod)
			s_pfnGetStats = (GetSessionStatsPtr)GetProcAddress(hMod, "GetSessionStats");

		s_bLookupDone = true;
	}

	CImGuiOverlay::SessionStats stats;
	memset(&stats, 0, sizeof(stats));

	if (s_pfnGetStats)
	{
		char szBuffer[512] = {0};
		s_pfnGetStats(szBuffer, sizeof(szBuffer));

		// Parse "key=value\n" lines
		char* pLine = szBuffer;
		while (pLine && *pLine)
		{
			char* pEnd = strchr(pLine, '\n');
			if (pEnd) *pEnd = '\0';

			char* pEq = strchr(pLine, '=');
			if (pEq)
			{
				*pEq = '\0';
				const char* pKey = pLine;
				const char* pVal = pEq + 1;

				if (_stricmp(pKey, "kills") == 0)
					stats.nKillCount = atoi(pVal);
				else if (_stricmp(pKey, "items") == 0)
					stats.nItemCount = atoi(pVal);
				else if (_stricmp(pKey, "zen") == 0)
					stats.ullZenTotal = _strtoui64(pVal, NULL, 10);
				else if (_stricmp(pKey, "exp") == 0)
					stats.ullExpGained = _strtoui64(pVal, NULL, 10);
				else if (_stricmp(pKey, "runtime") == 0)
					stats.ulRuntimeSeconds = strtoul(pVal, NULL, 10);
			}

			if (!pEnd) break;
			pLine = pEnd + 1;
		}
	}

	m_cOverlay.SetSessionStats(stats);
}


/**
 * \brief Calls ResetSessionStats in LordOfMU.dll to start a new session.
 */
void CMuWindow::ResetSessionStatsCall()
{
	typedef void (*ResetSessionStatsPtr)();
	static ResetSessionStatsPtr s_pfn = NULL;
	static bool s_bDone = false;

	if (!s_bDone)
	{
		HMODULE hMod = NULL;

		char szEnvBuf[32] = {0};
		if (GetEnvironmentVariableA("__LordOfMU_HMODULE__", szEnvBuf, sizeof(szEnvBuf)) > 0)
		{
			hMod = (HMODULE)(ULONG_PTR)_strtoui64(szEnvBuf, NULL, 16);
			if (hMod && !GetProcAddress(hMod, "ResetSessionStats"))
				hMod = NULL;
		}

		if (!hMod && m_hWnd)
		{
			hMod = (HMODULE)GetProp(m_hWnd, _T("__LordOfMU_Module__"));
			if (hMod && !GetProcAddress(hMod, "ResetSessionStats"))
				hMod = NULL;
		}

		if (!hMod)
		{
			hMod = GetModuleHandle(_T("LordOfMU.dll"));
			if (hMod && !GetProcAddress(hMod, "ResetSessionStats"))
				hMod = NULL;
		}

		if (hMod)
			s_pfn = (ResetSessionStatsPtr)GetProcAddress(hMod, "ResetSessionStats");

		s_bDone = true;
	}

	if (s_pfn)
		s_pfn();
}


/**
 * \brief Calls StopSession in LordOfMU.dll to freeze runtime counter.
 */
void CMuWindow::StopSessionCall()
{
	typedef void (*StopSessionPtr)();
	static StopSessionPtr s_pfn = NULL;
	static bool s_bDone = false;

	if (!s_bDone)
	{
		HMODULE hMod = NULL;

		char szEnvBuf[32] = {0};
		if (GetEnvironmentVariableA("__LordOfMU_HMODULE__", szEnvBuf, sizeof(szEnvBuf)) > 0)
		{
			hMod = (HMODULE)(ULONG_PTR)_strtoui64(szEnvBuf, NULL, 16);
			if (hMod && !GetProcAddress(hMod, "StopSession"))
				hMod = NULL;
		}

		if (!hMod && m_hWnd)
		{
			hMod = (HMODULE)GetProp(m_hWnd, _T("__LordOfMU_Module__"));
			if (hMod && !GetProcAddress(hMod, "StopSession"))
				hMod = NULL;
		}

		if (!hMod)
		{
			hMod = GetModuleHandle(_T("LordOfMU.dll"));
			if (hMod && !GetProcAddress(hMod, "StopSession"))
				hMod = NULL;
		}

		if (hMod)
			s_pfn = (StopSessionPtr)GetProcAddress(hMod, "StopSession");

		s_bDone = true;
	}

	if (s_pfn)
		s_pfn();
}
