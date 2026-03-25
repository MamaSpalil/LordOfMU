#include "stdafx.h"
#include "Interface.h"
#include "User.h"
#include "JewelsBank.h"
#include "Other.h"
#include "Ranking.h"
#include "ResetSystem.h"
#include "InterfaceEx.h"

#if(_JEWELS_BANK_)

JewelsBank gJewelsBank;

void JewelsBank::JewelsBankImageLoad()
{

}

void JewelsBank::JewelsBankLoad()
{
	this->Active = false;
	gRanking.Active = false;
	g_ResetSystem.Active = false;
	g_ResetSystem.State = false;
	g_ResetSystem.StateGR = false;
	gRanking.Active = 0;
	/*gInterfaces->Data[eRageTable].OnShow = false; */

	gInterfaces->BindObjects(eJewelsBank, 3549, 85, 100, 1, 23);
	//gInterface.BindObject(eJewelsBank, 2549, 85, 100, 1, 23);
	gInterfaces->Data[eJewelsBank].OnShow = true;
}

void JewelsBank::JewelsBankDraw()
{
	//if( gInterface.CheckWindow(eObjWindow::CashShop) || gInterface.CheckWindow(eObjWindow::MoveList))
	//{
	//	return;
	//}

	float StartX = 0.0f;
		float StartY = 0.0f;

		
	if(this->Active == false) return;
	pDrawImage2(3549, StartX, StartY+300, 84.0, 88.0, 0, 0, 0.66, 0.67, 1, 1, 0.0);
	//gInterface.DrawGUI(2549, eJewelsBank, 0, 350);
	//gInterface.DrawGUIs(eJewelsBank);
	char szTemp[256];
	char szTemp1[256];
	char szTemp2[256];
	char szTemp3[256];
	char szTemp4[256];
	pDrawTextOut((int)StartX +10, (int)StartY+303, (char*)"Jewel's Bank", 82, 1, 0);

	//pDrawTextOut((int)StartX+8, (int)StartY+315, (char*)"CSPoints:", 60, 0, 0);
	//sprintf(szTemp ,"%d",gObjUser.ExCred);
	//pDrawTextOut((int)StartX +45, (int)StartY+315, (char*)szTemp, 60, 1, 0);

	pDrawTextOut((int)StartX +8, (int)StartY+315, (char*)"Chaos:", 0, 0, 0);
	sprintf(szTemp1 ,"%d",gObjUser.ChaosBank);
	pDrawTextOut((int)StartX + 60, (int)StartY+315, (char*)szTemp1, 0, 0, 0);

	pDrawTextOut((int)StartX +8, (int)StartY+325, (char*)"Bless:", 0, 0, 0);
	sprintf(szTemp2 ,"%d",gObjUser.BlessBank);
	pDrawTextOut((int)StartX + 60, (int)StartY+325, (char*)szTemp2, 0, 0, 0);

	pDrawTextOut((int)StartX +8, (int)StartY+335, (char*)"Soul:", 0, 0, 0);
	sprintf(szTemp3 ,"%d",gObjUser.SoulBank);
	pDrawTextOut((int)StartX + 60, (int)StartY+335, (char*)szTemp3, 0, 0, 0);

	pDrawTextOut((int)StartX +8, (int)StartY+345, (char*)"Life:", 0, 0, 0);
	sprintf(szTemp4 ,"%d",gObjUser.LifeBank);
	pDrawTextOut((int)StartX + 60, (int)StartY+345, (char*)szTemp4, 0, 0, 0);

	pDrawTextOut((int)StartX +8, (int)StartY+355, (char*)"Creation:", 0, 0, 0);
	sprintf(szTemp4 ,"%d",gObjUser.CreationBank);
	pDrawTextOut((int)StartX + 60, (int)StartY+355, (char*)szTemp4, 0, 0, 0);

	pDrawTextOut((int)StartX +8, (int)StartY+365, (char*)"Guardian:", 0, 0, 0);
	sprintf(szTemp4 ,"%d",gObjUser.GuardianBank);
	pDrawTextOut((int)StartX + 60, (int)StartY+365, (char*)szTemp4, 0, 0, 0);

	pDrawTextOut((int)StartX +8, (int)StartY+375, (char*)"Harmony:", 0, 0, 0);
	sprintf(szTemp4 ,"%d",gObjUser.HarmonyBank);
	pDrawTextOut((int)StartX + 60, (int)StartY+375, (char*)szTemp4, 0, 0, 0);

	//gInterface.DrawFormat(eShinyGreen, 10, 25+10+10, 60, 1, "Premium Hour:");
	//gInterface.DrawFormat(eShinyGreen, 10, 25+20+10, 60, 1, "Jewel Of Chaos:");
	//gInterface.DrawFormat(eShinyGreen, 10, 25+30+10, 60, 1, "Jewel Of Bless:");
	//gInterface.DrawFormat(eShinyGreen, 10, 25+40+10, 60, 1, "Jewel Of Soul:");
	//gInterface.DrawFormat(eShinyGreen, 10, 25+50+10, 60, 1, "Jewel Of Life:");
	//gInterface.DrawFormat(eShinyGreen, 10, 25+60+10, 60, 1, "Credits:");


	//gInterface.DrawFormat(eWhite, 68, 25+10+10, 40, 1, "%d",(gObjUser.Premium/3600));
	//gInterface.DrawFormat(eWhite, 68, 25+20+10, 40, 1, "%d",gObjUser.ChaosBank);
	//gInterface.DrawFormat(eWhite, 68, 25+30+10, 40, 1, "%d",gObjUser.BlessBank);
	//gInterface.DrawFormat(eWhite, 68, 25+40+10, 40, 1, "%d",gObjUser.SoulBank);
	//gInterface.DrawFormat(eWhite, 68, 25+50+10, 40, 1, "%d",gObjUser.LifeBank);
	//gInterface.DrawFormat(eWhite, 40, 25+60+10, 40, 1, "%.2f",gObjUser.ExCred);

}

void JewelsBank::JewelsBankRecv(EXINFO_UPDATE * aRecv)
{
	//gObjUser.Premium = aRecv->PremiumTime;
	gObjUser.ChaosBank = aRecv->Chaos;
	gObjUser.BlessBank = aRecv->Bless;
	gObjUser.SoulBank = aRecv->Soul;
	gObjUser.LifeBank = aRecv->Life;
	////gObjUser.ExCred = aRecv->ExCred;
	gObjUser.CreationBank = aRecv->Creation;
	gObjUser.GuardianBank = aRecv->Guardian;
	gObjUser.HarmonyBank = aRecv->Harmony;


}

#endif