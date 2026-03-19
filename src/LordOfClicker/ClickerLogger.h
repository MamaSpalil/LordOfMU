#ifndef __ClickerLogger_H
#define __ClickerLogger_H

#pragma once

#include <stdio.h>
#include <time.h>
#include <windows.h>

/**
 * \brief Writes a log entry with timestamp to ClickerLog.txt in the game root folder.
 *        Thread-safe: uses a named mutex to serialize concurrent calls.
 */
static void WriteClickerLog(const char* szMessage)
{
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_ClickerLog_Mutex");
	if (!hMutex)
	{
		OutputDebugStringA("ClickerLogger: Failed to create mutex\n");
		return;
	}

	WaitForSingleObject(hMutex, INFINITE);

	char szPath[MAX_PATH + 1] = {0};
	GetModuleFileNameA(NULL, szPath, MAX_PATH);

	int i;
	for (i = (int)strlen(szPath) - 1; i >= 0 && szPath[i] != '\\'; --i);
	szPath[i + 1] = '\0';

	size_t nPathLen = strlen(szPath);
	strncat(szPath, "ClickerLog.txt", MAX_PATH - nPathLen);

	FILE* f = fopen(szPath, "a");
	if (f)
	{
		time_t t = time(NULL);
		struct tm* tm_info = localtime(&t);
		char szTime[32] = {0};
		strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", tm_info);

		fprintf(f, "[%s] %s\n", szTime, szMessage);
		fclose(f);
	}
	else
	{
		OutputDebugStringA("ClickerLogger: Failed to open ClickerLog.txt\n");
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
}

#endif //__ClickerLogger_H
