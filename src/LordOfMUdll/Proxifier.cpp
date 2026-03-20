#include "stdafx.h"
#include "Proxifier.h"
#include "ProxyBuilder.h"
#include "DebugOut.h"
#include "KillUtil.h"
#include "PEUtil.h"
#include "Psapi.h"
#include "ClickerLogger.h"



/**
 * \brief Single instance pointer
 */
CProxifier* CProxifier::m_sInstance = 0;


/**
 * \brief  Original socket interface
 */
CAcceptPtr CProxifier::accept;

CSendPtr CProxifier::send;
CRecvPtr CProxifier::recv;
CSocketConnectTramp CProxifier::connect;
CCloseSocketPtr CProxifier::closesocket;
CSelectPtr CProxifier::select;
CListenPtr CProxifier::listen;
CBindPtr CProxifier::bind;
CSocketPtr CProxifier::socket;
CShutdownPtr CProxifier::shutdown;
CIoctlSocketPtr CProxifier::ioctlsocket;
CWSAGetLastErrorPtr CProxifier::WSAGetLastError;
CWSASendDisconnectPtr CProxifier::WSASendDisconnect;




/**
 *
 */
CProxifier::CProxifier()
{
	m_hBypassEvent = CreateEvent(0, 1, 0, _T("__LordOfMU_Event01__"));

	m_hRunMonitor = CreateEvent(0, 1, 0, 0);
	m_hPatchMonitor = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PatchMonitor, this, 0, 0);
}


/**
 *
 */
CProxifier::~CProxifier()
{
	if (m_hPatchMonitor != 0 && m_hPatchMonitor != INVALID_HANDLE_VALUE)
	{
		TerminateThread(m_hPatchMonitor, 0);
		CloseHandle(m_hPatchMonitor);
	}

	if (m_hRunMonitor)
		CloseHandle(m_hRunMonitor);

	if (m_hBypassEvent)
		CloseHandle(m_hBypassEvent);
}


/**
 *
 */
bool CProxifier::StartUp()
{
	return GetInstance()->InternalInit();
}


/**
 * \brief 
 */
void CProxifier::CleanUp()
{
	GetInstance()->InternalClean();

	if (m_sInstance)
		delete m_sInstance;

	m_sInstance = 0;
}


/**
 *
 */
CProxifier* CProxifier::GetInstance()
{
	if (!m_sInstance)
		m_sInstance = new CProxifier();

	return m_sInstance;
}


/**
 *
 */
bool CProxifier::InternalInit()
{
	CDebugOut::PrintAlways("Initializing Proxifier ...");
	WriteHookLog("Proxifier initialization started");

#ifndef DEBUG
	if (IsDebuggerPresent())
	{
		// NOTE: Previously called CKillUtil::KillGame() here which terminated
		// main.exe after 3 seconds. Changed to warning because this was one of
		// the causes of the game window closing unexpectedly on startup.
		// SECURITY: In production anti-cheat deployments, consider re-enabling
		// this check or using a configuration flag to control behavior.
		CDebugOut::PrintAlways("[WARNING] Debugger application detected - ignoring to keep game running.");
	}
#endif

	// Hook Api functions
	CDebugOut::PrintAlways("Hooking Winsock API functions ...");
	if (!connect.Patch(&connect2, m_cWinsockApi))
	{
		CDebugOut::PrintAlways("[ERROR] Failed to patch ws2_32!connect - hook installation failed");
		WriteHookLog("ERROR: Failed to patch ws2_32!connect");
		return false;
	}
	CDebugOut::PrintAlways("ws2_32!connect hook installed successfully.");

	accept.Init();
	bind.Init();
	closesocket.Init();
	send.Init();
	recv.Init();
	select.Init();
	listen.Init();
	socket.Init();
	shutdown.Init();
	ioctlsocket.Init();
	WSAGetLastError.Init();
	WSASendDisconnect.Init();
	CDebugOut::PrintAlways("Winsock API hooks installed successfully.");
	WriteHookLog("All Winsock API hooks installed successfully");


	StartPatchMonitor();
	CDebugOut::PrintAlways("Patch monitor started.");

	CheckWinsockDir();
	CDebugOut::PrintAlways("Winsock directory check passed.");

	// NOTE: Removed taskkill of start-S2.exe. The start-S2.exe is a MU Online
	// game launcher/patcher process. Killing it from within the injected DLL 
	// via ShellExecute("cmd.exe /C taskkill /F /IM start-S2.exe") was causing 
	// main.exe to close within 2-3 seconds of startup, likely because the
	// launcher process is a parent/dependency of the game client.
	// If start-S2.exe termination is needed, it should be done by LordOfMU.exe
	// (the launcher) before or after main.exe starts, not from inside main.exe.
	CDebugOut::PrintAlways("Skipping start-S2.exe termination to prevent game closure.");

	CDebugOut::PrintAlways("Initialization finished successfully.");
	WriteHookLog("Proxifier initialization completed successfully");
	return true;
}


