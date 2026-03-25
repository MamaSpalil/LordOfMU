#include "stdafx.h"
#include "Protocol.h"
#pragma once

#define MAX_CHARACTER 5

#define ShowWin										((int(__thiscall*)(int a1,int a2)) 0x006FB5D2)
#define Instance									((int(*)()) 0x006FAAE2)
#define sub_463272									((int(*)()) 0x006FAAE2)
#define PressKey									((char(__cdecl*)(int vKey)) 0x0059FA60)
#define sub_532400									((int(__cdecl*)(char a1)) 0x0054545B)
#define pTextThis									((LPVOID(*)()) 0x004928CA )//xz//65ae49
#define MouseXs										*(int*)0x07EB37E4
#define MouseYs										*(int*)0x07EB37E0



#define SelectedHero								*(int*)0x7E81BC// ILI 48
#define SelectedCharacter							*(int*)0x7D4C58



//#define pSetTextColor								((void(__thiscall*)(LPVOID This, BYTE r, BYTE g, BYTE b, BYTE h)) 0x0041FF35)
//#define pDrawText									((int(__thiscall*)(LPVOID This, int PosX, int PosY, LPCTSTR Text, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)) 0x00420024)
//#define CUIRenderText_SetFont						((int(__thiscall*)(LPVOID This, HGDIOBJ a2)) 0x0041FFF9)
//#define g_hFontBold									*(HGDIOBJ*)0x7AFEE98

#define RenderBitmap								((void(__cdecl*)(int a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, char a10, char a11, float a12)) 0x006B97A7)
#define BeginBitmap									((void(*)()) 0x6B95C4)
#define DisableAlphaBlend							((void(__cdecl*)()) 0x006B81BA)
#define EnableAlphaBlend							((void(__cdecl*)()) 0x006B82D4)
#define PlayBuffer									((signed int (__cdecl*)(int a1, int a2, int a3)) 0x004207F5)

void CButtonCharacterS13();
//
//struct INFO_NEWS
//{
//	PBMSG_HEAD2 h;
//	BYTE Result;
//	char News1[50];
//	char News2[50];
//	char News3[50];
//	char News4[50];
//	char News5[50];
//	char News6[50];
//	char News7[50];
//	char News8[50];
//	char News9[50];
//	char News10[50];
//};
//struct INFO_DATANEWS
//{
//	BYTE Result;
//	char News1[50];
//	char News2[50];
//	char News3[50];
//	char News4[50];
//	char News5[50];
//	char News6[50];
//	char News7[50];
//	char News8[50];
//	char News9[50];
//	char News10[50];
//};
//extern INFO_DATANEWS n_info;
//void NewsRecv(INFO_NEWS * aRecv);