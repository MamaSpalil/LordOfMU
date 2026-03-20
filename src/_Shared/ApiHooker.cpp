#include "stdafx.h"
#include "ApiHooker.h"
#include "Disasm.h"
#include "stdlib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**
 * \brief 
 */
CApiHooker::CApiHooker()
{
	m_pfnApi = 0;
	m_dwPatchSize = 0;
	m_fPatched = false;
	memset(m_abTrampBuff, 0, sizeof(m_abTrampBuff));
}


/**
 * \brief 
 */
bool CApiHooker::PatchFunction(const char* szDll, const char* szFunc, DWORD_PTR dwHookProc)
{
	HMODULE hMod = GetModuleHandleA(szDll);

	if (!hMod)
		return false;

	BYTE* pFunc = (BYTE*)GetProcAddress(hMod, szFunc);

	if (!pFunc)
		return false;

	return PatchFunction(pFunc, dwHookProc);
}


/**
 * \brief 
 */
bool CApiHooker::UnPatchFunction(const char* szDll, const char* szFunc)
{
	HMODULE hMod = GetModuleHandleA(szDll);

	if (!hMod)
		return false;

	BYTE* pFunc = (BYTE*)GetProcAddress(hMod, szFunc);

	if (!pFunc)
		return false;

	return UnPatchFunction(pFunc);
}


/**
 * \brief 
 */
BYTE* CApiHooker::FindPatchPoint(BYTE* pFuncAddress, DWORD_PTR dwHookProc)
{
	BYTE* pFunc = pFuncAddress;

	while (pFunc[0] == 0xE9 || pFunc[0] == 0xEB)
	{
		if (pFunc[0] == 0xEB)
		{
			// Short JMP (rel8): follow the 1-byte signed offset
			signed char offset = (signed char)pFunc[1];
			pFunc = pFunc + 2 + offset;
		}
		else
		{
			DWORD_PTR dwRelAddr = 0;
			memcpy(&dwRelAddr, pFunc+1, 4);

			DWORD_PTR dwCurrHook = dwRelAddr + PtrToUlong(pFunc) + 5;

			if (dwCurrHook == dwHookProc)
				return 0;

			pFunc = (BYTE*)dwCurrHook;
		}
	}

	return pFunc;
}


/**
 * \brief 
 */
BYTE* CApiHooker::FindPatchedAddress(BYTE* pFuncAddress, DWORD_PTR dwHookProc)
{
	BYTE* pFunc = pFuncAddress;

	while (pFunc[0] == 0xE9 || pFunc[0] == 0xEB)
	{
		if (pFunc[0] == 0xEB)
		{
			// Short JMP (rel8): follow the 1-byte signed offset
			signed char offset = (signed char)pFunc[1];
			pFunc = pFunc + 2 + offset;
		}
		else
		{
			DWORD_PTR dwRelAddr = 0;
			memcpy(&dwRelAddr, pFunc+1, 4);

			DWORD_PTR dwCurrHook = dwRelAddr + PtrToUlong(pFunc) + 5;

			if (dwCurrHook == dwHookProc)
				return pFunc;

			pFunc = (BYTE*)dwCurrHook;
		}
	}

	return 0;
}


/**
 * \brief 
 */