/**
 * \brief 
 */
void CProxifier::InternalClean()
{
	WriteHookLog("Proxifier cleanup started - removing hooks");
	StopPatchMonitor();

	connect.UnPatch();
	WriteHookLog("Proxifier cleanup complete - all hooks removed");
}


/**
 *
 */
int WINAPI CProxifier::connect2(SOCKET s, const struct sockaddr* addr, int len)
{
	CDebugOut::PrintDebug("CProxifier::connect2() ... ");

	if (!s)
		return CProxifier::connect(s, addr, len);

	// Log socket connection details
	if (addr && addr->sa_family == AF_INET)
	{
		const struct sockaddr_in* pAddr = (const struct sockaddr_in*)addr;
		CDebugOut::PrintAlways("[SOCKET] connect() called: socket=%d, IP=%d.%d.%d.%d, port=%d",
			(int)s,
			pAddr->sin_addr.S_un.S_un_b.s_b1,
			pAddr->sin_addr.S_un.S_un_b.s_b2,
			pAddr->sin_addr.S_un.S_un_b.s_b3,
			pAddr->sin_addr.S_un.S_un_b.s_b4,
			ntohs(pAddr->sin_port));

		WriteSocketLog("connect() socket=%d IP=%d.%d.%d.%d port=%d",
			(int)s,
			pAddr->sin_addr.S_un.S_un_b.s_b1,
			pAddr->sin_addr.S_un.S_un_b.s_b2,
			pAddr->sin_addr.S_un.S_un_b.s_b3,
			pAddr->sin_addr.S_un.S_un_b.s_b4,
			ntohs(pAddr->sin_port));
	}

	CProxifier* pThis = CProxifier::GetInstance();
	pThis->cleanDead();

	CProxy* pProxy = CProxyBuilder::CreateProxy(s, addr, len);

	CProxyList::iterator it = pThis->m_vProxies.find(s);
	if (it != pThis->m_vProxies.end())
	{
		if (it->second)
			delete it->second;

		pThis->m_vProxies.erase(it);
	}

	if (pProxy)
		pThis->m_vProxies.insert(CProxyList::value_type(s, pProxy));

	return pProxy ? pProxy->connect(addr, len) : connect(s, addr, len);
}


/**  
 * \brief 
 */
void CProxifier::cleanDead()
{
	CProxyList vDead;

	for (CProxyList::iterator it = m_vProxies.begin(); it != m_vProxies.end(); ++it)
	{
		if (!it->second || it->second->isDead())
			vDead.insert(CProxyList::value_type(it->first, it->second));
	}

	for (CProxyList::iterator it = vDead.begin(); it != vDead.end(); ++it)
	{
		m_vProxies.erase(it->first);

		if (it->second)
			delete it->second;
	}
}


/**
 * \brief 
 */
