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
	: m_cUnifiedSettingsDlg(m_cSettingsDlg.GetSettingsObj())
{
	m_hEvent = CreateEvent(0, 0, 0, _T("__LordOfMU_Event01__"));
	SetEvent(m_hEvent);

	m_hKbdHook = NULL;
	m_fWasLastActiveInstance = FALSE;

	m_fIsWndActive = TRUE;
	m_fBlockInput = FALSE;

	m_fGuiActive = FALSE;
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

	if (s_pInstance != NULL)
	{
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

	m_cSettingsDlg.Create(m_hWnd);
	m_cLaunchMuDlg.Create(m_hWnd);
	// REDESIGN: Create unified dialog instead of separate advanced dialog
	// m_cAdvSettingsDlg.Create(m_hWnd);
	m_cUnifiedSettingsDlg.Create(m_hWnd);
	m_cHistoryDlg.Create(m_hWnd);

	// Create HUD overlay buttons (Settings, Start/Stop, History)
	m_cHUDButtons.Create(m_hWnd, _AtlBaseModule.GetModuleInstance());

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

	if (CMuWindow::GetForegroundWindowTr() != pThis->m_hWnd)
	{
		if (pThis->OnKeyboardEvent(kbd->vkCode, (UINT)wParam))
			return 0;
	}

	return CallNextHookEx(pThis->m_hKbdHook, code, wParam, lParam);
}


/**
 * \brief 
 */
BOOL CMuWindow::OnKeyboardEvent(UINT vkCode, UINT uMsg, BOOL fCheckFgWnd)
{
	if (vkCode == VK_F12 && uMsg == WM_KEYDOWN)
	{
		// Show/Hide all MU Windows
		if (CMuWindow::GetForegroundWindowTr() == m_hWnd)
		{
			m_fWasLastActiveInstance = TRUE;
			PostMessage(WM_HIDE_MU, 0, 0);
		}
		else if (m_fWasLastActiveInstance)
		{
			m_fWasLastActiveInstance = FALSE;
			PostMessage(WM_SHOW_MU, 0, 0);
		}
		else
		{
			m_fWasLastActiveInstance = FALSE;
			return FALSE;
		}

		return TRUE;
	}

	if (fCheckFgWnd && CMuWindow::GetForegroundWindowTr() != m_hWnd)
		return FALSE;

	// While a dialog (Settings/History) is open, block clicker control keys
	// to prevent accidental autoclicker toggling behind the dialog.
	if (m_fGuiActive)
	{
		if (vkCode >= VK_F5 && vkCode <= VK_F8)
			return TRUE;  // Swallow the keypress
	}

#if defined(__HACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
	if (vkCode == VK_F5 && uMsg == WM_KEYUP)
	{
		if ((CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
		{
			SayToServer("//script toggle");
		}
		else
		{
			SayToServer("//autokill toggle");
		}

		return TRUE;
	}
#else
	if (vkCode == VK_F5 && uMsg == WM_KEYUP)
	{
		if (m_pClicker != NULL)
		{
			PostMessage(WM_STOP_CLICKER, 0, 0);
		}
		else
		{
			PostMessage(WM_START_CLICKER, 0, 0);
		}

		return TRUE;
	}
#endif

	if (vkCode == VK_F6 && uMsg == WM_KEYUP)
	{
		PostMessage(WM_START_CLICKER, 0, 0);
		return TRUE;
	}

	if (vkCode == VK_F7 && uMsg == WM_KEYUP)
	{
		PostMessage(WM_START_CLICKER, 0, TRUE);
		return TRUE;
	}

	if (vkCode == VK_F8 && uMsg == WM_KEYUP)
	{
		PostMessage(WM_STOP_CLICKER, 0, 0);
		return TRUE;
	}

	if (vkCode == VK_F9 && uMsg == WM_KEYUP)
	{
		PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);
		return TRUE;
	}

	if (vkCode == VK_F10 && uMsg == WM_KEYUP)
	{
		if ((CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
		{
			SayToServer("//moveitem 12 76");
		}
		else
		{
			// Launch another MU instance
			PostMessage(WM_LAUNCH_MU, 0, 0);
		}

		return TRUE;
	}

	if (vkCode == VK_F11 && uMsg == WM_KEYUP)
	{
		// Switch between MU windows
		PostMessage(WM_SWITCH_INSTANCE, 0, 0);
		return TRUE;
	}

	if (vkCode == VK_ESCAPE && uMsg == WM_KEYUP && m_fGuiActive)
	{
		m_cSettingsDlg.ShowWindow(SW_HIDE);
		m_cUnifiedSettingsDlg.ShowWindow(SW_HIDE);
		m_cHistoryDlg.ShowWindow(SW_HIDE);
		return TRUE;
	}

	return FALSE;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnMouseMessage(UINT uMsg, WPARAM, LPARAM, BOOL& bHandled)
{
	// Always allow left mouse button messages through so the user can click
	// on HUD buttons and interact with the game even while the autoclicker
	// is running.  Other mouse messages (movement, right-click, wheel, etc.)
	// remain blocked when m_fBlockInput is TRUE to prevent interference
	// with the autoclicker's programmatic mouse control.
	if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDBLCLK)
	{
		bHandled = FALSE;
	}
	else
	{
		bHandled = m_fBlockInput;
	}

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnActivateApp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_fIsWndActive = (BOOL)wParam;

	// Immediately show/hide HUD buttons when the game application
	// gains or loses foreground.  Buttons must only be visible
	// and clickable inside the game client (main.exe).
	m_cHUDButtons.SetGameActive(m_fIsWndActive);

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

	if (::IsWindow(m_cUnifiedSettingsDlg.m_hWnd))
		m_cUnifiedSettingsDlg.DestroyWindow();

	if (::IsWindow(m_cHistoryDlg.m_hWnd))
		m_cHistoryDlg.DestroyWindow();

	m_cHUDButtons.Destroy();

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
 * \brief 
 */
LRESULT CMuWindow::OnShowSettingsGUI(UINT, WPARAM, LPARAM, BOOL&)
{
	// If autoclicker is running, stop it first and notify the user
	if (m_pClicker != NULL)
	{
		PostMessage(WM_STOP_CLICKER, 0, 0);
		MessageBeep(MB_ICONINFORMATION);
		return 0;
	}

	// REDESIGN: F9 and SHIFT+F9 merged into unified dialog
	CWindow dlg = m_cUnifiedSettingsDlg;

	if (m_fGuiActive || !dlg.IsWindow())
		return 0;

	m_fGuiActive = TRUE;

	// Show as non-blocking popup overlay - game continues rendering behind it
	dlg.ShowWindow(SW_SHOWNOACTIVATE);

	// Ensure the dialog is topmost and visible
	::SetWindowPos(dlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnErasebkgnd(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if (m_fGuiActive)
	{
		// Do not fill the background -- keep it transparent so the
		// game scene remains visible behind the settings dialog.
	}
	else
	{
		bHandled = FALSE;
	}

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
			m_cHUDButtons.SetClickerRunning(TRUE);
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

	m_cHUDButtons.SetClickerRunning(FALSE);
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

	if (m_fGuiActive || !m_cSettingsDlg.IsWindow())
		return 0;

	m_fGuiActive = TRUE;

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
		bHandled = FALSE;
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
	if (m_cHUDButtons.IsWindow() && hwndNewCapture == m_cHUDButtons.m_hWnd)
	{
		bHandled = FALSE;
		return 0;
	}

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

		// Monitor dialog visibility - reset m_fGuiActive when dialogs are closed
		if (m_fGuiActive)
		{
			BOOL bSettingsVisible = m_cUnifiedSettingsDlg.IsWindow() && m_cUnifiedSettingsDlg.IsWindowVisible();
			BOOL bHistoryVisible = m_cHistoryDlg.IsWindow() && m_cHistoryDlg.IsWindowVisible();

			if (!bSettingsVisible && !bHistoryVisible)
			{
				m_fGuiActive = FALSE;
			}
		}

		for (int i=0; m_vFnKeys[i].vk != 0; ++i)
		{
			bool fOldState = m_vFnKeys[i].fPressed;
			bool fNewState = (CMuWindow::GetAsyncKeyState(m_vFnKeys[i].vk) & 0x8000) != 0;

			if (fOldState != fNewState)
			{
				OnKeyboardEvent(m_vFnKeys[i].vk, fNewState ? WM_KEYDOWN : WM_KEYUP);
				m_vFnKeys[i].fPressed = fNewState;
			}
		}
	}

	return 0;
}


/**
 * \brief Game window moved or resized - immediately reposition the HUD overlay.
 *        Keeps buttons anchored without waiting for the 200ms reposition timer.
 */
LRESULT CMuWindow::OnGameWindowChanged(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	m_cHUDButtons.Reposition();
	bHandled = FALSE; // Let the message pass through to the game
	return 0;
}


/**
 * \brief HUD Settings button clicked - open the F9 settings dialog.
 */
LRESULT CMuWindow::OnHUDSettings(UINT, WPARAM, LPARAM, BOOL&)
{
	PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);
	return 0;
}


/**
 * \brief HUD Start/Stop button clicked - toggle autoclicker.
 */
LRESULT CMuWindow::OnHUDStartStop(UINT, WPARAM, LPARAM, BOOL&)
{
	WriteClickerLogFmt("HUD", "StartStop clicked, m_pClicker=%p", m_pClicker);

	if (m_pClicker != NULL)
	{
		PostMessage(WM_STOP_CLICKER, 0, 0);
	}
	else
	{
		PostMessage(WM_START_CLICKER, 0, 0);
	}

	return 0;
}


/**
 * \brief HUD History button clicked - open the pickup history dialog.
 *        Queries the LordOfMU DLL for pickup history data and displays it.
 */
LRESULT CMuWindow::OnHUDHistory(UINT, WPARAM, LPARAM, BOOL&)
{
	if (m_fGuiActive)
		return 0;

	// Query pickup history from LordOfMU DLL
	std::vector<CHistoryDialog::HistoryEntry> vHistory;

	typedef int (*GetPickupHistoryPtr)(char*, int);
	static GetPickupHistoryPtr s_pfnGetHistory = NULL;
	static bool s_bLookupDone = false;

	if (!s_bLookupDone)
	{
		// Find the DLL module using the same approach as SayToServer
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

	if (s_pfnGetHistory)
	{
		char szBuffer[32768] = {0};
		int nCount = s_pfnGetHistory(szBuffer, sizeof(szBuffer));

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
				CHistoryDialog::HistoryEntry entry;
				entry.sTime = pLine;
				entry.sItem = pSep + 1;
				vHistory.push_back(entry);
			}

			if (!pEnd) break;
			pLine = pEnd + 1;
		}
	}

	m_cHistoryDlg.SetHistory(vHistory);

	// Show history dialog as non-blocking popup overlay
	if (!m_cHistoryDlg.IsWindow())
		return 0;

	m_fGuiActive = TRUE;

	m_cHistoryDlg.ShowWindow(SW_SHOWNOACTIVATE);

	// Ensure the dialog is topmost and visible
	::SetWindowPos(m_cHistoryDlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

	return 0;
}


/**
 * \brief Character selected in game - show HUD buttons.
 *        Posted by CharInfoFilter when it detects the CCharSelectedPacket.
 */
LRESULT CMuWindow::OnCharSelected(UINT, WPARAM, LPARAM, BOOL&)
{
	WriteClickerLog("OnCharSelected: Showing HUD buttons");
	m_cHUDButtons.Show();
	return 0;
}
