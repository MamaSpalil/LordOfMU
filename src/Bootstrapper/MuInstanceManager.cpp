#include "stdafx.h"
#include "MuInstanceManager.h"
#include "version.h"
#include "DebugMode.h"
#include "ClickerLogger.h"

#include <string>
#include <vector>
using namespace std;


UINT_PTR g_uTimer = 0;


/**
 * \brief 
 */
static VOID CALLBACK DelayedTerm(HWND, UINT, UINT_PTR, DWORD)
{
	KillTimer(0, g_uTimer);

	TCHAR szMyName[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szMyName, _MAX_PATH);

	TCHAR* p = szMyName;
	for (int i=(int)_tcslen(szMyName)-1; i >= 0 && szMyName[i] != _T('\\'); p = &szMyName[i--]);

	FreeLibrary(GetModuleHandle(p));
}



/**
 * \brief Default IP hardcoded in original Main.dll binary.
 */
static const char s_szDefaultIP[] = "192.168.0.105";

/**
 * \brief Patch size for the IP field in Main.dll (16-byte slot: IP + null padding).
 */
static const size_t s_cbPatchSize = 16;


/**
 * \brief Validate that a string is a valid IPv4 address (digits and dots, 4 octets 0-255).
 */
static bool IsValidIPv4(const char* szIP)
{
	if (!szIP || szIP[0] == '\0')
		return false;

	int nOctets = 0;
	int nDigits = 0;
	int nValue = 0;

	for (const char* p = szIP; ; p++)
	{
		if (*p >= '0' && *p <= '9')
		{
			nValue = nValue * 10 + (*p - '0');
			nDigits++;
			if (nDigits > 3 || nValue > 255)
				return false;
		}
		else if (*p == '.' || *p == '\0')
		{
			if (nDigits == 0)
				return false;
			nOctets++;
			nDigits = 0;
			nValue = 0;
			if (*p == '\0')
				break;
		}
		else
		{
			return false;
		}
	}

	return (nOctets == 4);
}


/**
 * \brief Patch Main.dll file on disk with the IP address from Connect.ini.
 *
 *        This function modifies the actual Main.dll binary file before main.exe
 *        is launched, ensuring the game client connects to the correct server.
 *
 *        Flow:
 *        1. Reads the target IP from Connect.ini [Config] IP
 *        2. Reads the last patched IP from Connect.ini [Config] LastPatchedIP
 *           (defaults to "192.168.0.105" — the original hardcoded IP)
 *        3. Opens Main.dll as binary and searches for the last known IP
 *        4. If not found, falls back to searching for the default "192.168.0.105"
 *        5. Replaces the found IP with the new IP (zero-padded to 16 bytes)
 *        6. Writes the modified Main.dll back to disk
 *        7. Updates [Config] LastPatchedIP in Connect.ini
 *
 * \param szPath  Game directory path (with trailing backslash)
 */
