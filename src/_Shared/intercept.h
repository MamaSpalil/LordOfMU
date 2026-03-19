#ifndef __INTERCEPT_H
#define __INTERCEPT_H

#pragma once

BOOL InterceptDllCall(
	HMODULE hLocalModule, 
	const char* c_szDllName,
	const char* c_szApiName,
	PVOID pApiNew,
	PVOID* p_pApiOrg,
	PVOID pApiToChange	
);

PVOID HookFunction(HMODULE hTargetModule, LPCSTR pImportModuleName, LPCSTR pFunctionSymbol, DWORD dwHookFunction );

#endif //__INTERCEPT_H