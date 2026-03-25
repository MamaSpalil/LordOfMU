#include "StdAfx.h"
#include "Interface.h"
#include "Reconnect.h"
#include "TMemory.h"
#include "Other.h"
#include "Protocol.h"
#include "Structure.h"
#include "InterfaceEx.h"
#include "AttackHelper.h"
#include "CameraEx.h"
#include "ResetSystem.h"
#include "User.h"
#include "Ranking.h"
#include "RageSystem.h"
#include "CustomMenu.h"
#include "JewelsBank.h"
#include "AdvancedStats.h"
#include "DungeonSiege.h"
#include "Controller.h"
#include "ExTeleport.h"
#include "ButtonChar.h"
#include "SelectChar.h"
#include "LuckyWheel.h"
#include "AutoClickerUI.h"
#include "LordOfMUBridge.h"

CInterface * gInterfaces = new CInterface();

pDrawText			MU_DrawTextOut		= (pDrawText)		(iDraw_Text);
pMU_ADDSimple          MU_ADDSimple = (pMU_ADDSimple)MU_ADDSimple_Offset;
pDrawInterface		MU_DrawInterface	= (pDrawInterface)	(0x006037E0);
//pDrawInterfaceSelectCharacters		MU_DrawInterfaceSelectCharacters = (pDrawInterfaceSelectCharacters)(iDraw_Interface2);

void __cdecl cDrawInterfaceSelectCharacter123()
{
	////MU_DrawInterfaceSelectCharacters13();
	//	pSetBlend(true);
	//	char szTemp345[50][256];
	//	int PositionX1 = 291;
	//	int PositionY1 = 416;
	//	int Width = 25;
	//	int Height = 25;
	//	glColor3f(1.0, 1.0, 1.0); //White|
	//	pDrawImage2(3695,PositionX1,PositionY1,60,16,0.0,0.0,0.8128125, 0.8228125, 1, 1, 0.0);//0.6875 / 0.8828125
	//	if(pCursorX >= PositionX1
	//	&&	(float)pCursorX <= (float)PositionX1 + 60
	//	&&	pCursorY >= PositionY1 - 1 
	//	&&	pCursorY < PositionY1 + 16 )
	//	{
	//		pDrawImage2(3695,PositionX1,PositionY1,60,16,0.0,0.0,0.8128125, 0.8228125, 1, 1, 0.0);//0.6875 / 0.8828125
	//	}
	//	pDrawTextBackground = eDrawText::TextBackground::Invisible;
	//	sprintf_s(szTemp345[0],"Create Account");
	//	MU_DrawTextOut(PositionX1-18,PositionY1+4,szTemp345[0],GetRealWidth(100), 1, 0);


	//	//glColor3f(1.0, 1.0, 1.0); //White|
	//	//pDrawImage2(3695,PositionX1,PositionY1+20,60,16,0.0,0.0,0.8128125, 0.8228125, 1, 1, 0.0);//0.6875 / 0.8828125
	//	//if(pCursorX >= PositionX1
	//	//&&	(float)pCursorX <= (float)PositionX1 + 60
	//	//&&	pCursorY >= PositionY1+20 - 1 
	//	//&&	pCursorY < PositionY1+20 + 16 )
	//	//{
	//	//	pDrawImage2(3695,PositionX1,PositionY1+20,60,16,0.0,0.0,0.8128125, 0.8228125, 1, 1, 0.0);//0.6875 / 0.8828125
	//	//}
	//	//pDrawTextBackground = eDrawText::TextBackground::Invisible;
	//	//sprintf_s(szTemp345[0],"Terms of Use");
	//	//MU_DrawTextOut(PositionX1-18,PositionY1+4+20,szTemp345[0],GetRealWidth(100), 1, 0);

	//	pGLSwitchBlend();
	//	glEnd();

	////CameraOff123();
}

void CInterface::Init()
{
	SetOp((LPVOID)0x0060159D, (LPVOID)this->MainProc, ASM::CALL);
	//SetOp((LPVOID)0x006038C6, (LPVOID)this->MainProc2, ASM::CALL);
	DrawInterfaceBind();
	SetHook((LPVOID)cDrawInterfaceSelectCharacter123, (LPVOID)0x006CF4A2, ASM::CALL);

}

void __cdecl cDrawInterfaceSelectCharacter()
{
	//MU_DrawInterfaceSelectCharacters();
	
	//CameraDisable();
}

