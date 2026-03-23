// AutoClicker.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "AutoClicker.h"
#include "MuWindow.h"
#include "MuWindowUtil.h"
#include "version.h"
#include "ClickerLogger.h"
#include <sys/stat.h>

HANDLE g_hInitThread = 0;


/**
 * \brief Ensures MUEliteClicker.dll (LordOfMU DLL) is loaded in the game process.
 *        Called early in InitThread before InitClicker to install proxy hooks
 *        before the game connects to the server.
 *
 *        This handles the case where the LoaderDll (CBT hook) mechanism fails
 *        to load MUEliteClicker.dll, but the Clicker DLL is loaded independently.
 */
static void EnsureLordOfMUDllLoaded()
{
	// Check if already loaded via env var (set by LoaderDll or previous call)
	char szEnvBuf[32] = {0};
	if (GetEnvironmentVariableA("__LordOfMU_HMODULE__", szEnvBuf, sizeof(szEnvBuf)) > 0)
	{
		HMODULE hEnvMod = (HMODULE)(ULONG_PTR)_strtoui64(szEnvBuf, NULL, 16);
		if (hEnvMod && GetProcAddress(hEnvMod, "SendCommand"))
		{
			WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: already loaded (env var='%s')", szEnvBuf);
			return;
		}
	}

	// Check if loaded by original name
	HMODULE hMod = GetModuleHandle(_T(__LORDOFMU_DLL_NAME));
	if (hMod && GetProcAddress(hMod, "SendCommand"))
	{
		WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: %s already loaded at 0x%p",
			__LORDOFMU_DLL_NAME, (void*)hMod);

		// Store env var so SayToServer can find it quickly
		char szBuf[32] = {0};
		_snprintf_s(szBuf, sizeof(szBuf), _TRUNCATE, "0x%p", (void*)hMod);
		SetEnvironmentVariableA("__LordOfMU_HMODULE__", szBuf);
		return;
	}

	// Not loaded - determine our directory and load MUEliteClicker.dll from there
	TCHAR szDllDir[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szDllDir, _MAX_PATH);
	for (int i = (int)_tcslen(szDllDir) - 1; i >= 0 && szDllDir[i] != _T('\\'); szDllDir[i--] = 0);

	TCHAR szDllPath[_MAX_PATH+1] = {0};
	_tcscpy(szDllPath, szDllDir);
	_tcscat(szDllPath, _T(__LORDOFMU_DLL_NAME));

	WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: Loading %s from '%s'",
		__LORDOFMU_DLL_NAME, (LPCSTR)CT2A(szDllPath));

	struct _stat st = {0};
	if (0 != _tstat(szDllPath, &st))
	{
		WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: FAILED - file not found: '%s'",
			(LPCSTR)CT2A(szDllPath));
		return;
	}

	HMODULE hDirect = LoadLibrary(szDllPath);
	if (!hDirect)
	{
		DWORD dwErr = GetLastError();
		WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: LoadLibrary FAILED (error=%d)", (int)dwErr);
		return;
	}

	WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: Loaded at 0x%p - initializing proxy hooks",
		(void*)hDirect);

	// Call DllGetClassObject to initialize proxy hooks (connect hook, etc.)
	// This must happen BEFORE the game connects to the server.
	typedef HRESULT (__stdcall* InitModulePtr)(REFCLSID, REFIID, LPVOID*);
	InitModulePtr pInit = (InitModulePtr)GetProcAddress(hDirect, "DllGetClassObject");
	if (pInit)
	{
		GUID guid = {0};
		pInit(guid, guid, (LPVOID*)szDllDir);
		WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: DllGetClassObject called, root='%s'",
			(LPCSTR)CT2A(szDllDir));
	}
	else
	{
		WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: WARNING - DllGetClassObject not found");
	}

	// Store HMODULE via environment variable for SayToServer lookups
	char szBuf[32] = {0};
	_snprintf_s(szBuf, sizeof(szBuf), _TRUNCATE, "0x%p", (void*)hDirect);
	SetEnvironmentVariableA("__LordOfMU_HMODULE__", szBuf);
	WriteClickerLogFmt("CLICKER", "EnsureLordOfMUDllLoaded: env var __LordOfMU_HMODULE__ set to '%s'", szBuf);
}


/**
 * \brief 
 */
BOOL CAutoClickerModule::InitClicker()
{
	m_hMuWindow = 0;
	m_vMuInstances.clear();

	EnumWindows(MyEnumWndProc, (LPARAM)this);

	if (!m_hMuWindow)
		return FALSE;

	int iInstanceNumber = 1;
	for (iInstanceNumber; iInstanceNumber < 32000; ++iInstanceNumber)
	{
		if (m_vMuInstances.find(iInstanceNumber) == m_vMuInstances.end())
			break;
	}

	return CMuWindow::Init(m_hMuWindow, iInstanceNumber);
}


/**
 * \brief 
 */
void CAutoClickerModule::TermClicker()
{
	CMuWindow::Term();
}


/**
 * \brief 
 */
BOOL CALLBACK CAutoClickerModule::MyEnumWndProc(HWND hWnd, LPARAM pData)
{
	CAutoClickerModule* pThis = (CAutoClickerModule*)pData;

	if (CMuWindowUtil::IsMuWindow(hWnd))
	{
		DWORD_PTR dwRes = 0;
		if (SendMessageTimeout(hWnd, WM_IS_CLICKER_INSTALLED, 0, 0, SMTO_BLOCK, 500, &dwRes) != 0)
		{
			pThis->m_vMuInstances.insert((int)dwRes);
		}

		DWORD dwProcId = 0;
		GetWindowThreadProcessId(hWnd, &dwProcId);

		if (dwProcId == GetCurrentProcessId())
		{
			pThis->m_hMuWindow = hWnd;
		}
	}

	return TRUE;
}

CAutoClickerModule _AtlModule;


/**
 * \brief Initialization thread - runs after DllMain returns.
 *        Ensures MUEliteClicker.dll is loaded (proxy hooks installed)
 *        before the game connects to the server, then initializes the clicker.
 */
DWORD CALLBACK InitThread(void*)
{
	// CRITICAL: Load MUEliteClicker.dll FIRST, before InitClicker.
	// The proxy hooks (ws2_32!connect) must be installed before the game
	// calls connect() to the game server. InitClicker waits for the game
	// window, which is created before connect(), so loading here is early enough.
	EnsureLordOfMUDllLoaded();

	DWORD dwStart = GetTickCount();

	while (!_AtlModule.InitClicker())
	{
		Sleep(500);

		if (GetTickCount() - dwStart > 5000)
			break;
	}

	return 0;
}


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

		g_hInitThread = CreateThread(0, 0, InitThread, 0, 0, 0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		_AtlModule.TermClicker();
	}

    return TRUE; 
}
