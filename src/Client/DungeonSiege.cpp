#include "stdafx.h"
#include "DungeonSiege.h"
#include "Interface.h"
#include "User.h"
//#include "ExMenuV3.h"
#include "Other.h"
#include "Protocol.h"
#include "InterfaceEx.h"
#include "GrandReset.h"
#include "RageSystem.h"
#include "Ranking.h"
#include "ResetSystem.h"

#if(EVENT_DUNGEON_SIEGE)

CDungeonSiege g_DungeonSiege;

CDungeonSiege::CDungeonSiege()
{
	this->Init();
}

CDungeonSiege::~CDungeonSiege()
{
}
		 
void CDungeonSiege::Init()
{
	this->bRegistration = false;
	this->iMsgCode = 0;
	this->bCaptured = false;
	this->PlayerGuildNumber = 0;
	this->ireg_list_count = 0;
	this->iEventStatus = eEVENTDUNGEON_CLOSE;
	memset(this->szLordGuild, 0, sizeof(this->szLordGuild));
	memset(this->reg_list, 0, sizeof(this->reg_list));
	GetVolumeInformationA("c:\\", 0, 0, &this->dwHWID, 0, 0, 0, 0); 
	this->m_mysocre = 0;
	for(int i = 0; i < 10; i++)
	{
		this->m_guildscore[i] = 0;
	}
	this->m_finalscore1 = 0;
	this->m_finalscore2 = 0;
	this->m_finalguild1 = 0;
	this->m_finalguild2 = 0;
	memset(this->m_finalname1, 0, sizeof(this->m_finalname1));
	memset(this->m_finalname2, 0, sizeof(this->m_finalname2));
	memset(this->m_EventName, 0, sizeof(this->m_EventName));
	this->m_insiege = false;
}

void CDungeonSiege::Load()
{

}

void CDungeonSiege::BindImages()
{
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_MAIN, 0x7A5A, 222, 407, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_TITLE, 0x7A63, 230, 67, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_FRAME, 0x7A58, 230, 15, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_FOOTER, 0x7A59, 230, 50, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_DIV, 0x7A62, 223, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_DIV2, 0x7A62, 223, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_REG, 0x7A5F, 62, 27, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_OK, 0x7A5F, 62, 27, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_NAME, 0x7BAC, 168, 18, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_NAME2, 0x7BAC, 168, 18, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD1, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD2, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD3, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD4, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD5, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD6, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD7, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD8, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD9, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILD10, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_NAMEOWNER, 0x7BAC, 168, 18, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_DIVOWNER, 0x7A62, 223, 21, -1, -1,0.62, 0.62);
	gInterfaces->BindObject(ePT_DUNGEON_SIEGE_GUILDOWNER, 0x9C4C, 170, 21, -1, -1,0.62, 0.62);
}

