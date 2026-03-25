#include "stdafx.h"
#include "CustomNPC.h"
#include "Other.h"
#include "Structure.h"
#include "TMemory.h"

void NpcSetChat(int a1, int a2)
{
	((void(__cdecl*)(int, int)) 0x526DF4)(a1, a2);
	
	if (*(WORD*)(a1 + 950) == 600)
	{
		((void(__cdecl*)(const char *a1, const char *a2, int a3, int a4, int a5)) 0x5A39E0)((const char *)(a1 + 529), "Become a merciless killer", a1, 0, -1);
		lpViewObj gPreview = &*(ObjectPreview*)a1;
		gPreview->ModelScale = 0x3FA00000;
	}
	
	if (*(WORD*)(a1 + 950) == 602)
	{
		((void(__cdecl*)(const char *a1, const char *a2, int a3, int a4, int a5)) 0x5A39E0)((const char *)(a1 + 529), "Do you have the strength to become a Battle Master?", a1, 0, -1);
		lpViewObj gPreview = &*(ObjectPreview*)a1;
		gPreview->ModelScale = 0x3FA00000;
	}

	if (*(WORD*)(a1 + 950) == 601)
	{
		((void(__cdecl*)(const char *a1, const char *a2, int a3, int a4, int a5)) 0x5A39E0)((const char *)(a1 + 529), "Kill Monsters and Get Prize!", a1, 0, -1);
		lpViewObj gPreview = &*(ObjectPreview*)a1;
		gPreview->ModelScale = 0x3FA00000;
	}

	/*if (*(WORD*)(a1 + 950) == 500)
	{
		((void(__cdecl*)(const char *a1, const char *a2, int a3, int a4, int a5)) 0x5A39E0)((const char *)(a1 + 529), "Give me Trade, and mix Wings", a1, 0, -1);
		lpViewObj gPreview = &*(ObjectPreview*)a1;
		gPreview->ModelScale = 0x3FA00000;
	}*/
}

void __declspec(naked) NpcSetType()
{
	static DWORD NpcSetType_Offset01 = 0x00540ABD;
	static DWORD NpcSetType_Offset02 = 0x00540A7E;

	_asm
	{
		//CMP DWORD PTR SS : [EBP + 0xC], 0x17D
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1E0
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1E1
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1E2
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1E3
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1E4
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1E5
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1E6//486
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1F5//501
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1F6//502
		//JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x1F7//503
		//JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x1F4
		JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x17D
		JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x258
		JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x259//601
		JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x25A//6-2
		JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x25B//6-3
		JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x25C//6-4
		JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x25D//6-5
		JE Allow
		CMP DWORD PTR SS : [EBP + 0xC], 0x25E//6-6
		JE Allow
		//CMP DWORD PTR SS : [EBP + 0xC], 0x25F//6-7
		//JE Allow
		JMP [NpcSetType_Offset02]
		Allow:
		JMP [NpcSetType_Offset01]
	}
	
}

void __declspec(naked) NpcGetObject()
{
	static DWORD NpcGetObject_Offset01 = 0x0054488B;
	static DWORD NpcGetObject_Offset02 = 0x005408AD;

	_asm
	{
		MOV ECX, DWORD PTR SS : [EBP + 0x14]
		PUSH ECX
		MOV EDX, DWORD PTR SS : [EBP + 0x10]
		PUSH EDX
		MOV EAX, DWORD PTR SS : [EBP + 0xC]
		PUSH EAX
		MOV ECX, DWORD PTR SS : [EBP + 0x8]
		PUSH ECX
		CALL [CreateNpc]

		//ADD ESP, 0x08

		MOV EDX, DWORD PTR SS : [EBP + 0x10]
		PUSH EDX
		MOV EAX, DWORD PTR SS : [EBP + 0xC]
		PUSH EAX
		MOV ECX, DWORD PTR SS : [EBP + 0x8]
		PUSH ECX
		MOV EDX, DWORD PTR SS : [EBP - 0x4]
		PUSH EDX
		CALL[NpcGetObject_Offset02]
		JMP[NpcGetObject_Offset01]
	}
}

