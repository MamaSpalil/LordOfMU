#pragma once
// ----------------------------------------------------------------------------------------------

#include "Structure.h"
#include "Protocol.h"
// ----------------------------------------------------------------------------------------------

#define MAX_BATTLE_LIST	11
// ----------------------------------------------------------------------------------------------
void LoadUser();
void LoadFunction();
int AddEffect2(int ItemID, float a2, int a3);
struct PMSG_BATTLE_LIST
{
	PBMSG_HEAD	h;
	BYTE		BattleMapList[MAX_BATTLE_LIST];
	BYTE		BattleZoneList[MAX_BATTLE_LIST];
};
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct PMSG_TARGETDATA_REQ
{
	PBMSG_HEAD2 h;
	int			aIndex;
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct PMSG_TARGETDATA_ANS
{
	PBMSG_HEAD2 h;
#ifdef __NOVUS__
	float		TargetLife;
	float		TargetMaxLife;
	int			TargetLevel;
	int			TargetReset;
#endif
	float		TargetLifePercent;
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------

struct MUHELPER_ANS_DATA
{
	PBMSG_HEAD2 h;	
	WORD	Time;
	DWORD	Money;
	BYTE	Mode;
};
// ----------------------------------------------------------------------------------------------

struct OFFLINEATTACK_REQ
{
	PBMSG_HEAD2 h;
	int			MagicNumber;
};
// ----------------------------------------------------------------------------------------------
struct CHANGEWAREHOUSE
{
	PBMSG_HEAD2 h;
	int Number;
};
#pragma pack(push, 1)
struct CHAR_UPDATEINFO
{
	PBMSG_HEAD h;
	int points;
//	PBMSG_HEAD2 h;
//	WORD	Strength;
//	WORD	Dexterity;
//	WORD	Vitality;
//	WORD	Energy;
//	WORD	Leadership;
//	int		LevelUpPoint;
//	WORD Resets;
//	SHORT Level;
//	/*+16*/		DWORD	Experience;
///*+20*/		DWORD	NextExperience;
//int ExCred;
//	//PBMSG_HEAD2 h;
//	//unsigned short	Strength;
//	//unsigned short	Dexterity;
//	//unsigned short	Vitality;
//	//unsigned short	Energy;
//	//unsigned short	Leadership;
//	//int		LevelUpPoint;
//	//SHORT Level;
//	///*+16*/		DWORD	Experience;
//	///*+20*/		DWORD	NextExperience;
//	//int ExCred;
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------
struct OBJECTSTRUCT
{
	BYTE level;
	BYTE HPRate;
	BYTE SDRate;
	BYTE live;
	BYTE x;
	BYTE y;
};
struct ExViewePort
{
	PBMSG_HEAD2 h;
	short aIndex;
	int Life;
	int MaxLife;
	int SD;
	int MaxSD;
	int Level;
	int Reset;
	int GrandReset;
	int Defence;
	char Name[20];
	BYTE Live;
};
class User
{
public:
	void		Load();
	void		Refresh();
	void WarehouseReq(CHANGEWAREHOUSE * aRecv);
	// ----
	int			VieweTargetID;
	bool		BattleCursor;


	ExViewePort	ExViewe[40];
	//bool		GetTarget();
	//int			GetActiveSkill();
	// ----
	static void	SetEventEffect(int PreviewStruct);
	static bool	IsMasterClass(BYTE Class);
	static void	DrawPartyHP();
	void		DrawSome();
	// ----
	//lpCharObj	lpPlayer;
	lpViewObj	lpViewPlayer;
	lpViewObj	lpViewTarget;
	// ----
	BYTE		m_TargetType;	//1 - npc, 2 - attack
	int			m_MapNumber;
	int			m_CursorX;
	int			m_CursorY;

	//__NOVUS__
	DWORD		m_TargetUpdateTick;
	float		m_TargetLife;
	float		m_TargetMaxLife;
	float		m_TargetLifePercent;
	int			m_TargetLevel;
	int			m_TargetReset;

	// ----
	void		SetTargetData(PMSG_TARGETDATA_ANS * aRecv);
	bool CheckDistance(int aIndex, int bIndex, int distance);
	BYTE CalcDistance(int aIndex, int bIndex);
	DWORD GetIndex();
	// ----
	OBJECTSTRUCT GetObject(DWORD index);
		LPSTR GetCharacterName();
		void RefreshUserStruct();
			bool IsUpdate;
	WORD Level;
	//DWORD LevelUpPoint;
WORD LevelUpPoint;
	BYTE Class;
	DWORD Experience;
	DWORD NextExperience;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Leadership;
	WORD AttackSpeed;
	WORD MagicSpeed;
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	WORD BP;
	WORD MaxBP;
	int RealLevelUpPoint;
	bool InEvent;
	int Premium;

	int ChaosBank;
	int BlessBank;
	int SoulBank;
	int LifeBank;
	int GuardianBank;
	int CreationBank;
	int HarmonyBank;
	float ExCred;
	int number;
#if(DEV_DAMAGE_TABLE)
	bool m_SecondInfo;
	int m_SecondDamageMax;
	int m_SecondDamage;
	int m_SecondDefence;
	int m_SecondReflect;
#endif
	bool m_bGuildBank;
	int WarehouseNumber;
	//int Credits;
	//int CSPoints;
	//int Reset;
	//int GReset;
	//int VipGroup;
	//int VipTimeTick;

private:
//	lpCharObj m_CharObj;
	DWORD index;
	OBJECTSTRUCT m_Object[10000];
	char Name[11];





}; extern User gObjUser;
enum eSkillType
{
	SKILL_POISON = 0x1,
	SKILL_METEO = 0x2,
	SKILL_THUNDER = 0x3,
	SKILL_FIREBALL = 0x4,
	SKILL_FLAME = 0x5,
	SKILL_TELEPORT = 0x6,
	SKILL_SLOW = 0x7,
	SKILL_STORM = 0x8,
	SKILL_EVIL = 0x9,
	SKILL_HELL = 0xa,
	SKILL_POWERWAVE = 0xb,
	SKILL_FLASH = 0xc,
	SKILL_BLAST = 0xd,
	SKILL_INFERNO = 0xe,
	SKILL_TARGET_TELEPORT = 0xf,
	SKILL_MAGICDEFENSE = 0x10,
	SKILL_ENERGYBALL = 0x11,
	SKILL_BLOCKING = 0x12,
	SKILL_SWORD1 = 0x13,
	SKILL_SWORD2 = 0x14,
	SKILL_SWORD3 = 0x15,
	SKILL_SWORD4 = 0x16,
	SKILL_SWORD5 = 0x17,
	SKILL_CROSSBOW = 0x18,
	SKILL_BOW = 0x19,
	SKILL_HEALING = 0x1a,
	SKILL_DEFENSE = 0x1b,
	SKILL_ATTACK = 0x1c,
	SKILL_CALLMON1 = 0x1e,
	SKILL_CALLMON2 = 0x1f,
	SKILL_CALLMON3 = 0x20,
	SKILL_CALLMON4 = 0x21,
	SKILL_CALLMON5 = 0x22,
	SKILL_CALLMON6 = 0x23,
	SKILL_CALLMON7 = 0x24,
	SKILL_WHEEL = 0x29,
	SKILL_BLOWOFFURY = 0x2a,
	SKILL_STRIKE = 0x2b,
	SKILL_KNIGHTSPEAR = 0x2f,
	SKILL_KNIGHTADDLIFE = 0x30,
	SKILL_KNIGHTDINORANT = 0x31,
	SKILL_ELFHARDEN = 0x33,
	SKILL_PENETRATION = 0x34,
	SKILL_DEFENSEDOWN = 0x37,
	SKILL_SWORD6 = 0x38,
	SKILL_5CROSSBOW = 0x36,
	SKILL_EXPPOISON = 0x26,
	SKILL_EXPICE = 0x27,
	SKILL_EXPHELL = 0x28,
	SKILL_EXPHELL_START = 0x3a,
	SKILL_IMPROVE_AG_REFILL = 0x35,
	SKILL_DEVILFIRE = 0x32,
	SKILL_COMBO = 0x3b,
	SKILL_SPEAR = 0x3c,
	SKILL_FIREBURST = 0x3d,
	SKILL_DARKHORSE_ATTACK = 0x3e,
	SKILL_RECALL_PARTY = 0x3f,
	SKILL_ADD_CRITICALDAMAGE = 0x40,
	SKILL_ELECTRICSPARK = 0x41,
	SKILL_LONGSPEAR = 0x42,
	SKILL_RUSH = 0x2c,
	SKILL_JAVALIN = 0x2d,
	SKILL_DEEPIMPACT = 0x2e,
	SKILL_ONE_FLASH = 0x39,
	SKILL_DEATH_CANNON = 0x49,
	SKILL_SPACE_SPLIT = 0x4a,
	SKILL_BRAND_OF_SKILL = 0x4b,
	SKILL_STUN = 0x43,
	SKILL_REMOVAL_STUN = 0x44,
	SKILL_ADD_MANA = 0x45,
	SKILL_INVISIBLE = 0x46,
	SKILL_REMOVAL_INVISIBLE = 0x47,
	SKILL_REMOVAL_MAGIC = 0x48,
	SKILL_FENRIR_ATTACK = 0x4c,
	SKILL_INFINITY_ARROW = 0x4d,
	SKILL_FIRESCREAM = 0x4e,
	SKILL_EXPLOSION = 0x4f,
	SKILL_SUMMON = 0xc8,
	SKILL_IMMUNE_TO_MAGIC = 0xc9,
	SKILL_IMMUNE_TO_HARM = 0xca,
};
// ----------------------------------------------------------------------------------------------