void CDungeonSiege::Draw()
{
	this->SetPKLevelTarget();
	this->DrawRanking();

	//if(!gInterfaces->CheckWindowEx(ObjWindowsEx::exWinDungeonSiege))
	//{
	//	return;
	//}

	//pSetCursorFocus = true;
	if(this->Active == false) return;
	gRanking.Active = false;
	g_ResetSystem.StateGR = 0;//xz
	gRanking.Active = 0;
	gInterfaces->Data[eRageTable].OnShow = false; 
	if (pCheckWindow(0x1B)) //inventory
	{
		// keybd_event('V', 0,0,0);
			//keybd_event('V', 0,KEYEVENTF_KEYUP,0);
		//system("PAUSE");
		DrawTextOut(0, 105, "Close, Inventory first (V)",0,0,0);
		return;
		//return;
	}
	//MessageBoxA(NULL, "DS1", "ERROR", MB_SERVICE_NOTIFICATION | MB_ICONWARNING);
	pBlendSwitch(1);
	DWORD Color				= eWhite;
	float StartY			= 10.0;
	float StartX			= 205.0;
	float ContentX          = StartX;
	float ContentY			= StartY;
	float ButtonX			= StartX + 84;

	this->MainTableHigh = 300;
	this->MainTableWidth = 300;
	this->DrawStartX = 640/2 - MainTableWidth/2;
	this->DrawStartY = 50;
	this->DrawTable = false;
	pDrawImage2(3592, StartX, StartY, MainTableWidth, MainTableHigh, 0, 0, 0.94, 0.6, 1, 1, 0.0);


	gInterfaces->DrawGUI(0x119, eBankClose, 374, 13.125);

	//gInterfaces->DrawGUI(ePT_DUNGEON_SIEGE_MAIN, StartX, StartY+2);
	//gInterfaces->DrawGUI(ePT_DUNGEON_SIEGE_TITLE, StartX, StartY);
	//StartY = gInterfaces->DrawRepeatGUI(ePT_DUNGEON_SIEGE_FRAME, StartX, StartY+67.0, 20);
	//gInterfaces->DrawGUI(ePT_DUNGEON_SIEGE_FOOTER, StartX, StartY);

	ContentY += 20;
	MU_DrawTextOut(274, ContentY, this->m_EventName,0,0,0);
	//gInterfaces->DrawFormat(eGold, ContentX, ContentY, 230, 3, this->m_EventName);

	ContentY += 18;
	MU_DrawTextOut(274, ContentY+2, "Siege Owner:",0,0,0);
	//gInterfaces->DrawGUI(ePT_DUNGEON_SIEGE_NAMEOWNER, ContentX+30, ContentY);
	//gInterfaces->DrawFormat(eWhite, ContentX, ContentY+2, 230, 3, "Siege Owner");
	//gInterfaces->DrawGUI(ePT_DUNGEON_SIEGE_GUILDOWNER, (ContentX+28), (ContentY+20));
	ContentY += 10;
	if(this->bCaptured)
	{
		//gInterfaces->DrawFormat(eAncient, ContentX, (ContentY+25), 230, 3, this->szLordGuild);
		MU_DrawTextOut(274, ContentY+2, this->szLordGuild,0,0,0);
	}
	else
	{
		//gInterfaces->DrawFormat(eAncient, ContentX, (ContentY+25), 230, 3, "no castle owner");
		MU_DrawTextOut(268, ContentY+2, "No Castle Owner",0,0,0);
	}
	//gInterfaces->DrawGUI(ePT_DUNGEON_SIEGE_DIVOWNER, ContentX, (ContentY+35));

	ContentY += 28;
	//gInterfaces->DrawGUI(ePT_DUNGEON_SIEGE_NAME, ContentX+30, ContentY);
	//gInterfaces->DrawFormat(eWhite, ContentX, ContentY+2, 230, 3, "Registration guild on siege");
	MU_DrawTextOut(250, ContentY+2, "Registration guild on siege:",0,0,0);
	
	ContentY += 16;
	switch(this->iMsgCode)
	{
		case 0: 
		//	gInterfaces->DrawFormat(eExcellent, ContentX, ContentY+2, 230, 3, "You can register"); 
			MU_DrawTextOut(270, ContentY+2, "You can register",0,0,0);
			break;
		case 1: 
			//gInterfaces->DrawFormat(eRed, ContentX, ContentY+2, 230, 3, "You are not in a guild");
			MU_DrawTextOut(262, ContentY+2, "You are not in a guild",0,0,0);
			break;
		case 2: 
			//gInterfaces->DrawFormat(eRed, ContentX, ContentY+2, 230, 3, "You are not a Guild Master");
			MU_DrawTextOut(257, ContentY+2, "You are not a Guild Master",0,0,0);
			break;
		case 3: 
			//gInterfaces->DrawFormat(eRed, ContentX, ContentY+2, 230, 3, "You are the owners of the Dungeon castle");
			MU_DrawTextOut(233, ContentY+2, "You are the owners of the Dungeon Castle",0,0,0);
			break;
		case 4:
			//gInterfaces->DrawFormat(eRed, ContentX, ContentY+2, 230, 3, "The max number of guild siege on 10");
			MU_DrawTextOut(235, ContentY+2, "The max number of guild siege on 10",0,0,0);
			break;
		case 5: 
			//gInterfaces->DrawFormat(eRed, ContentX, ContentY+2, 230, 3, "To register for the siege should be min 10 player in guild"); 
			MU_DrawTextOut(220, ContentY+2, "To register for the siege should be min 10 player in guild",0,0,0);
			break;
		case 6: 
			//gInterfaces->DrawFormat(eRed, ContentX, ContentY+2, 230, 3, "Your guild is already registered"); 
			MU_DrawTextOut(239, ContentY+2, "Your guild is already registered",0,0,0);
			break;
	}
	
	ContentY += 16;
	//gInterfaces->DrawGUI(ePT_DUNGEON_SIEGE_REG, ButtonX, ContentY);
	//gInterfaces->DrawFormat(eWhite, ContentX, ContentY+10, 230, 3, "Registration");
	gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].Attribute = this->bRegistration;
	MU_DrawTextOut(274, ContentY+2, "Registration",0,0,0);
	//=========================================================================================================
	if(!this->bRegistration)
	{
		//gInterfaces->DrawGUIs(ePT_DUNGEON_SIEGE_REG, gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].X, gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].Y);
		gInterfaces->DrawGUIex(ePT_DUNGEON_SIEGE_REG);
		//gInterfaces->DrawColoredGUIObj(ePT_DUNGEON_SIEGE_REG, gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].X, gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].Y, eGray150);
	}
	else
	{
		if(gInterfaces->IsWorkZoneObj(ePT_DUNGEON_SIEGE_REG))
		{
			Color = eGray100;
			if(gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].OnClick)
			{
				Color = eGray150;
			}
			gInterfaces->DrawGUIex(ePT_DUNGEON_SIEGE_REG);
			//gInterfaces->DrawGUIs(ePT_DUNGEON_SIEGE_REG, gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].X, gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].Y);
			//gInterfaces->DrawColoredGUIObj(ePT_DUNGEON_SIEGE_REG, gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].X, gInterfaces->Data[ePT_DUNGEON_SIEGE_REG].Y, Color);
		}
	}

	ContentY += 25;
	//gInterfaces->DrawGUIs(ePT_DUNGEON_SIEGE_DIV, ContentX, ContentY);
	gInterfaces->DrawGUIex(ePT_DUNGEON_SIEGE_DIV);

	ContentY += 13;
	//gInterfaces->DrawGUIs(ePT_DUNGEON_SIEGE_NAME2, ContentX+30, ContentY);
	gInterfaces->DrawGUIex(ePT_DUNGEON_SIEGE_NAME2);
	//gInterfaces->DrawFormat(eWhite, ContentX, ContentY+2, 230, 3, "Guild registered for the siege");

	MU_DrawTextOut(241, ContentY+2, "Guild registered for the siege:",0,0,0);


	float GuildX = ContentX + 28;
	float GuildY = ContentY;
	for(int i = 0; i < this->ireg_list_count; i++)
	{
		int iObj = ePT_DUNGEON_SIEGE_GUILD1 + i;
		GuildY += 10;
		//gInterfaces->DrawGUIs(iObj, GuildX, GuildY);
		gInterfaces->DrawGUIex(iObj);
		//gInterfaces->DrawFormat(eYellow, (GuildX-28), (GuildY+5), 230, 3, this->reg_list[i].szGuildName);
		char szTemp25[256];
		sprintf(szTemp25,"%s",this->reg_list[i].szGuildName);
		pDrawTextOut((GuildX+48), (GuildY+3), (char*)szTemp25, 0, 0, 0);
	}

	ContentY += 215;
	//gInterfaces->DrawGUIs(ePT_DUNGEON_SIEGE_DIV2, ContentX, ContentY);
	gInterfaces->DrawGUIex(ePT_DUNGEON_SIEGE_DIV2);
	ContentY += 16;
	//gInterfaces->DrawGUIs(ePT_DUNGEON_SIEGE_OK, ButtonX, ContentY);
	gInterfaces->DrawGUIex(ePT_DUNGEON_SIEGE_OK);
	
	//gInterfaces->DrawFormat(eWhite, ContentX, ContentY+10, 230, 3, "Ok");
	MU_DrawTextOut(274, ContentY+2, "Ok",0,0,0);
	if(gInterfaces->IsWorkZoneObj(ePT_DUNGEON_SIEGE_OK))
	{
		Color = eGray100;
		if(gInterfaces->Data[ePT_DUNGEON_SIEGE_OK].OnClick)
		{
			Color = eGray150;
		}
		//gInterfaces->DrawGUIs(ePT_DUNGEON_SIEGE_OK, gInterfaces->Data[ePT_DUNGEON_SIEGE_OK].X, gInterfaces->Data[ePT_DUNGEON_SIEGE_OK].Y);
		gInterfaces->DrawGUIex(ePT_DUNGEON_SIEGE_OK);
		//gInterfaces->DrawColoredGUIObj(ePT_DUNGEON_SIEGE_OK, gInterfaces->Data[ePT_DUNGEON_SIEGE_OK].X, gInterfaces->Data[ePT_DUNGEON_SIEGE_OK].Y, Color);
	}
	// -------------------
	// Reset Info End
	// -------------------
	
	pDrawImage2(240, 242, 272, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(242, 272, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, 242, 272, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		
	}
	pDrawTextOut((int)242 + 40, (int)278, (char*)"Registration", 0, 0, 0);
}

