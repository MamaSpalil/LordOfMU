#include "stdafx.h"
#include "Other.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Structure.h"
#include "Items.h"
#include "HPBar.h"
#include "Visual.h"
#include "ObjectManager.h"
#include "SelectChar.h"
#include "Splash.h"
#include "Resolution.h"
#include "Interface.h"
#include "CustomMenu.h"
#include "m_WindowsOption.h"
//---------------------------------------------------------------------------


// ----
//int * GameState		= (int*) (iGameState);
//int * Object_Char	= (int*) (ObjectType_Player);
//int * Object_NPC	= (int*) (ObjectType_NPC);
__declspec(naked) void SetObjectFixPlayerPointer() // Fix Character
{
	static DWORD SetObjectFixPlayerPointer_Offset01 = 0x005F6964; // return LOOP
	static DWORD SetObjectFixPlayerPointer_Offset02 = 0x005F67DC;
	static DWORD SetObjectFixPlayerPointer_Pointer1;
	static BYTE SetObjectFixPlayerPointer_Pointer2;


	_asm
	{
		MOV BL, BYTE PTR SS : [ESP + 0x28]
		MOV SetObjectFixPlayerPointer_Pointer2, BL
		MOV SetObjectFixPlayerPointer_Pointer1, EBP
	}


	if (MAIN_STATEEx == 5 && SetObjectFixPlayerPointer_Pointer2 == 1)// && *(WORD*)(SetObjectFixPlayerPointer_Pointer1 + 0x234) == gObjectManager->GetIndex())
	{
		_asm
		{
			JMP GOTOLOOP;
		}
	}

	_asm
	{
		MOV AL, BYTE PTR SS : [EBP + 0xBC]
		MOV BL, BYTE PTR SS : [ESP + 0x28]
		MOV CL, AL
		AND CL, BL
		CMP CL, AL
		JNZ GOTOLOOP;


		JMP[SetObjectFixPlayerPointer_Offset02]
		GOTOLOOP:
		JMP[SetObjectFixPlayerPointer_Offset01]
	}
}
int LastSelectCharId = -1;
__declspec(naked) void FixObjectTarget()
{
	static DWORD FixObjectTarget_Offset01 = 0x005F7425;
	static DWORD FixObjectTarget_Offset02 = 0x005F73B4;
	static DWORD FixObjectTarget_Pointer01;
	static DWORD FixObjectTarget_Pointer02;
	static DWORD FixObjectTarget_Struct;
	static WORD FixObjectTarget_Index;

	_asm
	{
		MOV FixObjectTarget_Pointer01, EAX
		MOV FixObjectTarget_Pointer02, ESI
	}


	if (MAIN_STATEEx == 4 && FixObjectTarget_Pointer01 != -1 && MouseLeftButtonDown == 1)
	{
		if (LastSelectCharId != FixObjectTarget_Pointer01)
		{
			pActionSend(*(DWORD*)(PREVIEW_STRUCT)+(FixObjectTarget_Pointer01 * PREVIEW_STRUCT_SIZE), (153 + (rand() % 42)));

			LastSelectCharId = FixObjectTarget_Pointer01;
		}

	}

	
	MAIN_ATTACK_TARGET = FixObjectTarget_Pointer01;

	if (FixObjectTarget_Pointer01 == FixObjectTarget_Pointer02)
	{
		_asm
		{
			JMP[FixObjectTarget_Offset02]
		}
	}

	_asm
	{
		JMP[FixObjectTarget_Offset01]
	}

	/*_asm
	{
		CMP FixObjectTarget_Pointer01, FixObjectTarget_Pointer02
		JNZ JNZ_JMP;
		//MOV DWORD PTR DS : [0x007D4C58], EAX
		JMP [FixObjectTarget_Offset02]

		JNZ_JMP:
		JMP[FixObjectTarget_Offset01]
	}*/
	//MOV DWORD PTR DS : [0x07D4C58], EAX
}

//---------------------------------------------------------------------------
void cOtherLoad()
{
  	
	//SetRange((LPVOID)0x005F67C6, 10, ASM::NOP);
	//SetOp((LPVOID)0x005F67C6, (LPVOID)SetObjectFixPlayerPointer, ASM::JMP);

	//SetOp((LPVOID)0x005F73AD, (LPVOID)FixObjectTarget, ASM::JMP); // Animate Screen
	HookJump(0x5F678C, ObjectPointer);
	//// ----
	//// - Textures
	HookCall(0x6B3943, cTexture::Models);
	HookCall(0x6B3958, cTexture::Textures);
	//// - HP Bar
	SetNop(0x61FD89, 5);
	HookCall(0x6013E7, gHPBar.Draw);
	//// - Visual
	cLoadLifeManaFix();
	////Visual.Load();
	//// - Resolutions
	InitResolution();
	//SetOp((LPVOID)0x004DCFD5, (LPVOID)cGameLoad, ASM::CALL);
	//// - Guild Mark
	SetCompleteHook(0xE8, 0x006037C1, &DrawIcons);//
	//// - Fix Exp
	Fix();
	////InitFixExp();
	//AntiLag();
	////
	CButtonCharacterS13();
	// -- Frame
	SetByte((LPVOID)0x006D242F, 0xD1);
	SetRange((LPVOID)0x006D241C, 18, ASM::NOP);
	SetOp((LPVOID)0x006D241C, (LPVOID)SetFrameSleep, ASM::JMP);

	Changer();
}
//---------------------------------------------------------------------------

void FontLoad()
{
	HFONT Fonte = CreateFontA(14, 0, 0, 0, 300, 0, 0, 0, 0x01, 0, 0, 4, 0, "Arial");
}

//SHORT WINAPI KeysProc(int nCode) // OK
//{
//	if(GetForegroundWindow() != *(HWND*)(MAIN_WINDOW))
//	{
//		return 0;
//	}
//
//	return GetAsyncKeyState(nCode);
//}

