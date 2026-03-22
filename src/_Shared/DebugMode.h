#ifndef __DebugMode_H
#define __DebugMode_H

#pragma once

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")


/**
 * \brief Debug mode controller for LordOfMU.exe and main.exe.
 *
 *        Features:
 *        - Global exception handler to catch and report crashes
 *        - Debug log level control (toggle verbose logging)
 *        - Crash dump generation with reason reporting
 *        - All actions logged to ClickerLog.txt
 *
 *        Usage:
 *        - Call CDebugMode::Install() once during startup
 *        - Call CDebugMode::SetEnabled(true/false) to toggle debug mode
 *        - Crash reports are written automatically to CrashDump.txt and ClickerLog.txt
 */
class CDebugMode
{
public:
	/**
	 * \brief Install the global crash handler and initialize debug mode.
	 *        Must be called once during application startup.
	 */
	static void Install()
	{
		s_pfnOldFilter() = SetUnhandledExceptionFilter(CrashHandler);
		s_fInstalled() = true;
		LogDebugAction("Debug mode handler installed");
	}

	/**
	 * \brief Uninstall the crash handler.
	 */
	static void Uninstall()
	{
		if (s_fInstalled())
		{
			SetUnhandledExceptionFilter(s_pfnOldFilter());
			s_fInstalled() = false;
			LogDebugAction("Debug mode handler uninstalled");
		}
	}

	/**
	 * \brief Enable or disable verbose debug mode.
	 *        When enabled, allocates a console window for real-time log viewing.
	 *        All game actions, packet traffic, and auto-pickup events are shown
	 *        in the console and saved to ClickerLog.txt.
	 */
	static void SetEnabled(bool fEnabled)
	{
		s_fEnabled() = fEnabled;

		if (fEnabled)
		{
			if (!s_fConsoleAllocated())
			{
				bool fNewConsole = false;

				if (AllocConsole())
				{
					fNewConsole = true;
					SetConsoleTitleA("LordOfMU - Debug Console");

					// Set console buffer and window size for better readability
					HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
					if (hOut != INVALID_HANDLE_VALUE)
					{
						COORD bufSize;
						bufSize.X = 200;
						bufSize.Y = 9999;
						SetConsoleScreenBufferSize(hOut, bufSize);

						// Set console text color to green on black (MU Online style)
						SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					}
				}

				// Redirect stdout to the console in THIS module.
				// This is needed even when AllocConsole() failed because another
				// module (Bootstrapper / LordOfMUdll) may have already allocated
				// the console.  Each module has its own CRT stdout handle, so
				// every module must redirect independently.
				if (fNewConsole || GetConsoleWindow() != NULL)
				{
					FILE* fp = NULL;
					if (freopen_s(&fp, "CONOUT$", "w", stdout) == 0)
					{
						setvbuf(stdout, NULL, _IONBF, 0); // disable buffering for immediate output
					}
					else
					{
						LogDebugAction("Warning: failed to redirect stdout to console");
					}

					// Also redirect stderr so _cprintf / CDebugOut output reaches the console
					FILE* fpErr = NULL;
					freopen_s(&fpErr, "CONOUT$", "w", stderr);

					s_fConsoleAllocated() = true;

					printf("[DEBUG] Console window opened - all game actions will be logged here and to ClickerLog.txt\n");
					printf("[DEBUG] Debug mode ENABLED - verbose logging active\n");
				}
				else
				{
					LogDebugAction("Warning: AllocConsole() failed and no console window exists (error=%d)", (int)GetLastError());
				}
			}
			LogDebugAction("Debug mode ENABLED - verbose logging active, console window opened");
		}
		else
		{
			LogDebugAction("Debug mode DISABLED - closing console window");

			if (s_fConsoleAllocated())
			{
				printf("[DEBUG] Debug mode DISABLED - closing console window\n");
				fflush(stdout);
				FreeConsole();
				s_fConsoleAllocated() = false;
			}
		}
	}

	/**
	 * \brief Check if debug mode is currently enabled.
	 */
	static bool IsEnabled()
	{
		return s_fEnabled();
	}

