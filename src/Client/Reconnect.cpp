#include "stdafx.h"
#include "Reconnect.h"
#include "Structure.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Protocol.h"

#include "stdafx.h"
#include "Reconnect.h"
#include "Protocol.h"
#include "Packet.h"
#include "MakePreviewCharSet.h"
#include "Other.h"


int *SMAIN_STATE = (int*)0x007E8014;


// ----------------------------------------------------------------------------------------------

ConnectEx gConnectEx;
// ----------------------------------------------------------------------------------------------

DWORD GetAccountInfo_Buff;
DWORD GetAccountInfo_Buff2;
DWORD DisconnectExcept_Buff;
DWORD DisconnectEvent_Buff;
DWORD MenuExit_Buff;
char ReconnectAccount[11];
char ReconnectPassword[11];
int Counter;
DWORD ReconnectStatus = RECONNECT_STATUS_NONE;
DWORD ReconnectProgress = RECONNECT_PROGRESS_NONE;
DWORD ReconnectCurTime = 0;
DWORD ReconnectMaxTime = 0;
DWORD ReconnectCurWait = 0;
DWORD ReconnectMaxWait = 0;
DWORD ReconnectAuthSend = 0;

__declspec(naked) void ReconnectGetAccountInfo()
{
	static DWORD ReconnectGetAccountInfoAddress1 = 0x006DF01D;
	static DWORD ReconnectGetAccountInfoAddress2 = 0x079058F0;
	static DWORD ReconnectGetAccountInfoAddress3 = 0x079059F0;
	static DWORD ReconnectGetAccountInfoAddress4 = 0x0587C3A8;//A4

	memset(ReconnectAccount, 0, sizeof(ReconnectAccount));
	memcpy(ReconnectAccount, (char*)(0x079058F0), sizeof(ReconnectAccount)-1);

	memset(ReconnectPassword, 0, sizeof(ReconnectAccount));
	memcpy(ReconnectPassword, (char*)(0x079059F0), sizeof(ReconnectAccount)-1);
	_asm
	{
		Mov Dword Ptr Ds : [ReconnectGetAccountInfoAddress4], 0x01
		Jmp[ReconnectGetAccountInfoAddress1]
	}

	
}

__declspec(naked) void ReconnectCheckConnection()
{
	static DWORD ReconnectCheckConnectionAddress1 = 0x006D2465;
	static DWORD ReconnectCheckConnectionAddress2 = 0x006D24A0;

	_asm
	{
		Cmp Eax, -1
		Jnz EXIT
		Mov Ecx, ReconnectStatus
		Cmp Ecx, RECONNECT_STATUS_RECONNECT
		Je EXIT
		Jmp[ReconnectCheckConnectionAddress1]
		EXIT:
		Jmp[ReconnectCheckConnectionAddress2]
	}
}
void ReconnectSetInfo(DWORD status, DWORD progress, DWORD CurWait, DWORD MaxWait)
{
	ReconnectStatus = status;
	ReconnectProgress = progress;
	ReconnectCurTime = GetTickCount();
	ReconnectMaxTime = GetTickCount();
	ReconnectCurWait = CurWait;
	ReconnectMaxWait = MaxWait;
	ReconnectAuthSend = ((status == RECONNECT_STATUS_NONE) ? 0 : ReconnectAuthSend);
}
void ReconnectOnCloseSocket()
{
	if (*SMAIN_STATE == 5 && ReconnectStatus != RECONNECT_STATUS_DISCONNECT)
	{

		ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_NONE, 10000, 300000);
		ReconnectAuthSend = 0;
		//обрывает пати при риконекте
		PARTY_MEMBER_COUNT = 0;
//
		*(BYTE*)(oUserPreviewStruct + 0x484)	= 0;
		for (int i = 0; i < 400; i++)
		{
			if (*(BYTE*)(*(int*)PREVIEW_STRUCT + (PREVIEW_STRUCT_SIZE * i)) != 0)
			{
				*(BYTE*)(*(int*)PREVIEW_STRUCT + (PREVIEW_STRUCT_SIZE * i)) = 0;
			}
		}

		memcpy(ReconnectAccount, (void*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x00), sizeof(ReconnectAccount));//test offseta
	}
}
bool CheckSocketPort(SOCKET s)
{
	SOCKADDR_IN addr;
	int addr_len = sizeof(addr);

	if (getpeername(s, (SOCKADDR*)&addr, &addr_len) == SOCKET_ERROR)
	{
		return 0;
	}



	return 1;
}
__declspec(naked) void ReconnectCloseSocket()//ok
{
	static DWORD ReconnectCloseSocketAddress1 = 0x0051020C;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [MAIN_CONNECTION_STATUS]
		Cmp Eax, 0x00
		Je EXIT
		Mov Ecx, Dword Ptr Ss : [Ebp - 0x0C]
		Mov Edx, Dword Ptr Ds : [Ecx + 0x0C]
		Push Edx
		Call[CheckSocketPort]
		TEST EAX, EAX
		Je EXIT
		Call[ReconnectOnCloseSocket]
		EXIT :
		Mov Dword Ptr Ds : [MAIN_CONNECTION_STATUS], 0x00
		Jmp[ReconnectCloseSocketAddress1]
	}
}

