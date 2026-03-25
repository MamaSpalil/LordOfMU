#pragma once
#include "stdafx.h"

//dword_7966984
//#define pSetCursorFocus			*(DWORD*)0x7960B54
//#define pSetCursorFocus2		*(BYTE*)0x78C5274

//#define pSetCursorFocus		*(DWORD*)0x7EBD020	//Done
#define pProtocolCores										((int(__cdecl*)(int, PBYTE, int, int))0x0050A070)
#define pCursorX                *(int*)0x7EB37E4 //OK
#define pCursorY                *(int*)0x7EB37E0  //OK
#define pOnLine					*(DWORD*)0x587C394 //ok
#define pGameWindow				*(HWND*)0x5877F90  //ok
#define pUserStat				((LPVOID(*)()) 0x588D70)//???
//#define pSetCursorFocus				*(DWORD*)0x7EB71F0
#define	MAIN_WINDOW				0x05877F90
#define pPreviewThis            ((LPVOID(*)()) 0x0074C447) // ok
#define pGetPreviewStruct        ((DWORD(__thiscall*)(LPVOID This, int ViewportID)) 0x754F4BC)  //ok    // 0x00731FB8
#define pGetPosFromAngle        ((void(__cdecl*)(VAngle * Angle, int * PosX, int * PosY)) 0x006B7DE4)  //OK!!
//
//#define sub_6D7CE3            ((int(__cdecl*)(int)) 0x006D7CE3) // ok
//#define sub_682783      ((void(__cdecl*)(int a1, char a2, int a3, int a4)) 0x682783) //
//#define sub_6771F8            ((char(__cdecl*)(float a1, float a2, float a3)) 0x006771F8) // 
//#define sub_791979            ((int(__cdecl*)()) 0x00791979) //Random?
#define sub_45FC93  ((char(__cdecl*)(int Struct, int a2)) 0x0045FC93) // 
//int sub_791979()
//int __thiscall sub_6D7CE3(int this)
#define pSetBlend                ((void(__cdecl*)(BYTE Mode)) 0x6B8239) //ok
//tplist
#define pDrawBarForm			((int(__cdecl*)(float PosX, float PosY, float Width, float Height)) 0x006B96C4)
#define pDrawBarFormex			((void(__cdecl*)(float,float,float,float,float,int))0x006B96C4)
#define GetRealWidth(x)		(x * pWinWidth / 0x280)
#define GetRealHeight(y)	(y * pWinHeight / 0x1E0)
//
#define	pDrawToolTipEx			((void(*)(int x, int y, char *))0x005A0950)
#define pGLSwitchBlend            ((void(__cdecl*)()) 0x6B82D4) //ok
#define pGLSwitch                ((void(__cdecl*)()) 0x6B81BA) //ok
#define pDrawPartyHPBarForm      ((void(__cdecl*)(float PosX, float PosY, float Width, float Height, float Arg5, int Arg6)) 0x6B96C4) //ok
#define pLoadImageJPG             ((int(__cdecl*)(char * Folder, int Code, int Arg3, int Arg4, int Arg5, int Arg6)) 0x006D5B1B)
#define pLoadImageTGA            ((char(__cdecl*)(int ImgFolder, int ImgID, int a3, int a4, int a5, char a6)) 0x006D606C)//CHECK OK                         
#define sDrawImageJPG            ((int(__cdecl*) (int ItemID, int a2, float a3, float a4, float a5, float a6, float a7, float a8, float a10, char a11, char a12, float a13)) 0x006B97A7)//CHECK OK

#define pCreateConnect			((int(__cdecl*)(char * IP, WORD Port)) 0x004E2020) //OK
#define pUpdateUserStat			((void(__thiscall*)(LPVOID This, int * Value, int Code)) 0x4EA460)
#define sub_403D10			((void(__thiscall*)(int This, BYTE  a2, int a3)) 0x00403D10)
#define sub_6A73DE          ((int(__thiscall*)(int a1, int a2, int a3)) 0x006A727E) //OK

#define  sub_6BB4E0 ((void(__cdecl*)()) 0x6BB300)
#define  sub_4209D7	((void(__cdecl*)()) 0x4209D7)
#define  sub_622560	((void(__cdecl*)()) 0x622530)
#define  sub_6B3F8D	((void(__cdecl*)()) 0x6B3DA7)
#define  sub_448940	((void(__cdecl*)()) 0x448940)
#define sub_4E9140 ((int(__cdecl*)(int * Num)) 0x4E9140)

#define  sub_53D400 ((int(__cdecl*)(int Num)) 0x53D400)
#define  sub_4E3140 ((int(__cdecl*)()) 0x4E3140)
#define  sub_689842 ((int(__cdecl*)()) 0x6896DF)

#define  dword_7E8088  *(DWORD*)0x7E8014
#define  dword_587C388 *(DWORD*)0x587C318
#define  byte_7EBD04F   *(BYTE*)0x7EB721F
//signed int __thiscall sub_403D10(int this, int a2, int a3)


//NEED
//// ----------------------------------------------------------------------------------------------
#define pAddItemEffect            ((char(__cdecl*)(int ItemID, int ObjectID, int a3, GLfloat alpha,int a5, int a6, int a7, int a8, int a9)) 0x693bda)//CHECK OK
//#define pAddEffectSet1             ((char(__cdecl*)( *this, int a2, int  a3, int a4,char a5)) 0x515A29)//CHECK OK
//#define pAddEffectSet2             ((char(__cdecl*)(int ItemID, int  a2, DWORD  a3, int  a4,int a5,DWORD a6,DWORD a7)) 0x593958)//CHECK OK
//#define unk_69C6354			*(DWORD*)0x69C6354
//#define flt_587C6C0         *(float*)0x587C6C0
//#define flt_7B3A00          *(float*)0x7B3A00
#define flt_7B38C0          *(float*)0x7B38C0
#define flt_7B3A1C          *(float*)0x7B3A1C
#define flt_7B389C          *(float*)0x7B389C
#define flt_7B3A70          *(float*)0x7B3A70
//#define flt_7B373C          *(float*)0x7B373C
//#define dword_7A2DA28        *(DWORD*)0x7A2DA28
//#define dword_79FBECC        *(DWORD*)0x79FBECC
//#define dword_7A2DA2C        *(DWORD*)0x7A2DA2C
//#define dword_7A2AEC8        *(DWORD*)0x7A2AEC8
//#define pAddTextInformation      ((void(__cdecl*)(float a1, int a2, ObjectItem * lpItem, int a4, int a5)) 0x00615230) //ok
//#define pSetTextInformation      ((int(*)(int a1, const char *a2,...)) 0x00791CF0)
//#define pGetTextInformation      ((int(__cdecl*)(signed int a1, int a2, unsigned __int8 a3, unsigned __int8 a4, int a5)) 0x00614A30)