static void PatchMainDllFileIP(const TCHAR* szPath)
{
	// Build paths to Connect.ini and Main.dll
	TCHAR szIniPath[_MAX_PATH + 1] = {0};
	_tcscpy(szIniPath, szPath);
	_tcscat(szIniPath, _T("Connect.ini"));

	TCHAR szDllPath[_MAX_PATH + 1] = {0};
	_tcscpy(szDllPath, szPath);
	_tcscat(szDllPath, _T("Main.dll"));

	// Check if Main.dll exists
	struct _stat stDll = {0};
	if (0 != _tstat(szDllPath, &stDll))
	{
		CT2A aDllPath(szDllPath);
		WriteHookLog("[FILE_PATCH] Main.dll not found at %s, skipping file patch", (LPCSTR)aDllPath);
		CDebugMode::LogDebugAction("[FILE_PATCH] Main.dll not found, skipping");
		return;
	}

	// Check if Connect.ini exists
	struct _stat stIni = {0};
	if (0 != _tstat(szIniPath, &stIni))
	{
		CT2A aIniPath(szIniPath);
		WriteHookLog("[FILE_PATCH] Connect.ini not found at %s, skipping file patch", (LPCSTR)aIniPath);
		CDebugMode::LogDebugAction("[FILE_PATCH] Connect.ini not found, skipping");
		return;
	}

	// Read target IP from Connect.ini
	char szNewIP[64] = {0};
	{
		CT2A aIniPath(szIniPath);
		GetPrivateProfileStringA("Config", "IP", "", szNewIP, sizeof(szNewIP), (LPCSTR)aIniPath);
	}

	if (szNewIP[0] == '\0')
	{
		WriteHookLog("[FILE_PATCH] No IP in Connect.ini, skipping file patch");
		CDebugMode::LogDebugAction("[FILE_PATCH] Connect.ini IP is empty, skipping");
		return;
	}

	// Validate the new IP
	if (!IsValidIPv4(szNewIP))
	{
		WriteHookLog("[FILE_PATCH] Invalid IP address in Connect.ini: '%s'", szNewIP);
		CDebugMode::LogDebugAction("[FILE_PATCH] Invalid IP in Connect.ini: %s", szNewIP);
		return;
	}

	// Validate new IP length fits in the 16-byte slot
	size_t cbNewIP = strlen(szNewIP);
	if (cbNewIP + 1 > s_cbPatchSize)
	{
		WriteHookLog("[FILE_PATCH] IP '%s' too long (max %d chars)", szNewIP, (int)(s_cbPatchSize - 1));
		CDebugMode::LogDebugAction("[FILE_PATCH] IP too long: %s", szNewIP);
		return;
	}

	// Read the last patched IP (what's currently in Main.dll) from Connect.ini
	// Default is the original hardcoded "192.168.0.105"
	char szLastPatchedIP[64] = {0};
	{
		CT2A aIniPath(szIniPath);
		GetPrivateProfileStringA("Config", "LastPatchedIP", s_szDefaultIP,
			szLastPatchedIP, sizeof(szLastPatchedIP), (LPCSTR)aIniPath);
	}

	// If IPs already match, no patch needed
	if (strcmp(szNewIP, szLastPatchedIP) == 0)
	{
		WriteHookLog("[FILE_PATCH] Main.dll already has IP %s, no patch needed", szNewIP);
		CDebugMode::LogDebugAction("[FILE_PATCH] IP already matches: %s", szNewIP);
		return;
	}

	WriteHookLog("[FILE_PATCH] Patching Main.dll: '%s' -> '%s'", szLastPatchedIP, szNewIP);
	CDebugMode::LogDebugAction("[FILE_PATCH] Patching Main.dll file: %s -> %s", szLastPatchedIP, szNewIP);

	// Read Main.dll into memory
	HANDLE hFile = CreateFile(szDllPath, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		WriteHookLog("[FILE_PATCH] Failed to open Main.dll for reading (error %d)", (int)GetLastError());
		CDebugMode::LogDebugAction("[FILE_PATCH] Cannot open Main.dll for reading");
		return;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE || dwFileSize == 0)
	{
		CloseHandle(hFile);
		WriteHookLog("[FILE_PATCH] Invalid Main.dll file size");
		return;
	}

	std::vector<BYTE> vBuffer(dwFileSize);
	DWORD dwBytesRead = 0;
	if (!ReadFile(hFile, &vBuffer[0], dwFileSize, &dwBytesRead, NULL) || dwBytesRead != dwFileSize)
	{
		CloseHandle(hFile);
		WriteHookLog("[FILE_PATCH] Failed to read Main.dll (error %d)", (int)GetLastError());
		return;
	}
	CloseHandle(hFile);

	// Search for the last known IP in the binary
	size_t cbSearchIP = strlen(szLastPatchedIP) + 1; // include null terminator
	BYTE* pFound = NULL;

	if (dwFileSize >= cbSearchIP)
	{
		for (DWORD i = 0; i <= dwFileSize - cbSearchIP; i++)
		{
			if (memcmp(&vBuffer[i], szLastPatchedIP, cbSearchIP) == 0)
			{
				pFound = &vBuffer[i];
				WriteHookLog("[FILE_PATCH] Found IP '%s' at offset 0x%X", szLastPatchedIP, i);
				break;
			}
		}
	}

	// Fallback: if lastPatchedIP was not found and it's not the default, try the default
	if (!pFound && strcmp(szLastPatchedIP, s_szDefaultIP) != 0)
	{
		WriteHookLog("[FILE_PATCH] LastPatchedIP '%s' not found, trying default '%s'",
			szLastPatchedIP, s_szDefaultIP);

		size_t cbDefault = strlen(s_szDefaultIP) + 1;
		if (dwFileSize >= cbDefault)
		{
			for (DWORD i = 0; i <= dwFileSize - cbDefault; i++)
			{
				if (memcmp(&vBuffer[i], s_szDefaultIP, cbDefault) == 0)
				{
					pFound = &vBuffer[i];
					WriteHookLog("[FILE_PATCH] Found default IP '%s' at offset 0x%X", s_szDefaultIP, i);
					break;
				}
			}
		}
	}

	if (!pFound)
	{
		WriteHookLog("[FILE_PATCH] Could not find IP pattern in Main.dll, skipping patch");
		CDebugMode::LogDebugAction("[FILE_PATCH] IP pattern not found in Main.dll");
		return;
	}

	// Patch the IP in the buffer: zero-fill the 16-byte slot, then write new IP
	size_t nOffset = pFound - &vBuffer[0];

	// Safety check: ensure we don't write beyond the file
	if (nOffset + s_cbPatchSize > dwFileSize)
	{
		WriteHookLog("[FILE_PATCH] Not enough space at offset 0x%X to patch (file size 0x%X)",
			(DWORD)nOffset, dwFileSize);
		return;
	}

	memset(pFound, 0, s_cbPatchSize);
	memcpy(pFound, szNewIP, cbNewIP);

	// Write only the patched 16-byte IP field back to Main.dll at the found offset.
	// Using OPEN_EXISTING with SetFilePointer to modify only the specific bytes,
	// avoiding risk of file corruption from rewriting the entire file.
	hFile = CreateFile(szDllPath, GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		WriteHookLog("[FILE_PATCH] Failed to open Main.dll for writing (error %d)", (int)GetLastError());
		CDebugMode::LogDebugAction("[FILE_PATCH] Cannot open Main.dll for writing");
		return;
	}

	if (SetFilePointer(hFile, (LONG)nOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		CloseHandle(hFile);
		WriteHookLog("[FILE_PATCH] Failed to seek in Main.dll to offset 0x%X (error %d)",
			(DWORD)nOffset, (int)GetLastError());
		return;
	}

	DWORD dwBytesWritten = 0;
	BOOL bWriteOK = WriteFile(hFile, pFound, (DWORD)s_cbPatchSize, &dwBytesWritten, NULL);
	CloseHandle(hFile);

	if (!bWriteOK || dwBytesWritten != (DWORD)s_cbPatchSize)
	{
		WriteHookLog("[FILE_PATCH] Failed to write Main.dll (error %d)", (int)GetLastError());
		CDebugMode::LogDebugAction("[FILE_PATCH] Failed to write patched Main.dll");
		return;
	}

	// Update LastPatchedIP in Connect.ini so next run knows what IP is in Main.dll
	{
		CT2A aIniPath(szIniPath);
		WritePrivateProfileStringA("Config", "LastPatchedIP", szNewIP, (LPCSTR)aIniPath);
	}

	WriteHookLog("[FILE_PATCH] Main.dll patched successfully: '%s' -> '%s' at offset 0x%X",
		szLastPatchedIP, szNewIP, (DWORD)nOffset);
	CDebugMode::LogDebugAction("[FILE_PATCH] Main.dll file patched: %s -> %s", szLastPatchedIP, szNewIP);
}


/**
 * \brief 
 */
CMuInstanceManager::CMuInstanceManager()
{
	m_fProcessing = false;
	m_fServer = false;
	m_hHookDll = 0;
	m_fParasite = true;

	memset(m_szLoaderPath, 0, (_MAX_PATH+1)*sizeof(TCHAR));
}


/**
 * \brief 
 */
CMuInstanceManager::~CMuInstanceManager()
{
}


/**
 * \brief 
 */
BOOL CMuInstanceManager::Init(bool fRunAsParasite) 
{ 
	m_fParasite = fRunAsParasite;
	return Create(NULL) != NULL;
}


/**
 * \brief 
 */
void CMuInstanceManager::Term()
{
	if (IsWindow())
		DestroyWindow();
}


/**
 * \brief 
 */
LRESULT CMuInstanceManager::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA), m_hWnd, 101, 
			NIF_ICON | NIF_MESSAGE | NIF_TIP, SHELL_ICON_MESSAGE, LoadIcon(_AtlBaseModule.GetResourceInstance(), 
			MAKEINTRESOURCE(IDI_ICON1)),
			_T(__SOFTWARE_PRODUCT_NAME) };

	Shell_NotifyIcon(NIM_ADD, &nid);

	// Auto-enable debug mode on startup with logging of all LordOfMU.exe actions
	CDebugMode::SetEnabled(true);
	CDebugMode::SaveToRegistry();
	CDebugMode::LogDebugAction("Debug mode auto-enabled on LordOfMU.exe startup");

	TCHAR szPath[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szPath, _MAX_PATH);

	for (int i = (int)_tcslen(szPath) - 1; i >= 0 && szPath[i] != _T('\\'); szPath[i--] = 0);


	memset(m_szLoaderPath, 0, (_MAX_PATH+1)*sizeof(TCHAR));
	PrepareDll(m_szLoaderPath);

	{
		CT2A aLoaderPath(m_szLoaderPath);
		WriteHookLog("Loading hook DLL from: %s", (LPCSTR)aLoaderPath);
	}
	m_hHookDll = LoadLibrary(m_szLoaderPath);

	ATLASSERT(m_hHookDll);

	if (m_hHookDll)
	{
		WriteHookLog("Hook DLL loaded successfully at 0x%p", m_hHookDll);

		typedef HRESULT (__stdcall* InstallHookPtr)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
		InstallHookPtr InstallProc = (InstallHookPtr)GetProcAddress(m_hHookDll, "DllGetClassObject");

		GUID guid = {0};

		if (InstallProc)
		{
			InstallProc(guid, guid, (LPVOID*)szPath);
			WriteHookLog("DllGetClassObject called - hooks installed for main.exe");
		}
		else
		{
			WriteHookLog("WARNING: DllGetClassObject export not found in hook DLL");
		}
	}
	else
	{
		CT2A aLoaderPath(m_szLoaderPath);
		WriteHookLog("ERROR: Failed to load hook DLL from %s (error=%d)", (LPCSTR)aLoaderPath, (int)GetLastError());
	}

	// Validate Connect.ini before launching main.exe
	{
		TCHAR szIniPath[_MAX_PATH+1] = {0};
		_tcscpy(szIniPath, szPath);
		_tcscat(szIniPath, _T("Connect.ini"));

		struct _stat stIni = {0};
		if (0 == _tstat(szIniPath, &stIni))
		{
			char szIP[64] = {0};
			{
				CT2A aIniPath(szIniPath);
				GetPrivateProfileStringA("Config", "IP", "", szIP, sizeof(szIP), (LPCSTR)aIniPath);
			}

			if (szIP[0] != '\0')
			{
				WriteHookLog("Connect.ini found - server IP: %s", szIP);
				CDebugMode::LogDebugAction("Connect.ini validated: IP=%s", szIP);
			}
			else
			{
				WriteHookLog("WARNING: Connect.ini found but IP address is empty");
				CT2A aIniPath(szIniPath);
				CDebugMode::LogDebugAction("WARNING: Connect.ini found but IP address is empty at %s", (LPCSTR)aIniPath);
			}
		}
		else
		{
			CT2A aIniPath(szIniPath);
			WriteHookLog("WARNING: Connect.ini not found at %s - will be auto-created by DLL with Main.dll IP", (LPCSTR)aIniPath);
			CDebugMode::LogDebugAction("Connect.ini not found - will be auto-created during IP patch");
		}
	}

	// Patch Main.dll file on disk with IP from Connect.ini BEFORE launching main.exe.
	// This ensures the game client binary has the correct server IP embedded in it.
	PatchMainDllFileIP(szPath);

	// Launch the game client (main.exe) after Main.dll has been patched on disk.
	// The in-memory PatchMainDllIP() in LordOfMU.dll (Proxifier::InternalInit)
	// serves as an additional safety net for the IP override.
	{
		TCHAR szMainExe[_MAX_PATH+1] = {0};
		_tcscpy(szMainExe, szPath);
		_tcscat(szMainExe, _T("main.exe"));

		struct _stat stMain = {0};
		if (0 == _tstat(szMainExe, &stMain))
		{
			CT2A aMainExe(szMainExe);
			WriteHookLog("Launching main.exe: %s", (LPCSTR)aMainExe);
			HINSTANCE hResult = ShellExecute(NULL, _T("open"), szMainExe, NULL, szPath, SW_SHOWNORMAL);

			if ((INT_PTR)hResult <= 32)
			{
				WriteHookLog("ERROR: Failed to launch main.exe (error %d)", (int)(INT_PTR)hResult);
				TCHAR szErr[512] = {0};
				_sntprintf(szErr, 511, _T("Failed to launch main.exe (error %d).\nPath: %s"), (int)(INT_PTR)hResult, szMainExe);
				::MessageBox(NULL, szErr, _T("LordOfMU - Error"), MB_OK | MB_ICONERROR);
			}
			else
			{
				WriteHookLog("main.exe launched successfully");
			}
		}
		else
		{
			CT2A aMainExe(szMainExe);
			WriteHookLog("ERROR: main.exe not found at %s", (LPCSTR)aMainExe);
			TCHAR szErr[512] = {0};
			_sntprintf(szErr, 511, _T("main.exe not found.\nExpected path: %s"), szMainExe);
			::MessageBox(NULL, szErr, _T("LordOfMU - Error"), MB_OK | MB_ICONERROR);
		}
	}

	_tcscat(szPath, _T("LordOfServer.dll"));

	struct _stat st = {0};
	m_fServer = _tstat(szPath, &st) == 0;

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuInstanceManager::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	NOTIFYICONDATA nid = {sizeof(NOTIFYICONDATA), m_hWnd, 101, 0};
	Shell_NotifyIcon(NIM_DELETE, &nid);	

	if (m_hHookDll)
	{
		WriteHookLog("Removing DLL hooks from main.exe on exit");

		typedef HRESULT (__stdcall* RemoveHookPtr)();
		RemoveHookPtr RemoveProc = (RemoveHookPtr)GetProcAddress(m_hHookDll, "DllUnregisterServer");

		if (RemoveProc)
		{
			RemoveProc();
			WriteHookLog("DllUnregisterServer called - hooks removal initiated");
		}

		FreeLibrary(m_hHookDll);
		m_hHookDll = 0;
		WriteHookLog("Hook DLL unloaded via FreeLibrary");

		DeleteFile(m_szLoaderPath);
		CT2A aLoaderPath(m_szLoaderPath);
		WriteHookLog("Temporary hook DLL deleted: %s", (LPCSTR)aLoaderPath);

		TCHAR* pszFilename = PathFindFileName(m_szLoaderPath);

		if (pszFilename)
		{
			pszFilename[1] = _T('K');
			DeleteFile(m_szLoaderPath);

			pszFilename[1] = _T('T');
			DeleteFile(m_szLoaderPath);
		}

		WriteHookLog("DLL hook cleanup complete - all temporary files removed");
	}

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuInstanceManager::OnShellIcon(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (HIWORD(lParam) == 0 
			&& (LOWORD(lParam) == WM_RBUTTONUP || LOWORD(lParam) == WM_LBUTTONUP))
	{
		SetForegroundWindow(m_hWnd);


		if (m_fProcessing)
			return 0;

		m_fProcessing = true;

		POINT pt = {0};
		GetCursorPos(&pt);


		HMENU hPopup = CreatePopupMenu();
#if defined(__CLICKER_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
		AppendMenu(hPopup, MF_STRING, 101, _T("Help"));
#endif
		AppendMenu(hPopup, MF_STRING, 102, _T("About"));
		AppendMenu(hPopup, MF_SEPARATOR, 0, 0);

#if defined(__CLICKER_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
		HMENU hPopupST = CreatePopupMenu();

		CRegKey key;
		key.Create(HKEY_CURRENT_USER, _T("Software"));
		key.Create(key.m_hKey, _T("LordJerec"));
		key.Create(key.m_hKey, _T("MUAutoClicker"));
		key.Create(key.m_hKey, _T("Config"));

		DWORD dwType = 0;
		key.QueryDWORDValue(_T("ClientType"), dwType); // 0 - N/S, 1 - S3, 2 - S4

		AppendMenu(hPopup, MF_POPUP, (UINT_PTR)hPopupST, _T("Client Type"));

		AppendMenu(hPopupST, MF_STRING | ((dwType == 0 || dwType > 2) ? MF_CHECKED : 0), 200, _T("Not specified"));
		AppendMenu(hPopupST, MF_STRING | ((dwType == 1) ? MF_CHECKED : 0), 201, _T("Season 3"));
		AppendMenu(hPopupST, MF_STRING | ((dwType == 2) ? MF_CHECKED : 0), 202, _T("Season 4"));

		hPopupST = CreatePopupMenu();

		CRegKey key2;
		key2.Create(HKEY_CURRENT_USER, _T("Software"));
		key2.Create(key2.m_hKey, _T("LordJerec"));
		key2.Create(key2.m_hKey, _T("MUAutoClicker"));
		key2.Create(key2.m_hKey, _T("Config"));

		dwType = 0x00000001;
		key2.QueryDWORDValue(_T("ProtocolType"), dwType);

/*
	#define PTYPE_KOR 0x00000001
	#define PTYPE_CHS 0x00000002
	#define PTYPE_VTM 0x00000003
	#define PTYPE_JPN 0x00000004
	#define PTYPE_PHI 0x00000005
*/
	#if defined(__CLICKER_ELITE__) || defined(__CLICKER_AVANTA__) || defined(__INCLUDE_ALL_STUFF__)
		AppendMenu(hPopup, MF_POPUP, (UINT_PTR)hPopupST, _T("Protocol Type"));

		AppendMenu(hPopupST, MF_STRING | ((dwType == 1) ? MF_CHECKED : 0), 301, _T("Korea"));
//		AppendMenu(hPopupST, MF_STRING | ((dwType == 2) ? MF_CHECKED : 0), 302, _T("China"));
//		AppendMenu(hPopupST, MF_STRING | ((dwType == 3) ? MF_CHECKED : 0), 303, _T("Vietnam"));
		AppendMenu(hPopupST, MF_STRING | ((dwType == 4) ? MF_CHECKED : 0), 304, _T("Japan"));
		AppendMenu(hPopupST, MF_STRING | ((dwType == 5) ? MF_CHECKED : 0), 305, _T("Philipines"));
	#endif

		AppendMenu(hPopup, MF_SEPARATOR, 0, 0);
#endif

#if defined(__ANTIHACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)

		if (m_fServer)
		{
			AppendMenu(hPopup, MF_STRING, 104, _T("MD5 Calculator"));
			AppendMenu(hPopup, MF_SEPARATOR, 0, 0);
		}
#endif //defined(__ANTIHACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)

		AppendMenu(hPopup, MF_STRING, 103, _T("Exit"));

		// Debug mode toggle
		AppendMenu(hPopup, MF_SEPARATOR, 0, 0);
		AppendMenu(hPopup, MF_STRING | (CDebugMode::IsEnabled() ? MF_CHECKED : 0), 110, _T("Debug Mode"));

		int nCmd = (int)TrackPopupMenu(hPopup, TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, 0);
		
		if (nCmd == 102)
		{
			CAboutBox dlgAbout;
			dlgAbout.DoModal(m_hWnd);
		}
		else if (nCmd == 101)
		{
			TCHAR szFileName[_MAX_PATH+1] = {0};
			GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szFileName, _MAX_PATH);

			for (int i=(int)_tcslen(szFileName)-1; i >= 0 && szFileName[i] != _T('\\'); --i)
			{
				szFileName[i] = 0;
			}

			_tcscat(szFileName, _T("help\\index.html"));

			ShellExecute(0, _T("open"), szFileName, _T(""), 0, SW_SHOWNORMAL);
		}
		else if (nCmd == 103)
		{
			if (m_fParasite)
				g_uTimer = ::SetTimer(0, 101, 500, DelayedTerm);
			else
				PostQuitMessage(0);
		}
		else if (nCmd == 104)
		{
			CMD5CalcDlg dlg;
			dlg.DoModal(m_hWnd);
		}
		else if (nCmd == 200 || nCmd == 201 || nCmd == 202)
		{
			CRegKey key;
			key.Create(HKEY_CURRENT_USER, _T("Software"));
			key.Create(key.m_hKey, _T("LordJerec"));
			key.Create(key.m_hKey, _T("MUAutoClicker"));
			key.Create(key.m_hKey, _T("Config"));

			key.SetDWORDValue(_T("ClientType"), (DWORD)(nCmd - 200)); // 0 - N/S, 1 - S3, 2 - S4
		}
		else if (nCmd >= 301 && nCmd <= 305)
		{
			CRegKey key;
			key.Create(HKEY_CURRENT_USER, _T("Software"));
			key.Create(key.m_hKey, _T("LordJerec"));
			key.Create(key.m_hKey, _T("MUAutoClicker"));
			key.Create(key.m_hKey, _T("Config"));

			key.SetDWORDValue(_T("ProtocolType"), (DWORD)(nCmd - 300));
		}
		else if (nCmd == 110)
		{
			// Toggle debug mode
			bool fNewState = !CDebugMode::IsEnabled();
			CDebugMode::SetEnabled(fNewState);
			CDebugMode::SaveToRegistry();

			CDebugMode::LogDebugAction("Debug mode toggled %s by user from tray menu", fNewState ? "ON" : "OFF");

			TCHAR szMsg[256] = {0};
			_sntprintf(szMsg, 255, _T("Debug mode %s.\nAll actions will be logged to CrashDump.txt and ClickerLog.txt."), 
					   fNewState ? _T("ENABLED") : _T("DISABLED"));
			::MessageBox(m_hWnd, szMsg, _T("LordOfMU - Debug Mode"), MB_OK | MB_ICONINFORMATION);
		}

#if defined(__CLICKER_STUFF__)
		DestroyMenu(hPopupST);
#endif
		DestroyMenu(hPopup);
		m_fProcessing = false;
	}

	return 0;
}


