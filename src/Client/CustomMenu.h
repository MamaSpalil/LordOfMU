#include "stdafx.h"
#include "Protocol.h"

struct INFO_UPDATE
{
	PBMSG_HEAD2 h;
	BYTE Result;
	int Credits;
	//DWORD CSPoints;
	int Reset;
	int GReset;
	int VipGroup;
	int VipTimeTick;
	int QuestNum;
	int Exps;
//	int Credits;
};
struct INFO_DATA
{
	BYTE Result;
	int Credits;
	//DWORD CSPoints;
	int Reset;
	int GReset;
	int VipGroup;
	int VipTimeTick;
	int QuestNum;
	int Exps;
};
class MenuSystem
{
public:
	MenuSystem();

	~MenuSystem();

	bool Active;
	void rDrawInterface();
	/*RageSystem();
	void Load();
	void LoadImage();
	void LoadSkill(int ID, char *cName, int a1, int a2, int NextPoint, bool Percent);
	void rDrawInterface();
	void rDrawInfoText(int ID, int SkillID, int Point);
	void Button(DWORD Event);
	void DrawDirection(int X, int Y, int Width, int Height);
	void DrawButtonActive(int ID, char * Text, int Poin, int SkID);

	void DrawIcon(int ID1, int SkillID, int &ClickCount, int Tree,int NextShow);

	void FilterSkill(int SkillID);

	void RageSend(short Skill);
	void RageRecv(ClickSend * Recv);
	void RecvStart(sRageUserSend * Recv);
	void RecvExp(RageExpSend * Recv);
	void RageTabSend(short index);

	void RecvConfig(sConfigStruct * Recv);
	int StartX;
	int StartY;

	DWORD ClickTime;*/
	
	void InfoRecv(INFO_UPDATE * aRecv);
	void Button(DWORD Event);
	DWORD ClickTime;
	INFO_DATA m_info;
	int lastReport;
	int frameCount;
	int frameRate;
	char FPS_REAL[30];
private:

	bool Enable;

	/*int NextExp;
	RageUser rUser;
	RageSkill rSkill[R_MAX_SKILL];*/

};

extern MenuSystem gMenuSystem;