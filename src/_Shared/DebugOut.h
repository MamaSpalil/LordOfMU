#ifndef __DebugOut_H
#define __DebugOut_H

#pragma once

#include <conio.h>

#ifdef DEBUG
//	#define __DEBUG_OUT
	#define __DEBUG_LOG
	#define __LOG_LEVEL LOG_LEVEL_INFO
//	#define __LOG_LEVEL LOG_LEVEL_SILENT
	#define __MAX_LOG_SIZE 2000000
	#define __CONNECT_LOG
#else
	#ifdef _LORD_OF_SERVER_DLL
//		#define __DEBUG_LOG
//		#define __LOG_LEVEL LOG_LEVEL_INFO
		#define __LOG_LEVEL LOG_LEVEL_SILENT
		#define __MAX_LOG_SIZE 10000000
		#define __CONNECT_LOG
	#else
		#define __DEBUG_LOG
		#define __LOG_LEVEL LOG_LEVEL_ERROR
//		#define __LOG_LEVEL LOG_LEVEL_INFO
		#define __MAX_LOG_SIZE 2000000
	#endif
#endif 




/**
* \brief 
*/
class CConnectLog
{
public:
	static bool Init(long lPort)
	{
		s_hFile() = 0;
		s_lPort() = lPort;

		RotateLog();

		return true;
	}

	static void CleanUp()
	{
		if (s_hFile())
			CloseHandle(s_hFile());

		s_hFile() = 0;
	}

	static void Print(DWORD dwIP, const char* szAccount, const char* szChar, const char* szConnMessage)
	{
		Lock();
		PrintTimeStamp();

		if (s_hFile())
		{
			RotateLog();

			char szMessage[512] = {0};
			sprintf(szMessage, "[%d.%d.%d.%d][%s][%s] %s\r\n", LOBYTE(dwIP), HIBYTE(dwIP), LOBYTE(HIWORD(dwIP)), HIBYTE(HIWORD(dwIP)), szAccount, szChar, szConnMessage);

			DWORD len = (DWORD)strlen(szMessage);

			DWORD dwWritten = 0;
			WriteFile(s_hFile(), szMessage, len, &dwWritten, 0);
		}

		UnLock();
	}

protected:
	static CRITICAL_SECTION* GetCS()
	{
		static CRITICAL_SECTION cs = {0};
		static bool fInit = false;

		if (!fInit)
		{
			InitializeCriticalSection(&cs);
			fInit = true;
		}

		return &cs;
	}

	static void Lock()
	{
		EnterCriticalSection(GetCS());
	}

	static void UnLock()
	{
		LeaveCriticalSection(GetCS());
	}

	static void PrintTimeStamp()
	{
		SYSTEMTIME st = {0};
		GetLocalTime(&st);

		char szMessage[256] = {0};
		_sprintf_p(szMessage, 255, "[%02d/%02d/%04d %02d:%02d:%02d:%04d] ", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		DWORD len = (DWORD)strlen(szMessage);

		if (s_hFile())
		{
			DWORD dwWritten = 0;
			WriteFile(s_hFile(), szMessage, len, &dwWritten, 0);
		}
	}


private:
	static HANDLE& s_hFile(){ static HANDLE sHFile = 0; return sHFile; }
	static int& s_iCheckCount(){ static int iCounter = 0; return iCounter; }

	static long& s_lPort(){ static long lPort = 0; return lPort; }
	static SYSTEMTIME& s_tTime() { static SYSTEMTIME tTime = {0}; return tTime; }

	static void RotateLog()
	{
		SYSTEMTIME st = {0};
		GetLocalTime(&st);

		SYSTEMTIME& st2 = s_tTime();
		if (s_hFile() && st.wYear == st2.wYear && st.wMonth == st2.wMonth && st.wDay == st2.wDay)
			return;

		if (s_hFile())
			CloseHandle(s_hFile());

		st2 = st;
		CreateLogFile();
	}

	static void CreateLogFile()
	{
		CreateDirectory(_T("Log"), 0);
		SYSTEMTIME& st = s_tTime();

		TCHAR szFileName[_MAX_PATH+1] = {0};
		_stprintf(szFileName, _T("Log\\LordOfConnect_%04d-%02d-%02d_%d.log"), st.wYear, st.wMonth, st.wDay, s_lPort());

		s_hFile() = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == s_hFile())
			s_hFile() = 0;
		else
		{
			SetFilePointer(s_hFile(), 0, 0, FILE_END);
		}
	}
};





