#ifndef __KillUtil_H
#define __KillUtil_H

#pragma once


/**
 * \brief 
 */
class CKillUtil
{
public:
	/**
	 * \brief Kill the game client or crash it if impossible to kill.
	 */
	static void KillGame()
	{
		Sleep(3000);

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