//int sub_791CF0(int a1, const char *a2, ...)
#define flt_7B3728 *(float*)0x7B3728
#define flt_7B36EC *(float*)0x7B36EC
#define flt_7B3A90 *(float*)0x7B3A90
#define flt_7B3838 *(float*)0x7B3838
#define flt_7B3A74 *(float*)0x7B3A74
#define flt_7B58B8 *(float*)0x7B58B8
#define flt_7B3C84 *(float*)0x7B3C84
#define flt_7B3AA4 *(float*)0x7B3AA4
#define flt_7B51E4 *(float*)0x7B51E4
#define flt_7B3A98 *(float*)0x7B3A98
#define flt_7B58B0 *(float*)0x7B58B0
#define flt_7B3AA0 *(float*)0x7B3AA0
#define flt_7B5854 *(float*)0x7B5854
#define flt_7B3BEC *(float*)0x7B3BEC
#define flt_7B3B34 *(float*)0x7B3B34
#define flt_7B5418 *(float*)0x7B5418
#define flt_7B3C94 *(float*)0x7B3C94

//#define dword_587E610       *(DWORD*)0x587E610
//#define dword_7D60C8        *(DWORD*)0x7D60C8

//#define sub_6B8419             ((DWORD(__cdecl*)(int ItemID)) 0x6B8419)//CHECK OK
//#define sub_4136E0             ((int(__cdecl*)(float ItemID)) 0x4136E0)//CHECK OK
//#define sub_6BA1D4             ((int(__cdecl*)(float ItemID)) 0x6BA1D4)//CHECK OK


//#define sub_41E970             ((DWORD(__cdecl*)(int ItemID)) 0x41E970)//CHECK OK
// --------------------------------------------------------------------------------------------
//#define sub_6B839A             ((void(__cdecl*)(int ItemID)) 0x6B839A)
//#define sub_791092             ((DWORD(__cdecl*)(int ItemID)) 0x791092)
//#define sub_7924D9           ((int(__cdecl*)(int ItemID,int a2,int a3,int * a4,int * a5)) 0x7924D9)//CHECK OK
#define sub_698A05           ((void(__cdecl*)(int Struct,int a2,int a3,int  a4,GLfloat alpha,signed int a6,char a7,signed int a8,char a9,char a10,char a11,int a12,int a13)) 0x6988A2)//CHECK OK
//(int a1, int a2, int a3, int a4, GLfloat alpha, int a6, int a7, int a8, int a9, char a10, char a11, int a12, int a13)
//#define flt_7B3944          *(float*)0x7B3944
//#define flt_7B3AA8          *(float*)0x7B3AA8
//#define dword_587E610        *(DWORD*)0x587E610
#define pAddEffect			((int(__cdecl*)(int ItemID,float a2,int a3)) 0x534E27) //OK
//#define pSetEffect			((int(__cdecl*)(int ItemID,float a2,float a3,float a4,float a5,int a6,int a7,float a8,float a9)) 0x676734) //OK
//#define sub_6937C5         ((int(__cdecl*)(int ItemID,int a2,int a3,float  a4,int  a5,float a6,int a7)) 0x006937C5)//CHECK OK
//#define sub_5305AE         ((void(__cdecl*)(DWORD,DWORD,DWORD,int,int,DWORD,DWORD,DWORD,int,int,DWORD)) 0x005305AE)//CHECK OK
// ----------------------------------------------------------------------------------------------
#define MAX_WIN_WIDTH		640
#define MAX_WIN_HEIGHT		480
// ----------------------------------------------------------------------------------------------
#define pDrawTextColor			*(DWORD*)0x7D4C84
#define pDrawTextBackground		*(DWORD*)0x7D4C8C

#define pGameResolutionMode		*(int*)0x5877D58 //ok 100%
#define pGameLoad				((int(__cdecl*)()) 0x004DBE14) //ok
// ----------------------------------------------------------------------------------------------
///Interface
///////////////////////////Map
//#define pLoadMapNameImage        ((int(__cdecl*)(char * ItemID)) 0x414670) 
//#define pLoadMapName             ((int(__cdecl*)(int ItemID)) 0x6F8E60) 
 

// ----------------------------------------------------------------------------------------------
#define pDrawGUI                ((int(__cdecl*) (DWORD ItemID, int a2, float a3, float a4, float a5, float a6, float a7, float a8, float a10, char a11, char a12, float a13)) 0x006B97A7)//CHECK OK
//#define pDrawGUI			((void(__cdecl*)(DWORD, float, float, float, float))	0x006B97A7)//CHECK OK
///////////////////////////

#define pWinFontHeight			*(int*)0x78C5260
#define pWinWidth				*(GLsizei*)0x7E7F68
#define pWinHeight				*(GLsizei*)0x7E7F6C
#define pWinWidthReal			*(float*)0x587786C 
#define pWinHeightReal			*(float*)0x5877870

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define pMoveListInit			((int(__thiscall*)(void *)) 0x00467E3C)
//#define pMoveListInitEx			((void(__thiscall*)(LPVOID This, int Arg1, int Arg2)) 0x00467E3C)
//#define pMoveListSet			((int(__thiscall*)(void *)) 0x00468541)
#define sDrawToolTip			((int(__cdecl*)(int X, int Y, LPCSTR Text)) 0x005A0950)
//#define pCheckWindow			((bool(__thiscall*)(int Code)) 0x006F5BD6) // ok//791799//755F55
#define pCheckWindow			((bool(__stdcall*)(int Code))0x006F5BD6)

//#define pSetCursorFocus			*(DWORD*)0xE8CB3C
//#define pMoveListFuncUnk1        ((DWORD(__thiscall*)(int Unk1)) 0x6B8419)
//#define Unk1 			*(DWORD*)0x78CB090
//#define Unk2 			*(DWORD*)0x78CB090
//#define pMoveListFuncUnk2        ((int(__cdecl*)(char * unk0,int Unk1)) 0x4695D0)
//#define pMoveListFuncUnk3        ((void(__cdecl*)(float  unk0,float Unk1,int Unk2,int Unk3)) 0x6B98A4)

//#define sub_468E9B				((int(__cdecl*)()) 0x468E9B)

//#define sub_612950				((int(__cdecl*)()) 0x612950)

//#define OBJ_Login                    *(char*)0x0587C418 
//#define OBJ_Password                 *(char*)0x0790B820 
// ----------------------------------------------------------------------------------------------
#define pMakePreviewCharSet		((void(__cdecl*)(int Key, BYTE* Equipment, DWORD pCharacter, DWORD pHelper)) 0x0053FD2F)//0053FD2F
#define CreateBugSub			((void(__cdecl*)(int Type, DWORD Position, DWORD Owner, int SubType, int LinkBone)) 0x0045DC65)//0045DC65
#define CreateBug				((void(__cdecl*)(int Type, DWORD Position, DWORD Owner, int SubType, int LinkBone)) 0x0045DF87)//0045DF87
#define gTextDescription		((void(*)(DWORD,char*,...))0x00791B10)//00791B10//791B10
#define pViewportAddress		*(DWORD*)(0x0754F4BC)//754F4BC

//User
#define Color4f(r, b, g, o)			((o << 24) + (g << 16) + (b << 8) + r)


#define oUserObjectStruct		*(int*)0x077A8FD8 //0x77AEE08
#define oUserPreviewStruct      *(int*)0x754F4D0//754F4bC
#define pUserObjectStruct		&*(ObjectCharacter*)oUserObjectStruct


