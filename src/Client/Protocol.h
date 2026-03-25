#pragma once
//---------------------------------------------------------------------------

#define iDataRecvCustom		0x00509F6D
#define iDataRecvOriginal	0x0050A070
#define oXORFilterStart			0x004039E9

#define pActiveSocket			*(SOCKET*)(*(DWORD*)0x7D3B70 + 0x0C)

//SEND-RECV

// ----
#define iFrameSpeed			0x007B3A4C
//---------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------

#define SET_NUMBERH(x)			( (BYTE)((DWORD)(x)>>(DWORD)8) )
#define SET_NUMBERL(x)			( (BYTE)((DWORD)(x) & 0xFF) )
#define SET_NUMBERHW(x)			( (WORD)((DWORD)(x)>>(DWORD)16) )
#define SET_NUMBERLW(x)			( (WORD)((DWORD)(x) & 0xFFFF) )
#define SET_NUMBERHDW(x)		( (DWORD)((unsigned __int64)(x)>>(DWORD)32) )
#define SET_NUMBERLDW(x)		( (DWORD)((unsigned __int64)(x) & 0xFFFFFFFF) )
#define MAKE_NUMBERW(x,y)		( (WORD)(((BYTE)((y)&0xFF)) |   ((BYTE)((x)&0xFF)<<8 ))  )
#define MAKE_NUMBERQW(x,y)		( (unsigned __int64)(((DWORD)((y)&0xFFFFFFFF)) | ((DWORD)((x)&0xFFFFFFFF)<<32))  )
#define MAKE_NUMBERDW(x,y)		( (DWORD)(((WORD)((y)&0xFFFF)) | ((WORD)((x)&0xFFFF)<<16))  )
#define MAKEQWORD(a, b)			((__int64)(((DWORD)((__int64)(a) & 0xffffffff)) | ((__int64)((DWORD)((__int64)(b) & 0xffffffff))) << 32))  
#define LODWORD(h)				((DWORD)(__int64(h) & __int64(0xffffffff)))   
#define HIDWORD(h)				((DWORD)(__int64(h) >> __int64(32)))
#define DataRecv ( ( void(*) (DWORD protoNum, BYTE * aRecv, int aLen, int aIndex) ) iDataRecvOriginal)
#define GET_MAX_WORD_VALUE(x) (((x)>65000)?65000:((WORD)(x)))
#define GET_MAX_DWORD_VALUE(x) (((x)>4294967295)?4294967295:((DWORD)(x)))

//---------------------------------------------------------------------------

#pragma pack(push, 1)
struct PBMSG_HEAD
{
public:
	void set ( LPBYTE lpBuf, BYTE head, BYTE size)
	{
		lpBuf[0] = 0xC1;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};
	void setE ( LPBYTE lpBuf, BYTE head, BYTE size)
	{
		lpBuf[0] = 0xC3;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};
	BYTE c;
	BYTE size;
	BYTE headcode;
};
#pragma pack(pop)
struct PWMSG_HEAD2
{
public:
	void set(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
	{
		lpBuf[0] = 0xC2;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
		lpBuf[4] = sub;
	}
	BYTE c;
	BYTE sizeH;
	BYTE sizeL;
	BYTE headcode;
	BYTE subcode;
};
//---------------------------------------------------------------------------

//struct PBMSG_HEAD2
//{
//	BYTE c;
//	BYTE size;
//	BYTE headcode;
//	BYTE subcode;
//};
struct PBMSG_HEAD2
{
public:
	void set( LPBYTE lpBuf, BYTE head, BYTE sub, BYTE size)
	{
		lpBuf[0] = 0xC1;
		lpBuf[1] = size;
		lpBuf[2] = head;
		lpBuf[3] = sub;
	};
	void setB(LPBYTE lpBuf, BYTE head, int size)
	{
		lpBuf[0] = 0xC1;		// Packets
		lpBuf[1] = size;
		lpBuf[2] = head;
	}

	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
};
struct PSWMSG_HEAD
{
	void set(BYTE head,BYTE subh,WORD size) // OK
	{
		this->type=0xC2;
		this->size[0]=HIBYTE(size);
		this->size[1]=LOBYTE(size);
		this->head=head;
		this->subh=subh;
	}