DWORD WINAPI CProxifier::PatchMonitor(CProxifier* pThis)
{
	if (!pThis)
		return 0;

	CDebugOut::PrintDebug("Patch monitor started ... ");

	while (1)
	{
		if (WAIT_TIMEOUT != WaitForSingleObject(pThis->m_hRunMonitor, 0))
		{
#if defined(__ANTIHACK_STUFF__) && !defined(__INCLUDE_ALL_STUFF__)
			pThis->CheckPatches();
#endif
		}

		Sleep(2000);
	}

	return 0;
}


/**
 * \brief 
 */
void CProxifier::StartPatchMonitor()
{
	CDebugOut::PrintDebug("Starting patch monitor started ... ");
	SetEvent(m_hRunMonitor);
}


/**
 * \brief 
 */
void CProxifier::StopPatchMonitor()
{
	CDebugOut::PrintDebug("Stopping patch monitor started ... ");
	ResetEvent(m_hRunMonitor);
}


/**
 * \brief 
 */
void CProxifier::CheckPatches()
{
	CheckMyPatch("ws2_32.dll", "connect", (DWORD_PTR)connect2);
	CheckNotPatched("ws2_32.dll", "recv");
	CheckNotPatched("ws2_32.dll", "send");
	CheckNotPatched("ws2_32.dll", "closesocket");
	CheckNotPatched("ws2_32.dll", "WSAAsyncSelect");
	CheckNotPatched("kernel32.dll", "GetTickCount");
	CheckNotPatched("kernel32.dll", "QueryPerformanceCounter");
	CheckNotPatched("kernel32.dll", "QueryPerformanceFrequency");
	CheckNotPatched("winmm.dll", "timeGetTime");
	CheckNotPatched("user32.dll", "SetTimer");
	
	CheckForIATHook("ws2_32.dll", "connect");
	CheckForIATHook("ws2_32.dll", "recv");
	CheckForIATHook("ws2_32.dll", "send");
	CheckForIATHook("ws2_32.dll", "closesocket");
	CheckForIATHook("ws2_32.dll", "WSAAsyncSelect");
	CheckForIATHook("kernel32.dll", "GetTickCount");
	CheckForIATHook("kernel32.dll", "QueryPerformanceCounter");
	CheckForIATHook("kernel32.dll", "QueryPerformanceFrequency");
	CheckForIATHook("winmm.dll", "timeGetTime");
	CheckForIATHook("user32.dll", "SetTimer");

	if (WaitForSingleObject(m_hBypassEvent, 0) == WAIT_TIMEOUT)
	{
		CheckNotPatched("user32.dll", "GetAsyncKeyState");
		CheckNotPatched("user32.dll", "GetForegroundWindow");
		CheckNotPatched("user32.dll", "GetActiveWindow");

		CheckForIATHook("user32.dll", "GetAsyncKeyState");
		CheckForIATHook("user32.dll", "GetForegroundWindow");
		CheckForIATHook("user32.dll", "GetActiveWindow");
	}

	static int counter = 0;

	if ((counter = counter++ % 5) == 0)
	{
		EnumWindows(&CheckForHastyProc, 0);
	}
}


/**
 * \brief 
 */
