#ifndef G_CONSOLE_H
#define G_CONSOLE_H
// -----------------------------------------------------------------------
//#include "windows.h"
//#include <stdio.h>
// -----------------------------------------------------------------------
#define CONSOLETITLE "[ MU ONLINE ] Console Application."
// -----------------------------------------------------------------------
enum CNSL_E
{
	CONSOLE_RED			    = 1,
	CONSOLE_GREEN			= 2,
	CONSOLE_BLUE			= 3,
	CONSOLE_CYAN			= 4,
	CONSOLE_YELLOW			= 5,
};

class Console
{
public:
	Console();
	~Console();
	void InitCore();
	void ConsoleOutput(int Color, const char* Format, ...);
	int AddMessageToConsole(char* Buffer);
	HANDLE Handle(BOOL Input);
	void LoadConsoleCommands(char* gImput);
};
// -----------------------------------------------------------------------
extern Console g_Console;
// -----------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------
