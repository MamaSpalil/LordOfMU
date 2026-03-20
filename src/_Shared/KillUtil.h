#ifndef __KillUtil_H
#define __KillUtil_H

#pragma once

#include "DebugOut.h"

/**
 * \brief 
 */
class CKillUtil
{
public:
	/**
	 * \brief Log the reason and kill the game client or crash it if impossible to kill.
	 */
	static void KillGame(const char* reason = "Unknown reason")
	{
		CDebugOut::PrintAlways("[KILL] Game termination requested. Reason: %s", reason);
		CDebugOut::PrintAlways("[KILL] Process will be terminated in 3 seconds ...");

		Sleep(3000);

		CDebugOut::PrintAlways("[KILL] Terminating process now.");

		TerminateProcess(GetCurrentProcess(), 0);
		ExitProcess(0);

		// just crash the process if someone has hijacked TerminateProcess() and ExitProcess()
		__asm {
			mov eax, 12345
			jmp eax
		}
	}
};


#endif //__KillUtil_H