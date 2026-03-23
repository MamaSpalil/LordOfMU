// LoaderDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "stdlib.h"
#include "tchar.h"
#include "MuWindowUtil.h"
#include "KillUtil.h"
#include "version.h"
#include "atlstr.h"
#include "ClickerLogger.h"

#pragma data_seg(".shared")
HHOOK	g_hHook = 0;
HMODULE g_hModule = 0;
TCHAR   g_szRoot[_MAX_PATH+1] = {0};
#pragma data_seg()

#pragma comment(linker,"/SECTION:.shared,RWS") 

HMODULE g_hInjected = 0;
HANDLE g_hBypassEvent = 0;

static TCHAR g_szDllName[_MAX_PATH+1] = {0};
static TCHAR g_szDllName2[_MAX_PATH+1] = {0};
static bool g_fClient = false;

typedef HRESULT (__stdcall* InitClickerModulePtr)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

static void CopyDlls();


/**
 * \brief 
 */
BOOL APIENTRY DllMain(HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		g_hBypassEvent = CreateEvent(0, 1, 0, _T("__LordOfMU_Event01__"));

		g_hModule = hModule;
		DisableThreadLibraryCalls(hModule);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		if (g_hBypassEvent)
			CloseHandle(g_hBypassEvent);

		g_hBypassEvent = 0;
	}

	return TRUE;
}


/**
 * \brief 
 */
static void RunAutoUpdate()
{
	TCHAR szPath[_MAX_PATH+1] = {0};
	GetModuleFileName(g_hModule, szPath, _MAX_PATH);

	for (int i=(int)_tcslen(szPath)-1; i >= 0 && szPath[i] != _T('\\'); szPath[i] = 0, --i);

	_tcscat(szPath, _T("LordOf"));
	_tcscat(szPath, _T("Update"));
	_tcscat(szPath, _T(".dll"));

	struct _stat st = {0};
	if (0 == _tstat(szPath, &st))
	{
		STARTUPINFO si = {0};
		si.cb = sizeof(STARTUPINFO);

		PROCESS_INFORMATION pi = {0};

		if (CreateProcess(szPath, 0, 0, 0, 0, DETACHED_PROCESS, 0, 0, &si, &pi))
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
		}
	}
}



/**
 * \brief 
 */
static LRESULT WINAPI RunDllWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC pfnOldWndProc = (WNDPROC)RemoveProp(hWnd, _T("__OldWindowProc__"));
	SetWindowLongPtr(hWnd, GWL_WNDPROC, PtrToLong(pfnOldWndProc));

	LRESULT res = CallWindowProc(pfnOldWndProc, hWnd, uMsg, wParam, lParam);

	struct _stat st = {0};

