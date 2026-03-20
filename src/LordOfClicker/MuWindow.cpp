#include "stdafx.h"
#include "MuWindow.h"
#include "AutoClicker.h"
#include "ClickerJobFactory.h"
#include "AsyncKeyQueue.h"
#include "MuWindowUtil.h"
#include "version.h"
#include "ClickerLogger.h"
#include <map>

CMuWindow* CMuWindow::s_pInstance = NULL;

CGetAsyncKeyStateTramp CMuWindow::GetAsyncKeyState;
CChangeDisplaySettingsATramp CMuWindow::ChangeDisplaySettingsA;
CGetForegroundWindowTramp CMuWindow::GetForegroundWindowTr;
CGetActiveWindowTramp CMuWindow::GetActiveWindowTr;


/**
 * \brief 
 */
CMuWindow::CMuWindow()
	: m_cAdvSettingsDlg(m_cSettingsDlg.GetSettingsObj())
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

#ifdef __CLICKER_AVANTA__
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
	m_cAdvSettingsDlg.Create(m_hWnd);

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
		m_cAdvSettingsDlg.ShowWindow(SW_HIDE);
		return TRUE;
	}

	return FALSE;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnMouseMessage(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = m_fBlockInput;
	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnActivateApp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_fIsWndActive = (BOOL)wParam;

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

	if (::IsWindow(m_cAdvSettingsDlg.m_hWnd))
		m_cAdvSettingsDlg.DestroyWindow();

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
	if (m_pClicker != NULL)
		return 0;

	CWindow dlg = m_cSettingsDlg;
	
	if ((CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
	{
		dlg = m_cAdvSettingsDlg;

#ifdef __CLICKER_AVANTA__
		Beep(1000, 70);
		return 0;
#endif
	}

	if (m_fGuiActive || !dlg.IsWindow())
		return 0;

	m_fGuiActive = TRUE;

	dlg.ShowWindow(SW_SHOWNORMAL);

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
				dlg.ShowWindow(SW_HIDE);
			}
			else if (!dlg.IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dlg.ShowWindow(SW_HIDE);
		}

		if (!dlg.IsWindowVisible())
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

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuWindow::OnErasebkgnd(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if (m_fGuiActive)
	{
		HDC hDC = (HDC)wParam;

		RECT rc = {0};
		GetClientRect(&rc);

		SetBkColor(hDC, RGB(0,0,0));
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, 0, 0, 0);
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


#if defined(__CLICKER_AVANTA__)
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
LRESULT CMuWindow::OnCaptureChanged(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	if (m_pClicker == 0)
		bHandled = FALSE;
	
	return 0;
}


/**
 * \brief 
 */
void CMuWindow::SayToServer(const char* buf)
{
	TCHAR szPath[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szPath, _MAX_PATH);

	int i = (int)_tcslen(szPath) - 1;
	for (i; i >= 0 && szPath[i] != _T('\\'); --i);
	i++;

	TCHAR* pszFilename = szPath + i;
	pszFilename[1] = _T('K');

	HMODULE hMod = GetModuleHandle(pszFilename);

	if (!hMod)
		hMod = GetModuleHandle(_T("LordOfMU.dll"));

	if (!hMod)
		hMod = GetModuleHandle(_T(__LORDOFMU_DLL_NAME));

	if (!hMod)
		return;

	bool (*SendCommandPtr)(const char*) = (bool (*)(const char*))GetProcAddress(hMod, "SendCommand");

	if (SendCommandPtr)
		SendCommandPtr(buf);
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