bool CApiHooker::PatchFunction(BYTE* pFunc, DWORD_PTR dwHookProc)
{
	if (0 == (pFunc = FindPatchPoint(pFunc, dwHookProc)))
		return true;

	ULONG ulSizeToProtect = 5+2;
	DWORD dwIdx = 0;
	DISASSEMBLY disasm = {0};
	disasm.Address = PtrToUlong(pFunc);

	FlushDecoded(&disasm);

//	for (dwIdx = 0; dwIdx <= ulSizeToProtect; dwIdx++)
	for (; dwIdx <= ulSizeToProtect; )
	{
		dwIdx = 0;
		Decode(&disasm, (char*)UlongToPtr(disasm.Address), &dwIdx);
		disasm.Address += disasm.OpcodeSize + disasm.PrefixSize;

		dwIdx = disasm.Address - PtrToUlong(pFunc);
		FlushDecoded(&disasm);
	}

	m_dwPatchSize = dwIdx;

	DWORD dwAddr = PtrToUlong(dwHookProc) - PtrToUlong(pFunc) - 5 - 2;
	DWORD dwAddrBack = (PtrToUlong(pFunc) + dwIdx) - (PtrToUlong(m_abTrampBuff) + dwIdx) - 5;
	DWORD dwProtect = 0;

	memcpy(m_abTrampBuff, pFunc, dwIdx);

	// Fix up relative CALL (E8) and JMP (E9) instructions in the trampoline.
	// When original code is copied to the trampoline buffer, relative offsets
	// become invalid because the code is now at a different address.
	// Adjustment: new_offset = old_offset + (original_addr - trampoline_addr)
	{
		DWORD_PTR dwDisplacement = PtrToUlong(pFunc) - PtrToUlong(m_abTrampBuff);
		DISASSEMBLY fixupDisasm = {0};
		fixupDisasm.Address = PtrToUlong(pFunc);
		FlushDecoded(&fixupDisasm);

		DWORD dwPos = 0;
		while (dwPos < dwIdx)
		{
			DWORD dwTmp = 0;
			Decode(&fixupDisasm, (char*)UlongToPtr(fixupDisasm.Address), &dwTmp);
			DWORD dwInsnSize = fixupDisasm.OpcodeSize + fixupDisasm.PrefixSize;
			BYTE opcode = pFunc[dwPos + fixupDisasm.PrefixSize];

			// E8 = CALL rel32, E9 = JMP rel32 (opcode + 4-byte offset = 5 bytes)
			if ((opcode == 0xE8 || opcode == 0xE9) && dwInsnSize == 5
				&& dwPos + fixupDisasm.PrefixSize + 5 <= sizeof(m_abTrampBuff))
			{
				DWORD dwOldRel = 0;
				memcpy(&dwOldRel, &m_abTrampBuff[dwPos + fixupDisasm.PrefixSize + 1], 4);
				DWORD dwNewRel = dwOldRel + (DWORD)dwDisplacement;
				memcpy(&m_abTrampBuff[dwPos + fixupDisasm.PrefixSize + 1], &dwNewRel, 4);
			}
			// 0F 80-8F = conditional JMP rel32 (2-byte opcode + 4-byte offset = 6 bytes)
			else if (opcode == 0x0F && dwInsnSize == 6
				&& dwPos + fixupDisasm.PrefixSize + 6 <= sizeof(m_abTrampBuff)
				&& dwPos + fixupDisasm.PrefixSize + 1 < dwIdx)
			{
				BYTE opcode2 = pFunc[dwPos + fixupDisasm.PrefixSize + 1];
				if (opcode2 >= 0x80 && opcode2 <= 0x8F)
				{
					DWORD dwOldRel = 0;
					memcpy(&dwOldRel, &m_abTrampBuff[dwPos + fixupDisasm.PrefixSize + 2], 4);
					DWORD dwNewRel = dwOldRel + (DWORD)dwDisplacement;
					memcpy(&m_abTrampBuff[dwPos + fixupDisasm.PrefixSize + 2], &dwNewRel, 4);
				}
			}

			fixupDisasm.Address += dwInsnSize;
			dwPos += dwInsnSize;
			FlushDecoded(&fixupDisasm);
		}
	}

	m_abTrampBuff[dwIdx] = 0xE9;
	memcpy(m_abTrampBuff + dwIdx + 1, &dwAddrBack, 4);

	if (!::VirtualProtect((PVOID)pFunc, dwIdx, PAGE_EXECUTE_READWRITE, &dwProtect))
		return false;


	DWORD dwNewProtect = dwProtect;

	pFunc[0] = 0x90; 
	pFunc[1] = 0x90; 
	pFunc[2] = 0xE9; 
	memcpy(pFunc+3, &dwAddr, 4);

	m_dwHookProc = dwHookProc;
	m_fPatched = true;

	::VirtualProtect((PVOID)pFunc, 5, dwNewProtect, &dwProtect);

	::VirtualProtect((PVOID)m_abTrampBuff, 64, PAGE_EXECUTE_READWRITE, &dwProtect);
	return true;
}


