#pragma once
#include "Protocol.h"

int ReadFontFile(char* path);
void InitFont();
void ReloadFont();
HFONT FontNormal();
HFONT FontBool();
HFONT FontBig();
HFONT FontFixed();
void SetCursorFocus(int state);
extern int FontSize;

extern bool g_AntiLag;
#define Screen                  ".\\ScreenShot\\Screen(%02d_%02d-%02d_%02d)-%04d.jpg"
//---------------------------------------------------------------------------
#define MAX_OBJECTSTRUCT		10000
#define MAX_WIN_WIDTH		640
#define MAX_WIN_HEIGHT		480
#define	WindowName											0x007D36AC

#define Color4f(r, b, g, o)			((o << 24) + (g << 16) + (b << 8) + r)
// ----
#define ConnectAdress										0x007D2888
#define ClientExeSerial										0x007D3B5C
#define ClientExeVersion									0x007D3B54
#define GameState											*(int*)(0x7E8014)
#define ObjectChar											*(int*)(0x754F4D0)
#define PREVIEW_STRUCT										0x0754F4BC
#define PREVIEW_STRUCT_SIZE									0x484
#define PREVIEW_STRUCT_VIEW_POINTER							0x1A0 // nao tenho certeza
#define PARTY_MEMBER_COUNT									*(DWORD*)0x07A27B10
#define MAIN_CHARACTER_STRUCT								0x077A8FD8
#define MAIN_CONNECTION_STATUS								0x0587C394
#define pMainWorkProc										((void(__cdecl*)()) 0x006037E0)
#define iDraw_Text											0x005A0790
#define PREVIEW_STRUCT										0x0754F4BC
#define PREVIEW_STRUCT_SIZE									0x484
#define PREVIEW_STRUCT_VIEW_POINTER							0x1A0 // nao tenho certeza
#define MAIN_STATEEx										*(int*)0x007E8014
#define MAIN_CURSORX				*(int*)0x07EB37E4
#define MAIN_CURSORY				*(int*)0x07EB37E0
#define MAIN_MAPNUMBER				*(int*)0x007D60C8
#define MAIN_WIDTH					640
#define MAIN_HEIGHT					480
#define MAIN_RESOLUTION_WIDTH		*(GLsizei*)0x007E7F68
#define MAIN_RESOLUTION_HEIGHT		*(GLsizei*)0x007E7F6C
#define MAIN_REAL_WIDTH				*(float*)0x0587786C
#define MAIN_REAL_HEIGHT			*(float*)0x05877870
#define MAIN_FONT_HEIGHT			*(int*)0x078C5260
#define	MAIN_ATTACK_TARGET		*(int*)0x007D4C58
#define	MAIN_NPC_TARGET			*(int*)0x007D4C54
#define	OBJECT_TYPE_PLAYER		*(int*)0x0754F540
//---------------------------------------------------------------------------
#define pMainWorkProc			((void(__cdecl*)()) 0x006037E0)
#define	pBlendSwitch			((void(*)(bool))0x006B8239)
#define pDrawImage2				((void(__cdecl*)(int a1, float a2, float a3, float a4, float a5, int a6, int a7, float a8, float a9, char a10, char a11, float a12)) 0x006B97A7)
#define LoadFilesBMD			((void(__cdecl*)( int ModelID,char *Folder, char *Name, int Mode)) 0x69BA93)                            
#define LoadImagesJPG			((void(__cdecl*)(char *Folder, int ModelID, int a3, int a4, int a5, int a6)) 0x6D5B1B)//WORK!
#define LoadImagesTGA			((void(__cdecl*)(char *Folder, int ModelID, int a3, int a4, int a5, int a6)) 0x6D606C)//
#define PrintImages            ((void(__cdecl*)(int ImageId,  float cordX, float cordY, float Width, float Height, float a7, float a8, float a9, float a10, char a11, char a12, float a13))  0x6B97A7)
#define pLoadSomeForm			((void(__cdecl*)(HDC ItemID)) 0x006B0B0D)
#define oLoadSomeForm_Call		0x006BB741
#define pLoadImage			((int(__cdecl*)(char * Folder, int Code, int Arg3, int Arg4, int Arg5)) 0x6D606C)//
#define GameState               *(int*)(0x7E8014)
#define ObjectChar              *(int*)(0x754F4D0)
#define MapCheck                *(int*)0x7D60C8
#define ProtocolCore	        ((void(*)(DWORD protoNum,LPBYTE lpRecv,int aLen,int aIndex)) 0x50A070)