SHORT WINAPI KeysProc(int nCode) // OK
{
	if(GetForegroundWindow() != *(HWND*)(MAIN_WINDOW))
	{
		return 0;
	}

	return GetAsyncKeyState(nCode);
}
//new update 2.0.0
DWORD dword7CFF50[] =
{
	0x4,        //Greater Damage
	0x8, 	  	//Greater Defense
	0x10,		//Greater Life
	0x100,		//Soul Barrier
	0x200,		//Brand 
	0x800,		//Critical Damage Increase
	0x1000,		//Stern 
	0x2000,		//Swell Mana
	0x4000,		//Invisibility
	0x8000,		//Bless Potion
	0x10000,	//Soul Potion
	0x1,		//Blessing of Jack O'Lantern
	0x2,		//Scream of Jack O'Lantern
	0x4,		//Rage of Jack O'Lantern
	0x8,		//Food of Jack O'Lantern
	0x10,		//Drink of Jack O'Lantern
	0x20,		//Seal of Ascension
	0x40,		//Seal of Wealth
	0x80,		//Seal of Sustenance
	0x100,		//Seal of Mobility
	0x800,		//Scroll of Quickness
	0x1000,		//Scroll of Defense
	0x2000,		//Scroll of Wrath
	0x4000,		//Scroll of Wizardry
	0x8000,		//Scroll of Health
	0x10000,	//Scroll of Mana
	0x400000,	//Elixir of Strength
	0x800000,	//Elixir of Agility
	0x1000000,	//Elixir of Vtality
	0x2000000,	//Elixir of Energy
	0x4000000,	//Elixir of Commond
	0x40000,	//Magic Shield
	0x80000,	//Resistance
}; 

BYTE byte7CFFD0[] =
{
	0x1C,  //Greater Damage
	0x1B,  //Greater Defense
	0x30,  //Greater Life
	0x10,  //Soul Barrier
	0x4B,  //Brand 
	0x40,  //Critical Damage Increase
	0x43,  //Stern 
	0x45,  //Swell Mana
	0x46,  //Invisibility
	0x0CB, //Bless Potion
	0x0CC, //Soul Potion
	0x0CD, //Blessing of Jack O'Lantern
	0x0CE, //Scream of Jack O'Lantern
	0x0CF, //Rage of Jack O'Lantern
	0x0D0, //Food of Jack O'Lantern
	0x0D1, //Drink of Jack O'Lantern
	0x0D2, //Seal of Ascension
	0x0D3, //Seal of Wealth
	0x0D4, //Seal of Sustenance
	0x0D5, //Seal of Mobility
	0x64,  //Scroll of Quickness
	0x65,  //Scroll of Defense
	0x66,  //Scroll of Wrath
	0x67,  //Scroll of Wizardry
	0x68,  //Scroll of Health
	0x69,  //Scroll of Mana
	0x6E,  //Elixir of Strength
	0x6F,  //Elixir of Agility
	0x70,  //Elixir of Vtality
	0x71,  //Elixir of Energy
	0x72,  //Elixir of Commond
	0x78,  //Magic Shield
	0x79,  //Resistance
};
_declspec(naked) void FixChasingAttackMovement()
{
    static DWORD jmpBack = 0x005A67E1;
    static DWORD SendMove = 0x005B9E40;

    _asm
    {
        Mov Byte Ptr[Edi + 0x3B1], 5;
        Push Edi;
        Push Edi;
        Call SendMove;
        Add Esp, 0x8;
        Jmp jmpBack;
    }
}
int Delay[5];
void FixEnterChar()
{
	BeginBitmap();
	for(int i=0; i<5; i++)
	{
		if(SelectedHero == i)
		{
				 if(SelectedHero == 0){ Delay[1] = 0; Delay[2] = 0; Delay[3] = 0; Delay[4] = 0; }
			else if(SelectedHero == 1){ Delay[0] = 0; Delay[2] = 0; Delay[3] = 0; Delay[4] = 0; }
			else if(SelectedHero == 2){ Delay[0] = 0; Delay[1] = 0; Delay[3] = 0; Delay[4] = 0; }
			else if(SelectedHero == 3){ Delay[0] = 0; Delay[1] = 0; Delay[2] = 0; Delay[4] = 0; }
			else if(SelectedHero == 4){ Delay[0] = 0; Delay[1] = 0; Delay[2] = 0; Delay[3] = 0; }

			Delay[i]++;

			if(Delay[i] > 10)
			{ SetByte(0x006CE2BC,0x0D); }
			else if(Delay[i] <= 10){ SetByte(0x006CE2BC,0x7F); }
		}
		else if(SelectedHero == -1)
		{
			Delay[i] = 0;
			SetByte(0x006CE2BC,0x7F);
		}
	}
}