void CInterface::MainProc()
{
	MU_DrawInterface();
	glColor3f(1.0f, 1.0f, 1.0f);//Green
		gInterfaces->RightMiniInfoY = 430;
	if (TpList == true)
	{
		TeleportList();
	}

UPDATE_FPS();
	// gAutoClickerUI.Draw() removed — LordOfMU's ImGui overlay provides the
	// HUD buttons (Settings/Start-Stop/History) via D3D9 EndScene hook.
	SetWindowText((*(HWND*)(MAIN_WINDOW)), "A2K - MuOnline Season III Ultimate");
#if(_JEWELS_BANK_)
	
	gJewelsBank.JewelsBankDraw();
#endif

	gObjUser.Refresh();

	gConnectEx.Run();

	// gAttackHelper->DrawSelectMode() removed — LordOfMU's ImGui overlay
	// provides the attack mode selection UI.
	gRanking.Draw();
//
	g_ResetSystem.DrawReset();
	g_ResetSystem.DrawGrand();
#ifdef MULTI_WAREHOUSE
	gInterfaces->DrawBankCountWindow();//ne pawet
#endif
#if(EVENT_DUNGEON_SIEGE)
	g_DungeonSiege.Draw();
#endif
	gExInterface->DrawTimer();
	gCamera->MainProc();
	// gAttackHelper->MainProc() removed — LordOfMU AutoClicker (CClickerJob)
	// handles the attack loop.
	gExInterface->DrawNavBar();
	gMenuSystem.rDrawInterface();//ne pawet
#if(DEV_DAMAGE_TABLE)
	//DrawRightMiniInfoMapQuest()
	gInterfaces->m_EnableDamageTable = true;
	gInterfaces->DrawDamageTable();
#endif
#if(CUSTOM_VALUE_MENU)//vot
	gInterfaces->DrawValueMenu();
	gInterfaces->DrawGuildValueMenu();
#endif
	//RenderBitmap(260, 260.0, 0.0, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0);
	Test123();
	gRageSystem.rDrawInterface();

	gLuckyWheel.MainProc();

	//#if(CUSTOM_VALUE_MENU)
	//{
	//	gInterfaces->BindValueMenu();
	//	gInterfaces->BindGuildValueMenu();
	//}
	//#endif
	//DrawButtonsCharacters();
	//pMainWorkProc();
	
}
//CInterface::CInterface()
//{
//	//ZeroMemory(this->Data, sizeof(this->Data));
//	ZeroMemory(this->WindowEx, sizeof(this->WindowEx));
//}
void CInterface::MainProc2(int a1, float a2, float a3, float a4, float a5, int a6, int a7, float a8, float a9, char a10, char a11, float a12)
{
	//pDrawImage2(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	gInterfaces->RightMiniInfoY = 430;
	//CordRun();
	//DrawButtonsCharacters();
	//if (TpList == true)
	//{
	//	TeleportList();
	//}
	
		//gConnectEx.Run();

		//gAttackHelper->DrawSelectMode();
	
#if(QUEST_WORLD_ENABLE == 1)
	gQuestWorld.MainProc();
#endif
	gExInterface->DrawTimer();

	gExInterface->DrawNavBar();

//	UPDATE_FPS();
//	SetWindowText((*(HWND*)(MAIN_WINDOW)), "Season 3 Final World of MuOnline");
//#if(_JEWELS_BANK_)
//	
//	gJewelsBank.JewelsBankDraw();
//#endif
//	
//	gRanking.Draw();
//
//	g_ResetSystem.DrawReset();
//	g_ResetSystem.DrawGrand();
#ifdef MULTI_WAREHOUSE
	gInterfaces->DrawBankCountWindow();
#endif
	//gMinimap->Init();
#if(EVENT_DUNGEON_SIEGE)
	g_DungeonSiege.Draw();
#endif
#if(DEV_DAMAGE_TABLE)
	//DrawRightMiniInfoMapQuest()
	gInterfaces->m_EnableDamageTable = true;
	gInterfaces->DrawDamageTable();
#endif
	gRageSystem.rDrawInterface();

	
//	g_StatsAdvance.DrawInfos();
	/*gMenuSystem.rDrawInterface();*/
	
}
void CordRun()
{
	char szBuffer[256] = {0};
	// ---
	pSetBlend(true);
	// ---
	sprintf(szBuffer, "Cord X : %d | Cord Y : %d", pCursorX-7, pCursorY-6);
	// ---
	//pDrawText(0, 0, szBuffer);
	pDrawToolTipEx(0, 0, szBuffer);
}
void DrawInterfaceBind()
{
	gInterfaces->BindObject(eBankClose, 0x118, 16.0, 16.0, -1, -1, 0.75, 0.75);
#ifdef MULTI_WAREHOUSE
	gInterfaces->BindObject(eBankCount, 0x9F6, 173.0, 245.125, -1, -1, 0.7734375, 0.5683441);
	gInterfaces->BindObject(eBankAdd, 0x120, 16.0, 16.0, -1, -1, 0.75, 0.75);
	//gInterfaces->BindObject(eBankClose, 0x118, 16.0, 16.0, -1, -1, 0.75, 0.75);

	gInterfaces->BindObject(eBankNum1, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum2, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum3, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum4, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum5, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum6, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum7, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum8, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum9, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum10, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum11, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
	gInterfaces->BindObject(eBankNum12, 0x854, 40.0, 14.0, -1, -1, 1.0, 1.0);
#endif
	gInterfaces->BindObject(eResetClose, 0x118, 16.0, 16.0, -1, -1, 0.75, 0.75);
	gInterfaces->BindObject(eButtonMenu, 0x852, 32.0, 14.5625, -1, -1, 0.8671875, 0.73046875);
	gInterfaces->BindObject(eButtonCharacters_BackPage, 0xDFB, 12.375, 14.5625, -1, -1, 0.6484375, 0.7265625);
	

	//PrintImages(0x852, 306.125, 385.0, 32.0, 14.5625, 0, 0, 0.8671875, 0.73046875, 1, 1, 0.0);

	gInterfaces->BindObject(eButtonCharacters_NextPage, 0xDFA, 12.375, 14.5625, -1, -1, 0.6484375, 0.7265625);
	gRageSystem.Load();

	g_StatsAdvance.Bind();

#if(CUSTOM_VALUE_MENU)
	{
		gInterfaces->BindValueMenu();
		gInterfaces->BindGuildValueMenu();
	}
#endif
//	gInterfaces->BindObject(eRageTable, 0xE01, 640, 402, -1, -1, 0.9, 0.9);
//	gInterfaces->BindObject(eRagePointTable, 0x7B4E, 100, 100, -1, -1, 0.9, 0.9);
	
	//gInterfaces->BindObjects(eRageClose, 308536, 36, 29, 600, 10);
	//gRageSystem.Enable = true;
	

//#ifdef Custom_menu
//	gInterface.BindObject(eButtonMenu,0xA12, 32.0, 14.5625, -1, -1, 0.8671875, 0.73046875);
//	gInterface.BindObject(eeRageSystem, 0xA1E, 57, 17, 138, 0);
//	//gInterface.BindObject(eeRageSystem, 0xA1E, 57, 17, 1,1,  1, 1);
//	//gInterface.BindObject(eeRageSystem, 0xA1E, 57, 17, 112, 0);	//57, 17, 138, 0
//	//gInterface.BindObject(eeRageTable, 30850, 640, 480, 20, 20);
//	//gInterface.BindObjects(eeRageClose, 2591, 36, 29, 450, 50);
//#endif
	//PrintImages(0x854, 241.0, 179.5625, 40.0, 14.0, 0.0, 0.0, 1.0, 1.0, 0, 0, 0.0);
	//PrintImages(0x118, 377.875, 295.125, 16.0, 16.0, 0.0, 0.0, 0.75, 0.75, 0, 0, 0.0);
	//PrintImages(0x120, 283.125, 263.125, 16.0, 16.0, 0.0, 0.0, 0.75, 0.75, 0, 0, 0.0);
	//PrintImages(0x9F6, 231.4375, 76.5, 173.0, 245.125, 0.0, 0.0, 0.7734375, 0.5683441, 1, 1, 0.0);
}

void CInterface::BindObject(short MonsterID, DWORD ModelID, float Width, float Height, float X, float Y,float ScaleX,float ScaleY)
{
	this->Data[MonsterID].EventTick	= 0;
	this->Data[MonsterID].OnClick	= false;
	this->Data[MonsterID].OnShow	= false;
	this->Data[MonsterID].ModelID	= ModelID;
	this->Data[MonsterID].Width		= Width;
	this->Data[MonsterID].Height	= Height;
	this->Data[MonsterID].X			= X;
	this->Data[MonsterID].Y			= Y;
	this->Data[MonsterID].MaxX		= X + Width;
	this->Data[MonsterID].MaxY		= Y + Height;
	this->Data[MonsterID].ScaleX	= ScaleX;
	this->Data[MonsterID].ScaleY	= ScaleY;
	this->Data[MonsterID].Attribute	= 0;
}

void CInterface::BindObjects(short ID, DWORD ModelID, float Width, float Height, float X, float Y)
{
	this->Data[ID].EventTick	= 0;
	this->Data[ID].OnClick		= false;
	this->Data[ID].OnShow		= false;
	this->Data[ID].ModelID		= ModelID;
	this->Data[ID].Width		= Width;
	this->Data[ID].Height		= Height;
	this->Data[ID].X			= X;
	this->Data[ID].Y			= Y;
	this->Data[ID].MaxX			= X + Width;
	this->Data[ID].MaxY			= Y + Height;
	this->Data[ID].Attribute	= 0;
}
//bool CInterface::CheckWindowEx(int WindowID)
//{
//	if( WindowID < 0  || WindowID > MAX_WINDOW_EX)
//	{
//		return 0;
//	}
//	return this->WindowEx[WindowID];
//}
float CInterface::GetResizeX(short ObjectID)
{
	if( pWinWidth == 800 )
	{
		return this->Data[ObjectID].X + 16.0;
	}
	else if( pWinWidth != 1024 )
	{
		return this->Data[ObjectID].X - 16.0;
	}
	// ----
	return this->Data[ObjectID].X;
}
bool CInterface::IsWorkZone(short ObjectID)
{
	float PosX = this->Data[ObjectID].X;
	float MaxX = PosX + this->Data[ObjectID].Width;
	// ----
	if( ObjectID == eSAMPLEBUTTON 
		|| ObjectID == eCAMERA_MAIN 
		|| ObjectID == eCAMERA_BUTTON1
		|| ObjectID == eCAMERA_BUTTON2 )
	{
		PosX = this->GetResizeX(ObjectID);
		MaxX = PosX + this->Data[ObjectID].Width;
	}
	// ----
	if( (pCursorX < PosX || pCursorX > MaxX) 
		|| (pCursorY < this->Data[ObjectID].Y || pCursorY > this->Data[ObjectID].MaxY) )
	{
		return false;
	}
	// ----
	return true;
}

void CInterface::DrawGUIex(short ObjectID)
{
	//pDrawGUI(this->Data[ObjectID].ModelID, this->Data[ObjectID].X, this->Data[ObjectID].Y, 
	//	this->Data[ObjectID].Width, this->Data[ObjectID].Height,0.0,0.0,this->Data[ObjectID].ScaleX, this->Data[ObjectID].ScaleY, 1, 1, 0.0);

	PrintImages(this->Data[ObjectID].ModelID, this->Data[ObjectID].X, this->Data[ObjectID].Y, this->Data[ObjectID].Width,this->Data[ObjectID].Height,0.0,0.0,this->Data[ObjectID].ScaleX, this->Data[ObjectID].ScaleY, 1, 1, 0.0);//tyt

}
//void Interface::DrawGUI(short ObjectID, float PosX, float PosY)

void CInterface::DrawGUI(DWORD ModelID,short ObjectID, float PosX, float PosY)
{
	if( this->Data[ObjectID].X == -1 || this->Data[ObjectID].Y == -1 )
	{
		this->Data[ObjectID].X		= PosX;
		this->Data[ObjectID].Y		= PosY;
		this->Data[ObjectID].MaxX	= PosX + this->Data[ObjectID].Width;
		this->Data[ObjectID].MaxY	= PosY + this->Data[ObjectID].Height;
	}
	// ----
	PrintImages(ModelID,PosX, PosY,this->Data[ObjectID].Width,this->Data[ObjectID].Height,0.0,0.0,this->Data[ObjectID].ScaleX, this->Data[ObjectID].ScaleY, 1, 1, 0.0);//tyt

}

void CInterface::DrawGUIs(short ObjectID, float PosX, float PosY)
{
	if( this->Data[ObjectID].X == -1 || this->Data[ObjectID].Y == -1 )
	{
		this->Data[ObjectID].X		= PosX;
		this->Data[ObjectID].Y		= PosY;
		this->Data[ObjectID].MaxX	= PosX + this->Data[ObjectID].Width;
		this->Data[ObjectID].MaxY	= PosY + this->Data[ObjectID].Height;
	}
	// ----
	PrintImages(this->Data[ObjectID].ModelID, PosX, PosY, 
		this->Data[ObjectID].Width, this->Data[ObjectID].Height,0.0,0.0,this->Data[ObjectID].ScaleX, this->Data[ObjectID].ScaleY, 1, 1, 0.0);//tyt
}

bool CInterface::IsWorkZoneObj(short ObjectID)
{
	float PosX = this->Data[ObjectID].X;
	float MaxX = PosX + this->Data[ObjectID].Width;
	// ----
	//if( ObjectID == eSAMPLEBUTTON 
	//	|| ObjectID == eCAMERA_MAIN 
	//	|| ObjectID == eCAMERA_BUTTON1
	//	|| ObjectID == eCAMERA_BUTTON2 )
	//{
	//	PosX = this->GetResizeX(ObjectID);
	//	MaxX = PosX + this->Data[ObjectID].Width;
	//}
	// ----
	if( (pCursorX < PosX || pCursorX > MaxX) 
		|| (pCursorY < this->Data[ObjectID].Y || pCursorY > this->Data[ObjectID].MaxY) )
	{
		return false;
	}
	// ----
	return true;
}
int CInterface::DrawFormat(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen	= sizeof(Buff)-1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len	= vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);
	// ----
	int LineCount = 0;
	// ----
	char * Line = strtok(Buff, "\n");
	// ----
	while( Line != NULL )
	{
		//pDrawColorText(Line, PosX, PosY, Width, 0, Color, 0, Align);
		PosY += 10;
		Line = strtok(NULL, "\n");
	}
	// ----
	return PosY;
}
struct PMSG_GUILD_BANK1
{
	PBMSG_HEAD2 h;
	BYTE Result;
};
#define pWindowThis				((LPVOID(*)()) 0x860FC0)
#define pOpenWindow				((int(__thiscall*)(LPVOID This, int Code)) 0x0085EB00)//85ead0 check window
#define pCloseWindow			((int(__thiscall*)(LPVOID This, int Code)) 0x0085F850)
int CInterface::CloseWindow(int WindowID)
{
	return pCloseWindow(pWindowThis(), WindowID);
}
void CInterface::MouseClickProc()
{
	//if(pCheckWindow(5)) //inventory
	//{
		//if (CheckZone(584, 395, 23.0, 22.0))
		//{
		//	/*if(pCursorX >= 584
		//	&&	(float)pCursorX <= 584 + 26
		//	&&	pCursorY >= 395 - 1 
		//	&&	pCursorY < 395 + 26 )*/
		//	//this->CloseWindow(ObjWindow::Guild);

		//	PMSG_GUILD_BANK1 pMsg;
		//	pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0xD8, sizeof(pMsg));
		//	pMsg.Result = 1;
		//	DataSend((LPBYTE)&pMsg, pMsg.h.size);
		//	int test = MAIN_WIDTH / 2.8;
		//		pDrawToolTipEx(test,30, (char*)"   [ - PRESS F8 TO DISABLE BOT HELPER - ]   ");
		//}
	//}
	// AttackHelper mode selection removed — LordOfMU's ImGui overlay provides
	// the attack mode selection UI via CImGuiOverlay::RenderSettingsWindow().
	// The old AttackHelper SelectMode click handling is no longer needed.
	if (false /* was: gAttackHelper->State == ATTACKHELPER_STATE_SelectMode */)
	{
		float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
		float StartY = 50.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			// Was: gAttackHelper->AttackType = 1; gAttackHelper->SetState(ATTACKHELPER_STATE_Running);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			// Was: gAttackHelper->AttackType = 2; gAttackHelper->SetState(ATTACKHELPER_STATE_Running);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			// Was: gAttackHelper->AttackType = 3; gAttackHelper->SetState(ATTACKHELPER_STATE_Running);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			// Was: gAttackHelper->SetState(ATTACKHELPER_STATE_Stop);
		}

		return;
	}
	if (gInterfaces->StateWare == 1)
	{
		float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
		float StartY = 230.0f;

		if(CheckZone(StartX +120, StartY - 30, 16.0, 16.0))// (gInterfaces->IsWorkZone(eBankClose))
		{
			//if(Event == WM_LBUTTONDOWN && Delays > 100)
			//{
			//	Sleep(100);
				//this->ClickTime = GetTickCount();
				gInterfaces->StateWare = 0;
			//}
		}

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWare = 0;
			CGSendValueMenu(1, 0);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWare = 0;
			CGSendValueMenu(1, 1);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWare = 0;
			CGSendValueMenu(1, 2);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWare = 0;
			CGSendValueMenu(1, 3);
		}
		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWare = 0;
			CGSendValueMenu(1, 4);
		}
		//DWORD Delays = (GetTickCount() - this->ClickTime);