/**
 *
 */
class CDebugOut
{
public:
	static bool Init()
	{
		logLevel() = __LOG_LEVEL;

#ifdef __DEBUG_OUT
		AllocConsole();
#endif

#ifdef __DEBUG_LOG
		s_hFile() = 0;

		CreateLogFile();
		RotateLog();
#endif

		return true;
	}

	static void CleanUp()
	{
#ifdef __DEBUG_LOG
		if (s_hFile())
			CloseHandle(s_hFile());

		s_hFile() = 0;
#endif
	}

	static void SetLogLevel(int iLevel)
	{
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		Lock();
		logLevel() = iLevel;
		UnLock();
#endif
	}

	static const int LOG_LEVEL_SILENT = -1;
	static const int LOG_LEVEL_ERROR = 0;
	static const int LOG_LEVEL_WARN = 1;
	static const int LOG_LEVEL_INFO = 2;
	static const int LOG_LEVEL_DEBUG = 3;

	static int& logLevel(){ static int s_iLogLevel = 0; return s_iLogLevel; };

	static void PrintLastError(char* lpszFunction) 
	{ 
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		if (logLevel() < LOG_LEVEL_ERROR)
			return;

		Lock();

		char szBuf[80] = {0}; 
		LPSTR lpMsgBuf = 0;
		DWORD dw = GetLastError(); 

		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpMsgBuf, 0, NULL);
#endif

#if defined(__DEBUG_OUT)
		_cprintf("%s failed with error %d: %s\n", lpszFunction, dw, lpMsgBuf); 
#endif

#if defined(__DEBUG_LOG)
		if (s_hFile())
		{
			PrintTimeStamp();

			char szMessage[512] = {0};
			_snprintf(szMessage, 511, "%s failed with error %d: %s\n", lpszFunction, dw, lpMsgBuf);

			DWORD dwWritten = 0;
			WriteFile(s_hFile(), szMessage, (DWORD)strlen(szMessage), &dwWritten, 0);
		}
#endif

#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		LocalFree(lpMsgBuf);
		UnLock();
#endif
	}

	static void PrintLastError(char* lpszFunction, DWORD dw) 
	{ 
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		if (logLevel() < LOG_LEVEL_ERROR)
			return;

		Lock();

		char szBuf[80] = {0}; 
		LPSTR lpMsgBuf = 0;

		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpMsgBuf, 0, NULL);
#endif

#if defined(__DEBUG_OUT)
		_cprintf("%s failed with error %d: %s\n", lpszFunction, dw, lpMsgBuf); 
#endif

#if defined(__DEBUG_LOG)
		if (s_hFile())
		{
			PrintTimeStamp();

			char szMessage[512] = {0};
			_snprintf(szMessage, 511, "%s failed with error %d: %s\n", lpszFunction, dw, lpMsgBuf);

			DWORD dwWritten = 0;
			WriteFile(s_hFile(), szMessage, (DWORD)strlen(szMessage), &dwWritten, 0);
		}
#endif

#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		LocalFree(lpMsgBuf);

		UnLock();
#endif
	}

	static void PrintDebug(char* format, ...)
	{
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		if (logLevel() < LOG_LEVEL_DEBUG)
			return;

		va_list args;
		va_start(args, format);

		PrintV(format, args);
#endif
	}

	static void PrintWarn(char* format, ...)
	{
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		if (logLevel() < LOG_LEVEL_WARN)
			return;

		va_list args;
		va_start(args, format);

		PrintV(format, args);
#endif
	}

	static void PrintInfo(char* format, ...)
	{
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		if (logLevel() < LOG_LEVEL_INFO)
			return;

		va_list args;
		va_start(args, format);

		PrintV(format, args);
#endif
	}

	static void PrintError(char* format, ...) 
	{ 
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		if (logLevel() < LOG_LEVEL_ERROR)
			return;

		va_list args;
		va_start(args, format);

		PrintV(format, args);
#endif
	}


	static void PrintAlways(const char* format, ...) 
	{ 
		va_list args;
		va_start(args, format);

		PrintV(format, args);
	}