/**
 * \brief 
 */
bool CApiHooker::UnPatchFunction(BYTE* pFunc)
{
	DWORD dwProtect = 0;
	DWORD dwAddr = PtrToUlong(m_abTrampBuff) - PtrToUlong(pFunc) - 5;

	if (m_abTrampBuff[0] != 0 && m_abTrampBuff[0] != 0xE9 && pFunc[0] == 0xE9 && m_dwPatchSize != 0)
	{
		if (0 != (pFunc = FindPatchedAddress(pFunc, m_dwHookProc)))
		{
			if (!::VirtualProtect((PVOID)pFunc, m_dwPatchSize, PAGE_EXECUTE_READWRITE, &dwProtect))
				return false;

			DWORD dwNewProtect = dwProtect;
			memcpy(pFunc, m_abTrampBuff, m_dwPatchSize);

			::VirtualProtect((PVOID)pFunc, m_dwPatchSize, dwNewProtect, &dwProtect);
		}
	}

	// Set out trampoline jump directly to original function
	::VirtualProtect((PVOID)m_abTrampBuff, 64, PAGE_EXECUTE_READWRITE, &dwProtect);
	memset(m_abTrampBuff, 0, sizeof(m_abTrampBuff));

	m_abTrampBuff[0] = 0xE9;
	memcpy(m_abTrampBuff+1, &dwAddr, 4);

	m_dwHookProc = 0;
	m_dwPatchSize = 0;
	m_fPatched = false;
	return true;
}



/**
 * \brief 
 */
CApiDllMirror::CApiDllMirror(const char* pszDllName)
{
	m_hDll = 0;

	if (pszDllName)
	{
		char szMirrorPath[_MAX_PATH+1] = {0};
		GetTempPathA(_MAX_PATH, szMirrorPath);

		int iLen = (int)strlen(szMirrorPath);

		if (iLen > 0 && szMirrorPath[iLen-1] != '\\')
			szMirrorPath[iLen] = '\\';

		strcat(szMirrorPath, "Mirror_");
		strcat(szMirrorPath, pszDllName);

		char szOrigPath[_MAX_PATH+1] = {0};
		GetSystemDirectoryA(szOrigPath, _MAX_PATH);

		iLen = (int)strlen(szOrigPath);

		if (iLen > 0 && szOrigPath[iLen-1] != '\\')
			szOrigPath[iLen] = '\\';

		strcat(szOrigPath, pszDllName);

		CopyFileA(szOrigPath, szMirrorPath, false);
		m_hDll = LoadLibraryA(szMirrorPath);
	}
}


/**
 * \brief 
 */
CApiDllMirror::~CApiDllMirror()
{
	if (m_hDll)
		FreeLibrary(m_hDll);
}


/**
 * \brief 
 */
void* CApiDllMirror::GetProcAddress(const char* pszProc)
{
	if (!pszProc || !m_hDll)
		return 0;

	return ::GetProcAddress(m_hDll, pszProc);
}


/**
 * \brief 
 */
CWinsockApiMirror::CWinsockApiMirror()
	: CApiDllMirror("ws2_32.dll")
{
	if (m_hDll)
	{
		int (WINAPI* WSAStartupFn)(WORD, LPWSADATA) = 0;
		
		WSAStartupFn = (int (WINAPI*)(WORD, LPWSADATA))::GetProcAddress(m_hDll, "WSAStartup");

		if (WSAStartupFn)
		{
			WSADATA wsa = {0};
			WSAStartupFn(MAKEWORD(2,2), &wsa);
		}
	}
}


/**
 * \brief 
 */
CWinsockApiMirror::~CWinsockApiMirror()
{
	if (m_hDll)
	{
		int (WINAPI* WSACleanupFn)(void) = 0;

		WSACleanupFn = (int (WINAPI*)(void))::GetProcAddress(m_hDll, "WSACleanup");

		if (WSACleanupFn)
		{
			WSACleanupFn();
		}
	}
}