#if defined(__CLICKER_ELITE__) || defined(__ANTIHACK_STUFF__) || defined(__HACK_STUFF__) || defined(__CLICKER_STUFF__)

	TCHAR szPath[_MAX_PATH+1] = {0};


	if (g_fClient)
	{
#if defined(__CLICKER_ELITE__) || defined(__CLICKER_AVANTA__)
		GetTempPath(_MAX_PATH, szPath);
		PathAddBackslash(szPath);
#else
		GetModuleFileName(g_hModule, szPath, _MAX_PATH);
		for (int i=(int)_tcslen(szPath)-1; i >= 0 && szPath[i] != _T('\\'); szPath[i--] = 0);
#endif
	}
	else
	{
		GetModuleFileName(g_hModule, szPath, _MAX_PATH);
		for (int i=(int)_tcslen(szPath)-1; i >= 0 && szPath[i] != _T('\\'); szPath[i--] = 0);
	}

	_tcscat(szPath, g_szDllName);


	WriteHookLog("RunDllWndProc: Loading LordOfMU DLL from '%s'", (LPCSTR)CT2A(szPath));

	if (0 != _tstat(szPath, &st))
	{
		WriteHookLog("RunDllWndProc: CRITICAL - LordOfMU DLL file not found at '%s'", (LPCSTR)CT2A(szPath));

#if defined(__CLICKER_ELITE__) || defined(__CLICKER_AVANTA__)
		MessageBox(0, _T("Clicker module not found!"), _T("Error"), MB_OK);
#else
		MessageBox(0, _T("MU Anti-hack module not found!"), _T("Error"), MB_OK);
#endif

		CKillUtil::KillGame("RunDllWndProc: module DLL not found");
		return res; // Prevent fall-through if KillGame doesn't terminate immediately
	}

	if (0 == (g_hInjected = LoadLibrary(szPath)))
	{
		DWORD dwErr = GetLastError();
		WriteHookLog("RunDllWndProc: CRITICAL - LoadLibrary failed for '%s' (error=%d)", (LPCSTR)CT2A(szPath), (int)dwErr);

		#if defined(__CLICKER_ELITE__) || defined(__CLICKER_AVANTA__)
			MessageBox(0, _T("Cannot load clicker module!"), _T("Error"), MB_OK);
		#else
			MessageBox(0, _T("Cannot load Lord of MU Anti-hack!"), _T("Error"), MB_OK);
		#endif

		CKillUtil::KillGame("RunDllWndProc: LoadLibrary failed for module DLL");
		return res; // Prevent fall-through if KillGame doesn't terminate immediately
	}

	// Store the LordOfMU DLL HMODULE as a window property so the Clicker DLL
	// can find it reliably via GetProp() without depending on module name lookups.
	SetProp(hWnd, _T("__LordOfMU_Module__"), (HANDLE)g_hInjected);

	// Also store HMODULE via environment variable as a reliable fallback.
	// Window properties can fail if the Clicker DLL subclasses a different window
	// than the one SetProp was called on (e.g., game recreates its window).
	// Environment variables are per-process and accessible from any DLL.
	{
		char szBuf[32] = {0};
		_snprintf_s(szBuf, sizeof(szBuf), _TRUNCATE, "0x%p", (void*)g_hInjected);
		SetEnvironmentVariableA("__LordOfMU_HMODULE__", szBuf);
		WriteHookLog("RunDllWndProc: __LordOfMU_HMODULE__ env var set to '%s'", szBuf);
	}

	WriteHookLog("LordOfMU DLL loaded at 0x%p, HMODULE stored via SetProp(hWnd=0x%p) and env var",
		(void*)g_hInjected, (void*)hWnd);

	InitClickerModulePtr InitProc = (InitClickerModulePtr)GetProcAddress(g_hInjected, "DllGetClassObject");

	GUID guid = {0};

	if (InitProc)
	{
		InitProc(guid, guid, (LPVOID*)g_szRoot);
		WriteHookLog("RunDllWndProc: LordOfMU DLL DllGetClassObject called successfully");
	}
	else
	{
		WriteHookLog("RunDllWndProc: WARNING - DllGetClassObject export not found in LordOfMU DLL");
	}

#endif

#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__)
	if (g_fClient)
	{
		// Only load Clicker DLL if LordOfMU DLL was successfully loaded.
		// Without the LordOfMU DLL, the Clicker cannot send commands via SayToServer
		// and all advanced pickup/packet functionality will be non-functional.
		if (!g_hInjected)
		{
			WriteHookLog("RunDllWndProc: SKIPPING Clicker DLL load - LordOfMU DLL not loaded (g_hInjected=NULL)");
		}
		else
		{
			TCHAR szPath2[_MAX_PATH+1] = {0};
			GetTempPath(_MAX_PATH, szPath2);
			PathAddBackslash(szPath2);
			_tcscat(szPath2, g_szDllName2);

			WriteHookLog("RunDllWndProc: Loading Clicker DLL from '%s'", (LPCSTR)CT2A(szPath2));

			if (0 != _tstat(szPath2, &st))
			{
				WriteHookLog("RunDllWndProc: WARNING - Clicker DLL not found at '%s'", (LPCSTR)CT2A(szPath2));
			}
			else
			{
				HMODULE hClicker = LoadLibrary(szPath2);

				if (hClicker)
				{
					WriteHookLog("RunDllWndProc: Clicker DLL loaded at 0x%p", (void*)hClicker);

					InitClickerModulePtr InitProc2 = (InitClickerModulePtr)GetProcAddress(hClicker, "DllGetClassObject");

					GUID guid2 = {0};

					if (InitProc2)
					{
						InitProc2(guid2, guid2, (LPVOID*)g_szRoot);
						WriteHookLog("RunDllWndProc: Clicker DLL DllGetClassObject called successfully");
					}
				}
				else
				{
					DWORD dwErr2 = GetLastError();
					WriteHookLog("RunDllWndProc: WARNING - Clicker DLL LoadLibrary failed (error=%d)", (int)dwErr2);
				}
			}
		}
	}
