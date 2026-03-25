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
#include "Splash.h"
#include "BMDLoad.h"
#include "Crc32Load.h"
#include "Jewels_Wings.h"
#include "JewelsBank.h"
#include "Shop.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "Protect.h"
#include "console.h"
#include "m_WindowsOption.h"
#include "QuickAccess.h"
#include "Texture.h"
#include "HPBar.h"

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

bool ItemPosition(DWORD a1, DWORD a2, int a3, int Columna, int Fila)
{
	//if (pCheckWindow(6)) //Trade
	//{
	//	return ((bool(*)(DWORD a1, DWORD a2, int a3, int Fila, int a5))0x6321A0)(0x113, 0x10F, 0x07A25110, Columna, 4);
	//}
	//else if (pCheckWindow(9) /* Chaos Mix */ || pCheckWindow(21) /* Senior Mix */ || pCheckWindow(28) /* Refinery */)
	//{
	//	return ((bool(*)(DWORD a1, DWORD a2, int a3, int Fila, int a5))0x6321A0)(0x113, 0x6E, 0x07EA9508, Columna, 4);
	//}
	//else if (pCheckWindow(7)) //Warehouse
	//{
	//	return ((bool(*)(DWORD a1, DWORD a2, int a3, int Fila, int a5))0x6321A0)(a1, a2, a3, Columna, Fila);
	//}
	//return 0;
	if (pCheckWindow(6)) //Trade
	{
		return ((bool(*)(DWORD a1, DWORD a2, int a3, int Fila, int a5))0x06321A0)(0x113, 0x10F, 0x07A25110, Columna, 4); //Correto
	}
	else if (pCheckWindow(9) /* ChaosMix */ || pCheckWindow(21) /* SeniorMix */ || pCheckWindow(28) /* Refinery */)
	{
		return ((bool(*)(DWORD a1, DWORD a2, int a3, int Fila, int a5))0x06321A0)(0x113, 0x6E, 0x07EA9508, Columna, 4);
	}
	else if (pCheckWindow(7)) //Warehouse
	{
		return ((bool(*)(DWORD a1, DWORD a2, int a3, int Fila, int a5))0x06321A0)(a1, a2, a3, Columna, Fila);
	}
	return 0;
}

bool __stdcall CheckClick(int a1)
{
	if (pCheckWindow(6))
	{
		return true;
	}

	if (pCheckWindow(9))
	{
		return true;
	}

	return pCheckWindow(a1);
}

void InitItem()
{
	 //MemorySet(0x062F598, 0x90, 2); //Move item Interface -> Inventario OK // ok
	 //MemorySet(0x062F51D, 0x90, 2); //Inventario -> Interface // ok
	 //SetCompleteHook(0xE8, 0x062F53B, &ItemPosicion);
	//MemorySet(0x0062F598, 0x90, 2); //Move item Interface -> Inventory//62F598
	//MemorySet(0x0062F51D, 0x90, 2); //Move item Inventory -> Interface//62F51D
	//SetCompleteHook(0xE8, 0x0062F53D, &ItemPosition);//62F53D
	
	//MemorySet(0x062F598, 0x90, 2); //Move item Interface -> Inventario OK // ok
	//MemorySet(0x062F51D, 0x90, 2); //Inventario -> Interface // ok
	 //1.02.11
	SetCompleteHook(0xE8, 0x0062F516, &CheckClick);//CheckClick); //0062F546   E8 6B680C00      CALL Main.006F5DB6 //Need Fix //0062F5EA   E8 E12B0000      CALL Main.006321D0  0062F516
	SetCompleteHook(0xE8, 0x0062F591, &CheckClick);//CheckClick); //0062F5C1   E8 F0670C00      CALL Main.006F5DB6
	SetCompleteHook(0xE8, 0x0062F53B, &ItemPosition);


}