const int CallF = 0x6B97A7; // Done
const int CallS = 0x6B82D4; //Done
const int ImageLoad = 0x6D5B1B;
const int ImageLoadTGA = 0x006D606C;


///Items
//#define pSetItemEffect			((void(__cdecl*)(int Arg1, int ModelID, VAngle Angle, int Arg4, int ItemLevel, char Arg6, int Arg7, int Arg8, int Arg9)) 0x609E70)//BAD OFFSET
#define ItemConvert              ((void(__cdecl*)(ObjectItem * lpItem, BYTE Option, BYTE Special, BYTE Value)) 0x596A52)
 
////MonsterLoad
#define pMapNumber				*(int*)0x007D60C8 
//#define oNpcName				*(WORD*)0x77AEE10 //ok 
//#define oMonsterSetName                ((void(__cdecl*)(int MobName)) 0x791C00) //ok
//#define pShowMonster			((double(__cdecl*)(int MonsterID, int a2, int a3, int a4)) 0x540ACB) //ok
#define pActionSend			((int(__cdecl*)(int ViewPortId,int ActionId)) 0x00512F1A)  

#define pShowPrice				((int(*)(int a1, const char *a2, ...)) 0x791B10)
#define pSetItemPrice           ((__int64(__cdecl*)(ObjectItem * lpItem, int Mode)) 0x599217)
#define pSetItemTextLine		((void(__cdecl*)(int Line, LPSTR Text, ...)) 0x791B10)
#define pSetItemOptionText		((void(__cdecl*)(int ItemID, int Line, WORD OptionID, BYTE Arg4, int Arg5)) 0x614A00)

#define MAX_ITEMOPT_BIT				0x3F
#define GET_ITEMOPT_LEVEL(x)		((x >> 3) & 0xF)
#define GET_ITEMOPT0(x)				(x & 3)
#define GET_ITEMOPT16(x)			(x & 0x40)
#define GET_ITEMOPT_SKILL(x)		(x >> 7)
#define GET_ITEMOPT_LUCK(x)			((x >> 2) & 1)
#define GET_ITEMOPT_0(x)			(x & 1)
#define GET_ITEMOPT_1(x)			(((x & MAX_ITEMOPT_BIT) >> 1) & 1)
#define GET_ITEMOPT_2(x)			(((x & MAX_ITEMOPT_BIT) >> 2) & 1)
#define GET_ITEMOPT_3(x)			(((x & MAX_ITEMOPT_BIT) >> 3) & 1)
#define GET_ITEMOPT_4(x)			(((x & MAX_ITEMOPT_BIT) >> 4) & 1)
#define GET_ITEMOPT_5(x)			(((x & MAX_ITEMOPT_BIT) >> 5) & 1)

namespace eDrawText
{
	enum TextColor
	{
		Orange			    = 0xFF0096FF,	
		Red                 = 0xFF6A5AFF,
		InvisibleText       = 0x6A5AFF,
		Golden              = 0xFF000000,
		Blue                = 0xFFFFB27F,
		Blue2               = 0xFFFF9664,
		//WizardryDamage	    = 81,
	};
	enum TextBackground
	{
		Invisible			 = 0x64,		//
		Black                = 0xFF141414,
		Red1                  = 0xFF6A5AFF,

	};
};