//
		
		return;
	}
		if (gInterfaces->StateWarex == 1)
	{
		float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
		float StartY = 230.0f;

		if(CheckZone(StartX +120, StartY - 30, 16.0, 16.0))// (gInterfaces->IsWorkZone(eBankClose))
		{
			//if(Event == WM_LBUTTONDOWN && Delays > 100)
			//{
			//	Sleep(100);
				//this->ClickTime = GetTickCount();
				gInterfaces->StateWarex = 0;
			//}
		}

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWarex = 0;
			CGSendValueMenu(2, 0);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWarex = 0;
			CGSendValueMenu(2, 1);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWarex = 0;
			CGSendValueMenu(2, 2);
		}

		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWarex = 0;
			CGSendValueMenu(2, 3);
		}
		StartY += 30.0f;

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			gInterfaces->StateWarex = 0;
			CGSendValueMenu(2, 4);
		}
		//DWORD Delays = (GetTickCount() - this->ClickTime);
//
		
		return;
	}
	if (gMenuSystem.Active == 1)
	{
		float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
		float StartY = 50.0f;
		//==============================================================================
		if (CheckZone(StartX-80, StartY+50, 120.0, 22.0))
		{
			if (g_ResetSystem.State == 0)//gRanking.Active)
			{
				g_ResetSystem.State = 1;

			}
			else
			{
				g_ResetSystem.State = 0;
			}
			gMenuSystem.Active=false;
		}
		if (CheckZone(StartX+80, StartY+50, 120.0, 22.0))
		{
			if (gRanking.Active == 0)
			{
				gRanking.Active = 1;

			}
			else
			{
				gRanking.Active = 0;
			}
			gMenuSystem.Active=false;
		}
		if (CheckZone(StartX-80, StartY+32+50, 120.0, 22.0))
		{
			if(g_ResetSystem.StateGR == 0)//gRanking.Active)
					{
						g_ResetSystem.StateGR = 1;
					}
					else
					{
					
						g_ResetSystem.StateGR = 0;
					}
			gMenuSystem.Active=false;
		}
		if (CheckZone(StartX+80, StartY+32+50, 120.0, 22.0))
		{
				gCamera->Switch();
				gMenuSystem.Active=false;
		}
		if (CheckZone(StartX-80, StartY+64+50, 120.0, 22.0))
		{
			if(gJewelsBank.Active)
					{
						gJewelsBank.Active = false;
					}
					else
					{
						gJewelsBank.Active = true;
					}
			gMenuSystem.Active=false;
		}
		if (CheckZone(StartX+80, StartY+64+50, 120.0, 22.0))
		{
				gCamera->Reset();
				gMenuSystem.Active=false;
		}
		if (CheckZone(StartX-80, StartY+96+50, 120.0, 22.0))
		{
			if (gInterfaces->Data[eRageTable].OnShow==0)
			{
				gInterfaces->Data[eRageTable].OnShow = 1;

			}
			else
			{
				gInterfaces->Data[eRageTable].OnShow = 0;
			}
			gMenuSystem.Active=false;
		}
		if (CheckZone(StartX+80, StartY+96+50, 120.0, 22.0))
		{
			// REPLACED: Menu button now toggles LordOfMU AutoClicker instead
			// of the old AttackHelper. Send F5 to toggle via LordOfMU.
			LordOfMU_ToggleClicker(*(HWND*)(MAIN_WINDOW));
			gMenuSystem.Active=false;
		}

		
	}
	//==============================================================================
	if(g_ResetSystem.State == 1)
	{
		if (CheckZone(282, 330, 120.0, 22.0))
		{
			GC_RESET_RESULT pMSg;
				pMSg.h.set((LPBYTE)&pMSg, 0xFB, 0x43, sizeof(pMSg));
				DataSend((LPBYTE)&pMSg, pMSg.h.size);
			g_ResetSystem.State = 0;
			//pSendChatTextEx((const char *)"", (const char *)"Attack Helper - Start", Blue);
		}
	}

	if (g_ResetSystem.StateGR == 1)
	{
		if (CheckZone(282, 330, 120.0, 22.0))
		{
			GC_RESET_RESULT pMSg;
			pMSg.h.set((LPBYTE)&pMSg, 0xFB, 0xA2, sizeof(pMSg));
			DataSend((LPBYTE)&pMSg, pMSg.h.size);
			g_ResetSystem.StateGR = 0;
			//pSendChatTextEx((const char *)"", (const char *)"Attack Helper - Start", Blue);
		}
	}
	if (g_DungeonSiege.Active == 1)
	{
		if (CheckZone(282, 275, 120.0, 22.0))
		{
			PMSG_PROTOCOL_RESULT pMsg;
			pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0x44,  sizeof(pMsg));
			pMsg.Result = true;
			//gProtocol.DataSend((LPBYTE)&pMsg, sizeof(pMsg));
			DataSend((LPBYTE)&pMsg, pMsg.h.size);
		}

	}
	
}
//#define pCheckWindow			((bool(__stdcall*)(int Code))0x006F5BD6)
#ifdef MULTI_WAREHOUSE
void CInterface::DrawBankCountWindow()
{
	
	/*if(pCheckWindow(0x1A)
			|| pCheckWindow(7))
			{
				return;
			}*/

	

	if( !this->Data[eBankCount].OnShow == true )
	{
		return;
	}
	pBlendSwitch(1);
	if (this->StateBank == 1)
	{
		if (pCheckWindow(0x1B)) //inventory
		{
			// keybd_event('V', 0,0,0);
			 //keybd_event('V', 0,KEYEVENTF_KEYUP,0);
			//system("PAUSE");
			DrawTextOut(0, 105, "Close, Inventory first (V)",0,0,0);
			return;
			//return;
		}
	int BankCount = this->TotalBanks;
	float StartBankCountX = 248.0;
	float StartBankCountY = 179.5625;
	float Radius_1 = 20.0;
	float CordX;
	float CordY;
	float LastCordX;
	float LastCordY;
	//PrintImages(0xDDF,231.4375, 76.5,400,300,0.0,0.0,0.998125,0.768125, 1, 1, 0.0);//map
	this->DrawGUI(0xDDF,eBankCount, 231.4375, 76.5);
	
	DrawTextOut(294, 85, "Vault System",0,0,0);

	PrintImages(0x988, 302.0, 105.6875, 40.25, 50.5, 0, 0, 0.5234375, 0.9765625, 1, 1, 0.0);

	PrintImages(0x989, 294.0, 105.6875, 40.25, 50.5, 0, 0, 0.5234375, 0.9765625, 1, 1, 0.0);

	char szTemp[15];
	if (this->TotalBanks >= 12)
	{
		sprintf(szTemp, "-");

	}
	else
	{

		sprintf(szTemp, "%d", this->Zen);
	}
	
	glColor3f(0.9f, 0.f, 0.f);
	DrawTextOut(340.4375,267, szTemp, 0, 0, 0);
	GLSwitch();
	glColor3f(1.f, 1.f, 1.f);
	//((void(*)())(0x602B50))();


	for (int i = 0; i < BankCount; i++)
	{
		
		if (i == 0)
		{
			CordX = StartBankCountX;
			CordY = StartBankCountY;
		}
		else if (i == 3)
		{
			CordX = StartBankCountX ;
			CordY = StartBankCountY + Radius_1;

		}
		else if (i == 6)
		{
			CordX = StartBankCountX ;
			CordY = StartBankCountY + Radius_1*2;

		}
		else if (i == 9)
		{
			CordX = StartBankCountX;
			CordY = StartBankCountY + Radius_1 *3;

		}
		else
		{
		
			CordX = LastCordX+ Data[eBankNum1+i].Width + 8.0;
			CordY = LastCordY;
		}
	 
	
		if (this->IsWorkZone(eBankNum1 + i))
		{
			this->DrawGUI(3555 + i * 2, eBankNum1 + i, CordX, CordY);
		}
		else
		{
			this->DrawGUI(3554 + i * 2, eBankNum1 + i, CordX, CordY);
		}
		LastCordX = CordX;
		LastCordY = CordY;
		
	}
	//PrintImages(0x120, 283.125, 263.125, 16.0, 16.0, 0.0, 0.0, 0.75, 0.75, 0, 0, 0.0);
	// ----
	if (this->Data[eBankClose].OnClick)
	{
		this->DrawGUI(0x119, eBankClose, 377.875, 295.125);
	}
	else
	{
		this->DrawGUI(0x118, eBankClose, 377.875, 295.125);
	}

	if (this->TotalBanks < 12)
	{
	//	this->Data[eBankAdd].OnShow = true;
		if (this->Data[eBankAdd].OnClick)
		{
			this->DrawGUI(0x121, eBankAdd, 283.125, 263.125);
		}
		else
		{
			this->DrawGUI(0x120, eBankAdd, 283.125, 263.125);
		}
	}
	else
	{
		//this->Data[eBankAdd].OnShow = false;

	}
	//if( this->IsWorkZone(eBankCount) )
	//{
	//	DWORD Color = eGray100;
	//}
	}

}

