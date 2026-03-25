#include "stdafx.h"
#include "Other.h"

#pragma once
enum ColorMacro
{
	eShinyGreen		= Color4f(172, 255, 56, 255),
	eGold			= Color4f(255, 189, 25, 255),
	eWhite			= Color4f(255, 255, 255, 255),
	eWhite180		= Color4f(255, 255, 255, 180),
	eOrange			= Color4f(255, 105, 25, 255),
	eGray100		= Color4f(50, 50, 50, 100),
	eGray150		= Color4f(50, 50, 50, 150),
	eBlowPink		= Color4f(220, 20, 60, 255),
	eRed			= Color4f(225, 0, 0, 255),
	eExcellent		= Color4f(0, 225, 139, 255),
	eAncient		= Color4f(1, 223, 119, 255),
	eSocket			= Color4f(153, 102, 204, 255),
	eBlue			= Color4f(36, 242, 252, 255),
	eYellow			= Color4f(255, 255, 0, 255),
	eBrown			= Color4f(69, 39, 18, 255),
};
#define MAX_WINDOW_EX		100
enum ObjectID
{
	eSAMPLEBUTTON = 1,
		eRageSystem,
	eRageTable,
	eRageClose,
	eRagePointTable,
	eRageIcon1,
	eRageIcon2,
	eRageIcon3,
	eRageIcon4,
	eRageIcon5,
	eRageIcon6,
	eRageIcon7,
	eRageIcon8,
	eRageIcon9,
	eRageIcon10,
	eRageIcon11,
	eRageIcon12,
	eRageIcon13,
	eRageIcon14,
	eRageIcon15,
	eRageIcon16,
	eRageIcon17,
	eRageIcon18,
	eRageIcon19,
	eRageIcon20,
	eRageIcon21,
	eRageIcon22,
	eRageIcon23,
	eRageIcon24,
	eRageIcon25,
	eRageIcon26,
	eRageIcon27,
	eRageIcon28,
	eRageIcon29,
	eRageIcon30,
	eRageIcon31,
	eRageIcon32,
	eRageIcon33,
	eRageIcon34,
	eRageIcon35,
	eRageTeleport,
	eRagePet,

	//eRANK_MAIN,
	//eRANK_TITLE,
	//eRANK_FRAME,
	//eRANK_FOOTER,
	//eRANK_DIV,
	//eRANK_BUTTON,
	//eRANK_TAB1,
	//eRANK_TAB2,
	//eRANK_TAB3,
	//eRANK_TAB4,
	eCAMERA_MAIN,
	eCAMERA_BUTTON1,
	eCAMERA_BUTTON2,
	//eCRAFT_MAIN,
	//eCRAFT_TITLE,
	//eCRAFT_FRAME,
	//eCRAFT_FOOTER,
	//eCRAFT_DIV,
	//eCRAFT_TAB1,
	//eCRAFT_TAB2,
	//eCRAFT_TAB3,
	//eCRAFT_TAB4,
	//eCRAFT_CLOSE,
	//eCRAFT_INFOBG,
	//eCRAFT_MONEYBG,
	//eCRAFT_FINISH,
	//eRESET_MAIN,
	//eRESET_TITLE,
	//eRESET_FRAME,
	//eRESET_FOOTER,
	//eRESET_DIV,
	//eRESET_INFOBG,
	//eRESET_MONEYBG,
	//eRESET_CLOSE,
	//eRESET_FINISH,
	//eOFFEXP_SWITCH,
	//eNEWS_MAIN,
	//eNEWS_TITLE,
	//eNEWS_FRAME,
	//eNEWS_FOOTER,
	//eNEWS_DIV,
	//eNEWS_INFOBG,
	//eNEWS_CLOSE,
	//eNEWS_BACK,
	//eTIME,
	//eQUEST_MAIN,
	//eQUEST_TITLE,
	//eQUEST_FRAME,
	//eQUEST_FOOTER,
	//eQUEST_DIV,
	//eQUEST_CLOSE,
	eBankCount,
	eBankAdd,
	eBankClose,
	eBankNum1,
	eBankNum2,
	eBankNum3,
	eBankNum4,
	eBankNum5,
	eBankNum6,
	eBankNum7,
	eBankNum8,
	eBankNum9,
	eBankNum10,
	eBankNum11,
	eBankNum12,
	//eButtonCharacters_BackGround,
	eButtonCharacters_NextPage,
	eButtonCharacters_BackPage,
		


	eButtonMenu,
	eJewelsBank,
	eResetClose,

	ePT_DUNGEON_SIEGE_MAIN,
	ePT_DUNGEON_SIEGE_TITLE,
	ePT_DUNGEON_SIEGE_FRAME,
	ePT_DUNGEON_SIEGE_FOOTER,
	ePT_DUNGEON_SIEGE_DIV,
	ePT_DUNGEON_SIEGE_DIV2,
	ePT_DUNGEON_SIEGE_REG,
	ePT_DUNGEON_SIEGE_OK,