enum eOptionType {
  AT_IMPROVE_DAMAGE = 0x50,
  AT_IMPROVE_MAGIC = 0x51,
  AT_IMPROVE_BLOCKING = 0x52,
  AT_IMPROVE_DEFENSE = 0x53,
  AT_LUCK = 0x54,
  AT_LIFE_REGENERATION = 0x55,
  AT_IMPROVE_LIFE = 0x56,
  AT_IMPROVE_MANA = 0x57,
  AT_DECREASE_DAMAGE = 0x58,
  AT_REFLECTION_DAMAGE = 0x59,
  AT_IMPROVE_BLOCKING_PERCENT = 0x5a,
  AT_IMPROVE_GAIN_GOLD = 0x5b,
  AT_EXCELLENT_DAMAGE = 0x5c,
  AT_IMPROVE_DAMAGE_LEVEL = 0x5d,
  AT_IMPROVE_DAMAGE_PERCENT = 0x5e,
  AT_IMPROVE_MAGIC_LEVEL = 0x5f,
  AT_IMPROVE_MAGIC_PERCENT = 0x60,
  AT_IMPROVE_ATTACK_SPEED = 0x61,
  AT_IMPROVE_GAIN_LIFE = 0x62,
  AT_IMPROVE_GAIN_MANA = 0x63,
  AT_PLUS_WING_MAXLIFE = 0x64,
  AT_PLUS_WING_MAXMANA = 0x65,
  AT_PLUS_ONE_PERCENT_DAMAGE = 0x66,
  AT_IMPROVE_AG_MAX = 0x67,
  AT_DAMAGE_ABSORB = 0x68,
  AT_PLUS_WING_ADD_LEADERSHIP = 0x69,
  AT_PLUS_FENRIR_INCREASE_LAST_DAMAGE = 0x6a,
  AT_PLUS_FENRIR_DECREASE_LAST_DAMAGE = 0x6b,
  AT_ATTACK1 = 0x78,
  AT_ATTACK2 = 0x79,
  AT_STAND1 = 0x7a,
  AT_STAND2 = 0x7b,
  AT_MOVE1 = 0x7c,
  AT_MOVE2 = 0x7d,
  AT_DAMAGE1 = 0x7e,
  AT_DIE1 = 0x7f,
  AT_SIT1 = 0x80,
  AT_POSE1 = 0x81,
  AT_HEALING1 = 0x82,
  AT_GREETING1 = 0x83,
  AT_GOODBYE1 = 0x84,
  AT_CLAP1 = 0x85,
  AT_GESTURE1 = 0x86,
  AT_DIRECTION1 = 0x87,
  AT_UNKNOWN1 = 0x88,
  AT_CRY1 = 0x89,
  AT_CHEER1 = 0x8a,
  AT_AWKWARD1 = 0x8b,
  AT_SEE1 = 0x8c,
  AT_WIN1 = 0x8d,
  AT_SMILE1 = 0x8e,
  AT_SLEEP1 = 0x8f,
  AT_COLD1 = 0x90,
  AT_AGAIN1 = 0x91,
  AT_RESPECT1 = 0x92,
  AT_SALUTE1 = 0x93,
  AT_RUSH1 = 0x94,
  AT_SET_RING_OPTION_ADD_SKILLDAMAGE = 0x95,
  AT_SET_OPTION_IMPROVE_STRENGTH = 0xa0,
  AT_SET_OPTION_IMPROVE_DEXTERITY = 0xa1,
  AT_SET_OPTION_IMPROVE_ENERGY = 0xa2,
  AT_SET_OPTION_IMPROVE_VITALITY = 0xa3,
  AT_SET_OPTION_IMPROVE_LEADERSHIP = 0xa4,
  AT_SET_OPTION_IMPROVE_MINATTACKDAMAGE = 0xa5,
  AT_SET_OPTION_IMPROVE_MAXATTACKDAMAGE = 0xa6,
  AT_SET_OPTION_IMPROVE_MAGICDAMAGE = 0xa7,
  AT_SET_OPTION_IMPROVE_DAMAGE = 0xa8,
  AT_SET_OPTION_IMPROVE_ATTACKRATE = 0xa9,
  AT_SET_OPTION_IMPROVE_DEFENCE = 0xaa,
  AT_SET_OPTION_IMPROVE_MAXLIFE = 0xab,
  AT_SET_OPTION_IMPROVE_MAXMANA = 0xac,
  AT_SET_OPTION_IMPROVE_MAXAG = 0xad,
  AT_SET_OPTION_IMPROVE_AG_VALUE = 0xae,
  AT_SET_OPTION_IMPROVE_CRITICALDAMAGE_SUCCESS = 0xaf,
  AT_SET_OPTION_IMPROVE_CRITICALDAMAGE = 0xb0,
  AT_SET_OPTION_IMPROVE_EX_DAMAGE_SUCCESS = 0xb1,
  AT_SET_OPTION_IMPROVE_EX_DAMAGE = 0xb2,
  AT_SET_OPTION_IMPROVE_SKILLDAMAGE = 0xb3,
  AT_SET_OPTION_DOUBLE_DAMAGE = 0xb4,
  AT_SET_OPTION_DEFENCE_IGNORE = 0xb5,
  AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE = 0xb6,
  AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE = 0xb7,
  AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_STR = 0xb8,
  AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_DEX = 0xb9,
  AT_SET_OPTION_IMPROVE_DEFENCE_WITH_DEX = 0xba,
  AT_SET_OPTION_IMPROVE_DEFENCE_WITH_VIT = 0xbb,
  AT_SET_OPTION_IMPROVE_MAGICDAMAGE_WITH_ENERGY = 0xbc,
  AT_SET_OPTION_ICE_MASTERY = 0xbd,
  AT_SET_OPTION_POSION_MASTERY = 0xbe,
  AT_SET_OPTION_THUNDER_MASTERY = 0xbf,
  AT_SET_OPTION_FIRE_MASTERY = 0xc0,
  AT_SET_OPTION_EARTH_MASTERY = 0xc1,
  AT_SET_OPTION_WIND_MASTERY = 0xc2,
  AT_SET_OPTION_WATER_MASTERY = 0xc3,
  AT_SET_IMPROVE_STRENGTH = 0xc4,
  AT_SET_IMPROVE_DEXTERITY = 0xc5,
  AT_SET_IMPROVE_ENERGY = 0xc6,
  AT_SET_IMPROVE_VITALITY = 0xc7,
};
namespace eItemOption
{
	enum T
	{
		Damage			    = 80,		//-> Additional Damage +<N>
		WizardryDamage	    = 81,
		Blocking    	    = 82,
		Defense			    = 83,		//-> Additional Defense +<N>
		Luck			    = 84,		//-> Default: 0                  
		HPRecovery          = 85,		//-> Default: 0                 
		LifeIncrease	    = 86,		
		ManaIncrease	    = 87,		//-> Increase Max HP +4%%
		DamageDecrease	    = 88,		//-> Increase Max Mana +4%%
		Reflect			    = 89,		//-> Damage Decrease +4%%
		DefenseSuccess	    = 90,		//-> Reflect Damage +5%%
		IncreaseZen		    = 91,		//-> Defense success rate +10%%
		// ----
		ExcellentDamage	    = 92,		//-> Increases acquisition rate of Zen after hunting monsters +30%%
		DamageByLevel	    = 93,		//-> Excellent Damage rate +10%%
		IncreaseDamage	    = 94,		//-> Increase Damage +level/20
		IncreaseWizardry    = 95,		//-> Increase Damage +%d%%
		WizardrySucces      = 96,		//-> Increase Wizardry Dmg +level/20
		IncreaseSpeed	    = 97,		//-> Increase Wizardry Dmg +2%
		IgnorOpDefense	    = 98,		//-> Increase Attacking(Wizardry)speed +%d
		IgnorOpDefenseMana	= 99,		//-> Increases acquisition rate of Life after hunting monsters +life/8
		ManaPerMob			= 100,		//-> Increases acquisition rate of Mana after hunting monsters +Mana/8
		IgnorOpDefense1		= 103,
		ReturnAttack	    = 105,		//-> Return's the enemy's attack power in <N>%
		CompleteLife	    = 106,		//-> Complete recovery of life in <N>% rate
		CompleteMana	    = 107,		//-> Complete recovery of Mana in <N>% rate

		//Damage				= 80,		//-> Additional Damage +<N>
		//WizardryDamage		= 81,
		//CurseSpell			= 82,
		//Defense				= 84,		//-> Additional Defense +<N>
		//Luck				= 85,		//-> Default: 0
		//HPRecovery			= 86,
		//// ----
		//LifeIncrease		= 87,		//-> Increase Max HP +4%%
		//ManaIncrease		= 88,		//-> Increase Max Mana +4%%
		//DamageDecrease		= 89,		//-> Damage Decrease +4%%
		//Reflect				= 90,		//-> Reflect Damage +5%%
		//DefenseSuccess		= 91,		//-> Defense success rate +10%%
		//IncreaseZen			= 92,		//-> Increases acquisition rate of Zen after hunting monsters +30%%
		//// ----
		//ExcellentDamage		= 93,		//-> Excellent Damage rate +10%%
		//DamageByLevel		= 94,		//-> Increase Damage +level/20
		//IncreaseDamage		= 95,		//-> Increase Damage +%d%%
		//IncreaseWizardry	= 96,		//-> Increase Wizardry Dmg +level/20
		//IncreaseWizardryDmg = 97,		//-> Increase Wizardry Dmg +2%
		//IncreaseSpeed		= 98,		//-> Increase Attacking(Wizardry)speed +%d
		//LifePerMob			= 99,		//-> Increases acquisition rate of Life after hunting monsters +life/8
		//ManaPerMob			= 100,		//-> Increases acquisition rate of Mana after hunting monsters +Mana/8
		//IgnorOpDefense		= 103,
		//ReturnAttack		= 106,		//-> Return's the enemy's attack power in <N>%
		//CompleteLife		= 107,		//-> Complete recovery of life in <N>% rate
		//CompleteMana		= 108,		//-> Complete recovery of Mana in <N>% rate
	};
};


//---------------------------------------------------------------------------
enum eObjectType	//-> Complete
{
	emPlayer	= 1,
	emMonster	= 2,
	emNPC		= 4,
	emNone		= 8,
};

//---------------------------------------------------------------------------

enum ObjClass // -> Complete -> Season 5.2 JPN
{
	DarkWizard 		= 0,
	SoulMaster 		= 8,
	GrandMaster		= 24,
	// ----
	DarkKnight		= 1,
	BladeKnight		= 9,
	BladeMaster		= 25,
	// ----
	Elf				= 2,
	MuseElf			= 10,
	HightElf		= 26,
	// ----
	MagicGladiator	= 3,
	DuelMaster		= 19,
	// ----
	DarkLord		= 4,
	LordEmperor		= 20,
};
//---------------------------------------------------------------------------

enum eEffectState
{
	AtDarkHorse		= 1,
	AtFenrir		= 4,
	AtGameMaster	= 5,
};

//---------------------------------------------------------------------------

