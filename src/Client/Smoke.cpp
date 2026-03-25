#include "stdafx.h"
#include "Smoke.h"
#include "TMemory.h"
#include "Structure.h"



CItemSmoker  gItemSmoker;


__declspec(naked) void ItemSmokerSetEffect()
{
	static DWORD ItemSmokerSetEffect_Offset01 = 0x0053C0AD;
	static DWORD ItemSmokerSetEffect_Offset02 = 0x0053BA67;

	__asm
	{
		MOV gItemSmoker.SmokeItemType, EAX
	}

	if(gItemSmoker.SmokeItemType == 0)
	{
		_asm
		{
			JMP[ItemSmokerSetEffect_Offset01]
		}
	}
	else
	{
		_asm
		{
			JMP[ItemSmokerSetEffect_Offset02]
		}
	}
}

__declspec(naked) void ItemSmokerSetColor()
{
	static DWORD ItemSmokerSetColor_Offset01 = 0x0053BBB3;

	_asm
	{
		MOV EAX, DWORD PTR SS : [EBP + 0x8]
		MOVSX ECX, WORD PTR DS : [EAX + 0x2D8]
		MOV gItemSmoker.SmokeItemType, ECX
	}
	
	
	for(int i = 0; i < gItemSmoker.SmokeCount; i++)
	{
		if( gItemSmoker.SmokeItemType == ITEM2(11,gItemSmoker.Data[i].Index))
		{
			memcpy(&gItemSmoker.SmokeRed, &gItemSmoker.Data[i].Red, sizeof( float ));
			memcpy(&gItemSmoker.SmokeGreen, &gItemSmoker.Data[i].Green, sizeof( float ));
			memcpy(&gItemSmoker.SmokeBlue, &gItemSmoker.Data[i].Blue, sizeof( float ));
			// ----
			_asm
			{
				MOV EDX, DWORD PTR SS : [EBP + 0x0C]
				MOV ECX, gItemSmoker.SmokeRed
				MOV DWORD PTR DS : [EDX + 0x128], ECX
				MOV EAX, DWORD PTR SS : [EBP + 0x0C]
				MOV EDX, gItemSmoker.SmokeGreen
				MOV DWORD PTR DS : [EAX + 0x12C], EDX
				MOV ECX, DWORD PTR SS : [EBP + 0x0C]
				MOV EAX, gItemSmoker.SmokeBlue
				MOV DWORD PTR DS : [ECX + 0x130], EAX
			}
		}
	}

	_asm
	{
		JMP[ItemSmokerSetColor_Offset01]
	}
}

__declspec(naked) void ItemSmokerSetColor_LevelFix()
{
	static DWORD ItemSmokerSetColor_LevelFix_Offset01 = 0x0053C089;
	static DWORD ItemSmokerSetColor_LevelFix_Offset02 = 0x0053BF76;

	if (*(DWORD*)0x754F4E8 >= 10 && *(DWORD*)0x754F4E8 <= 16)
	{
		_asm
		{
			JMP [ItemSmokerSetColor_LevelFix_Offset02]
		}
	}

	_asm
	{
		JMP[ItemSmokerSetColor_LevelFix_Offset01]
	}
}

void CItemSmoker::Init()
{
	
	this->ReadList(SMOKE_EFFECT_DIR);

	SetRange((LPVOID)0x0053BF69, 7, ASM::NOP);
	SetOp((LPVOID)0x0053BF69, (LPVOID)ItemSmokerSetColor_LevelFix, ASM::JMP);
	
	
		SetRange((LPVOID)0x0053BA46, 8, ASM::NOP);
		SetRange((LPVOID)0x0053BA98, 8, ASM::NOP);
		SetOp((LPVOID)0x0053BA46, (LPVOID)ItemSmokerSetEffect, ASM::JMP);
		SetOp((LPVOID)0x0053BA98, (LPVOID)ItemSmokerSetColor, ASM::JMP);
	
}

CItemSmoker::CItemSmoker()
{
	this->SmokeItemType = 0;
	this->SmokeRed = 0;
	this->SmokeGreen = 0;
	this->SmokeBlue = 0;
	this->SmokeCount = 0;
	ZeroMemory(&Data,sizeof(Data));
}
// ----------------------------------------------------------------------------------------------

CItemSmoker::~CItemSmoker()
{

}
void CItemSmoker::ReadList(char * File)
{
	int Token;
	// ----
	SMDFile = fopen(File, "r");
	// ----
	if (!SMDFile)
	{
		MessageBoxError("%s - file nor found", File);
		ExitProcess(0);
		return;
	}
	// ----
	this->SmokeCount = 0;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END || !strcmp(TokenString, "end") )
		{
			break;
		}
		// ----
		if( this->SmokeCount >= MAX_SMOKER)
		{
			break;
		}
		// ----
		this->Data[this->SmokeCount].Index	= TokenNumber;
		// ----
		GetToken();
		this->Data[this->SmokeCount].Red	= TokenNumber;
		// ----
		GetToken();
		this->Data[this->SmokeCount].Green	= TokenNumber;
		// ----
		GetToken();
		this->Data[this->SmokeCount].Blue	= TokenNumber;

		// ----
		this->SmokeCount++;
	}
	fclose(SMDFile);	
}
// ----------------------------------------------------------------------------------------------