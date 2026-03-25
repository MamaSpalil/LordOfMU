#include "stdafx.h"
#include "Ranking.h"
#include "Interface.h"
#include "Protocol.h"
#include "Control.h"
#include "Other.h"
#include "InterfaceEx.h"
#include "Controller.h"
#include "ResetSystem.h"
#include "GrandReset.h"
#include "JewelsBank.h"
#include "Structure.h"
#include "DungeonSiege.h"

//#include "TestMenu.h"

cRanking gRanking;

cRanking::cRanking()
{
	this->Active = false;
	//this->Show = false;

	this->StartX = 0;//(MAX_WIN_WIDTH / 2)
	this->StartY = 20;//

	this->Page = 1;

	ZeroMemory(&RankingChar,sizeof(RankingChar));
	
}

cRanking::~cRanking()
{

}

void cRanking::ImageLoad()
{

}
void cRanking::RecvGS(DWORD Case, LPBYTE Data, int Len, int aIndex)
{
	//if( !this->Active ) return;

	BYTE ProtocolType = Data[0];

	if( ProtocolType == 0xC2 )
	{
		PWMSG_HEAD2 * lpDef = (PWMSG_HEAD2 *)Data;

		switch(BYTE(Case))
		{
		case 0xFA:
			{
				PWMSG_HEAD2 * lpDef = (PWMSG_HEAD2 *)Data;
				switch(lpDef->subcode)
				{
				case 0x06:
					{
						DGCharTop * Recv = (DGCharTop*)Data;

						ZeroMemory(&RankingChar,sizeof(RankingChar));

						for(int i=0;i<MAXTOP;i++)
						{
							strncpy(this->RankingChar[i].Name,Recv->tp[i].Name,10);

							this->RankingChar[i].Class = Recv->tp[i].Class;
							this->RankingChar[i].level = Recv->tp[i].level;
							this->RankingChar[i].Reset = Recv->tp[i].Reset;
							this->RankingChar[i].Grand = Recv->tp[i].Grand;
							this->RankingChar[i].Str = Recv->tp[i].Str;
							this->RankingChar[i].Agi = Recv->tp[i].Agi;
							this->RankingChar[i].Vit = Recv->tp[i].Vit;
							this->RankingChar[i].Ene = Recv->tp[i].Ene;
							this->RankingChar[i].Cmd = Recv->tp[i].Cmd;
							this->RankingChar[i].Premium = Recv->tp[i].Premium;

							strncpy(this->RankingChar[i].Guild,Recv->tp[i].Guild,8);
						}
					}
					break;
				}
			}
		}
	}
}


char * CharacterCode (int a)
{
	switch(a)
	{
		case 0:  return "Dark Wizard";
		case 1:  return "Soul Master";
		case 2:  return "Grand Master";
		case 3:  return "Grand Master";
		case 16: return "Dark Knight";
		case 17: return "Blade Knight";
		case 18: return "Blade Master";
		case 19: return "Blade Master";
		case 32: return "Fairy Elf";
		case 33: return "Muse Elf";
		case 34: return "High Elf";
		case 35: return "High Elf";
		case 48: return "Magic Gladiator";
		case 50: return "Duel Master";
		case 66: return "Dark Lord";
		case 67: return "Dark Lord";
		case 64: return "Dark Lord";
		case 65: return "Dark Lord";
		case 68: return "Lord Emperial";
	}
	return "unknown";
}

