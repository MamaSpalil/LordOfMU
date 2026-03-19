/****************************************************************************/
/** intercept.cpp                                                          **/
/** ---------------------------------------------------------------------- **/
/** Example of interception of an API or any DLL function call             **/
/** ---------------------------------------------------------------------- **/
/** The method shown here may be very impressive in conjunction with       **/
/** CreateRemoteThread API                                                 **/
/** ---------------------------------------------------------------------- **/
/** July 23, 1998 by Oleg Kagan                                            **/
/****************************************************************************/

//============================================================================
#include "stdafx.h"

#pragma once

// Switch all optimizations off 
// (Visual C specific... For any other compiler do the same thing)
//============================================================================
#pragma optimize("", off)

//============================================================================
#define MakePtr(Type, Base, Offset) ((Type)(DWORD_PTR(Base) + (DWORD_PTR)(Offset)))

//============================================================================
BOOL InterceptDllCall(
	HMODULE hLocalModule, 
	const char* c_szDllName,
	const char* c_szApiName,
	PVOID pApiNew,
	PVOID* p_pApiOrg,
	PVOID pApiToChange)
{
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hLocalModule;
    PIMAGE_NT_HEADERS pNTHeader;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    DWORD dwProtect;
	BOOL bSuccess = FALSE;
    
    DWORD_PTR dwAddressToIntercept; 

	if (pApiToChange) {
		dwAddressToIntercept = (DWORD_PTR)pApiToChange;
	}
	else {
		dwAddressToIntercept = (DWORD_PTR)GetProcAddress(
			GetModuleHandleA((char*)c_szDllName), (char*)c_szApiName
		) /*GetProcAddress*/;
	} /*iff*/;

    if (IsBadReadPtr(hLocalModule, sizeof(PIMAGE_NT_HEADERS)))
	{
        return FALSE;
	}
    
    if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
        return FALSE;
	}
    
    pNTHeader = MakePtr(PIMAGE_NT_HEADERS, pDOSHeader, pDOSHeader->e_lfanew);
    if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
        return FALSE;
	}

    pImportDesc = MakePtr(
		PIMAGE_IMPORT_DESCRIPTOR, hLocalModule, 
        pNTHeader->OptionalHeader.DataDirectory[
			IMAGE_DIRECTORY_ENTRY_IMPORT
		] /*pNTHeader->OptionalHeader.DataDirectory*/.VirtualAddress
	) /*MakePtr*/;
    
    if (pImportDesc == (PIMAGE_IMPORT_DESCRIPTOR)pNTHeader) 
	{
		return FALSE;
	}
    
	while (pImportDesc->Name) {
		PIMAGE_THUNK_DATA pThunk;
		    
		pThunk = MakePtr(
			PIMAGE_THUNK_DATA, hLocalModule, pImportDesc->FirstThunk
		) /*MakePtr*/;
    
		while (pThunk->u1.Function) {

			if ((DWORD)pThunk->u1.Function == dwAddressToIntercept) {
				if (
					!IsBadWritePtr(
						(PVOID)&pThunk->u1.Function, sizeof(PVOID)
					) /*!IsBadWritePtr*/
				){
					if (p_pApiOrg) 
						*p_pApiOrg = (PVOID)UlongToPtr(pThunk->u1.Function);
					pThunk->u1.Function = PtrToUlong(pApiNew);
					bSuccess = TRUE;
				}
				else 
				{
					if (
						VirtualProtect(
							(PVOID)&pThunk->u1.Function, sizeof(PVOID),
							PAGE_EXECUTE_READWRITE, &dwProtect
						) /*VirtualProtect*/
					){
						DWORD dwNewProtect;

						if (p_pApiOrg) 
							*p_pApiOrg = UlongToPtr(pThunk->u1.Function);
						pThunk->u1.Function = PtrToUlong(pApiNew);
						bSuccess = TRUE;

						dwNewProtect = dwProtect;
						VirtualProtect(
							(PVOID)&pThunk->u1.Function, sizeof(PVOID),
							dwNewProtect, &dwProtect
						) /*VirtualProtect*/;
					} /*if*/
				} /*iff*/
			} /*if*/
			pThunk++;
		} /*while*/
		pImportDesc++;
	} /*while*/

    return bSuccess;
} /*InterceptDllCall(HMODULE, const char*, const char*, PVOID,PVOID*,PVOID)*/