void Splash()
{
    CSplash splash1(TEXT(".\\Data\\Customs\\Phoenix.bmp"), RGB(128, 128, 128));
	splash1.ShowSplash();
    Sleep(3000);

    splash1.CloseSplash();
}

void Splash1()
{
    CSplash splash1(TEXT(".\\Data\\Customs\\PhoenixGuard.bmp"), RGB(128, 128, 128));
	splash1.ShowSplash();
    Sleep(3000);

    splash1.CloseSplash();
}
void __declspec(naked) iconHook()
{
	//- Custom Client Ico (S8)
	//if (gProtect.m_MainInfo.CustomClientIconType == 1)
	//{
		static DWORD dwIcon = (DWORD)LoadIcon(gController.Instance, MAKEINTRESOURCE(IDI_ICON1));

		static DWORD dwJmp = 0x004DBC92;

		_asm
		{
			MOV EAX, dwIcon
			JMP dwJmp
		}
	//}

	////- Custom Client Ico (EX)
	//if (gProtect.m_MainInfo.CustomClientIconType == 2)
	//{
	//	static DWORD dwIcon = (DWORD)LoadIcon(gController.Instance, MAKEINTRESOURCE(IDI_ICON3));

	//	static DWORD dwJmp = 0x004D0E3C;

	//	_asm
	//	{
	//		MOV EAX, dwIcon
	//		JMP dwJmp
	//	}
	//}

	////- Custom Client Ico (Mu Plasma)
	//if (gProtect.m_MainInfo.CustomClientIconType == 3)
	//{
	//	static DWORD dwIcon = (DWORD)LoadIcon(gController.Instance, MAKEINTRESOURCE(IDI_ICON4));

	//	static DWORD dwJmp = 0x004D0E3C;

	//	_asm
	//	{
	//		MOV EAX, dwIcon
	//		JMP dwJmp
	//	}
	//}

	////- Custom Client Ico (Mu Plasma)
	//if (gProtect.m_MainInfo.CustomClientIconType == 4)
	//{
	//	static DWORD dwIcon = (DWORD)LoadIcon(gController.Instance, MAKEINTRESOURCE(IDI_ICON5));

	//	static DWORD dwJmp = 0x004D0E3C;

	//	_asm
	//	{
	//		MOV EAX, dwIcon
	//		JMP dwJmp
	//	}
	//}

	////- Custom Client Ico (Classic)
	//else
	//{
	//	static DWORD dwIcon = (DWORD)LoadIcon(gController.Instance, MAKEINTRESOURCE(IDI_ICON1));

	//	static DWORD dwJmp = 0x004D0E3C;

	//	_asm
	//	{
	//		MOV EAX, dwIcon
	//		JMP dwJmp
	//	}
	//}
}
HWND hWndMu;
void Mux()
{
	//CRYPT_START
	Sleep(1000);
	WinExec("AutoClicker\\MUAutoClicker.exe", 1);
	//ShellExecute(hWndMu,"open","Mu.exe",NULL, NULL, 0);
	//CRYPT_END
}
void Mu()
{
	//CRYPT_START
	FILE *fp;
	fp=fopen("AutoClicker\\MUAutoClicker.exe","rb");
	if(!fp)
	{
		MessageBoxA(NULL,"File Mu.exe not found!\nPlease restore the file","Error on startup...",MB_OK);
		::ExitProcess(0);
	}
	//CRYPT_END
}
void MutexDenied(LPCSTR mutexName) {
    HANDLE hMutex = OpenMutexA(MUTEX_ALL_ACCESS, 0, mutexName); // Открываем Mutex

    if (!hMutex)
        hMutex = CreateMutexA(0, 0, mutexName); // Если наш Mutex не найден создаем
    else
    {

        MessageBoxA(nullptr, "Programm already started", "Error", 0u); // Программа уже запущена выводим MSBOX
        CloseHandle(hMutex); // Закрываем ручку
        exit(-1); // Выходим с результатом return -1
    }
}