void GCPeriodicEffectRecv(PMSG_PERIODIC_EFFECT_RECV* lpMsg)
{
	if (lpMsg->group == 2) // Seals
	{
		if (lpMsg->state) // State 1 -> Delete buff
		{
			if ((*(DWORD*)(Hero + 172) & 0x20) == 0x20)
			{
				*(DWORD*)(Hero + 172) -= 0x20;
			}

			if ((*(DWORD*)(Hero + 172) & 0x40) == 0x40)
			{
				*(DWORD*)(Hero + 172) -= 0x40;
			}

			if ((*(DWORD*)(Hero + 172) & 0x80) == 0x80)
			{
				*(DWORD*)(Hero + 172) -= 0x80;
			}

			KillTimer(g_hWnd, 0x3EF); // Kill buff timer

			//*(int*)0x5877F88 = 0; // Buff Time
		}
		else // State 0 -> Add buff
		{
			switch (lpMsg->value) // Switch buff value
			{
				case 6:
				{
					if ((*(DWORD*)(Hero + 172) & 0x20) != 0x20)
					{
						*(DWORD*)(Hero + 172) |= 0x20;
					}

					if ((*(DWORD*)(Hero + 172) & 0x40) == 0x40)
					{
						*(DWORD*)(Hero + 172) -= 0x40;
					}

					if ((*(DWORD*)(Hero + 172) & 0x80) == 0x80)
					{
						*(DWORD*)(Hero + 172) -= 0x80;
					}

					break;
				}

				case 7:
				{
					if ((*(DWORD*)(Hero + 172) & 0x20) == 0x20)
					{
						*(DWORD*)(Hero + 172) -= 0x20;
					}

					if ((*(DWORD*)(Hero + 172) & 0x40) != 0x40)
					{
						*(DWORD*)(Hero + 172) |= 0x40;
					}

					if ((*(DWORD*)(Hero + 172) & 0x80) == 0x80)
					{
						*(DWORD*)(Hero + 172) -= 0x80;
					}

					break;
				}

				case 8:
				{
					if ((*(DWORD*)(Hero + 172) & 0x20) == 0x20)
					{
						*(DWORD*)(Hero + 172) -= 0x20;
					}

					if ((*(DWORD*)(Hero + 172) & 0x40) == 0x40)
					{
						*(DWORD*)(Hero + 172) -= 0x40;
					}

					if ((*(DWORD*)(Hero + 172) & 0x80) != 0x80)
					{
						*(DWORD*)(Hero + 172) |= 0x80;
					}

					break;
				}
			}
//
			//*(int*)0x5877F88 = lpMsg->time; // Buff Time//XZ

			KillTimer(g_hWnd, 0x3EF); // Kill buff timer

			SetTimer(g_hWnd, 0x3EF, 1000, NULL);
		}
	}

	((void(__cdecl*)(PMSG_PERIODIC_EFFECT_RECV * lpMsg)) 0x00501C10)(lpMsg);//694BE0
}
void __declspec(naked) CheckTickCount() // OK
{
	static DWORD CheckTickCountAddress1 = 0x006D2422;//

	_asm
	{
		Push 1
		Call Dword Ptr Ds:[Sleep]
		Call Dword Ptr Ds:[GetTickCount]
		Jmp[CheckTickCountAddress1]
	}
}
#define SceneFlag				*(int*)0x007E8014
#define CameraAngle				*(float*)0x7EB3830 
void Scene()
{
	((char(*)())0x006CFA60)();
	
    if ( SceneFlag == 2 )
    {
		CameraAngle = -84.0;
      *(float*)0x7EB3834 = 0;			
      *(float*)0x7EB3838 = -45.0;		
      *(float*)0x7EB384C = 24475.79687; 
      *(float*)0x7EB3850 = 7581.581055; 
      *(float*)0x7EB3854 = 1834.539917; 
	 
    }
}
void RenderSands(int a1, float a2, float a3, float a4, float a5, int a6, float a7, float a8, float a9)
{

}