void NpcManagerInit()
{
	//0053F319 -- Npc Soldier
	SetOp((LPVOID)0x00544876, (LPVOID)NpcGetObject, ASM::JMP);
	SetOp((LPVOID)0x00540A75, (LPVOID)NpcSetType, ASM::JMP);
	SetOp((LPVOID)0x00527761, (LPVOID)NpcSetChat, ASM::CALL);
	SetByte(0x00527459,0xFF); // Monster Kill..
	
	SetByte(0x00527458,0xFF); // Monster Kill//
}

void CreateNpc(int a1, int a2, int a3, int a4)
{
//	int c = 4;//0 default size
//
//	if (a1 >= 501 && a1 <= 503)
//	{
//		c = pCreateMonster(a4, 627, a2, a3, 4);
//		pCreateJoint(1806, (int)(c + 16), (int)(c + 16), (int)(c + 28), 42, (int)c, 15.0, -1, 0, 0, -1, 0);
//
//		lpViewObj gPreview = &*(ObjectPreview*)c;
//		
//		//if (a1 == 480)
//		//{
//		//	strcpy(gPreview->Name, "Npc Buff [Life]");
//
//		//	gPreview->WingsSlot = ITEMGET2(12, 36);
//		//	// ----
//		//	gPreview->HelmSlot = ITEMGET2(7, 29);
//		//	gPreview->HelmLevel = 13;
//		//	gPreview->HelmExcellent = 1;
//		//	// ----
//		//	gPreview->ArmorSlot = ITEMGET2(8, 29);
//		//	gPreview->ArmorLevel = 13;
//		//	gPreview->ArmorExcellent = 1;
//		//	// ----
//		//	gPreview->PantsSlot = ITEMGET2(9, 29);
//		//	gPreview->PantsLevel = 13;
//		//	gPreview->PantsExcellent = 1;
//		//	// ----
//		//	gPreview->GlovesSlot = ITEMGET2(10, 29);
//		//	gPreview->GlovesLevel = 13;
//		//	gPreview->GlovesExcellent = 1;
//		//	// ----
//		//	gPreview->BootsSlot = ITEMGET2(11, 29);
//		//	gPreview->BootsLevel = 13;
//		//	gPreview->BootsExcellent = 1;
//		//}
//		//else if (a1 == 481)
//		//{
//		//	strcpy(gPreview->Name, "Npc Buff [Shield]");
//
//		//	gPreview->WingsSlot = ITEMGET2(12, 37);
//		//	// ----
//		//	gPreview->HelmSlot = ITEMGET2(7, 30);
//		//	gPreview->HelmLevel = 13;
//		//	gPreview->HelmExcellent = 1;
//		//	// ----
//		//	gPreview->ArmorSlot = ITEMGET2(8, 30);
//		//	gPreview->ArmorLevel = 13;
//		//	gPreview->ArmorExcellent = 1;
//		//	// ----
//		//	gPreview->PantsSlot = ITEMGET2(9, 30);
//		//	gPreview->PantsLevel = 13;
//		//	gPreview->PantsExcellent = 1;
//		//	// ----
//		//	gPreview->GlovesSlot = ITEMGET2(10, 30);
//		//	gPreview->GlovesLevel = 13;
//		//	gPreview->GlovesExcellent = 1;
//		//	// ----
//		//	gPreview->BootsSlot = ITEMGET2(11, 30);
//		//	gPreview->BootsLevel = 13;
//		//	gPreview->BootsExcellent = 1;
//		//}
//		//else if (a1 == 482)
//		//{
//		//	strcpy(gPreview->Name, "Npc Buff [Critical]");
//
//		//	gPreview->WingsSlot = ITEMGET2(12, 40);
//		//	// ----
//		//	gPreview->HelmSlot = ITEMGET2(7, 33);
//		//	gPreview->HelmLevel = 13;
//		//	gPreview->HelmExcellent = 1;
//		//	// ----
//		//	gPreview->ArmorSlot = ITEMGET2(8, 33);
//		//	gPreview->ArmorLevel = 13;
//		//	gPreview->ArmorExcellent = 1;
//		//	// ----
//		//	gPreview->PantsSlot = ITEMGET2(9, 33);
//		//	gPreview->PantsLevel = 13;
//		//	gPreview->PantsExcellent = 1;
//		//	// ----
//		//	gPreview->GlovesSlot = ITEMGET2(10, 33);
//		//	gPreview->GlovesLevel = 13;
//		//	gPreview->GlovesExcellent = 1;
//		//	// ----
//		//	gPreview->BootsSlot = ITEMGET2(11, 33);
//		//	gPreview->BootsLevel = 13;
//		//	gPreview->BootsExcellent = 1;
//		//}
//		//else if (a1 == 483)
//		//{
//		//	strcpy(gPreview->Name, "Npc Buff [Attack]");
//
//		//	gPreview->WingsSlot = ITEMGET2(12, 38);
//		//	// ----
//		//	gPreview->HelmSlot = ITEMGET2(7, 31);
//		//	gPreview->HelmLevel = 13;
//		//	gPreview->HelmExcellent = 1;
//		//	// ----
//		//	gPreview->ArmorSlot = ITEMGET2(8, 31);
//		//	gPreview->ArmorLevel = 13;
//		//	gPreview->ArmorExcellent = 1;
//		//	// ----
//		//	gPreview->PantsSlot = ITEMGET2(9, 31);
//		//	gPreview->PantsLevel = 13;
//		//	gPreview->PantsExcellent = 1;
//		//	// ----
//		//	gPreview->GlovesSlot = ITEMGET2(10, 31);
//		//	gPreview->GlovesLevel = 13;
//		//	gPreview->GlovesExcellent = 1;
//		//	// ----
//		//	gPreview->BootsSlot = ITEMGET2(11, 31);
//		//	gPreview->BootsLevel = 13;
//		//	gPreview->BootsExcellent = 1;
//		//}
//		//else if (a1 == 484)
//		//{
//		//	strcpy(gPreview->Name, "Npc Buff [HP/MP]");
//
//		//	gPreview->WingsSlot = ITEMGET2(12, 1);
//		//	// ----
//		//	gPreview->HelmSlot = ITEMGET2(7, 7);
//		//	gPreview->HelmLevel = 13;
//		//	gPreview->HelmExcellent = 1;
//		//	// ----
//		//	gPreview->ArmorSlot = ITEMGET2(8, 7);
//		//	gPreview->ArmorLevel = 13;
//		//	gPreview->ArmorExcellent = 1;
//		//	// ----
//		//	gPreview->PantsSlot = ITEMGET2(9, 7);
//		//	gPreview->PantsLevel = 13;
//		//	gPreview->PantsExcellent = 1;
//		//	// ----
//		//	gPreview->GlovesSlot = ITEMGET2(10, 7);
//		//	gPreview->GlovesLevel = 13;
//		//	gPreview->GlovesExcellent = 1;
//		//	// ----
//		//	gPreview->BootsSlot = ITEMGET2(11, 7);
//		//	gPreview->BootsLevel = 13;
//		//	gPreview->BootsExcellent = 1;
//		//}
//		//else 
//	if (a1 == 501)
//		{
//			strcpy(gPreview->Name, "Murder Trainer");
//
//			gPreview->WingsSlot = ITEMGET2(12, 36);
//			// ----
//			gPreview->WeaponFirstSlot = ITEMGET2(0, 25);
//			gPreview->WeaponFirstLevel = 13;
//			gPreview->WeaponFirstExcellent = 1;
//			// ----
//			gPreview->WeaponSecondSlot = ITEMGET2(0, 25);
//			gPreview->WeaponSecondLevel = 13;
//			gPreview->WeaponSecondExcellent = 1;
//			// ----
//			gPreview->HelmSlot = ITEMGET2(7, 1);
//			gPreview->HelmLevel = 13;
//			gPreview->HelmExcellent = 1;
//			// ----
//			gPreview->ArmorSlot = ITEMGET2(8, 1);
//			gPreview->ArmorLevel = 13;
//			gPreview->ArmorExcellent = 1;
//			// ----
//			gPreview->PantsSlot = ITEMGET2(9, 1);
//			gPreview->PantsLevel = 13;
//			gPreview->PantsExcellent = 1;
//			// ----
//			gPreview->GlovesSlot = ITEMGET2(10, 1);
//			gPreview->GlovesLevel = 13;
//			gPreview->GlovesExcellent = 1;
//			// ----
//			gPreview->BootsSlot = ITEMGET2(11, 1);
//			gPreview->BootsLevel = 13;
//			gPreview->BootsExcellent = 1;
//		}
//	else if (a1 == 502)
//	{
//		strcpy(gPreview->Name, "Quest Master");
//
//		//gPreview->WingsSlot = ITEMGET2(12, 36);
//		// ----
//		gPreview->WeaponFirstSlot = ITEMGET2(2, 12);
//		gPreview->WeaponFirstLevel = 13;
//		gPreview->WeaponFirstExcellent = 1;
//		// ----
//		gPreview->WeaponSecondSlot = ITEMGET2(6, 16);
//		gPreview->WeaponSecondLevel = 13;
//		gPreview->WeaponSecondExcellent = 1;
//		// ----
//		gPreview->HelmSlot = ITEMGET2(7, 33);
//		gPreview->HelmLevel = 13;
//		gPreview->HelmExcellent = 1;
//		// ----
//		gPreview->ArmorSlot = ITEMGET2(8, 33);
//		gPreview->ArmorLevel = 13;
//		gPreview->ArmorExcellent = 1;
//		// ----
//		gPreview->PantsSlot = ITEMGET2(9, 33);
//		gPreview->PantsLevel = 13;
//		gPreview->PantsExcellent = 1;
//		// ----
//		gPreview->GlovesSlot = ITEMGET2(10, 33);
//		gPreview->GlovesLevel = 13;
//		gPreview->GlovesExcellent = 1;
//		// ----
//		gPreview->BootsSlot = ITEMGET2(11, 33);
//		gPreview->BootsLevel = 13;
//		gPreview->BootsExcellent = 1;
//	}
//	else if (a1 == 503)
//	{
//		strcpy(gPreview->Name, "Battle Master");
//
//		gPreview->WingsSlot = ITEMGET2(12, 37);
//		// ----
//		gPreview->WeaponFirstSlot = ITEMGET2(5, 12);
//		gPreview->WeaponFirstLevel = 13;
//		gPreview->WeaponFirstExcellent = 1;
//		// ----
//		gPreview->WeaponSecondSlot = ITEMGET2(6, 15);
//		gPreview->WeaponSecondLevel = 13;
//		gPreview->WeaponSecondExcellent = 1;
//		// ----
//		gPreview->HelmSlot = ITEMGET2(7, 30);
//		gPreview->HelmLevel = 13;
//		gPreview->HelmExcellent = 1;
//		// ----
//		gPreview->ArmorSlot = ITEMGET2(8, 30);
//		gPreview->ArmorLevel = 13;
//		gPreview->ArmorExcellent = 1;
//		// ----
//		gPreview->PantsSlot = ITEMGET2(9, 30);
//		gPreview->PantsLevel = 13;
//		gPreview->PantsExcellent = 1;
//		// ----
//		gPreview->GlovesSlot = ITEMGET2(10, 30);
//		gPreview->GlovesLevel = 13;
//		gPreview->GlovesExcellent = 1;
//		// ----
//		gPreview->BootsSlot = ITEMGET2(11, 30);
//		gPreview->BootsLevel = 13;
//		gPreview->BootsExcellent = 1;
//	}
//		gPreview->Action = 9;
//		gPreview->ModelScale = 0x3F800000;
//
//		gPreview->SelectZone += 70.0;
//		
//	}
//	else if (a1 == 415)
//	{
//		pLoadMonsterModel(415);
//		c = pCreateMonster(a4, 415  + 404, a2, a3, 0);
//
//		lpViewObj gPreview = &*(ObjectPreview*)c;
//		strcpy(gPreview->Name, "Scorpion");
//	}
//	else 
//if (a1 == 496)
//	{
//		pLoadMonsterModel(497);
//		c = pCreateMonster(a4, 497  + 404, a2, a3, 0);
//
//		lpViewObj gPreview = &*(ObjectPreview*)c;
//		strcpy(gPreview->Name, "Captain Enginer");
//	}
//	else if (a1 == 498)
//	{
//		pLoadMonsterModel(496);
//		c = pCreateMonster(a4, 496 + 404, a2, a3, 1);
//
//		lpViewObj gPreview = &*(ObjectPreview*)c;
//		strcpy(gPreview->Name, "Mad MAX");
//	}
//	else if (a1 == 499)
//	{
//		pLoadMonsterModel(499);
//		c = pCreateMonster(a4, 499 + 404, a2, a3, 0);
//
//		lpViewObj gPreview = &*(ObjectPreview*)c;
//		strcpy(gPreview->Name, "Viperia");
//	}
//	//else if (a1 == 490)
//	//{
//	//	pLoadMonsterModel(154);
//	//	c = pCreateMonster(a4, 154 + 404, a2, a3, 0);
//
//	//	lpViewObj gPreview = &*(ObjectPreview*)c;
//	//	//strcpy(gPreview->Name, "Moss Merchant");
//	//}
//}
	int c = 0;//0 default size

	/*if (a1 >= 600 && a1 <= 602)
	{
		c = pCreateMonster(a4, 627, a2, a3, 4);
		pCreateJoint(1806, (int)(c + 16), (int)(c + 16), (int)(c + 28), 42, (int)c, 15.0, -1, 0, 0, -1, 0);

		lpViewObj gPreview = &*(ObjectPreview*)c;*/
		
	//if (a1 == 600)
	//	{
	//		strcpy(gPreview->Name, "Murder Trainer");

	//		gPreview->WingsSlot = ITEMGET2(12, 36);
	//		// ----
	//		gPreview->WeaponFirstSlot = ITEMGET2(0, 25);
	//		gPreview->WeaponFirstLevel = 13;
	//		gPreview->WeaponFirstExcellent = 1;
	//		// ----
	//		gPreview->WeaponSecondSlot = ITEMGET2(0, 25);
	//		gPreview->WeaponSecondLevel = 13;
	//		gPreview->WeaponSecondExcellent = 1;
	//		// ----
	//		gPreview->HelmSlot = ITEMGET2(7, 1);
	//		gPreview->HelmLevel = 13;
	//		gPreview->HelmExcellent = 1;
	//		// ----
	//		gPreview->ArmorSlot = ITEMGET2(8, 1);
	//		gPreview->ArmorLevel = 13;
	//		gPreview->ArmorExcellent = 1;
	//		// ----
	//		gPreview->PantsSlot = ITEMGET2(9, 1);
	//		gPreview->PantsLevel = 13;
	//		gPreview->PantsExcellent = 1;
	//		// ----
	//		gPreview->GlovesSlot = ITEMGET2(10, 1);
	//		gPreview->GlovesLevel = 13;
	//		gPreview->GlovesExcellent = 1;
	//		// ----
	//		gPreview->BootsSlot = ITEMGET2(11, 1);
	//		gPreview->BootsLevel = 13;
	//		gPreview->BootsExcellent = 1;
	//	}
	//else if (a1 == 601)
	//{
	//	strcpy(gPreview->Name, "Quest Master");

	//	//gPreview->WingsSlot = ITEMGET2(12, 36);
	//	// ----
	//	gPreview->WeaponFirstSlot = ITEMGET2(2, 12);
	//	gPreview->WeaponFirstLevel = 13;
	//	gPreview->WeaponFirstExcellent = 1;
	//	// ----
	//	gPreview->WeaponSecondSlot = ITEMGET2(6, 16);
	//	gPreview->WeaponSecondLevel = 13;
	//	gPreview->WeaponSecondExcellent = 1;
	//	// ----
	//	gPreview->HelmSlot = ITEMGET2(7, 33);
	//	gPreview->HelmLevel = 13;
	//	gPreview->HelmExcellent = 1;
	//	// ----
	//	gPreview->ArmorSlot = ITEMGET2(8, 33);
	//	gPreview->ArmorLevel = 13;
	//	gPreview->ArmorExcellent = 1;
	//	// ----
	//	gPreview->PantsSlot = ITEMGET2(9, 33);
	//	gPreview->PantsLevel = 13;
	//	gPreview->PantsExcellent = 1;
	//	// ----
	//	gPreview->GlovesSlot = ITEMGET2(10, 33);
	//	gPreview->GlovesLevel = 13;
	//	gPreview->GlovesExcellent = 1;
	//	// ----
	//	gPreview->BootsSlot = ITEMGET2(11, 33);
	//	gPreview->BootsLevel = 13;
	//	gPreview->BootsExcellent = 1;
	//}
	//else if (a1 == 602)
	//{
	//	strcpy(gPreview->Name, "Battle Master");

	//	gPreview->WingsSlot = ITEMGET2(12, 37);
	//	// ----
	//	gPreview->WeaponFirstSlot = ITEMGET2(5, 12);
	//	gPreview->WeaponFirstLevel = 13;
	//	gPreview->WeaponFirstExcellent = 1;
	//	// ----
	//	gPreview->WeaponSecondSlot = ITEMGET2(6, 15);
	//	gPreview->WeaponSecondLevel = 13;
	//	gPreview->WeaponSecondExcellent = 1;
	//	// ----
	//	gPreview->HelmSlot = ITEMGET2(7, 30);
	//	gPreview->HelmLevel = 13;
	//	gPreview->HelmExcellent = 1;
	//	// ----
	//	gPreview->ArmorSlot = ITEMGET2(8, 30);
	//	gPreview->ArmorLevel = 13;
	//	gPreview->ArmorExcellent = 1;
	//	// ----
	//	gPreview->PantsSlot = ITEMGET2(9, 30);
	//	gPreview->PantsLevel = 13;
	//	gPreview->PantsExcellent = 1;
	//	// ----
	//	gPreview->GlovesSlot = ITEMGET2(10, 30);
	//	gPreview->GlovesLevel = 13;
	//	gPreview->GlovesExcellent = 1;
	//	// ----
	//	gPreview->BootsSlot = ITEMGET2(11, 30);
	//	gPreview->BootsLevel = 13;
	//	gPreview->BootsExcellent = 1;
	//}
	//	gPreview->Action = 9;
	//	gPreview->ModelScale = 0x3F800000;

	//	gPreview->SelectZone += 70.0;
	//	
	//}
	/*else if (a1 == 516)
	{
		pLoadMonsterModel(129);
		c = pCreateMonster(a4, 129  + 404, a2, a3, 0);

		lpViewObj gPreview = &*(ObjectPreview*)c;
		strcpy(gPreview->Name, "Scorpion");
		gPreview->WingsSlot = ITEMGET2(12, 37);
		gPreview->Action = 9;
		gPreview->ModelScale = 0x3F800000;

		gPreview->SelectZone += 70.0;
	}
	else */

if (a1 == 651)
	{
		pLoadMonsterModel(107);
		c = pCreateMonster(a4, 107  + 404, a2, a3, 0);


		sprintf((char*)(c + 529), "Splinter Wolf");//529
		*(float *)(c + 12) = 0.8;//Size
	}
if (a1 == 515)
	{
		pLoadMonsterModel(165);
		c = pCreateMonster(a4, 165  + 404, a2, a3, 0);


		sprintf((char*)(c + 529), "Scorpion");//529
		*(float *)(c + 12) = 1.0;//Size
	}
if (a1 == 516)
	{
		pLoadMonsterModel(126+1);//bmd128
		c = pCreateMonster(a4, 127  + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "Lucky Rabbit");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 696)
	{
		pLoadMonsterModel(128+1);//130 bmd
		c = pCreateMonster(a4, 129 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "Captain Enginer");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 698)
	{
		pLoadMonsterModel(127+1);//129 bmd
		c = pCreateMonster(a4, 128 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "Mad MAX");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 699)
	{
		pLoadMonsterModel(130);//131bmd
		c = pCreateMonster(a4, 130 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "Viperia");
		*(float *)(c + 12) = 0.9;//Size
	}
//npcs
	else if (a1 == 600)//
	{
		pLoadMonsterModel(131);//132bmd
		c = pCreateMonster(a4, 131 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "KillQuest");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 601)//
	{
		pLoadMonsterModel(132);//133bmd
		c = pCreateMonster(a4, 132 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "MonQuest");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 602)//
	{
		pLoadMonsterModel(133);//134bmd
		c = pCreateMonster(a4, 133 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "BMQuest");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 603)//
	{
		pLoadMonsterModel(134);//135bmd
		c = pCreateMonster(a4, 134 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "Carnage");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 604)//
	{
		pLoadMonsterModel(135);//136bmd
		c = pCreateMonster(a4, 135 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "Dungeon Siege");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 605)
	{
		pLoadMonsterModel(136);//137bmd
		c = pCreateMonster(a4, 136 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "SkyEvent");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 606)//
	{
		pLoadMonsterModel(137);//138bmd
		c = pCreateMonster(a4, 137 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "TvTEvent");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 700)
	{
		pLoadMonsterModel(139);//140bmd
		c = pCreateMonster(a4, 139 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "Slayer");
		*(float *)(c + 12) = 1.0;//Size
	}
	else if (a1 == 701)//
	{
		pLoadMonsterModel(138);//139bmd
		c = pCreateMonster(a4, 138 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "THANOS");
		*(float *)(c + 12) = 1.1;//Size
	}
	else if (a1 == 500)//
	{
		pLoadMonsterModel(140);//141bmd
		c = pCreateMonster(a4, 140 + 404, a2, a3, 0);

		sprintf((char*)(c + 529), "Moss Merchant");
		*(float *)(c + 12) = 1.1;//Size
	}

}


