#include "stdafx.h"
#include "Other.h"
#include "Protocol.h"
#include "User.h"

extern DWORD AddExperience;
#define MAX_OBJECTSTRUCT		10000
struct GC_RESET_DATA1
{
	PBMSG_HEAD2	h;
	BYTE Result;
	WORD Reset;
	//short Level;
	//WORD MaxReset;
};
struct GC_GRESET_DATA
{
	PBMSG_HEAD2	h;
	BYTE Result;
	WORD GReset;
	//short Level;
	//WORD MaxReset;
};
struct GC_UpPoints_DATA
{
	PBMSG_HEAD2	h;
	BYTE Result;
	int LevelUpPoints;
	//short Level;
	//WORD MaxReset;
};
struct GC_Exp_DATA
{
	PBMSG_HEAD2	h;
	BYTE Result;
	//int Exp;
	int Experiences;
	//short Level;
	//WORD MaxReset;
};
//struct OBJECTSTRUCT
//{
//	BYTE level;
//	BYTE HPRate;
//	BYTE SDRate;
//	BYTE live;
//	BYTE x;
//	BYTE y;
//};
class CObjectManager
{
public:
	CObjectManager();
	~CObjectManager();
	void Init();

	void RefreshUserStruct();
	LPSTR GetCharacterName();

	void SetIndex(DWORD index);
	DWORD GetIndex();

	
	void SetLife(DWORD Life, DWORD Shield, BYTE Type);
	void SetMana(DWORD Mana, DWORD BP, BYTE Type);
	void SetAddPoints(DWORD LifeAndMana, DWORD Shield, DWORD BP, BYTE Type);
	void SetPosition(BYTE x, BYTE y);
	void SetDamage(DWORD DamageHP, DWORD DamageSD);
	void SetExperience(DWORD Experience);
	void SetCharacterSpeed(WORD AttackSpeed, WORD MagicSpeed, WORD MaxAttackSpeed, WORD MaxMagicSpeed);
	void CheckDamage(int index, DWORD DamageHP, DWORD DamageSD);
	float GetFillHP();
	float GetFillMP();
	float GetFillSD();
	float GetFillBP();
	DWORD GetLife() { return this->Life; };
	DWORD GetMaxLife() { return this->MaxLife; };
	DWORD GetMana() { return this->Mana; };
	DWORD GetMaxMana() { return this->MaxMana; };
	DWORD GetShield() { return this->Shield; };
	DWORD GetMaxShield() { return this->MaxShield; };
	DWORD GetBP() { return this->BP; };
	DWORD GetMaxBP() { return this->MaxBP; };
	//DWORD GetUpPoint() { return this->LevelUpPoints; };
	DWORD GetDamageHP() { return this->DamageHP; };
	DWORD GetDamageSD() { return this->DamageSD; };
	DWORD GetExperience() { return this->Experience; };
	WORD GetAttackSpeed() { return this->AttackSpeed; }
	WORD GetMagicSpeed() { return this->MagicSpeed; }
	WORD GetMaxAttackSpeed() { return this->MaxAttackSpeed; }
	WORD GetMaxMagicSpeed() { return this->MaxMagicSpeed; }
	static void CustomHPBar();

	

	OBJECTSTRUCT GetObject(DWORD index);
	void ObjectMove(PMSG_MOVE_SEND * lpRecv);
	//void ObjectLifeAddRecv(SGPM_OBJECT_LIFEBAR_ADD * lpRecv);
	//void ObjectLifeRecv(SGPM_OBJECT_LIFEBAR * lpRecv);
	void ObjectResetRecv(GC_RESET_DATA1 * lpRecv);
	void SetReset(int reset) { this->reset = reset; };
	DWORD GetResets() { return this->reset; };

	void ObjectGResetRecv(GC_GRESET_DATA * lpRecv);
	void SetGReset(int Greset) { this->Greset = Greset; };
	DWORD GetGResets() { return this->Greset; };

	void ObjectUpPointsRecv(GC_UpPoints_DATA * lpRecv);
	//void SetUpPoints(int LevelUpPoints) {this->LevelUpPoints  = LevelUpPoints; };
	DWORD GetUpPoint() { return this->LevelUpPoints; };
	void SetUpPoints(DWORD LevelUpPoints);

	void ObjectExpRecv(GC_Exp_DATA * lpRecv);
	//DWORD GetExp() { return this->Experience; };
	//void SetExp(DWORD Experience);
	//void SetExp(int Experience) { this->Experience = Experience; };


	void SetLevel(int level) { this->level = level; };
	DWORD GetLevel() { return this->level; };
	BYTE CalcDistance(int aIndex, int bIndex);
	bool CheckDistance(int aIndex, int bIndex, int distance);
	void ObjectDie(int index);


	
private:
	//lpCharObj m_CharObj;
	OBJECTSTRUCT m_Object[MAX_OBJECTSTRUCT];
	char Name[11];
	DWORD index;
	DWORD level;
	DWORD reset;
	DWORD Greset;
	int LevelUpPoints;
	//DWORD LevelUpPoints;
	DWORD Life;
	DWORD Mana;
	DWORD Shield;
	DWORD BP;
	DWORD MaxLife;
	DWORD MaxMana;
	DWORD MaxShield;
	DWORD MaxBP;

	DWORD DamageHP;
	DWORD DamageSD;

	/*int Experiences;*/
	DWORD Experience;

	WORD AttackSpeed;
	WORD MagicSpeed;
	WORD MaxAttackSpeed;
	WORD MaxMagicSpeed;
};

extern CObjectManager * gObjectManager;