//=================================================================================
void Changer()
{
	SetCompleteHook(0xE8,0x006CF061, &Scene); //006CF241   E8 FA090000      CALL Main.006CFC40
	SetByte1((DWORD)0x006CE91F+6, 94); //006CEAFF   C705 C8607D00 37>MOV DWORD PTR DS:[7D60C8],37
	SetByte1((DWORD)0x006CF13D+1, 480);//006CF31D   68 AE010000      PUSH 1AE
	SetByte1((DWORD)0x006CF147+1, 0);  //006CF327   6A 19            PUSH 19
	SetCompleteHook(0xE8,0x006CF233, RenderSands);//006CF413   E8 42ACFEFF      CALL Main.006BA05A
	SetCompleteHook(0xE8,0x006CF282, RenderSands);//006CF462   E8 F3ABFEFF      CALL Main.006BA05A


	//new update 2.0.0
	SetByte(0x00442F06 + 3, (BYTE)(sizeof(dword7CFF50) / sizeof(int))); // Change for size
	SetByte(0x00442F10 + 3, 12); // Change compare value for character struct
	SetDword(0x00442F36 + 3, (DWORD)&dword7CFF50); // Redirect the array reference
	SetDword(0x00442F40 + 3, (DWORD)&dword7CFF50); // Redirect the array reference
	SetDword(0x00442F7B + 2, (DWORD)&byte7CFFD0); // Redirect the array reference
	SetDword(0x00442FBF + 2, (DWORD)&byte7CFFD0); // Redirect the array reference
	SetCompleteHook(0xE9,0x006D241C,&CheckTickCount);//
	SetCompleteHook(0xE9, 0x005A67DA, &FixChasingAttackMovement);

	//SetCompleteHook(0xE8, 0x0050DAAD, &GCPeriodicEffectRecv);


	//SetCompleteHook(0xE8, 0x006CE89B, &FixEnterChar);
	//=================================================================================
	//SetByte(0x00533306, 0xEB); // Corrupted Filter.bmd // 1.04J 00533306 OK
	//SetByte(0x00496F7A, 0xEB); // Corrupted ItemSetType.bmd // 1.04J 00496F7A OK
	//SetByte(0x004971AF, 0xEB); // Corrupted ItemSetOption.bmd // 1.04J 004971AF OK
	//SetByte(0x004F8B3B,0xEB); // Ctrl Fix 1.04J 004F8B3B OK
	//SetByte(0x0050CFB8, 0xC3); //Fix 1.04J Recu 0050CFB8 OK
	//SetByte(0x004DCC4F,0xEB); // Multi Instance 1.04J 00648C71 OK
	SetByte(0x006CE2BC,0x7F); // Fix Enter Select Char
	// - Fonte
	// - Hook 
	SetDword(0x007B345C,(DWORD)&KeysProc);//7B345C

	//SetNop(0x6DA806, 6);
	//HookCall(0x6DA806, FontLoad);
	//SetNop(0x4DD62C, 6);
	//HookCall(0x4DD62C, FontLoad);
	//SetNop(0x4DD66C, 6);
	//HookCall(0x4DD66C, FontLoad);
	//SetNop(0x4DD6AE, 6);
	//HookCall(0x4DD6AE, FontLoad);
	//SetNop(0x4DD708, 6);
	//HookCall(0x4DD708, FontLoad);

	//BYTE Font[3] = { 0x01, 0x00, 0x00 }; 
	//memcpy((int*)0x007D2790, Font, sizeof(Font));

	// - Crack

	*(BYTE*)(0x4DCCCE) = 0xEB;
	
	*(BYTE*)(0x4DD078) = 0xE9;
	*(BYTE*)(0x4DD079) = 0x8A;
	*(BYTE*)(0x4DD07A) = 0x00;

	//ressource
	*(BYTE*)(0x4E56EA) = 0xEB;
	//*(BYTE*)(0x4DD02F) = 0x75;
	*(BYTE*)(0x4DD02F) = 0xEB;
	SetByte(0x007D2790,0xA0); // Accent//7D2790
	SetByte(0x004D3CCCE,0xEB); // Crack (mu.exe)//4D3CCCE

	// - Char Set

	BYTE CharSet[] = {0xD4};
	memcpy((char*)0x4DD611,CharSet,sizeof(CharSet));
	memcpy((char*)0x4DD651,CharSet,sizeof(CharSet));
	memcpy((char*)0x4DD691,CharSet,sizeof(CharSet));
	memcpy((char*)0x4DD6EB,CharSet,sizeof(CharSet));
	memcpy((char*)0x6DA7EB,CharSet,sizeof(CharSet));
	memcpy((char*)0x73D00F,CharSet,sizeof(CharSet));


	// - Move w/o seal

	*(BYTE*)(0x4683D9) = 0xEB;
	*(BYTE*)(0x468BBC) = 0xEB;

	// - Name Server

	//SetDword(0x6E3992+1, (DWORD)"Niflheimr");

	// - GameMaster Chat

	BYTE ChatColor[] = {0x6A, 0x03};
	memcpy((char*)0x4E70AF, ChatColor, sizeof(ChatColor));

	// - cpu

	*(BYTE*)(0x5209D8) = 0xC3;

	// - Lag use CTRL
	SetByte(0x00476EBB,0xEB); // Ctrl Fix//00476EBB
	SetByte(0x00476EC9,0x02); // Ctrl Fix//00476EC9


	// - Optimize CPU

	HookJump(0x6D241C, Optimize);

	//FPS Limit Removal //
//	*(BYTE*)(0x006D2413+6) = 0x1F; 
	//SetByte(0x006D2413+6,0x1F);			// FPS Limit Removal 
	SetByte(0x006D2413+6,0x19);			// - FPS De 40 para 34 

	//MemorySet(0x0062F51D,0x90,0x02); // Item Move Inventory -> Interface

	//MemorySet(0x0062F598,0x90,0x02); // Item Move Interface -> Inventory
	// - Screenshots

	CreateDirectory("ScreenShot", NULL);

	*(DWORD*)(0x6D1FD3 + 1) = (DWORD)(Screen);


	// -- Fix GameGuard
	SetByte((LPVOID)0x007782CF, 0xEB);

	SetByte((LPVOID)(0x004DD592 + 6), 0x0C-1);
	SetByte((LPVOID)(0x004DD59E + 6), 0x0D-1);
	SetByte((LPVOID)(0x004DD5AA + 6), 0x0E-1);
	SetByte((LPVOID)(0x004DD5B6 + 6), 0x0F-1);

	
	// - Wings Level 1

	*(BYTE*)(0x616AAF + 3) = 0x00;
	*(BYTE*)(0x5978AE + 1) = 0x00;
	*(BYTE*)(0x61C0BE + 3) = 0x00;

	//// - MoveList

	//BYTE MoveList[] = {0xEB, 0x28};
	//memcpy((char*)0x467C68, MoveList, sizeof(MoveList));
	//SetByte((LPVOID)0x0046865D, 0x10); // Fix move
	//SetByte((LPVOID)0x00467F4E, 0x10); // Fix move
	SetByte1(0x00467E3C, 0xC3); //Off M movelist
	SetByte1(0x00468541, 0xC3); //Off M movelist
	// - MuError

	*(BYTE*)(0x4DCB59) = 0xC3;
	*(BYTE*)(0x421627) = 0xC3;

	// - Quest Itens

	*(BYTE*)(0x64ED33) = 0xEB;
	*(BYTE*)(0x64ED3F) = 0xEB;
	

	SetByte( (PVOID) iClock_Interface1, 0x25);
	SetByte( (PVOID) iClock_Interface2, 0x20);
	SetByte( (PVOID) iClock_Interface3, 0x2C);
	SetByte( (PVOID) iClock_Interface4, 0x84);
	SetByte( (PVOID) iClock_Interface5, 0x90);
	SetByte( (PVOID) iClock_Interface6, 0xC0);
	SetByte( (PVOID) iClock_Interface7, 0x99);
	SetByte( (PVOID) iClock_Interface8, 0x09);

	//Quest T
	//SetByte(0x0041E2A0, 0xC3); //window and image
	//SetByte(0x0041CE00, 0xC3); //open window
	//SetByte(0x005CF473, 0xEB); //teclado
	////===================================================================================
	////-- Remove Dark Horse -  
	////===================================================================================
	//SetRange((LPVOID)0x006CE9FD, 5, ASM::NOP);//6CEBDD

	////===================================================================================
	////-- Remove Dark Lord -  
	////===================================================================================
	//SetRange((LPVOID)0x006CE9B8, 5, ASM::NOP);//6CEB98

	////===================================================================================
	////-- Remove Flying Red Dragon
	////===================================================================================
	//SetRange((LPVOID)0x006AC02A, 5, ASM::NOP);//6AC18A

	// ----
	//char cWindowName[16];// = {"MUTEST"};
	//sprintf(cWindowName, "%s", "MU Online");
	//WriteMemory( (PVOID) WindowName, cWindowName, sizeof( cWindowName ) );
	// ----
	char cAdress[20];
	sprintf(cAdress, "%s", SERVER_ADRESS);
	WriteMemory( (PVOID) ConnectAdress, cAdress, sizeof( cAdress ) );
	//// ----
	char cSerial[17];
	sprintf(cSerial, "%s", MAIN_SERIAL);
	WriteMemory( (PVOID) ClientExeSerial, cSerial, sizeof( cSerial ) );
	//// ----
	char cVersion[6];
	sprintf(cVersion, "%s", MAIN_VERSION);
	WriteMemory( (PVOID) ClientExeVersion, cVersion, sizeof( cVersion ) );
	// ----

	//FIX CLOCK IN QUEST DIALOG
	BYTE QuestClockFix[] = { 0xA7 };
	WriteMemory( (PVOID) 0x0041C425, & QuestClockFix, sizeof( QuestClockFix ) );
	WriteMemory( (PVOID) 0x00669379, & QuestClockFix, sizeof( QuestClockFix ) );
	WriteMemory( (PVOID) 0x0066943B, & QuestClockFix, sizeof( QuestClockFix ) );


#ifdef DONT_SHOW_IP_IN_FILE
	SetByte((PVOID)0x004E2059, 0x80);
	SetByte((PVOID)0x004E205A, 0x7B);
	SetByte((PVOID)0x004E205B, 0x87);
	SetByte((PVOID)0x004E205C, 0x05);
#endif
#ifdef CASTLE_SKILL
	// - Skills siege
	
	*(BYTE*)(0x5FD7F7 + 1) = 0x2B;
	*(BYTE*)(0x5FD7FC + 1) = 0x26;
	*(BYTE*)(0x5FD801 + 1) = 0x21;
	*(BYTE*)(0x5FD806 + 1) = 0x1C;
	*(BYTE*)(0x5FD80B + 1) = 0x17;
	*(BYTE*)(0x61E226 + 1) = 0x00;
	*(BYTE*)(0x61E22A + 1) = 0x00;
	*(BYTE*)(0x61E22E + 1) = 0x00;
	*(BYTE*)(0x61E232 + 1) = 0x00;
	*(BYTE*)(0x61E236 + 1) = 0x00;
	*(BYTE*)(0x61E23A + 1) = 0x00;
	*(BYTE*)(0x5FDCF2 + 1) = 0x3C;
	*(BYTE*)(0x5FDCF7 + 1) = 0x37;
	*(BYTE*)(0x5FDCFC + 1) = 0x32;
	*(BYTE*)(0x5FDD01 + 1) = 0x2D;
	*(BYTE*)(0x5FDD06 + 1) = 0x28;
	*(BYTE*)(0x5FD810) = 0xEB;
	*(BYTE*)(0x5FDD0B) = 0xEB;	
#endif
	SetRange((LPVOID)0x00665939, 5, ASM::NOP);
	SetRange((LPVOID)0x00665978, 5, ASM::NOP);
	SetRange((LPVOID)0x0066599A, 5, ASM::NOP);
	SetRange((LPVOID)0x006659BD, 5, ASM::NOP);
	SetRange((LPVOID)0x006659E4, 5, ASM::NOP);
	SetRange((LPVOID)0x00665A06, 5, ASM::NOP);
	SetRange((LPVOID)0x00665A35, 5, ASM::NOP);
	SetRange((LPVOID)0x00665A87, 5, ASM::NOP);
	SetRange((LPVOID)0x00665AAA, 5, ASM::NOP);
	SetRange((LPVOID)0x00665ACE, 5, ASM::NOP);
	SetRange((LPVOID)0x00665AF3, 5, ASM::NOP);
	SetRange((LPVOID)0x00665B16, 5, ASM::NOP);
	SetRange((LPVOID)0x00665B46, 5, ASM::NOP);
	SetRange((LPVOID)0x00665B6B, 5, ASM::NOP);
	//*(BYTE*)(0x006CA355 + 1) = 9;
	//*(BYTE*)(0x006BED4E)  = 0xEB;
	////Fenrir SafeZone (Struct +14)
	//SetByte(0x0051E02D+2,0);//51e02d
	//MemorySet(0x0051D6D3, 0x90, 0x06);//SetPlayerStop//405477//51D6D3
	////MemorySet(0x0054FC92, 0x90, 0x06);//SetPlayerWalk


	////if(DisableStaticEffect)
	////{
	//	//Remove Effect Static
	//	SetByte((DWORD)0x00593971+2, 0);
	////}
	////if(DisableDynamicEffect)
	////{
	////	//Remove Effect Dynamic
	//	SetByte((DWORD)0x0057F6DA+2, 0);
	////}
	////if(DisableSkillEffect)
	////{
	//	//Remove Effect Skill
	//	//SetByte((DWORD)0x00547D33+3, 0);
	//	//Remove Efeect Skill 2
	//	SetByte((DWORD)0x0056626C+2, 0);
	//}

	// Fix Amount Zen Max (2kkk) in Trade/PersonalStore/Warehouse
	SetByte(0x006CA355 + 1, 10);//006CA355

	SetByte(0x0065CA22 + 1, 10);//0065CA22

	SetByte(0x006642A3 + 1, 10);//006642A3

	MemorySet(0x006BED82, 0x90, 5);//006BED82

	MemorySet(0x00605DA8, 0x90, 6);		//00605DA8	

	// [#] Correção de cor do nome do summon (elf) ao equipar pet	
	SetCompleteHook(0xE9,0x00605D85,&Summon);

	// [#] Correção de personagem usar skill sem arma
	SetWord(0x0053FDB2 + 2, 0xFFF);//0053FDB2
	SetWord(0x0053FE23 + 2, 0xFFF);//0053FE23
	SetWord(0x006201E2 + 2, 0xFFF);//006201E2
	//SetWord(0x005BCE86 + 2, 0xFFF);//006716DE xz

}



