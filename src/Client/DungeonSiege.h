#pragma once

#include "stdafx.h"
#include "Protocol.h"

#if(EVENT_DUNGEON_SIEGE)

enum DungeonState
{
	eEVENTDUNGEON_CLOSE,
	eEVENTDUNGEON_WAIT,
	eEVENTDUNGEON_START_STAGE1,
	eEVENTDUNGEON_WAIT_STAGE2,
	eEVENTDUNGEON_START_STAGE2,
};

struct PMSG_GC_DUNGEON_SIEGE_NPC
{
	PBMSG_HEAD2 h;
	bool bResultReg;
	int  iMsgCode;
	bool Captured;
	int  iCountGuild;
	char szGuildOwner[9];
	char szGuildName[10][9];
};

struct DUNGEON_SIEGE_REG_LIST
{
	char szGuildName[9];
};

struct PMSG_CG_DUNBEON_SIEGE_HWID
{
	PBMSG_HEAD2 h;
	DWORD dwHWID;
};

struct PMSG_GC_GUILD_INDEX
{
	PBMSG_HEAD2 h;
	int GuildNumber;
};

struct DUNGEON_VIEWE
{
	int aIndex;
	int GuildNumber;
};

struct PMSG_DUNGEON_SIEGE_SCORE
{
	PBMSG_HEAD2 h;
	int MyScore;
	int GuildCount;
	int GuildScore[10];
	int FinalGuildID1;
	int FinalGuildID2;
	int FinalScore1;
	int FinalScore2;
	char Name1[11];
	char Name2[11];
	bool InSiege;
};

struct PMSG_DUNGEON_SIEGE_PLAYER_CONNECT
{
	PBMSG_HEAD2 h;
	bool Captured;
	char szGuildOwner[9];
	int  iCountGuild;
	char szGuildName[10][9];
	char EventName[50];
};

struct PMSG_DUNGEON_SIEGE_STATE
{
	PBMSG_HEAD2 h;
	int EventState;
};

struct PMSG_PROTOCOL_RESULT
{
	PBMSG_HEAD2 h;
	int Result;
};

class CDungeonSiege
{
public:
		 CDungeonSiege();
		 ~CDungeonSiege();	 
	bool Active;
	int State;
	void Init();
	void Load();
	void BindImages();
	void Draw();
	void Button(DWORD Event);
	void GCNpcInfo(PMSG_GC_DUNGEON_SIEGE_NPC* lpMsg);
	void CGHWID();
	void GCVieweSiege(LPBYTE aRecv);
	void SetPKLevelTarget();
	void GCGuildNumber(PMSG_GC_GUILD_INDEX* lpMsg);
	void DrawRanking();
	void GCSiegeScore(PMSG_DUNGEON_SIEGE_SCORE* lpMsg);
	void GCPlayerConnect(PMSG_DUNGEON_SIEGE_PLAYER_CONNECT* lpMsg);
	void GCEventState(PMSG_DUNGEON_SIEGE_STATE* lpMsg);
	bool CheckStatus(int Status);
	
	int iEventStatus;
	int ireg_list_count;
	bool bRegistration;
	int  iMsgCode;
	bool bCaptured;
	char szLordGuild[9];
	DWORD dwHWID;
	int PlayerGuildNumber;
	int m_mysocre;
	int m_guildscore[10];
	int m_finalguild1;
	int m_finalguild2;
	int m_finalscore1;
	int m_finalscore2;
	char m_finalname1[11];
	char m_finalname2[11];
	char m_EventName[50];
	bool m_insiege;

	DUNGEON_VIEWE m_viewe[75];
	DUNGEON_SIEGE_REG_LIST reg_list[10];
	int MainTableHigh;
	int MainTableWidth;
	int DrawStartX;
	int DrawStartY;
	int MenuOpen;
	bool DrawTable;
	DWORD ClickTime;
};

extern CDungeonSiege g_DungeonSiege;

#endif