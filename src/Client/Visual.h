#include "stdafx.h"
#include "Protocol.h"
#pragma once

extern DWORD MovHP_OffSet; //
extern DWORD MovMana_OffSet;
extern DWORD MovStamina_OffSet;
extern DWORD MovShield_OffSet;
extern DWORD ShieldCurrentShow;
extern DWORD StaminaCurrentShow;
extern DWORD HPCurrentShow;
extern DWORD ManaCurrentShow;
extern DWORD HPMaxShow;
extern DWORD ManaMaxShow;
extern DWORD ShieldMaxShow;
extern DWORD StaminaMaxShow;
#define pDrawCircle  ((void(__cdecl*)(DWORD ModelID, float Arg2, float Arg3, float Arg4, float Arg5, float Arg6, float Arg7, float Arg8, float Arg9, bool Arg10, bool Arg11, float Arg12)) 0x6B97A7)


void cLoadLifeManaFix();
void cHookHP_Show();
void TestLoad();
void GCGetRecupationCharacter(PMSG_SEND_CHARACTER_INFO * gObj);
int CurrentAttributShow(int Show_T);
void LoadLifeShow();
void LoadManaShow();
void LoadStaminaShow();
void LoadShieldShow();
int GetPercent(int MaxState,int State);
void DrawCircle(DWORD ModelID, float Arg2, float Arg3, float Arg4, float Arg5, float Arg6, float Arg7, float Arg8, float Arg9, int Arg10, int Arg11, int Arg12);
//
class VisualState
{
public:
	void Load();
	void Index(PMSG_JOINRESULT* Data);
	void RecvDamage(PMSG_ATTACKRESULT * Data);
	void GCMonsterDamageRecv(PMSG_MONSTER_DAMAGE_RECV* Data);
	void RecvHP(PMSG_REFILL* Data);
	void RecvMP(PMSG_MANASEND* Data);
	void Respawn();
	void Killed(PMSG_DIEPLAYER* Data);
	static void DrawHP(float a1, float a2, int Value, float a4, float a5);
	static void DrawMP(float a1, float a2, int Value, float a4, float a5);
	static void DrawAG(float a1, float a2, int Value, float a4, float a5);
	static void DrawSD(float a1, float a2, int Value, float a4, float a5);
	static void DrawTabHP(int PosX, int PosY, LPCSTR lpString);
	static void DrawTabMP(int PosX, int PosY, LPCSTR lpString);
	static void DrawTabAG(int PosX, int PosY, LPCSTR lpString);
	static void DrawTabSD(int PosX, int PosY, LPCSTR lpString);
	static void InfoHP(int a1, int a2, LPCSTR a3, int a4, char a5, int a6);
	static void InfoMP(int a1, int a2, LPCSTR a3, int a4, char a5, int a6);
	static void CheckMP1();
	static void CheckMP2();
	static void CheckMP3();
	static void CheckMP4();
	static void CheckMP5();
	static void CheckMP6();
	static void CheckMP7();
	static void CheckBP1();
	static void CheckBP2();
	static void CheckBP3();
	static void CheckBP4();
	static void CheckBP5();
	static void CheckBP6();
	static void LevelBoxCreate(int a1, float a2, float a3, float a4, float a5, int a6, int a7, float a8, float a9, char a10, char a11, float a12);
	static void LevelBoxDraw(int a1, int a2, LPSTR a3, int a4, int a5, int a6);

	static void Work();
	static void Window();
	static void DrawTimer();

	DWORD server_timer;
	DWORD server_tick;

public:
	bool Kill;
	short aIndex;
	int Attack;
	int Life;
	int MaxLife;
	int Mana;
	int MaxMana;
	int Ag;
	int MaxAg;
	int Sd;
	int MaxSd;
	int ResetCount;
	char Buffer[6][50];
	int AttackHP;
	int AttackSD;


};



extern VisualState Visual;