#endif 

	return res;
}


/**
 * \brief 
 */
static void RunClient(HWND hWnd)
{
#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__)
	CopyDlls();
#else
	_tcscpy(g_szDllName, _T(__LORDOFMU_DLL_NAME));
#endif

	if (GetModuleHandle(g_szDllName) != 0)
		return; // already injected

	RunAutoUpdate();

	g_fClient = true;
	SetProp(hWnd, _T("__OldWindowProc__"), LongToHandle(SetWindowLongPtr(hWnd, GWL_WNDPROC, PtrToLong(RunDllWndProc))));
	PostMessage(hWnd, WM_NULL, 0, 0);
}


/**
 * \brief 
 */
static void RunServer(HWND hWnd)
{
	_tcscpy(g_szDllName, _T("LordOfServer.dll"));

	if (GetModuleHandle(g_szDllName) != 0)
		return; // already injected

	SetProp(hWnd, _T("__OldWindowProc__"), LongToHandle(SetWindowLongPtr(hWnd, GWL_WNDPROC, PtrToLong(RunDllWndProc))));
	PostMessage(hWnd, WM_NULL, 0, 0);
}


/**
 *
 */
static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(g_hHook, nCode, wParam, lParam);
	}

	if (nCode == HCBT_CREATEWND)
	{
		HWND hWnd = (HWND)wParam;

		if (CMuWindowUtil::IsMuWindow(hWnd)) //MU
		{
			RunClient(hWnd);
		}
		else if (CMuWindowUtil::IsGSWindow(hWnd))
		{
			RunServer(hWnd);
		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}


/**
 * \brief
 */
STDAPI DllRegisterServer()
{
	return S_OK;
}


/**
 * \brief
 */
STDAPI DllUnregisterServer()
{
	if (g_hHook)
	{
		UnhookWindowsHookEx(g_hHook);
		g_hHook = 0;
	}

	return S_OK;
}


/**
 * \brief
 */
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	LPCTSTR lpszRoot = (LPCTSTR)ppv;

	if (!g_hHook)
	{
		if (lpszRoot)
			_tcscpy(g_szRoot, lpszRoot);
		else
			g_szRoot[0] = 0;

		g_hHook = SetWindowsHookEx(WH_CBT, CBTProc, g_hModule, 0);
	}

	return S_OK;
}


/**
 * \brief
 */
STDAPI DllCanUnloadNow()
{
	return S_OK;
}


/**
 * \brief 
 */
void Main()
{
	RunAutoUpdate();

	_tcscpy(g_szDllName, _T("LordOfMU.dll"));

	TCHAR szPath[_MAX_PATH+1] = {0};
	GetModuleFileName(g_hModule, szPath, _MAX_PATH);

	for (int i=(int)_tcslen(szPath)-1; i >= 0 && szPath[i] != _T('\\'); szPath[i] = 0, --i);
	_tcscat(szPath, g_szDllName);

	struct _stat st = {0};

	if (0 != _tstat(szPath, &st)
		|| 0 == (g_hInjected = LoadLibrary(szPath)))
	{
		MessageBox(0, _T("Cannot load Lord of MU Anti-hack!"), _T("Error"), MB_OK);
		CKillUtil::KillGame("Main: cannot load LordOfMU.dll");
	}
}