/**
 * \brief This one is better than InterceptDllCall()
 */
PVOID HookFunction(HMODULE hTargetModule, LPCSTR pImportModuleName, LPCSTR pFunctionSymbol, DWORD dwHookFunction )
{
	if ( ! hTargetModule ) { return 0; } // If the target module is invalid return unsuccessfully.

	bool bResult = false;
	PDWORD pIAT, pINT;
	DWORD nIndex, dwBackup, dwOriginalFunction;

	PIMAGE_DATA_DIRECTORY pDataDirectory;
	PIMAGE_DOS_HEADER pDOSHeader;
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor;
	PIMAGE_IMPORT_BY_NAME pImportName;
	PIMAGE_OPTIONAL_HEADER pOptionalHeader;
	PIMAGE_NT_HEADERS pPEHeader;

	pDOSHeader = ( PIMAGE_DOS_HEADER ) hTargetModule; // The DOS header is held over in PE files for legacy support of DOS systems. The result of this is, that if run in DOS, instead of crashing the system it will print an interesting message ;)
	pPEHeader = ( PIMAGE_NT_HEADERS ) ( (BYTE*)hTargetModule + pDOSHeader->e_lfanew );
	pOptionalHeader = & pPEHeader->OptionalHeader;
	pDataDirectory = pOptionalHeader->DataDirectory;
	pImportDescriptor = ( PIMAGE_IMPORT_DESCRIPTOR ) ( (BYTE*)hTargetModule + pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );

	// This loop locates the import descriptor for the module from which are hooking an IAT entry.
	for ( nIndex = 0; nIndex < pDataDirectory [ IMAGE_DIRECTORY_ENTRY_IMPORT ].Size; nIndex++ )
	{
		if ( _stricmp((LPCSTR)((BYTE*)hTargetModule + pImportDescriptor[nIndex].Name), pImportModuleName) == 0)
		{
			bResult = true; break;
		}
	}

	if ( ! bResult ) { return 0; } bResult = false; // If no module was found return unsuccessfully, otherwise continue.

	pIAT = ( PDWORD ) ( (BYTE*)hTargetModule + ( pImportDescriptor [ nIndex ].FirstThunk ) ); // This is the pointer to the IAT.
	pINT = ( PDWORD ) ( (BYTE*)hTargetModule + ( pImportDescriptor [ nIndex ].OriginalFirstThunk ) ); // This is the pointer to the import name table, which contains the symbols/ordinals the IAT addresses were resolved from.

	// This loop locates the index to the IAT and INT that is associated with the symbol of the function we would like to hook.
	for ( nIndex = 0; pINT [ nIndex ] != 0; nIndex++ )
	{
		pImportName = ( PIMAGE_IMPORT_BY_NAME ) ( (BYTE*)hTargetModule + ( pINT [ nIndex ] ) );
		if ( _stricmp ( ( LPCSTR ) pImportName->Name, pFunctionSymbol ) == 0) 
		{
			bResult = true; break;
		}
	}

	if ( ! bResult ) { return false; } bResult = false; // If no index was found, return unsucessfully.

	// We have located the IAT entry, we will now replace the address with our own.
	VirtualProtect ( & pIAT [ nIndex ], sizeof ( DWORD ), PAGE_READWRITE, & dwBackup ); // Virtual protect ensures that the page of memory we are writing to is not set to read only.

	dwOriginalFunction = pIAT [ nIndex ]; // Save the old functions address.
	pIAT [ nIndex ] = dwHookFunction; // Hook the IAT entry!

	VirtualProtect ( & pIAT [ nIndex ], sizeof ( DWORD ), dwBackup, & nIndex ); // Undo the VM protections.

	return ( PVOID ) ULongToPtr(dwOriginalFunction); // Return the original functions address.
}