	void setE(BYTE head,BYTE subh,WORD size) // OK
	{
		this->type=0xC4;
		this->size[0]=HIBYTE(size);
		this->size[1]=LOBYTE(size);
		this->head=head;
		this->subh=subh;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
	BYTE subh;
};
 struct PBMSG_CLOCK
{
	PBMSG_HEAD h; 
	DWORD Hour;
	DWORD Minute;
	DWORD Second;

};
//---------------------------------------------------------------------------

struct PMSG_UPDATECHARACTER
{
	PBMSG_HEAD h;
	BYTE Code;
	DWORD Experience;
	DWORD NextExperience;
	WORD Level;
	int LevelUpPoint;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Leadership;
	int Life;
	int MaxLife;
	int Mana;
	int MaxMana;
	int Shield;
	int MaxShield;
	int BP;
	int MaxBP;
	DWORD ResetCount;
};

struct PMSG_LEVELUP
{
	PBMSG_HEAD h;
	BYTE subcode;
	WORD Level;
	WORD LevelUpPoint;
	WORD MaxLife;
	WORD MaxMana;
	WORD wMaxShield;
	WORD MaxBP;
	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Leadership;
};
struct PMSG_LVPOINTADD
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE ResultType;
	WORD MaxLifeAndMana;
	WORD wMaxShield;
	WORD MaxBP;
};
struct PMSG_CHARINFOMAIN
{
	PBMSG_HEAD h;
	BYTE Code;
	WORD Class;
	WORD Dexterity;
	WORD AttackSpeed;
	WORD MagicSpeed;
	int LevelUpPoint;
	DWORD ResetCount;
};
struct PMSG_UPDATEPOINTS
{
	PBMSG_HEAD h;
	BYTE Code;
	int LevelUpPoint;
	int ResetCount;
};
//---------------------------------------------------------------------------

struct PMSG_CUSTOM_INFO // C1:F9:01
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	int CtrCode;
	char Name[11];
	int Exp;
	int NextExp;
};

