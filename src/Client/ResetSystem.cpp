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
#include "GrandReset.h"
#include "RageSystem.h"
#include "Ranking.h"
#include "DungeonSiege.h"

CResetSystem g_ResetSystem;

CResetSystem::CResetSystem()
{
	this->Init();
}

CResetSystem::~CResetSystem()
{
}

void CResetSystem::Init()
{
	memset(&this->m_reset, 0, sizeof(this->m_reset));
}

void CResetSystem::Load()
{
	this->Init();
}

void CResetSystem::BindImages()
{

}


void CGResetMenu()
{
	PMSG_RESET_RESULT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0xA0,  sizeof(pMsg));
	pMsg.Result = 1;
	DataSend((LPBYTE)&pMsg, sizeof(pMsg));
}
void CResetSystem::DrawReset()
{

	if (this->State == 1)
	{

		pBlendSwitch(1);
			//gJewelsBank.Active = false;
			gRanking.Active = false;
			g_ResetSystem.StateGR = 0;//xz
			gRanking.Active = 0;
			gInterfaces->Data[eRageTable].OnShow = false; 
			g_DungeonSiege.Active = false;
	CGResetMenu();
	if (pCheckWindow(0x1B)) //inventory
	{
		// keybd_event('V', 0,0,0);
			//keybd_event('V', 0,KEYEVENTF_KEYUP,0);
		//system("PAUSE");
		DrawTextOut(0, 105, "Close, Inventory first (V)",0,0,0);
		return;
		//return;
	}
	// ----
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
	gInterfaces->DrawGUI(0x119, eBankClose, 374, 69.125);
	/*if (gInterfaces->Data[eResetClose].OnClick)
	{
		gInterfaces->DrawGUI(0x119, eResetClose, 374, 69.125);
	}
	else
	{
		gInterfaces->DrawGUI(0x118, eResetClose, 374, 69.125);
	}
	if (gInterfaces->IsWorkZone(eResetClose))
	{
		if(gController.wMouse == WM_LBUTTONDOWN)
		{
			this->Active  = false;
			return;
		}
			
	}*/

	// ----
	MU_DrawTextOut(274, 85, "Reset System",0,0,0);


	RESETSYSTEM_DATA * res = &this->m_reset;

	//Reset Info Start
	char szTemp1[256];
	sprintf(szTemp1 ,"Reset:  %d / %d", res->Reset , res->MaxReset);
	
	pDrawTextOut(StartX + 70, 105, (char*)szTemp1, 0,0, 0);


	// ----

	int CountMissionY = 130;
	
	pDrawTextOut(StartX + 70, CountMissionY - 10, "Need to Reset:", 0, 0, 0);

	//Need Level
	pDrawTextOut(StartX + 15, CountMissionY +5, "Need Level:", 0, 0, 0);

	char szTemp3[256];
	sprintf(szTemp3 ,"%d", res->NeedLevel);
	pDrawTextOut(StartX + 30 + 90, CountMissionY + 5, (char*)szTemp3, 0, 0, 0);
	CountMissionY += 15;

	//Need Money
	pDrawTextOut(StartX + 15, CountMissionY +5, "Money Zen:", 0, 0, 0);
	char szTemp4[256];
	sprintf(szTemp4 ,"%d", res->NeedMoney);
	pDrawTextOut(StartX + 30 + 90, CountMissionY + 5, (char*)szTemp4, 0, 0, 0);
	CountMissionY += 15;

	//Need Quest
	pDrawTextOut(StartX + 15, CountMissionY +5, "Need MobQuest ¹:", 0, 0, 0);

	char szTemp13[256];
	sprintf(szTemp13 ,"%d", res->NeedQuestN);
	pDrawTextOut(StartX + 30 + 90, CountMissionY + 5, (char*)szTemp13, 0, 0, 0);
	CountMissionY += 15;
	//Need Items
	if(res->AddEnableItem)
	{
		pDrawTextOut(StartX + 15, CountMissionY + 5, "Need Item:", 0, 0, 0);
		glColor3f(0.83f, 0.667f, 0.3154f);
		strcpy(szGetItem, this->GetItemString(res->AddItem, res->AddItemLevel+1, res->AddItemSkill, res->AddItemLuck, res->AddItemOpt, res->AddItemExl, res->AddItemAnc));
		char szTemp5[256];
		sprintf(szTemp5 , "%s", szGetItem);
		
		//pDrawTextOut(StartX + 15, CountMissionY + 5, (char*)szTemp5, 0, 0, 0);
		
		pDrawTextOut(StartX + 100, CountMissionY + 5, (char*)szTemp5, 0, 0, 0);
		CountMissionY += 10;
	if (CheckZone(StartX + 100, CountMissionY - 5 , 66.0, 12.0))
	{
			float flWidth = 94;
			float flHight = 100;
			SetBlend(1);
			glColor4f(0.f, 0.f, 0.f, 0.9f);
			DrawBarForm((float)(StartX+186), (float)(StartY), (float)(flWidth)-20.f , 294.f);
			GLSwitchBlend();

			//glColor4f(1.f, 1.f, 1.f, 1.f);
			DrawItemIMG(StartX + 220,   CountMissionY + 5, 1.0, 1.0, res->AddItem, res->AddItemLevel, res->AddItemExl, res->AddItemAnc, 1);

			/*GLSwitch();
			glColor3f(1.f, 1.f, 1.f);*/
	}

		

		//strcpy(szGetItem, this->GetItemString(res->AddItem, res->AddItemLevel+1, res->AddItemSkill, res->AddItemLuck, res->AddItemOpt, res->AddItemExl, res->AddItemAnc));
		/*char szTemp15[256];
		if(res->AddItemLevel >=1 && res->AddItemSkill >=1 && res->AddItemLuck >=1 && res->AddItemOpt>=1 && res->AddItemExl>=1 && res->AddItemAnc >=1)
		{
			sprintf(szTemp15 , "+%d+Skill+Luck+%dadd+Exc+Acn", res->AddItemLevel,res->AddItemOpt*4);
			pDrawTextOut(StartX + 55, CountMissionY + 5, (char*)szTemp15, 0, 0, 0);
		}*/
		///////////////////////////////////////////////////////////////////////////////////////
		//SetBlend(1);
	//glColor4f(0.f, 0.f, 0.f, 0.5f);
	//DrawBarForm((float)(PosX + 10), (float)(PosY - 2), 20.f + (float)(Wind), 60.f);
	//GLSwitchBlend();

	//glColor4f(1.f, 1.f, 1.f, 1.f);
	
		if(res->AddItemLevel >=1)
		{
			char szTemp100[256];
			sprintf(szTemp100 , "+%dLvl", res->AddItemLevel);
			pDrawTextOut(StartX + 15, CountMissionY + 5, (char*)szTemp100, 0, 0, 0);

		}
		else
		{

		}
		if(res->AddItemSkill >=1)
		{
			char szTemp101[256];
			sprintf(szTemp101 , "+Skill");
			pDrawTextOut(StartX + 40, CountMissionY + 5, (char*)szTemp101, 0, 0, 0);

		}
		else
		{

		}
		if(res->AddItemLuck >=1)
		{
			char szTemp102[256];
			sprintf(szTemp102 , "+Luck");
			pDrawTextOut(StartX + 65, CountMissionY + 5, (char*)szTemp102, 0, 0, 0);

		}
		else
		{

		}
		if(res->AddItemOpt>=1)
		{
			char szTemp103[256];
			sprintf(szTemp103 , "+%dadd",res->AddItemOpt*4);
			pDrawTextOut(StartX + 90, CountMissionY + 5, (char*)szTemp103, 0, 0, 0);

		}
		else
		{

		}
		if(res->AddItemExl>=1)
		{
			char szTemp104[256];
			sprintf(szTemp104 , "+Exc");
			pDrawTextOut(StartX + 125, CountMissionY + 5, (char*)szTemp104, 0, 0, 0);

		}
		else
		{

		}
		if(res->AddItemAnc >=1)
		{
			char szTemp105[256];
			sprintf(szTemp105 , "+Anc");
			pDrawTextOut(StartX + 150, CountMissionY + 5, (char*)szTemp105, 0, 0, 0);

		}
		else
		{

		}
		
			//GLSwitch();
	glColor3f(1.f, 1.f, 1.f);
		//glColor3f(1.f, 1.f, 1.f);
		//////////////////////////////////////////////////////////////////////////////////////
	glColor3f(0.9f, 0.f, 0.f);
		char szTemp10[256];
		sprintf(szTemp10 ,"Count: %d", res->AddItemCount);
		pDrawTextOut(StartX + 30 + 90, CountMissionY + 15, (char*)szTemp10, 0, 0, 0);
		glColor3f(1.f, 1.f, 1.f);
		CountMissionY += 10;
	}
	//			GLSwitch();
	//glColor3f(1.f, 1.f, 1.f);
	int CountMissionY2 = 200;
	CountMissionY2 += 15;
	pDrawTextOut(StartX + 60, CountMissionY2, "Reward for reset:", 0, 0, 0);
	CountMissionY2 += 15;

	// Reward points
	pDrawTextOut(StartX +15, CountMissionY2 +5, "Points:", 0, 0, 0);
	char szTemp6[256];
	sprintf(szTemp6 ,"%d", res->RewardPoint);
	pDrawTextOut(StartX + 30 + 90, CountMissionY2 + 5, (char*)szTemp6, 0, 0, 0);
	CountMissionY2 += 15;

	// Reward Wcoin
	if(res->AddWcoinC > 0)
	{
		pDrawTextOut(StartX +15, CountMissionY2 +5, "CSPoints:", 0, 0, 0);

		char szTemp7[256];
		sprintf(szTemp7 ,"%d", res->AddWcoinC);
		pDrawTextOut(StartX + 30 + 90, CountMissionY2 + 5, (char*)szTemp7, 0, 0, 0);
		CountMissionY2 += 15;
	}

	// Reward Credits
	if(res->AddCredits > 0)
	{
		pDrawTextOut(StartX +15, CountMissionY2 +5, "Credits:", 0, 0, 0);
		char szTemp8[256];
		sprintf(szTemp8 ,"%d", res->AddCredits);
		pDrawTextOut(StartX + 30 + 90, CountMissionY2 + 5, (char*)szTemp8, 0, 0, 0);
		CountMissionY2 += 15;
	}
	// Reward Item
	if(res->RewardEnableItem)
	{
		//strcpy(szGetItem,this->GetItemString(res->RewardItem, res->RewardLevel+1, res->RewardSkill, res->RewardLuck, res->RewardOpt, res->RewardExl, res->RewardAnc));
		//pDrawTextOut(StartX +15, CountMissionY2 +5, szGetItem, 0, 0, 0);



		pDrawTextOut(StartX + 15, CountMissionY2 + 5, "Reward Item:", 0, 0, 0);

		glColor3f(0.3689f, 0.58f, 0.2552f);

		strcpy(szGetItem,this->GetItemString(res->RewardItem, res->RewardLevel+1, res->RewardSkill, res->RewardLuck, res->RewardOpt, res->RewardExl, res->RewardAnc));
		char szTemp51[256];
		sprintf(szTemp51 , "%s", szGetItem);
		pDrawTextOut(StartX + 100, CountMissionY2 + 5, (char*)szTemp51, 0, 0, 0);


		CountMissionY2 += 10;
		if (CheckZone(StartX + 100, CountMissionY2 - 5 , 66.0, 12.0))
		{
			float flWidth = 94;
			float flHight = 100;
			SetBlend(1);
			glColor4f(0.f, 0.f, 0.f, 0.9f);
			DrawBarForm((float)(StartX+186), (float)(StartY), (float)(flWidth)-20.f , 294.f);
			GLSwitchBlend();
			DrawItemIMG(StartX + 220,   CountMissionY2 + 5, 1.0, 1.0, res->RewardItem, res->RewardLevel, res->RewardExl, res->RewardAnc, 1);
		}
	
		if(res->RewardLevel >=1)
		{
			char szTemp200[256];
			sprintf(szTemp200 , "+%dLvl", res->RewardLevel);
			pDrawTextOut(StartX + 15, CountMissionY2 + 5, (char*)szTemp200, 0, 0, 0);

		}
		else
		{

		}
		if(res->RewardSkill >=1)
		{
			char szTemp201[256];
			sprintf(szTemp201 , "+Skill");
			pDrawTextOut(StartX + 40, CountMissionY2 + 5, (char*)szTemp201, 0, 0, 0);

		}
		else
		{

		}
		if(res->RewardLuck >=1)
		{
			char szTemp202[256];
			sprintf(szTemp202 , "+Luck");
			pDrawTextOut(StartX + 65, CountMissionY2 + 5, (char*)szTemp202, 0, 0, 0);

		}
		else
		{

		}
		if(res->RewardOpt>=1)
		{
			char szTemp203[256];
			sprintf(szTemp203 , "+%dadd",res->RewardOpt*4);
			pDrawTextOut(StartX + 90, CountMissionY2 + 5, (char*)szTemp203, 0, 0, 0);

		}
		else
		{

		}
		if(res->RewardExl>=1)
		{
			char szTemp204[256];
			sprintf(szTemp204 , "+Exc");
			pDrawTextOut(StartX + 125, CountMissionY2 + 5, (char*)szTemp204, 0, 0, 0);

		}
		else
		{

		}
		if(res->RewardAnc >=1)
		{
			char szTemp205[256];
			sprintf(szTemp205 , "+Anc");
			pDrawTextOut(StartX + 150, CountMissionY2 + 5, (char*)szTemp205, 0, 0, 0);

		}
		else
		{

		}
		
			//GLSwitch();
	glColor3f(1.f, 1.f, 1.f);


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
	pDrawTextOut((int)242 + 50, (int)333, (char*)"Reset", 0, 0, 0);
	}
}

void CResetSystem::Button(DWORD Event)
{
	
	DWORD Delays = (GetTickCount() - this->ClickTime);

	if(CheckZone(374, 69.125, 16.0, 16.0))//gInterfaces->IsWorkZone(eBankClose))
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
}

void CResetSystem::GCDialogInfo(GC_RESET_DATAEx* aRecv)
{
	this->m_reset.Result = aRecv->Result;
	this->m_reset.Reset = aRecv->Reset;
	this->m_reset.MaxReset = aRecv->MaxReset;
	this->m_reset.NeedLevel = aRecv->NeedLevel;
	this->m_reset.NeedQuestN = aRecv->NeedQuestN;
	this->m_reset.NeedMoney = aRecv->NeedMoney;
	this->m_reset.RewardPoint = aRecv->RewardPoint;
	this->m_reset.AddCredits = aRecv->AddCredits;
	this->m_reset.AddWcoinC = aRecv->AddWcoinC;
	this->m_reset.AddWcoinP = aRecv->AddWcoinP;
	this->m_reset.AddWcoinG = aRecv->AddWcoinG;
	this->m_reset.ExFreePoint = aRecv->ExFreePoint;

	this->m_reset.AddEnableItem = aRecv->AddEnableItem;
	this->m_reset.AddItem = aRecv->AddItem;
	this->m_reset.AddItemLevel = aRecv->AddItemLevel;
	this->m_reset.AddItemSkill = aRecv->AddItemSkill;
	this->m_reset.AddItemLuck = aRecv->AddItemLuck;
	this->m_reset.AddItemOpt = aRecv->AddItemOpt;
	this->m_reset.AddItemExl = aRecv->AddItemExl;
	this->m_reset.AddItemAnc = aRecv->AddItemAnc;
	this->m_reset.AddItemCount = aRecv->AddItemCount;

	this->m_reset.RewardEnableItem = aRecv->RewardEnableItem;
	this->m_reset.RewardItem = aRecv->RewardItem;
	this->m_reset.RewardLevel = aRecv->RewardLevel;
	this->m_reset.RewardSkill = aRecv->RewardSkill;
	this->m_reset.RewardLuck = aRecv->RewardLuck;
	this->m_reset.RewardOpt = aRecv->RewardOpt;
	this->m_reset.RewardExl = aRecv->RewardExl;
	this->m_reset.RewardAnc = aRecv->RewardAnc;

//	this->Active = true;
//	gInterface.Data[eRESETSYSTEM_MAIN].OnShow = true;
}

char* CResetSystem::GetItemString(int Item, int Level, bool Skill, bool Luck, bool Opt, bool Exl, bool Anc)
{
	char szItem[150];//0x78CB064
	sprintf(szItem, "%s", (char*)(*(DWORD*)0x78C5234 + 84 * Item));

	char szOptions[64];
	
	if(Level)
	{
		// if(this->m_reset.AddItemLevel == 0  || this->m_reset.RewardLevel == 0)
		 //{
			 //sprintf(szOptions, "");
			sprintf(szOptions,"");
		// }
		/* else if(this->m_reset.AddItemLevel >= 1 || this->m_reset.RewardLevel >= 1)
		 {
			sprintf(szOptions, "+%d", Level-1);
		 }*/

	}
	//if(Skill)
	//{
	//	strcat(szOptions,"+Skill");
	//}
	//if(Luck)
	//{
	//	strcat(szOptions,"+Luck");
	//}
	//if(Opt)
	//{
	//	strcat(szOptions,"+Opt");
	//}
	//if(Exl)
	//{
	//	strcat(szOptions,"+Exc");
	//}
	//if(Anc)
	//{
	//	strcat(szOptions,"+Anc");
	//}
		if(Skill)
	{
		strcat(szOptions,"");
	}
	if(Luck)
	{
		strcat(szOptions,"");
	}
	if(Opt)
	{
		strcat(szOptions,"");
	}
	if(Exl)
	{
		strcat(szOptions,"");
	}
	if(Anc)
	{
		strcat(szOptions,"");
	}
	char szResult[150];
	sprintf(szResult, "%s %s", szItem, szOptions);
	

	return szResult;
}

char* CResetSystem::GetItemString1(int Item, int Level, bool Skill, bool Luck, bool Opt, bool Exl, bool Anc)
{
	char szItem[150];//0x78CB064
	sprintf(szItem, "%s", (char*)(*(DWORD*)0x78C5234 + 84 * Item));

	char szOptions[64];
	
	if(Level)
	{
		if(this->m_Grand.AddItemLevel == 0 || this->m_Grand.RewardLevel == 0)
		 {
			 sprintf(szOptions, "");
		 }
		 else
		 {
			sprintf(szOptions, "+%d", Level-1);
		 }

	}
	if(Skill)
	{
		strcat(szOptions,"+Skill");
	}
	if(Luck)
	{
		strcat(szOptions,"+Luck");
	}
	if(Opt)
	{
		strcat(szOptions,"+Opt");
	}
	if(Exl)
	{
		strcat(szOptions,"+Exc");
	}
	if(Anc)
	{
		strcat(szOptions,"+Anc");
	}
	
	char szResult[150];
	sprintf(szResult, "%s %s", szItem, szOptions);
	

	return szResult;
}