#ifndef __MuWindowUtil_H
#define __MuWindowUtil_H

#pragma once


/**
 * \brief 
 */
class CMuWindowUtil
{
public:
	/**
	 * \brief 
	 */
	static bool IsMuWindow(HWND hwnd)
	{
		if (!IsWindow(hwnd))
			return false;

		char szClass[256] = {0};
		GetClassNameA(hwnd, szClass, 255);

		return IsMuWindowClassName(szClass);
	}


	/**
	 * \brief 
	 */
	static bool IsMuWindowClassName(const char* szClass)
	{
		return szClass != 0 && (0 == _stricmp(szClass, "MU")
			|| 0 == _stricmp(szClass, "DIEmWin")
			|| 0 == _stricmp(szClass, "MU Online")
			|| 0 == _stricmp(szClass, "MU BLUE"));
	}


	/**
	 * \brief 
	 */
	static bool IsGSWindow(HWND hwnd)
	{
		if (!IsWindow(hwnd))
			return false;

		TCHAR szClass[256] = {0};
		GetClassName(hwnd, szClass, 255);

		return (0 == _tcsicmp(szClass, _T("GAMESERVER")));
	}


	/**
	 * \brief 
	 */
	static HWND FindMuWindow()
	{
		HWND hwnd = 0;
		EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);

		return hwnd;
	}


	/**  
	 * \brief 
	 */
	static LRESULT PostMessageToMuWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		HWND hwndMU = FindMuWindow();

		if (!IsWindow(hwndMU))
			return 0;

		return PostMessage(hwndMU, uMsg, wParam, lParam);
	}


	/**  
	 * \brief 
	 */
	static LRESULT SendMessageToMuWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		HWND hwndMU = FindMuWindow();

		if (!IsWindow(hwndMU))
			return 0;

		return SendMessage(hwndMU, uMsg, wParam, lParam);
	}

protected:
	/**
	 * \brief 
	 */
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
	{
		if (IsMuWindow(hwnd))
		{
			DWORD procId = 0; 
			GetWindowThreadProcessId(hwnd, &procId);

			if (GetCurrentProcessId() == procId)
			{
				*((HWND*)lParam) = hwnd;
				return FALSE;
			}
		}

		return TRUE;
	}
};

#endif // __MuWindowUtil_H