void __declspec(naked) Summon() // OK
{
	static DWORD Back = 0x00605D8A;//00605D8A
	static DWORD Function = 0x0061F970;//0061F970


	__asm
	{
		PUSHAD;
	}

	glColor3f(1.0f, 1.0f, 1.0f);

	__asm
	{
		POPAD;
		CALL Function;
		JMP Back;
	}
}
//---------------------------------------------------------------------------
bool			Antilag	= true; 

void AntilagSwith()
{

	if(!Antilag)
	{
		AntiLagOff();

	}
	else
	{
		AntiLagOn();
	}
}

void AntiLagOn()
{
	Antilag =!Antilag;
	SetByte((DWORD)0x00593971+2, 0);//Sprite/Static
	SetByte((DWORD)0x0057F6DA+2, 0);//Particle/Dynamic
	SetByte((DWORD)0x00547D33+3, 0);//Skill
	SetByte((DWORD)0x0056626C+2, 0);//Skill
	pSendChatTextEx((const char *)"", (const char *)"AntiLag System On", Blue);
	//pDrawMessage(gCustomMessage.GetMessage(14),1);
}

void AntiLagOff()
{
	Antilag =!Antilag;
	SetByte((DWORD)0x00593971+2, 0x55);//Sprite/Static
	SetByte((DWORD)0x0057F6DA+2, 0x55);//Particle/Dynamic
	SetByte((DWORD)0x00547D33+3, 0x55);//Skill
	SetByte((DWORD)0x0056626C+2, 0x55);//Skill
	pSendChatTextEx((const char *)"", (const char *)"AntiLag System Off", Red);
}

