#include "stdafx.h"
#include "ResetSystem.h"
#include "Interface.h"
#include "Protocol.h"
#include "Control.h"
#include "Other.h"
#include "InterfaceEx.h"
#include "Interface.h"
#include "TMemory.h"
#include "Controller.h"
#include "Structure.h"
#include "DungeonSiege.h"


void CResetSystem::GCGrandDialogInfo(PMSG_GRAND_DIALOG* lpMsg)
{
	//this->m_Grand.Result = lpMsg->Result;
	//this->m_Grand.GrandNum = lpMsg->GrandNum;
	//this->m_Grand.GrandMax = lpMsg->GrandMax;
	//this->m_Grand.NeedReset = lpMsg->NeedReset;
	//this->m_Grand.NeedLevel = lpMsg->NeedLevel;
	//this->m_Grand.NeedMoney = lpMsg->NeedMoney;
	//this->m_Grand.RewardPoint = lpMsg->RewardPoint;
	//this->m_Grand.RewardWCoin = lpMsg->RewardWCoin;
	//this->m_Grand.RewardCredit = lpMsg->RewardCredit;

	this->m_Grand.Result = lpMsg->Result;
	this->m_Grand.GrandNum = lpMsg->GrandNum;
	this->m_Grand.GrandMax = lpMsg->GrandMax;
	this->m_Grand.NeedReset = lpMsg->NeedReset;
	this->m_Grand.NeedLevel = lpMsg->NeedLevel;
	this->m_Grand.NeedMoney = lpMsg->NeedMoney;
	this->m_Grand.RewardPoint = lpMsg->RewardPoint;
	this->m_Grand.RewardWCoin = lpMsg->RewardWCoin;
	this->m_Grand.RewardCredit = lpMsg->RewardCredit;
	this->m_Grand.ExFreePoint = lpMsg->ExFreePoint;

	this->m_Grand.AddEnableItem = lpMsg->AddEnableItem;
	this->m_Grand.AddItem = lpMsg->AddItem;
	this->m_Grand.AddItemLevel = lpMsg->AddItemLevel;
	this->m_Grand.AddItemSkill = lpMsg->AddItemSkill;
	this->m_Grand.AddItemLuck = lpMsg->AddItemLuck;
	this->m_Grand.AddItemOpt = lpMsg->AddItemOpt;
	this->m_Grand.AddItemExl = lpMsg->AddItemExl;
	this->m_Grand.AddItemAnc = lpMsg->AddItemAnc;
	this->m_Grand.AddItemCount = lpMsg->AddItemCount;

	this->m_Grand.RewardEnableItem = lpMsg->RewardEnableItem;
	this->m_Grand.RewardItem = lpMsg->RewardItem;
	this->m_Grand.RewardLevel = lpMsg->RewardLevel;
	this->m_Grand.RewardSkill = lpMsg->RewardSkill;
	this->m_Grand.RewardLuck = lpMsg->RewardLuck;
	this->m_Grand.RewardOpt = lpMsg->RewardOpt;
	this->m_Grand.RewardExl = lpMsg->RewardExl;
	this->m_Grand.RewardAnc = lpMsg->RewardAnc;

}

void CGGrandMenu()
{
	PMSG_RESET_RESULT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0xA1, sizeof(pMsg));
	pMsg.Result = 1;
	DataSend((LPBYTE)&pMsg, sizeof(pMsg));
}