/**  
 * \brief 
 */
void CopyDlls()
{
	TCHAR szPath[_MAX_PATH + 1] = {0};
	GetModuleFileName(g_hModule, szPath, _MAX_PATH);

	WriteHookLog("CopyDlls: LoaderDll module path: '%s'", (LPCSTR)CT2A(szPath));

	int i=(int)_tcslen(szPath) - 1;
	for (i; i >= 0 && szPath[i] != _T('\\'); --i);

	if (i >= 0)
		szPath[i] = 0;

	i++;

	TCHAR* pszModName = szPath + i;
	_tcscpy(g_szDllName, pszModName);
	_tcscpy(g_szDllName2, pszModName);

	g_szDllName[1] = _T('K');
	g_szDllName2[1] = _T('T');

	WriteHookLog("CopyDlls: Stealth names - LordOfMU='%s', Clicker='%s'",
		(LPCSTR)CT2A(g_szDllName), (LPCSTR)CT2A(g_szDllName2));
	WriteHookLog("CopyDlls: g_szRoot='%s'", (LPCSTR)CT2A(g_szRoot));

	TCHAR szSrcFile[_MAX_PATH+1] = {0};
	TCHAR szDstFile[_MAX_PATH+1] = {0};

	// Copy LordOfMU DLL (MUEliteClicker.dll) to stealth name
	_tcscpy(szSrcFile, g_szRoot);
	_tcscat(szSrcFile, CA2T(__LORDOFMU_DLL_NAME));

	_tcscpy(szDstFile, szPath);
	_tcscat(szDstFile, _T("\\"));
	_tcscat(szDstFile, g_szDllName);

	// Validate source file exists before attempting copy
	struct _stat stSrc = {0};
	if (0 != _tstat(szSrcFile, &stSrc))
	{
		WriteHookLog("CopyDlls: CRITICAL - LordOfMU DLL source not found: '%s' (check g_szRoot)",
			(LPCSTR)CT2A(szSrcFile));
	}

	if (!CopyFile(szSrcFile, szDstFile, FALSE))
	{
		DWORD dwErr = GetLastError();
		WriteHookLog("CopyFile FAILED for LordOfMU DLL: src='%s' dst='%s' err=%d",
			(LPCSTR)CT2A(szSrcFile), (LPCSTR)CT2A(szDstFile), (int)dwErr);
	}
	else
	{
		WriteHookLog("CopyFile OK for LordOfMU DLL: src='%s' dst='%s'",
			(LPCSTR)CT2A(szSrcFile), (LPCSTR)CT2A(szDstFile));
	}

	// Copy Clicker DLL (MUAutoClicker.dll) to stealth name
	_tcscpy(szSrcFile, g_szRoot);
	_tcscat(szSrcFile, CA2T(__CLICKER_DLL_NAME));

	_tcscpy(szDstFile, szPath);
	_tcscat(szDstFile, _T("\\"));
	_tcscat(szDstFile, g_szDllName2);

	// Validate source file exists before attempting copy
	if (0 != _tstat(szSrcFile, &stSrc))
	{
		WriteHookLog("CopyDlls: CRITICAL - Clicker DLL source not found: '%s' (check g_szRoot)",
			(LPCSTR)CT2A(szSrcFile));
	}

	if (!CopyFile(szSrcFile, szDstFile, FALSE))
	{
		DWORD dwErr = GetLastError();
		WriteHookLog("CopyFile FAILED for Clicker DLL: src='%s' dst='%s' err=%d",
			(LPCSTR)CT2A(szSrcFile), (LPCSTR)CT2A(szDstFile), (int)dwErr);
	}
	else
	{
		WriteHookLog("CopyFile OK for Clicker DLL: src='%s' dst='%s'",
			(LPCSTR)CT2A(szSrcFile), (LPCSTR)CT2A(szDstFile));
	}
}

