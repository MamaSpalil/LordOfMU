#include "stdafx.h"
#include "AntiCheatPlus.h"
#include "Splash.h"
#include "Protocol.h"
#include "AntiCheat.h"
#if(ANTI_CHEAT_PLUS==TRUE)
// ----------------------------------------------------------------------------------------------

AntiCheatPlus g_AntiCheatPlus;
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::Init()
{
	ZeroMemory(&this->TitleName, sizeof(this->TitleName));
	ZeroMemory(&this->ClassName, sizeof(this->ClassName));
	ZeroMemory(&this->InjectName, sizeof(this->InjectName));
	ZeroMemory(&this->ProcessName, sizeof(this->ProcessName));

	this->ExitCheat = false;
	this->ExitCount = 0;
#if(ANTI_CHEAT_PLUS_CRC)
	this->m_UpdFileCount = 0;
#endif
	this->TitleCount = 0;
	this->ClassCount = 0;
	this->InjectCount = 0;
	this->ProcessCount = 0;

	this->DumpActive = false;
	this->TitleActive = false;
	this->ClassActive = false;
	this->InjectActive = false;
	this->ProcessActive = false;
	this->EliteActive = false;
	this->SpeedActive = false;
#if(ANTI_CHEAT_PLUS_CRC)
	this->CRCActive = false;
#endif

	this->DumpTime = 10000;
	this->TitleTime = 10000;
	this->ClassTime = 10000;
	this->InjectTime = 10000;
	this->ProcessTime = 10000;
	this->SpeedTime = 10000;

	this->hdd_id = 0;

	GetVolumeInformationA(DISK_DIR,0,0,&this->hdd_id,0,0,0,0); 
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::Load()
{
	this->Init();

	CreateDirectory("Log", 0);

	//this->Splash();

//	SetRange((LPVOID)0x00788F9D, 5, ASM::NOP);
//	SetOp((LPVOID)0x00788F9D,  (LPVOID)this->EliteClickerCheck, ASM::CALL);

	CreateThread(NULL,0,g_AntiCheatPlus.DampTimer,NULL,0,NULL);
	CreateThread(NULL,0,g_AntiCheatPlus.ClassTimer,NULL,0,NULL);
	CreateThread(NULL,0,g_AntiCheatPlus.InjectTimer,NULL,0,NULL);
	CreateThread(NULL,0,g_AntiCheatPlus.ProcessTimer,NULL,0,NULL);
	CreateThread(NULL,0,g_AntiCheatPlus.TitleTimer,NULL,0,NULL);
	CreateThread(NULL,0,g_AntiCheatPlus.SpeedTimer,NULL,0,NULL);
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::Splash()
{
    CSplash splash1(TEXT(".\\Data\\Custom\\PhoenixProject.bmp"), RGB(128, 128, 128));
    splash1.ShowSplash();
    Sleep(3000);

    splash1.CloseSplash();
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::GC_Recv(LPBYTE aRecv)
{
	PMSG_AH_SUB * aType = (PMSG_AH_SUB*)aRecv;

	BYTE ProtocolType = aType->type;

	switch(ProtocolType)
	{
	case eAH_Config:
		{
			PMSG_AH_CONFIG * aConfig = (PMSG_AH_CONFIG*)aRecv;

			this->DumpActive = aConfig->DumpActive;
			this->TitleActive = aConfig->TitleActive;
			this->ClassActive = aConfig->ClassActive;
			this->InjectActive = aConfig->InjectActive;
			this->ProcessActive = aConfig->ProcessActive;
			this->EliteActive = aConfig->EliteActive;
			this->MultiWindows = aConfig->MultiWindows;

			this->DumpTime = aConfig->DumpTime;
			this->TitleTime = aConfig->TitleTime;
			this->ClassTime = aConfig->ClassTime;
			this->InjectTime = aConfig->InjectTime;
			this->ProcessTime = aConfig->ProcessTime;

			if(!this->MultiWindows)
			{
				this->MultiWindowCheck();
			}
		}
		break;
	case eAH_Init:
		{
			this->Init();
		}
		break;
	case eAH_Title:
		{
			PMSG_CheatBase * pName = (PMSG_CheatBase*)aRecv;
			memcpy(this->TitleName[this->TitleCount], pName->Name, sizeof(this->TitleName[this->TitleCount]));
			this->TitleCount++;
		}
		break;
	case eAH_Class:
		{
			PMSG_CheatBase * pName = (PMSG_CheatBase*)aRecv;
			memcpy(this->ClassName[this->ClassCount], pName->Name, sizeof(this->ClassName[this->ClassCount]));
			this->ClassCount++;
		}
		break;
	case eAH_Inject:
		{
			PMSG_CheatBase * pName = (PMSG_CheatBase*)aRecv;
			memcpy(this->InjectName[this->InjectCount], pName->Name, sizeof(this->InjectName[this->InjectCount]));
			this->InjectCount++;
		}
		break;
	case eAH_Process:
		{
			PMSG_CheatBase * pName = (PMSG_CheatBase*)aRecv;
			memcpy(this->ProcessName[this->ProcessCount], pName->Name, sizeof(this->ProcessName[this->ProcessCount]));
			this->ProcessCount++;
		}
		break;
	}
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::CG_Cheater(char* szText)
{
	// ----
	PMST_CheaterUser pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 0xFF, sizeof(PMST_CheaterUser));
	pRequest.Result = TRUE;
	memcpy(pRequest.CheatText, szText, sizeof(pRequest.CheatText));
	DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::CG_HddSend()
{
	CG_PMSG_HDDKEY pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 0xFE, sizeof(CG_PMSG_HDDKEY));
	pRequest.hdd_id = this->hdd_id;
	DataSend((LPBYTE)&pRequest, pRequest.h.size);
}

// ----------------------------------------------------------------------------------------------

DWORD WINAPI AntiCheatPlus::DampTimer(LPVOID lpParam)
{
	while(true)
	{
		if(g_AntiCheatPlus.DumpActive)
		{
			g_AntiCheatPlus.Dump();
		}
		
		Sleep(g_AntiCheatPlus.DumpTime);
	}
	return 0;
}
// ----------------------------------------------------------------------------------------------

DWORD WINAPI AntiCheatPlus::TitleTimer(LPVOID lpParam)
{
	while(true)
	{
		if(g_AntiCheatPlus.TitleActive)
		{
			for(int i=0; i < g_AntiCheatPlus.TitleCount; i++)//	for(int i=0; i < g_AntiCheatPlus.TitleCount; i++)
			{
				g_AntiCheatPlus.TitleWindowCheck(g_AntiCheatPlus.TitleName[i]);
			}
		}
		Sleep(g_AntiCheatPlus.TitleTime);
	}
	return 0;
}
// ----------------------------------------------------------------------------------------------

DWORD WINAPI AntiCheatPlus::ClassTimer(LPVOID lpParam)
{
	while(true)
	{
		if(g_AntiCheatPlus.ClassActive)
		{
			for(int i = 0; i < g_AntiCheatPlus.ClassCount; i++)
			{
				g_AntiCheatPlus.ClassWindowCheck(g_AntiCheatPlus.ClassName[i]);
			}
		}
		Sleep(g_AntiCheatPlus.ClassTime);
	}
	return 0;
}
// ----------------------------------------------------------------------------------------------

DWORD WINAPI AntiCheatPlus::InjectTimer(LPVOID lpParam)
{
	while(true)
	{
		if(g_AntiCheatPlus.InjectActive)
		{
			for(int i = 0; i < g_AntiCheatPlus.InjectCount; i++)
			{
				g_AntiCheatPlus.InjectCheck(g_AntiCheatPlus.InjectName[i]);
			}
		}
		Sleep(g_AntiCheatPlus.InjectTime);
	}
	return 0;
}
// ----------------------------------------------------------------------------------------------

DWORD WINAPI AntiCheatPlus::ProcessTimer(LPVOID lpParam)
{
	while(true)
	{
		if(g_AntiCheatPlus.ProcessActive)
		{
			for(int i = 0; i < g_AntiCheatPlus.ProcessCount; i++)
			{
				g_AntiCheatPlus.ProcessIDCheck(g_AntiCheatPlus.ProcessName[i]);
			}
		}
		Sleep(g_AntiCheatPlus.ProcessTime);
	}
	return 0;
}
// ----------------------------------------------------------------------------------------------

DWORD WINAPI AntiCheatPlus::SpeedTimer(LPVOID lpParam)
{
	while(true)
	{
		if(g_AntiCheatPlus.SpeedActive)
		{
			g_AntiCheatPlus.Speed();
		}
		
		Sleep(g_AntiCheatPlus.SpeedTime);
	}
	return 0;
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::ClassWindowCheck(LPCSTR WindowClasse)
{
	HWND WindowHwnd;
	//HWND WinClasse = FindWindowExA(NULL,NULL,WindowClasse,NULL);
	//if( WinClasse > 0)
	if((WindowHwnd=FindWindowExA(0,0,WindowClasse,0)) != 0)
	{
		this->Write(CLASS_WINDOW_TEXT,WindowClasse);
	}
	//TitleWindowMain();
	/*if((WindowHwnd=FindWindowExA(0,0,0,WindowClasse)) != 0)
	{
		this->Write(CLASS_WINDOW_TEXT,WindowClasse);
	}*/
}
// ----------------------------------------------------------------------------------------------

bool AntiCheatPlus::DumpCheck(HANDLE hProcess) 
{
	for( int i = 0; i < MAX_PROCESS_DUMP; i++ )
	{
		char aTmpBuffer[MAX_DUMP_SIZE];
		SIZE_T aBytesRead = 0;
		ReadProcessMemory(hProcess, (LPCVOID)g_ProcessesDumps[i].m_aOffset, (LPVOID)aTmpBuffer, sizeof(aTmpBuffer), &aBytesRead);

		if(memcmp(aTmpBuffer, g_ProcessesDumps[i].m_aMemDump, MAX_DUMP_SIZE) == 0)
		{
			this->Write(DAMP_LOG_TEXT, g_ProcessesDumps[i].Name);
			return true;
		}
	}
	return false;
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::Dump() 
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if(Process32First(hProcessSnap, &pe32))
		{
			do
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				if(hProcess != NULL)
				{
					if(this->DumpCheck(hProcess))
					{
					}
				}
			}
			while(Process32Next(hProcessSnap, &pe32));
		}
	}
	CloseHandle(hProcessSnap);
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::Speed() 
{
	if( *(unsigned long*)QueryPerformanceCounter != 2337669003 )
	{
		this->Write("Speed Hack detected!");
	
	}

	int ispidi = 0;

again2:
	int timer = ::GetTickCount();
again:
	Sleep(500); 
	ispidi++;
	//cout << isp <<endl;
	if (ispidi != 20)
	{
		goto again;
	}
	else
	{
		ispidi = 0;
		int timeElapsed = ::GetTickCount() - timer; 
		if ("%d", timeElapsed > 30500)//30500
		{ 
			this->Write("Speed Hack or System Freeze detected!");
		}
		else
		{
			goto again2;
		}
	}
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::InjectCheck(LPCSTR InjectHandle)
{
	if(GetModuleHandle(InjectHandle))
	{
		this->Write(INJECT_TEXT,InjectHandle);
	}
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::ProcessIDCheck(LPCSTR ProcName)
{
	PROCESSENTRY32 pe32;
    HANDLE hSnapshot = NULL;

	pe32.dwSize = sizeof( PROCESSENTRY32 );
    hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

    if( Process32First( hSnapshot, &pe32 ))
	{
        do
		{
            if( strcmp( pe32.szExeFile, ProcName ) == 0 )
            {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);  // Close detected process
				TerminateProcess(hProcess,NULL);                                               // Close detected process

				this->Write(PROCESS_ID_TEXT, ProcName);
            }
        }
		while( Process32Next( hSnapshot, &pe32 ) );
    }
    if( hSnapshot != INVALID_HANDLE_VALUE )
	{
        CloseHandle( hSnapshot );   
	}
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::TitleWindowCheck(LPCSTR WindowTitle)
{
	HWND WindowHwnd;
	//HWND WinClasse = FindWindowExA(NULL,NULL,WindowClasse,NULL);
	//if( WinClasse > 0)
	if((WindowHwnd=FindWindowExA(0,0,WindowTitle,0)) != 0)
	{
		this->Write(TITLE_WINDOW_TEXT,WindowTitle);
	}
	//HWND WinTitle = FindWindowA(NULL,WindowTitle);
	//if( WinTitle > 0)
	//{
	//	this->Write(TITLE_WINDOW_TEXT,WindowTitle);
	//}
	///*HWND WindowHwnd;
	//if((WindowHwnd=FindWindowExA(0,0,0,WindowTitle)) != 0)
	//{
	//	this->Write(TITLE_WINDOW_TEXT,WindowTitle);
	//}*/
}
// ----------------------------------------------------------------------------------------------
char gWindowTitle[50];
void AntiCheatPlus::MultiWindowCheck()
{
	CreateMutexA(NULL, TRUE, gWindowTitle);
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		g_AntiCheatPlus.Write(MULTI_WIN_TEXT);
	}
}
// ----------------------------------------------------------------------------------------------

//void AntiCheatPlus::EliteClickerCheck(LPVOID This, int PosX, int PosY, LPCTSTR Text, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
//{
//	if(!g_AntiCheatPlus.EliteActive)
//	{
//		char tt[] = " > meet player ";
//		if(!strncmp(Text, tt, strlen(tt)))
//		{
//			g_AntiCheatPlus.Write("Hack Elite Clicker");
//		}
//	}
//	pDrawText(This, PosX, PosY, (char*)Text, nCount, nTabPositions, lpnTabStopPositions,  nTabOrigin);
//}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::Write(LPSTR Text, ...)
{
	char TimeBuff[100] = { 0 };
	char TextBuff[1024] = { 0 };
	char LogBuff[1024] = { 0 };
	// ----
	va_list ArgPointer;
    va_start(ArgPointer, Text);
	vsprintf(TextBuff, Text, ArgPointer);
	va_end(ArgPointer);
	// ----

	SYSTEMTIME Time;
	GetLocalTime(&Time);
	sprintf_s(TimeBuff, "[%02d:%02d:%02d] ", Time.wHour, Time.wMinute, Time.wSecond);
	sprintf_s(LogBuff, "%s %s\n", TimeBuff, TextBuff);
	char LogFile[MAX_PATH] = { 0 };
	sprintf_s(LogFile, ".\\Log\\[%02d-%02d-%04d] Anti-Cheat.log", Time.wDay, Time.wMonth, Time.wYear);
	FILE* Input = fopen(LogFile, "a");
	if(Input)
	{
		fprintf(Input, LogBuff);
		fclose(Input);
	}

	if(this->ExitCheat == FALSE)
	{
		this->ExitCheat = TRUE;

		this->CG_Cheater(TextBuff);
		
		//g_AntiCheatPlus.Exit();
		//MessageBoxA(NULL, TextBuff, AH_TITLE, ERROR);
		//Sleep(3000);
	}
}
// ----------------------------------------------------------------------------------------------

void AntiCheatPlus::Exit()
{
	CreateThread(NULL,0,g_AntiCheatPlus.End,0,0,NULL);
}
// ----------------------------------------------------------------------------------------------

DWORD WINAPI AntiCheatPlus::End(LPVOID lpParam)
{
	g_AntiCheatPlus.ExitCount = 0;
	while(true)
	{
		if(g_AntiCheatPlus.ExitCount >= 3)
		{
			ExitProcess(0);
		}
		g_AntiCheatPlus.ExitCount++;
		Sleep(1000);
	}
}
// ----------------------------------------------------------------------------------------------

#if(ANTI_CHEAT_PLUS_CRC)
void AntiCheatPlus::GC_CRCRecv(PMSG_CRCBase* lpMsg)
{
	DWORD dwCRC = g_Crc32.Get(lpMsg->path);

	gConsole.Output(cGREY, "%X > %X = %s", dwCRC, lpMsg->key, lpMsg->path);

	if(dwCRC != lpMsg->key)
	{
		this->Write("%s File Error", lpMsg->path);
		//this->m_UpdFileCount++;
		//this->GCReqUpdateFile(lpMsg->num);
	}	
}

void AntiCheatPlus::GCReqUpdateFile(DWORD id)
{
	PMSG_UPDATEFILE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0xFD, sizeof(PMSG_UPDATEFILE));
	pMsg.num = id;
	gProtocol.DataSend((LPBYTE)&pMsg, pMsg.h.size);
}

void AntiCheatPlus::CGAnsSendFile(PMSG_FILESEND* lpMsg)
{
	gConsole.Output(cGREY, "%d %d", lpMsg->Stage, lpMsg->Size);

	if(lpMsg->Stage == 1)
	{
		if(this->m_FileHandle == true)
		{
			return;
		}

		this->m_handle = CreateFile(lpMsg->Buff, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0x80, NULL);

		if(this->m_handle == INVALID_HANDLE_VALUE)
		{
			return;
		}

		this->m_FileHandle = true;
	}
	else if(lpMsg->Stage == 2)
	{
		if(this->m_FileHandle == false)
		{
			return;
		}

		DWORD dwPackSize;

		WriteFile(this->m_handle, lpMsg->Buff, lpMsg->Size, &dwPackSize, 0);
	}
	else if(lpMsg->Stage == 3)
	{
		if(this->m_FileHandle == false)
		{
			return;
		}

		CloseHandle(this->m_handle);

		this->m_FileHandle = false;
		this->m_UpdFileCount--;
	}

	if(this->m_UpdFileCount == 0)
	{

	}
}

void AntiCheatPlus::Draw()
{

}

#endif
// ----------------------------------------------------------------------------------------------

#endif