void CInterface::WarehouseButtonAddUI(DWORD Event)
{
	DWORD CurrentTick = GetTickCount();
	// ----
	//if( this->CheckWindow(CashShop)
	//	||	this->CheckWindow(MoveList) 
	//	||  (this->CheckWindow(Inventory) 
	//	&& this->CheckWindow(ExpandInventory) 
	//	&& this->CheckWindow(Store)) 
	//	||  (this->CheckWindow(Inventory) 
	//	&& this->CheckWindow(Warehouse) 
	//	&& this->CheckWindow(ExpandWarehouse)) )
	//{
	//	return;
	//}
	// ----

	//if (sub_6F5DB6(27)
	//	&& (sub_6F5DB6(26)
	//		|| sub_6F5DB6(6)
	//		|| sub_6F5DB6(7)
	//		|| sub_6F5DB6(9)
	//		|| sub_6F5DB6(13)
	//		|| sub_6F5DB6(15)
	//		|| sub_6F5DB6(17)
	//		|| sub_6F5DB6(12)
	//		|| sub_6F5DB6(18)
	//		|| sub_6F5DB6(16)))
	//{
	//	sub_47D0A4(dword_5877FC0, 0, 420);
	//}
	if (!this->Data[eBankCount].OnShow == true)
	{
		return;
	}
	if (this->StateBank == 1)
	{
	if (IsWorkZone(eBankAdd))
	{
		if (this->TotalBanks < 12)
		{
			//if (this->Data[eBankAdd].OnShow == true)
			//{
			DWORD Delay = (CurrentTick - this->Data[eBankAdd].EventTick);
			//DWORD Delay2 = (CurrentTick - this->Data[eBankAdd].EventTick);
			// ----
			if (Event == WM_LBUTTONDOWN)
			{
				this->Data[eBankAdd].OnClick = true;
				//	return;
			}
			// ----
			if (this->Data[eBankAdd].OnClick != true)
			{
				return;
			}
			if (Delay < 100)
			{
				return;
			}
			this->Data[eBankAdd].OnClick = false;
			// ----
			if (Delay < 400)
			{
				return;
			}

			// ----
			this->Data[eBankAdd].EventTick = GetTickCount();
			// ----
			CreateBank(this->TotalBanks);
			this->TotalBanks++;
		}
	}
	else if (IsWorkZone(eBankClose))
	{
			DWORD Delay = (CurrentTick - this->Data[eBankClose].EventTick);
			// ----
			if (Event == WM_LBUTTONDOWN)
			{
				this->Data[eBankClose].OnClick = true;
			}
			// ----
			if (this->Data[eBankClose].OnClick != true)
			{
				return;
			}

			if (Delay < 300)
			{
				return;
			}
			this->Data[eBankClose].OnClick = false;
			// ----
			if (Delay < 500)
			{
				return;
			}
			// ----
			this->Data[eBankClose].EventTick = GetTickCount();
			// ----
			this->Close_DrawBankCountWindow();
	}
	else
	{
		for (int i = 0; i < this->TotalBanks; i++)
		{
			if (IsWorkZone(eBankNum1 + i))
			{
					DWORD Delay = (CurrentTick - this->Data[eBankNum1 + i].EventTick);
					// ----
					if (Event == WM_LBUTTONDOWN)
					{
						this->Data[eBankNum1 + i].OnClick = true;
					}
					// ----
					if (this->Data[eBankNum1 + i].OnClick != true)
					{
						return;
					}
					// ----
					if (Delay < 300)
					{
						return;
					}
					this->Data[eBankNum1 + i].OnClick = false;
					// ----
					if (Delay < 500)
					{
						return;
					}
					// ----
					this->Data[eBankNum1 + i].EventTick = GetTickCount();
					
					BankNumberOpen(i + 1);
					this->Close_DrawBankCountWindow();
			}
		}
	
	
	
	}


	//	eBankClose

	}
}
// ----------------------------------------------------------------------------------------------
#endif