#define LevelUpAddPoint	        ((void(*)(struct PMSG_LVPOINTADD*)) 0x4F5E70)
#define LevelUpRecv		        ((void(*)(struct PMSG_LEVELUP*)) 0x4F5B40)

#define DrawBar                 ((double(__cdecl*)(float a1, float a2, int a3, float a4, float a5)) 0x6B9509)
#define DrawTab                 ((int(__cdecl*)(signed int a1, signed int a2, LPCSTR lpMultiByteStr)) 0x5A0950)
#define DrawInfo                ((int(__cdecl*)(int a1, int a2, LPCSTR a3, int a4, char a5, int a6)) 0x5A0790)
#define GetPosFromAngle		    ((void(__cdecl*)(struct Angle* Angle, int * PosX, int * PosY)) 0x6B7DE4)
#define SetBlend				((void(__cdecl*)(BYTE Mode)) 0x6B8239)
#define GLSwitchBlend			((void(__cdecl*)()) 0x6B82D4)
#define GLSwitch				((void(__cdecl*)()) 0x6B81BA)
#define DrawBarForm			    ((void(__cdecl*)(float PosX, float PosY, float Width, float Height)) 0x6B96C4)
#define DrawTextOut				((int(*)(int, int, LPCSTR, int, char, int))(0x5A0850))
#define GetCursorWorkSize       ((int(__cdecl*)(int PosX,int PosY,int Width,int Height,int Mode)) 0x47CEA4)
#define WndProcEx                 (WNDPROC)(0x4DA056)
#define MainSpeedFrame1	        (float*)(0x7B3A4C)
#define MainSpeedFrame2		    (float*)(0x7B3A20)
#define LoadImg                 ((int(*)(char*, int, int, int, int, int)) 0x6D5B1B)
#define UnBlend                 ((int(*)()) 0x6B82D4)
#define pDrawImage2				((void(__cdecl*)(int a1, float a2, float a3, float a4, float a5, int a6, int a7, float a8, float a9, char a10, char a11, float a12)) 0x006B97A7)
#define	pDrawTextOut			((char(*)(int x, int y, char *, int BackgroundWidth, int AlignCenter, int a6))0x005A0790)
#define	pSendChatTextEx			((void(*)(const char * szSender, const char *, BYTE Color))0x005A17E0)
#define	pDrawToolTipEx			((void(*)(int x, int y, char *))0x005A0950)
#define	pBlendDisableEx			((void(*)(void))0x006B82D4)
typedef char * (__cdecl *pDrawText) (int x, int y, char * Text, int BackgroundWidth, int AlignCenter, int a6);
extern pDrawText MU_DrawTextOut;
typedef void(__cdecl *pMU_ADDSimple)();
extern pMU_ADDSimple MU_ADDSimple;
#define FPTR(Func)											(DWORD)Func
#define SHIWORD(x)											 (*((short*)&(x)+1))
void cOtherLoad();

#define	pDrawBarForm2			((void(__cdecl*)(float,float,float,float,float,int))0x006B96C4)//006B96C4
#define pSetCursorFocus					*(DWORD*)0x07EB71F0
#define IsRelease						*(bool*)0x07EB368C
#define pChatWindow				*(BYTE*)0x7D4C90


#define Hero *(DWORD*)0x0754F4D0
#define g_hWnd	*(HWND*)0x05877F90