protected:
	static void PrintV(const char* format, va_list args)
	{
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		Lock();
		PrintTimeStamp();
#endif

#if defined(__DEBUG_OUT)
		_vcprintf(format, args);
		_cprintf("\n"); 
#endif

#if defined(__DEBUG_LOG)
		if (s_hFile())
		{
			char szMessage[512] = {0};
			_vsprintf_p(szMessage, 510, format, args);

			DWORD len = (DWORD)strlen(szMessage);
			szMessage[len++] = '\n';

			DWORD dwWritten = 0;
			WriteFile(s_hFile(), szMessage, len, &dwWritten, 0);

			if (s_iCheckCount()++ >= 500)
			{
				RotateLog();
				s_iCheckCount() = 0;
			}
		}
#endif

#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		UnLock();
#endif
	}

	static void PrintTimeStamp()
	{
#if defined(__DEBUG_OUT) || defined(__DEBUG_LOG)
		SYSTEMTIME st = {0};
		GetLocalTime(&st);

		char szMessage[256] = {0};
		_sprintf_p(szMessage, 255, "[%02d/%02d/%04d %02d:%02d:%02d:%04d] ", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		DWORD len = (DWORD)strlen(szMessage);
#endif

#if defined(__DEBUG_OUT)
		_cprintf(szMessage);
#endif

#if defined(__DEBUG_LOG)
		if (s_hFile())
		{
			DWORD dwWritten = 0;
			WriteFile(s_hFile(), szMessage, len, &dwWritten, 0);
		}
#endif
	}

protected:
	static CRITICAL_SECTION* GetCS()
	{
		static CRITICAL_SECTION cs = {0};
		static bool fInit = false;

		if (!fInit)
		{
			InitializeCriticalSection(&cs);
			fInit = true;
		}

		return &cs;
	}

	static void Lock()
	{
		EnterCriticalSection(GetCS());
	}

	static void UnLock()
	{
		LeaveCriticalSection(GetCS());
	}

#ifdef __DEBUG_LOG
private:
	static HANDLE& s_hFile(){ static HANDLE sHFile = 0; return sHFile; };
	static int& s_iCheckCount(){ static int iCounter = 0; return iCounter; };

	static void RotateLog()
	{
		DWORD dwSize = GetFileSize(s_hFile(), 0);

		if (dwSize >= __MAX_LOG_SIZE)
		{
			CloseHandle(s_hFile());
			
#ifdef _LORD_OF_SERVER_DLL
			DeleteFile(_T("Log\\LordOfServer_old.log"));
			MoveFile(_T("Log\\LordOfServer.log"), _T("Log\\LordOfServer_old.log"));
#else
			DeleteFile(_T("Log\\LordOfMU_old.log"));
			MoveFile(_T("Log\\LordOfMU.log"), _T("Log\\LordOfMU_old.log"));
#endif
			CreateLogFile();
		}
	}

	static void CreateLogFile()
	{
		CreateDirectory(_T("Log"), 0);

#ifdef _LORD_OF_SERVER_DLL
		s_hFile() = CreateFile(_T("Log\\LordOfServer.log"), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
#else
		s_hFile() = CreateFile(_T("Log\\LordOfMU.log"), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
#endif

		if (INVALID_HANDLE_VALUE == s_hFile())
			s_hFile() = 0;
		else
		{
			SetFilePointer(s_hFile(), 0, 0, FILE_END);

			PrintAlways("    ----------  Logging started ----------------- ");
		}
	}
#endif
};



#endif //__DebugOut_H