// LordOfMUdll.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "Proxifier.h"
#include "DebugOut.h"
#include "DebugMode.h"
#include "ClickerLogger.h"

TCHAR g_szRoot[_MAX_PATH + 1] = {0};
static HINSTANCE g_hDllInstance = NULL;
static volatile LONG g_lHookApplied = 0;


/**
 * \brief Check if this DLL is already loaded (hooked) in the process.
 *        Returns TRUE if the DLL was previously injected.
 *        Checks both "LordOfMU.dll" (standard build) and "MUEliteClicker.dll"
 *        (Release [Clicker] build) as the DLL name varies by build configuration.
 */
static BOOL CheckDllHookPresence()
{
	HMODULE hMod = GetModuleHandle(_T("LordOfMU.dll"));
	if (!hMod)
		hMod = GetModuleHandle(_T("MUEliteClicker.dll"));

	if (hMod && hMod != g_hDllInstance)
	{
		CDebugOut::PrintAlways("[HOOK_CHECK] Another instance of LordOfMU DLL detected at 0x%p", hMod);
		WriteHookLog("DLL hook already present: another instance at 0x%p (our instance 0x%p)", hMod, g_hDllInstance);
		return TRUE;
	}

	WriteHookLog("DLL hook presence check passed - no duplicate DLL detected");
	return FALSE;
}


/**
 * \brief Verify all hook patches are intact and not tampered with.
 */
static void VerifyHookIntegrity()
{
	HMODULE hWs2 = GetModuleHandleA("ws2_32.dll");
	if (hWs2)
	{
		BYTE* pConnect = (BYTE*)GetProcAddress(hWs2, "connect");
		if (pConnect && pConnect[0] == 0xE9)
		{
			CDebugOut::PrintAlways("[HOOK_CHECK] ws2_32!connect hook is active (JMP at 0x%p)", pConnect);
			WriteHookLog("Hook integrity OK: ws2_32!connect hook active (JMP 0xE9 at 0x%p)", pConnect);
		}
		else if (pConnect)
		{
			CDebugOut::PrintAlways("[HOOK_CHECK] WARNING: ws2_32!connect hook is NOT active (byte=0x%02X at 0x%p)", pConnect[0], pConnect);
			WriteHookLog("Hook integrity WARNING: ws2_32!connect NOT hooked (byte=0x%02X at 0x%p)", pConnect[0], pConnect);
		}
	}
	else
	{
		CDebugOut::PrintAlways("[HOOK_CHECK] ws2_32.dll not loaded yet - hooks will be applied during init");
		WriteHookLog("ws2_32.dll not yet loaded - hooks pending");
	}
}


/**
 * \brief Clean up DLL hook traces when main.exe is closing.
 *        Unhooks API patches and frees resources.
 */
static void CleanupDllHook()
{
	CDebugOut::PrintAlways("[HOOK_CLEANUP] Cleaning up DLL hooks from main.exe process...");
	WriteHookLog("Starting DLL hook cleanup from main.exe process");

	CProxifier::CleanUp();

	InterlockedExchange(&g_lHookApplied, 0);

	CDebugOut::PrintAlways("[HOOK_CLEANUP] DLL hook cleanup complete. All hooks removed.");
	WriteHookLog("DLL hook cleanup complete - all hooks removed, g_lHookApplied reset to 0");
}


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
		CDebugOut::PrintAlways("=== LordOfMU DLL initializing (InternalInit) ===");
		WriteDebugLog("LordOfMU DLL initializing (InternalInit)");

		// Install crash handler for main.exe
		CDebugMode::Install();
		CDebugOut::PrintAlways("[DEBUG_MODE] Crash handler installed for main.exe");
		WriteHookLog("Crash handler installed for main.exe process");

		// Check for existing DLL hook presence
		if (CheckDllHookPresence())
		{
			CDebugOut::PrintAlways("[HOOK_CHECK] DLL hook already present - skipping re-initialization");
			WriteHookLog("DLL hook already present - skipping re-initialization");
			return true;
		}

		// Verify hook integrity before applying
		VerifyHookIntegrity();

		// Apply hooks only once
		if (InterlockedCompareExchange(&g_lHookApplied, 1, 0) == 0)
		{
			CDebugOut::PrintAlways("[HOOK] Applying DLL hooks for the first time...");
			WriteHookLog("Applying DLL hooks for the first time (InterlockedCompareExchange 0->1)");
			if (CProxifier::StartUp())
			{
				CDebugOut::PrintAlways("[HOOK] DLL hooks applied successfully.");
				WriteHookLog("DLL hooks applied successfully - proxy system active");
			}
			else
			{
				CDebugOut::PrintAlways("[HOOK] ERROR: DLL hook installation failed! Game will run without proxy.");
				WriteHookLog("ERROR: DLL hook installation failed - game running without proxy");
				InterlockedExchange(&g_lHookApplied, 0);
			}
		}
		else
		{
			CDebugOut::PrintAlways("[HOOK] DLL hooks already applied - skipping duplicate hook.");
			WriteHookLog("DLL hooks already applied (g_lHookApplied=1) - skipping duplicate");
		}

		// Post-init hook verification
		VerifyHookIntegrity();

		CDebugOut::PrintAlways("=== LordOfMU DLL initialization complete ===");
		WriteDebugLog("LordOfMU DLL initialization complete");
		return true;
	}

	static int OnExit(void)
	{
		CDebugOut::PrintAlways("=== LordOfMU DLL shutting down (OnExit) ===");
		WriteDebugLog("LordOfMU DLL shutting down (OnExit)");
		WriteHookLog("DLL shutdown initiated - starting hook removal from main.exe");

		// Uninstall crash handler
		CDebugMode::Uninstall();
		WriteHookLog("Crash handler uninstalled");

		// Clean up hooks when main.exe closes
		CleanupDllHook();

		CDebugOut::PrintAlways("=== LordOfMU DLL cleanup complete ===");
		WriteDebugLog("LordOfMU DLL cleanup complete - all resources released");
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
		g_hDllInstance = hInstance;
		DisableThreadLibraryCalls(hInstance);

		// Do NOT call InitClient() here - creating threads and calling
		// ShellExecute under the loader lock causes deadlock, preventing
		// main.exe window from opening. Initialization is deferred to
		// DllGetClassObject() which runs after LoadLibrary() returns.
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		CDebugOut::PrintAlways("[DLL] DLL_PROCESS_DETACH - game process unloading DLL.");
		WriteHookLog("DLL_PROCESS_DETACH - main.exe unloading LordOfMU DLL, removing hooks");
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

	static volatile LONG s_lInitialized = 0;

	if (InterlockedCompareExchange(&s_lInitialized, 1, 0) == 0)
	{
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