struct PMSG_PERIODIC_EFFECT_RECV
{
	PBMSG_HEAD header; // C1:2D
	WORD group;
	WORD value;
	BYTE state;
	DWORD time;
};
void GCPeriodicEffectRecv(PMSG_PERIODIC_EFFECT_RECV* lpMsg);


#define iClock_Interface1			0x00603830
#define iClock_Interface2			0x00603834
#define iClock_Interface3			0x00603835
#define iClock_Interface4			0x0060383C
#define iClock_Interface5			0x00603841
#define iClock_Interface6			0x00603846
#define iClock_Interface7			0x0060384A
#define iClock_Interface8			0x0060384B

struct Propensity_
{
	PBMSG_HEAD2 h;
	int number;

};
void Propensity(Propensity_ * aRecv);

void Changer();
void ObjectPointer();
void Optimize();
void SetFrameSleep();
int cGameLoad();
void DrawIcons(int x, int y, DWORD c);
void Fix();
void InitFixExp();
void AntiLag();



enum MAP_INDEX
{
	MAP_INDEX_RORENCIA = 0x0,
	MAP_INDEX_DUNGEON = 0x1,
	MAP_INDEX_DEVIAS = 0x2,
	MAP_INDEX_NORIA = 0x3,
	MAP_INDEX_LOSTTOWER = 0x4,
	MAP_INDEX_RESERVED = 0x5,
	MAP_INDEX_BATTLESOCCER = 0x6,
	MAP_INDEX_ATHLANSE = 0x7,
	MAP_INDEX_TARKAN = 0x8,
	MAP_INDEX_DEVILSQUARE = 0x9,
	MAP_INDEX_ICARUS = 0xa,
	MAP_INDEX_BLOODCASTLE1 = 0xb,
	MAP_INDEX_BLOODCASTLE2 = 0xc,
	MAP_INDEX_BLOODCASTLE3 = 0xd,
	MAP_INDEX_BLOODCASTLE4 = 0xe,
	MAP_INDEX_BLOODCASTLE5 = 0xf,
	MAP_INDEX_BLOODCASTLE6 = 0x10,
	MAP_INDEX_BLOODCASTLE7 = 0x11,
	MAP_INDEX_CHAOSCASTLE1 = 0x12,
	MAP_INDEX_CHAOSCASTLE2 = 0x13,
	MAP_INDEX_CHAOSCASTLE3 = 0x14,
	MAP_INDEX_CHAOSCASTLE4 = 0x15,
	MAP_INDEX_CHAOSCASTLE5 = 0x16,
	MAP_INDEX_CHAOSCASTLE6 = 0x17,
	MAP_INDEX_KALIMA1 = 0x18,
	MAP_INDEX_KALIMA2 = 0x19,
	MAP_INDEX_KALIMA3 = 0x1a,
	MAP_INDEX_KALIMA4 = 0x1b,
	MAP_INDEX_KALIMA5 = 0x1c,
	MAP_INDEX_KALIMA6 = 0x1d,
	MAP_INDEX_CASTLESIEGE = 0x1e,
	MAP_INDEX_CASTLEHUNTZONE = 0x1f,
	MAP_INDEX_DEVILSQUARE2 = 0x20,
	MAP_INDEX_AIDA = 0x21,
	MAP_INDEX_CRYWOLF_FIRSTZONE = 0x22,
	MAP_INDEX_CRYWOLF_SECONDZONE = 0x23,
	MAP_INDEX_KALIMA7 = 0x24,
	MAP_INDEX_KANTURU1 = 0x25,
	MAP_INDEX_KANTURU2 = 0x26,
	MAP_INDEX_KANTURU_BOSS = 0x27,
	MAP_INDEX_GM_SUMMONZONE = 0x28,
	MAP_INDEX_BALGASS_BARRACKS = 0x29,
	MAP_INDEX_BALGASS_REFUGEE = 0x2A,
	MAP_INDEX_ILLUSIONTEMPLE1 = 0x2D,
	MAP_INDEX_ILLUSIONTEMPLE2 = 0x2E,
	MAP_INDEX_ILLUSIONTEMPLE3 = 0x2F,
	MAP_INDEX_ILLUSIONTEMPLE4 = 0x30,
	MAP_INDEX_ILLUSIONTEMPLE5 = 0x31,
	MAP_INDEX_ILLUSIONTEMPLE6 = 0x32,
	MAP_INDEX_ELBELAND = 0x33,
	MAP_INDEX_BLOODCASTLE8 = 0x34,
	MAP_INDEX_CHAOSCASTLE7 = 0x35,
	MAP_INDEX_DEVILSQUARE7 = 0x36,
	MAP_INDEX_SWAMPOFCALMNESS = 0x38,
	MAP_INDEX_RAKLION = 0x39,
	MAP_INDEX_RAKLION_BOSS = 0x3A,
	MAP_INDEX_SANTATOWN = 0x3E,
	MAP_INDEX_VULCAN			= 0x3f,
	MAP_INDEX_VULCANROOM		= 0x40,
	MAP_INDEX_DOUBLE_GOER1 = 65,	//Season 5 DoppelGanger
	MAP_INDEX_DOUBLE_GOER2 = 66,	//Season 5 DoppelGanger
	MAP_INDEX_DOUBLE_GOER3 = 67,	//Season 5 DoppelGanger
	MAP_INDEX_DOUBLE_GOER4 = 68,	//Season 5 DoppelGanger
	MAP_INDEX_IMPERIAL1 = 69,		//Season 5 ImperialFort
	MAP_INDEX_IMPERIAL2 = 70,
	MAP_INDEX_IMPERIAL3 = 71,
	MAP_INDEX_IMPERIAL4 = 72
};
extern bool			Antilag;