void CDungeonSiege::Button(DWORD Event)
{

	DWORD Delays = (GetTickCount() - this->ClickTime);

	if(CheckZone(374, 13.125, 16.0, 16.0))//gInterfaces->IsWorkZone(eBankClose))
	{
		if(Event == WM_LBUTTONDOWN && Delays > 100)
		{
			Sleep(100);
				this->ClickTime = GetTickCount();
				this->Active  = false;
				this->State  = 0;
				return;
			}
		}
	//if(!gInterfaces->CheckWindowEx(ObjWindowsEx::exWinDungeonSiege))
	//{
	//	return;
	//}
	////=============================================================================
	//if(gInterfaces->ButtonEx(Event, ePT_DUNGEON_SIEGE_REG, true))
	//{
	//	PMSG_PROTOCOL_RESULT pMsg;
	//	pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0x44,  sizeof(pMsg));
	//	pMsg.Result = true;
	//	//gProtocol.DataSend((LPBYTE)&pMsg, sizeof(pMsg));
	//	DataSend((LPBYTE)&pMsg, pMsg.h.size);
	//}

	/*
	else if(gInterfaces->ButtonEx(Event, ePT_DUNGEON_SIEGE_OK, false))
	{
		gInterfaces->CloseWindowEx(ObjWindowsEx::exWinDungeonSiege);
	}*/
}