enum eMauntType
{
	DarkHorse		= 210,  //OK
	Uniria			= 217,  //OK
	Dinorant		= 302,  //OK
	BlackFenrir		= 360,  //OK
	RedFenrir		= 361,  //OK
	BlueFenrir		= 362,  //OK
	GoldenFenrir	= 363,  //OK
#ifdef NEW_FENRIR
	WhiteFenrir     = 359,  //OK
#endif
};
//---------------------------------------------------------------------------


#define pVisible(x)						*(BYTE*)(x + 1059) //ok
#define pPlayerClass(x)					*(BYTE*)(x + 524) //ok
//#define pAncientEffect(x)				*(BYTE*)(x + 22)
//#define pAtEffect(x)					*(BYTE*)(x + 23)
//#define pHelmSlot(x)					*(WORD*)(x + 232)
//#define pHelmSetLevel(x)				*(BYTE*)(x + 234)
//#define pHelmSetExcellent(x)			*(BYTE*)(x + 235)
//#define pHelmSetAncient(x)				*(BYTE*)(x + 236)
//#define pArmorSlot(x)					*(WORD*)(x + 264)
//#define pArmorSetLevel(x)				*(BYTE*)(x + 266)
//#define pArmorSetExcellent(x)			*(BYTE*)(x + 267)
//#define pArmorSetAncient(x)				*(BYTE*)(x + 268)
//#define pPantsSlot(x)					*(WORD*)(x + 296)
//#define pPantsSetLevel(x)				*(BYTE*)(x + 298)
//#define pPantsSetExcellent(x)			*(BYTE*)(x + 299)
//#define pPantsSetAncient(x)				*(BYTE*)(x + 300)
//#define pGlovesSlot(x)					*(WORD*)(x + 328)
//#define pGlovesSetLevel(x)				*(BYTE*)(x + 330)
//#define pGlovesSetExcellent(x)			*(BYTE*)(x + 331)
//#define pGlovesSetAncient(x)			*(BYTE*)(x + 332)
//#define pBootsSlot(x)					*(WORD*)(x + 360)
//#define pBootsSetLevel(x)				*(BYTE*)(x + 362)
//#define pBootsSetExcellent(x)			*(BYTE*)(x + 363)
//#define pBootsSetAncient(x)				*(BYTE*)(x + 364)
//#define pWeaponFirstSlot(x)				*(WORD*)(x + 392)
//#define pWeaponFirstSetLevel(x)			*(BYTE*)(x + 394)
//#define pWeaponFirstSetExcellent(x)		*(BYTE*)(x + 395)
//#define pWeaponFirstSetAncient(x)		*(BYTE*)(x + 396)
//#define pWeaponSecondSlot(x)			*(WORD*)(x + 424)
//#define pWeaponSecondSetLevel(x)		*(BYTE*)(x + 426)
//#define pWeaponSecondSetExcellent(x)	*(BYTE*)(x + 427)
//#define pWeaponSecondSetAncient(x)		*(BYTE*)(x + 428)
//#define pWingsSlot(x)					*(WORD*)(x + 456)
//#define pSetUnknown1(x)					*(BYTE*)(x + 458)
//#define pSetUnknown2(x)					*(BYTE*)(x + 459)
//#define pSetUnknown3(x)					*(BYTE*)(x + 460)
#define pPetSlot(x)						*(WORD*)(x + 488)//??
//#define pSetUnknown4(x)					*(BYTE*)(x + 490)
//#define pFenrirCharSet(x)				*(BYTE*)(x + 491)
//#define pExpectPet(x)					*(BYTE*)(x + 492)
//#define pObjAction(x)					*(BYTE*)(x + 718)
//#define pObjScale(x)					*(DWORD*)(x + 796)
//#define pObjSelectZone(x)				*(float*)(x + 924)
//#define pGetPosition(x)					*(WORD*)(x + 952)



#define LODWORD(h)				((DWORD)(__int64(h) & __int64(0xffffffff)))
// ----
#define CS_START				1156 //ok
// ----
#define CS_EMPTY_SLOT			-1
#define CS_EMPTY_ITEM			511
#define CS_EMPTY_WEAPON			4095 //4095-old // ITEM(7, 511)
// ----
#define	CS_SET_1(x)				( x >> 2)
// ----8191
#define CS_HELM					8827 // 9297 for 5.4, need check, maybe it value from * on ITEM_INTER//OK!
#define CS_ARMOR				CS_HELM + 20
#define CS_PANTS				CS_ARMOR + 20
#define CS_GLOVES				CS_PANTS + 20
#define CS_BOOTS				CS_GLOVES + 20
// ----
#define CS_GET_STRUCT(x, y)		(CS_START * x + *(int*)y)
#define CS_GET_WEAPON(x, y)		( y | (16 * (x & 0xF0)) )
#define CS_GET_HELM(x, y, z)	( (32 * (z & 0xF)) + 16 * ((x >> 7) & 1) + (y >> 4) )
#define CS_GET_ARMOR(x, y, z)	( (32 * ((z >> 4) & 0xF)) + 16 * ((x >> 6) & 1) + (y & 0xF) )
#define CS_GET_PANTS(x, y, z)	( (32 * (z & 0xF)) + 16 * ((x >> 5) & 1) + (y >> 4) )
#define CS_GET_GLOVES(x, y, z)	( (32 * ((z >> 4) & 0xF)) + 16 * ((x >> 4) & 1) + (y & 0xF) )
#define CS_GET_BOOTS(x, y, z)	( (32 * (z & 0xF)) + 16 * ((x >> 3) & 1) + (y >> 4) )
#pragma pack(push, 1)
struct VAngle
{
    float X;
    float Y;
    float Z;
};

#pragma pack(pop)

