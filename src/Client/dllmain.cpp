#include "stdafx.h"

#include "TMemory.h"
#include "Other.h"
#include "Protocol.h"
#include "Effects.h"
#include "MakePreviewCharSet.h"
#include "detours.h"
#include "Reconnect.h"
#include <Windows.h>
#include "Map.h"
#include "MoveList.h"
#include <fcntl.h>
#include <io.h>
#include "Packets.h"
#include "detours.h"
#include "Interface.h"
#include "MiniMap.h"
#include "WeaponFloat.h"
#include "CustomNPC.h"
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <gl\gl.h>
#include <gl\glu.h>   
#include <process.h>
#include "Smoke.h"
#include "Glow.h"
#include "InterfaceEx.h"
#include "CameraEx.h"
#include "AttackHelper.h"
#include "WndProc.h"
#include "Controller.h"
#include "AntiCheatPlus.h"
#include "User.h"
#include "Splash.h"
#include "AntiCheat.h"
#include "Map.h"
#include "Fog.h"
#include "RageSystem.h"
#include "AutoClickerUI.h"
#pragma comment( lib, "opengl32.lib" )  
#pragma comment( lib, "glu32.lib" )  
#pragma comment(lib, "detours2.lib")
#pragma comment(lib, "detoured.lib")

int *MAP_CHECK      = (int*)0x007D60C8;
int *MAP_ADD_EFFECT = (int*)0x007D60C8;
int *MAIN_STATE     = (int*)0x007E8014;

bool FOG_ENABLED = true;
bool GLTEX_ENABLED = true;