void CDungeonSiege::GCNpcInfo(PMSG_GC_DUNGEON_SIEGE_NPC* lpMsg)
{
	//MessageBoxA(NULL, "DS", "ERROR", MB_SERVICE_NOTIFICATION | MB_ICONWARNING);
		char szOwName[9];
	memset(this->reg_list, 0, sizeof(this->reg_list));

	szOwName[8] = 0;
	this->bCaptured = lpMsg->Captured;
	memcpy(szOwName, lpMsg->szGuildOwner, 8);
	memcpy(this->szLordGuild, szOwName, 8);

	this->bRegistration = lpMsg->bResultReg;
	this->iMsgCode = lpMsg->iMsgCode;
	this->ireg_list_count = lpMsg->iCountGuild;

	for(int i = 0; i < this->ireg_list_count; i++)
	{
		char szGName[9];
		szGName[8] = 0;
		memcpy(szGName, lpMsg->szGuildName[i], 8);
		memcpy(this->reg_list[i].szGuildName, szGName, 8);	
	}

	//gInterfaces->OpenWindowEx(ObjWindowsEx::exWinDungeonSiege);
	this->Active = true;
	//this->Draw();
}

void CDungeonSiege::CGHWID()
{
	PMSG_CG_DUNBEON_SIEGE_HWID pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0x45,  sizeof(pMsg));
	pMsg.dwHWID = this->dwHWID;
	DataSend((LPBYTE)&pMsg, pMsg.h.size);
	//gProtocol.DataSend((LPBYTE)&pMsg, sizeof(pMsg));
}