void CProxifier::CheckMyPatch(const char* szDll, const char* szFunc, DWORD_PTR dwHookProc)
{
	HMODULE hMod = GetModuleHandleA(szDll);

	if (!hMod)
	{
		CDebugOut::PrintAlways("[PATCH_CHECK] Cannot find module handle: %s", szDll);
		return;
	}

	BYTE* pFunc = (BYTE*)GetProcAddress(hMod, szFunc);

	if (!pFunc)
	{
		CDebugOut::PrintAlways("[PATCH_CHECK] Cannot get function pointer: %s, %s", szDll, szFunc);
		return;
	}

	// Follow JMP chains (E9 long JMP, EB short JMP) to find the actual patched point.
	// The patch format is: NOP (0x90) NOP (0x90) JMP rel32 (0xE9 xx xx xx xx)
	BYTE* pPatchPoint = pFunc;
	while (pPatchPoint[0] == 0xE9 || pPatchPoint[0] == 0xEB)
	{
		if (pPatchPoint[0] == 0xEB)
		{
			signed char offset = (signed char)pPatchPoint[1];
			pPatchPoint = pPatchPoint + 2 + offset;
		}
		else
		{
			break; // Found an E9 JMP - this could be our patch
		}
	}

	// Check for NOP NOP JMP pattern (our patch applies: 0x90 0x90 0xE9 rel32)
	if (pPatchPoint[0] == 0x90 && pPatchPoint[1] == 0x90 && pPatchPoint[2] == 0xE9)
	{
		// Verify the JMP target matches our hook function
		// Offset calculation: hookProc - (patchPoint + NOP_PREFIX_SIZE + JMP_INSN_SIZE)
		// where NOP_PREFIX_SIZE=2 (two 0x90 bytes) and JMP_INSN_SIZE=5 (0xE9 + 4-byte rel32)
		DWORD dwAddr = PtrToUlong(dwHookProc) - PtrToUlong(pPatchPoint) - 5 - 2;
		if (memcmp(pPatchPoint+3, &dwAddr, 4) != 0)
		{
			CDebugOut::PrintAlways("[PATCH_CHECK] Illegal function patch detected (%s => %s)! Patch target mismatch.", szDll, szFunc);
			CKillUtil::KillGame("CheckMyPatch: illegal function patch detected");
		}
	}
	else if (pPatchPoint[0] == 0xE9)
	{
		// Legacy check: direct E9 JMP at function start
		DWORD dwAddr = PtrToUlong(dwHookProc) - PtrToUlong(pPatchPoint) - 5;
		if (memcmp(pPatchPoint+1, &dwAddr, 4) != 0)
		{
			CDebugOut::PrintAlways("[PATCH_CHECK] Illegal function patch detected (%s => %s)! Patch target mismatch.", szDll, szFunc);
			CKillUtil::KillGame("CheckMyPatch: illegal function patch detected");
		}
	}
	else
	{
		CDebugOut::PrintAlways("[PATCH_CHECK] Function not patched properly (%s => %s)! Expected NOP-NOP-JMP or JMP at 0x%p, found 0x%02X 0x%02X 0x%02X",
			szDll, szFunc, pPatchPoint, pPatchPoint[0], pPatchPoint[1], pPatchPoint[2]);
		CKillUtil::KillGame("CheckMyPatch: function not patched properly");
	}

	if (((BYTE*)dwHookProc)[0] == 0xE9)
	{
		DWORD_PTR dwRelJump = 0;
		memcpy(&dwRelJump, (BYTE*)dwHookProc+1, 4);

		BYTE* pPatch = (BYTE*)ULongToPtr(PtrToUlong(dwRelJump) + PtrToUlong(dwHookProc) + 5);

		if (pPatch[0] == 0xE9)
		{
			CDebugOut::PrintAlways("[PATCH_CHECK] Illegal double-jump patch detected (%s => %s)!", szDll, szFunc);
			CKillUtil::KillGame("CheckMyPatch: illegal double-jump patch detected");
		}
	}
}


/**
 * \brief 
 */