//CMonsterEx g_MonsterEx;
//
//CMonsterEx::CMonsterEx()
//{
//	this->Init();
//}
//
//CMonsterEx::~CMonsterEx()
//{
//}
//void CMonsterEx::Init()
//{
//	for (int i = 0; i < 512; i++)
//	{
//		this->m_Data[i].Index = -1;
//		this->m_Data[i].Type = -1;
//		memset(this->m_Data[i].Name, 0, sizeof(this->m_Data[i].Name));
//		memset(this->m_Data[i].Folder, 0, sizeof(this->m_Data[i].Folder));
//		memset(this->m_Data[i].BMDFile, 0, sizeof(this->m_Data[i].BMDFile));
//		this->m_Data[i].Size = 0.0;
//	}
//};
//void CMonsterEx::Read(char* filename)
//{
//	int Token;
//
//	SMDFile = fopen(filename, "r");
//
//	if (!SMDFile)
//	{
//		ExitProcess(0);
//		return;
//	}
//
//	this->Count = 0;
//
//	while (true)
//	{
//		Token = GetToken();
//
//		if (Token == END || !strcmp(TokenString, "end"))
//		{
//			break;
//		}
//
//		if (Count >= 512)
//		{
//			break;
//		}
//
//		this->m_Data[Count].Index = Count;
//
//		Token = GetToken();
//		this->m_Data[Count].Type = TokenNumber;
//
//		Token = GetToken();
//		memcpy(this->m_Data[Count].Name, TokenString, sizeof(this->m_Data[Count].Name) - 1);
//
//		Token = GetToken();
//		memcpy(this->m_Data[Count].Folder, TokenString, sizeof(this->m_Data[Count].Folder) - 1);
//
//		Token = GetToken();
//		memcpy(this->m_Data[Count].BMDFile, TokenString, sizeof(this->m_Data[Count].BMDFile) - 1);
//
//		Token = GetToken();
//		this->m_Data[Count].Size = TokenNumber;
//
//		this->Count++;
//	}
//
//	fclose(SMDFile);
//}
//
//void CMonsterEx::InitMonster() // OK
//{
//	this->Init();
//
//	this->Read(".\\Data\\Customs\\Configs\\Monster.bmd");
//
//
//
//	SetCompleteHook(0xE8,0x00540B2D,&MonsterRender);//
//	
//	SetCompleteHook(0xE8,0x00540B4E,&MonsterSetup);//
//
//	//SetOp((LPVOID)0x00911951, this->MonsterRender, ASM::CALL);
//	//SetOp((LPVOID)0x0091196B, this->MonsterSetup, ASM::CALL);
//}
//
//DWORD CMonsterEx::MonsterRender(int index,int x,int y,int key) // OK
//{
//	
//	//CUSTOM_MONSTER_INFO* lpInfo = gCustomMonster.GetInfoByIndex(index);
//	for (int i = 0; i < g_MonsterEx.Count; i++)
//	{
//		g_MonsterEx.m_Data[i].Index =index;
//
//
//		if(g_MonsterEx.m_Data[i].Index  != 0)
//		{
//			if(g_MonsterEx.m_Data[i].Type != 0)
//			{
//				index += 404;
//			}
//
//			DWORD ViewportAddress = 240 * index + *(DWORD*)0x0587E5A0;//
//
//			if(*(WORD*)(ViewportAddress + 38) <= 0)
//			{
//				char path[MAX_PATH] = {0};
//
//				wsprintf(path,"Data\\%s",g_MonsterEx.m_Data[i].Folder);
//
//				//pLoadItemModel(index,path,g_MonsterEx.m_Data[i].BMDFile,-1);
//
//				if(g_MonsterEx.m_Data[i].Type == 0)
//				{
//					for(int i=0;i < *(WORD*)(ViewportAddress + 38);++i)
//					{
//						*(float*)(*(DWORD*)(ViewportAddress + 48) + 16 * i + 4) = 0.25f;
//					}
//				}
//				else
//				{
//					*(float*)(*(DWORD*)(ViewportAddress + 48) + 4) = 0.25f;
//					*(float*)(*(DWORD*)(ViewportAddress + 48) + 20) = 0.2f;
//					*(float*)(*(DWORD*)(ViewportAddress + 48) + 36) = 0.34f;
//					*(float*)(*(DWORD*)(ViewportAddress + 48) + 52) = 0.33000001f;
//					*(float*)(*(DWORD*)(ViewportAddress + 48) + 68) = 0.33000001f;
//					*(float*)(*(DWORD*)(ViewportAddress + 48) + 84) = 0.5f;
//					*(float*)(*(DWORD*)(ViewportAddress + 48) + 100) = 0.55000001f;
//					*(BYTE*)(*(DWORD*)(ViewportAddress + 48) + 96) = 1;
//				}
//			}
//
//			//pLoadItemTexture(index,g_MonsterEx.m_Data[i].Folder,GL_REPEAT,GL_NEAREST,GL_TRUE);
//
//			return ((DWORD(__cdecl*)(int,int,int,int,float))0x0053E757)(key,index,x,y,0);//
//		}
//
//	}
//
//	return ((DWORD(__cdecl*)(int,int,int,int))0x00453816)(index,x,y,key);//
//
//}
//
//DWORD CMonsterEx::MonsterSetup(DWORD address,int index) // OK
//{
//	//CUSTOM_MONSTER_INFO* lpInfo = gCustomMonster.GetInfoByIndex(index);
//	for (int i = 0; i < g_MonsterEx.Count; i++)
//	{
//		//index = g_MonsterEx.m_Data[i].Index ;
//		g_MonsterEx.m_Data[i].Index =index;
//		if(g_MonsterEx.m_Data[i].Index  != 0)
//		{
//			*(float*)(address + 12) = g_MonsterEx.m_Data[i].Size;//12
//
//			*(BYTE*)(address + 188) = (g_MonsterEx.m_Data[i].Type == 0) ? 4 : 2;//188
//
//			memcpy((DWORD*)(address + 529),g_MonsterEx.m_Data[i].Name,sizeof(g_MonsterEx.m_Data[i].Name));//529
//
//			*(DWORD*)(address + 942) = (g_MonsterEx.m_Data[i].Type  == 2) ? 43 : index;//950
//
//			return address;
//		}
//		
//	}
//
//	return ((DWORD(__cdecl*)(DWORD,int))0x005408AD)(address,index);//
//
//}