void CDungeonSiege::GCVieweSiege(LPBYTE aRecv)
{
	memset(this->m_viewe, 0, sizeof(this->m_viewe));

	int datasize = 6;
	int excount = aRecv[datasize-1];

	for (int n = 0; n < excount; n++)
	{
		DUNGEON_VIEWE Vp;
		memcpy(&Vp,&aRecv[datasize],sizeof(Vp));

		this->m_viewe[n].aIndex = Vp.aIndex;
		this->m_viewe[n].GuildNumber = Vp.GuildNumber;

		datasize += sizeof(Vp);
	}
}

void CDungeonSiege::SetPKLevelTarget()
{
	if(this->CheckStatus(eEVENTDUNGEON_CLOSE))
	{
		return;
	}

	if(gObjUser.m_MapNumber != MAP_INDEX_DUNGEON)
	{
		return;
	}

	if(this->PlayerGuildNumber < 1)
	{
		return;
	}

	for(int i = 0; i < 400; i++)
	{
		lpViewObj lpObj    = &*(ObjectPreview*)pGetPreviewStruct(pPreviewThis(), i);

		if( !lpObj || !lpObj->m_Model.Unknown4 )
		{
			continue;
		}

		if(lpObj->m_Model.ObjectType != emPlayer)
		{
			continue;
		}

		if(lpObj->VPIndex == MAIN_STATEEx)
		{
			lpObj->PKLevel = 1;
			continue;
		}

		for(int n = 0; n < 75; n++)
		{
			DUNGEON_VIEWE* lpViewe = &this->m_viewe[n];

			if(lpViewe->aIndex == lpObj->VPIndex)
			{
				if(this->PlayerGuildNumber == lpViewe->GuildNumber)
				{
					lpObj->PKLevel = 1;
				}
				else
				{
					lpObj->PKLevel = 7;
				}
				break;
			}
		}
	}
}

void CDungeonSiege::GCGuildNumber(PMSG_GC_GUILD_INDEX* lpMsg)
{
	this->PlayerGuildNumber = lpMsg->GuildNumber;
}