	/**
	 * \brief Log a debug action to CrashDump.txt and ClickerLog.txt.
	 *        When debug mode is enabled, also outputs to the debug console.
	 */
	static void LogDebugAction(const char* szFormat, ...)
	{
		HANDLE hMutex = CreateMutexA(NULL, FALSE, "LordOfMU_DebugMode_Mutex");
		if (!hMutex)
			return;

		WaitForSingleObject(hMutex, INFINITE);

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

		// Write to CrashDump.txt
		char szPath[MAX_PATH + 1] = {0};
		GetModuleFileNameA(NULL, szPath, MAX_PATH);

		int i;
		for (i = (int)strlen(szPath) - 1; i >= 0 && szPath[i] != '\\'; --i);
		szPath[i + 1] = '\0';

		char szDir[MAX_PATH + 1] = {0};
		strncpy(szDir, szPath, MAX_PATH);

		strncat(szPath, "CrashDump.txt", MAX_PATH - strlen(szPath) - 1);

		FILE* f = fopen(szPath, "a");
		if (f)
		{
			fprintf(f, "[%s] [DEBUG] %s\n", szTime, szMessage);
			fclose(f);
		}

		// Also write to ClickerLog.txt for unified logging
		char szLogPath[MAX_PATH + 1] = {0};
		strncpy(szLogPath, szDir, MAX_PATH);
		strncat(szLogPath, "ClickerLog.txt", MAX_PATH - strlen(szLogPath) - 1);

		FILE* fLog = fopen(szLogPath, "a");
		if (fLog)
		{
			fprintf(fLog, "[%s] [DEBUG] %s\n", szTime, szMessage);
			fclose(fLog);
		}

		// Output to debug console if enabled
		if (s_fEnabled() && s_fConsoleAllocated())
		{
			printf("[%s] [DEBUG] %s\n", szTime, szMessage);
			fflush(stdout);
		}

		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}


private:
	static bool& s_fEnabled()
	{
		static bool fEnabled = false;
		return fEnabled;
	}

	static bool& s_fInstalled()
	{
		static bool fInstalled = false;
		return fInstalled;
	}

	static bool& s_fConsoleAllocated()
	{
		static bool fAllocated = false;
		return fAllocated;
	}

	static LPTOP_LEVEL_EXCEPTION_FILTER& s_pfnOldFilter()
	{
		static LPTOP_LEVEL_EXCEPTION_FILTER pfn = NULL;
		return pfn;
	}

	/**
	 * \brief Get a human-readable string for an exception code.
	 */
	static const char* GetExceptionCodeString(DWORD dwCode)
	{
		switch (dwCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:         return "ACCESS_VIOLATION";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_BREAKPOINT:               return "BREAKPOINT";
		case EXCEPTION_DATATYPE_MISALIGNMENT:    return "DATATYPE_MISALIGNMENT";
		case EXCEPTION_FLT_DENORMAL_OPERAND:     return "FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT:       return "FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION:    return "FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW:             return "FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK:          return "FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW:            return "FLT_UNDERFLOW";
		case EXCEPTION_ILLEGAL_INSTRUCTION:      return "ILLEGAL_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR:            return "IN_PAGE_ERROR";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW:             return "INT_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION:      return "INVALID_DISPOSITION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_PRIV_INSTRUCTION:         return "PRIV_INSTRUCTION";
		case EXCEPTION_SINGLE_STEP:              return "SINGLE_STEP";
		case EXCEPTION_STACK_OVERFLOW:           return "STACK_OVERFLOW";
		default:                                 return "UNKNOWN_EXCEPTION";
		}
	}

