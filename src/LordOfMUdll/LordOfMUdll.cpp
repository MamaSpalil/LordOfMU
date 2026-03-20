// LordOfMUdll.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "Proxifier.h"
#include "DebugOut.h"

TCHAR g_szRoot[_MAX_PATH + 1] = {0};


/**
 *
 */
class CLordOfMUdllModule : public CAtlDllModuleT< CLordOfMUdllModule >
{
public:
	CLordOfMUdllModule() {}

	void InitClient()
	{
		InternalInit();
	}

	void TermClient()
	{
		OnExit();
	}

protected:
	bool InternalInit()
	{
		CDebugOut::Init();
		CProxifier::StartUp();

		CDebugOut::PrintDebug("InternalInit() OK.");
		return true;
	}

	static int OnExit(void)
	{
		// do clean-up here
		CProxifier::CleanUp();
		CDebugOut::CleanUp();

		return 0;
	}
};


/**
 * \brief
 */
CLordOfMUdllModule _AtlModule;


/**
 * \brief DLL Entry Point
 */
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (!_AtlModule.DllMain(dwReason, lpReserved))
		return FALSE;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInstance);

		// Do NOT call InitClient() here - creating threads and calling
		// ShellExecute under the loader lock causes deadlock, preventing
		// main.exe window from opening. Initialization is deferred to
		// DllGetClassObject() which runs after LoadLibrary() returns.
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		CDebugOut::PrintDebug("DLL_PROCESS_DETACH ... ");
		_AtlModule.TermClient();
	}

	return TRUE; 
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
	return S_OK;
}


/**
 * \brief
 */
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	LPCTSTR lpszRoot = (LPCTSTR)ppv;

	if (lpszRoot)
		_tcscpy(g_szRoot, lpszRoot);

	static bool s_bInitialized = false;

	if (!s_bInitialized)
	{
		s_bInitialized = true;
		_AtlModule.InitClient();
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