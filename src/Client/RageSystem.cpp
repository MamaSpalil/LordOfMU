#include "stdafx.h"
#include "RageSystem.h"
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
#include "DungeonSiege.h"
#include "Structure.h"

RageSystem gRageSystem;


RageSystem::RageSystem()
{
	this->Enable = true;
	this->NextExp = 0;
}

void RageSystem::LoadSkill(int ID, char *cName, int a1, int a2, int NextPoint, bool Percent)
{
	strcpy(this->rSkill[ID].Name,cName);
	this->rSkill[ID].StartDamage = a1;
	this->rSkill[ID].MaxPoint = a2;
	this->rSkill[ID].PointNext = NextPoint;
	this->rSkill[ID].Percent = Percent;
}

void RageSystem::Load()
{
	gInterfaces->BindObject(eRagePointTable, 3850, 100, 100, 460, 70, 0.76, 0.60);//0.76, 0.75);
	gInterfaces->BindObject(eRageIcon1, 3851, 30, 30, 105, 60, 0.62, 0.62);

	gInterfaces->BindObject(eRageIcon2,  3852, 30, 30, 80+25, 120, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon3,  3853, 30, 30, 80+25, 180, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon4,  3854, 30, 30, 80+25, 240, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon5,  3855, 30, 30, 80+25, 300, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon6,  3856, 30, 30, 80+25, 360, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon7,  3857, 30, 30, 465, 300, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon8,  3858, 30, 30, 465, 360, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon9,  3859, 30, 30, 405, 360, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon10, 3860, 30, 30, 140+25, 240, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon11, 3861, 30, 30, 140+25, 300, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon12, 3862, 30, 30, 140+25, 360, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon13, 3863, 30, 30, 200+25, 60, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon14, 3864, 30, 30, 200+25, 120, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon15, 3865, 30, 30, 200+25, 180, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon16, 3866, 30, 30, 200+25, 240, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon17, 3867, 30, 30, 200+25, 300, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon18, 3868, 30, 30, 200+25, 360, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon19, 3869, 30, 30, 440+25, 180, 0.62, 0.62);

	gInterfaces->BindObject(eRageIcon21, 3871, 30, 30, 260+25, 180, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon22, 3872, 30, 30, 260+25, 240, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon23, 3873, 30, 30, 260+25, 300, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon24, 3874, 30, 30, 260+25, 360, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon25, 3875, 30, 30, 320+25, 60, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon26, 3876, 30, 30, 320+25, 120, 0.62, 0.62);

	gInterfaces->BindObject(eRageIcon27, 3877, 30, 30, 320+25, 180, 0.62, 0.62);

	gInterfaces->BindObject(eRageIcon28, 3878, 30, 30, 320+25, 240, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon29, 3879, 30, 30, 320+25, 300, 0.62, 0.62);

	gInterfaces->BindObject(eRageIcon30, 3880, 30, 30, 320+25, 360, 0.62, 0.62);	
	gInterfaces->BindObject(eRageIcon32, 3882, 30, 30, 380+25, 120, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon33, 3883, 30, 30, 525, 180, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon34, 3884, 30, 30, 525, 240, 0.62, 0.62);
	gInterfaces->BindObject(eRageIcon35, 3885, 30, 30, 525, 300, 0.62, 0.62);

	gInterfaces->BindObject(eRageIcon31, 3881, 30, 30, 380+25, 180, 0.62, 0.62);

	gInterfaces->BindObject(eRageIcon20, 3870, 30, 30, 440+25, 240, 0.62, 0.62);
	
	gInterfaces->BindObject(eRageTeleport, 3849, 57, 17, 37, 70,0.89, 0.53);
	gInterfaces->BindObject(eRagePet, 3849, 57, 17, 37, 100,0.89, 0.53);




	//------------

	this->Enable = true;
}