__declspec(naked) void ReconnectMenuExitGame()
{
	static DWORD ReconnectMenuExitGameAddress1 = 0x0064D4FD;
	static DWORD ReconnectMenuExitGameAddress2 = 0x007E8EF8;
	_asm
	{
		Mov Eax, ReconnectStatus
		Cmp Eax, RECONNECT_STATUS_RECONNECT
		Jnz EXIT
		Push 0
		Call[ExitProcess]
		EXIT:
		Mov ReconnectStatus, RECONNECT_STATUS_DISCONNECT
		Push ReconnectMenuExitGameAddress2
		Jmp[ReconnectMenuExitGameAddress1]
	}
}
// ----------------------------------------------------------------------------------------------

__declspec(naked) void ReconnectMenuExitGame2()
{
	static DWORD ReconnectMenuExitGame2Address1 = 0x006BC8DB;
	static DWORD ReconnectMenuExitGame2Address2 = 0x007E8590;
	_asm
	{
		Mov Eax, ReconnectStatus
		Cmp Eax, RECONNECT_STATUS_RECONNECT
		Jnz EXIT
		Push 0
		Call[ExitProcess]
		EXIT:
		Mov ReconnectStatus, RECONNECT_STATUS_DISCONNECT
		Push ReconnectMenuExitGame2Address2
		Jmp[ReconnectMenuExitGame2Address1]
	}
}
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------

