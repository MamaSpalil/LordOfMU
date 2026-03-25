#include "stdafx.h"
#include "CustomMenu.h"
#include "Interface.h"
#include "Control.h"
#include "TMemory.h"
#include "readscript.h"
#include "InterfaceEx.h"
#include "Controller.h"
#include "SelectChar.h"
#include "Ranking.h"
#include "ResetSystem.h"
#include "GrandReset.h"
#include "JewelsBank.h"
#include "Structure.h"
#include "User.h"
#include "ObjectManager.h"
#include "AdvancedStats.h"


//void UPDATE_FPS()
//{



//}
MenuSystem gMenuSystem;

MenuSystem::MenuSystem()
{
	this->Active = false;
	
}

MenuSystem::~MenuSystem()
{

}

void MenuSystem::rDrawInterface()
{


	if(this->Active == false) return;

	if (pCheckWindow(0x1B)) //inventory
	{
		// keybd_event('V', 0,0,0);
			//keybd_event('V', 0,KEYEVENTF_KEYUP,0);
		//system("PAUSE");
		DrawTextOut(0, 105, "Close, Inventory first (V)",0,0,0);
		return;
		//return;
	}
	//pSetCursorFocus = true;
	float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
	float StartY = 100.0f;

	//this->frameCount++;
	//if (GetTickCount() - this->lastReport >= 1000)
	//{
	//	this->frameRate = this->frameCount / ((GetTickCount() - this->lastReport) / 1000);
	//	sprintf(FPS_REAL, "FPS: %d", this->frameRate);
	//	this->lastReport = GetTickCount();
	//	this->frameCount = 0;
	//}

	//pDrawToolTipEx(StartX -85, StartY - 77, (char*)FPS_REAL);

	////pDrawTextOut((int)StartX -85, (int)StartY - 77, (char*)FPS_REAL, 0, 0, 0);


	INFO_DATA * res = &this->m_info;
	//pBlendSwitch(1);
	SetBlend(1);
	glColor4f(0.f, 0.f, 0.f, 0.9f);
	DrawBarForm((float)(StartX - 90), (float)(StartY - 80), 300.f + (float)(0), 210.f);
	GLSwitchBlend();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	gInterfaces->DrawGUI(0x119, eBankClose, StartX +193, StartY - 80);
	gInterfaces->DrawGUIs(AdvanceStats, StartX +163, StartY - 80);



	pDrawTextOut((int)StartX -75, (int)StartY - 55, (char*)"Resets:", 0, 0, 0);
	char szTemp1[256];
	sprintf(szTemp1 ,"%d",res->Reset);
	pDrawTextOut((int)StartX - 10, (int)StartY-55, (char*)szTemp1, 0, 0, 0);

	pDrawTextOut((int)StartX -75, (int)StartY - 42, (char*)"Grand Resets:", 0, 0, 0);
	char szTemp2[256];
	sprintf(szTemp2 ,"%d",res->GReset);
	pDrawTextOut((int)StartX - 10, (int)StartY-42, (char*)szTemp2, 0, 0, 0);

	pDrawTextOut((int)StartX -75, (int)StartY - 29, (char*)"Credits:", 0, 0, 0);
	char szTemp3[256];
	sprintf(szTemp3 ,"%d",res->Credits);
	pDrawTextOut((int)StartX - 10, (int)StartY-29, (char*)szTemp3, 0, 0, 0);

	pDrawTextOut((int)StartX -75, (int)StartY - 16, (char*)"Exp:", 0, 0, 0);
	char szTemp14[256];
	if(res->VipTimeTick > 0)
	{
		if(res->VipGroup == 1)
		{
			//pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"Bronze", 0, 0, 0);
			sprintf(szTemp14 ,"%d",res->Exps+10);
		pDrawTextOut((int)StartX - 10, (int)StartY-16, (char*)szTemp14, 0, 0, 0);
		}
		if(res->VipGroup == 2)
		{
			//pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"Silver", 0, 0, 0);
			sprintf(szTemp14 ,"%d",res->Exps+15);
		pDrawTextOut((int)StartX - 10, (int)StartY-16, (char*)szTemp14, 0, 0, 0);
		}
		if(res->VipGroup == 3)
		{
			//pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"Gold", 0, 0, 0);
			sprintf(szTemp14 ,"%d",res->Exps+20);
			pDrawTextOut((int)StartX - 10, (int)StartY-16, (char*)szTemp14, 0, 0, 0);
		}
		if(res->VipGroup <= 0)
		{
			sprintf(szTemp14 ,"%d",res->Exps);
			pDrawTextOut((int)StartX - 10, (int)StartY-16, (char*)szTemp14, 0, 0, 0);
		}
		if(res->VipGroup >= 4)
		{
			sprintf(szTemp14 ,"%d",res->Exps);
			pDrawTextOut((int)StartX - 10, (int)StartY-16, (char*)szTemp14, 0, 0, 0);
		}
	}
	else{
		
		sprintf(szTemp14 ,"%d",res->Exps);
		pDrawTextOut((int)StartX - 10, (int)StartY-16, (char*)szTemp14, 0, 0, 0);
	}

	//pDrawTextOut((int)StartX -75, (int)StartY - 15, (char*)"CSPoints:", 0, 0, 0);
	//char szTemp4[256];
	//sprintf(szTemp4 ,"%d",res->CSPoints);
	//pDrawTextOut((int)StartX - 10, (int)StartY-15, (char*)szTemp4, 0, 0, 0);


	pDrawTextOut((int)StartX +90, (int)StartY - 55, (char*)"Vip Group:", 0, 0, 0);
	//char szTemp5[256];
	//sprintf(szTemp1 ,"%d",res->VipGroup);
	if(res->VipTimeTick > 0)
	{

		if(res->VipGroup == 1)
		{
			pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"Bronze", 0, 0, 0);
		}
		if(res->VipGroup == 2)
		{
			pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"Silver", 0, 0, 0);
		}
		if(res->VipGroup == 3)
		{
			pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"Gold", 0, 0, 0);
		}
		if(res->VipGroup <= 0)
		{
			pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"-", 0, 0, 0);
		}
		if(res->VipGroup >= 4)
		{
			pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"-", 0, 0, 0);
		}
	}
	else
	{
		pDrawTextOut((int)StartX +155, (int)StartY-55, (char*)"-", 0, 0, 0);
	}
	pDrawTextOut((int)StartX +90, (int)StartY - 42, (char*)"Vip Time:", 0, 0, 0);
	char szTemp5[256];
	sprintf(szTemp5 ,"%d min",res->VipTimeTick / 60);
	pDrawTextOut((int)StartX +155, (int)StartY-42, (char*)szTemp5, 0, 0, 0);

	pDrawTextOut((int)StartX +90, (int)StartY - 29, (char*)"Quest Num:", 0, 0, 0);
	char szTemp13[256];
	sprintf(szTemp13 ,"%d",res->QuestNum);
	pDrawTextOut((int)StartX +155, (int)StartY-29, (char*)szTemp13, 0, 0, 0);