#if(DEV_DAMAGE_TABLE)

void CInterface::DrawDamageTable()
{
	if (this->StateD == 1)
	{
	if (pCheckWindow(1)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(2)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(3)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(4)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(5)) //WAREHOUSE
	{
		return;
	}
				if (pCheckWindow(6)) //WAREHOUSE
	{
		return;
	}
	if (pCheckWindow(7)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(13)) //WAREHOUSE
	{
		return;
	}
			if (pCheckWindow(12)) //WAREHOUSE
	{
		return;
	}
				if (pCheckWindow(11)) //WAREHOUSE
	{
		return;
	}
					if (pCheckWindow(10)) //WAREHOUSE
	{
		return;
	}
						if (pCheckWindow(9)) //WAREHOUSE
	{
		return;
	}
							if (pCheckWindow(8)) //WAREHOUSE
	{
		return;
	}

if (pCheckWindow(14)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(15)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(16)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(17)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(18)) //WAREHOUSE
	{
		return;
	}
	if (pCheckWindow(19)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(20)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(21)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(22)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(23)) //WAREHOUSE
	{
		return;
	}
if (pCheckWindow(24)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(25)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(26)) //character
	{
		return;
	}
		if (pCheckWindow(27)) //inventory
	{
		return;
	}
		if (pCheckWindow(28)) //WAREHOUSE
	{
		return;
	}
	if (pCheckWindow(29)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(30)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(31)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(32)) //WAREHOUSE
	{
		return;
	}

	if (pCheckWindow(33)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(34)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(35)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(36)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(37)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(38)) //WAREHOUSE
	{
		return;
	}
	if (pCheckWindow(39)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(40)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(41)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(42)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(43)) //WAREHOUSE
	{
		return;
	}





	if (pCheckWindow(44)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(45)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(46)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(47)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(48)) //WAREHOUSE
	{
		return;
	}
	if (pCheckWindow(49)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(50)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(51)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(52)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(53)) //WAREHOUSE
	{
		return;
	}

			if (pCheckWindow(54)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(55)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(56)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(57)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(58)) //WAREHOUSE
	{
		return;
	}
	if (pCheckWindow(59)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(60)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(61)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(62)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(63)) //WAREHOUSE
	{
		return;
	}
				if (pCheckWindow(64)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(65)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(66)) //WAREHOUSE
	{
		return;
	}

				if (pCheckWindow(67)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(68)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(69)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(70)) //WAREHOUSE
	{
		return;
	}
				if (pCheckWindow(71)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(72)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(73)) //WAREHOUSE
	{
		return;
	}

						if (pCheckWindow(74)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(75)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(76)) //WAREHOUSE
	{
		return;
	}

				if (pCheckWindow(77)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(78)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(79)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(80)) //WAREHOUSE
	{
		return;
	}
				if (pCheckWindow(81)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(82)) //WAREHOUSE
	{
		return;
	}
		if (pCheckWindow(83)) //WAREHOUSE
	{
		return;
	}
	//if(!gDamageTable)
	//{
	//	return;
	//}

	//if(!gObjUser.m_SecondInfo)
	//{
	//	return;
	//}

	//if(this->MiniMapCheck() || this->CombinedChecks())
	//{
	//	return;
	//}

	//if(g_ExMenuV3.m_Enable && g_ExMenuV3.m_Version == EXMENU_NUMBER::eVERSION_ET)
	//{
	//	if(gInterface.CheckWindowEx(exWinMenuV3))
	//	{
	//		return;
	//	}
	//}



			
	float Wind = 100;
	float Hig = 10 + 40;
	
	gInterfaces->RightMiniInfoY -= Hig;

	float PosX = 535;
	float PosY = gInterfaces->RightMiniInfoY - 5;

	//this->DrawBarForm(PosX, PosY, 100, 50, 0, 0, 0, 255);
	//pDrawColorButton(0x7880, PosX, PosY, Wind, Hig, NULL, NULL, Color4f(0, 0, 0, 100));
	SetBlend(1);
	glColor4f(0.f, 0.f, 0.f, 0.5f);
	DrawBarForm((float)(PosX + 10), (float)(PosY - 2), 20.f + (float)(Wind), 60.f);
	GLSwitchBlend();

	glColor4f(1.f, 1.f, 1.f, 1.f);


	DrawTextOut(PosX + 20, PosY, "Damage Table Info", 0, 0, 0);

	///*pSetBlend(true);
	//pDrawTextColor = eDrawText::TextColor::Orange;
	//pDrawTextBackground = eDrawText::TextBackground::Invisible;
	//MU_DrawTextOut(PosX, PosY, "Damage Table Info", 0, 0, 0);*/



	
	//MU_DrawTextOut(PosX + 20, PosY + 10, "Damage:", 0, 0, 0);
	//MU_DrawTextOut(PosX + 20, PosY + 20, "Defence:", 0, 0, 0);
	//MU_DrawTextOut(PosX + 20, PosY + 30, "Reflect:", 0, 0, 0);
	//MU_DrawTextOut(PosX + 20, PosY + 40, "DamageMax:", 0, 0, 0);
	DrawTextOut(PosX + 20, PosY + 10, "Damage:", 0, 0, 0);
	DrawTextOut(PosX + 20, PosY + 20, "Defence:", 0, 0, 0);
	DrawTextOut(PosX + 20, PosY + 30, "Reflect:", 0, 0, 0);
	DrawTextOut(PosX + 20, PosY + 40, "DamageMax:", 0, 0, 0);
	//gInterfaces->DrawFormat(eWhite, PosX + 20, PosY + 10, 100, 1, "Damage:");
	//gInterfaces->DrawFormat(eWhite, PosX + 20, PosY + 20, 100, 1, "Defence:");
	//gInterfaces->DrawFormat(eWhite, PosX + 20, PosY + 30, 100, 1, "Reflect:");
	//gInterfaces->DrawFormat(eWhite, PosX + 20, PosY + 40, 100, 1, "DamageMax:");
	//pSetBlend(true);
	//pDrawTextColor = eDrawText::TextColor::Red;
	//pDrawTextBackground = eDrawText::TextBackground::Invisible;
	char szTemp1[256];
	sprintf(szTemp1 ,"%d", gObjUser.m_SecondDamage);
	char szTemp2[256];
	sprintf(szTemp2 ,"%d", gObjUser.m_SecondDefence);
	char szTemp3[256];
	sprintf(szTemp3 ,"%d", gObjUser.m_SecondReflect);
	char szTemp4[256];
	sprintf(szTemp4 ,"%d", gObjUser.m_SecondDamageMax);
	glColor3f(0.9f, 0.f, 0.f);
	DrawTextOut(PosX + 70, PosY + 10, (char*)szTemp1, 0, 0, 0);
	DrawTextOut(PosX + 70, PosY + 20, (char*)szTemp2, 0, 0, 0);
	DrawTextOut(PosX + 70, PosY + 30, (char*)szTemp3, 0, 0, 0);
	DrawTextOut(PosX + 70, PosY + 40, (char*)szTemp4, 0, 0, 0);
	
	//pGLSwitch();
	//gInterfaces->DrawFormat(eYellow, PosX + 70, PosY + 10, 100, 1, "%d", gObjUser.m_SecondDamage);
	//gInterfaces->DrawFormat(eYellow, PosX + 70, PosY + 20, 100, 1, "%d", gObjUser.m_SecondDefence);
	//gInterfaces->DrawFormat(eYellow, PosX + 70, PosY + 30, 100, 1, "%d", gObjUser.m_SecondReflect);
	//gInterfaces->DrawFormat(eYellow, PosX + 70, PosY + 40, 100, 1, "%d", gObjUser.m_SecondDamageMax);

	
	//float iX = 535;
	//float iY = 320;
	//float Wind = 100;
	//float Hig = qCount + mCount;

	//float StartiY = 425;
	//// ----
	//StartiY = gInterface.RightMiniInfoY - 5;
	//gInterface.RightMiniInfoY -= Hig;
	//// ----
	//iY = StartiY - Hig;

	//pDrawColorButton(0x7880, iX, iY, Wind, Hig, NULL, NULL, Color4f(0, 0, 0, 100));



	GLSwitch();
	glColor3f(1.f, 1.f, 1.f);

	//((void(*)())(0x602B50))();
	}
}