int main() {
MutexDenied("Main");
}
void WINAPI ReduceConsumeProc() // OK
{
	while(true)
	{
		Sleep(5000);
		SetProcessWorkingSetSize(GetCurrentProcess(),0xFFFFFFFF,0xFFFFFFFF);
		SetThreadPriority(GetCurrentProcess(),THREAD_PRIORITY_LOWEST);
	}
}
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport)void Init()
{
	//main();
	//main();
	/*DWORD OldProtect;
	if(VirtualProtect(LPVOID(0x401000),0x00875FFF,PAGE_EXECUTE_READWRITE,&OldProtect))
	{*/
	//char buff[256];

	//wsprintf(buff,"gMessage.GetMessage(5)","TEST");

	//SplashScreen(&SplashInit,0,1,buff,5);
	//Crc32();
	//g_Console.InitCore();
	/*Mu();
	Mux();*/
	//Splash();
#ifdef _LAUNCHER_
		///*char **	Command	= 0;
		//CommandLineToArg(GetCommandLine(), &Command);
		//if( strcmp(_LAUNCHER_, Command[1]) )
		//{
		//	MessageBox(0, "Please start game from Launcher", "Start Error", ERROR);
		//	ExitProcess(0);
		//}*/
	//if(OpenMutex(MUTEX_ALL_ACCESS,0,"Launcher") == 0)
	//{
	//	MessageBox(0, "Please start game from Launcher", "Start Error", ERROR);
	//	ExitProcess(0);
	//}
#endif
	SetOp((LPVOID)0x004DBC85, iconHook, ASM::CALL);

	SetOp((LPVOID)0x00791660, (LPVOID)_strlen, ASM::JMP);
	ChangeAddress(MU_WND_PROC_HOOK, FPTR(WndProc));
	gController.Load();
#if(ANTI_CHEAT_PLUS==TRUE)
		//g_AntiCheatPlus.Load();
#endif
	cOtherLoad();
	cProtocolLoad();
	cBMDLoad();
	cItemPropertiesLoad();
	gInterfaces->Init();
	gInterfaces->StateD = 1;
	gInterfaces->StateFog = 1;
	gInterfaces->StateWare = 0;
	gInterfaces->StateWarex = 0;
	//SetOp((LPVOID)oLoadSomeForm_Call,LoadImages, ASM::CALL);
	gExInterface->Init();//load images
	gConnectEx.Load();
	cMakePreviewCharSetLoad();
	InitMiniMap();
	cWeaponFloatLoad();
	NpcManagerInit();
	//g_MonsterEx.InitMonster();
	gItemSmoker.Init();
	gGlow.Load();
	gCamera->Init();
	gAttackHelper->Init();

	InitFont();

	gOptionsMenu.Init();
	MonsterHPBarMode ^= 1;
	//gOptionWindow.Main();
	gObjUser.Load();
	gCustomMap.Load();
	InitItem();
	gJewelsBank.JewelsBankLoad();

	gCLoginWin.Load();

	glEnable_o = (FUNC_GLENABLE)DetourFunction((PBYTE)DetourFindFunction("Opengl32.dll","glBegin"),(PBYTE)glEnable_h);
//
	//LoadLibraryA("MUEliteClicker.dll");

	if(gProtect.ReadMainFile(".\\Data\\Customs\\Configs\\Main.info") != 0)
	{
		
		//pTexture.Load();
		gItemStack.Load(gProtect.m_MainInfo.ItemStackInfo);
		gItemValue.Load(gProtect.m_MainInfo.ItemValueInfo);
		InitShop();
		gCustomFenrir.Load(gProtect.m_MainInfo.CustomFenrirInfo);
		gCustomFenrir.Init();
		//OTKLYCHIL
		//gProtect.CheckLauncher();
		//gProtect.CheckInstance();
		//gProtect.CheckClientFile();
		//gProtect.CheckPluginFile();
	}
	else
	{
		ErrorMessageBox("Could not load Main.info!");
		ExitProcess(0);
	}
	CreateThread(0,0,(LPTHREAD_START_ROUTINE)ReduceConsumeProc,0,0,0);
	//}
//	WaitForMultipleObjects(3,ThreadHandles,1,2000);
	//SplashInit.CloseSplash();
}

