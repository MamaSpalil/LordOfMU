#pragma once

#define MAX_RESOLUTIONS 9
#define MAX_FONTS 9
#define MouseLButtonPush		*(BYTE*)0x07EB368C
#define MouseUpdateTime			*(int*)0x07960B4C//
#define MouseUpdateTimeMax		*(int*)0x007D4BF4//
#define ErrorMessage			*(int*)0x07EB71F0
#define pMouseOnZone			((int(__cdecl*)(int,int,int,int,int))0x0047CEA4)//47CEA4
#define pPlayBuffer				((int(__cdecl*)(int,int,int))0x004207F5)//00
//#define pGetFontDC				((HDC(__thiscall*)(LPVOID)) 0x0041FE93)

//#define	pSetTextColor			((void(__thiscall*)(LPVOID,BYTE,BYTE,BYTE,BYTE))0x0041FF35)//7D4C84
//#define	pSetBGTextColor			((void(__thiscall*)(LPVOID,BYTE,BYTE,BYTE,BYTE))0x0041FF97)
////#define pSetTextFont			((void(__thiscall*)(LPVOID,HFONT))0x0041FFF9)
//#define	pDrawText				((int(__thiscall*)(LPVOID,int,int,char*,int,int,int,int))0x00420024)
#define pTextureBlind			((void(__cdecl*)())0x0047CE63)//0047CE63
#define pFontNormal				*(HFONT*)0x05877FA0
#define pFontBold				*(HFONT*)0x05877FA4//38  //A0
#define pFontBig				*(HFONT*)0x05877FA8
#define pFontFixed				*(HFONT*)0x05877FAC

class COptionsMenu
{
public:
    COptionsMenu();
    ~COptionsMenu();

    void Init();

    static void DrawOptionsMenuUI();
    static void HandleOptionsMenuControl();

    void DrawOptionsMenuMain();
    void DrawOptionsMenuRender();
    void DrawOptionsMenuGeneral();
    void DrawOptionsMenuResolution();
	void DrawOptionsMenuFont();

	void DrawFloorDeletionOption();
	void DrawMapObjectDeletionOption();
	void DrawObjectsDeletionOption();
	void DrawShadowDeletionOption();
	void DrawEffectDeletionOption();
	void DrawSkillDeletionOption();
	void DrawMonsterHealthBarOption();
	void DrawAutoAttackBarOption();
    void DrawAutomaticAttackOption();
	void DrawSoundWhisperOption();
	void DrawVolumeOption();
	void DrawSlideHelpOption();
	void DrawGlowOption();
	void DrawOptionsMenuClose();
	bool IsDeleteFloorEnabled();
    bool IsDeleteMapObjectsEnabled();
    bool IsDeleteObjectsEnabled();
    bool IsDeleteShadowsEnabled();
    bool IsDeleteEffectsEnabled();
    bool IsDeleteSkillsEnabled();
	bool IsMonsterHealthBarEnabled();
	bool IsAutoAttackBarEnabled();
    bool IsAutomaticAttackEnabled();
    bool IsSoundWhisperEnabled();
    bool IsVolumeEnabled();
    bool IsSlideHelpEnabled();
    bool IsGlowEnabled();
    bool IsResolutionsEnabled();
    bool IsFontSizeEnabled();
    bool IsCloseEnabled();
	void ApplyFullScreenSettings();
public:
    float MainWidth;
    float MainHeight;
    float MainPosX;
    float MainPosY;
    int SectionWidth;
    float SectionPosY;
    float AntilagPosX;
    float GeneralPosX;
    float ResolutionPosX;
    float FontPosX;
    int DeleteFloor;
    int DeleteMapObjects;
    int DeleteObjects;
    int DeleteShadows;
    int DeleteEffects;
    int DeleteSkills;
	int DeleteAttackBar;
    int Resolutions[MAX_RESOLUTIONS];
    char ResolutionsText[MAX_RESOLUTIONS][10];
    int Fonts[MAX_FONTS];
};

extern COptionsMenu gOptionsMenu;

//#pragma once
//
//#include "stdafx.h"
//#include "Structure.h"

//#define RenderText				((char*(__cdecl*)(int,int,char*,int,int,int))0x005A0790)//005A0790
//#define EnableAlphaTest			((void(__cdecl*)(BYTE))0x006B8239)//006B8239
//#define RenderBitmap            ((void(*)(DWORD,float,float,float,float,float,float,float,float,int,int,GLfloat))0x006B97A7)//006B97A7
//
//#define CheckMouseIn            ((bool(*)(int, int, int, int, bool))0x0047CEA4)//0047CEA4
////#define PlayBuffer				((signed int(__cdecl*)(int a1, int a2, int a3)) 0x00420875)
//#define PlayBuffer				((signed int (__cdecl*)(int a1, int a2, int a3)) 0x004207F5)
//#define MouseXe				    *(int*)0x07EB37E4
//#define MouseYe				    *(int*)0x07EB37E0
//#define MouseLButtonPush		*(BYTE*)0x07EB368C
//#define MouseUpdateTime		    *(DWORD*)0x07960B4C
//#define MouseUpdateTimeMax		*(DWORD*)0x007D4BF4
//
//enum AntiLag_Options {
//	m_MapObject,
//	m_ItemWeapon,
//	m_ItemArmors,
//	m_ItemWings,
//	m_MapPlayer,
//	m_MapShadow,
//	m_EffStatic,
//	m_EffDynamic,
//	m_Fog,
//};
//
//class COptionWindow {
//	public:
//		COptionWindow();
//		~COptionWindow();
//		void Main();
//		void DrawBarFormex(float PosX, float PosY, float Width, float Height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
//		bool IsWorkZone(float a1, float a2, float a3, float a4);
//		static void RenderFrame(int a1, float a2, float a3);
//		static void RenderButtons();
//		static void RenderContents();
//		static void RenderButtonsAntiLag(AntiLag_Options id, bool type);
//		DWORD Delay_AntiLag;
//}; extern COptionWindow gOptionWindow;
