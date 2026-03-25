#include "stdafx.h"
#include "AntiCheat.h"

#define NameClas HWND WinClasse = FindWindowExA(NULL,NULL,WindowClasse,NULL);



void CN_Fail()
{
    LogsAdded ("[Closed]: Process can't be closed! Exiting Game!");
	Sleep(3000); 
	ExitProcess(0);	
}

void CloseClas(LPCSTR WindowClasse)
{
	NameClas
	if( WinClasse > 0)
	{
		SendMessage(WinClasse, WM_CLOSE,0,0);  //CLOSE HACK WINDOW
	
		LogsAdded ("[Closed]: %s",WindowClasse);
		Sleep (2000);
		NameClas
		if( WinClasse > 0)
		{
			LogsAdded ("[Closed]: %s can't be closed, exiting game!",WindowClasse);
 
			CN_Fail();

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//ClassWindow Close - CN-Close 
//Are NON Case-sensitive - Find it using [Handler 1.5 by Oliver Bock / Classname] 
//CloseClas("PROCEXPL");
////////////////////////////////////////////////////////////////////////////////////////////////

void Close_C(){
CloseClas("PROCEXPL");
//CloseClas("ConsoleWindowClass"); //Mu Graphic speed (windows console) Detect mxmain(fake too)
}

void CloseCla(){

again:
	//CloseClas(); // Antikill
    Close_C();
    Sleep(10538);
goto again;

}

void  Close_Class(){
	CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(CloseCla),NULL,0,0);
}


