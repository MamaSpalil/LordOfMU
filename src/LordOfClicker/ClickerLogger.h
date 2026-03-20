#ifndef __ClickerLogger_H
#define __ClickerLogger_H

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>

/**
 * \brief Central logging system for the auto-clicker module.
 *        Logs all actions, DLL hook operations, packet/socket traffic,
 *        and debug information to ClickerLog.txt.
 *        Thread-safe: uses a named mutex to serialize concurrent calls.
 *
 *        Log categories:
 *        [CLICKER]   - Auto-clicker actions (heal, pickup, repair, start/stop)
 *        [HOOK]      - DLL hook check, injection, removal
 *        [PACKET]    - Packet send/receive traffic
 *        [SOCKET]    - Socket connection events
 *        [DEBUG]     - Debug mode messages
 *        [CRASH]     - Crash detection and reports
 */

/**
 * \brief Gets the path to the ClickerLog.txt file in the game root folder.
 */
static void GetClickerLogPath(char* szPath, size_t nMaxLen)
{
	GetModuleFileNameA(NULL, szPath, (DWORD)nMaxLen);

	int i;
	for (i = (int)strlen(szPath) - 1; i >= 0 && szPath[i] != '\\'; --i);
	szPath[i + 1] = '\0';

	size_t nPathLen = strlen(szPath);
	strncat(szPath, "ClickerLog.txt", nMaxLen - nPathLen - 1);
}


/**
 * \brief Writes a formatted log entry with timestamp to ClickerLog.txt.
 *        Supports printf-style format strings.
 */
static void WriteClickerLogFmt(const char* szCategory, const char* szFormat, ...)
{
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_ClickerLog_Mutex");
	if (!hMutex)
	{
		OutputDebugStringA("ClickerLogger: Failed to create mutex\n");
		return;
	}

	WaitForSingleObject(hMutex, INFINITE);

	char szPath[MAX_PATH + 1] = {0};
	GetClickerLogPath(szPath, MAX_PATH);

	FILE* f = fopen(szPath, "a");
	if (f)
	{
		time_t t = time(NULL);
		struct tm* tm_info = localtime(&t);
		char szTime[32] = {0};
		strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", tm_info);

		fprintf(f, "[%s] [%s] ", szTime, szCategory);

		va_list args;
		va_start(args, szFormat);
		vfprintf(f, szFormat, args);
		va_end(args);

		fprintf(f, "\n");
		fclose(f);
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


/**
 * \brief Writes a simple log entry with timestamp to ClickerLog.txt.
 */
static void WriteClickerLog(const char* szMessage)
{
	WriteClickerLogFmt("CLICKER", "%s", szMessage);
}


/**
 * \brief Logs a DLL hook-related event.
 */
static void WriteHookLog(const char* szFormat, ...)
{
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_ClickerLog_Mutex");
	if (!hMutex)
		return;

	WaitForSingleObject(hMutex, INFINITE);

	char szPath[MAX_PATH + 1] = {0};
	GetClickerLogPath(szPath, MAX_PATH);

	FILE* f = fopen(szPath, "a");
	if (f)
	{
		time_t t = time(NULL);
		struct tm* tm_info = localtime(&t);
		char szTime[32] = {0};
		strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", tm_info);

		fprintf(f, "[%s] [HOOK] ", szTime);

		va_list args;
		va_start(args, szFormat);
		vfprintf(f, szFormat, args);
		va_end(args);

		fprintf(f, "\n");
		fclose(f);
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


/**
 * \brief Logs a packet traffic event (client<->server).
 */
static void WritePacketLog(const char* szDirection, const char* szDescription, int nLen, const char* szHexData)
{
	WriteClickerLogFmt("PACKET", "%s | %s | Len=%d | %s", 
		szDirection, szDescription, nLen, szHexData ? szHexData : "");
}


/**
 * \brief Logs a socket connection event.
 */
static void WriteSocketLog(const char* szFormat, ...)
{
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_ClickerLog_Mutex");
	if (!hMutex)
		return;

	WaitForSingleObject(hMutex, INFINITE);

	char szPath[MAX_PATH + 1] = {0};
	GetClickerLogPath(szPath, MAX_PATH);

	FILE* f = fopen(szPath, "a");
	if (f)
	{
		time_t t = time(NULL);
		struct tm* tm_info = localtime(&t);
		char szTime[32] = {0};
		strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", tm_info);

		fprintf(f, "[%s] [SOCKET] ", szTime);

		va_list args;
		va_start(args, szFormat);
		vfprintf(f, szFormat, args);
		va_end(args);

		fprintf(f, "\n");
		fclose(f);
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


/**
 * \brief Logs a debug mode message.
 */
static void WriteDebugLog(const char* szFormat, ...)
{
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_ClickerLog_Mutex");
	if (!hMutex)
		return;

	WaitForSingleObject(hMutex, INFINITE);

	char szPath[MAX_PATH + 1] = {0};
	GetClickerLogPath(szPath, MAX_PATH);

	FILE* f = fopen(szPath, "a");
	if (f)
	{
		time_t t = time(NULL);
		struct tm* tm_info = localtime(&t);
		char szTime[32] = {0};
		strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", tm_info);

		fprintf(f, "[%s] [DEBUG] ", szTime);

		va_list args;
		va_start(args, szFormat);
		vfprintf(f, szFormat, args);
		va_end(args);

		fprintf(f, "\n");
		fclose(f);
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


#endif //__ClickerLogger_H
