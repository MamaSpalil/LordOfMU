#pragma once

#include "stdafx.h"
//#include "ProtocolDefine.h"
#include "Protocol.h"



#define ITEMRANK_MAX_OPTION		6

struct ItemRank_Data
{
	BYTE  RankID;
	short OptionValue[ITEMRANK_MAX_OPTION];
	ItemRank_Data()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct ItemRank_List
{
	BYTE RankID;
	WORD ItemType;
	BYTE MinLevel;
	ItemRank_List()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct PMSG_INITRANK
{
	PBMSG_HEAD2 h;
	BYTE Result;
};

struct PMSG_DATARANK
{
	PBMSG_HEAD2 h;
	BYTE  RankID;
	BYTE OptionValue[ITEMRANK_MAX_OPTION];
};

struct PMSG_LISTRANK
{
	PBMSG_HEAD2 h;
	BYTE RankID;
	WORD ItemType;
	BYTE MinLevel;
};

class ItemRank
{
//public:
//		 ItemRank();
//		 ~ItemRank();
//
//	void Init();
//	void Load();
//	int  IsRankItem(WORD ItemType, BYTE ItemLevel);
//	bool IsDataRank(BYTE RankID);
//	int  GetRankValueOption(int id, BYTE OptionType);
//	static void	SetOptionText(int ItemID, int Line, WORD OptionID, BYTE Arg4, int Arg5);
//	void GCRankInit(PMSG_INITRANK* aRecv);
//	void GCRankData(PMSG_DATARANK* aRecv);
//	void GCRankList(PMSG_LISTRANK* aRecv);
//
//private:
//	std::vector<ItemRank_Data> m_Data;
//	std::vector<ItemRank_List> m_ItemList;
}; 
extern ItemRank g_ItemRank;