void CDungeonSiege::DrawRanking()
{
	//if(this->CheckStatus(eEVENTDUNGEON_CLOSE))
	//{
	//	return;
	//}

	bool DrawTable = true;

	//if(    gInterfaces->CheckWindow(Party)
	//	|| gInterfaces->CheckWindow(Guild)
	//	|| gInterfaces->CheckWindow(Trade)
	//	|| gInterfaces->CheckWindow(Warehouse)
	//	|| gInterfaces->CheckWindow(ChaosBox)
	//	|| gInterfaces->CheckWindow(CommandWindow)
	//	|| gInterfaces->CheckWindow(PetInfo)
	//	|| gInterfaces->CheckWindow(Shop)
	//	|| gInterfaces->CheckWindow(Inventory)
	//	|| gInterfaces->CheckWindow(Store)
	//	|| gInterfaces->CheckWindow(OtherStore)
	//	|| gInterfaces->CheckWindow(Character)
	//	|| gInterfaces->CheckWindow(FastMenu)
	//	|| gInterfaces->CheckWindow(SkillTree)
	//	//|| gInterfaces->CheckWindow(NPC_Titus)
	//	|| gInterfaces->CheckWindow(CashShop)
	//	/*|| gInterfaces->CheckWindow(FullMap)
	//	|| gInterfaces->CheckWindow(NPC_Dialog)
	//	|| gInterfaces->CheckWindow(GensInfo)
	//	|| gInterfaces->CheckWindow(NPC_Julia)
	//	|| gInterfaces->CheckWindow(ExpandInventory)
	//	|| gInterfaces->CheckWindow(ExpandWarehouse) 
	//	|| gInterfaces->CheckWindow(MuHelper) */
	//	|| gInterfaces->CheckWindow(Quest) 
	//	/*|| gInterfaces->CheckWindow(NPC_Devin) 
	//	|| gInterfaces->CheckWindow(DevilSquare) */
	//	|| gInterfaces->CheckWindow(GuardNPC) 
	//	|| gInterfaces->CheckWindow(CastleGateSwitch) 
	//	|| gInterfaces->CheckWindow(GoldenArcher2) 
	//	//|| gInterfaces->CheckWindow(LuckyCoin1)
	//	//|| gInterfaces->CheckWindow(NPC_Titus)
	//	//|| gInterfaces->CheckWindow(BloodCastle)
	//	)
	//{
	//	DrawTable = false;
	//}

	//if( gInterfaces->CheckWindowEx(exWinRanking)		  ||
	//	gInterfaces->CheckWindowEx(exWinPersonalPrice) )
	//{
	//	DrawTable = false;
	//}

	//if(g_ExMenuV3.m_Enable && g_ExMenuV3.m_Version == EXMENU_NUMBER::eVERSION_ET)
	//{
	//	if(gInterfaces->CheckWindowEx(exWinMenuV3))
	//	{
	//		DrawTable = false;
	//	}
	//}

	if(!DrawTable)
	{
		return;
	}

	if(this->CheckStatus(eEVENTDUNGEON_WAIT) || this->CheckStatus(eEVENTDUNGEON_START_STAGE1))
	{
		//float RightMiniInfoY = 430;
		int mCount = 0;
		bool DrInfo = false;

		if(this->m_insiege)
		{
			mCount += 10;
		}

		for(int i = 0; i < this->ireg_list_count; i++)
		{
			mCount += 10;
			DrInfo = true;
		}

		if(!DrInfo)
		{
			return;
		}

		float Wind = 100;
		float Hig = 10 + mCount;
		//float StartiY = RightMiniInfoY - 10;
		float StartiY = gInterfaces->RightMiniInfoY - 10;
		
		float iX = 535;
		float iY = StartiY - Hig;	

		gInterfaces->RightMiniInfoY -= Hig;
		

		char szTemp8[256];
		sprintf(szTemp8 ,"[%s]", this->m_EventName);
		pDrawTextOut(iX, iY, (char*)szTemp8, 100, 3, 0);

		//pDrawColorButton(0x7880, iX, iY, Wind, Hig, NULL, NULL, Color4f(0, 0, 0, 100));

		//gInterfaces->DrawFormat(eOrange, iX, iY, 100, 3, "[%s]", this->m_EventName);
		
		if(this->m_insiege)
		{
			iY += 10;
			//gInterfaces->DrawFormat(eYellow, iX+10, iY, 200, 1, "My Score: %d", this->m_mysocre);
			char szTemp7[256];
			sprintf(szTemp7 ,"My Score: %d", this->m_mysocre);
			pDrawTextOut(iX+10, iY, (char*)szTemp7, 200, 1, 0);
		}

		for(int i = 0; i < this->ireg_list_count; i++)
		{
			iY+=10;
			char szTemp6[256];
			sprintf(szTemp6 ,"%s: %d", this->reg_list[i].szGuildName, this->m_guildscore[i]);
			pDrawTextOut(iX+10, iY, (char*)szTemp6, 200, 1, 0);
			//gInterfaces->DrawFormat(eWhite, iX+10, iY, 200, 1, "%s: %d", this->reg_list[i].szGuildName, this->m_guildscore[i]);	
		}
	}
	else if(this->CheckStatus(eEVENTDUNGEON_WAIT_STAGE2) || this->CheckStatus(eEVENTDUNGEON_START_STAGE2))
	{
		//float RightMiniInfoY = 430;
		int mCount = 0;

		mCount += 10;
		mCount += 10;
		mCount += 10;
		mCount += 10;

		float Wind = 100;
		float Hig = 10 + mCount;
		//float StartiY = RightMiniInfoY - 10;
		float StartiY = gInterfaces->RightMiniInfoY - 15;
		gInterfaces->RightMiniInfoY -= Hig;

		float iX = 535;
		float iY = StartiY - Hig;	

		//pDrawColorButton(0x7880, iX, iY, Wind, Hig, NULL, NULL, Color4f(0, 0, 0, 100));
		//gInterfaces->DrawFormat(eOrange, iX, iY, 100, 3, "[%s]", this->m_EventName);
		char szTemp1[256];
			sprintf(szTemp1 ,"[%s]", this->m_EventName);
			pDrawTextOut(iX, iY, (char*)szTemp1, 100, 3, 0);
		
		iY += 10;
		//gInterfaces->DrawFormat(eYellow, iX+10, iY, 200, 1, "- %s", this->reg_list[this->m_finalguild1].szGuildName);
		char szTemp2[256];
			sprintf(szTemp2 ,"- %s", this->reg_list[this->m_finalguild1].szGuildName);
			pDrawTextOut(iX+10, iY, (char*)szTemp2, 200, 1, 0);
		iY += 10;
		//gInterfaces->DrawFormat(eWhite, iX+10, iY, 200, 1, "%s: %d", this->m_finalname1 , this->m_finalscore1);
		char szTemp3[256];
			sprintf(szTemp3 ,"%s: %d", this->m_finalname1 , this->m_finalscore1);
			pDrawTextOut(iX, iY, (char*)szTemp3, 200, 1, 0);

		iY += 10;
		//gInterfaces->DrawFormat(eYellow, iX+10, iY, 200, 1, "- %s", this->reg_list[this->m_finalguild2].szGuildName);
		char szTemp4[256];
			sprintf(szTemp4 ,"- %s", this->reg_list[this->m_finalguild2].szGuildName);
			pDrawTextOut(iX, iY, (char*)szTemp4, 200, 1, 0);
		iY += 10;
	//	gInterfaces->DrawFormat(eWhite, iX+10, iY, 200, 1, "%s: %d", this->m_finalname2 , this->m_finalscore2);
		char szTemp5[256];
			sprintf(szTemp5 ,"%s: %d", this->m_finalname2 , this->m_finalscore2);
			pDrawTextOut(iX, iY, (char*)szTemp5, 200, 1, 0);
	}
}

