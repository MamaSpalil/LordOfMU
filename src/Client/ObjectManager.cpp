#include "StdAfx.h"
#include "ObjectManager.h"
#include "Interface.h"
#include "Other.h"
#include "Visual.h"

//#include "ExInterface.h"
//#include "User.h"
//#include "Fix.h"
DWORD AddExperience = 0;
CObjectManager * gObjectManager = new CObjectManager();
CObjectManager::CObjectManager()
{
	//this->m_CharObj = NULL;
	//this->AttackSpeed = 20;
	//this->MagicSpeed = 20;
	//this->MaxAttackSpeed = (WORD)-1;
	//this->MaxMagicSpeed = (WORD)-1;
}

CObjectManager::~CObjectManager()
{

}

void CObjectManager::SetLife(DWORD Life, DWORD Shield, BYTE Type)
{
	if (Type == 0xFF)
	{
		HPCurrentShow = Life;
		ShieldCurrentShow = Shield;
	}
	else if (Type == 0xFE)
	{
		HPMaxShow = Life;
		ShieldMaxShow = Shield;
	}
	else
	{
		return;
	}

	if (HPCurrentShow < 0)
		HPCurrentShow = 0;

	if (ShieldCurrentShow < 0)
		ShieldCurrentShow = 0;

	if (HPMaxShow < 0)
		HPMaxShow = 0;

	if (ShieldMaxShow < 0)
		ShieldMaxShow = 0;
}
void CObjectManager::SetMana(DWORD Mana, DWORD BP, BYTE Type)
{
	if (Type == 0xFF)
	{
		this->Mana = Mana;
		this->BP = BP;
	}
	else if (Type == 0xFE)
	{
		this->MaxMana = Mana;
		this->MaxBP = BP;
	}
	else
	{
		return;
	}

	if (this->Mana < 0)
		this->Mana = 0;

	if (this->BP < 0)
		this->BP = 0;

	if (this->MaxMana < 0)
		this->MaxMana = 0;

	if (this->MaxBP < 0)
		this->MaxBP = 0;
}
void CObjectManager::SetPosition(BYTE x, BYTE y)
{
	this->m_Object[this->index].x = x;
	this->m_Object[this->index].y = y;
}
void CObjectManager::SetAddPoints(DWORD LifeAndMana, DWORD Shield, DWORD BP, BYTE Type)
{
	if (Type >= 16 && Type <= 20)
	{
		this->LevelUpPoints--;

		Type -= 0x10;

		switch (Type)
		{
		case 2:
			this->MaxLife = LifeAndMana;
			break;

		case 3:
			this->MaxMana = LifeAndMana;
			break;
		}

		this->MaxShield = Shield;
		this->MaxBP = BP;
	}
}

void CObjectManager::SetDamage(DWORD DamageHP, DWORD DamageSD)
{
	this->DamageHP = DamageHP;
	this->DamageSD = DamageSD;
}

void CObjectManager::CheckDamage(int index, DWORD DamageHP, DWORD DamageSD)
{
	if (this->index == index)
	{
		this->Life -= DamageHP;
		this->Shield -= DamageSD;

		if (this->Life < 0)
			this->Life = 0;

		if (this->Shield < 0)
			this->Shield = 0;
	}
}
void CObjectManager::SetExperience(DWORD Experience)
{
	AddExperience = Experience;
	this->Experience = Experience;
}
void CObjectManager::ObjectDie(int index)
{
	if (index < 0 || index > MAX_OBJECTSTRUCT)
	{
		return;
	}

	this->m_Object[index].live = 0;

	if (this->index == index)
	{
		this->Life = 0;
		this->Shield = 0;
	}
}
//
void CObjectManager::ObjectResetRecv(GC_RESET_DATA1 * lpRecv)
{
	this->reset = lpRecv->Reset;
	//this->level = lpRecv->Level;
}

void CObjectManager::ObjectGResetRecv(GC_GRESET_DATA * lpRecv)
{
	this->Greset = lpRecv->GReset;
	//this->level = lpRecv->Level;
}

void CObjectManager::ObjectUpPointsRecv(GC_UpPoints_DATA * lpRecv)
{
	this->LevelUpPoints = lpRecv->LevelUpPoints;
}

//void CObjectManager::ObjectExpRecv(GC_Exp_DATA * lpRecv)
//{
//	EXP = lpRecv->Experiences;
//	this->Experience = lpRecv->Experiences;
//}

void CObjectManager::SetUpPoints(DWORD LevelUpPoints)
{
	//GC_UpPoints_DATA * lpRecv;
	this->LevelUpPoints = LevelUpPoints;// gObjectManager->GetUpPoint();//lpRecv->LevelUpPoints;//Points;
}

//void CObjectManager::SetExp(DWORD Experiences)
//{
//	EXP = Experience;
//	this->Experience = Experiences;
//}
void CObjectManager::ObjectMove(PMSG_MOVE_SEND * lpRecv)
{
	DWORD index = MAKE_NUMBERW(lpRecv->index[0], lpRecv->index[1]);

	if (index < 0 || index > MAX_OBJECTSTRUCT)
	{
		return ;
	}

	this->m_Object[index].x = lpRecv->x;
	this->m_Object[index].y = lpRecv->y;
}