	ePT_DUNGEON_SIEGE_NAME,
	ePT_DUNGEON_SIEGE_NAME2,

	ePT_DUNGEON_SIEGE_GUILD1,
	ePT_DUNGEON_SIEGE_GUILD2,
	ePT_DUNGEON_SIEGE_GUILD3,
	ePT_DUNGEON_SIEGE_GUILD4,
	ePT_DUNGEON_SIEGE_GUILD5,
	ePT_DUNGEON_SIEGE_GUILD6,
	ePT_DUNGEON_SIEGE_GUILD7,
	ePT_DUNGEON_SIEGE_GUILD8,
	ePT_DUNGEON_SIEGE_GUILD9,
	ePT_DUNGEON_SIEGE_GUILD10,
	ePT_DUNGEON_SIEGE_NAMEOWNER,
	ePT_DUNGEON_SIEGE_DIVOWNER,
	ePT_DUNGEON_SIEGE_GUILDOWNER,

	AdvanceStats,

	eMAPTELEPORT_POINT1,
	eMAPTELEPORT_POINT2,
	eMAPTELEPORT_POINT3,
	eMAPTELEPORT_POINT4,
	eMAPTELEPORT_POINT5,
	eMAPTELEPORT_POINT6,
	eMAPTELEPORT_POINT7,
	eMAPTELEPORT_POINT8,
	eMAPTELEPORT_POINT9,
	eMAPTELEPORT_POINT10,

	eMAPTELEPORT_POINT11,
	eMAPTELEPORT_POINT12,
	eMAPTELEPORT_POINT13,
	eMAPTELEPORT_POINT14,
	eMAPTELEPORT_POINT15,
	eMAPTELEPORT_POINT16,
	eMAPTELEPORT_POINT17,
	eMAPTELEPORT_POINT18,
	eMAPTELEPORT_POINT19,
	eMAPTELEPORT_POINT20,

	eMAPTELEPORT_POINT21,
	eMAPTELEPORT_POINT22,
	eMAPTELEPORT_POINT23,
	eMAPTELEPORT_POINT24,
	eMAPTELEPORT_POINT25,
	eMAPTELEPORT_POINT26,
	eMAPTELEPORT_POINT27,
	eMAPTELEPORT_POINT28,
	eMAPTELEPORT_POINT29,
	eMAPTELEPORT_POINT30,

	eMAPTELEPORT_POINT31,
	eMAPTELEPORT_POINT32,
	eMAPTELEPORT_POINT33,
	eMAPTELEPORT_POINT34,
	eMAPTELEPORT_POINT35,
	eMAPTELEPORT_POINT36,
	eMAPTELEPORT_POINT37,
	eMAPTELEPORT_POINT38,
	eMAPTELEPORT_POINT39,
	eMAPTELEPORT_POINT40,

	eRewardDay,
	eLuckyWheelMain,

};

struct InterfaceObject
{
	DWORD	ModelID;
	float	Width;
	float	Height;
	float	X;
	float	Y;
	float	MaxX;
	float	MaxY;
	float   ScaleX;
	float   ScaleY;
	DWORD	EventTick;
	bool	OnClick;
	bool	OnShow;
	BYTE	Attribute;
};
//enum ObjWindowsEx
//{
//	exWinStatsAdvance	= 1,
//	/*exWinReset			= 2,
//	exWinGrandReset		= 3,
//	exWinRanking		= 4,
//	exWinDonate			= 5,
//	exWinPremium		= 6,
//	exWinMiniMenu		= 7,
//	exWinSettings		= 8,
//	exWinPTSearchMaster	= 9,
//	exWinPTSearchUser	= 10,
//	exWinPersonalSearch	= 12,
//	exWinPersonalPrice	= 13,
//	exWinNPCBuffer,
//	exWinSmithy,
//	exWinSmithyCheck,
//	exWinCheckOffAfk,
//	exWinMarriage,
//	exWinAddPoints,
//	exWinAddResetPoint,
//	exWinCustomMenu,
//	exWinCustomMenuChangeClass,
//	exWinDungeonSiege,
//	exWinTeamVsTeam,
//	exWinTeamVsTeamStatistic,
//	exWinAchievements,
//	exWinAchievementsPower,
//	exWinTeleport,
//	exWinTeleportEvo,
//	exWinResetRage,
//	exWinDonateCheck,
//	exWinPKClear,
//	exWinAccWarning,
//	exWinAccMenu,
//	exWinAccOption,
//	exWinAccPC,
//	exWinAccMessage,
//	exWinAccNumber,
//	exWinReferralMenu,
//	exWinReferralSystem,
//	exWinReferralSystem2,
//	exWinQuest,
//	exWinLottery,
//	exWinItemMarket,
//	exWinItemMarket2,
//	exWinMenuV3,
//	exWinStatsAdvance,*/
//};
class CInterface
{
public:
	//CInterface();
	void Init();
	static void MainProc();
	static void MainProc2(int a1, float a2, float a3, float a4, float a5, int a6, int a7, float a8, float a9, char a10, char a11, float a12);
	void MainProcBar();
	
