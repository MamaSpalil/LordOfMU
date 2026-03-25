#include "stdafx.h"
#include "LuckyWheel.h"
#include "Interface.h"
#include "TMemory.h"
#include "Other.h"
#include "InterfaceEx.h"
#include "m_Disable.h"
#include "Structure.h"
#include "SelectChar.h"
#include "define.h"
#include "Protocol.h"
#include "QuickAccess.h"
#include "CustomMenu.h"
CLuckyWheel gLuckyWheel;

PMSG_LUCKYWHEEL_INFO gLuckyWheels[MAX_ITEM_LUCKY];

CLuckyWheel::CLuckyWheel()
{
	this->Init();
}

CLuckyWheel::~CLuckyWheel()
{

}

void CLuckyWheel::RecvInfo(CG_LUCKYWHEEL_RECV* lpMsg)
{
	CoinValue = lpMsg->CoinValue;
}


void CLuckyWheel::Init()
{
	LuckyWheelNumber = -1;

	this->ClearLuckyWheelList();
}

void CLuckyWheel::ClearLuckyWheelList() // OK
{
	for (int n = 0; n < MAX_ITEM_LUCKY; n++)
	{
		gLuckyWheels[n].Index = -1;
		gLuckyWheels[n].ItemType = -1;
		gLuckyWheels[n].ItemIndex = -1;
		gLuckyWheels[n].Level = -1;
		gLuckyWheels[n].Luck = -1;
		gLuckyWheels[n].Skill = -1;
		gLuckyWheels[n].Option = -1;
		gLuckyWheels[n].Exc = -1;

	}
}

void CLuckyWheel::InsertLuckyWheel(PMSG_LUCKYWHEEL_LIST* lpInfo) // OK
{
	for (int n = 0; n < MAX_ITEM_LUCKY; n++)
	{
		if (gLuckyWheels[n].Index == -1)
		{
			gLuckyWheels[n].Index = lpInfo->Index;
			gLuckyWheels[n].ItemType = lpInfo->ItemType;
			gLuckyWheels[n].ItemIndex = lpInfo->ItemIndex;
			gLuckyWheels[n].Level = lpInfo->Level;
			gLuckyWheels[n].Luck = lpInfo->Luck;
			gLuckyWheels[n].Skill = lpInfo->Skill;
			gLuckyWheels[n].Option = lpInfo->Option;
			gLuckyWheels[n].Exc = lpInfo->Exc;
			return;
		}
	}
}

void CLuckyWheel::MainProc()
{
	//gInterfaces->Data[eLuckyWheelMain].OnShow = false;
	this->Open();

	this->Render();
}

void CLuckyWheel::Open()
{
	if (GetKeyState('L') & 0x4000)
	{
		if (GetForegroundWindow() == pGameWindow && !pChatWindow && !pCheckWindow(WINDOWS_FRIEND_LIST) && !pCheckWindow(WINDOWS_STORE) && !pCheckWindow(WINDOWS_GUILD_MASTER))
		{
			if ((GetTickCount() - gInterfaces->Data[eLuckyWheelMain].EventTick) > 300)
			{
				if (gInterfaces->Data[eLuckyWheelMain].OnShow)
				{
					gInterfaces->Data[eLuckyWheelMain].EventTick = GetTickCount();
					gInterfaces->Data[eLuckyWheelMain].OnShow = false;
					SetCursorFocus(1);
				}
				else
				{
					gInterfaces->Data[eLuckyWheelMain].EventTick = GetTickCount();
					gInterfaces->Data[eLuckyWheelMain].OnShow = true;
					SetCursorFocus(0);
				}
			}
		}
	}
}