void cRanking::Draw()
{

	////gJewelsBank.Active = false;
	//g_ResetSystem.State = 0;
	////	g_ResetSystem.StateGR = 0;
	//gInterfaces->Data[eRageTable].OnShow = false; 

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
	SetBlend(1);
	glColor4f(0.f, 0.f, 0.f, 0.7f);
	DrawBarForm((float)(StartX + 20), (float)(StartY + 20), 600.f + (float)(0), 380.f);
	GLSwitchBlend();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	gInterfaces->DrawGUI(0x119, eBankClose, 602, StartY + 20);

	int TitleX = StartX + 10 + 20;
	int TitleY = StartY + 50;

	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"(N)", 100, 1, 0);

	TitleX += 40;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Name", 100, 1, 0);

	TitleX += 80;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Class", 100, 1, 0);

	TitleX += 70;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Level", 100, 1, 0);

	TitleX += 30;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Reset", 100, 1, 0);

	TitleX += 30;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Grand", 100, 1, 0);

	TitleX += 45;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Strength", 100, 1, 0);

	TitleX += 45;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Agility", 100, 1, 0);

	TitleX += 45;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Vitality", 100, 1, 0);

	TitleX += 45;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Energy", 100, 1, 0);

	TitleX += 45;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Command", 100, 1, 0);

	TitleX += 45;
	pDrawTextOut((int)TitleX, (int)TitleY, (char*)"Guild", 100, 1, 0);
	
	int InfoY = StartY + 50+15;

	int c = 0;

	for(int i = 0; i < 25; i++)
	{
		int InfoX = StartX + 10 + 20;
		int up = i + ( ( this->Page - 1 ) * 25 );

		if(this->RankingChar[up].Name[0] == NULL) continue;

		DWORD Color;

		if(c == 0 )
		{
			Color = eWhite;
		}
		else
		{
			Color = eShinyGreen;
		}

		char szTemp[256];
		sprintf(szTemp ,"%d",up+1);
		pDrawTextOut(InfoX, InfoY, (char*)szTemp, 100, 1, 0);


		InfoX += 40;
		char NameID[10+1];
		NameID[10] = 0;
		memcpy(NameID, this->RankingChar[up].Name, sizeof(this->RankingChar[up].Name));
		pDrawTextOut((int)InfoX, (int)InfoY, (char*)NameID, 100, 1, 0);

		InfoX += 80;
		pDrawTextOut((int)InfoX, (int)InfoY, (char*)CharacterCode(this->RankingChar[up].Class), 100, 1, 0);
		
		bool bLicenesePremium = false;

		bLicenesePremium = true;

		InfoX += 70;
		char szTemp1[256];
		sprintf(szTemp1 ,"%d",this->RankingChar[up].level);
		pDrawTextOut((int)InfoX, (int)InfoY, (char*)szTemp1, 100, 1, 0);
	
		InfoX += 30;
		char szTemp2[256];
		sprintf(szTemp2 ,"%d",this->RankingChar[up].Reset);
		pDrawTextOut((int)InfoX, (int)InfoY, (char*)szTemp2, 100, 1, 0);

		InfoX += 30;
		char szTemp3[256];
		sprintf(szTemp3 ,"%d",this->RankingChar[up].Grand);
		pDrawTextOut((int)InfoX, (int)InfoY, (char*)szTemp3, 100, 1, 0);

		if(bLicenesePremium == true && this->RankingChar[up].Premium > 0)
		{
			InfoX += 45;
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)"- / -", 100, 1, 0);

			InfoX += 45;
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)"- / -", 100, 1, 0);

			InfoX += 45;
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)"- / -", 100, 1, 0);

			InfoX += 45;
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)"- / -", 100, 1, 0);

			InfoX += 45;
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)"- / -", 100, 1, 0);
		}
		else
		{
			InfoX += 45;
			char szTemp5[256];
			sprintf(szTemp5 ,"%d",this->RankingChar[up].Str);
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)szTemp5, 100, 1, 0);
			
			InfoX += 45;
			char szTemp6[256];
			sprintf(szTemp6 ,"%d",this->RankingChar[up].Agi);
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)szTemp6, 100, 1, 0);
			
			InfoX += 45;
			char szTemp7[256];
			sprintf(szTemp7 ,"%d",this->RankingChar[up].Vit);
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)szTemp7, 100, 1, 0);
			
			InfoX += 45;
			char szTemp8[256];
			sprintf(szTemp8 ,"%d",this->RankingChar[up].Ene);
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)szTemp8, 100, 1, 0);
			
			InfoX += 45;
			char szTemp9[256];
			sprintf(szTemp9 ,"%d",this->RankingChar[up].Cmd);
			pDrawTextOut((int)InfoX, (int)InfoY, (char*)szTemp9, 100, 1, 0);
		}

		InfoX += 45;

		char GuildID[8+1];
		GuildID[8] = 0;
		memcpy(GuildID, this->RankingChar[up].Guild, sizeof(this->RankingChar[up].Guild));
		pDrawTextOut((int)InfoX, (int)InfoY, (char*)GuildID, 100, 1, 0);
		InfoY+=12;
		c++;
		if(c==2)c=0;
	}

	
	DWORD Delay = (GetTickCount() - this->ClickTime);

	int ButX = 180;
	int ButY = 98;

	gInterfaces->DrawGUI(3578, eButtonCharacters_NextPage, StartX+90+ButX, StartY+275+ButY);
	if (gInterfaces->IsWorkZone(eButtonCharacters_NextPage))
	{
		if(gController.wMouse == WM_LBUTTONDOWN && Delay > 200)
		{
			this->ClickTime = GetTickCount();
			if(this->Page != 1) this->Page--;
		}
	}

	char szTemp10[256];
	sprintf(szTemp10 ,"Page: %d/%d",this->Page,4);
	pDrawTextOut(StartX+102+ButX, StartY+277+ButY, (char*)szTemp10, 100, 1, 0);
	


	gInterfaces->DrawGUI(3579, eButtonCharacters_BackPage, StartX+175+ButX, StartY+275+ButY);


	if (gInterfaces->IsWorkZone(eButtonCharacters_BackPage))
	{
		if(gController.wMouse == WM_LBUTTONDOWN && Delay > 200)
		{
			this->ClickTime = GetTickCount();
			if(this->Page != 4) this->Page++;
		}
	}

	glColor3f(0.9f, 0.f, 0.f);
	DrawTextOut((int)TitleX-250, (int)TitleY-20, (char*)"Top Player's", 0, 0, 0);
	GLSwitch();
	glColor3f(1.f, 1.f, 1.f);
	//((void(*)())(0x602B50))();


}

void cRanking::Button(DWORD Event)
{
	DWORD Delays = (GetTickCount() - this->ClickTime);

	if(CheckZone(602, StartY + 20, 16.0, 16.0))//if (gInterfaces->IsWorkZone(eBankClose))
	{
		if(Event == WM_LBUTTONDOWN && Delays > 100)
		{
			Sleep(100);
			this->ClickTime = GetTickCount();
			gRanking.Active = false;
		}
	}



}