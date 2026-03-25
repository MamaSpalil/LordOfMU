#include "stdafx.h"
#include "CustomFenrir.h"
#include "Protocol.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Structure.h"

CCustomFenrir gCustomFenrir;

CCustomFenrir::CCustomFenrir()
{
	for (int i = 0; i < MAX_FENRIR_INFO; i++)
	{
		this->m_CustomFenrirInfo[i].ExeOption = -1;
	}

	this->Init();
}

CCustomFenrir::~CCustomFenrir()
{

}

void CCustomFenrir::Load(CUSTOM_FENRIR_INFO* info)
{
	for (int i = 0; i < MAX_FENRIR_INFO; i++)
	{
		this->SetInfo(info[i]);
	}
}

void CCustomFenrir::SetInfo(CUSTOM_FENRIR_INFO info)
{
	if (info.ExeOption < 0 || info.ExeOption >= MAX_FENRIR_INFO)
	{
		return;
	}

	this->m_CustomFenrirInfo[info.ExeOption] = info;
}

void CCustomFenrir::Init()
{
	SetCompleteHook(0xE9, 0x00540178, 0x00540302); //Skip charge of fenrir charset//00540178	00540302

	SetByte(0x0054017D, 0x90); //Skip charge of fenrir charset//0054017D

	SetCompleteHook(0xE9, 0x006229A6, &this->CustomFenrirSkin); //Show fenrir texture//006229A6

	SetCompleteHook(0xE9, 0x00620B16, &this->CustomFenrirVisualBug); //Show fenrir texture//00620B16

	SetCompleteHook(0xE9, 0x004E968F, &this->CustomFenrirChangeFix); //Fix fenrir texture on change//004E968F

	SetCompleteHook(0xE9, 0x00616D5E, &this->CustomFenrirName); //Change fenrir name//00616D5E

	SetCompleteHook(0xE9, 0x0061B137, &this->CustomFenrirOptions); //Add golden fenrir options//0061B137

	SetCompleteHook(0xE9, 0x0045E13B, &this->CustomFenrirMovement); //Set fenrir movement//0045E13B

	SetCompleteHook(0xE9, 0x0045DDE8, &this->CustomFenrirScale1); //Adapt fenrir scale//0045DDE8

	SetByte(0x0045DDF4, 0x6F); //Skip original fenrir scale//0045DDF4 //xz

	SetByte(0x0045DDF8, 0x7F); //Skip original fenrir scale//0045DDF8

	SetByte(0x0045DDF9, 0x20); //Skip original fenrir scale//0045DDF9

	SetCompleteHook(0xE9, 0x0045FD63, &this->CustomFenrirScale2); //Fix custom fenrir scale and select character scale//0045FD63

	SetByte(0x0045FD6B, 0x50); //Skip original select character scale//0045FD6B

	SetCompleteHook(0xE9, 0x0056848E, &this->CustomFenrirSkillColor); //Change fenrir skill and color//0056848E

	SetCompleteHook(0xE9, 0x0067D279, &this->CustomFenrirEffect); //Fix custom fenrir spark effect//0067D279
	//#define pMakePreviewCharSet		((void(__cdecl*)(int Key, BYTE* Equipment, DWORD pCharacter, DWORD pHelper)) 0x52CB8F)//0053FD2F
	SetCompleteHook(0xE8, 0x004C0BD4, &this->ChangeCharacterExt); //CUIPhotoViewer::SetEquipmentPacket//004C0BD4

	SetCompleteHook(0xE8, 0x004E2EC7, &this->ChangeCharacterExt); //ReceiveCharacterList//004E2EC7

	SetCompleteHook(0xE8, 0x004E9135, &this->ChangeCharacterExt); //ReceiveEquipment//004E9135

	SetCompleteHook(0xE8, 0x004EA2E0, &this->ChangeCharacterExt); //ReceiveCreatePlayerViewport//004EA2E0

	SetCompleteHook(0xE8, 0x004EAC11, &this->ChangeCharacterExt); //ReceiveCreateTransformViewport//004EAC11

	SetCompleteHook(0xE8, 0x00507F68, &this->ChangeCharacterExt); //ReceivePreviewPort//00507F68

SetCompleteHook(0xE8, 0x0050A7DF, &this->ChangeCharacterExt); //ReceiveEquipment//0050A7DF

	//SetCompleteHook(0xE9, 0x0054017E, &this->FenrirMakePreviewCharSet);
}