void AntilagSwith();
void AntiLagOn();
void AntiLagOff();
__declspec()void Summon();


enum eWindowsType
{
	WINDOWS_NONE				= 0,
	WINDOWS_FRIEND_LIST			= 1,
	WINDOWS_MOVE_LIST			= 2,
	WINDOWS_PARTY				= 3,
	WINDOWS_QUEST				= 4,
	WINDOWS_GUILD				= 5,
	WINDOWS_TRADE				= 6,
	WINDOWS_WAREHOUSE			= 7,
	WINDOWS_UNKNOWN_1			= 8,
	WINDOWS_CHAOS_MIX			= 9,
	WINDOWS_COMMAND				= 10,
	WINDOWS_PET					= 11,
	WINDOWS_STORE				= 12,
	WINDOWS_DEVIL_SQUARE		= 13,
	WINDOWS_MOVING_SERVER		= 14,
	WINDOWS_BLOOD_CASTLE		= 15,
	WINDOWS_PET_TRAINER			= 16,
	WINDOWS_SHOP				= 17,
	WINDOWS_STORE_OTHER			= 18,
	WINDOWS_GUILD_MASTER		= 19,
	WINDOWS_GUARDMAN_SIEGUE		= 20,
	WINDOWS_SENIOR_MIX			= 21,
	WINDOWS_GUARDMAN_LAND		= 22,
	WINDOWS_CATAPULT_INVADING	= 23,
	WINDOWS_CATAPULT_DEFENDING	= 24,
	WINDOWS_CASTLE_GATE_SWITCH	= 25,
	WINDOWS_CHARACTER			= 26,
	WINDOWS_INVENTORY			= 27,
	WINDOWS_REFINERY			= 28,
	WINDOWS_REFINERY_WARNING	= 29,
	WINDOWS_KANTURU_GATE		= 30,
	WINDOWS_WEREWOLF			= 31,
	WINDOWS_ILLUSION_TEMPLE1	= 32,
	WINDOWS_ILLUSION_TEMPLE2	= 34,
};
//---------------------------------------------------------------------------