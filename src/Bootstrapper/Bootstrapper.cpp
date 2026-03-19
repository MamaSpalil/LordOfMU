// Bootstrapper.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "MuInstanceManager.h"

//-------------------------------------------------------
// shared data 
// Notice:	seen by both: the instance of "HookSpy.dll" maped
//			into the remote process as well as by the instance
//			of "HookSpy.dll" mapped into our "HookSpy.exe"
#pragma data_seg (".shared")
HHOOK	g_hHook = 0;
UINT	WM_HOOKSPY = 0;
#pragma data_seg ()

#pragma comment(linker,"/SECTION:.shared,RWS") 



class CBootstrapperModule : public CAtlDllModuleT<CBootstrapperModule>
{
public:
	CBootstrapperModule() : hMutex(0), hModule(0) {}

	HMODULE hModule;
	HANDLE hMutex;
	CMuInstanceManager cInstManager;
};

CBootstrapperModule _AtlModule;


// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	_AtlModule.hModule = (HMODULE)hInstance;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DWORD pid = 0;
		DWORD pid2 = GetCurrentProcessId();
		HWND hWnd = FindWindow(_T("Shell_TrayWnd"), 0);

		GetWindowThreadProcessId(hWnd, &pid);

		if (pid2 == pid)
		{
			_AtlModule.hMutex = CreateMutex(0, 0, _T("__LordOfMU__"));

			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(_AtlModule.hMutex);
				_AtlModule.hMutex = 0;
			}
			else if (!_AtlModule.cInstManager.Init())
			{
				CloseHandle(_AtlModule.hMutex);
				_AtlModule.hMutex = 0;
			}
			else
			{
				TCHAR szLibName[_MAX_PATH+1] = {0};
				GetModuleFileName((HMODULE)hInstance, szLibName, _MAX_PATH);

				LoadLibrary(szLibName);
			}
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		if (_AtlModule.hMutex != 0)
		{
			_AtlModule.cInstManager.Term();
			CloseHandle(_AtlModule.hMutex);
		}
	}

    return _AtlModule.DllMain(dwReason, lpReserved); 
}


/**
 * \brief
 */
static LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam)
{	
	if (((CWPSTRUCT*)lParam)->message == WM_HOOKSPY) 
	{
		::UnhookWindowsHookEx(g_hHook);
	}

	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}


/**
 * \brief
 */
extern "C" __declspec(dllexport) bool Load()
{	
	HWND hWnd = FindWindow(_T("Shell_TrayWnd"), 0);
	DWORD dwThreadId = GetWindowThreadProcessId(hWnd, NULL);

	g_hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)&HookProc, _AtlModule.hModule, dwThreadId);

	if (g_hHook == NULL) 
		return false;
	
	if (WM_HOOKSPY == NULL)
		WM_HOOKSPY = ::RegisterWindowMessage(_T("WM_LORDOFMU_INIT"));

	SendMessage(hWnd, WM_HOOKSPY, 0, 0);
	return true;
}


/**
 * \brief
 */
extern "C" __declspec(dllexport) bool Load2()
{	
	_AtlModule.hMutex = CreateMutex(0, 0, _T("__LordOfMU__"));

	if (GetLastError() == ERROR_ALREADY_EXISTS || !_AtlModule.cInstManager.Init(false))
	{
		CloseHandle(_AtlModule.hMutex);
		return false;
	}
	else
	{
		TCHAR szLibName[_MAX_PATH+1] = {0};
		GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szLibName, _MAX_PATH);

		LoadLibrary(szLibName);
	}

	MSG msg = {0};

	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	_AtlModule.cInstManager.Term();
	CloseHandle(_AtlModule.hMutex);
	
	_AtlModule.hMutex = 0;

	return true;
}


