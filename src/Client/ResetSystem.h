#pragma once

#include "stdafx.h"
#include "Protocol.h"

#define RESET_STATE_Stop				0
#define RESET_STATE_SelectMode		1

struct PMSG_RESET_RESULT
{
	PBMSG_HEAD2 h;
	BYTE Result;
};

struct GC_RESET_DATAEx
{
	PBMSG_HEAD2	h;
	BYTE Result;
	WORD Reset;
	WORD MaxReset;
	WORD NeedLevel;
	WORD NeedQuestN;
	DWORD NeedMoney;
	DWORD RewardPoint;
	WORD AddCredits;
	WORD AddWcoinC;
	WORD AddWcoinP;
	WORD AddWcoinG;
	DWORD ExFreePoint;

	bool AddEnableItem;
	int AddItem;
	BYTE AddItemLevel;
	bool AddItemSkill;
	bool AddItemLuck;
	bool AddItemOpt;
	bool AddItemExl;
	bool AddItemAnc;
	BYTE AddItemCount;

	bool RewardEnableItem;
	int RewardItem;
	int RewardLevel;
	bool RewardSkill;
	bool RewardLuck;
	bool RewardOpt;
	bool RewardExl;
	bool RewardAnc;
};

struct RESETSYSTEM_DATA
{
	BYTE Result;
	WORD Reset;
	WORD MaxReset;
	WORD NeedLevel;
	WORD NeedQuestN;
	DWORD NeedMoney;
	DWORD RewardPoint;
	WORD AddCredits;
	WORD AddWcoinC;
	WORD AddWcoinP;
	WORD AddWcoinG;
	DWORD ExFreePoint;

	bool AddEnableItem;
	int AddItem;
	BYTE AddItemLevel;
	bool AddItemSkill;
	bool AddItemLuck;
	bool AddItemOpt;
	bool AddItemExl;
	bool AddItemAnc;
	BYTE AddItemCount;

	bool RewardEnableItem;
	int RewardItem;
	int RewardLevel;
	bool RewardSkill;
	bool RewardLuck;
	bool RewardOpt;
	bool RewardExl;
	bool RewardAnc;
};

struct GC_RESET_RESULT
{
	PBMSG_HEAD2	h;
};
struct PMSG_GRAND_DIALOG
{
	PBMSG_HEAD2	h;
	BYTE Result;
	WORD GrandNum;
	WORD GrandMax;
	WORD NeedReset;
	WORD NeedLevel;
	DWORD NeedMoney;
	DWORD RewardPoint;
	DWORD RewardWCoin;
	DWORD RewardCredit;
	DWORD ExFreePoint;

	bool AddEnableItem;
	int AddItem;
	BYTE AddItemLevel;
	bool AddItemSkill;
	bool AddItemLuck;
	bool AddItemOpt;
	bool AddItemExl;
	bool AddItemAnc;
	BYTE AddItemCount;

	bool RewardEnableItem;
	int RewardItem;
	int RewardLevel;
	bool RewardSkill;
	bool RewardLuck;
	bool RewardOpt;
	bool RewardExl;
	bool RewardAnc;
};

struct GRANDSYSTEM_DATA
{
	BYTE Result;
	WORD GrandNum;
	WORD GrandMax;
	WORD NeedReset;
	WORD NeedLevel;
	DWORD NeedMoney;
	DWORD RewardPoint;
	DWORD RewardWCoin;
	DWORD RewardCredit;
	DWORD ExFreePoint;

	bool AddEnableItem;
	int AddItem;
	BYTE AddItemLevel;
	bool AddItemSkill;
	bool AddItemLuck;
	bool AddItemOpt;
	bool AddItemExl;
	bool AddItemAnc;
	BYTE AddItemCount;

	bool RewardEnableItem;
	int RewardItem;
	int RewardLevel;
	bool RewardSkill;
	bool RewardLuck;
	bool RewardOpt;
	bool RewardExl;
	bool RewardAnc;
};

//struct PMSG_GRAND_DIALOG
//{
//	PBMSG_HEAD2	h;
//	BYTE Result;
//	WORD GrandNum;
//	WORD GrandMax;
//	WORD NeedReset;
//	WORD NeedLevel;
//	DWORD NeedMoney;
//	DWORD RewardPoint;
//	DWORD RewardWCoin;
//	DWORD RewardCredit;
//};
//
//struct GRANDSYSTEM_DATA
//{
//	BYTE Result;
//	WORD GrandNum;
//	WORD GrandMax;
//	WORD NeedReset;
//	WORD NeedLevel;
//	DWORD NeedMoney;
//	DWORD RewardPoint;
//	DWORD RewardWCoin;
//	DWORD RewardCredit;
//	DWORD ExFreePoint;
//};

class CResetSystem
{
public:
		 CResetSystem();
		 ~CResetSystem();
	bool Active;
	int State;
	int StateGR;
	void Init();
	void Load();
	void BindImages();
	void DrawReset();
	void Button(DWORD Event);
	void GCDialogInfo(GC_RESET_DATAEx* aRecv);
	char* GetItemString(int Item, int Level, bool Skill, bool Luck, bool Opt, bool Exl, bool Anc);
	char* GetItemString1(int Item, int Level, bool Skill, bool Luck, bool Opt, bool Exl, bool Anc);

	void GCGrandDialogInfo(PMSG_GRAND_DIALOG* lpMsg);
	void DrawGrand();
	void GrandButton(DWORD Event);


	RESETSYSTEM_DATA m_reset;
	GRANDSYSTEM_DATA m_Grand;
	int MainTableHigh;
	int MainTableWidth;
	int DrawStartX;
	int DrawStartY;
	int MenuOpen;
	bool DrawTable;

	DWORD ClickTime;
};
extern CResetSystem g_ResetSystem;

