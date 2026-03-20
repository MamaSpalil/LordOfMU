#include "stdafx.h"
#include "MuInstanceManager.h"
#include "version.h"

#include <string>
using namespace std;


UINT_PTR g_uTimer = 0;


/**
 * \brief 
 */
static VOID CALLBACK DelayedTerm(HWND, UINT, UINT_PTR, DWORD)
{
	KillTimer(0, g_uTimer);

	TCHAR szMyName[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szMyName, _MAX_PATH);

	TCHAR* p = szMyName;
	for (int i=(int)_tcslen(szMyName)-1; i >= 0 && szMyName[i] != _T('\\'); p = &szMyName[i--]);

	FreeLibrary(GetModuleHandle(p));
}



/**
 * \brief 
 */
CMuInstanceManager::CMuInstanceManager()
{
	m_fProcessing = false;
	m_fServer = false;
	m_hHookDll = 0;
	m_fParasite = true;

	memset(m_szLoaderPath, 0, (_MAX_PATH+1)*sizeof(TCHAR));
}


/**
 * \brief 
 */
CMuInstanceManager::~CMuInstanceManager()
{
}


/**
 * \brief 
 */
BOOL CMuInstanceManager::Init(bool fRunAsParasite) 
{ 
	m_fParasite = fRunAsParasite;
	return Create(NULL) != NULL;
}


/**
 * \brief 
 */
void CMuInstanceManager::Term()
{
	if (IsWindow())
		DestroyWindow();
}


/**
 * \brief 
 */