void __declspec(naked) ObjectPointer()
{
	DWORD Pointer;

	_asm
	{
		MOV Pointer, EBP
	}

	if (GameState == 4)
	{
		_asm
		{
			JE RETURN
		}
	}

	if (Pointer == ObjectChar)
	{
		_asm
		{
			JE CONTINUE
		}
	}

	_asm
	{
	    RETURN:
		MOV AL, BYTE PTR SS : [EBP + 0x46C]
		MOV ESI, 0x5F6792
		JMP ESI

		CONTINUE:
		MOV ESI, 0x5F6964
		JMP ESI
	}
}
__declspec(naked) void Optimize()
{
	static DWORD OptimizeBack = 0x6D2422;

	__asm
	{
		PUSH 1;
		CALL DWORD PTR DS : [0x7B3164];
		CALL DWORD PTR DS : [0x7B3160]; 

		JMP OptimizeBack;
	}
}

__declspec(naked) void SetFrameSleep()
{
	static DWORD SetFrameSleep_Offset01 = 0x006D242E;
	Sleep(1);

	_asm
	{
		jmp [SetFrameSleep_Offset01]
	}

}

int cGameLoad()
{
	pGameLoad();
	// ----
	switch(pGameResolutionMode)
	{
	case 0:
		pWinWidth		= 640;
		pWinHeight		= 480;
		break;
		// --
	case 1:
		pWinWidth		= 800;
		pWinHeight		= 600;
		break;
		// --
	case 2:
		pWinWidth		= 1024;
		pWinHeight		= 768;
		break;
		// --
	case 3:
		pWinWidth		= 1152;
		pWinHeight		= 864;
		break;
		// --
	case 4:
		pWinWidth		= 1280;
		pWinHeight		= 768;
		break;
		// --
	case 5:
		pWinWidth		= 1280;
		pWinHeight		= 800;
		break;
		// --
	case 6:
		pWinWidth		= 1280;
		pWinHeight		= 960;
		break;
		// --
	case 7:
		pWinWidth		= 1280;
		pWinHeight		= 1024;
		break;
		// --
	case 8:
		pWinWidth		= 1366;
		pWinHeight		= 768;
		break;
		// --
	case 9:
		pWinWidth		= 1440;
		pWinHeight		= 900;
		break;
		// --
	case 10:
		pWinWidth		= 1600;
		pWinHeight		= 900;
		break;
		// --
	case 11:
		pWinWidth		= 1600;
		pWinHeight		= 1200;
		break;
		// --
	case 12:
		pWinWidth		= 1680;
		pWinHeight		= 1050;
		break;
		// --
	case 13:
		pWinWidth		= 1920;
		pWinHeight		= 1080;
		break;
		// --
	case 14:
		pWinWidth		= 1920;
		pWinHeight		= 1200;
		break;
		// --
	case 15:
		pWinWidth		= 1024;
		pWinHeight		= 600;
		break;
		// --
	case 16:
		pWinWidth		= 1910;
		pWinHeight		= 970;
		break;
		// --
	case 17:
		pWinWidth		= 1350;
		pWinHeight		= 650;
		break;
		// --
	}
	// ----
	pWinFontHeight	= 15;
	pWinWidthReal	= (float)pWinWidth / MAX_WIN_WIDTH;
	pWinHeightReal	= (float)pWinHeight / MAX_WIN_HEIGHT;

	if(pGameResolutionMode >= 5)
	{
		float Aux = abs(GetMemBuffer((DWORD*)(float*) 0x007B3720) - 150) * 3;
		UnProtect((DWORD*)(float*)0x007B5834, 1190.0f + Aux + 3000.0f);
		UnProtect((DWORD*)(float*)0x007B5834, 2400.0f + Aux + 3000.0f);
		UnProtect((DWORD*)(float*)0x007B532C, 3000.0f + Aux + 1500.0f);
	}

	return 1;
	return pGameLoad();
}


void DrawIcons(int x, int y, DWORD c)
{


	DWORD character = *(DWORD*)(c + 636); //Get Character Struct..636

	if (*(BYTE*)(character + 188) == 1) //Check Character Type//196
	{
		if (*(short*)(character + 562) != -1) //Check Character Guild//562
		{
			((void(__cdecl*)(int nMarkIndex, bool blend)) 0x006639D0)(*(WORD*)(character + 562), true); //CreateGuildMark//0x00663A00
			((void(__cdecl*)(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale, float Alpha)) 0x006B97A7)(0x22, ((float)x - 21.0f), (float)y, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, 0); //RenderBitmap//0x006B9987
			
			
		}
	}

	((void(__cdecl*)(int x, int y, DWORD c)) 0x005A21B0)(x, y, c); //RenderBoolean //5A21E0
}