//---------------------------------------------------------------------------
 BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	
	switch (ul_reason_for_call)
	{
		
		case DLL_PROCESS_ATTACH:
			{

			//
#if ANTI_HACKex
			////Process Dump
			//CloseHandle(CreateThread(NULL,0,UpdateTimer,NULL,0,NULL));
			////Process Scan
			//YProtectionMain();
			//Close_Window();
			////Class Name
			//ZProtectionMain();
			//Close_Class();
			////Hide Detect
			//DetectHide();//not work
			////Speed Detect
			// SPerformance();
			// DetectSpeed();
			////DLL Inject
			//Dll_Inject();
			////Detect Process ID
			//DetectID();
			////Heuristic Detect
			////Heuristica2();
			//HProtection();
			//HeuristicW();
			//Heuristicclass();

			//XProtectionMain();


			//Anti Kill
			//AntiKill();  
			//AntiKill2();
			//AntiKill3();
	
#endif
				Init();
				gController.Instance = hModule;
				//KeyboardSetHook(true); 
				//MouseSetHook(true);
				//gController.MouseHook = SetWindowsHookEx(WH_MOUSE, KeyboardProc, (HINSTANCE)hModule, GetCurrentThreadId());
				//gController.KeyboardHook= SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, (HINSTANCE)hModule, GetCurrentThreadId());
				DisableThreadLibraryCalls(hModule);
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)glEnable_Real, glEnable_Hooked);
				DetourAttach(&(PVOID&)glClearColor_Real, glClearColor_Hooked);
				DetourTransactionCommit();
				//LoadLibraryA("Clicker.dll");
				//
				////LoadLibraryA("Clicker.dll");
				//LoadLibraryA("LordOfMudll.dll");
				/*LoadLibraryA("MUAutoClicker.dll");
				LoadLibraryA("MUEliteClicker.dll");*/
				//(GetModuleHandle(L"Kernel32.dll"), "GetProcAddress"));
	//			
	//			HMODULE hModule = LoadLibrary("wz_at.dll");

	//////			/*if(module == 0)
	//////			{
	//////				ExitProcess(0);
	//////			}*/
	//////			LoadLibraryA("wz_at.dll");

	//			void (*EntryProc)(char*) = ((void(*)(char*))GetProcAddress(hModule, "EntryProc"));

	//if (EntryProc)
	//{
 //// //choose one
 //// //EntryProc("EX700");
 //// //EntryProc("SS2");
	//}
	/*LoadLibraryA("LordOfMU.dll");*/
			//	LoadLibraryA("LordOfClicker.dll");
				/*if(EntryProc != 0)
				{
					EntryProc();
				}*/
				//LoadLibraryA("LordOfMU.dll");
				////LoadLibraryA("wz_at.dll");
				//LoadLibraryA("MUAutoLoader.dll");
				//////LoadLibraryA("MUAutoClicker.dll");
				//LoadLibraryA("MUAutoClicker.dll");
				//LoadLibraryA("MUEliteClicker.dll");
				
				
			}
		break;
			
		case DLL_PROCESS_DETACH:
			{
				gController.Instance = hModule;
				////KeyboardSetHook(false); MouseSetHook(false);
				DisableThreadLibraryCalls(hModule);
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)glEnable_Real, glEnable_Hooked);
				DetourAttach(&(PVOID&)glClearColor_Real, glClearColor_Hooked);
				DetourTransactionCommit();

				gController.Load();
			}
		break;
	}
	return TRUE;
} 
//---------------------------------------------------------------------------