void RageSystem::LoadImage()
{
	LoadImagesTGA("Customs\\Interface\\newui_Figure_kantru.tga", 3850, 0x2601, 0x2900,0, 1);
	LoadImagesTGA("Customs\\Interface\\RageButton.tga", 3849, 0x2601, 0x2900, 0, 1);

	LoadImagesTGA("Customs\\Interface\\icon1.tga", 3851, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon2.tga", 3852, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon3.tga", 3853, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon4.tga", 3854, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon5.tga", 3855, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon6.tga", 3856, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon7.tga", 3857, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon8.tga", 3858, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon9.tga", 3859, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon10.tga", 3860, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon11.tga", 3861, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon12.tga", 3862, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon13.tga", 3863, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon14.tga", 3864, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon15.tga", 3865, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon16.tga", 3866, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon17.tga", 3867, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon18.tga", 3868, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon19.tga", 3869, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon20.tga", 3870, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon21.tga", 3871, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon22.tga", 3872, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon23.tga", 3873, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon24.tga", 3874, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon25.tga", 3875, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon26.tga", 3876, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon27.tga", 3877, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon28.tga", 3878, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon29.tga", 3879, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon30.tga", 3880, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon31.tga", 3881, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon32.tga", 3882, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon33.tga", 3883, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon34.tga", 3884, 0x2601, 0x2900, 0, 1);
	LoadImagesTGA("Customs\\Interface\\icon35.tga", 3885, 0x2601, 0x2900, 0, 1);
}

void RageSystem::rDrawInterface()
{
	////gJewelsBank.Active = false;
	//gRanking.Active = false;
	//g_ResetSystem.State = 0;
	////	g_ResetSystem.StateGR = 0;


	if(!gInterfaces->Data[eRageTable].OnShow)
	{
		//pSetCursorFocus = false;
		return;
	}
	if (pCheckWindow(0x1B)) //inventory
	{
		// keybd_event('V', 0,0,0);
			//keybd_event('V', 0,KEYEVENTF_KEYUP,0);
		//system("PAUSE");
		DrawTextOut(0, 105, "Close, Inventory first (V)",0,0,0);
		return;
		//return;
	}
	int TitleX = StartX + 10 + 20;
	int TitleY = StartY + 50;
	SetBlend(1);
	glColor4f(0.f, 0.f, 0.f, 0.9f);
	DrawBarForm((float)(StartX + 20), (float)(StartY + 20), 600.f + (float)(0), 400.f);
	GLSwitchBlend();
	glColor4f(1.f, 1.f, 1.f, 1.f);


	gInterfaces->DrawGUI(0x119, eBankClose, 602, 20);


	gInterfaces->DrawGUIex(eRagePointTable);

	char szTemp0[256];
	sprintf(szTemp0 ,"%d", this->rUser.rLevel);
	pDrawTextOut(460, 90, "Rage Level:", 100, 1, 0);
	pDrawTextOut(510, 90, (char*)szTemp0, 100, 1, 0);
	//gInterfaces->DrawFormat(eShinyGreen, gInterfaces->Data[eRagePointTable].X+15, gInterfaces->Data[eRagePointTable].Y+11, 100, 1, "Rage Level:");
	//gInterfaces->DrawFormat(eYellow, gInterfaces->Data[eRagePointTable].X+65, gInterfaces->Data[eRagePointTable].Y+11, 100, 1, "%d",this->rUser.rLevel);


	char szTemp1[256];
	sprintf(szTemp1 ,"%d", this->rUser.rPoints);
	pDrawTextOut(460, 117, "Rage Point:", 100, 1, 0);
	pDrawTextOut(510, 117, (char*)szTemp1, 100, 1, 0);
	//gInterfaces->DrawFormat(eShinyGreen, gInterfaces->Data[eRagePointTable].X+15, gInterfaces->Data[eRagePointTable].Y+33, 100, 1, "Rage Point:");
	//gInterfaces->DrawFormat(eYellow, gInterfaces->Data[eRagePointTable].X+65, gInterfaces->Data[eRagePointTable].Y+33, 100, 1, "%d",this->rUser.rPoints);

	pDrawTextOut(460, 144, "Rage Exp:", 100, 1, 0);
	//gInterfaces->DrawFormat(eShinyGreen, gInterfaces->Data[eRagePointTable].X+15, gInterfaces->Data[eRagePointTable].Y+55, 100, 1, "Rage Exp:");
	int ProcExp = 0;
	if(this->NextExp>0)
	{
		ProcExp = (this->rUser.rExperience*100)/this->NextExp;
	}
	char szTemp255[256];
	sprintf(szTemp255 ,"%d%%", ProcExp);
	pDrawTextOut(510, 144, (char*)szTemp255, 100, 1, 0);
	//gInterfaces->DrawFormat(eYellow, gInterfaces->Data[eRagePointTable].X+65, gInterfaces->Data[eRagePointTable].Y+55, 100, 1, "%d%%",ProcExp);

	//if(gInterfaces->IsWorkZoneObj(eRageClose))
	//{
	//	gInterfaces->DrawToolTip(gInterfaces->Data[eRageClose].X + 5, gInterfaces->Data[eRageClose].Y + 25, "Close");
	//	if(mParam == WM_LBUTTONDOWN)
	//	{
	//		gInterfaces->Data[eRageTable].OnShow = false;
	//		return;
	//	}
	//}
	
	this->DrawButtonActive(eRageTeleport,"Teleport",this->rUser.Teleport,eSkill8);
	this->DrawButtonActive(eRagePet,"Summon Pet",this->rUser.SummonPet,eSkill3);
	
	//------------

	this->DrawDirection(gInterfaces->Data[eRageIcon1].X+20, gInterfaces->Data[eRageIcon1].Y+20, 3, 300);
	this->DrawDirection(gInterfaces->Data[eRageIcon4].X+20, gInterfaces->Data[eRageIcon4].Y+20, 50, 3);
	this->DrawDirection(gInterfaces->Data[eRageIcon10].X+20, gInterfaces->Data[eRageIcon10].Y+20, 3, 100);
	this->DrawDirection(gInterfaces->Data[eRageIcon12].X+20, gInterfaces->Data[eRageIcon12].Y+20, 50, 3);

	this->DrawDirection(gInterfaces->Data[eRageIcon33].X+20, gInterfaces->Data[eRageIcon33].Y+20, 3, 100);

	this->DrawDirection(gInterfaces->Data[eRageIcon13].X+20, gInterfaces->Data[eRageIcon13].Y+20, 3, 250);
	this->DrawDirection(gInterfaces->Data[eRageIcon16].X+20, gInterfaces->Data[eRageIcon16].Y+20, 50, 3);
	this->DrawDirection(gInterfaces->Data[eRageIcon22].X+20, gInterfaces->Data[eRageIcon22].Y+20, 3, 100);
	this->DrawDirection(gInterfaces->Data[eRageIcon7].X+20, gInterfaces->Data[eRageIcon7].Y+20, 50, 3);
	this->DrawDirection(gInterfaces->Data[eRageIcon7].X+20, gInterfaces->Data[eRageIcon7].Y+20, 3, 50);

	this->DrawDirection(gInterfaces->Data[eRageIcon25].X+20, gInterfaces->Data[eRageIcon25].Y+20, 3, 300);
	this->DrawDirection(gInterfaces->Data[eRageIcon26].X+20, gInterfaces->Data[eRageIcon26].Y+20, 50, 3);
	this->DrawDirection(gInterfaces->Data[eRageIcon30].X+20, gInterfaces->Data[eRageIcon30].Y+20, 50, 3);
	this->DrawDirection(gInterfaces->Data[eRageIcon21].X+20, gInterfaces->Data[eRageIcon21].Y+20, 200, 3);
	this->DrawDirection(gInterfaces->Data[eRageIcon19].X+20, gInterfaces->Data[eRageIcon19].Y+20, 3, 50);

	//------------
	int Next = -1;

	this->DrawIcon(eRageIcon1,eSkill1,this->rUser.Damage,-1,Next);	//1
	this->DrawIcon(eRageIcon2,eSkill2,this->rUser.FreePoints,this->rUser.Damage,this->rSkill[eSkill1].PointNext);
	this->DrawIcon(eRageIcon3,eSkill3,this->rUser.SummonPet,this->rUser.FreePoints,this->rSkill[eSkill2].PointNext);
	this->DrawIcon(eRageIcon4,eSkill4,this->rUser.WingsDamagePlus,this->rUser.SummonPet,this->rSkill[eSkill3].PointNext);
	this->DrawIcon(eRageIcon5,eSkill5,this->rUser.FinalMana,this->rUser.WingsDamagePlus,this->rSkill[eSkill4].PointNext);
	this->DrawIcon(eRageIcon6,eSkill6,this->rUser.IgnoreDefence,this->rUser.FinalMana,this->rSkill[eSkill5].PointNext);

	this->DrawIcon(eRageIcon7,eSkill7,this->rUser.ResetLevel,this->rUser.CriticalDamage,this->rSkill[eSkill35].PointNext);
	this->DrawIcon(eRageIcon8,eSkill8,this->rUser.Teleport,this->rUser.ResetLevel,this->rSkill[eSkill7].PointNext);
	this->DrawIcon(eRageIcon9,eSkill9,this->rUser.CriticalDamageRate,this->rUser.IceDamage,this->rSkill[eSkill30].PointNext);
	this->DrawIcon(eRageIcon10,eSkill10,this->rUser.AbsordDamage,this->rUser.WingsDamagePlus,this->rSkill[eSkill4].PointNext);	//1
	this->DrawIcon(eRageIcon11,eSkill11,this->rUser.PetDamageBonus,this->rUser.AbsordDamage,this->rSkill[eSkill10].PointNext);
	this->DrawIcon(eRageIcon12,eSkill12,this->rUser.FinalLife,this->rUser.PetDamageBonus,this->rSkill[eSkill11].PointNext);

	this->DrawIcon(eRageIcon13,eSkill13,this->rUser.Strength,-1,Next);	//2
	this->DrawIcon(eRageIcon14,eSkill14,this->rUser.DefenceRate,this->rUser.Strength,this->rSkill[eSkill13].PointNext);
	this->DrawIcon(eRageIcon15,eSkill15,this->rUser.FireDamage,this->rUser.DefenceRate,this->rSkill[eSkill14].PointNext);
	this->DrawIcon(eRageIcon16,eSkill16,this->rUser.Energy,this->rUser.FireDamage,this->rSkill[eSkill15].PointNext);
	this->DrawIcon(eRageIcon17,eSkill17,this->rUser.Agility,this->rUser.Energy,this->rSkill[eSkill16].PointNext);
	this->DrawIcon(eRageIcon18,eSkill18,this->rUser.DoubleDamage,this->rUser.FinalLife,this->rSkill[eSkill12].PointNext);	//1

	this->DrawIcon(eRageIcon19,eSkill19,this->rUser.FinalSD,this->rUser.DistanceAttack,this->rSkill[eSkill31].PointNext);
	this->DrawIcon(eRageIcon20,eSkill20,this->rUser.FinalDefence,this->rUser.FinalSD,this->rSkill[eSkill19].PointNext);
	this->DrawIcon(eRageIcon21,eSkill21,this->rUser.ZenDrop,this->rUser.Experience,this->rSkill[eSkill27].PointNext);
	this->DrawIcon(eRageIcon22,eSkill22,this->rUser.GuildBonus,this->rUser.Energy,this->rSkill[eSkill16].PointNext);	//2
	this->DrawIcon(eRageIcon23,eSkill23,this->rUser.ReflectDamage,this->rUser.GuildBonus,this->rSkill[eSkill22].PointNext);
	this->DrawIcon(eRageIcon24,eSkill24,this->rUser.ExellentDamageRate,this->rUser.ReflectDamage,this->rSkill[eSkill23].PointNext);

	this->DrawIcon(eRageIcon25,eSkill25,this->rUser.PvPBonusPlus,-1,Next);	//3
	this->DrawIcon(eRageIcon26,eSkill26,this->rUser.MinDefence,this->rUser.PvPBonusPlus,this->rSkill[eSkill25].PointNext);
	this->DrawIcon(eRageIcon27,eSkill27,this->rUser.Experience,this->rUser.MinDefence,this->rSkill[eSkill26].PointNext);
	this->DrawIcon(eRageIcon28,eSkill28,this->rUser.StartSD,this->rUser.Experience,this->rSkill[eSkill27].PointNext);
	this->DrawIcon(eRageIcon29,eSkill29,this->rUser.PartyBonus,this->rUser.StartSD,this->rSkill[eSkill28].PointNext);
	this->DrawIcon(eRageIcon30,eSkill30,this->rUser.IceDamage,this->rUser.PartyBonus,this->rSkill[eSkill29].PointNext);

	this->DrawIcon(eRageIcon31,eSkill31,this->rUser.DistanceAttack,this->rUser.Experience,this->rSkill[eSkill27].PointNext);
	this->DrawIcon(eRageIcon32,eSkill32,this->rUser.AttackRate,this->rUser.MinDefence,this->rSkill[eSkill26].PointNext);	//3
	this->DrawIcon(eRageIcon33,eSkill33,this->rUser.WeaponBonus,-1,Next);	//4
	this->DrawIcon(eRageIcon34,eSkill34,this->rUser.ExellentDamage,this->rUser.WeaponBonus,this->rSkill[eSkill33].PointNext);
	this->DrawIcon(eRageIcon35,eSkill35,this->rUser.CriticalDamage,this->rUser.ExellentDamage,this->rSkill[eSkill34].PointNext);

	//------------

	this->rDrawInfoText(eRageIcon1, eSkill1, this->rUser.Damage);
	this->rDrawInfoText(eRageIcon2, eSkill2, this->rUser.FreePoints);
	this->rDrawInfoText(eRageIcon3, eSkill3, this->rUser.SummonPet);
	this->rDrawInfoText(eRageIcon4, eSkill4, this->rUser.WingsDamagePlus);
	this->rDrawInfoText(eRageIcon5, eSkill5, this->rUser.FinalMana);
	this->rDrawInfoText(eRageIcon6, eSkill6, this->rUser.IgnoreDefence);

	this->rDrawInfoText(eRageIcon7, eSkill7, this->rUser.ResetLevel);
	this->rDrawInfoText(eRageIcon8, eSkill8, this->rUser.Teleport);
	this->rDrawInfoText(eRageIcon9, eSkill9, this->rUser.CriticalDamageRate);
	this->rDrawInfoText(eRageIcon10, eSkill10, this->rUser.AbsordDamage);
	this->rDrawInfoText(eRageIcon11, eSkill11, this->rUser.PetDamageBonus);
	this->rDrawInfoText(eRageIcon12, eSkill12, this->rUser.FinalLife);

	this->rDrawInfoText(eRageIcon13, eSkill13, this->rUser.Strength);
	this->rDrawInfoText(eRageIcon14, eSkill14, this->rUser.DefenceRate);
	this->rDrawInfoText(eRageIcon15, eSkill15, this->rUser.FireDamage);
	this->rDrawInfoText(eRageIcon16, eSkill16, this->rUser.Energy);
	this->rDrawInfoText(eRageIcon17, eSkill17, this->rUser.Agility);
	this->rDrawInfoText(eRageIcon18, eSkill18, this->rUser.DoubleDamage);

	this->rDrawInfoText(eRageIcon19, eSkill19, this->rUser.FinalSD);
	this->rDrawInfoText(eRageIcon20, eSkill20, this->rUser.FinalDefence);
	this->rDrawInfoText(eRageIcon21, eSkill21, this->rUser.ZenDrop);
	this->rDrawInfoText(eRageIcon22, eSkill22, this->rUser.GuildBonus);
	this->rDrawInfoText(eRageIcon23, eSkill23, this->rUser.ReflectDamage);
	this->rDrawInfoText(eRageIcon24, eSkill24, this->rUser.ExellentDamageRate);

	this->rDrawInfoText(eRageIcon25, eSkill25, this->rUser.PvPBonusPlus);
	this->rDrawInfoText(eRageIcon26, eSkill26, this->rUser.MinDefence);
	this->rDrawInfoText(eRageIcon27, eSkill27, this->rUser.Experience);
	this->rDrawInfoText(eRageIcon28, eSkill28, this->rUser.StartSD);
	this->rDrawInfoText(eRageIcon29, eSkill29, this->rUser.PartyBonus);
	this->rDrawInfoText(eRageIcon30, eSkill30, this->rUser.IceDamage);

	this->rDrawInfoText(eRageIcon31, eSkill31, this->rUser.DistanceAttack);
	this->rDrawInfoText(eRageIcon32, eSkill32, this->rUser.AttackRate);

	this->rDrawInfoText(eRageIcon33, eSkill33, this->rUser.WeaponBonus);	//4
	this->rDrawInfoText(eRageIcon34, eSkill34, this->rUser.ExellentDamage);
	this->rDrawInfoText(eRageIcon35, eSkill35, this->rUser.CriticalDamage);


	glColor3f(0.9f, 0.f, 0.f);
	DrawTextOut((int)TitleX+250, (int)TitleY-20, (char*)"Rage System", 0, 0, 0);
	GLSwitch();
	glColor3f(1.f, 1.f, 1.f);
	//((void(*)())(0x602B50))();
}

void RageSystem::DrawIcon(int ID1, int SkillID, int &ClickCount, int Tree, int NextShow)
{
	BeginBitmap();
	glColor3f(1.0, 1.0, 1.0);

	DWORD Delay = (GetTickCount() - gInterfaces->Data[ID1].EventTick);

	int PNext = this->rSkill[SkillID].PointNext;

	if(gInterfaces->IsWorkZoneObj(ID1))
	{
		if(gController.wMouse == WM_LBUTTONDOWN && 
			Delay > 100 && 
			this->rUser.rPoints > 0 && 
			this->rSkill[SkillID].MaxPoint > ClickCount && 
			(Tree==-1||Tree>=NextShow))
		{
			gInterfaces->Data[ID1].OnClick = true;
			gInterfaces->Data[ID1].EventTick = GetTickCount();

			this->RageSend(SkillID);
		}
		else
		{
			gInterfaces->Data[ID1].OnClick = false;
		}
			
		gInterfaces->DrawGUIs(ID1, gInterfaces->Data[ID1].X, gInterfaces->Data[ID1].Y);

	}
	else
	{
		if(ClickCount>0 || Tree==-1 || Tree >= NextShow)
		{
			gInterfaces->DrawGUIs(ID1, gInterfaces->Data[ID1].X, gInterfaces->Data[ID1].Y);
		}
		else
		{
			SetBlend(1);
			glColor4f(1.f, 1.f, 1.f, 0.255f);
			gInterfaces->DrawGUIs(ID1, gInterfaces->Data[ID1].X, gInterfaces->Data[ID1].Y);
			GLSwitchBlend();
			glColor4f(1.f, 1.f, 1.f, 1.f);
			//gInterfaces->DrawColoredGUI(ID1, (int)Color4f(100, 100, 100, 255));
		}
	}

//	gInterfaces->DrawFormat(eShinyGreen, gInterfaces->Data[ID1].X+20, gInterfaces->Data[ID1].Y+30, 30, 3, "%d",ClickCount);
	char szTemp2[256];
	sprintf(szTemp2 ,"%d", ClickCount);
	pDrawTextOut(gInterfaces->Data[ID1].X+20, gInterfaces->Data[ID1].Y+30, (char*)szTemp2, 40, 3, 0);
}
void RageSystem::Button(DWORD Event)
{
	DWORD Delays = (GetTickCount() - this->ClickTime);

	if(CheckZone(602, 20, 16.0, 16.0))//if (gInterfaces->IsWorkZone(eBankClose))
	{
		if(Event == WM_LBUTTONDOWN && Delays > 100)
		{
			Sleep(100);
			this->ClickTime = GetTickCount();
			gInterfaces->Data[eRageTable].OnShow  = false;
		}
	}



}
void RageSystem::rDrawInfoText(int ID, int SkillID, int Point)
{
	BeginBitmap();


	if(gInterfaces->IsWorkZoneObj(ID))
	{

		int X = gInterfaces->Data[ID].X;
		int Y = gInterfaces->Data[ID].Y;

		if(X+45+100 > MAX_WIN_WIDTH)
		{
			SetBlend(1);
			glColor4f(0.f, 0.f, 0.f, 1.0f);
			DrawBarForm((float) X-105, (float)Y+7, 120.f + (float)(0), 50.f);
			GLSwitchBlend();
			glColor4f(1.f, 1.f, 1.f, 1.f);
			//MU_DrawColorButton(NEWUI_BAR_SWITCH02, X-105, Y+7, 100, 50, 0, 0, MU_CreateColor(0, 0, 0, 200));
			char szTemp3[256];
			sprintf(szTemp3 ,"%s", this->rSkill[SkillID].Name);
			pDrawTextOut(X-105, Y+10, (char*)szTemp3, 150, 3, 0);

			char szTemp4[1256];
			sprintf(szTemp4 ,"Distributed point: %d/%d", Point,this->rSkill[SkillID].MaxPoint);
			pDrawTextOut(X-105, Y+20, (char*)szTemp4, 150, 3, 0);
			//gInterfaces->DrawFormat(eSocket, X-105, Y+10, 100, 3, this->rSkill[SkillID].Name);
			//gInterfaces->DrawFormat(eYellow, X-105, Y+20, 100, 3, "Distributed point: %d/%d",Point,this->rSkill[SkillID].MaxPoint);
			if(this->rSkill[SkillID].Percent)
			{
				char szTemp5[256];
				sprintf(szTemp5 ,"Increase: %d %%", this->rSkill[SkillID].StartDamage * (Point));
				pDrawTextOut(X-105, Y+30, (char*)szTemp5, 150, 3, 0);
				//gInterfaces->DrawFormat(eAncient, X-105, Y+30, 100, 3, "Increase: %d %%",this->rSkill[SkillID].StartDamage * (Point));
				if(Point >= this->rSkill[SkillID].MaxPoint)
				{
					pDrawTextOut(X-105, Y+40, "Next Increase: Finish", 150, 3, 0);
					//gInterfaces->DrawFormat(eAncient, X-105, Y+40, 100, 3, "Next Increase: Finish");
				}
				else
				{
					char szTemp6[256];
					sprintf(szTemp6 ,"Next Increase: %d %%", this->rSkill[SkillID].StartDamage * (Point+1));
					pDrawTextOut(X-105, Y+40, (char*)szTemp6, 150, 3, 0);
					//gInterfaces->DrawFormat(eAncient, X-105, Y+40, 100, 3, "Next Increase: %d %%",this->rSkill[SkillID].StartDamage * (Point+1));
				}
			}
			else
			{
				char szTemp7[256];
				sprintf(szTemp7 ,"Increase: %d", this->rSkill[SkillID].StartDamage * (Point));
				pDrawTextOut(X-105, Y+30, (char*)szTemp7, 150, 3, 0);
				//gInterfaces->DrawFormat(eAncient, X-105, Y+30, 100, 3, "Increase: %d",this->rSkill[SkillID].StartDamage * (Point));
				if(Point >= this->rSkill[SkillID].MaxPoint)
				{
					//gInterfaces->DrawFormat(eAncient, X-105, Y+40, 100, 3, "Next Increase: Finish");
					pDrawTextOut(X-105, Y+40, "Next Increase: Finish", 150, 3, 0);
				}
				else
				{
					char szTemp8[256];
					sprintf(szTemp8 ,"Next Increase: %d %%", this->rSkill[SkillID].StartDamage * (Point+1));
					pDrawTextOut(X-105, Y+40, (char*)szTemp8, 150, 3, 0);
					//gInterfaces->DrawFormat(eAncient, X-105, Y+40, 100, 3, "Next Increase: %d",this->rSkill[SkillID].StartDamage * (Point+1));
				}
			}
		}
		else
		{
			SetBlend(1);
			glColor4f(0.f, 0.f, 0.f, 1.0f);
			DrawBarForm((float) X+45, (float)Y+7, 120.f + (float)(0), 50.f);
			GLSwitchBlend();
			glColor4f(1.f, 1.f, 1.f, 1.f);
			//MU_DrawColorButton(NEWUI_BAR_SWITCH02, X+45, Y+7, 100, 50, 0, 0, MU_CreateColor(0, 0, 0, 200));
			char szTemp9[256];
			sprintf(szTemp9 ,"%s", this->rSkill[SkillID].Name);
			pDrawTextOut(X+45, Y+10, (char*)szTemp9, 150, 3, 0);

			char szTemp10[1256];
			sprintf(szTemp10 ,"Distributed point: %d/%d", Point,this->rSkill[SkillID].MaxPoint);
			pDrawTextOut(X+45, Y+20, (char*)szTemp10, 150, 3, 0);

			//gInterfaces->DrawFormat(eSocket, X+45, Y+10, 100, 3, this->rSkill[SkillID].Name);
			//gInterfaces->DrawFormat(eYellow, X+45, Y+20, 100, 3, "Distributed point: %d/%d",Point,this->rSkill[SkillID].MaxPoint);

			if(this->rSkill[SkillID].Percent)
			{
				char szTemp11[256];
				sprintf(szTemp11 ,"Increase: %d %%",this->rSkill[SkillID].StartDamage * (Point));
				pDrawTextOut(X+45, Y+30, (char*)szTemp11, 150, 3, 0);
				//gInterfaces->DrawFormat(eAncient, X+45, Y+30, 100, 3, "Increase: %d %%",this->rSkill[SkillID].StartDamage * (Point));
				if(Point >= this->rSkill[SkillID].MaxPoint)
				{
					pDrawTextOut(X+45, Y+40, "Next Increase: Finish", 150, 3, 0);
					//gInterfaces->DrawFormat(eAncient, X+45, Y+40, 100, 3, "Next Increase: Finish");
				}
				else
				{
					char szTemp12[256];
					sprintf(szTemp12 ,"Next Increase: %d %%",this->rSkill[SkillID].StartDamage * (Point+1));
					pDrawTextOut(X+45, Y+40, (char*)szTemp12, 150, 3, 0);
					//gInterfaces->DrawFormat(eAncient, X+45, Y+40, 100, 3, "Next Increase: %d %%",this->rSkill[SkillID].StartDamage * (Point+1));
				}
			}
			else
			{
				char szTemp13[256];
				sprintf(szTemp13,"Increase: %d",this->rSkill[SkillID].StartDamage * (Point));
				pDrawTextOut(X+45, Y+30, (char*)szTemp13, 150, 3, 0);
				//gInterfaces->DrawFormat(eAncient, X+45, Y+30, 100, 3, "Increase: %d",this->rSkill[SkillID].StartDamage * (Point));
				if(Point >= this->rSkill[SkillID].MaxPoint)
				{
					char szTemp14[256];
					sprintf(szTemp14,"Next Increase: Finish",this->rSkill[SkillID].StartDamage * (Point+1));
					pDrawTextOut(X+45, Y+40, (char*)szTemp14, 150, 3, 0);
					//gInterfaces->DrawFormat(eAncient, X+45, Y+40, 100, 3, "Next Increase: Finish",this->rSkill[SkillID].StartDamage * (Point+1));
				}
				else
				{
					char szTemp15[256];
					sprintf(szTemp15,"Next Increase: %d",this->rSkill[SkillID].StartDamage * (Point+1));
					pDrawTextOut(X+45, Y+40, (char*)szTemp15, 150, 3, 0);
					//gInterfaces->DrawFormat(eAncient, X+45, Y+40, 100, 3, "Next Increase: %d",this->rSkill[SkillID].StartDamage * (Point+1));
				}
			}
			
		}
	}
}

void RageSystem::DrawDirection(int X, int Y, int Width, int Height)
{
	//gInterfaces->DrawColorGUI(NEWUI_BAR_SWITCH02, X, Y, Width, Height, eBlack);
}

void RageSystem::FilterSkill(int SkillID)
{
	switch(SkillID)
	{
	case eSkill1:  this->rUser.Damage++;		break;
	case eSkill2:  this->rUser.FreePoints++;		break;
	case eSkill3:  this->rUser.SummonPet++;		break;
	case eSkill4:  this->rUser.WingsDamagePlus++;		break;
	case eSkill5:  this->rUser.FinalMana++;		break;
	case eSkill6:  this->rUser.IgnoreDefence++;		break;
	case eSkill7:  this->rUser.ResetLevel++;		break;
	case eSkill8:  this->rUser.Teleport++;		break;
	case eSkill9:  this->rUser.CriticalDamageRate++;		break;
	case eSkill10: this->rUser.AbsordDamage++;	break;
	case eSkill11: this->rUser.PetDamageBonus++;	break;
	case eSkill12: this->rUser.FinalLife++;	break;
	case eSkill13: this->rUser.Strength++;	break;
	case eSkill14: this->rUser.DefenceRate++;	break;
	case eSkill15: this->rUser.FireDamage++;	break;
	case eSkill16: this->rUser.Energy++;	break;
	case eSkill17: this->rUser.Agility++;	break;
	case eSkill18: this->rUser.DoubleDamage++;	break;
	case eSkill19: this->rUser.FinalSD++;	break;
	case eSkill20: this->rUser.FinalDefence++;	break;
	case eSkill21: this->rUser.ZenDrop++;	break;
	case eSkill22: this->rUser.GuildBonus++;	break;
	case eSkill23: this->rUser.ReflectDamage++;	break;
	case eSkill24: this->rUser.ExellentDamageRate++;	break;
	case eSkill25: this->rUser.PvPBonusPlus++;	break;
	case eSkill26: this->rUser.MinDefence++;	break;
	case eSkill27: this->rUser.Experience++;	break;
	case eSkill28: this->rUser.StartSD++;	break;
	case eSkill29: this->rUser.PartyBonus++;	break;
	case eSkill30: this->rUser.IceDamage++;	break;
	case eSkill31: this->rUser.DistanceAttack++;	break;
	case eSkill32: this->rUser.AttackRate++;	break;
	case eSkill33: this->rUser.WeaponBonus++;	break;
	case eSkill34: this->rUser.ExellentDamage++;	break;
	case eSkill35: this->rUser.CriticalDamage++;	break;
	}

	//ObjectCharacter * pPlayer = &*(ObjectCharacter*)*(DWORD*)0x7BA627C;//0x7BA6278;

	//if(SkillID == eSkill13)
	//{
	//	pPlayer->Strength += this->rSkill[eSkill13].StartDamage;
	//}
	//else if(SkillID == eSkill16)
	//{
	//	pPlayer->Energy += this->rSkill[eSkill16].StartDamage;
	//}
	//else if(SkillID == eSkill17)
	//{
	//	pPlayer->Dexterity += this->rSkill[eSkill17].StartDamage;
	//}
	//else if(SkillID == eSkill2)
	//{
	//	pPlayer->LevelPoint += this->rSkill[eSkill2].StartDamage;
	//	UP_POINT += this->rSkill[eSkill2].StartDamage;
	//}
}

void RageSystem::RageSend(short Skill)
{
	ClickSend rSend;
	rSend.h.set((LPBYTE)&rSend, 0xFB, 1, sizeof(rSend));
	rSend.Click = 1;
	rSend.SkillID = Skill;
	DataSend((LPBYTE)&rSend, rSend.h.size);
}

void RageSystem::RageRecv(ClickSend *Recv)
{
	this->rUser.rPoints--;
	this->FilterSkill(Recv->SkillID);
}

void RageSystem::RecvStart(sRageUserSend * Recv)
{
	//Console.ConsoleOutput(GREY,"rLevel: %d",Recv->rLevel);
	this->rUser.rLevel = Recv->rLevel;
	this->rUser.rPoints = Recv->rPoints;
	this->rUser.rExperience = Recv->rExperience;

	this->rUser.Damage = Recv->Damage;
	this->rUser.FreePoints = Recv->FreePoints;
	this->rUser.SummonPet = Recv->SummonPet;
	this->rUser.WingsDamagePlus = Recv->WingsDamagePlus;
	this->rUser.FinalMana = Recv->FinalMana;
	this->rUser.IgnoreDefence = Recv->IgnoreDefence;
	this->rUser.ResetLevel = Recv->ResetLevel;
	this->rUser.Teleport = Recv->Teleport;
	this->rUser.CriticalDamageRate = Recv->CriticalDamageRate;
	this->rUser.AbsordDamage = Recv->AbsordDamage;
	this->rUser.PetDamageBonus = Recv->PetDamageBonus;
	this->rUser.FinalLife = Recv->FinalLife;
	this->rUser.Strength = Recv->Strength;
	this->rUser.DefenceRate = Recv->DefenceRate;
	this->rUser.FireDamage = Recv->FireDamage;
	this->rUser.Energy = Recv->Energy;
	this->rUser.Agility = Recv->Agility;
	this->rUser.DoubleDamage = Recv->DoubleDamage;
	this->rUser.FinalSD = Recv->FinalSD;
	this->rUser.FinalDefence = Recv->FinalDefence;
	this->rUser.ZenDrop = Recv->ZenDrop;
	this->rUser.GuildBonus = Recv->GuildBonus;
	this->rUser.ReflectDamage = Recv->ReflectDamage;
	this->rUser.ExellentDamageRate = Recv->ExellentDamageRate;
	this->rUser.PvPBonusPlus = Recv->PvPBonusPlus;
	this->rUser.MinDefence = Recv->MinDefence;
	this->rUser.Experience = Recv->Experience;
	this->rUser.StartSD = Recv->StartSD;
	this->rUser.PartyBonus = Recv->PartyBonus;
	this->rUser.IceDamage = Recv->IceDamage;
	this->rUser.DistanceAttack = Recv->DistanceAttack;
	this->rUser.AttackRate = Recv->AttackRate;
	this->rUser.WeaponBonus = Recv->WeaponBonus;
	this->rUser.ExellentDamage = Recv->ExellentDamage;
	this->rUser.CriticalDamage = Recv->CriticalDamage;

	this->NextExp = Recv->NextExp;

	//ObjectCharacter * pPlayer = &*(ObjectCharacter*)*(DWORD*)0x7BA627C;
	//pPlayer->Strength += this->rUser.Strength * rSkill[eSkill13].StartDamage;
	//pPlayer->Dexterity += this->rUser.Agility * rSkill[eSkill17].StartDamage;
	//pPlayer->Energy += this->rUser.Energy * rSkill[eSkill16].StartDamage;
}

void RageSystem::RecvExp(RageExpSend * Recv)
{
	this->rUser.rLevel = Recv->Level;
	this->rUser.rPoints = Recv->Point;
	this->rUser.rExperience = Recv->Exp;
	this->NextExp = Recv->NextExp;
}

void RageSystem::DrawButtonActive(int ID, char * Text, int Poin, int SkID)
{
	gInterfaces->DrawGUIex(ID);
	//gInterfaces->DrawGUIs(ID, gInterfaces->Data[ID].X, gInterfaces->Data[ID].Y);

	DWORD Delay = (GetTickCount() - gInterfaces->Data[ID].EventTick);

	if(gInterfaces->IsWorkZoneObj(ID))
	{
		if(gController.wMouse == WM_LBUTTONDOWN && Delay > 200 && Poin > 0)
		{
			gInterfaces->Data[ID].OnClick = true;
			gInterfaces->Data[ID].EventTick = GetTickCount();

			this->RageTabSend(SkID);
		}
		else
		{
			gInterfaces->Data[ID].OnClick = false;
		}

		if(Poin>0)
		{
			char szTemp25[256];
			sprintf(szTemp25,"%s",Text);
			pDrawTextOut(gInterfaces->Data[ID].X+5, gInterfaces->Data[ID].Y+5, (char*)szTemp25, 0, 0, 0);
			//gInterfaces->DrawFormat(eYellow, gInterfaces->Data[ID].X+5, gInterfaces->Data[ID].Y+3, 50, 3, Text);
		}
		else
		{
			char szTemp26[256];
			sprintf(szTemp26,"%s",Text);
			pDrawTextOut(gInterfaces->Data[ID].X+5, gInterfaces->Data[ID].Y+5, (char*)szTemp26, 0, 0, 0);
			//gInterfaces->DrawFormat(eWhite, gInterfaces->Data[ID].X+5, gInterfaces->Data[ID].Y+3, 50, 3, Text);
		}
		
	}
	else
	{
		char szTemp27[256];
		sprintf(szTemp27,"%s",Text);
		pDrawTextOut(gInterfaces->Data[ID].X+5, gInterfaces->Data[ID].Y+5, (char*)szTemp27, 0, 0, 0);
		//gInterfaces->DrawFormat(eWhite, gInterfaces->Data[ID].X+5, gInterfaces->Data[ID].Y+3, 50, 3, Text);
	}		
}

void RageSystem::RageTabSend(short index)
{
	sRageTabSend rSend;
	rSend.h.set((LPBYTE)&rSend, 0xFB, 4, sizeof(rSend));
	rSend.Num = index;
	DataSend((LPBYTE)&rSend, rSend.h.size);
}

void RageSystem::RecvConfig(sConfigStruct * Recv)
{
	if(Recv->SkillID > R_MAX_SKILL) return;
	this->LoadSkill(Recv->SkillID,Recv->Name,Recv->Damage,	Recv->MaxPoint,	Recv->PointNext, Recv->Percent);
}