void CProxifier::CheckNotPatched(const char* szDll, const char* szFunc)
{
	HMODULE hMod = GetModuleHandleA(szDll);

	if (!hMod)
	{
		CDebugOut::PrintAlways("[PATCH_CHECK] Cannot find module handle: %s", szDll);
		return;
	}

	BYTE* pFunc = (BYTE*)GetProcAddress(hMod, szFunc);

	if (!pFunc)
	{
		CDebugOut::PrintAlways("[PATCH_CHECK] Cannot get function pointer: %s, %s", szDll, szFunc);
		return;
	}

	// Follow legitimate OS-level JMP forwarding (E9 long JMP, EB short JMP)
	// to reach the real function body before checking for illegal patches.
	// Windows may use JMP stubs for hotpatching, CFG, or DLL forwarding.
	// Limit to 8 follows to prevent infinite loops from malformed JMP chains.
	static const int MAX_JMP_CHAIN_DEPTH = 8;
	BYTE* pRealFunc = pFunc;
	int maxFollows = MAX_JMP_CHAIN_DEPTH;
	while (maxFollows-- > 0 && (pRealFunc[0] == 0xE9 || pRealFunc[0] == 0xEB))
	{
		if (pRealFunc[0] == 0xEB)
		{
			signed char offset = (signed char)pRealFunc[1];
			BYTE* pNext = pRealFunc + 2 + offset;
			// Only follow if target is within a known module
			HMODULE hTargetMod = NULL;
			if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
								   GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
								   (LPCSTR)pNext, &hTargetMod) && hTargetMod)
			{
				pRealFunc = pNext;
			}
			else
			{
				break; // Target is not in a known module - suspicious
			}
		}
		else // 0xE9
		{
			DWORD_PTR dwRelAddr = 0;
			memcpy(&dwRelAddr, pRealFunc+1, 4);
			BYTE* pNext = (BYTE*)(dwRelAddr + PtrToUlong(pRealFunc) + 5);
			// Only follow if target is within a known module
			HMODULE hTargetMod = NULL;
			if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
								   GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
								   (LPCSTR)pNext, &hTargetMod) && hTargetMod)
			{
				pRealFunc = pNext;
			}
			else
			{
				break; // Target is not in a known module - suspicious
			}
		}
	}

	// Check the real function body (after following OS JMP stubs) for illegal patches
	if (pRealFunc[0] == 0xE9 || pRealFunc[0] == 0x90)
	{
		// Only flag if the target is NOT within a known system module
		HMODULE hTargetMod = NULL;
		bool bSuspicious = true;

		if (pRealFunc[0] == 0xE9)
		{
			DWORD_PTR dwRelAddr = 0;
			memcpy(&dwRelAddr, pRealFunc+1, 4);
			BYTE* pTarget = (BYTE*)(dwRelAddr + PtrToUlong(pRealFunc) + 5);
			if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
								   GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
								   (LPCSTR)pTarget, &hTargetMod) && hTargetMod)
			{
				bSuspicious = false; // JMP to a known module - likely OS-level forwarding
			}
		}

		if (bSuspicious)
		{
			CDebugOut::PrintAlways("[PATCH_CHECK] Illegal function patch detected on %s::%s! First byte: 0x%02X at 0x%p", szDll, szFunc, pRealFunc[0], pRealFunc);
			CKillUtil::KillGame("CheckNotPatched: illegal function patch detected");
		}
	}

}


/**
 * \brief 
 */
void CProxifier::CheckForIATHook(const char* szDll, const char* szFunc)
{
	PVOID pfnOrigAddr = GetProcAddress(GetModuleHandleA(szDll), szFunc);
	PVOID pfnIatAddr = 0;
	
	if (!CPEUtil::GetFunctionPtrFromIAT(GetModuleHandle(0), szDll, szFunc, &pfnIatAddr))
		return;

	if (pfnOrigAddr != pfnIatAddr)
	{
		CDebugOut::PrintAlways("[PATCH_CHECK] IAT hook detected on %s::%s! Original=0x%08X, IAT=0x%08X", szDll, szFunc, pfnOrigAddr, pfnIatAddr);
		CKillUtil::KillGame("CheckForIATHook: IAT hook detected");
	}
}


/**
 * \brief Check if ws2_32.dll is loaded from the game folder (another method of hacking)
 */
