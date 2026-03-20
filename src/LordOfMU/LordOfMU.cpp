// LordOfMU.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "MuInstanceManager.h"
#include <VersionHelpers.h>


bool IsVista();
void RunNormal();

/**  
 * \brief 
 */
class CLordOfMUModule : public CAtlExeModuleT< CLordOfMUModule >
{
public:
	CLordOfMUModule() : hMutex(0), hModule(0) {}

	bool Is_x64OS()
	{
		SYSTEM_INFO SystemInfo = {0};
		SystemInfo.wProcessorArchitecture = 0xffff;
		
		GetNativeSystemInfo(&SystemInfo);

		switch (SystemInfo.wProcessorArchitecture)
		{
		case 9: // PROCESSOR_ARCHITECTURE_AMD64; x64 (AMD or Intel)
		case 6: // PROCESSOR_ARCHITECTURE_IA64; Intel Itanium Processor Family (IPF)
			return true;
		case 0: // PROCESSOR_ARCHITECTURE_INTEL; x86
		default:
			return false;
		}

		return false;
	}

	HMODULE hModule;
	HANDLE hMutex;
	CMuInstanceManager cInstManager;
};

CLordOfMUModule _AtlModule;



/**  
 * \brief 
 */
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	if (IsVista() || _AtlModule.Is_x64OS())
	{
		TCHAR szDir[_MAX_PATH+1] = {0};
		GetModuleFileName(0, szDir, _MAX_PATH);
		for (int i=(int)_tcslen(szDir)-1; i >= 0 && szDir[i] != _T('\\'); szDir[i--] = 0);

		TCHAR szRdllDir[_MAX_PATH+1] = {0};
		GetWindowsDirectory(szRdllDir, _MAX_PATH);
		PathAddBackslash(szRdllDir);

		if (_AtlModule.Is_x64OS())
			_tcscat(szRdllDir, _T("SysWOW64\\rundll32.exe"));
		else
			_tcscat(szRdllDir, _T("system32\\rundll32.exe"));


		SHELLEXECUTEINFO shexecnfo = {0};
		shexecnfo.cbSize = sizeof(SHELLEXECUTEINFO);
		shexecnfo.lpVerb = _T("open");
		shexecnfo.lpFile = szRdllDir;
		shexecnfo.lpParameters = _T("Bootstrapper.dll,Load2");
		shexecnfo.lpDirectory = szDir;
		shexecnfo.nShow = SW_SHOWNORMAL;
		
		ShellExecuteEx(&shexecnfo);
	}
	else
	{

		HMODULE hLib = LoadLibrary(_T("Bootstrapper.dll"));

		if (!hLib)
			return -1;

		bool (*LoadProc)() = (bool(*)()) GetProcAddress(hLib, "Load");

		if (LoadProc)
			LoadProc();

		FreeLibrary(hLib);
	}

	return 0;
}


/**  
 * \brief 
 */
bool IsVista()
{
	return IsWindowsVistaOrGreater();
}


/**  
 * \brief 
 */
void RunNormal()
{
	_AtlModule.hMutex = CreateMutex(0, 0, _T("__LordOfMU__"));

	if (GetLastError() == ERROR_ALREADY_EXISTS || !_AtlModule.cInstManager.Init(false))
	{
		CloseHandle(_AtlModule.hMutex);
	}
	else
	{
		TCHAR szLibName[_MAX_PATH+1] = {0};
		GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szLibName, _MAX_PATH);

		LoadLibrary(szLibName);
	}

	_AtlModule.RunMessageLoop();

	_AtlModule.cInstManager.Term();
	CloseHandle(_AtlModule.hMutex);
}