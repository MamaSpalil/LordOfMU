#pragma once
#include "stdafx.h"
void LogsAdded (char* Message,...);
#if ANTI_HACKex

#define MAX_DUMP_OFFSETS 3
#define MAX_DUMP_SIZE 32
#define MAX_PROCESS_DUMP 710	//708

//typedef struct ANITHACK_PROCDUMP
//{
//	unsigned int m_aOffset;
//	unsigned char m_aMemDump[MAX_DUMP_SIZE];
//	char Name[100];
//} *PANITHACK_PROCDUMP;
//
//extern ANITHACK_PROCDUMP g_ProcessesDumps[MAX_PROCESS_DUMP];

bool LoadFilesBase(char * cFile);



#define MAIN_SIZE 1178112
#define PLAYER_SIZE 2662002
#define wzexgah_SIZE 194048
#define Glow_SIZE 69632
#define MaEx_SIZE 65536
#define mulan_SIZE 98304
#define rus_SIZE 53760
#define tex_SIZE 61440

void SystemProcessesScan();
void DetectHide();
void TitleWindowMain();
bool ScanProcessMemory(HANDLE hProcess);
void TxtCheckWindow();
void XProtectionMain();
void ZProtectionMain();
void ClasseCheckWindow();
void SPerformance();
void DetectSpeed();
void DetectID();
void Heuristica2();
void HProtection();
void HeuristicW();
void Heuristicclass();
void Dll_Inject();
void AntiKill();
void AntiKill2();
void AntiKill3();
void Close_Class();
void Close_Window();
int	iGetFileSize(char* FileName);
bool iCheckFileSize(char* cFile,int Size);
bool AllCheckSizeFiles();
void CheckFilesDir(bool TypeStart);
void YProtectionMain();
void HideMain();
#endif