void CProxifier::CheckWinsockDir()
{
	TCHAR szPath1[_MAX_PATH+1] = {0};
	GetModuleFileName(GetModuleHandleA("ws2_32.dll"), szPath1, _MAX_PATH);
	for (int i=(int)_tcslen(szPath1)-1; i >= 0 && szPath1[i] != _T('\\'); szPath1[i--] = 0);

	TCHAR szPath2[_MAX_PATH+1] = {0};
	GetModuleFileName(GetModuleHandle(0), szPath2, _MAX_PATH);
	for (int i=(int)_tcslen(szPath2)-1; i >= 0 && szPath2[i] != _T('\\'); szPath2[i--] = 0);

	TCHAR szWorkDir[_MAX_PATH+1] = {0};
	GetCurrentDirectory(_MAX_PATH, szWorkDir);
	PathAddBackslash(szWorkDir);

	if (_tcsicmp(szPath1, szPath2) == 0 || _tcsicmp(szPath1, szWorkDir) == 0)
	{
		// NOTE: Previously called CKillUtil::KillGame() here which terminated
		// main.exe. Changed to warning because some MU Online server setups
		// legitimately place ws2_32.dll in the game folder, and this check was
		// causing the game to close unexpectedly on startup.
		// SECURITY: In production anti-cheat deployments, consider re-enabling
		// this check with additional integrity verification of ws2_32.dll.
		CDebugOut::PrintAlways("[WARNING] ws2_32.dll loaded from game folder - ignoring to keep game running.");
		CDebugOut::PrintAlways("[WARNING] ws2_32.dll path: %s", szPath1);
		CDebugOut::PrintAlways("[WARNING] Game exe path: %s", szPath2);
	}
}


/**
 * \brief 
 */
BOOL CALLBACK CProxifier::CheckForHastyProc(HWND hwnd, LPARAM lParam)
{
	TCHAR szClass[_MAX_PATH+1];
	GetClassName(hwnd, szClass, _MAX_PATH);

	if (_tcsicmp(szClass, _T("#32770 (Dialog)")) != 0 && _tcsicmp(szClass, _T("#32770")) != 0 && _tcsicmp(szClass, _T("ConsoleWindowClass")) != 0)
		return TRUE;

	TCHAR szFile[_MAX_PATH+1] = {0};

	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, dwProcessId);
	GetModuleFileNameEx(hProcess, 0, szFile, _MAX_PATH);

	DWORD dwLen = 0, dwUseless = 0;
	LPTSTR lpVI = 0;

	if (0 == (dwLen = GetFileVersionInfoSize((LPTSTR)szFile, &dwUseless)))
		return TRUE;

	if (0 != (lpVI = (LPTSTR)GlobalAlloc(GPTR, dwLen)))
	{
		WORD* langInfo = 0;
		UINT cbLang = 0;
		TCHAR tszVerStrName[128] = {0};
		LPVOID lpt = 0;
		UINT cbBufSize = 0;

		GetFileVersionInfo((LPTSTR)szFile, NULL, dwLen, lpVI);

		//First, to get string information, we need to get language information.
		VerQueryValue(lpVI, _T("\\VarFileInfo\\Translation"), (LPVOID*)&langInfo, &cbLang);

		//Prepare the label -- default lang is bytes 0 & 1
		//of langInfo
		wsprintf(tszVerStrName, _T("\\StringFileInfo\\%04x%04x\\%s"), langInfo[0], langInfo[1], _T("ProductName"));

		//Get the string from the resource data
		if (VerQueryValue(lpVI, tszVerStrName, &lpt, &cbBufSize))
		{
			CString strProdName((LPTSTR)lpt);
			strProdName.MakeLower();

			if (strProdName.Find(_T("hastymu")) >= 0 || strProdName.Find(_T("hasty mu")) >= 0)
			{
				CDebugOut::PrintAlways("[PATCH_CHECK] HastyMU cheating software detected! Product: %s", (LPCSTR)CStringA(strProdName));
				CKillUtil::KillGame("CheckForHastyProc: HastyMU detected");
				return FALSE;
			}
		}

		GlobalFree((HGLOBAL)lpVI);
	}

	return TRUE;
}