void CDungeonSiege::GCSiegeScore(PMSG_DUNGEON_SIEGE_SCORE* lpMsg)
{
	this->m_mysocre = lpMsg->MyScore;
	int iGuildCount = lpMsg->GuildCount;
	this->ireg_list_count = iGuildCount;
	for(int i = 0; i < iGuildCount; i++)
	{
		this->m_guildscore[i] = lpMsg->GuildScore[i];
	}
	this->m_finalguild1 = lpMsg->FinalGuildID1;
	this->m_finalguild2 = lpMsg->FinalGuildID2;
	this->m_finalscore1 = lpMsg->FinalScore1;
	this->m_finalscore2 = lpMsg->FinalScore2;

	this->m_finalname1[10] = 0;
	this->m_finalname2[10] = 0;
	memcpy(this->m_finalname1, lpMsg->Name1, 10);
	memcpy(this->m_finalname2, lpMsg->Name2, 10);

	this->m_insiege = lpMsg->InSiege;
}

void CDungeonSiege::GCPlayerConnect(PMSG_DUNGEON_SIEGE_PLAYER_CONNECT* lpMsg)
{
	char szOwName[9];
	memset(this->reg_list, 0, sizeof(this->reg_list));

	szOwName[8] = 0;
	this->bCaptured = lpMsg->Captured;
	memcpy(szOwName, lpMsg->szGuildOwner, 8);
	memcpy(this->szLordGuild, szOwName, 8);

	this->ireg_list_count = lpMsg->iCountGuild;

	for(int i = 0; i < this->ireg_list_count; i++)
	{
		char szGName[9];
		szGName[8] = 0;
		memcpy(szGName, lpMsg->szGuildName[i], 8);
		memcpy(this->reg_list[i].szGuildName, szGName, 8);	
	}
	memset(this->m_EventName, 0, sizeof(this->m_EventName));
	memcpy(this->m_EventName, lpMsg->EventName, sizeof(this->m_EventName));
}

void CDungeonSiege::GCEventState(PMSG_DUNGEON_SIEGE_STATE* lpMsg)
{
	this->iEventStatus = lpMsg->EventState;
}

bool CDungeonSiege::CheckStatus(int Status)
{
	if(this->iEventStatus == Status)
	{
		return true;
	}

	return false;
}

#endif