struct PMSG_CLIENT_ACCEPT // C1:F9:01
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BOOL Accept;
};
struct PMSG_CHATPOSTDATA
{
	PBMSG_HEAD h;	//	
	char chatid[10];	//	3
	char chatmsg[60];	//	D
};
//---------------------------------------------------------------------------
struct PMSG_ARCHER_INFO_LEFT_RENAS // C1:F9:01
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	WORD LeftRenas;

};
//---------------------------------------------------------------------------
struct PMSG_SHOP_PRICE_LIST_RECV
{
	PSWMSG_HEAD header;// C1:F3:E7
	BYTE count;
	BYTE type;
};
struct PMSG_SHOP_PRICE_RECV
{
	DWORD ItemIndex;
	DWORD ItemLevel;
	DWORD ItemDur;
	DWORD ItemNewOption;
	DWORD ItemPrice;
};
//---------------------------------------------------------------------------
struct PMSG_ARCHER_INFO_STAT_RENAS // C1:F9:02
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	WORD Result;
	WORD NeedRenas;
};
//---------------------------------------------------------------------------
struct PMSG_SEND_CHARACTER_INFO
{
	PBMSG_HEAD2 h;
	DWORD life;
	DWORD MaxLife;
	DWORD Mana;
	DWORD MaxMana;
	DWORD SD;
	DWORD MaxSD;
	DWORD Stanina;
	DWORD MaxStamina;
};
#pragma pack(push, 1)
struct CONNECTEX_LOGIN
{
	PBMSG_HEAD2	h;
	int         SecretCode;
	char		AccountID[11];
	char		Password[11];
	char		Name[11];
	DWORD		TickCount;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct BANK_OPEN_NUM
{
	PBMSG_HEAD2	h;
	int         BankNum;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct CREATE_BANK
{
	PBMSG_HEAD2	h;
	int         BankNum;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct PMSG_WAREHOUSE_PRICE
{
	PBMSG_HEAD h;	// C1:
	DWORD PRICE;	// 4
};
#pragma pack(pop)
#pragma pack(push, 1)
struct BANK_START_INFO
{
	PBMSG_HEAD2	h;
	DWORD         Zen;
	int TotalWarehouseHave;
};
#pragma pack(pop)

struct SDHP_GETNUMPAGE_MULTICHAR
{
	PBMSG_HEAD h;	// C1:01
	int Page;
};

#if(CUSTOM_VALUE_MENU)

struct PMSG_REQ_VALUE_MENU
{
	PBMSG_HEAD2 h;
	int ValueOpen;
};

struct PMSG_ANS_VALUE_MENU
{
	PBMSG_HEAD2 h;
	int ValueOpen;
	int ValueNumber;
};

#endif

struct PSBMSG_HEAD
{
	void set(BYTE head, BYTE subh, BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head, BYTE subh, BYTE size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size;
	BYTE head;
	BYTE subh;
};
struct PMSG_TPLIST
{
	PSBMSG_HEAD h;
	int Result;
};
#pragma pack(1)
struct PMSG_IDPASS
{
 PBMSG_HEAD h;
 BYTE subcode; // 3
 char Id[10]; // 4
 char Pass[20]; //  -> in new same as E 
 DWORD TickCount; // 18 -> in new is 0x22
 BYTE CliVersion[5]; // 1C
 BYTE CliSerial[16]; // 21 
 //char IpAddress[17];	// 1A
};
#pragma pack()
struct SDHP_RECONNECT_IDPASS
{
	PBMSG_HEAD h;	// 
	int CheckCode;
	char Id[11];	//
	char Pass[11];	//
	char Name[11];  //
};

#pragma pack(push, 1)
struct TEST_PACKET
{
	PBMSG_HEAD	h;
	int 		TestCode;
};
#pragma pack(pop)

struct SDHP_IDPASS
{
	PBMSG_HEAD h;	// C1:F1:01	[11::LOGIN-TEST]
	BYTE subcode; // 3
	char Id[10];	// 3
	char Pass[10];	// D
	short Number;	// 18
	char IpAddress[17];	// 1A
};
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct PMSG_ANS_SERVER_INFO
{
	PBMSG_HEAD2 h;
	char		Ip[16];
	WORD		Port;
};
#pragma pack(pop)
struct PMSG_MONSTERHP_VIEWPORT_NOTIFY
{

  PBMSG_HEAD h;	// C1:26
  BYTE        NumberH; // 3
  BYTE        NumberL; // 4
  float Life;
  float MaxLife;
};

struct PMSG_PROPENSITY_VIEWPORT_NOTIFY
{

  PBMSG_HEAD h;	// C1:26
  BYTE        NumberH; // 3
  BYTE        NumberL; // 4
  float	Number;

};
//---------------------------------------------------------------------------
//struct PMSG_ATTACKRESULT
//{
//    PBMSG_HEAD    h; // C1:DC
//    BYTE        NumberH; // 3
//    BYTE        NumberL; // 4
//    BYTE        DamageH; // 5
//    BYTE        DamageL; // 6
//    BYTE        DamageType; // 7
//    BYTE        btShieldDamageH; // 8
//    BYTE        btShieldDamageL; // 9
//    float        Life; //A
//    float        MaxLife; //E
//};
#pragma pack(push, 1)
struct PMSG_DEFAULT2
{
	PBMSG_HEAD h;
	BYTE subcode;
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct PWMSG_HEAD
{
public:
	void set( LPBYTE lpBuf, BYTE head, int size)
	{
		lpBuf[0] = 0xC2;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
	};
	void setE( LPBYTE lpBuf, BYTE head, int size)
	{
		lpBuf[0] = 0xC4;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
	};

	BYTE c;
	BYTE sizeH;
	BYTE sizeL;
	BYTE headcode;
};
#pragma pack(pop)

// ----------------------------------------------------------------------------------------------
struct PMSG_HEALTHBAR
{
	PWMSG_HEAD h;
	BYTE Code;
	BYTE Count;
};

struct PMSG_HEALTHBAR_INFO
{
	short Index;
	BYTE Rate;
};
//struct PMSG_ATTACKRESULT
//{
//	PBMSG_HEAD h;	// C1:DC
//	BYTE NumberH;	// 3
//	BYTE NumberL;	// 4
//	BYTE DamageH;	// 5
//	BYTE DamageL;	// 6
//	BYTE DamageType;	// 7
//	BYTE btShieldDamageH;	// 8
//	BYTE btShieldDamageL;	// 9
//	DWORD Life; // A
//	DWORD MaxLife; // E
//	DWORD AttackDamage;	//20
//	DWORD iShieldDamage;//24
//	DWORD SD;
//	DWORD MaxSD;
//};
struct PMSG_ATTACKRESULT_PRIMARY
{
	PBMSG_HEAD h;	// C1:DC
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE DamageH;	// 5
	BYTE DamageL;	// 6
	BYTE DamageType;	// 7
	BYTE btShieldDamageH;	// 8
	BYTE btShieldDamageL;	// 9
};

struct UP_TUCHAN_REQ
{
	PSBMSG_HEAD h;
};

struct ITEM_WIN_SEND
{
	PSBMSG_HEAD header;
	int	number;
};

struct PMSG_LUCKYWHEEL_LIST
{
	int Index;
	int ItemType;
	int ItemIndex;
	int Level;
	int Luck;
	int Skill;
	int Option;
	int Exc;
};

struct PMSG_LUCKYWGEEL_LIST__RECV
{
	PSWMSG_HEAD header; // C1:F3:E6
	BYTE count;
};
struct PMSG_DIEPLAYER
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE Skill;
	BYTE KillerNumberH;
	BYTE KillerNumberL;
};

struct PMSG_REFILL
{
	PBMSG_HEAD h;
	BYTE IPos;
	BYTE LifeH;
	BYTE LifeL;
	BYTE Flag;
	BYTE btShieldH;
	BYTE btShieldL;
	int Life;
	int Shield;
};

struct PMSG_MANASEND
{
	PBMSG_HEAD h;
	BYTE IPos;
	BYTE ManaH;
	BYTE ManaL;
	BYTE BPH;
	BYTE BPL;
	int Mana;
	int BP;
};

struct PMSG_JOINRESULT
{
	PBMSG_HEAD h;
	BYTE scode;
	BYTE result;
	BYTE NumberH;
	BYTE NumberL;
	BYTE CliVersion[5];
};
struct PMSG_MONSTER_DAMAGE_RECV
{
	PBMSG_HEAD h; // C1:F3:07
	BYTE damage[2];
	// Extra
	DWORD ViewCurHP;
	DWORD ViewDamageHP;
};
struct PMSG_MOVE_SEND
{
	PBMSG_HEAD h;
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE dir;
};
struct PMSG_CHARMAPJOINRESULT
{
	PBMSG_HEAD h;	// C3:F3:03
	BYTE subcode;	// 3
	BYTE Class;
	BYTE MapX;	// 4
	BYTE MapY;	// 5
	BYTE MapNumber;	// 6
	BYTE Dir;	// 7
	DWORD Exp;	// 8
	DWORD NextExp;	// C
	DWORD LevelUpPoint;	// 10
	WORD Str;	// 12
	WORD Dex;	// 14
	WORD Vit;	// 16
	WORD Energy;	// 18
	DWORD Life;	// 1A
	DWORD MaxLife;	// 1C
	DWORD Mana;	// 1E
	DWORD MaxMana;	// 20
	DWORD wShield;	// 22
	DWORD wMaxShield;	// 24
	DWORD BP;	// 26
	DWORD MaxBP;	// 28
	int Money;	// 2C
	BYTE PkLevel;	// 30
	BYTE CtlCode;	// 31
	short AddPoint;	// 32
	short MaxAddPoint;	// 34
	WORD Leadership;	// 36
	WORD wMinusPoint;	// 38
	WORD wMaxMinusPoint;	// 3A
};
struct PMSG_CHARMAPJOINRESULT_PRIMARY
{
	PBMSG_HEAD h;	// C3:F3:03
	BYTE subcode;	// 3
	BYTE MapX;	// 4
	BYTE MapY;	// 5
	BYTE MapNumber;	// 6
	BYTE Dir;	// 7
	DWORD Exp;	// 8
	DWORD NextExp;	// C
	WORD LevelUpPoint;	// 10
	WORD Str;	// 12
	WORD Dex;	// 14
	WORD Vit;	// 16
	WORD Energy;	// 18
	WORD Life;	// 1A
	WORD MaxLife;	// 1C
	WORD Mana;	// 1E
	WORD MaxMana;	// 20
	WORD wShield;	// 22
	WORD wMaxShield;	// 24
	WORD BP;	// 26
	WORD MaxBP;	// 28
	int Money;	// 2C
	BYTE PkLevel;	// 30
	BYTE CtlCode;	// 31
	short AddPoint;	// 32
	short MaxAddPoint;	// 34
	WORD Leadership;	// 36
	WORD wMinusPoint;	// 38
	WORD wMaxMinusPoint;	// 3A
};
//---------------------------------------------------------------------------

enum ChatColor
{
	Yellow  	= 0x00,
	Blue		= 0x01,
	Red			= 0x02,
	White   	= 0x03, //WHITE?
	Unknown4   	= 0x04, //
	Unknown5   	= 0x05, //
	Unknown6   	= 0x06, //
	Lightyellow = 0x09,
	Chocolate	= 0x0A,
	Gold		= 0x09,
	Green		= 0x0B,
};
//---------------------------------------------------------------------------
extern DWORD EXP;
//extern int ResetCount;
extern DWORD Experience;
extern int IsGameMaster;
void GCGetPostMessage(PMSG_CHATPOSTDATA * gObj);
void GCGetServerTime(PBMSG_CLOCK * gObj);
void GCGetCustomInfo(PMSG_CUSTOM_INFO * gObj);
void GCGetRenasLeftInfo(PMSG_ARCHER_INFO_LEFT_RENAS * gObj);
void GCGetRenasStatInfo(PMSG_ARCHER_INFO_STAT_RENAS * gObj);
void GCShopPriceListRecv(PMSG_SHOP_PRICE_LIST_RECV* lpMsg);
void GCGetVpMonsterInfo(PMSG_MONSTERHP_VIEWPORT_NOTIFY * pMsg);
void GCGetVpPropensitySystem(PMSG_PROPENSITY_VIEWPORT_NOTIFY * pMsg);
void BankNumberOpen(int BankNum);
void cProtocolLoad();
void FixDMG();
void SpeedFix();
extern DWORD *GameIndex;
//void cDataRecv(DWORD protoNum, BYTE *aRecv, int aLen, int aIndex);
#if(CUSTOM_VALUE_MENU)
void GCRecvValueMenu(PMSG_REQ_VALUE_MENU* lpMsg);
void CGSendValueMenu(int ValueType, int ValueNum);
#endif
void UpdateLuckyWheel(ITEM_WIN_SEND* lpMsg);
void GCLuckyWheelRecv(PMSG_LUCKYWGEEL_LIST__RECV* lpMsg);
void Encode32(BYTE *buf);
void GetLoginInfo(PMSG_IDPASS * obj_acc);


void BuxConvert(char* buf, int size);
void CreateBank(int BankHave);
void PageCharacterSet(int PageNum);

void DataSend(LPBYTE Data, int Size);

//struct PMSG_KILLPLAYER
//{
//	PBMSG_HEAD h;
//	BYTE NumberH;
//	BYTE NumberL;
//	BYTE ExpH;
//	BYTE ExpL;
//	BYTE DamageH;
//	BYTE DamageL;
//	DWORD Exp;
//
//};

struct PMSG_KILLPLAYER_PRIMARY
{
	PBMSG_HEAD h;	// C1:16
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE ExpH;	// 5
	BYTE ExpL;	// 6
	BYTE DamageH;	// 7
	BYTE DamageL;	// 8
};

struct PMSG_KILLPLAYER
{
	PBMSG_HEAD h;	// C1:16
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	DWORD Experience;
	DWORD DamageHP;
};
void KillExpRecv(PMSG_KILLPLAYER * lpRecv, int key, int Protonum);
void UserDieRecv(PMSG_DIEPLAYER * lpRecv);
struct PMSG_ATTACKRESULT
{
	//PBMSG_HEAD h;	// C1:DC
	//BYTE NumberH;	// 3
	//BYTE NumberL;	// 4
	//BYTE DamageH;	// 5
	//BYTE DamageL;	// 6
	//BYTE DamageType;	// 7
	//BYTE btShieldDamageH;	// 8
	//BYTE btShieldDamageL;	// 9
	//DWORD Life; // A
	//DWORD MaxLife; // E
	//DWORD AttackDamage;	//20
	//DWORD iShieldDamage;//24
	//DWORD SD;
	//DWORD MaxSD;
	PBMSG_HEAD h;	// C1:DC
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	DWORD DamageHP;
	BYTE DamageType;	// 7
	DWORD DamageSD;
};

struct PMSG_DAMAGE_PRIMARY
{
	PBMSG_HEAD h;	// C1:F3:07
	BYTE subcode;	// 3
	BYTE DamageH;	// 4
	BYTE DamageL;	// 5
	BYTE btShieldDamageH;	// 6
	BYTE btShieldDamageL;	// 7
};

struct PMSG_DAMAGE
{
	PBMSG_HEAD h;	// C1:F3:07
	BYTE subcode;	// 3
	DWORD DamageHP;	// 4
	DWORD DamageSD;	// 5
	DWORD Life;	// 6
	DWORD Shield;	// 7
};
struct PMSG_KILLPLAYER_EXT
{
	PBMSG_HEAD h;	// C3:9C
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	WORD ExpH;	// 6
	WORD ExpL;	// 8
	BYTE DamageH;	// A
	BYTE DamageL;	// B
	//DWORD ExDamage;
};
struct PMSG_SERVER_TIME
{
	PBMSG_HEAD2 h;
    WORD Hour;
    WORD Minute;
    WORD Second;
};
struct ServerTime
{
    WORD m_Hour;
    WORD m_Minute;
    WORD m_Second;
};
struct PMSG_DURATION_MAGIC_SEND
{
	struct PBMSG_HEAD h;
	BYTE MagicNumber;
	//BYTE MagicNumberL;
	BYTE NumberH;
	BYTE NumberL;
	BYTE X;
	BYTE Y;
	BYTE Dir;
};
extern DWORD MAGIC;
extern ServerTime g_ServerTime;
#if(DEV_DAMAGE_TABLE)
struct PMSG_DAMAGE_TABLE
{
	PBMSG_HEAD2 h;
	int SecondDamage;
	int SecondDefence;
	int SecondReflect;
};
#endif
#if(DEV_DAMAGE_TABLE)
	void RecvDamageTable(PMSG_DAMAGE_TABLE* lpMsg);
#endif
void GCServerTime(PMSG_SERVER_TIME* lpMsg);
void RecvExp(PMSG_KILLPLAYER * Data);
void RecvMasterExp(PMSG_KILLPLAYER_EXT * Data);
void AttackRecv(PMSG_ATTACKRESULT*lpRecv, int key);
void CharacterJoinMapResultRecv(PMSG_CHARMAPJOINRESULT * lpRecv, int key);
void DamageRecv(PMSG_DAMAGE*lpRecv, int key);
void ClientAccept();
class cProtocol
{
public:
	static void cDataRecv(DWORD protoNum, BYTE * aRecv, int aLen, int aIndex);
	static void UpdateCharacterNoEffect();
	static void UpdateCharacterEffect();
	static void LevelUpPoints();
	void Agility();
	void AutoLogin();
	void Load();

public:
	bool Update;
	WORD Level;
	WORD LevelUpPoint;
	WORD Class;
	int RealLevelUpPoint;
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
	WORD Shield;
	WORD MaxShield;
	WORD BP;
	WORD MaxBP;
	int ResetCount;
};

extern cProtocol Protocol;
//---------------------------------------------------------------------------