//=====================================================================================================
	pDrawImage2(240, StartX-80, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(StartX-80, StartY, 120.0, 22.0))
	{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX-80, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
	}

	pDrawImage2(240, StartX+80, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(StartX+80, StartY, 120.0, 22.0))
	{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX+80, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
	}

	pDrawImage2(240, StartX-80, StartY+32, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(StartX-80, StartY+32, 120.0, 22.0))
	{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX-80, StartY+32, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
	}

	pDrawImage2(240, StartX+80, StartY+32, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(StartX+80, StartY+32, 120.0, 22.0))
	{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX+80, StartY+32, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
	}

	pDrawImage2(240, StartX-80, StartY+64, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(StartX-80, StartY+64, 120.0, 22.0))
	{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX-80, StartY+64, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
	}

	pDrawImage2(240, StartX+80, StartY+64, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(StartX+80, StartY+64, 120.0, 22.0))
	{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX+80, StartY+64, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
	}

	pDrawImage2(240, StartX-80, StartY+96, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(StartX-80, StartY+96, 120.0, 22.0))
	{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX-80, StartY+96, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
	}

	pDrawImage2(240, StartX+80, StartY+96, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
	if (CheckZone(StartX+80, StartY+96, 120.0, 22.0))
	{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX+80, StartY+96, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
	}
	
	if (CheckZone(StartX +163, StartY - 80, 16.0, 12.0))
	{
		g_StatsAdvance.DrawInfos();
	}
		//pBlendSwitch(1);

	//����� ������� ����
	pDrawTextOut((int)StartX - 30, (int)StartY+7, (char*)"Reset", 0, 0, 0);
	pDrawTextOut((int)StartX - 43, (int)StartY+39, (char*)"Grand Reset", 0, 0, 0);
	pDrawTextOut((int)StartX - 43, (int)StartY+71, (char*)"Jewels Bank", 0, 0, 0);
	pDrawTextOut((int)StartX - 43, (int)StartY+103, (char*)"Rage System", 0, 0, 0);
	//������ ������� ����
	pDrawTextOut((int)StartX + 120, (int)StartY+7, (char*)"Top Players", 0, 0, 0);
	pDrawTextOut((int)StartX + 123, (int)StartY+39, (char*)"3D Camera", 0, 0, 0);
	pDrawTextOut((int)StartX + 107, (int)StartY+71, (char*)"3D Camera Reset", 0, 0, 0);
	pDrawTextOut((int)StartX + 117, (int)StartY+103, (char*)"Attack Helper", 0, 0, 0);

	glColor3f(0.9f, 0.f, 0.f);
	DrawTextOut((int)StartX+47, (int)StartY-78, (char*)"Menu", 0, 0, 0);
	GLSwitch();
	glColor3f(1.f, 1.f, 1.f);
	//((void(*)())(0x602B50))();
}
void MenuSystem::Button(DWORD Event)
{
	DWORD Delays = (GetTickCount() - this->ClickTime);
	float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
	float StartY = 100.0f;

	if(CheckZone(StartX +193, StartY - 80, 16.0, 16.0))// (gInterfaces->IsWorkZone(eBankClose))
	{
		if(Event == WM_LBUTTONDOWN && Delays > 100)
		{
			Sleep(100);
			this->ClickTime = GetTickCount();
			gMenuSystem.Active=false;
		}
	}



}
void MenuSystem::InfoRecv(INFO_UPDATE * aRecv)
{
	this->m_info.Result = aRecv->Result;
	this->m_info.Credits = aRecv->Credits;
	//this->m_info.CSPoints = aRecv->CSPoints;
	this->m_info.Reset = aRecv->Reset;
	this->m_info.GReset = aRecv->GReset;
	this->m_info.VipGroup = aRecv->VipGroup;
	this->m_info.VipTimeTick = aRecv->VipTimeTick;
	this->m_info.QuestNum = aRecv->QuestNum;
	this->m_info.Exps = aRecv->Exps;
}