#endif

//int CInterface::DrawFormat(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...)
//{
////#if(ENABLE_CHANGE)
////	Text = g_Text.Change(Text);
////#endif
////
////	char Buff[2048];
////	int BuffLen	= sizeof(Buff)-1;
////	ZeroMemory(Buff, BuffLen);
////	// ----
////	va_list args;
////	va_start(args, Text);
////	int Len	= vsprintf_s(Buff, BuffLen, Text, args);
////	va_end(args);
////	// ----
////	int LineCount = 0;
////	// ----
////	char * Line = strtok(Buff, "\n");
////	// ----
////	while( Line != NULL )
////	{
////		pDrawColorText(Line, PosX, PosY, Width, 0, Color, 0, Align);
////		PosY += 10;
////		Line = strtok(NULL, "\n");
////	}
////	// ----
////	return PosY;
//}

bool CheckZone(float PosX, float PosY, float Width, float Height)
{
	float MaxPosX = PosX + Width;
	float MaxPosY = PosY + Height;

	if (MAIN_CURSORX >= PosX && MAIN_CURSORX <= MaxPosX)
	{
		if (MAIN_CURSORY >= PosY && MAIN_CURSORY <= MaxPosY)
		{
			return true;
		}
	}

	return false;
}
	int lastReport;
	int frameCount;
	int frameRate;
	char FPS_REAL[30];