_declspec(naked) void CCustomFenrir::CustomFenrirSkin()
{
	static DWORD jmpBack = 0x00622A44;//00622A44
	static BYTE FenrirExeOption;
	static int FenrirTexture;

	_asm
	{
		Mov Al, Byte Ptr Ds : [0x079F5AD3] ;//79F5AD3
		Mov FenrirExeOption, Al;
	}

	if (gCustomFenrir.m_CustomFenrirInfo[FenrirExeOption].ExeOption != -1)
	{																				//754F4D0				//754F4D0
		CreateBug(gCustomFenrir.m_CustomFenrirInfo[FenrirExeOption].Texture, *(DWORD*)0x0754F4D0 + 16, *(DWORD*)0x0754F4D0, 0, 0);
	}
	else
	{
		//0x169						//754F4D0				//754F4D0
		CreateBug(0x169, *(DWORD*)0x0754F4D0 + 16, *(DWORD*)0x0754F4D0, 0, 0);
	}

	_asm
	{
		Jmp[jmpBack];
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirVisualBug()
{
	static DWORD jmpBack = 0x00620B6E;//00620B6E
	static BYTE FenrirExeOption;
	static int FenrirTexture;

	_asm
	{
		Mov Al, Byte Ptr Ds : [Edi + 0x1B] ;//1B
		Mov Byte Ptr Ds : [Esi + 0x35B] , Al;//35B
		Mov FenrirExeOption, Al;
	}

	if (gCustomFenrir.m_CustomFenrirInfo[FenrirExeOption].ExeOption != -1)
	{																				//754F4D0				//754F4D0
		CreateBug(gCustomFenrir.m_CustomFenrirInfo[FenrirExeOption].Texture, *(DWORD*)0x0754F4D0 + 16, *(DWORD*)0x0754F4D0, 0, 0);
	}
	else
	{//			//169			//754F4D0					//754F4D0
		CreateBug(0x169, *(DWORD*)0x0754F4D0 + 16, *(DWORD*)0x0754F4D0, 0, 0);
	}

	_asm
	{
		Mov Ecx, Dword Ptr Ds : [0x0754F4D0] ;//754F4D0
		Jmp[jmpBack];
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirChangeFix()
{
	static DWORD jmpBack = 0x004E96DF;//004E96DF
	static BYTE FenrirExeOption;
	static int FenrirTexture;
	static DWORD Struct;

	_asm
	{
		Mov Byte Ptr Ds : [Esi + 0x35B] , Bl;//35B
		Mov FenrirExeOption, Bl;
		Mov Struct, Esi;
	}

	if (gCustomFenrir.m_CustomFenrirInfo[FenrirExeOption].ExeOption != -1)
	{
		CreateBug(gCustomFenrir.m_CustomFenrirInfo[FenrirExeOption].Texture, Struct + 16, Struct, 0, 0);
	}
	else
	{
		CreateBug(0x169, Struct + 16, Struct, 0, 0);//169
	}

	_asm
	{
		Jmp[jmpBack];
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirName()
{
	static DWORD jmpBack = 0x00617034;//00617034
	static BYTE m_OptionExe;

	_asm
	{
		Pushad;
		Mov m_OptionExe, Cl;
	}

	if (gCustomFenrir.m_CustomFenrirInfo[m_OptionExe].ExeOption != -1)
	{
										//7A27BF8		//79F4F00	
		gTextDescription(100 * (*(DWORD*)0x7A27BF8) + 0x79F4F00, gCustomFenrir.m_CustomFenrirInfo[m_OptionExe].PetName);
	}
	else
	{		//							//7A27BF8		//79F4F00				//78C5234		//180	//
		gTextDescription(100 * (*(DWORD*)0x7A27BF8) + 0x79F4F00, (char*)(*(DWORD*)0x78C5234 + 84 * 6693));//6246
	}

	_asm
	{
		Popad;
		Jmp jmpBack;
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirOptions()
{
	static DWORD CustomFenrirOptionsAddress1 = 0x0061B140;//0061B140
	static DWORD CustomFenrirOptionsAddress2 = 0x0061B302;//0061B302
	static BYTE m_OptionExe;

	_asm
	{
		Pushad;
		Mov m_OptionExe, Cl;
	}

	if (gCustomFenrir.m_CustomFenrirInfo[m_OptionExe].ExeOption != -1 && gCustomFenrir.m_CustomFenrirInfo[m_OptionExe].ExeOption != 0 && gCustomFenrir.m_CustomFenrirInfo[m_OptionExe].ExeOption != 1 && gCustomFenrir.m_CustomFenrirInfo[m_OptionExe].ExeOption != 2)
	{
		_asm
		{
			Popad;
			Jmp CustomFenrirOptionsAddress1;
		}
	}

	_asm
	{
		Popad;
		Jmp CustomFenrirOptionsAddress2;
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirMovement()
{
	static DWORD CustomFenrirMovementAddress1 = 0x0045E171;//0045E171
	static DWORD CustomFenrirMovementAddress2 = 0x0045F91D;//45F91D
	static DWORD i;
	static DWORD TextureIndex;

	_asm
	{
		Pushad;
		Mov Eax, Dword Ptr Ss : [Ebp - 0x124] ;
		Mov TextureIndex, Eax;
	}

	for (i = 0; i < MAX_FENRIR_INFO; i++)
	{
		if (gCustomFenrir.m_CustomFenrirInfo[i].ExeOption != -1)
		{
			if (gCustomFenrir.m_CustomFenrirInfo[i].Texture == TextureIndex)
			{
				_asm
				{
					Popad;
					jmp CustomFenrirMovementAddress1;
				}
			}
		}
	}

	_asm
	{
		Popad;
		jmp CustomFenrirMovementAddress2;
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirScale1()
{
	static DWORD CustomFenrirScaleAddress1 = 0x0045DF7D;//0045DF7D
	static DWORD CustomFenrirScaleAddress2 = 0x0045DDF1;//0045DDF1
	static DWORD i;
	static DWORD TextureIndex;
	static float FenrirScale;

	_asm
	{
		Pushad;
		Mov TextureIndex, Edx;
	}

	for (i = 0; i < MAX_FENRIR_INFO; i++)
	{
		if (gCustomFenrir.m_CustomFenrirInfo[i].ExeOption == -1)
		{
			continue;
		}

		if (gCustomFenrir.m_CustomFenrirInfo[i].Texture != TextureIndex)
		{
			continue;
		}

		FenrirScale = gCustomFenrir.m_CustomFenrirInfo[i].ModelScale;

		_asm
		{
			Mov Eax, [Ebp + 0x14];
			Mov Ebx, FenrirScale;
			Mov Dword Ptr Ds : [Eax + 0xC] , Ebx;
			Popad;
			jmp CustomFenrirScaleAddress1;
		}
	}

	_asm
	{
		Popad;
		jmp CustomFenrirScaleAddress2;
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirScale2()
{
	static DWORD CustomFenrirScaleAddress1 = 0x0045FDC9;//0045FDC9
	static DWORD CustomFenrirScaleAddress2 = 0x0045FD6A;//0045FD6A
	static DWORD i;
	static DWORD TextureIndex;

	_asm
	{
		Pushad;
		Mov Eax, [Ebp + 0x08];
		Movsx Eax, Word Ptr Ss : [Eax + 2] ;
		Mov TextureIndex, Ecx;
	}

	for (i = 0; i < MAX_FENRIR_INFO; i++)
	{
		if (gCustomFenrir.m_CustomFenrirInfo[i].ExeOption == -1)
		{
			continue;
		}

		if (gCustomFenrir.m_CustomFenrirInfo[i].Texture != TextureIndex)
		{
			continue;
		}

		_asm
		{
			Popad;
			jmp CustomFenrirScaleAddress1;
		}
	}

	_asm
	{
		Popad;
		Cmp Dword Ptr Ds : [0x007E8014] , 4;//7E8014
		jmp CustomFenrirScaleAddress2;
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirSkillColor()
{
	static DWORD CustomFenrirSkillColorAddress1 = 0x00568633;//00568633
	static DWORD CustomFenrirSkillColorAddress2 = 0x00568495;//00568495
	static DWORD structure;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Ebp - 0x08] ;
		Pushad;
		Mov structure, Eax;
	}

	if (gCustomFenrir.m_CustomFenrirInfo[*(BYTE*)0x079F5AD3].ExeOption != -1)//79F5AD3
	{
		*(DWORD*)(structure + 0x8) = gCustomFenrir.m_CustomFenrirInfo[*(BYTE*)0x079F5AD3].SkillID;//79F5AD3

		*(float*)(structure + 0x48) = gCustomFenrir.m_CustomFenrirInfo[*(BYTE*)0x079F5AD3].SkillR;//79F5AD3

		*(float*)(structure + 0x4C) = gCustomFenrir.m_CustomFenrirInfo[*(BYTE*)0x079F5AD3].SkillG;//79F5AD3

		*(float*)(structure + 0x50) = gCustomFenrir.m_CustomFenrirInfo[*(BYTE*)0x079F5AD3].SkillB;//79F5AD3

		_asm
		{
			Popad;
			jmp CustomFenrirSkillColorAddress1;
		}
	}

	_asm
	{
		Popad;
		Cmp Dword Ptr Ds : [Eax + 0xC] , 0;
		jmp CustomFenrirSkillColorAddress2;
	}
}

_declspec(naked) void CCustomFenrir::CustomFenrirEffect()
{
	static DWORD CustomFenrirEffectAddress1 = 0x0067D2B0;//0067D2B0
	static DWORD CustomFenrirEffectAddress2 = 0x0067DE9F;//0067DE9F
	static DWORD i;
	static DWORD TextureIndex;

	_asm
	{
		Pushad;
		Mov TextureIndex, Eax;
	}

	for (i = 0; i < MAX_FENRIR_INFO; i++)
	{

		if (gCustomFenrir.m_CustomFenrirInfo[i].ExeOption == -1)
		{
			continue;
		}

		if (gCustomFenrir.m_CustomFenrirInfo[i].Texture != TextureIndex)
		{
			continue;
		}

		if (gCustomFenrir.m_CustomFenrirInfo[i].Effect == 0)
		{
			break;
		}

		_asm
		{
			Popad;
			jmp CustomFenrirEffectAddress1;
		}
	}

	_asm
	{
		Popad;
		jmp CustomFenrirEffectAddress2;
	}
}
#define PREVIEW_STRUCT										0x0754F4BC
#define PREVIEW_STRUCT_SIZE									0x484

void CCustomFenrir::ChangeCharacterExt(int Key, BYTE* Equipment, DWORD pCharacter, DWORD pHelper)
{
	pMakePreviewCharSet(Key, Equipment, pCharacter, pHelper);

	DWORD ViewportAddress = (pCharacter) ? pCharacter : pViewportAddress + (Key * 0x484);//484
	
	BYTE WingCharSet = Equipment[16] >> 2;

	if (WingCharSet > 0)
	{
		*(WORD*)(*(int*)PREVIEW_STRUCT + (PREVIEW_STRUCT_SIZE * Key) + 0x338) = ITEMGET2(12, 41) + (WingCharSet - 1);
	}

	if ((Equipment[11] & 4) == 4)
	{
		*(WORD*)(ViewportAddress + 856) = 7328;//856	7328

		BYTE aux = Equipment[15] & 0xE3;

		int Level = (((aux >> 3) | (aux & 0x3)) | ((Equipment[16] & 0x1) << 5));

		if (gCustomFenrir.m_CustomFenrirInfo[Level].ExeOption != -1)
		{
			if (pHelper)
			{
				CreateBugSub(gCustomFenrir.m_CustomFenrirInfo[Level].Texture, ViewportAddress + 16, ViewportAddress, pHelper, 0);
			}
			else
			{
				CreateBug(gCustomFenrir.m_CustomFenrirInfo[Level].Texture, ViewportAddress + 16, ViewportAddress, 0, 0);
			}

			*(BYTE*)(ViewportAddress + 859) = Level;//859
		}
		else
		{
			if (pHelper)
			{
				CreateBugSub(361, ViewportAddress + 16, ViewportAddress, pHelper, 0);//361
			}
			else
			{
				CreateBug(361, ViewportAddress + 16, ViewportAddress, 0, 0);//361
			}

			*(BYTE*)(ViewportAddress + 859) = 0;//859
		}
	}
	/*((void(*)(int, LPBYTE, LPBYTE, int))0x0053FD2F)(ObjectIndex, CharSet, Object, Mode);


	BYTE WingCharSet = CharSet[16] >> 2;

	if (WingCharSet > 0)
	{
		*(WORD*)(*(int*)PREVIEW_STRUCT + (PREVIEW_STRUCT_SIZE * ObjectIndex) + 0x338) = ITEMGET2(12, 41) + (WingCharSet - 1);
	}*/
}

_declspec(naked) void CCustomFenrir::FenrirMakePreviewCharSet()
{
	static DWORD jmpBack = 0x00540302;//00540302
	static DWORD ViewportAddress;
	static DWORD Equipment;
	static DWORD pHelper;
	static BYTE aux;
	static int Level;

	_asm
	{
		Mov Ecx, [Ebp + 0x14];
		Mov pHelper, Ecx;
		Mov Ecx, Dword Ptr Ds : [Ebp - 0x08] ;
		Mov Word Ptr Ds : [Ecx + 856] , 0x1CA0;//856	1CA0
		Mov ViewportAddress, Ecx;
		Mov Edx, Dword Ptr Ss : [Ebp + 0xC] ;
		Mov Equipment, Edx;
	}

	aux = *(BYTE*)(Equipment + 15) & 0xE3;//XZ

	Level = (((aux >> 3) | (aux & 0x3)) + ((*(BYTE*)(Equipment + 15) & 0x1) << 5));//XZ

	if (gCustomFenrir.m_CustomFenrirInfo[Level].ExeOption != -1)
	{
		if (pHelper != NULL)
		{
			CreateBugSub(gCustomFenrir.m_CustomFenrirInfo[Level].Texture, ViewportAddress + 16, ViewportAddress, (int)pHelper, 0);//XZ
		}
		else
		{
			CreateBug(gCustomFenrir.m_CustomFenrirInfo[Level].Texture, ViewportAddress + 16, ViewportAddress, 0, 0);//XZ
		}

		*(BYTE*)(ViewportAddress + 859) = Level;//859
	}
	else
	{
		if (pHelper != NULL)
		{
			CreateBugSub(361, ViewportAddress + 16, ViewportAddress, (int)pHelper, 0);//361
		}
		else
		{
			CreateBug(361, ViewportAddress + 16, ViewportAddress, 0, 0);//361
		}

		*(BYTE*)(ViewportAddress + 859) = 0;//859
	}

	_asm
	{
		Jmp[jmpBack];
	}
}