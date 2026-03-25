#include "stdafx.h"
#include "m_Disable.h"
#include "Interface.h"

DWORD	SetInvisibleEffect_PointerYY;
DWORD	SetInvisibleEffect_BuffYY;
DWORD	SetInvisibleEffect_PointerY;
DWORD	SetInvisibleEffect_BuffY;

C_Disable m_Disable;

C_Disable::C_Disable(void){
}

C_Disable::~C_Disable(void){
}

/*InvisibleArmors*/
__declspec(naked) void ItemInvisible1() {
	static DWORD m_aaaaab = 0x006950D8;
	_asm {
		mov SetInvisibleEffect_BuffYY, esi
		mov ecx, dword ptr ss : [ebp + 8]
		mov esi, dword ptr ds : [ecx + 0x30]
		mov SetInvisibleEffect_PointerYY, esi
		mov esi, SetInvisibleEffect_BuffYY
	}
	if (SetInvisibleEffect_PointerYY != 0x1C8D
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(12, 15)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(12, 30)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(12, 31)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 13)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 14)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 16)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 31)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 42)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 13)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 41)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 43)
		|| SetInvisibleEffect_PointerYY != GET_ITEM_MODEL(14, 44)) {
		_asm {
			jmp[m_aaaaab]
		}
	}
}

__declspec(naked) void ItemInvisible2() {
	static DWORD m_aaaaaa = 0x00695157;
	_asm {
		mov SetInvisibleEffect_BuffY, esi
		mov ecx, dword ptr ss : [ebp + 8]
		mov esi, dword ptr ds : [ecx + 0x30]
		jmp[m_aaaaaa]
	}
}

__declspec(naked) void DelectlpViewPlayerOn() {
	static DWORD Buff_lpViewPlayer = 0x0053D054;
	_asm {
		CMP DWORD PTR SS : [EBP - 0x4], 0
		JMP[Buff_lpViewPlayer]
	}
}

__declspec(naked) void DelectlpViewPlayerOff() {
	static DWORD Buff_lpViewPlayer = 0x0053D054;
	_asm {
		CMP DWORD PTR SS : [EBP - 0x4], 400
		JMP[Buff_lpViewPlayer]
	}
}

__declspec(naked) void CreateSprite_Off() {
	static DWORD Buff_EffectStatic = 0x00593947;
	_asm {
		CMP DWORD PTR SS : [EBP - 0x4], 0
		JMP[Buff_EffectStatic]
	}
}

__declspec(naked) void CreateSprite_On() {
	static DWORD Buff_EffectStatic = 0x00593947;
	_asm {
		CMP DWORD PTR SS : [EBP - 0x4], 0x3E8
		JMP[Buff_EffectStatic]
	}
}

void C_Disable::KG_ActivateStaticEffect_1() {
	SetCompleteHook(0xE9, 0x00593940, &CreateSprite_Off);//00593940;
}

void C_Disable::KG_ActivateStaticEffect_2() {
	SetCompleteHook(0xE9, 0x00593940, &CreateSprite_On);//00593940;
}

__declspec(naked) void CreateParticle_Off() {
	static DWORD Buff_EffectDynamic = 0x0057F6B3;
	_asm {
		CMP DWORD PTR SS : [EBP - 0x4], 0
		JMP[Buff_EffectDynamic]
	}
}

__declspec(naked) void CreateParticle_On() {
	static DWORD Buff_EffectDynamic = 0x0057F6B3;
	_asm {
		CMP DWORD PTR SS : [EBP - 0x4], 0x0BB8
		JMP[Buff_EffectDynamic]
	}
}

void C_Disable::KG_ActivateFog_1() {
	gInterfaces->StateFog = 0;
}

void C_Disable::KG_ActivateFog_2() {
	gInterfaces->StateFog = 1;
}

void C_Disable::KG_ActivateDynamicEffect_1() {
	SetCompleteHook(0xE9, 0x0057F6AC, &CreateParticle_Off);//0057F6AC
}

void C_Disable::KG_ActivateDynamicEffect_2() {
	SetCompleteHook(0xE9, 0x0057F6AC, &CreateParticle_On);//0057F6AC
}

void C_Disable::KG_ActiveShadow_1() {
	SetByte(0x0051A061, 0xC3);//0051A061
}

void C_Disable::KG_ActiveShadow_2() {
	SetByte(0x0051A061, 0x55);//0051A061
}

void C_Disable::KG_ActivateInvisiblePlayer_1() {
	SetCompleteHook(0xE9, 0x0053D04D, &DelectlpViewPlayerOn);//0053D04D
}
void C_Disable::KG_ActivateInvisiblePlayer_2() {
	SetCompleteHook(0xE9, 0x0053D04D, &DelectlpViewPlayerOff);//0053D04D
}

void C_Disable::KG_ActivateInvisibleWings_1(){
	MemorySet(0x00539513, 0x90, 0x5);//00539513
}

void C_Disable::KG_ActivateInvisibleWings_2() {
	SetOp((LPVOID)0x00539513, (LPVOID)0x005305AE, ASM::CALL);//00539513  //005305AE
}

void C_Disable::KG_ActivateInvisibleArmors_1() {
	SetCompleteHook(0xE9, 0x006950C9, &ItemInvisible1);//006950C9
}

void C_Disable::KG_ActivateInvisibleArmors_2() {
	SetCompleteHook(0xE9, 0x006950C9, &ItemInvisible2);//006950C9
}

void C_Disable::KG_ActivateInvisibleWeapons_1() {
	MemorySet(0x00536496, 0x90, 0x5); //Unk //00536496
	MemorySet(0x0053654A, 0x90, 0x5); //Unk //0053654A
	MemorySet(0x0053669C, 0x90, 0x5); //Unk //0053669C
	MemorySet(0x00539201, 0x90, 0x5); //Unk //00539201
	MemorySet(0x00539241, 0x90, 0x5); //Unk //00539241
	MemorySet(0x0053937F, 0x90, 0x5); //Unk //0053937F
	MemorySet(0x005394C6, 0x90, 0x5); //Unk //005394C6
	MemorySet(0x005395A2, 0x90, 0x5); //Unk //005395A2
	MemorySet(0x00539C35, 0x90, 0x5); //weapon //00539C35
}
//470
void C_Disable::KG_ActivateInvisibleWeapons_2() {
	SetOp((LPVOID)0x00536496, (LPVOID)0x005305AE, ASM::CALL); //Unk
	SetOp((LPVOID)0x0053654A, (LPVOID)0x005305AE, ASM::CALL); //Unk
	SetOp((LPVOID)0x0053669C, (LPVOID)0x005305AE, ASM::CALL); //Unk
	SetOp((LPVOID)0x00539201, (LPVOID)0x005305AE, ASM::CALL); //Unk
	SetOp((LPVOID)0x00539241, (LPVOID)0x005305AE, ASM::CALL); //Unk
	SetOp((LPVOID)0x0053937F, (LPVOID)0x005305AE, ASM::CALL); //Unk
	SetOp((LPVOID)0x005394C6, (LPVOID)0x005305AE, ASM::CALL); //Unk
	SetOp((LPVOID)0x005395A2, (LPVOID)0x005305AE, ASM::CALL); //Unk
	SetOp((LPVOID)0x00539C35, (LPVOID)0x005305AE, ASM::CALL); //weapon
}

void C_Disable::KG_ActivateInvisibleObjects_1() {
	SetByte(0x0067BFCF, 1);//0067BFCF
}

void C_Disable::KG_ActivateInvisibleObjects_2() {
	SetByte(0x0067BFCF, 0);////0067BFCF
}