void CLuckyWheel::Render()
{
	if (gInterfaces->Data[eLuckyWheelMain].OnShow)
	{
		return;
	}

	if (pCheckWindow(WINDOWS_MOVE_LIST) || pCheckWindow(WINDOWS_INVENTORY) || pCheckWindow(WINDOWS_PET) || pCheckWindow(WINDOWS_QUEST) || pCheckWindow(WINDOWS_GUILD) || GetKeyState(VK_ESCAPE) & 0x8000)
	{
		keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
		SetCursorFocus(false);
		gInterfaces->Data[eLuckyWheelMain].OnShow = false;
		return;
	}

	float MainWidth = 230.0;
	float MainHeight = 313.0;
	float StartY = 20;
	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	float ContentX = StartX;
	float ContentY = StartY;


	float Widht = 0.53;
	float Eight = 0.75;

	RenderBitmap(2000, (float)(ContentX)-95, (float)(ContentY + 50), 190.0f, 276.0f, 0.f, 0.f, 0.7421875f, 1.0f, 1.f, 1.f, 0.f);
	RenderBitmap(1999, (float)(ContentX)+95, (float)(ContentY + 50), 190.0f, 276.0f, 0.f, 0.f, 0.7421875f, 1.0f, 1.f, 1.f, 0.f);

	pGLSwitchBlend();
	glColor3f(1.0, 1.0, 1.0);
	pGLSwitch();


	if (this->LuckyWheelNumber >= 0)
	{
		pSetBlend(true);
		glColor4f(0.4, 0.4, 0.4, 0.3);
		switch (LuckyWheelNumber)
		{
		case 0:
			pDrawBarForm2(134, 113, 80, 48, 0, 0);
			break;
		case 1:
			pDrawBarForm2(220, 113, 80, 48, 0, 0);
			break;
		case 2:
			pDrawBarForm2(303, 113, 80, 48, 0, 0);
			break;
		case 3:
			pDrawBarForm2(388, 113, 80, 48, 0, 0);
			break;
		case 4:
			pDrawBarForm2(388, 166, 80, 48, 0, 0);
			break;
		case 5:
			pDrawBarForm2(388, 220, 80, 48, 0, 0);
			break;
		case 6:
			pDrawBarForm2(388, 276, 80, 48, 0, 0);
			break;
		case 7:
			pDrawBarForm2(303, 274, 80, 48, 0, 0);
			break;
		case 8:
			pDrawBarForm2(220, 274, 80, 48, 0, 0);
			break;
		case 9:
			pDrawBarForm2(134, 274, 80, 48, 0, 0);
			break;
		case 10:
			pDrawBarForm2(134, 220, 80, 48, 0, 0);
			break;
		case 11:
			pDrawBarForm2(134, 166, 80, 48, 0, 0);
			break;
		}
		pGLSwitchBlend();
		glColor3f(1.0, 1.0, 1.0);
	}

	if (this->StartRoll >= 1)
	{
		pSetBlend(true);
		glColor4f(0.4, 0.4, 0.4, 0.3);
		int roll = rand() % 12;
		switch (roll)
		{
		case 0:
			pDrawBarForm2(134, 113, 80, 48, 0, 0);
			break;
		case 1:
			pDrawBarForm2(220, 113, 80, 48, 0, 0);
			break;
		case 2:
			pDrawBarForm2(303, 113, 80, 48, 0, 0);
			break;
		case 3:
			pDrawBarForm2(388, 113, 80, 48, 0, 0);
			break;
		case 4:
			pDrawBarForm2(388, 166, 80, 48, 0, 0);
			break;
		case 5:
			pDrawBarForm2(388, 220, 80, 48, 0, 0);
			break;
		case 6:
			pDrawBarForm2(388, 276, 80, 48, 0, 0);
			break;
		case 7:
			pDrawBarForm2(303, 274, 80, 48, 0, 0);
			break;
		case 8:
			pDrawBarForm2(220, 274, 80, 48, 0, 0);
			break;
		case 9:
			pDrawBarForm2(134, 274, 80, 48, 0, 0);
			break;
		case 10:
			pDrawBarForm2(134, 220, 80, 48, 0, 0);
			break;
		case 11:
			pDrawBarForm2(134, 166, 80, 48, 0, 0);
			break;
		}

		pGLSwitchBlend();
		glColor3f(1.0, 1.0, 1.0);
	}

	DrawItemIMG(147, 100, 50, 50, GET_ITEM(gLuckyWheels[0].ItemType, gLuckyWheels[0].ItemIndex), gLuckyWheels[0].Level, gLuckyWheels[0].Exc, 0, 0);
	DrawItemIMG(232, 100, 50, 47, GET_ITEM(gLuckyWheels[1].ItemType, gLuckyWheels[1].ItemIndex), gLuckyWheels[1].Level, gLuckyWheels[1].Exc, 0, 0);
	DrawItemIMG(317, 100, 50, 47, GET_ITEM(gLuckyWheels[2].ItemType, gLuckyWheels[2].ItemIndex), gLuckyWheels[2].Level, gLuckyWheels[2].Exc, 0, 0);
	DrawItemIMG(405, 100, 50, 50, GET_ITEM(gLuckyWheels[3].ItemType, gLuckyWheels[3].ItemIndex), gLuckyWheels[3].Level, gLuckyWheels[3].Exc, 0, 0);
	DrawItemIMG(405, 152, 50, 50, GET_ITEM(gLuckyWheels[4].ItemType, gLuckyWheels[4].ItemIndex), gLuckyWheels[4].Level, gLuckyWheels[4].Exc, 0, 0);
	DrawItemIMG(405, 209, 50, 50, GET_ITEM(gLuckyWheels[5].ItemType, gLuckyWheels[5].ItemIndex), gLuckyWheels[5].Level, gLuckyWheels[5].Exc, 0, 0);
	DrawItemIMG(405, 266, 50, 45, GET_ITEM(gLuckyWheels[6].ItemType, gLuckyWheels[6].ItemIndex), gLuckyWheels[6].Level, gLuckyWheels[6].Exc, 0, 0);
	DrawItemIMG(317, 265, 50, 45, GET_ITEM(gLuckyWheels[7].ItemType, gLuckyWheels[7].ItemIndex), gLuckyWheels[7].Level, gLuckyWheels[7].Exc, 0, 0);
	DrawItemIMG(232, 265, 50, 45, GET_ITEM(gLuckyWheels[8].ItemType, gLuckyWheels[8].ItemIndex), gLuckyWheels[8].Level, gLuckyWheels[8].Exc, 0, 0);
	DrawItemIMG(147, 276, 50, 45, GET_ITEM(gLuckyWheels[9].ItemType, gLuckyWheels[9].ItemIndex), gLuckyWheels[9].Level, gLuckyWheels[9].Exc, 0, 0);
	DrawItemIMG(147, 209, 50, 50, GET_ITEM(gLuckyWheels[10].ItemType, gLuckyWheels[10].ItemIndex), gLuckyWheels[10].Level, gLuckyWheels[10].Exc, 0, 0);
	DrawItemIMG(147, 157, 50, 50, GET_ITEM(gLuckyWheels[11].ItemType, gLuckyWheels[11].ItemIndex), gLuckyWheels[11].Level, gLuckyWheels[11].Exc, 0, 0);

	if (this->LuckyWheelNumber >= 0)
	{
		DrawItemIMG(275, 180, 50, 50, GET_ITEM(gLuckyWheels[this->LuckyWheelNumber].ItemType, gLuckyWheels[this->LuckyWheelNumber].ItemIndex), gLuckyWheels[this->LuckyWheelNumber].Level, gLuckyWheels[this->LuckyWheelNumber].Exc, 0, 0);
	}

	float sizex = 49.0;
	float sizey = 16.0;
	char szTemp10[256];
	sprintf(szTemp10 ,"Price: %d Cred", CoinValue);
	//WindowRenderText(260, 260, pFontBold, 1, 0, "Price: %d", CoinValue);
	DrawTextOut(247, 255, (char*)szTemp10, 0, 0, 0);
	pSetBlend(true);
	//DrawTextOut(330, 255, "Start", 0, 0, 0);
	if (CheckMouseIn(300, 250, 66, sizey))
	{
		//WindowRenderText(330, 260, pFontBold, 1, 0, "Girar");
		DrawTextOut(320, 255, "Spin", 0, 0, 0);
		if (IsRelease)
		{
			if (gMenuSystem.m_info.Credits < CoinValue)
			{
				return;
			}

			this->Send();
			IsRelease = false;
		}
	}
	else
	{
		//WindowRenderText(330, 260, pFontBold, 1, 0, "Girar");
		DrawTextOut(320, 255, "Spin", 0, 0, 0);
	}
}


void CLuckyWheel::Send()
{
	static DWORD StatusTick = 0;

	if ((GetTickCount() - StatusTick) < 400)
	{
		return;
	}

	StatusTick = GetTickCount();

	UP_TUCHAN_REQ pMsg;
	pMsg.h.set(0xFB, 0x16, sizeof(pMsg));
	DataSend((BYTE*)&pMsg, pMsg.h.size);
	StartRoll = 1;
}