Naked(DisconnectEvent)
{
	_asm
	{
		mov DisconnectEvent_Buff, ecx
	}
	// ----
	if (DisconnectEvent_Buff == 32)
	{
		if (gConnectEx.m_ConnectState != ConnectExType::OnForceDisconnect)
		{
			gConnectEx.m_ConnectState = ConnectExType::OnReconnect;
		
		}
		else
		{
			SetByte((LPVOID)0x006D246C, 0x75);
		}
		// ----
		_asm
		{
			mov DisconnectEvent_Buff, 0x004DA3AE //ok
			jmp DisconnectEvent_Buff
		}
	}
	// ----
	_asm
	{
		mov DisconnectEvent_Buff, 0x004DA3EC //ok
		jmp DisconnectEvent_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(MenuExit)
{
	gConnectEx.m_ConnectState = ConnectExType::OnForceDisconnect;
	// ----
	_asm
	{
		mov MenuExit_Buff, 0x006BC8ED //OK
		jmp MenuExit_Buff
	}
}
// ----------------------------------------------------------------------------------------------


void ConnectEx::Load()
{
	this->m_ConnectState = ConnectExType::OnLine;
	this->m_LastSendTick = 0;
	this->m_ReconnectTick = 0;
	this->CheckPreviewCharSet = false;
	//this->m_WantMUHelper = false;
	// ----

	SetOp((LPVOID)0x006DF013, (LPVOID)ReconnectGetAccountInfo, ASM::JMP);//0x006DF1F3

	//SetOp((LPVOID)0x006D2460, (LPVOID)ReconnectCheckConnection, ASM::JMP);//0x006D2640
	SetOp((LPVOID)0x00510202, (LPVOID)ReconnectCloseSocket, ASM::JMP);//ok
	SetOp((LPVOID)0x006E670C, (LPVOID)ReconnectMenuExitGame, ASM::JMP);//0x006E68EC
	SetOp((LPVOID)0x006BC8D6, (LPVOID)ReconnectMenuExitGame2, ASM::JMP);

	// ---- 

	//SetRange((LPVOID)0x006BC8D6, 18, ASM::NOP); //OK   OLD- 0x007A889B
	//SetOp((LPVOID)0x006BCAB6, (LPVOID)MenuExit, ASM::JMP);//OK   OLD- 0x007A889B

	//???
	SetRange((LPVOID)0x004DA38B, 9, ASM::NOP); // OK   OLD 0x004D035A
	SetOp((LPVOID)0x004DA38B, (LPVOID)DisconnectEvent, ASM::JMP);// OK   OLD 0x004D035A

	SetByte((LPVOID)0x006D2454, 0xEB);  //OK OLD 0x004DA422

	//SetOp((LPVOID)0x006D2460, (LPVOID)ReconnectCheckConnection, ASM::JMP);



	//test
	SetByte((LPVOID)0x004E28CD, 0xEB);	//-> You are connected to the server	//x3
	//SetByte((LPVOID)0x0050AA65,0xEB);	//-> You are connected to the server  //??
	SetOp((LPVOID)0x00414345, this->CreateConnect, ASM::CALL); //OK
	SetOp((LPVOID)0x004E28A7, this->CreateConnect, ASM::CALL); //OK
	SetOp((LPVOID)0x0050AA3F, this->CreateConnect, ASM::CALL); //OK
	SetOp((LPVOID)0x006CE958, this->CreateConnect, ASM::CALL); //OK
	SetOp((LPVOID)0x006DF239, this->CreateConnect, ASM::CALL); //OK

}
// ----------------------------------------------------------------------------------------------

void ConnectEx::Run()
{
	DWORD Delay = GetTickCount() - this->m_LastSendTick;
	// ----
	
	if (this->m_ConnectState != ConnectExType::OnReconnect)
	{
		return;
	}
	// ----
	this->ClearGame();
	this->DrawProgres();
	
	// ----
	if (Delay < 1000 || *SMAIN_STATE != 5)
	{
		return;
	}
	// ----
	if (pOnLine == 0)
	{
		this->Reconnect();
	}
	// ----
	if (pOnLine == 1)
	{
	
		this->Relogin();
	}
	// ----
	this->m_LastSendTick = GetTickCount();
}
// ----------------------------------------------------------------------------------------------

int ConnectEx::CreateConnect(char * IP, WORD Port)
{
	if (Port == 44405)
	{
		sprintf(gConnectEx.m_CSIP, "%s", IP);
		gConnectEx.m_CSPort = Port;
	}
	// ----
	sprintf(gConnectEx.m_GSIP, "%s", IP);
	gConnectEx.m_GSPort = Port;
	// ----
	return pCreateConnect(IP, Port);
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::ClearGame()
{
	*(BYTE*)(oUserPreviewStruct + 0x484)	= 0;
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::RestoreGame()
{

	// ----
	this->m_LastSendTick = 0;
	this->m_ConnectState = ConnectExType::OnLine;
	this->CheckPreviewCharSet = false;
	// ----
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::DrawProgres()
{
	pBlendSwitch(1);
	float StartX = 230;
	// ----
	Counter++;
	// ----
	if (Counter > 150)
	{
		Counter = 0;
	}
	//PrintImages(0x934, StartX, 176.0, 192.0, 47.25, 0.0, 0.0, 0.6875, 0.8828125, 1, 1, 0.0);
	// ----
	PrintImages(0x860,  241.5625, 201.0, 164.0, 13.0625, 0.0, 0.0, 0.78149414, 1.0727539, 1, 1, 0.0);
	PrintImages(0x8C3, 245.0, 204.0, Counter, 6.5, 0.0, 0.0, 0.5, 0.5, 1, 1, 0.0);
	// ----
	 if (pOnLine == 0)
	 {
	 	 pSetBlend(true);
		 pDrawTextColor = eDrawText::TextColor::Orange;
		 pDrawTextBackground = eDrawText::TextBackground::Invisible;
		 MU_DrawTextOut(StartX + 65, 202, "Reconnecting...", 0, 0, 0);
		 pGLSwitch();

	
	 }
	 else if (pOnLine == 1)
	 {

		 pSetBlend(true);
		 pDrawTextColor = eDrawText::TextColor::Orange;
		 pDrawTextBackground = eDrawText::TextBackground::Invisible;
		 MU_DrawTextOut(StartX + 80, 202, "Relogin...", 0, 0, 0);
		 pGLSwitch();

	 }
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::Reconnect()
{
	PMSG_ANS_SERVER_INFO pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xF4, 3, sizeof(PMSG_ANS_SERVER_INFO));
	pRequest.Port = this->m_GSPort;
	memcpy(pRequest.Ip, this->m_GSIP, 16);
	DataRecv(pRequest.h.headcode, (BYTE*)&pRequest, pRequest.h.size, 0);
}
// ----------------------------------------------------------------------------------------------
void ConnectEx::SetAccInformation(SDHP_RECONNECT_IDPASS * RecInfo)
{
	memcpy(this->m_AccountID, RecInfo->Id, 10);
	memcpy(this->m_Password, RecInfo->Pass, 10);
	memcpy(this->m_Name, RecInfo->Name, 10);
}

void ConnectEx::Relogin()
{
	lpCharObj lpPlayer			= pUserObjectStruct;
	CONNECTEX_LOGIN pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 12, sizeof(CONNECTEX_LOGIN));
	memcpy(pRequest.AccountID, this->m_AccountID, 10);
	memcpy(pRequest.Password, this->m_Password, 10);
	memcpy(pRequest.Name, this->m_Name, 10);
	pRequest.TickCount = GetTickCount();
	DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::Disconnect()
{
	CONNECTEX_CLOSE pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 13, sizeof(CONNECTEX_CLOSE));
	DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// ----------------------------------------------------------------------------------------------