void UPDATE_FPS()
{


	//frameCount++;
	//if (GetTickCount() - lastReport >= 1000)
	//{
	//	frameRate = frameCount / ((GetTickCount() - lastReport) / 1000);
	//	sprintf(FPS_REAL, "FPS: %d", frameRate);
	//	lastReport = GetTickCount();
	//	frameCount = 0;
	//}

	//pDrawToolTipEx(600, 3, (char*)FPS_REAL);

	//	if (World == 30 || World == 34 || World >= 47 && World <= 49)
	//{
	//	return;
	//}

	if (pCheckWindow(27) && pCheckWindow(26) || pCheckWindow(27) && pCheckWindow(7) || pCheckWindow(27) && pCheckWindow(6) || pCheckWindow(27) && pCheckWindow(9) || pCheckWindow(27) && pCheckWindow(12) || pCheckWindow(27) && pCheckWindow(21))
	{
		return;
	}

	float pos_x = 0.0;
	float pos_y = 0.0;

	if (MAIN_RESOLUTION_WIDTH > 800)
	{
		pos_y = 0.5;
	}

	char INFO[50] = { 0 };
	// ---
	EnableAlphaTest(true);

	glColor3f(1.0, 1.0, 1.0);

	RenderBitmap(334, pos_x, pos_y, 66.0f, 28.0f, 0.0, 0.0, 0.52, 0.78125, 1, 1, 1.0);

	glColor3f(1.0, 1.0, 1.0);

	DisableAlphaBlend();


	char Fps[60];
	sprintf_s(Fps, "FPS: %.01f", *(float*)0x587C620);//5754B58
	EnableAlphaTest(true);

	EnableAlphaBlend();

	glColor3f(1.0, 1.0, 1.0);
	// ---
	//pSetBackgroundTextColor(pTextThis2(), 255, 255, 255, 0);
	//pSetTextColor(pTextThis(), 255, 255, 255, 255);

	if (MAIN_RESOLUTION_WIDTH > 800)
	{
		//pDrawTextOut(pTextThis(), pos_x - 63.0f, pos_y + 5.0f, Fps, 190, 0, (LPINT)3, 0);
		DrawTextOut((int)pos_x + 13.5f, pos_y + 5.0f, Fps, 0, 0, 0);
	}
	else
	{
		//pDrawTextOut(pTextThis(), pos_x - 63.0f, pos_y + 5.5f, Fps, 190, 0, (LPINT)3, 0);
		DrawTextOut((int)pos_x + 14.0f, pos_y + 5.5f, Fps, 0, 0, 0);
	}

	EnableAlphaBlend();
	// ---
	DisableAlphaBlend();

	glColor3f(1.0, 1.0, 1.0);
}

#if(CUSTOM_VALUE_MENU)

void CInterface::BindValueMenu()
{
	//gInterfaces->BindObject(eVALUEMENU_MAIN, 0x7A5A, 222, 210, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_TITLE, 0x7A63, 230, 67, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_FRAME, 0x7A58, 230, 15, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_FOOTER, 0x7A59, 230, 50, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_OK, 0x7A5F, 62, 27, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_CLOSE, 0x7A5F, 62, 27, -1, -1);

	//gInterfaces->BindObject(eVALUEMENU_VALUE1, 0x7A5E, 108, 29, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_VALUE2, 0x7A5E, 108, 29, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_VALUE3, 0x7A5E, 108, 29, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_VALUE4, 0x7A5E, 108, 29, -1, -1);
	//gInterfaces->BindObject(eVALUEMENU_VALUE5, 0x7A5E, 108, 29, -1, -1);
}

void CInterface::DrawValueMenu()
{

	//if(!g_ExLicense.m_MultiWarehouseType)
	//{
	//	return;
	//}

	//if(!gInterface.CheckWindowEx(ObjWindowsEx::exWinValueMenu))
	//{
	//	return;
	//}

	//pSetCursorFocus = true;
	if(this->StateWare	== 1) 
	{

	if (pCheckWindow(0x1B)) //inventory
	{
		// keybd_event('V', 0,0,0);
			//keybd_event('V', 0,KEYEVENTF_KEYUP,0);
		//system("PAUSE");
		DrawTextOut(0, 105, "Close, Inventory first (V)",0,0,0);
		return;
		//return;
	}
		float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
		float StartY = 230.0f;
		
		SetBlend(1);
	glColor4f(0.f, 0.f, 0.f, 0.9f);
	DrawBarForm((float)(StartX - 20), (float)(StartY - 30), 157.f + (float)(0), 180.f);
	GLSwitchBlend();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glColor3f(0.9f, 0.f, 0.f);
	DrawTextOut((int)StartX+25, (int)StartY-25, (char*)" Warehouse Menu", 0, 0, 0);
	GLSwitch();
	glColor3f(1.f, 1.f, 1.f);
		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
		gInterfaces->DrawGUI(0x119, eBankClose, StartX +120, StartY - 30);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}


		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}


		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}
		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}
		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}
		pBlendSwitch(1);

		StartY = 237.0f;
		pDrawTextOut((int)StartX + 35, (int)StartY, (char*)" Warehouse", 0, 0, 0);
		pDrawTextOut((int)StartX + 35, (int)StartY + 30, (char*)" Warehouse 2", 0, 0, 0);
		pDrawTextOut((int)StartX + 35, (int)StartY + 60, (char*)" Warehouse 3", 0, 0, 0);
		pDrawTextOut((int)StartX + 35, (int)StartY + 90, (char*)" Warehouse 4", 0, 0, 0);
		pDrawTextOut((int)StartX + 35, (int)StartY + 120, (char*)" Warehouse 5", 0, 0, 0);
	//gInterface.DrawWindow(eVALUEMENU_MAIN, eVALUEMENU_TITLE, eVALUEMENU_FRAME, eVALUEMENU_FOOTER, 7, StartX, StartY, "Warehouse Menu");

	//this->DrawButtonBig(eVALUEMENU_VALUE1, true, ContentX, ContentY, "Warehouse 1");
	//ContentY += plusY;

	//this->DrawButtonBig(eVALUEMENU_VALUE2, true, ContentX, ContentY, "Warehouse 2");
	//ContentY += plusY;

	//this->DrawButtonBig(eVALUEMENU_VALUE3, true, ContentX, ContentY, "Warehouse 3");
	//ContentY += plusY;

	//this->DrawButtonBig(eVALUEMENU_VALUE4, true, ContentX, ContentY, "Warehouse 4");
	//ContentY += plusY;

	//this->DrawButtonBig(eVALUEMENU_VALUE5, true, ContentX, ContentY, "Warehouse 5");
	//ContentY += plusY;
	}
}

void CInterface::ControllerValueMenu(DWORD Event)
{
	//if(!g_ExLicense.m_MultiWarehouseType)
	//{
	//	return;
	//}

	//if(!gInterface.CheckWindowEx(ObjWindowsEx::exWinValueMenu))
	//{
	//	return;
	//}

	//if(this->ButtonEx(Event, eVALUEMENU_VALUE1, true))
	//{
	//	gProtocol.CGSendValueMenu(1, 0);
	//}
	//else if(this->ButtonEx(Event, eVALUEMENU_VALUE2, true))
	//{
	//	gProtocol.CGSendValueMenu(1, 1);
	//}
	//else if(this->ButtonEx(Event, eVALUEMENU_VALUE3, true))
	//{
	//	gProtocol.CGSendValueMenu(1, 2);
	//}
	//else if(this->ButtonEx(Event, eVALUEMENU_VALUE4, true))
	//{
	//	gProtocol.CGSendValueMenu(1, 3);
	//}
	//else if(this->ButtonEx(Event, eVALUEMENU_VALUE5, true))
	//{
	//	gProtocol.CGSendValueMenu(1, 4);
	//}
}