#pragma pack(push, 1)
struct ObjectModel_424
{
	float Unknown0;
	float Unknown4;
	float Unknown8;
	float Unknown12;
	float Unknown16;
	float Unknown20;
	float Unknown24;
	float Unknown28;
	float Unknown32;
	float Unknown36;
	float Unknown40;
	float Unknown44;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Character_Structure
{
	float Unknown0;
	float Unknown4;
	float Unknown8;
	float Unknown12;
	float Unknown16;
	float Unknown20;
	float Unknown24;
	float Unknown28;
	float Unknown32;
	float Unknown36;
	float Unknown40;
	float Unknown44;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct UnknownStruct0
{
	/*+0*/	WORD	Unknown0;//?
	/*+2*/	WORD	Unknown2;//?
	/*+4*/	WORD	Unknown4;//?
	/*+6*/	WORD	Unknown6;
	/*+8*/	WORD	Unknown8;
	/*+10*/	WORD	Unknown10;
	/*+12*/	WORD	Unknown12;//?
	/*+14*/	WORD	Unknown14;
	/*+16*/	WORD	Unknown16;
};
#pragma pack(pop)
#pragma pack(push, 1)
typedef struct	//-> Complete (size: 107)
{
/*+0*/		WORD	m_Type;   //OK
BYTE	Shift1[2];
/*+4*/	   DWORD	m_Level;  //OK
/*+8*/		BYTE	Unknown8; //OK
BYTE	Shift2[1];
/*+10*/		BYTE	Unknown10; //OK
BYTE	Shift3[1];
/*+12*/		WORD	DamageMin; //OK
/*+14*/		WORD	DamageMax; //OK
/*+16*/		BYTE	Unknown16; //OK
BYTE	Shift4[1];
/*+18*/		WORD	Defense;   //OK
/*+20*/		WORD	MagicDefense; //OK
/*+22*/		BYTE	Unknown22;  //OK
/*+23*/		BYTE	Unknown23;  //OK
/*+24*/		WORD	Unknown24; //OK
/*+26*/	    BYTE	Unknown26; //OK
/*+27*/	    BYTE	FenrirCode; //OK ??? or excellent opt??
/*+28*/	    BYTE	Unknown28; //OK
/*+29*/	    BYTE	Unknown29; //OK
/*+30*/		WORD	ReqStrenght; //OK
/*+32*/		WORD	ReqDextirity; //OK
/*+34*/		WORD	ReqEnergy; //OK
/*+36*/		WORD	ReqVitality; //OK
/*+38*/		WORD	ReqCommand; //OK
/*+40*/		WORD	ReqLevel; //OK
/*+42*/		BYTE	SpecialCount; //OK
/*+43*/		BYTE	SpecialType[8];  //OK
/*+51*/		BYTE	SpecialValue[8]; //OK


///*+59*/		BYTE	Unknown59; //OK
///*+60*/	   DWORD	Unknown60; //OK
///*+62*/		BYTE	Unknown62; //OK
///*+63*/		BYTE	Unknown63; //OK
///*+64*/		BYTE	Unknown64; //OK
///*+65*/		BYTE	Unknown65; //OK
///*+66*/		BYTE	Unknown66;
///*+67*/		BYTE	Unknown67;
///*+68*/		BYTE	Unknown68;
///*+69*/		BYTE	Unknown69;
///*+70*/		WORD	Unknown70;
///*+72*/		WORD	Unknown72;
///*+74*/		BYTE	Unknown74;


///SIZE 124
} ObjectItem, * lpItemObj;
#pragma pack(pop)

//---------------------------------------------------------------------------

//#pragma pack(push, 1)
////struct ObjectCharacter	
//typedef struct//struct ObjectCharacter	//-> ~InDev (size: 6012) [77AEE08 | ??]
//{
///*+0*/		char	Name[11];
///*+11*/		char	Class;
///*+12*/		BYTE	Unknown12;
///*+13*/		BYTE	Unknown13;
///*+14*/		short	Level;
//WORD Resets;
///*+16*/		DWORD	Experience;
///*+20*/		DWORD	NextExperience;
///*+24*/		short	Strength;
///*+26*/		short	Dexterity;
///*+28*/		short	Vitality;
///*+30*/		short	Energy;
///*+32*/		short	Leadership;
///*+34*/		WORD	Life;	//22
///*+36*/		WORD	Mana;	//24
///*+38*/		WORD	MaxLife;	//26
///*+40*/		WORD	MaxMana;	//28
///*+42*/		WORD	Shield;		//2A
///*+44*/		WORD	MaxShield;	//2C
///*+46*/		WORD	AttackRate;	//2E
///*+48*/		WORD	DefenseRate;
///*+50*/		short	AddStrength;
///*+52*/		short	AddDexterity;
///*+54*/		short	AddVitality;
///*+56*/		short	AddEnergy;
//BYTE gap01[4];
///*+62*/		WORD	AddLeadership;	//GS use unsigned value...
///*+64*/		WORD	Stamina;
///*+66*/		WORD	MaxStamina;
///*+68*/		BYTE	ItemSpecialUseFlag;		//Bit decomposit (1, 2, 8)
//BYTE UnknownGap;
///*+70*/		WORD	ItemSpecialUseTime[3];	//[ ]
///*+76*/		WORD	AddPoint;	//FruitStat start
///*+78*/		WORD	MaxAddPoint;
///*+80*/		WORD	MinusPoint;
///*+82*/		WORD	MaxMinusPoint;	//FruitStat end
///*+84*/		WORD	AttackSpeed;
///*+86*/		WORD	DamageRate;
///*+88*/		WORD	DamageMin;
///*+90*/		WORD	DamageMax;
///*+92*/		WORD	Unknown92;
///*+94*/		WORD	Unknown94;
///*+96*/		WORD	MagicSpeed;	//Test
//BYTE gap02[10];
///*+108*/	WORD	Unknown108;
///*+110*/	WORD	Unknown110;
//BYTE gap03[4];
///*+116*/	WORD	LevelPoint;
///*+118*/	BYTE	MagicCount;
///*+119*/	BYTE	Unknown119;
///*+120*/	WORD	pMagicList[650];	//maybe 150?	//Original 650
///*1420*/
//BYTE gap04[3252];
///*+4672*/	ObjectItem pEquipment[12];
///*+5956*/	DWORD	MoneyInventory;	//(C4)
///*+5960*/	DWORD	MoneyWarehouse;	//(C4)
//BYTE gap05[8];
///*+5972*/	UnknownStruct0 Unknown5972;
//BYTE gap06[2];
///*+5992*/	WORD	Unknown5992;
///*+5994*/	WORD	Unknown5994;
//BYTE gap07[2];
///*+5998*/	WORD	Unknown5998;
///*+6000*/	WORD	Unknown6000;
///*+6002*/	WORD	Unknown6002;
///*+6004*/	WORD	Unknown6004;
///*+6006*/	WORD	Unknown6006;
///*+6008*/	BYTE	Unknown6008;
///*+6009*/	BYTE	Unknown6009;
///*+6010*/	BYTE	Unknown6010;
///*+6011*/	BYTE	Unknown6011;
//}ObjectCharacter, * lpCharObj;//;
//#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
//#pragma pack(push, 1)
typedef struct	//-> ~InDev (size: 6012) [77AEE08 | ??]
{
/*+0*/		char	Name[11];
/*+11*/		BYTE	Class; //OK
/*+12*/		BYTE	Unknown12; 
/*+13*/		BYTE	Unknown13;
/*+14*/		short	Level;  //OK
/*+16*/		DWORD	Experience; //OK
/*+20*/		DWORD	NextExperience; //OK
/*+24*/		short	Strength;
/*+26*/		short	Dexterity;
/*+28*/		short	Vitality;
/*+30*/		short	Energy;
/*+32*/		short	Leadership;
/*+34*/		WORD	Life;
/*+36*/		WORD	Mana;
/*+38*/		WORD	MaxLife;
/*+40*/		WORD	MaxMana;
/*+42*/		WORD	Shield;
/*+44*/		WORD	MaxShield;
/*+46*/		WORD	AttackRate;
/*+48*/		WORD	DefenseRate;
/*+50*/		short	AddStrength;
/*+52*/		short	AddDexterity;
/*+54*/		short	AddVitality;
/*+56*/		short	AddEnergy;
			short	Resets;
BYTE gap01[4];
/*+62*/		WORD	AddLeadership;	//GS use unsigned value...
/*+64*/		WORD	Stamina;
/*+66*/		WORD	MaxStamina;
/*+68*/		BYTE	ItemSpecialUseFlag;		//Bit decomposit (1, 2, 8)
BYTE UnknownGap;
/*+70*/		WORD	ItemSpecialUseTime[3];	//[ ]
///*+74*/		WORD	Unknown74;	//[ ]???
/*+76*/		WORD	AddPoint;	//FruitStat start
/*+78*/		WORD	MaxAddPoint;
/*+80*/		WORD	MinusPoint;
/*+82*/		WORD	MaxMinusPoint;	//FruitStat end
/*+84*/		WORD	AttackSpeed;
/*+86*/		WORD	DamageRate;
/*+88*/		WORD	DamageMin;
/*+90*/		WORD	DamageMax;
/*+92*/		WORD	Unknown92;
/*+94*/		WORD	Unknown94;
/*+96*/		WORD	Unknown96;
BYTE gap02[10];
///*+104*/	WORD	Unknown104;
///*+106*/	WORD	Unknown106;
/*+108*/	WORD	Unknown108;
/*+110*/	WORD	Unknown110;
/*+112*/	WORD	Unknown112;
/*+114*/	BYTE	Unknown114;
/*+115*/	BYTE	Unknown115;
/*+116*/	WORD	LevelPoint;  //BYTE?
/*+118*/	BYTE	MagicCount;
/*+119*/	BYTE	Unknown119;
/*+120*/	WORD	pMagicList[650];	//maybe 150?
/*1420*/
///*+628*/	BYTE	Unknown628;
BYTE gap04[3252];
/*+4672*/	ObjectItem pEquipment[12];
/*+5956*/	DWORD	MoneyInventory;	//(C4)
/*+5960*/	DWORD	MoneyWarehouse;	//(C4)
BYTE gap05[8];
/*+5972*/	UnknownStruct0 Unknown5972;
BYTE gap06[2];
/*+5992*/	WORD	Unknown5992;
/*+5994*/	WORD	Unknown5994;
BYTE gap07[2];
/*+5998*/	WORD	Unknown5998;
/*+6000*/	WORD	Unknown6000;
/*+6002*/	WORD	Unknown6002;
/*+6004*/	WORD	Unknown6004;
/*+6006*/	WORD	Unknown6006;
/*+6008*/	BYTE	Unknown6008;
/*+6009*/	BYTE	Unknown6009;
/*+6010*/	BYTE	Unknown6010;
/*+6011*/	BYTE	Unknown6011;
}ObjectCharacter, * lpCharObj;
//#pragma pack(pop)
/////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
struct ObjectModel	//648? 848!!
{
BYTE gap00[4];
/*+4*/	BYTE	Unknown4;
BYTE gap01[4];
/*+9*/	BYTE	Unknown9;
BYTE gap02[2];
/*+12*/	BYTE	Unknown12;
BYTE gap022[2];
/*+15*/	BYTE	Unknown15;
BYTE gap03[2];
/*+18*/	WORD	AnimationID;
/*+20*/	WORD	Unknown20;
/*+22*/	BYTE	Unknown22;
/*+23*/	BYTE	Unknown23;
/*+24*/	BYTE	ObjectType;
BYTE gap04[13];
/*+38*/	WORD	Unknown38;
BYTE gap05[8];
/*+48*/	DWORD	Unknown48;
BYTE gap06[44];
/*+96*/	float	Scale;
BYTE gap07[16];
/*+116*/	float	Unknown116;
BYTE gap08[20];
/*+140*/	float	Unknown140;
/*+144*/	float	Unknown144;
BYTE gap09[56];
/*+204*/	VAngle	Unknown204;
/*+216*/	VAngle	Unknown216;
BYTE gap10[24];
/*+252*/	float	VecPosX;
/*+256*/	float	VecPosY;
/*+260*/	float	VecPosZ;
/*+252*/	VAngle	VecPos;
/*+264*/	VAngle	Angle;
/*+264*/	float	Unknown264;
/*+268*/	float	Unknown268;
/*+272*/	float	Unknown272;
BYTE gap11[148];
/*+424*/	ObjectModel_424 Unknown424;
BYTE gap12[176];
/*+484*/
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------
//SIZE 1156
//#pragma pack(push, 1)
typedef struct
{
/*0*/   WORD    Live; //??
/*2*/	WORD	MauntType;      //??           
/*4*/	DWORD	Unknown4;      //??     
/*8*/	BYTE	Unknown8;
BYTE	Shift2[2];
/*12*/	DWORD	ModelScale;              
/*16*/	float	VecPosX;           
/*20*/	float	VecPosY;              
/*24*/	float	VecPosZ;    
///*28*/	DWORD	Unknow28;
///*32*/	DWORD	Unknow32;
/*+32*/        BYTE    PKLevel;
///*36*/	DWORD	Unknow36;
///*38*/	WORD	Unknow38;   //!!!!!!!!!!
BYTE	Shift33[53];  
/*82*/	WORD	UnknowTest;             
BYTE	Shift3[103];  
///*+126*/    WORD    aIndex;
///*148*/	DWORD	Unknow148;
///*152*/	DWORD	Unknow152;
///*168*/	DWORD	Unknow168;  
///*176*/	DWORD	Unknow176;  //!!!!!!!!!
/*188*/ BYTE  TypeName;  
/*189*/ BYTE  Type; //?? 
/*190*/ WORD  Unknow190;   //??
///*204*/	BYTE	Unknow204;
///*296*/	DWORD	Unknow296;  !!
///*300*/	DWORD	Unknow300;  !!
///*304*/	DWORD	Unknow304;   !!
BYTE	Shift113[124];  
/*316*/	DWORD	Unknow316; //??
BYTE	Shift1113[5];
/*325*/	BYTE	Action; 
BYTE	Shift114[2];  
/*328*/	DWORD	Unknow328; //??
///*332*/	DWORD	Unknow332;
BYTE	Shift4[32];
/*364*/	float	SelectZone;   
BYTE	Shift44[48];
///*416*/ BYTE    Unknown416; //!!!!!!!!
///*448*/ BYTE    Unknown448; //!!!!!!!!
///*452*/ DWORD   Unknown452; //!!!!!!!!
/*416*/	BYTE	Unknown416;
BYTE	Shift412[48];
/*464*/	BYTE	Unknown464;   
BYTE	Shift5[38];     
///*500*/	DWORD	Unknown500;  
/*504*/	WORD    Unknown504;  
BYTE	Shift51[18]; 
/*524*/	BYTE	Class;   
///*525*/	BYTE	Unknow525;      !!!!
///*526*/	WORD	Unknown526;   //!!!!!!!!
BYTE	Shift512[4]; 
///*528*/	BYTE	Unknow28;
/*529*/	char	Name[25];  
BYTE	Shift612[8];
/*562*/	WORD	Unknow562;// ???
/*564*/	WORD	VPIndex;
BYTE	Shift6[34];
/*600*/	WORD	HelmSlot;				 
/*602*/	BYTE	HelmLevel;				
/*603*/	BYTE	HelmExcellent;		   
/*604*/	BYTE	HelmAncient;	
///*605*/ BYTE   Unknown605;         //!!!!!!!!!??
BYTE	Shift7[27];
/*632*/	WORD	ArmorSlot;		   
/*634*/	BYTE	ArmorLevel;		    
/*635*/	BYTE	ArmorExcellent;		   
/*636*/	BYTE	ArmorAncient;   		
///*637*/ BYTE   Unknown637;         //!!!!!!!!!??
BYTE	Shift8[27];
/*664*/	WORD	PantsSlot;   		   
/*666*/	BYTE	PantsLevel;   		  
/*667*/	BYTE	PantsExcellent;   	
/*668*/	BYTE	PantsAncient;  
///*669*/ BYTE   Unknown669;         //!!!!!!!!!??
BYTE	Shift9[27];
/*696*/	WORD	GlovesSlot;   		   
/*698*/	BYTE	GlovesLevel;   		  
/*699*/	BYTE	GlovesExcellent;   		
/*700*/	BYTE	GlovesAncient;   
///*701*/ BYTE   Unknown701;         //!!!!!!!!!??
BYTE	Shift10[27];
/*728*/	WORD	BootsSlot;   		    
/*730*/	BYTE	BootsLevel;   		     
/*731*/	BYTE	BootsExcellent;   		
/*732*/	BYTE	BootsAncient;  
///*733*/ BYTE   Unknown733;         //!!!!!!!!!??
BYTE	Shift11[27];
/*760*/	WORD	WeaponFirstSlot;		
/*762*/	BYTE	WeaponFirstLevel;		
/*763*/	BYTE	WeaponFirstExcellent;	
/*764*/	BYTE	WeaponFirstAncient;  
///*765*/ BYTE   Unknown765;         //!!!!!!!!!??
BYTE	Shift12[27];
/*792*/	WORD	WeaponSecondSlot;		
/*794*/	BYTE	WeaponSecondLevel;	
/*795*/	BYTE	WeaponSecondExcellent;   
/*796*/	BYTE	WeaponSecondAncient;     
///*797*/ BYTE   Unknown797;         //!!!!!!!!!
BYTE	Shift13[27];
/*824*/	WORD	WingsSlot;              
/*826*/ BYTE	WingsLevel;              
/*827*/	BYTE	WingsExcellent;           
/*828*/	BYTE	WingsAncient; 
///*829*/ BYTE   Unknown829;         //!!!!!!!!!
///*832*/ DWORD   Unknown832;         //!!!!!!!!!
BYTE	Shift14[27];
/*856*/	WORD	PetSlot;                
/*858*/ BYTE	Unknown858;            
/*859*/ BYTE	FenrirCharSet;          
/*860*/ BYTE	ExpectPet;      
///*861*/ BYTE   Unknown861;         //!!!!!!!!!
BYTE	Shift15[59];
/*920*/	BYTE	AncientEffect; 
///*924*/ DWORD   Unknown924;         //!!!!!!!!!
BYTE	Shift16[7];
/*928*/	BYTE	StatusEffect;   
BYTE	Shift117[21];
///*948*/ BYTE   Unknown948;         //!!!!!!!!!
///*949*/ BYTE   Unknown949;         //!!!!!!!!!
/*950*/	WORD	m_Change; 
BYTE	Shift145[28];
///*969*/ BYTE   Unknown969;         //!!!!!!!!!
/*980*/	DWORD	Unknown980;  
BYTE	Shift197[12];
/*996*/ WORD	Unknown996;  
BYTE	Shift17[61];
///*1014*/ DWORD   Unknown1014;     //!!!!!!!!!!!!!
///*1016*/ DWORD   Unknown1016;     //!!!!!!!!!!!!!
///*1020*/ DWORD   Unknown1020;     //!!!!!!!!!!!!!
///*1058*/ BYTE   Unknown1058;     //!!!!!!!!!!!!!
/*1059*/ BYTE   Visible;  
///*1060*/	BYTE	Unknown1060;   //!!!!!!!!!!!!!
///*1116*/ DWORD	Unknow1116; 
///*1120*/ DWORD	Unknow1120;  
///*1144*/ DWORD   Unknown1144;    //!!!!!!!!!!!
/*+776*/    ObjectModel    m_Model;
/*+1424*/    BYTE    Unknown1424;    //maybe gens rank
BYTE gap23[3];
/*+1428*/    DWORD    Unknown1428;    //-> end
}ObjectPreview, * lpViewObj;
//#pragma pack(pop)
//#pragma pack(push, 1)
//struct struct_a1
//{
//  _BYTE f0[2];
//  _WORD word2;
//  _DWORD dword4;
//  _BYTE f8[440];
//  _BYTE byte1C0;
//  _BYTE f1C1[3];
//  _DWORD dword1C4;
//  _BYTE f1C8[68];
//  _BYTE byte20C;
//  _BYTE f20D[1];
//  _WORD word20E;
//  _BYTE f210[34];
//  _WORD word232;
//  _WORD word234;
//  _BYTE f236[66];
//  _WORD word278;
//  _BYTE f27A[94];
//  _WORD word2D8;
//  _BYTE f2DA[30];
//  _WORD word2F8;
//  _BYTE f2FA[3];
//  _BYTE byte2FD;
//  _BYTE f2FE[26];
//  _WORD word318;
//  _BYTE f31A[3];
//  _BYTE byte31D;
//  _BYTE f31E[26];
//  _WORD word338;
//  _BYTE f33A[3];
//  _BYTE byte33D;
//  _BYTE f33E[2];
//  _DWORD dword340;
//  _BYTE f344[20];
//  _WORD word358;
//  _BYTE byte35A;
//  _BYTE f35B[2];
//  _BYTE byte35D;
//  _BYTE f35E[58];
//  _BYTE byte398;
//  _BYTE f399[3];
//  _DWORD dword39C;
//  _BYTE byte3A0;
//  _BYTE f3A1[20];
//  _BYTE byte3B5;
//  _WORD word3B6;
//  _BYTE f3B8[17];
//  _BYTE byte3C9;
//  _BYTE f3CA[26];
//  _WORD word3E4;
//  _BYTE f3E6[14];
//  _DWORD dword3F4;
//  _DWORD dword3F8;
//  _DWORD dword3FC;
//  _BYTE f400[34];
//  _BYTE byte422;
//  _BYTE byte423;
//  _BYTE byte424;
//  _BYTE f425[83];
//  _DWORD dword478;
//};
//#pragma pack(pop)
// ----------------------------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct  //-> Complete (size: 27)
{
	WORD	ObjectID;
	char	Name[25];
} NpcNameList, * lpNpcNameList;
#pragma pack(pop)
//---------------------------------------------------------------------------
struct VpMonsterInfo
{
  int VPIndex;
  float life;
  float MaxLife;
  char Name;
  short Resets;
};
//---------------------------------------------------------------------------
struct VpPropensityInfo
{
  int VPIndex;
  int Number;
};
typedef VpMonsterInfo * VPOBJ;
extern VpMonsterInfo VpObj[65535];
typedef VpPropensityInfo * VPPROPOBJ;
extern VpPropensityInfo VpPropObj[65535];


struct CameraStruct 
{
		float Zoom;
		float RotY;
		float RotZ;
		float PosZ;
		float ClipX;
		float ClipY;
		float GlClip;
		int MouseX;
		int MouseY;
		bool InitCamera;
		bool MoveCamera;
};extern CameraStruct Camera;

//struct ServerTime
//{
//int Hour;
//int Minute;
//int Second;
//};