void CResetSystem::DrawGrand()
{
	if (this->StateGR == 1)
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
		CGGrandMenu();
		// ----
		pBlendSwitch(1);
	DWORD ItemNameColor		= eWhite;
	float MainWidth			= 230.0;
	float MainHeight		= 313.0;
	float StartY			= 66.0;
	float StartX			= (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	float MainCenter		= StartX + (MainWidth / 3);
	float ButtonX			= MainCenter - (30.0 / 2);
	char szGetItem[150];

	this->MainTableHigh = 300;
	this->MainTableWidth = 300;
	this->DrawStartX = 640/2 - MainTableWidth/2;
	this->DrawStartY = 50;
	this->DrawTable = false;
		// ----														//0,0, 
		pDrawImage2(3592, StartX, StartY, MainTableWidth, MainTableHigh, 0, 0, 0.94, 0.6, 1, 1, 0.0);

		// ----

		if (gInterfaces->Data[eResetClose].OnClick)
		{
			gInterfaces->DrawGUI(0x119, eResetClose, 374, 69.125);
		}
		else
		{
			gInterfaces->DrawGUI(0x118, eResetClose, 374, 69.125);
		}
		if (gInterfaces->IsWorkZone(eResetClose))
		{
			if (gController.wMouse == WM_LBUTTONDOWN)
			{
				this->Active = false;
				return;
			}

		}

	

	MU_DrawTextOut(274, 85, "Grand System", 0, 0, 0);

	GRANDSYSTEM_DATA * pGrand = &this->m_Grand;

	//Reset Info Start
	char szTemp1[256];
	sprintf(szTemp1, "Grand Reset:  %d / %d", pGrand->GrandNum, pGrand->GrandMax);

	pDrawTextOut(StartX + 60, 105, (char*)szTemp1, 0, 0, 0);
	////gInterface.DrawGUI(eGRANDSYSTEM_COUNT, StartX + 30, 127);
	//gInterface.DrawFormat(eAncient, StartX + 10, 130, 210, 3, "Grand:  %d / %d", pGrand->GrandNum, pGrand->GrandMax);

	// ----
	// ----

	int CountMissionY = 130;

	pDrawTextOut(StartX + 65, CountMissionY - 10, "Need to Grand:", 0, 0, 0);

	//Need Level
	pDrawTextOut(StartX + 15, CountMissionY + 5, "Need Level:", 0, 0, 0);

	char szTemp2[256];
	sprintf(szTemp2, "%d", pGrand->NeedLevel);
	pDrawTextOut(StartX + 30 + 80, CountMissionY + 5, (char*)szTemp2, 0, 0, 0);
	CountMissionY += 20;

	//Need Reset
	pDrawTextOut(StartX + 15, CountMissionY + 5, "Need Reset:", 0, 0, 0);

	char szTemp3[256];
	sprintf(szTemp3, "%d", pGrand->NeedReset);
	pDrawTextOut(StartX + 30 + 80, CountMissionY + 5, (char*)szTemp3, 0, 0, 0);
	CountMissionY += 20;

	//Need Money
	pDrawTextOut(StartX + 15, CountMissionY + 5, "Money Zen:", 0, 0, 0);
	char szTemp4[256];
	sprintf(szTemp4, "%d", pGrand->NeedMoney);
	pDrawTextOut(StartX + 30 + 80, CountMissionY + 5, (char*)szTemp4, 0, 0, 0);
	CountMissionY += 20;


	//Need Items
	if(pGrand->AddEnableItem)
	{
		strcpy(szGetItem, this->GetItemString1(pGrand->AddItem, pGrand->AddItemLevel+1, pGrand->AddItemSkill, pGrand->AddItemLuck, pGrand->AddItemOpt, pGrand->AddItemExl, pGrand->AddItemAnc));
		char szTemp5[256];
		sprintf(szTemp5 , "%s", szGetItem);
		pDrawTextOut(StartX + 15, CountMissionY + 5, (char*)szTemp5, 0, 0, 0);

		char szTemp10[256];
		sprintf(szTemp10 ,"Count: %d", pGrand->AddItemCount);
		pDrawTextOut(StartX + 30 + 82, CountMissionY + 15, (char*)szTemp10, 0, 3, 0);
		CountMissionY += 10;
	}

	int CountMissionY2 = 200;
	CountMissionY2 += 20;
	pDrawTextOut(StartX + 60, CountMissionY2, "Reward for Grand:", 0, 0, 0);
	CountMissionY2 += 10;

	// Reward points
	pDrawTextOut(StartX + 15, CountMissionY2 + 5, "Points:", 0, 0, 0);
	char szTemp6[256];
	sprintf(szTemp6, "%d", pGrand->RewardPoint);
	pDrawTextOut(StartX + 30 + 80, CountMissionY2 + 5, (char*)szTemp6, 0, 0, 0);
	CountMissionY2 += 20;

	// Reward Wcoin
	if (pGrand->RewardWCoin > 0)
	{
		pDrawTextOut(StartX + 15, CountMissionY2 + 5, "CSPoints:", 0, 0, 0);

		char szTemp7[256];
		sprintf(szTemp7, "%d", pGrand->RewardWCoin);
		pDrawTextOut(StartX + 30 + 80, CountMissionY2 + 5, (char*)szTemp7, 0, 0, 0);
		CountMissionY2 += 20;
	}

	// Reward Credits
	if (pGrand->RewardCredit > 0)
	{
		pDrawTextOut(StartX + 15, CountMissionY2 + 5, "Credits:", 0, 0, 0);
		char szTemp8[256];
		sprintf(szTemp8, "%d", pGrand->RewardCredit);
		pDrawTextOut(StartX + 30 + 80, CountMissionY2 + 5, (char*)szTemp8, 0, 0, 0);
		CountMissionY2 += 20;
	}
	// Reward Item
	if (pGrand->RewardEnableItem)
	{
		strcpy(szGetItem, this->GetItemString1(pGrand->RewardItem, pGrand->RewardLevel + 1, pGrand->RewardSkill, pGrand->RewardLuck, pGrand->RewardOpt, pGrand->RewardExl, pGrand->RewardAnc));
		pDrawTextOut(StartX + 15, CountMissionY2 + 5, szGetItem, 0, 0, 0);
	}



	// -------------------
	// Reset Info End
	// -------------------

	pDrawImage2(240, 242, 327, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(242, 327, 120.0, 22.0))
	{
		glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
		pBlendDisableEx();
		pDrawImage2(240, 242, 327, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
		glColor3f(1065353216, 1065353216, 1065353216);
		pGLSwitch();

	}
	pDrawTextOut((int)242 + 40, (int)333, (char*)"Grand Reset", 0, 0, 0);
	}
}

void CResetSystem::GrandButton(DWORD Event)
{
	DWORD CurrentTick = GetTickCount();


	if (gInterfaces->IsWorkZone(eResetClose))
	{
		//DWORD Delay = (CurrentTick - this->Data[eBankClose].EventTick);
		// ----
		if (Event == WM_LBUTTONDOWN)
		{
			Sleep(100);
			//pSetCursorFocus = false;
			//pSetCursorFocus2 = false;
			this->Active = false;
			this->StateGR = 0;
			return;
		}
	}
}

