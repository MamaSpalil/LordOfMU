#pragma once
//---------------------------------------------------------------------------

#include "targetver.h"
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <gl/gl.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <stdarg.h>
#include "ReadScript.h"
#include <tlhelp32.h>
#include <mmsystem.h>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <stdio.h>
#include <math.h>
#include <WindowsX.h>
#include <ShellAPI.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <tchar.h>
#include <map>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <direct.h>
#include <unordered_map>

#include <stdlib.h>
#include <direct.h>

#include <process.h>
#include <unordered_map>
#include <gl\gl.h>
#include <gl\glu.h>
#include <wingdi.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment( lib, "opengl32.lib" )  
#pragma comment( lib, "glu32.lib" )  
#pragma comment(lib, "detours2.lib")
#pragma comment(lib, "detoured.lib")
#include "zzzMathLib.h"
#include "resource.h"
//#include "detours.h"
//#pragma comment(lib,"detours.lib")
//---------------------------------------------------------------------------
extern BOOL MouseWellButtonDown;
extern BOOL MouseLeftButtonDown;
extern BOOL MouseRightButtonDown;

#define DEBUG


void APIENTRY glEnable_h(GLenum mode);



#define WIN32_LEAN_AND_MEAN
#define JAPAN_PROTOCOL
extern bool         TpList;
//---------------------------------------------------------------------------
#define MAIN_SERIAL			"rPVessXV1Xr2as11"//"rPVessXV1Xr2as11"
#define SERVER_ADRESS		"192.168.101.8"//"192.168.0.109"//"192.168.0.105"//"85.10.231.66"//"85.10.231.66"//"192.168.0.109"//192.168.1.101"//192.168.0.108"//"192.168.0.108"//192.168.88.247"//192.168.0.108"
#define MAIN_VERSION		"22556"
#define WINDOW_NAME			"Season 3 Final"
#define WINDOW_NAME_EXE		"Main.exe"
#define _LAUNCHER_		"Launcher"
#define ANTI_HACKex	1

//		MemorySet(0x00830BA6, 0x90, 0x05);
//		MemorySet(0x00832ED8, 0x90, 0x05);
//		MemorySet(0x0083279C, 0x90, 0x05);
//		MemorySet(0x008329A2, 0x90, 0x05);
//---------------------------------------------------------------------------
#define CASTLE_SKILL
#define DONT_SHOW_IP_IN_FILE
//#define MULTI_WAREHOUSE
#define ANTI_CHEAT_PLUS			TRUE
#define DEV_DAMAGE_TABLE		1
#define CUSTOM_WINGS 
//#define ENABLE_AUTOLOGIN		1
#define LOGIN			"1234"
#define PASSWORD		"1234"
//#define AUTO_LOGIN				
//#define PROPENSITY_SYSTEM
#define CheckClient
#define EVENT_DUNGEON_SIEGE		1
#define _JEWELS_BANK_	1	//20$
#define DEV_STATS_ADVANCE		1
void Splash1();
//#define _LAUNCHER_				"Launcher"
#define CUSTOM_MAP_TELEPORT		1
#define CUSTOM_VALUE_MENU		1
//---------------------------------------------------------------------------

#define Naked(Function) void __declspec(naked) Function()
#define ItemStartIndex	635
#define ITEM(x, y)		((x * 512) + y)
#define ITEMGET(X,Y)    ((X)* 512 + (Y))
#define ITEM2(x, y)		((x * 512) + y + ItemStartIndex)
#define ITEMGET2(type, index)		(type*512 + index + 635)
#define GET_ITEM_OPT_LEVEL(x)	((x>>3)&15)
#define GET_ITEM_OPT_EXC(x)		((x)-(x&64))
#define GET_MAX_WORD_VALUE(x)	(((x)>65000)?65000:((WORD)(x)))
#define MAX_VIEWPORT				400
#define NONE						-1

#define ITEM_BASE_MODEL			635

#define GET_ITEM(x,y)			(((x)*512)+(y))
#define GET_ITEM_MODEL(x,y)		((((x)*512)+(y))+ITEM_BASE_MODEL)
//---------------------------------------------------------------------------

namespace ItemColor
{
	enum T
	{
		White		= 0x00,
		Blue		= 0x01,
		Red			= 0x02,
		Gold		= 0x03,
		Green		= 0x04,
		WhiteRed	= 0x05,
		Violet		= 0x06,
		WhiteBlue	= 0x07,
		WhiteBeige	= 0x08,
		GreenBlue	= 0x09,
		Gray		= 0x0A,
		DarkViolet	= 0x0B,
	};
};
//---------------------------------------------------------------------------

enum BlendType
{
	JPG		= 0x2600,
	TGA		= 0x2601,
};

//---------------------------------------------------------------------------

namespace ASM
{
	enum T
	{
		EAX = 0x50,
		JMP = 0xE9,
		JMPS = 0xEB,
		JE = 0x74,
		JNE = 0x75,
		JGE = 0x7D,
		NOP = 0x90,
		CALL = 0xE8,
		PUSH = 0x68,
		RETN = 0xC3,
		QUIT = 0xFF,
	};
};
//---------------------------------------------------------------------------

enum ItemWeight
{
	Bold	= 0x01,
	Normal  = 0x00,
};
//---------------------------------------------------------------------------

enum IndexState
{
	Loading			= 1,
	SelectServer 	= 2,
	SwitchCharacter	= 4,
	InGame			= 5,
};
//---------------------------------------------------------------------------