LRESULT CMuInstanceManager::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA), m_hWnd, 101, 
			NIF_ICON | NIF_MESSAGE | NIF_TIP, SHELL_ICON_MESSAGE, LoadIcon(_AtlBaseModule.GetResourceInstance(), 
			MAKEINTRESOURCE(IDI_ICON1)),
			_T(__SOFTWARE_PRODUCT_NAME) };

	Shell_NotifyIcon(NIM_ADD, &nid);

	TCHAR szPath[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szPath, _MAX_PATH);

	for (int i = (int)_tcslen(szPath) - 1; i >= 0 && szPath[i] != _T('\\'); szPath[i--] = 0);


	memset(m_szLoaderPath, 0, (_MAX_PATH+1)*sizeof(TCHAR));
	PrepareDll(m_szLoaderPath);

	m_hHookDll = LoadLibrary(m_szLoaderPath);

	ATLASSERT(m_hHookDll);

	if (m_hHookDll)
	{
		typedef HRESULT (__stdcall* InstallHookPtr)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
		InstallHookPtr InstallProc = (InstallHookPtr)GetProcAddress(m_hHookDll, "DllGetClassObject");

		GUID guid = {0};

		if (InstallProc)
			InstallProc(guid, guid, (LPVOID*)szPath);
	}

	// Launch the game client (main.exe) after hooks are installed
	{
		TCHAR szMainExe[_MAX_PATH+1] = {0};
		_tcscpy(szMainExe, szPath);
		_tcscat(szMainExe, _T("main.exe"));

		struct _stat stMain = {0};
		if (0 == _tstat(szMainExe, &stMain))
		{
			HINSTANCE hResult = ShellExecute(NULL, _T("open"), szMainExe, NULL, szPath, SW_SHOWNORMAL);

			if ((INT_PTR)hResult <= 32)
			{
				TCHAR szErr[512] = {0};
				_sntprintf(szErr, 511, _T("Failed to launch main.exe (error %d).\nPath: %s"), (int)(INT_PTR)hResult, szMainExe);
				MessageBox(NULL, szErr, _T("LordOfMU - Error"), MB_OK | MB_ICONERROR);
			}
		}
		else
		{
			TCHAR szErr[512] = {0};
			_sntprintf(szErr, 511, _T("main.exe not found.\nExpected path: %s"), szMainExe);
			MessageBox(NULL, szErr, _T("LordOfMU - Error"), MB_OK | MB_ICONERROR);
		}
	}

	_tcscat(szPath, _T("LordOfServer.dll"));

	struct _stat st = {0};
	m_fServer = _tstat(szPath, &st) == 0;

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuInstanceManager::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	NOTIFYICONDATA nid = {sizeof(NOTIFYICONDATA), m_hWnd, 101, 0};
	Shell_NotifyIcon(NIM_DELETE, &nid);	

	if (m_hHookDll)
	{
		typedef HRESULT (__stdcall* RemoveHookPtr)();
		RemoveHookPtr RemoveProc = (RemoveHookPtr)GetProcAddress(m_hHookDll, "DllUnregisterServer");

		if (RemoveProc)
			RemoveProc();

		FreeLibrary(m_hHookDll);
		m_hHookDll = 0;

		DeleteFile(m_szLoaderPath);

		TCHAR* pszFilename = PathFindFileName(m_szLoaderPath);

		if (pszFilename)
		{
			pszFilename[1] = _T('K');
			DeleteFile(m_szLoaderPath);

			pszFilename[1] = _T('T');
			DeleteFile(m_szLoaderPath);
		}
	}

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuInstanceManager::OnShellIcon(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (HIWORD(lParam) == 0 
			&& (LOWORD(lParam) == WM_RBUTTONUP || LOWORD(lParam) == WM_LBUTTONUP))
	{
		SetForegroundWindow(m_hWnd);


		if (m_fProcessing)
			return 0;

		m_fProcessing = true;

		POINT pt = {0};
		GetCursorPos(&pt);


		HMENU hPopup = CreatePopupMenu();
#if defined(__CLICKER_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
		AppendMenu(hPopup, MF_STRING, 101, _T("Help"));
#endif
		AppendMenu(hPopup, MF_STRING, 102, _T("About"));
		AppendMenu(hPopup, MF_SEPARATOR, 0, 0);

#if defined(__CLICKER_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
		HMENU hPopupST = CreatePopupMenu();

		CRegKey key;
		key.Create(HKEY_CURRENT_USER, _T("Software"));
		key.Create(key.m_hKey, _T("LordJerec"));
		key.Create(key.m_hKey, _T("MUAutoClicker"));
		key.Create(key.m_hKey, _T("Config"));

		DWORD dwType = 0;
		key.QueryDWORDValue(_T("ClientType"), dwType); // 0 - N/S, 1 - S3, 2 - S4

		AppendMenu(hPopup, MF_POPUP, (UINT_PTR)hPopupST, _T("Client Type"));

		AppendMenu(hPopupST, MF_STRING | ((dwType == 0 || dwType > 2) ? MF_CHECKED : 0), 200, _T("Not specified"));
		AppendMenu(hPopupST, MF_STRING | ((dwType == 1) ? MF_CHECKED : 0), 201, _T("Season 3"));
		AppendMenu(hPopupST, MF_STRING | ((dwType == 2) ? MF_CHECKED : 0), 202, _T("Season 4"));

		hPopupST = CreatePopupMenu();

		CRegKey key2;
		key2.Create(HKEY_CURRENT_USER, _T("Software"));
		key2.Create(key2.m_hKey, _T("LordJerec"));
		key2.Create(key2.m_hKey, _T("MUAutoClicker"));
		key2.Create(key2.m_hKey, _T("Config"));

		dwType = 0x00000001;
		key2.QueryDWORDValue(_T("ProtocolType"), dwType);

/*
	#define PTYPE_KOR 0x00000001
	#define PTYPE_CHS 0x00000002
	#define PTYPE_VTM 0x00000003
	#define PTYPE_JPN 0x00000004
	#define PTYPE_PHI 0x00000005
*/
	#if defined(__CLICKER_ELITE__) || defined(__INCLUDE_ALL_STUFF__)
		AppendMenu(hPopup, MF_POPUP, (UINT_PTR)hPopupST, _T("Protocol Type"));

		AppendMenu(hPopupST, MF_STRING | ((dwType == 1) ? MF_CHECKED : 0), 301, _T("Korea"));
//		AppendMenu(hPopupST, MF_STRING | ((dwType == 2) ? MF_CHECKED : 0), 302, _T("China"));
//		AppendMenu(hPopupST, MF_STRING | ((dwType == 3) ? MF_CHECKED : 0), 303, _T("Vietnam"));
		AppendMenu(hPopupST, MF_STRING | ((dwType == 4) ? MF_CHECKED : 0), 304, _T("Japan"));
		AppendMenu(hPopupST, MF_STRING | ((dwType == 5) ? MF_CHECKED : 0), 305, _T("Philipines"));
	#endif

		AppendMenu(hPopup, MF_SEPARATOR, 0, 0);
#endif

#if defined(__ANTIHACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)

		if (m_fServer)
		{
			AppendMenu(hPopup, MF_STRING, 104, _T("MD5 Calculator"));
			AppendMenu(hPopup, MF_SEPARATOR, 0, 0);
		}
#endif //defined(__ANTIHACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)

		AppendMenu(hPopup, MF_STRING, 103, _T("Exit"));

		int nCmd = (int)TrackPopupMenu(hPopup, TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, 0);
		
		if (nCmd == 102)
		{
			CAboutBox dlgAbout;
			dlgAbout.DoModal(m_hWnd);
		}
		else if (nCmd == 101)
		{
			TCHAR szFileName[_MAX_PATH+1] = {0};
			GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szFileName, _MAX_PATH);

			for (int i=(int)_tcslen(szFileName)-1; i >= 0 && szFileName[i] != _T('\\'); --i)
			{
				szFileName[i] = 0;
			}

			_tcscat(szFileName, _T("help\\index.html"));

			ShellExecute(0, _T("open"), szFileName, _T(""), 0, SW_SHOWNORMAL);
		}
		else if (nCmd == 103)
		{
			if (m_fParasite)
				g_uTimer = ::SetTimer(0, 101, 500, DelayedTerm);
			else
				PostQuitMessage(0);
		}
		else if (nCmd == 104)
		{
			CMD5CalcDlg dlg;
			dlg.DoModal(m_hWnd);
		}
		else if (nCmd == 200 || nCmd == 201 || nCmd == 202)
		{
			CRegKey key;
			key.Create(HKEY_CURRENT_USER, _T("Software"));
			key.Create(key.m_hKey, _T("LordJerec"));
			key.Create(key.m_hKey, _T("MUAutoClicker"));
			key.Create(key.m_hKey, _T("Config"));

			key.SetDWORDValue(_T("ClientType"), (DWORD)(nCmd - 200)); // 0 - N/S, 1 - S3, 2 - S4
		}
		else if (nCmd >= 301 && nCmd <= 305)
		{
			CRegKey key;
			key.Create(HKEY_CURRENT_USER, _T("Software"));
			key.Create(key.m_hKey, _T("LordJerec"));
			key.Create(key.m_hKey, _T("MUAutoClicker"));
			key.Create(key.m_hKey, _T("Config"));

			key.SetDWORDValue(_T("ProtocolType"), (DWORD)(nCmd - 300));
		}

#if defined(__CLICKER_STUFF__)
		DestroyMenu(hPopupST);
#endif
		DestroyMenu(hPopup);
		m_fProcessing = false;
	}

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuInstanceManager::OnMuWindowControl(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam)
	{
	case 1: // register MU window
		break;
	case 2: // next MU window
		break;
	case 3: // hide all MU windows
		break;
	}

	return 0;
}


/**  
 * \brief 
 */
void CMuInstanceManager::PrepareDll(TCHAR* pszPath)
{
	if (!pszPath)
		return;

	TCHAR szPath[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szPath, _MAX_PATH);

	for (int i=(int)_tcslen(szPath)-1; i >= 0 && szPath[i] != _T('\\'); szPath[i--] = 0);
	_tcscat(szPath, CA2CT(__LOADER_DLL_NAME));

	TCHAR szDestPath[_MAX_PATH+1] = {0};
	GetTempPath(_MAX_PATH, szDestPath);
	PathAddBackslash(szDestPath);

	srand(GetTickCount());

	int len = (int)_tcslen(szDestPath);
	szDestPath[len++] = _T('x');

	for (int i=0; i < 4; i++)
	{
		szDestPath[len++] = (rand() % 0x19) + 0x61;
		szDestPath[len++] = (rand() % 0x19) + 0x41;
	}

	_tcscat(szDestPath, _T(".dll"));

	CopyFile(szPath, szDestPath, FALSE);

	_tcscpy(pszPath, szDestPath);
}