void APIENTRY glEnable_Hooked(GLenum); 
typedef void (APIENTRY *ptr_glEnable)(GLenum);
ptr_glEnable glEnable_Real = (ptr_glEnable)&glEnable; //Addr_glEnable;
void APIENTRY glClearColor_Hooked(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (APIENTRY *ptr_glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
ptr_glClearColor glClearColor_Real = (ptr_glClearColor)&glClearColor; //Addr_glClearColor;
unsigned int Textures[5];

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_GENERATE_MIPMAP 0x8191
void APIENTRY glEnable_Hooked(GLenum cap)
{		
	RGBAStruct rgb = FogMapColor(*MAP_CHECK); 

	GLfloat rgba[4] = { rgb.r, rgb.g, rgb.b, rgb.a };

	// ����������� �������: �������� � �����������
	if (GLTEX_ENABLED == true)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (*MAIN_STATE == 0x4)
		glDisable(GL_FOG);

	if (*MAIN_STATE == 5)
	{
		if (gInterfaces->StateFog == 1)
		{
			if (cap == GL_BLEND || cap == GL_TEXTURE_2D || cap == GL_DEPTH_TEST)
			{
				glDisable(GL_FOG);  
			}

			//if(pCheckWindow(5))
			//{
			//	glDisable(GL_FOG);
			//
			//}

			// ----------
			glEnable_Real(GL_FOG);
		  // ----------

		  glFogi(GL_FOG_MODE, GL_LINEAR);
		  glFogf(GL_FOG_DENSITY, 0.025f);
		  glFogfv(GL_FOG_COLOR, rgba);
		  glFogf(GL_FOG_START, 1350.0f);//1800
		  glFogf(GL_FOG_END, 2200.0f);
		  glHint(GL_FOG_HINT, GL_NICEST); // glHint(GL_FOG_HINT, GL_FASTEST);

			if (cap == GL_BLEND || cap == GL_TEXTURE_2D || cap == GL_DEPTH_TEST)   
			{
				glDisable(GL_FOG);  
			}
		}
	}

	// ----------
	glEnable_Real(cap);
	// ---------- 
} 
void APIENTRY glClearColor_Hooked(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	RGBAStruct rgb = FogMapColor(*MAP_CHECK); 

	if (*MAIN_STATE == 2 || *MAIN_STATE == 4)
	{ 
		glBindTexture(GL_TEXTURE_2D, *Textures);
		glClearColor_Real(0.0f, 0.0f, 0.0f, 0.0f); 
	}

	if (*MAIN_STATE == 5)
	{ 

		glBindTexture(GL_TEXTURE_2D, *Textures); 
		glClearColor_Real(rgb.r, rgb.g, rgb.b, 0.0f);
	}
}
typedef void (APIENTRY *FUNC_GLENABLE) (GLenum mode);
FUNC_GLENABLE glEnable_o = NULL;
int Ani = GetPrivateProfileInt("Engine","UseCustomAnisotropy",1,"");
int Tri = GetPrivateProfileInt("Engine","UseCustomTrilinear",1,"");
int Lin = GetPrivateProfileInt("Engine","UseCustomLinear",1,"");
void APIENTRY glEnable_h(GLenum mode)
{
	if (*MAIN_STATE == 5)
	{
	if(Lin!=0)
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	// ----
	if(Tri!=0)
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	// ----
	if(Ani!=0)
	{
		glGetIntegerv(0x84FF,&Ani);
		glTexParameteri(GL_TEXTURE_2D,0x84FE,Ani-0.1);
		/*switch(gDisplay.Counter)
		{
			case 1:
			{
				Ani = 2;
				glGetIntegerv(0x84FF,&Ani);
				glTexParameteri(GL_TEXTURE_2D,0x84FE,Ani-0.1);
			}
			break;

			case 2:
			{
				Ani = 4;
				glGetIntegerv(0x84FF,&Ani);
				glTexParameteri(GL_TEXTURE_2D,0x84FE,Ani-0.1);
			}
			break;

			case 3:
			{
				Ani = 6;
				glGetIntegerv(0x84FF,&Ani);
				glTexParameteri(GL_TEXTURE_2D,0x84FE,Ani-0.1);
			}
			break;

			case 4:
			{
				Ani = 8;
				glGetIntegerv(0x84FF,&Ani);
				glTexParameteri(GL_TEXTURE_2D,0x84FE,Ani-0.1);
			}
			break;
		}
		*/
	}
	//
	}
	glEnable_o(mode);
	
}
#define BYTEn(x, n)   (*((BYTE*)&(x)+n))


int _strlen(DWORD pointer)
{
	DWORD v1;
	BYTE v2;
	int v3;
	int v4;

	if((LPVOID)&pointer == NULL)
	{
		return 0;
	}

	v1 = pointer;
	BYTE sub = 4;
	if((pointer & 3))
	{
		do
		{
			v2 = *(BYTE*)v1++;

			if ( !v2 )
			{
				sub = 1;
				goto _finish;
			}
		}
		while ( v1 & 3 );
	}

	while(true)
	{
		do
		{
			v3 = (*(DWORD*)v1 + 0x7EFEFEFF) ^ ~*(DWORD*)v1;
			v1 += 4;
		}
		while(!(v3 & 0x81010100));

		v4 = *(DWORD*)(v1 - 4);

		if(!BYTEn(v4, 0))
		{
			sub = 4;
			break;
		}

		if(!BYTEn(v4, 1))
		{
			sub = 3;
			break;
		}

		if(!BYTEn(v4, 2))
		{
			sub = 2;
			break;
		}

		if(!BYTEn(v4, 3))
		{
			sub = 1;
			break;
		}
	}

_finish:

	return v1 - sub - pointer;
}
DWORD WINAPI UpdateTimer(LPVOID lpParam)
{
	Sleep(5000);
	while(true)
	{
#if ANTI_HACKex
		SystemProcessesScan();
		//CheckFilesDir(1);
#endif
		Sleep(5000);	//5 sek perekur
	}
	return 1;
}
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)void Init()
{
	SetOp((LPVOID)0x00791660, (LPVOID)_strlen, ASM::JMP);
	ChangeAddress(MU_WND_PROC_HOOK, FPTR(WndProc));
#if(ANTI_CHEAT_PLUS==TRUE)
		g_AntiCheatPlus.Load();
#endif
	cOtherLoad();
	cProtocolLoad();
	gInterfaces->Init();
	gInterfaces->StateD = 1;
	gInterfaces->StateFog = 1;
	////SetOp((LPVOID)oLoadSomeForm_Call,LoadImages, ASM::CALL);
	gExInterface->Init();
	gConnectEx.Load();
	cMakePreviewCharSetLoad();
	InitMiniMap();
	cWeaponFloatLoad();
	NpcManagerInit();
	gItemSmoker.Init();
	gGlow.Load();
	gCamera->Init();
	gAttackHelper->Init();
	gAutoClickerUI.Init();
	gController.Load();
	KeyboardSetHook(true);
	gObjUser.Load();
	gCustomMap.Load();

	glEnable_o = (FUNC_GLENABLE)DetourFunction((PBYTE)DetourFindFunction("Opengl32.dll","glBegin"),(PBYTE)glEnable_h);


#if ANTI_HACKex
	//CloseHandle(CreateThread(NULL,0,UpdateTimer,NULL,0,NULL));
	//YProtectionMain();		//Process Scan
	//HideMain();			//Detect HideToolz
	
#endif

}
void StartAddress(LPVOID lpThreadParameter)
{
	HANDLE v1;
	HANDLE v2;
	// ----
	while(TRUE)
	{
		Sleep(5000);
		v1 = GetCurrentProcess();
		SetProcessWorkingSetSize(v1,0xFFFFFFFF,0xFFFFFFFF);
		v2 = GetCurrentProcess();
		SetThreadPriority(v2,-2);
	}
}
//---------------------------------------------------------------------------
 BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			
			//

			gController.Instance = hModule;
			DisableThreadLibraryCalls(hModule);
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)glEnable_Real, glEnable_Hooked);
			DetourAttach(&(PVOID&)glClearColor_Real, glClearColor_Hooked);
			DetourTransactionCommit();
			Init();
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)StartAddress,0,0,0);
		break;

		case DLL_PROCESS_DETACH:
			DisableThreadLibraryCalls(hModule);
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)glEnable_Real, glEnable_Hooked);
			DetourAttach(&(PVOID&)glClearColor_Real, glClearColor_Hooked);
			DetourTransactionCommit();
		break;
	}
	return TRUE;
} 
//---------------------------------------------------------------------------