// --

void CInterface::BindGuildValueMenu()
{
	//gInterface.BindObject(eVALUEMENU_MAIN, 0x7A5A, 222, 210, -1, -1);
	//gInterface.BindObject(eVALUEMENU_TITLE, 0x7A63, 230, 67, -1, -1);
	//gInterface.BindObject(eVALUEMENU_FRAME, 0x7A58, 230, 15, -1, -1);
	//gInterface.BindObject(eVALUEMENU_FOOTER, 0x7A59, 230, 50, -1, -1);
	//gInterface.BindObject(eVALUEMENU_OK, 0x7A5F, 62, 27, -1, -1);
	//gInterface.BindObject(eVALUEMENU_CLOSE, 0x7A5F, 62, 27, -1, -1);

	//gInterface.BindObject(eVALUEMENU_VALUE1, 0x7A5E, 108, 29, -1, -1);
	//gInterface.BindObject(eVALUEMENU_VALUE2, 0x7A5E, 108, 29, -1, -1);
	//gInterface.BindObject(eVALUEMENU_VALUE3, 0x7A5E, 108, 29, -1, -1);
	//gInterface.BindObject(eVALUEMENU_VALUE4, 0x7A5E, 108, 29, -1, -1);
	//gInterface.BindObject(eVALUEMENU_VALUE5, 0x7A5E, 108, 29, -1, -1);
}


void DrawItemIMG(float PosX, float PosY, float Width, float Height, int ItemID, int Level, int Excl, int Anc, bool OnMouse)
{
	glPopMatrix();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	pglViewport2(0, 0, *(DWORD*)MAIN_RESOLUTION_X, *(DWORD*)MAIN_RESOLUTION_Y);
	pgluPerspective2(1.f, (float)(*(float*)MAIN_RESOLUTION_X / *(float*)MAIN_RESOLUTION_Y), 20.f, 2000.f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	pGetOpenGLMatrix(pCameraMatrix);
	pEnableDepthTest();
	pEnableDepthMask();

	pRenderItem3D(PosX, PosY, Width, Height, ItemID, Level << 3, Excl, Anc, OnMouse);

	pUpdateMousePosition();

	glPopMatrix();
	glPopMatrix();

	pBeginBitmap();

	glColor3f(1, 1, 1);

	EnableAlphaTest(true);
}
void Test123()
{
	float FIX;
	if (pCheckWindow(27))
	{
		FIX = -190;
	}
	else
	{
		FIX = 0;
	}
	if (pCheckWindow(27) && pCheckWindow(12))
	{
		float v22 = 18.0 / 32.0;
		float v23 = 30.0 / 32.0;

		char* CoinName[] = { "chaos","life","soul","bless","creation"};

		char Info[50];

		for (int i = 0; i < 5; i++)
		{
			/*if (CheckMouseIn(465 + FIX + (i * 32), 200, 30.0, 24.0) && IsRelease)
			{
				EnableAlphaTest(true);
				glColor3f(1.0, 1.0, 1.0);

				sprintf_s(Info, "/store %s", CoinName[i]);
				ChatSend(Info);

				RenderBitmap(224, 465 + FIX + (i * 32), 200, 30.0, 28.0, 0, 0, v23, v22, 1, 1, 0);
				IsRelease = false;
			}
			else
			{*/
				RenderBitmap(223, 465 + FIX + (i * 32), 200, 30.0, 28.0, 0, 0, v23, v22, 1, 1, 0);

			//}
		}

		DrawItemIMG(478 + FIX + 3,   214, 1.0, 1.0, 6159, 0, 0, 0, 1);
		DrawItemIMG(478 + FIX + 34,  224, 1.0, 1.0, 7184, 0, 0, 0, 1);
		DrawItemIMG(478 + FIX + 66,  223, 1.0, 1.0, 7182, 0, 0, 0, 1);
		DrawItemIMG(478 + FIX + 97,  223, 1.0, 1.0, 7181, 0, 0, 0, 1);
		DrawItemIMG(478 + FIX + 130, 222, 1.0, 1.0, 7190, 0, 0, 0, 1);
	}
}
void CInterface::DrawGuildValueMenu()
{
	if(this->StateWarex	== 1) 
	{

	if (pCheckWindow(0x1B)) //inventory
	{
		// keybd_event('V', 0,0,0);
			//keybd_event('V', 0,KEYEVENTF_KEYUP,0);
		//system("PAUSE");
		DrawTextOut(0, 105, "Close, Inventory first (V)",0,0,0);
		return;
		//return;
	}


		float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
		float StartY = 230.0f;
		
		SetBlend(1);
	glColor4f(0.f, 0.f, 0.f, 0.9f);
	DrawBarForm((float)(StartX - 20), (float)(StartY - 30), 157.f + (float)(0), 180.f);
	GLSwitchBlend();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glColor3f(0.9f, 0.f, 0.f);
	DrawTextOut((int)StartX+15, (int)StartY-25, (char*)" Guild Warehouse Menu", 0, 0, 0);
	GLSwitch();
	glColor3f(1.f, 1.f, 1.f);
		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
		gInterfaces->DrawGUI(0x119, eBankClose, StartX +120, StartY - 30);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}


		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}


		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}
		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}
		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}
		pBlendSwitch(1);

		StartY = 237.0f;
		pDrawTextOut((int)StartX + 27, (int)StartY, (char*)" Guild Warehouse", 0, 0, 0);
		pDrawTextOut((int)StartX + 27, (int)StartY + 30, (char*)" Guild Warehouse 2", 0, 0, 0);
		pDrawTextOut((int)StartX + 27, (int)StartY + 60, (char*)" Guild Warehouse 3", 0, 0, 0);
		pDrawTextOut((int)StartX + 27, (int)StartY + 90, (char*)" Guild Warehouse 4", 0, 0, 0);
		pDrawTextOut((int)StartX + 27, (int)StartY + 120, (char*)" Guild Warehouse 5", 0, 0, 0);
	}
}

void CInterface::ControllerGuildValueMenu(DWORD Event)
{
	//if(!g_ExLicense.m_MultiWarehouseType)
	//{
	//	return;
	//}

	//if(!gInterface.CheckWindowEx(ObjWindowsEx::exWinGuildValueMenu))
	//{
	//	return;
	//}

	//if(this->ButtonEx(Event, eVALUEMENU_VALUE1, true))
	//{
	//	gProtocol.CGSendValueMenu(2, 0);
	//}
	//else if(this->ButtonEx(Event, eVALUEMENU_VALUE2, true))
	//{
	//	gProtocol.CGSendValueMenu(2, 1);
	//}
	//else if(this->ButtonEx(Event, eVALUEMENU_VALUE3, true))
	//{
	//	gProtocol.CGSendValueMenu(2, 2);
	//}
	//else if(this->ButtonEx(Event, eVALUEMENU_VALUE4, true))
	//{
	//	gProtocol.CGSendValueMenu(2, 3);
	//}
	//else if(this->ButtonEx(Event, eVALUEMENU_VALUE5, true))
	//{
	//	gProtocol.CGSendValueMenu(2, 4);
	//}
}

#endif