void LevelBoxCreate(int a1, float a2, float a3, float a4, float a5, int a6, int a7, float a8, float a9, char a10, char a11, float a12)
{
	if (a3 == 56.0f)
	{
		pDrawImage2(a1, a2, 36.0f, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}
	
	pDrawImage2(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
}


void LevelBoxDraw(int a1, int a2, LPSTR a3, int a4, int a5, int a6)//vota
{

	pDrawTextOut(a1, a2 - 20, a3, a4, a5, a6);
	char szBuff[50];
	//sprintf_s(szBuff, "Resets: %d", gObjectManager->GetResets());
	sprintf_s(szBuff, "R: %d | Gr: %d", gObjectManager->GetResets(), gObjectManager->GetGResets());
	pDrawTextOut(a1, a2, szBuff, a4, a5, a6);
	
}


__declspec(naked) void FixExperience()
{
	static DWORD FixExperience_Offset01 = 0x004F260D;

	__asm
	{
		MOV ESI, DWORD PTR DS : [EAX + 0x10]
		PUSH EBX
		ADD ESI, AddExperience
		JMP [FixExperience_Offset01]

	}
}
void DrawExperience(LPSTR Dest, LPSTR Format, DWORD Experience)
{
	char szBuff[50];
	sprintf_s(szBuff, (const char*)Format, AddExperience);
	strcpy(Dest, szBuff);
}
void SpeedCalc()
{

	
	//Default
	SetByte((PVOID)(0x0059D9A1 + 1), 80);	//00593C4D + 1 -> str Speed -> 20 //0059D9A1
	SetByte((PVOID)(0x0059D9B8 + 1), 70);	//00593C60 + 1 -> Mag Speed -> 10 //0059D9B8

	//Class 4
	//SetByte((PVOID)(0x0059D971 + 1), 100);	//00593BC6 + 1 -> str Speed -> 10 //0059D971
	//SetByte((PVOID)(0x0059D988 + 1), 100);	//00593BD9 + 1 -> Mag Speed -> 10 //0059D988

	//Class 2
	SetByte((PVOID)(0x0059D8FC + 1), 90);	//00593B5E + 1 -> str Speed -> 50 //0059D8FC
	SetByte((PVOID)(0x0059D913 + 1), 90);	//00593B71 + 1 -> Mag Speed -> 50 //0059D913

	//Class 1 & 3
	SetByte((PVOID)(0x0059D93B + 1), 65);	//00593B95 + 1 -> str Speed -> 15 //0059D93B
	SetByte((PVOID)(0x0059D952 + 1), 70);	//00593BA8 + 1 -> Mag Speed -> 20 //0059D952
	

}
void ZenWarehouse(int a1, float a2, float a3, float a4, float a5, int a6, int a7, float a8, float a9, char a10, char a11, float a12)
{
	pDrawImage2(a1, a2, a3, a4-48, a5-5, a6, a7, a8, a9, a10, a11, a12);
	pDrawImage2(2535,a2+67, a3+1,10,10,0.0,0.0,0.780, 0.900, 1, 1, 0.0);//portal
}
//DWORD FCoin1;
void ZenDraw(int a1, int a2, LPSTR a3, int a4, int a5, int a6)
{
	pDrawTextOut(a1, a2 , a3, a4, a5, a6);
	//glColor3f(0.9254901960784314, 0.8705882352941176, 0.7647058823529412); //LemonChiffon
	glColor3f(0.9450980392156863, 0.8705882352941176, 0.7137254901960784); //White|
	char szBuff[50];
	sprintf_s(szBuff, "Cred");
	pDrawTextOut(a1, a2+13, szBuff, a4, a5, a6);
	char szBuff1[50];
	//sprintf_s(szBuff1, "%d",FCoin1);//,FCoin1);

		if(gMenuSystem.m_info.Credits >= 1000000000)
		{
			glColor3f(1.0f,0.0f, 0.0f);//Red
			//glColor3f(0.6549019607843137, 0.1450980392156863, 0.1450980392156863); //Red
			wsprintf(szBuff1,"%d,%03d,%03d,%03d",gMenuSystem.m_info.Credits/1000000000,gMenuSystem.m_info.Credits%1000000000/1000000,gMenuSystem.m_info.Credits%1000000/1000,gMenuSystem.m_info.Credits%1000);
		}
		else if(gMenuSystem.m_info.Credits >= 1000000)
		{
			if(gMenuSystem.m_info.Credits >= 1000000 && gMenuSystem.m_info.Credits <= 9999999)
			{
				glColor3f(1.0f, 0.5f, 0.0f);//Orange
				//glColor3f(0.8588235294117647, 0.6549019607843137, 0.3529411764705882); //Orange
			}
			else if(gMenuSystem.m_info.Credits >= 10000000)
			{
				glColor3f(1.0f,0.0f, 0.0f);//Red
				//glColor3f(0.6549019607843137, 0.1450980392156863, 0.1450980392156863); //Red
			}
			wsprintf(szBuff1,"%d,%03d,%03d",gMenuSystem.m_info.Credits%1000000000/1000000,gMenuSystem.m_info.Credits%1000000/1000,gMenuSystem.m_info.Credits%1000);
		}
		else if(gMenuSystem.m_info.Credits > 1000)
		{
			if(gMenuSystem.m_info.Credits > 99999)
			{
				//glColor3f(0.2509803921568627, 0.6392156862745098, 0.2823529411764706); //Green
				glColor3f(0.0f, 1.0f, 0.0f);//Green
			}
			else if(gMenuSystem.m_info.Credits > 1000)
			{
				glColor3f(0.9450980392156863, 0.8705882352941176, 0.7137254901960784); //White|
			}
			wsprintf(szBuff1,"%d,%03d",gMenuSystem.m_info.Credits%1000000/1000,gMenuSystem.m_info.Credits%1000);
		}
		else
		{	
			glColor3f(0.9450980392156863, 0.8705882352941176, 0.7137254901960784); //White|
			wsprintf(szBuff1,"%d",gMenuSystem.m_info.Credits%1000);
		}

	pDrawTextOut(a1+40, a2+13, szBuff1, a4, a5, a6);
	glColor3f(1.0f, 1.0f, 1.0f);//Green;
}
void Fix()
{
	SetOp((LPVOID)0x004F27AE, (LPVOID)DrawExperience, ASM::CALL);

	SetRange((LPVOID)0x004F2607, 6, ASM::NOP);
	SetOp((LPVOID)0x004F2607, (LPVOID)FixExperience, ASM::JMP);
	SetByte2((PVOID)(0x0052484A + 6), 2); // Fix Skills Attack Speed Ver 1.0
	//SetByte((PVOID)(0x00649E24 + 3), 14);	//-> 1 Method
	SetOp((LPVOID)0x0065F609, (LPVOID)LevelBoxCreate, ASM::CALL);
	SetOp((LPVOID)0x0065F779, (LPVOID)LevelBoxDraw, ASM::CALL);
	//SpeedCalc();
	//00664B60   E8 5BBCF3FF      CALL Main.005A07C0
	SetOp((LPVOID)0x00664B30, (LPVOID)ZenDraw, ASM::CALL);//664B30
	//SetOp((LPVOID)0x006673C8, (LPVOID)ZenWarehouse, ASM::CALL);
}


//===============================================================
//		Fix Exp
//===============================================================

DWORD EXP_RET = 0x004F247F;
__declspec(naked)void FixExp()
{
	_asm
	{
		MOV EDI,EXP
		JMP EXP_RET
	}
}
void InitFixExp()
{
	SetNop(0x004F247A,5);
	WriteJmp(0x004F247A, (DWORD)&FixExp);
}

int FontSize;

DWORD FontCharSet;

char FontName[100];

int ReadFontFile(char* path) // OK
{	
	FontSize = GetPrivateProfileInt("FontInfo","FontSize",13,path);
	
	FontCharSet = GetPrivateProfileInt("FontInfo","FontCharSet",1,path);
	
	GetPrivateProfileString("FontInfo","FontName","Tahoma",FontName,sizeof(FontName),path);

	return GetPrivateProfileInt("FontInfo","ChangeFontSwitch",0,path);
}

void InitFont() // OK
{
	if(ReadFontFile(".\\Config.ini") != 0)
	{
		SetCompleteHook(0xE8,0x004DD62C,&FontNormal);//004DD62C

		SetCompleteHook(0xE8,0x004DD66C,&FontBool);//004DD66C
		
		SetCompleteHook(0xE8,0x004DD6AE,&FontBig);//004DD6AE
		
		SetCompleteHook(0xE8,0x004DD708,&FontFixed);//004DD708

		SetByte(0x004DD631,0x90);//004DD631
		
		SetByte(0x004DD671,0x90);//004DD671
		
		SetByte(0x004DD6B3,0x90);//004DD6B3
		
		SetByte(0x004DD70D,0x90);//004DD70D
	}
}

void ReloadFont() // OK
{
	DWORD offsetFunc = ((DWORD(*)())0x006FAAE2)();//006FAAE2
	
	offsetFunc = offsetFunc + 0x45C8;

	*(HFONT*)(offsetFunc + 0x183) = FontNormal();

	pFontNormal = FontNormal();

	pFontBold = FontBool();

	pFontBig = FontBig();

	pFontFixed = FontFixed();

	*(DWORD*)MAIN_FONT_SIZE = FontSize;

	((void(*)())0x006B0040)();//006B0040
}

HFONT FontNormal() // OK
{
	return CreateFont(FontSize,0,0,0,FW_NORMAL,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontBool() // OK
{
	return CreateFont(FontSize,0,0,0,FW_BOLD,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontBig() // OK
{
	return CreateFont(FontSize*2,0,0,0,FW_BOLD,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontFixed() // OK
{
	return CreateFont(FontSize,0,0,0,FW_NORMAL,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

void SetCursorFocus(int state)
{
	if (state != 1)
	{
		//Primeira Verificação (Button OK)
		SetDword(0x006C3EE1 + 1, 40);      //40	//006C3EE1
		SetDword(0x006C3EE6 + 1, 323);     //323	//006C3EE6
		SetDword(0x006C3ED5 + 1, 70);      //70	//006C3ED5
		SetDword(0x006C3EDA + 1, 234);     //234	//006C3EDA
		//Segunda Verificação (Button Cancel)
		SetDword(0x006C5D76 + 1, 40); //40		//006C5D76
		SetDword(0x006C5D7B + 1, 373); //373	//006C5D7B
		SetDword(0x006C5D6A + 1, 70); //70		//006C5D6A
		SetDword(0x006C5D6F + 1, 334); //334	//006C5D6F

		SetCompleteHook(0xE8, 0x006CCDA9, 0x006C7450);  //006CCDA9	//006C7450
		SetCompleteHook(0xE8, 0x006CE8AF, 0x006C7450);  //006CE8AF	//006C7450
		SetCompleteHook(0xE8, 0x006CF49D, 0x006C7450);  //006CF49D	//006C7450
		SetCompleteHook(0xE8, 0x006D1441, 0x006C7450);  //006D1441	//006C7450

		pSetCursorFocus = 0;
	}
	else
	{
		pSetCursorFocus = 125;
		MemorySet(0x006CCDA9, 0x90, 0x5);  //DISABLE PSETCURSORFOCUS INTERFACE
		MemorySet(0x006CE8AF, 0x90, 0x5);  //DISABLE PSETCURSORFOCUS INTERFACE
		MemorySet(0x006CF49D, 0x90, 0x5);  //DISABLE PSETCURSORFOCUS INTERFACE
		MemorySet(0x006D1441, 0x90, 0x5);  //DISABLE PSETCURSORFOCUS INTERFACE
		//Primeira Verificação (Button OK)
		SetDword(0x006C3EE1 + 1, 0); //40
		SetDword(0x006C3EE6 + 1, 0); //323
		SetDword(0x006C3ED5 + 1, 0); //70
		SetDword(0x006C3EDA + 1, 0); //234
		//Segunda Verificação (Button Cancel)
		SetDword(0x006C5D76 + 1, 0); //40
		SetDword(0x006C5D7B + 1, 0); //373
		SetDword(0x006C5D6A + 1, 0); //70
		SetDword(0x006C5D6F + 1, 0); //334
	}
}