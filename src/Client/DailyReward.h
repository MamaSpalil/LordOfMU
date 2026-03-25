#pragma once
//#include "Protocol.h"
//#include "Interface.h"
//
//typedef void(*PUSHEVENT_CALLBACK_LPVOID)(LPVOID);
//
//#define cDR_SPLIT_DAY    7
//
//
//#pragma pack(push, 1)
//typedef struct	//-> Complete (size: 107)
//{
//    /*+0*/		short	ItemID;
//    /*+2*/		int		Level;
//    /*+6*/		char	Unknown6;
//    /*+7*/		char	Unknown7;
//    /*+8*/		char	Unknown8;
//    /*+9*/		WORD	DamageMin;
//    /*+11*/		WORD	DamageMax;
//    /*+13*/		char	Unknown13;
//    /*+14*/		WORD	Defense;
//    /*+16*/		WORD	MagicDefense;
//    /*+18*/		char	MagicPower;
//    /*+19*/		char	Unknown19;
//    /*+20*/		short	Unknown20;
//    /*+22*/		char	Durability;
//    /*+23*/		char	ExcellentOption;
//    /*+24*/		char	AncientOption;
//    /*+25*/		short	ReqStrenght;
//    /*+27*/		short	ReqDextirity;
//    /*+29*/		short	ReqEnergy;
//    /*+31*/		short	ReqVitality;
//    /*+33*/		short	ReqCommand;
//    /*+35*/		short	ReqLevel;
//    /*+37*/		char	SpecialCount;
//    /*+38*/		WORD	SpecialType[8];
//    /*+54*/		BYTE	SpecialValue[8];
//    /*+62*/		int		UniqueID;
//    /*+66*/		char	Unknown66;
//    /*+67*/		char	PosX;
//    /*+68*/		char	PosY;
//    /*+69*/		WORD	HarmonyType;
//    /*+71*/		short	HarmonyValue;
//    /*+73*/		char	Is380Item;
//    /*+74*/		char	SocketOption[5];
//    /*+79*/		char	Unknown79;
//    /*+80*/		char	SocketSeedIndex[5];
//    /*+85*/		char	SocketSphereLevel[5];
//    /*+90*/		char	SocketSet;
//    BYTE gap01[5];
//    /*+96*/		char	DurabilityState;
//    /*+97*/		char	PeriodItem;
//    /*+98*/		char	ExpiredItem;
//    /*+99*/		int		ExpireDateConvert;
//    /*+103*/	int		Unknown103;
//} 
//ObjectItem, * lpItemObj;
//#pragma pack(pop)
//
//struct PMSG_DAILY_REWARD_SEND
//{
//    PBMSG_HEAD2 h;
//    BYTE Type;
//    int  Value;
//};
//
//struct PMSG_DAILY_REWARD_USER_INFO_RECV
//{
//    PBMSG_HEAD2 h;
//    BYTE MaxDay;
//    BYTE CurDay;
//    BYTE FWday;
//    BYTE State[31];
//    DWORD Timer;
//};
//
//struct PMSG_DAILY_REWARD_DAY_INFO_RECV
//{
//    PBMSG_HEAD2 h;
//    short Type;
//    BYTE Dur;
//    BYTE Item[12];
//    int  PeriodTime;
//    int Wcoin;
//    int Zen;
//    char Text[100];
//};
//
//class cDailyReward : public cInterfaceBase
//{
//public:
//    cDailyReward();
//    ~cDailyReward();
//    void ToggleWindow(bool mode = false);
//    void OpenWindow(bool mode = false);
//    void CloseWindow(bool mode = false);
//    void SetWindow(bool state, bool mode = false);
//    void Init();
//    void DrawIcon();
//    void Draw();
//    void DrawBase();
//    void DrawMain();
//    void OnOpen();
//    void OnClose();
//    bool AllowedGift();
//    void ClearDayGift();
//    void SendRequest(BYTE type, int value);
//    void GCRecvUserInfo(BYTE* a);
//    void GCRecvDayInfo(BYTE* a);
//    int  CheckItemInfo(signed int a1);
//    static void RecvGiftAction(LPVOID pClass);
//
//    BYTE SelectedDay;
//    BOOL Blink;
//    BOOL Loaded;
//    BOOL Checked;
//    BOOL DayInfo;
//    BYTE MaxDay;
//    BYTE CurDay;
//    BYTE FWday;
//    BYTE State[31];
//    DWORD Timer;
//    ObjectItem Item;
//    short Type;
//    BYTE Level;
//    BYTE Ex;
//    int Wcoin;
//    int Zen;
//    char Text[100];
//    DWORD TickCount;
//
//    bool OpenDaily;
//
//    float MainWidth;
//
//    float MainHeight;
//
//    float MainPosX;
//
//    float MainPosY;
//
//    int SectionWidth;
//
//    float SectionPosY;
//
//    float GeneralPosX;
//
//};
//
//extern cDailyReward DailyReward;
