#include "stdafx.h"
#include "AntiCheat.h"




void Close_W_fail()
{
	LogsAdded ("[Closed]: Process can't be closed! Exiting Game!");
	Sleep(3000); 
	ExitProcess(0);	
}

bool CloseWin(LPCSTR WindowTitle)
{
	HWND WinTitle = FindWindowA(NULL,WindowTitle);
	if( WinTitle > 0)
	{
		SendMessage(WinTitle, WM_CLOSE,0,0); 
	
		LogsAdded ("[Closed]: %s",WindowTitle);

	
		Sleep (2000);
		HWND WinTitle = FindWindowA(NULL,WindowTitle);
		if( WinTitle > 0)
		{
	
	
			LogsAdded ("[Closed]: %s can't be closed, exiting game!",WindowTitle);
			Close_W_fail();
	
			return false;
		}

	}
	return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////
//Window close scan - W-Close 
//Are Case-sensitive - Find it just looking window name or using [Handler 1.5 by Oliver Bock] 
//TitleWindow("xxxx");
////////////////////////////////////////////////////////////////////////////////////////////////
void Close_Winx(){
CloseWin("Cheat Engine 5.0");
}

void CloseWin_(){
	
again:
    Close_Winx();
    Sleep(10536);
    goto again;

}

void Close_Window(){
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(CloseWin_),NULL,0,0);
}