	/**
	 * \brief Get the module name that contains a given address.
	 */
	static void GetModuleFromAddress(PVOID pAddr, char* szModule, size_t nMax)
	{
		HMODULE hMod = NULL;
		if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
							   GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
							   (LPCSTR)pAddr, &hMod))
		{
			GetModuleFileNameA(hMod, szModule, (DWORD)nMax);
		}
		else
		{
			strncpy(szModule, "<unknown>", nMax - 1);
		}
	}

	/**
	 * \brief Write a crash event to ClickerLog.txt with [CRASH] category.
	 *        Used by CrashHandler to log crash details separately from [DEBUG].
	 */
	static void WriteCrashLog(const char* szFormat, ...)
	{
		char szPath[MAX_PATH + 1] = {0};
		GetModuleFileNameA(NULL, szPath, MAX_PATH);

		int i;
		for (i = (int)strlen(szPath) - 1; i >= 0 && szPath[i] != '\\'; --i);
		szPath[i + 1] = '\0';
		strncat(szPath, "ClickerLog.txt", MAX_PATH - strlen(szPath) - 1);

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
			fprintf(f, "[%s] [CRASH] %s\n", szTime, szMessage);
			fclose(f);
		}

		if (s_fConsoleAllocated())
		{
			printf("[%s] [CRASH] %s\n", szTime, szMessage);
		}
	}

	/**
	 * \brief Global crash handler - catches unhandled exceptions in main.exe.
	 *        Reports crash reason, address, module, and registers to
	 *        CrashDump.txt and ClickerLog.txt with [CRASH] category.
	 */
	static LONG WINAPI CrashHandler(EXCEPTION_POINTERS* pExceptionInfo)
	{
		if (!pExceptionInfo || !pExceptionInfo->ExceptionRecord)
		{
			if (s_pfnOldFilter())
				return s_pfnOldFilter()(pExceptionInfo);
			return EXCEPTION_CONTINUE_SEARCH;
		}

		EXCEPTION_RECORD* pRecord = pExceptionInfo->ExceptionRecord;
		CONTEXT* pContext = pExceptionInfo->ContextRecord;

		DWORD dwCode = pRecord->ExceptionCode;
		PVOID pAddr = pRecord->ExceptionAddress;

		char szModule[MAX_PATH + 1] = {0};
		GetModuleFromAddress(pAddr, szModule, MAX_PATH);

		// Log crash to CrashDump.txt and ClickerLog.txt
		LogDebugAction("========== CRASH DETECTED ==========");
		LogDebugAction("Exception: %s (0x%08X)", GetExceptionCodeString(dwCode), dwCode);
		LogDebugAction("Address: 0x%p", pAddr);
		LogDebugAction("Module: %s", szModule);

		// Also write a [CRASH] summary to ClickerLog.txt for unified tracking
		WriteCrashLog("CRASH DETECTED: %s (0x%08X) at 0x%p in %s",
			GetExceptionCodeString(dwCode), dwCode, pAddr, szModule);

		if (dwCode == EXCEPTION_ACCESS_VIOLATION && pRecord->NumberParameters >= 2)
		{
			const char* szOp = (pRecord->ExceptionInformation[0] == 0) ? "reading" : "writing";
			LogDebugAction("Access violation %s address 0x%p", szOp, (PVOID)pRecord->ExceptionInformation[1]);
			WriteCrashLog("Access violation %s address 0x%p", szOp, (PVOID)pRecord->ExceptionInformation[1]);
		}

		if (pContext)
		{
#ifdef _M_IX86
			LogDebugAction("Registers: EAX=0x%08X EBX=0x%08X ECX=0x%08X EDX=0x%08X",
				pContext->Eax, pContext->Ebx, pContext->Ecx, pContext->Edx);
			LogDebugAction("           ESI=0x%08X EDI=0x%08X EBP=0x%08X ESP=0x%08X",
				pContext->Esi, pContext->Edi, pContext->Ebp, pContext->Esp);
			LogDebugAction("           EIP=0x%08X EFLAGS=0x%08X",
				pContext->Eip, pContext->EFlags);
			WriteCrashLog("Registers: EAX=0x%08X EBX=0x%08X ECX=0x%08X EDX=0x%08X ESI=0x%08X EDI=0x%08X EBP=0x%08X ESP=0x%08X EIP=0x%08X",
				pContext->Eax, pContext->Ebx, pContext->Ecx, pContext->Edx,
				pContext->Esi, pContext->Edi, pContext->Ebp, pContext->Esp,
				pContext->Eip);
#endif
		}

		// Try to generate a minidump
		{
			char szDumpPath[MAX_PATH + 1] = {0};
			GetModuleFileNameA(NULL, szDumpPath, MAX_PATH);

			int i;
			for (i = (int)strlen(szDumpPath) - 1; i >= 0 && szDumpPath[i] != '\\'; --i);
			szDumpPath[i + 1] = '\0';
			strncat(szDumpPath, "CrashDump.dmp", MAX_PATH - strlen(szDumpPath) - 1);

			HANDLE hFile = CreateFileA(szDumpPath, GENERIC_WRITE, 0, NULL, 
									   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				MINIDUMP_EXCEPTION_INFORMATION mei;
				mei.ThreadId = GetCurrentThreadId();
				mei.ExceptionPointers = pExceptionInfo;
				mei.ClientPointers = FALSE;

				MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
								 hFile, MiniDumpNormal, &mei, NULL, NULL);
				CloseHandle(hFile);
				LogDebugAction("Minidump saved to: %s", szDumpPath);
			}
		}

		LogDebugAction("========== END CRASH REPORT ==========");

		if (s_pfnOldFilter())
			return s_pfnOldFilter()(pExceptionInfo);

		return EXCEPTION_CONTINUE_SEARCH;
	}
};


#endif //__DebugMode_H