/**
 * \brief 
 */
LRESULT CMuInstanceManager::OnMuWindowControl(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam)
	{
	case 1: // register MU window
		break;
	case 2: // next MU window
		break;
	case 3: // hide all MU windows
		break;
	}

	return 0;
}


/**  
 * \brief 
 */
void CMuInstanceManager::PrepareDll(TCHAR* pszPath)
{
	if (!pszPath)
		return;

	TCHAR szPath[_MAX_PATH+1] = {0};
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szPath, _MAX_PATH);

	for (int i=(int)_tcslen(szPath)-1; i >= 0 && szPath[i] != _T('\\'); szPath[i--] = 0);
	_tcscat(szPath, CA2CT(__LOADER_DLL_NAME));

	TCHAR szDestPath[_MAX_PATH+1] = {0};
	GetTempPath(_MAX_PATH, szDestPath);
	PathAddBackslash(szDestPath);

	srand(GetTickCount());

	int len = (int)_tcslen(szDestPath);
	szDestPath[len++] = _T('x');

	for (int i=0; i < 4; i++)
	{
		szDestPath[len++] = (rand() % 0x19) + 0x61;
		szDestPath[len++] = (rand() % 0x19) + 0x41;
	}

	_tcscat(szDestPath, _T(".dll"));

	CopyFile(szPath, szDestPath, FALSE);

	_tcscpy(pszPath, szDestPath);
}