	void BindObject(short MonsterID, DWORD ModelID, float Width, float Height, float X, float Y,float ScaleX,float ScaleY);
	void BindObjects(short ID, DWORD ModelID, float Width, float Height, float X, float Y);
	bool IsWorkZone(short ObjectID);
	float GetResizeX(short ObjectID);
	//bool WindowEx[MAX_WINDOW_EX];
	//bool CheckWindowEx(int WindowID);
	void DrawGUIex(short ObjectID);
	void DrawGUI(DWORD ModelID,short ObjectID, float PosX, float PosY);
	void DrawGUIs(short ObjectID, float PosX, float PosY);
	bool IsWorkZoneObj(short ObjectID);
	int CloseWindow(int WindowID);
	static void LoadImages();
	InterfaceObject Data[100];
	void MouseClickProc();
		#if(CUSTOM_VALUE_MENU)
	void BindValueMenu();
	void DrawValueMenu();
	void ControllerValueMenu(DWORD Event);
	// --
	void BindGuildValueMenu();
	void DrawGuildValueMenu();
	void ControllerGuildValueMenu(DWORD Event);
	#endif
#ifdef MULTI_WAREHOUSE
	void        WarehouseButtonAddUI(DWORD Event);
	void		Allow_DrawBankCountWindow() { this->Data[eBankCount].OnShow = true; this->StateBank = 1;};// pSetCursorFocus = true; pSetCursorFocus2 = true;  /*pSetCursorFocus2 = true;*/ };
	void		Close_DrawBankCountWindow() { this->Data[eBankCount].OnShow = false; this->StateBank = 0;};// pSetCursorFocus= false; pSetCursorFocus2 = false;/* pSetCursorFocus2 = false;*/ };
	void        DrawBankCountWindow();
	int         TotalBanks;
	int			StateBank;
	DWORD       Zen;
#endif
#if(DEV_DAMAGE_TABLE)
	bool m_EnableDamageTable;
	void DrawDamageTable();
	float		RightMiniInfoY;

#endif
	int StateD;
	int StateFog;
	int StateWare;
	int StateWarex;
	int DrawFormat(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...);
};

extern CInterface * gInterfaces;

#define iBlend_Enable			0x006B8239
#define iBlend_Disable			0x006B82D4
#define iThis_Pointer			0x0079191C
#define MU_DRAWGUI              0x006B97A7 // ??
#define MU_DRAW_COLORED_BUTTON	0x007334F6
#define MU_ADDSimple_Offset     0x006B81BA

#define pRenderItem3D			((void(*)(float,float,float,float,int,int,int,int,int))0x00651930)//00651930//!!!!
#define pBeginBitmap			((void(*)())0x006B95C4)//006B95C4!!!!
#define pUpdateMousePosition	((void(*)())0x006BAD0E)//006BAD0E //xz
#define pgluPerspective2		((void(*)(float,float,float,float))0x006B7B85)//006B7B86
#define pGetOpenGLMatrix		((void(*)(LPVOID))0x006B7B1A)//006B7B1A
#define pCameraMatrix			(LPVOID*)0x07EB36A8//07EB36A8
#define	EnableAlphaTest			((void(*)(bool))0x006B8239) //pSetBlend//006B8239//!!!!
#define pglViewport2			((void(*)(int,int,int,int))0x006B8590)//006B8590
#define pEnableDepthTest		((void(*)())0x006B8082)//006B8082
#define pEnableDepthMask		((void(*)())0x006B80C6)//006B80C6
#define RenderBitmap            ((void(__cdecl*)(int ImageId,  float cordX, float cordY, float Width, float Height, float a7, float a8, float a9, float a10, char a11, char a12, float a13))  0x006B97A7)//006B97A7

#define MAIN_RESOLUTION_X		0x007E7F68
#define MAIN_RESOLUTION_Y		0x007E7F6C
void DrawItemIMG(float PosX, float PosY, float Width, float Height, int ItemID, int Level, int Excl, int Anc, bool OnMouse);
void LoadAlphaImg(char * a1, int a2, int a3, int a4, int a5, int a6);
void LoadJpgImg(char * a1, int a2, int a3, int a4, int a5, int a6);
void GXDrawTextEx(int PosX, int PosY, char * Text, int a4, int a5, int a6, int a7, int a8);
bool CheckZone(float PosX, float PosY, float Width, float Height);
void UPDATE_FPS();
void DrawInterfaceBind();
void TeleportList();
void CordRun();
extern int CheckLoadImages;
typedef void (__cdecl *pDrawInterface) (void);
void Test123();
extern pDrawInterface MU_DrawInterface;