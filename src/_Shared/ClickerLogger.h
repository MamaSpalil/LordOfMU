#ifndef __ClickerLogger_H
#define __ClickerLogger_H

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>
#include "DebugMode.h"

/**
 * \brief Central logging system for the auto-clicker module.
 *        Logs all actions, DLL hook operations, packet/socket traffic,
 *        and debug information to ClickerLog.txt.
 *        Thread-safe: uses a named mutex to serialize concurrent calls.
 *
 *        When CDebugMode::IsEnabled(), also outputs to the debug console window.
 *
 *        Log categories:
 *        [CLICKER]   - Auto-clicker actions (heal, pickup, repair, start/stop)
 *        [HOOK]      - DLL hook check, injection, removal
 *        [PACKET]    - Packet send/receive traffic
 *        [SOCKET]    - Socket connection events
 *        [DEBUG]     - Debug mode messages
 *        [CRASH]     - Crash detection and reports
 *        [PICKUP]    - Auto-pickup item detection and processing
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
 *        When debug mode is enabled, also outputs to the debug console window.
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

	time_t t = time(NULL);
	struct tm tm_storage = {0};
	localtime_s(&tm_storage, &t);
	char szTime[32] = {0};
	strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", &tm_storage);

	// Format the message
	char szMessage[1024] = {0};
	va_list args;
	va_start(args, szFormat);
	_vsnprintf(szMessage, sizeof(szMessage) - 1, szFormat, args);
	szMessage[sizeof(szMessage) - 1] = '\0';
	va_end(args);

	// Write to file
	FILE* f = fopen(szPath, "a");
	if (f)
	{
		fprintf(f, "[%s] [%s] %s\n", szTime, szCategory, szMessage);
		fclose(f);
	}

	// Write to debug console if enabled
	if (CDebugMode::IsEnabled())
	{
		printf("[%s] [%s] %s\n", szTime, szCategory, szMessage);
		fflush(stdout);
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
 *        Writes to ClickerLog.txt and debug console (if enabled).
 */
static void WriteHookLog(const char* szFormat, ...)
{
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_ClickerLog_Mutex");
	if (!hMutex)
		return;

	WaitForSingleObject(hMutex, INFINITE);

	char szPath[MAX_PATH + 1] = {0};
	GetClickerLogPath(szPath, MAX_PATH);

	time_t t = time(NULL);
	struct tm tm_storage = {0};
	localtime_s(&tm_storage, &t);
	char szTime[32] = {0};
	strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", &tm_storage);

	char szMessage[1024] = {0};
	va_list args;
	va_start(args, szFormat);
	_vsnprintf(szMessage, sizeof(szMessage) - 1, szFormat, args);
	szMessage[sizeof(szMessage) - 1] = '\0';
	va_end(args);

	FILE* f = fopen(szPath, "a");
	if (f)
	{
		fprintf(f, "[%s] [HOOK] %s\n", szTime, szMessage);
		fclose(f);
	}

	if (CDebugMode::IsEnabled())
	{
		printf("[%s] [HOOK] %s\n", szTime, szMessage);
		fflush(stdout);
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


/**
 * \brief Logs a packet traffic event (client<->server).
 *        Writes to ClickerLog.txt and debug console (if enabled).
 */
static void WritePacketLog(const char* szDirection, const char* szDescription, int nLen, const char* szHexData)
{
	WriteClickerLogFmt("PACKET", "%s | %s | Len=%d | %s", 
		szDirection, szDescription, nLen, szHexData ? szHexData : "");
}


/**
 * \brief Logs a socket connection event.
 *        Writes to ClickerLog.txt and debug console (if enabled).
 */
static void WriteSocketLog(const char* szFormat, ...)
{
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_ClickerLog_Mutex");
	if (!hMutex)
		return;

	WaitForSingleObject(hMutex, INFINITE);

	char szPath[MAX_PATH + 1] = {0};
	GetClickerLogPath(szPath, MAX_PATH);

	time_t t = time(NULL);
	struct tm tm_storage = {0};
	localtime_s(&tm_storage, &t);
	char szTime[32] = {0};
	strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", &tm_storage);

	char szMessage[1024] = {0};
	va_list args;
	va_start(args, szFormat);
	_vsnprintf(szMessage, sizeof(szMessage) - 1, szFormat, args);
	szMessage[sizeof(szMessage) - 1] = '\0';
	va_end(args);

	FILE* f = fopen(szPath, "a");
	if (f)
	{
		fprintf(f, "[%s] [SOCKET] %s\n", szTime, szMessage);
		fclose(f);
	}

	if (CDebugMode::IsEnabled())
	{
		printf("[%s] [SOCKET] %s\n", szTime, szMessage);
		fflush(stdout);
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


/**
 * \brief Logs a debug mode message.
 *        Writes to ClickerLog.txt and debug console (if enabled).
 */
static void WriteDebugLog(const char* szFormat, ...)
{
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_ClickerLog_Mutex");
	if (!hMutex)
		return;

	WaitForSingleObject(hMutex, INFINITE);

	char szPath[MAX_PATH + 1] = {0};
	GetClickerLogPath(szPath, MAX_PATH);

	time_t t = time(NULL);
	struct tm tm_storage = {0};
	localtime_s(&tm_storage, &t);
	char szTime[32] = {0};
	strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", &tm_storage);

	char szMessage[1024] = {0};
	va_list args;
	va_start(args, szFormat);
	_vsnprintf(szMessage, sizeof(szMessage) - 1, szFormat, args);
	szMessage[sizeof(szMessage) - 1] = '\0';
	va_end(args);

	FILE* f = fopen(szPath, "a");
	if (f)
	{
		fprintf(f, "[%s] [DEBUG] %s\n", szTime, szMessage);
		fclose(f);
	}

	if (CDebugMode::IsEnabled())
	{
		printf("[%s] [DEBUG] %s\n", szTime, szMessage);
		fflush(stdout);
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}


#endif //__ClickerLogger_H
