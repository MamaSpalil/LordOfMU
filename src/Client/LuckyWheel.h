#pragma once

#include "Protocol.h"
#include "Interface.h"

#define MAX_ITEM_LUCKY                12 

struct LUCKYWHEEL_INFO
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


struct PMSG_LUCKYWHEEL_INFO
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

struct CG_LUCKYWHEEL_RECV
{
	PSBMSG_HEAD header;
	int CoinValue;
};

class CLuckyWheel
{
public:
	CLuckyWheel();
	virtual ~CLuckyWheel();
	void	Init();
	void	ClearLuckyWheelList();
	void	InsertLuckyWheel(PMSG_LUCKYWHEEL_LIST* lpInfo);
	void	MainProc();
	void	Open();
	void	Render();
	void	RecvInfo(CG_LUCKYWHEEL_RECV* lpMsg);
	void	Send();
public:
	LUCKYWHEEL_INFO m_LuckyWheelInfo[MAX_ITEM_LUCKY];
	int		LuckyWheelNumber;
	DWORD	LuckyWheelTime;
	int		StartRoll;
	DWORD	EventTick;
	int		CoinValue;
};

extern CLuckyWheel gLuckyWheel;
