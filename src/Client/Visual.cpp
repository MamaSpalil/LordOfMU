#include "stdafx.h"
#include "Visual.h"
#include "Protocol.h"
#include "Other.h"
#include "TMemory.h"

DWORD MovHP_OffSet; //
DWORD MovMana_OffSet;
DWORD MovStamina_OffSet;
DWORD MovShield_OffSet;
DWORD ShieldCurrentShow = 1;
DWORD StaminaCurrentShow = 1;
DWORD HPCurrentShow = 1;
DWORD ManaCurrentShow = 1;
DWORD HPMaxShow = 1;
DWORD ManaMaxShow = 1;
DWORD ShieldMaxShow = 1;
DWORD StaminaMaxShow = 1;
void cHookHP_Show()
{
	//SET PUSH TO LIFE
	SetByte( (PVOID) 0x00661535, 0x00);
	SetByte( (PVOID) 0x00661536, 0x24);
	SetByte( (PVOID) 0x00661537, 0x7B);
	SetByte( (PVOID) 0x00661538, 0x00);

	//SET PUSH TO LIFE 2
	SetByte( (PVOID) 0x00603F82, 0x00);
	SetByte( (PVOID) 0x00603F83, 0x24);
	SetByte( (PVOID) 0x00603F84, 0x7B);
	SetByte( (PVOID) 0x00603F85, 0x00);
	//SET PUSH TO Mana
	SetByte( (PVOID) 0x00661809, 0x30);
	SetByte( (PVOID) 0x0066180A, 0x24);
	SetByte( (PVOID) 0x0066180B, 0x7B);
	SetByte( (PVOID) 0x0066180C, 0x00);

	//SET PUSH TO Mana 2
	SetByte( (PVOID) 0x00604049, 0x30);
	SetByte( (PVOID) 0x0060404A, 0x24);
	SetByte( (PVOID) 0x0060404B, 0x7B);
	SetByte( (PVOID) 0x0060404C, 0x00);
	////Stamina Coordinats
	SetByte( (PVOID) 0x0060317F, 0xD9);//0059ED03
	SetByte( (PVOID) 0x00603183, 0x00);//0059ED07
	////SD  Coordinats
	SetByte( (PVOID) 0x0060350F, 0xD9);//
	SetByte( (PVOID) 0x00603514, 0xB6);//
	//XZ
	//SET PUSH TO Stamina
	SetByte( (PVOID) 0x006031CA, 0x90);
	SetByte( (PVOID) 0x006031CB, 0x24);
	SetByte( (PVOID) 0x006031CC, 0x7B);
	SetByte( (PVOID) 0x006031CD, 0x00);

	//SET PUSH TO Shield
	SetByte( (PVOID) 0x0060355A, 0x60);
	SetByte( (PVOID) 0x0060355B, 0x24);
	SetByte( (PVOID) 0x0060355C, 0x7B);
	SetByte( (PVOID) 0x0060355D, 0x00);
	
	
	SetRange((LPVOID)0x0060350B, 6, ASM::NOP);//0059F06F
    SetJmp((LPVOID)0x0060350B, LoadShieldShow);//0059F06F
	SetRange((LPVOID)0x0060317B, 6, ASM::NOP);//0059ECFF
    SetJmp((LPVOID)0x0060317B, LoadStaminaShow);//0059ECFF
	SetRange((LPVOID)0x00604E2C, 4, ASM::NOP);//005A095D
    SetJmp((LPVOID)0x0604E2C, LoadLifeShow);//005A095D
	SetRange((LPVOID)0x00604E51, 4, ASM::NOP);//005A0982
    SetJmp((LPVOID)0x00604E51, LoadManaShow);//005A0982
}

//---------------------------------------------------------------------------

int GetPercent(int MaxState,int State)
{

	int PercentSet;
	int PercentState;
	if(State || MaxState != 0)
	{
	PercentSet = MaxState/100;
	PercentState = State/PercentSet;
	if(PercentState > 100)
    PercentState = 100;
	else if (PercentState <= 0)
	PercentState = 1;
	if((PercentState == 100)&&(MaxState != State))
    PercentState = 99;

	return PercentState;
	}
	else
	{
    PercentState = 0;
	return PercentState;
	}
}
void GCGetRecupationCharacter(PMSG_SEND_CHARACTER_INFO * gObj)
{

	char msg[30];
	char msg2[30];
	char msg3[30];
	char msg4[30];
	//int LifeShowPercent = GetPercent(gObj->MaxLife,gObj->life);
	//int ManaShowPercent = GetPercent(gObj->MaxMana,gObj->Mana);
	//int ShieldShowPercent = GetPercent(gObj->SD,gObj->MaxSD);
	//int StaminaShowPercent = GetPercent(gObj->Stanina,gObj->MaxStamina);
	HPCurrentShow = gObj->life;
	HPMaxShow = gObj->MaxLife;
	ManaCurrentShow = gObj->Mana;
	ManaMaxShow = gObj->MaxMana;
	StaminaCurrentShow = gObj->Stanina;
	ShieldCurrentShow = gObj->SD;
	ShieldMaxShow = gObj->MaxSD;
    StaminaMaxShow = gObj->MaxStamina;




	sprintf(msg, "Life: %u / %u", gObj->life,gObj->MaxLife);
	sprintf(msg2, "Mana: %u / %u", gObj->Mana,gObj->MaxMana);
	sprintf(msg3, "Shield: %u / %u", gObj->SD,gObj->MaxSD);
	sprintf(msg4, "Stamina: %u / %u", gObj->Stanina,gObj->MaxStamina);
	memcpy((char*)0x007B2400,msg,29);//63f81
	memcpy((char*)0x007B2430,msg2,29);
	memcpy((char*)0x007B2460,msg3,29);
	memcpy((char*)0x007B2490,msg4,29);
	//SetByte( (PVOID) 0x00604E5D,LifeShowPercent);
	//SetByte( (PVOID) 0x00604E82,ManaShowPercent);

}

int CurrentAttributShow(int Show_T)
{
PMSG_SEND_CHARACTER_INFO * gObj;
if(Show_T == 1)
return gObj->life;
else if (Show_T == 2)
return gObj->Mana;
else
return 0;
}

void __declspec(naked) LoadLifeShow()
{
    _asm
    {
	   push HPCurrentShow
       mov MovHP_OffSet, 0x000604E31 //005A0962
       jmp MovHP_OffSet //

    }
}
void __declspec(naked) LoadManaShow()
{
    _asm
    {
	   push ManaCurrentShow
       mov MovMana_OffSet, 0x00604E56 //005A0987
       jmp MovMana_OffSet //

    }
}

void __declspec(naked) LoadStaminaShow()
{
    _asm
    {
	   push StaminaCurrentShow
	   push 0x43D98000
       mov MovStamina_OffSet, 0x00603181 //0059ED05
       jmp MovStamina_OffSet //
    }
}
void __declspec(naked) LoadShieldShow()
{
    _asm
    {
		
	   push ShieldCurrentShow
	   push 0x43D98000
       mov MovShield_OffSet, 0x00603511 //0059F075
       jmp MovShield_OffSet //
    }
}
//---------------------------------------------------------------------------


void cLoadLifeManaFix()
{

	cHookHP_Show();
	SetOp((LPVOID)0x00603EE5, (LPVOID)DrawCircle, ASM::CALL);	// - hp  // ok
	SetOp((LPVOID)0x00603F2D, (LPVOID)DrawCircle, ASM::CALL);	// - hp  // ok//0059FA5E
	SetOp((LPVOID)0x00603FF6, (LPVOID)DrawCircle, ASM::CALL);	// - mana  // ok
	SetOp((LPVOID)0x006034FC, (LPVOID)DrawCircle, ASM::CALL);   // - SD // OK
	SetOp((LPVOID)0x0060316C, (LPVOID)DrawCircle, ASM::CALL);   // - Stamina // OK//0059ECF0
	
	
}

void DrawCircle(DWORD ModelID, float Arg2, float Arg3, float Arg4, float Arg5, float Arg6, float Arg7, float Arg8, float Arg9, int Arg10, int Arg11, int Arg12)
{


    float Height    = 48.0f;
    float PosY		= 432.0f;
	float LifeFill  = (float)(HPMaxShow - HPCurrentShow) / (float)HPMaxShow;
	float ManaFill  = (float)(ManaMaxShow - ManaCurrentShow) / (float)ManaMaxShow;
	float AgFill	= (float)(StaminaMaxShow - StaminaCurrentShow) / (float)StaminaMaxShow;
	float SdFill	= (float)(ShieldMaxShow - ShieldCurrentShow) / (float)ShieldMaxShow;
    // ----
    if( ModelID == 0x0EA || ModelID == 0x0F7 )
    {
        float StartY        = LifeFill * Height + PosY;
        float StartHeight   = Height - LifeFill * Height;
        float SourceY       = LifeFill * Height / 64.0;
        float SouceHeight   = (1.0 - LifeFill) * Height / 64.0;
        return pDrawCircle(ModelID, Arg2, StartY, Arg4, StartHeight, 0, SourceY, Arg8, SouceHeight, 1, 1, 0);
    }
    // ----
    if( ModelID == 0x0EB )
    {
        float StartY        = ManaFill * Height + PosY;
        float StartHeight   = Height - ManaFill * Height;
        float SourceY       = ManaFill * Height / 64.0;
        float SouceHeight   = (1.0 - ManaFill) * Height / 64.0;
        return pDrawCircle(ModelID, Arg2, StartY, Arg4, StartHeight, 0, SourceY, Arg8, SouceHeight, 1, 1, 0);
    }
    //// ----
	//if( ModelID == 0x101 )
	//{
    //    float StartY        = AgFill * Height + PosY;
    //    float StartHeight   = Height - AgFill * Height;
    //    float SourceY       = AgFill * Height / 64.0;
    //    float SouceHeight   = (1.0 - AgFill) * Height / 64.0;
    //    return pDrawCircle(ModelID, Arg2, StartY, Arg4, StartHeight, 0, SourceY, Arg8, SouceHeight, 1, 1, 0);
	//}
	////// ----
	//if( ModelID == 0x864 )
	//{
	//
	//	 float StartY        = AgFill * Height + PosY;
    //    float StartHeight   = Height - AgFill * Height;
    //    float SourceY       = AgFill * Height / 64.0;
    //    float SouceHeight   = (1.0 - AgFill) * Height / 64.0;
    //    return pDrawCircle(ModelID, Arg2, StartY, Arg4, StartHeight, 0, SourceY, Arg8, SouceHeight, 1, 1, 0);
	//}
	// ----
    pDrawCircle(ModelID, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12);
}
VisualState* gExInterface = new VisualState;
//
void VisualState::Load()
{
	this->Kill = TRUE;
	this->Attack = 0;
	this->aIndex = 0;
	this->Life = 0;
	this->MaxLife = 0;
	this->Mana = 0;
	this->MaxMana = 0;
	this->Ag = 0;
	this->MaxAg = 0;
	this->Sd = 0;
	this->MaxSd = 0;
	this->ResetCount = 0;
	this->AttackHP = 0;
	this->AttackSD = 0;

	//HookCall(0x604E3B, DrawHP);//005A096C
	//HookCall(0x604E62, DrawMP);//005A0993
	//HookCall(0x603186, DrawAG);//0059ED0A
	//HookCall(0x603516, DrawSD);
	//HookCall(0x603F98, DrawTabHP);
	//HookCall(0x604062, DrawTabMP);
	//HookCall(0x6031E3, DrawTabAG);//0059EDAB
	//HookCall(0x603570, DrawTabSD);
	//HookCall(0x661583, InfoHP);
	//HookCall(0x661854, InfoMP);
	//HookJump(0x5A6AD6, CheckMP1);
	//HookJump(0x5A7392, CheckMP2);
	//HookJump(0x5A85E0, CheckMP3);
	//HookJump(0x5AFF65, CheckMP4);
	//HookJump(0x5D054C, CheckMP5);
	//HookJump(0x5D3A2F, CheckMP6);
	//HookJump(0x5D5D67, CheckMP7);
	//HookJump(0x5A72A2, CheckBP1);
	//HookJump(0x5B085D, CheckBP2);
	//HookJump(0x5D0D4D, CheckBP3);
	//HookJump(0x5D6534, CheckBP4);
	//HookJump(0x5E09B7, CheckBP5);
	//HookJump(0x5EB2C2, CheckBP6);
	//HookCall(0x65F609, LevelBoxCreate);
	//HookCall(0x65F779, LevelBoxDraw);
	//HookCall(oDrawInterface_Call, Work);
}
//
//void VisualState::Work()
//{
//	((void(__cdecl*)()) 0x006698A0)();
//
//	//VisualState::Window();
//
//	//VisualState::DrawTimer();
//
//	return;
//}
//
//void  VisualState::Window()
//{
//	//char test[500];								
//	//sprintf_s(test, sizeof(test), "Server: [%s] || Resets: [%d] || Pontos: [%d] ||    %s", gProtect.m_MainInfo.WindowName, Visual.ResetCount, Protocol.RealLevelUpPoint, gProtect.m_MainInfo.Site);
//	//SetWindowText(g_hWnd, test);
//}
//
//
////Relogin
//void VisualState::DrawTimer()
//{
//	char LocalTime[50];
//	char ServerTime[50];
//
//		//if ((GetTickCount() - gExInterface->server_tick) > 1000)
//		//{
//		//	gExInterface->server_timer += ((GetTickCount() - gExInterface->server_tick) / 1000);
//		//	gExInterface->server_tick = GetTickCount();
//		//}
//
//		time_t TimeLocal;
//		struct tm* LocalT;
//		time(&TimeLocal);
//		LocalT = localtime(&TimeLocal);
//		sprintf_s(LocalTime, "Horário Local: %02d:%02d:%02d", LocalT->tm_hour, LocalT->tm_min, LocalT->tm_sec);
//
//	SetBlend(1);
//
//	//SetDword2((LPVOID)0x007D4C84, 0xFF19BDFF); // Text
//	//SetDword2((LPVOID)0x007D4C8C, 0xFF010101); // Background
//
//	//if (gServerInfo->MuConfig.TimerBar == 2)
//	//{
//	//	if (this->server_timer != 0)
//	//		pDrawTextOut(3, 400, ServerTime, 0, 0, 0);
//	//}
//
//	pDrawTextOut(3, 416, LocalTime, 0, 0, 0);
//	GLSwitch();
//}
//
////FIM RELOGIN
//void VisualState::LevelBoxCreate(int a1, float a2, float a3, float a4, float a5, int a6, int a7, float a8, float a9, char a10, char a11, float a12)
//{
//	if (a3 == 56.0f)
//	{
//		pDrawImage2(a1, a2, 36.0f, a4, a5, a6, a7, a8, a9, a10, a11, a12);
//	}
//
//	pDrawImage2(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
//}
//
//void VisualState::LevelBoxDraw(int a1, int a2, LPSTR a3, int a4, int a5, int a6)
//{
//	pDrawTextOut(a1, a2 - 20, a3, a4, a5, a6);
//	char szBuff[50];
//	sprintf_s(szBuff, "Resets: %d", Visual.ResetCount);
//	//sprintf_s(Visual.Buffer[2], "Resets: %d", Visual.Resets);
//	pDrawTextOut(a1, a2, szBuff, a4, a5, a6);
//
//}
//
// ================================================================== gx customs ============================================================================= //
//void VisualState::Index(PMSG_JOINRESULT* Data)
//{
//	this->aIndex = MAKE_NUMBERW(Data->NumberH, Data->NumberL);
//	this->Kill = FALSE;
//}
//void VisualState::GCMonsterDamageRecv(PMSG_MONSTER_DAMAGE_RECV* Data)
//{
//	this->Life = Data->ViewCurHP;
//
//	this->Attack = Data->ViewDamageHP;
//}
//void VisualState::RecvDamage(PMSG_ATTACKRESULT * Data)
//{
//
//	//this->AttackHP = Data->AttackDamage;
//
//	//this->AttackSD = Data->iShieldDamage;
//
//	//short tIndex =  MAKE_NUMBERW(Data->NumberH,Data->NumberL);
//
//	//if (this->aIndex == tIndex)
//	//{
//	//	this->Life -= this->AttackHP;
//	//	this->Sd -= this->AttackSD;
//	//	if(0 > this->Life)
//	//	{
//	//		this->Life = 0;
//	//	}
//	//}
//
//	//this->AttackHP = Data->AttackDamage;
//	//this->AttackSD = Data->iShieldDamage;
//	//short tIndex =  MAKE_NUMBERW(Data->NumberH,Data->NumberL);
//	//if (this->aIndex == tIndex)
//	//{
//	//	this->Life -= this->AttackHP;
//	//	this->SD -= this->AttackSD;
//	//	if(0 > this->Life)
//	//	{
//	//		this->Life = 0;
//	//	}
//	//}
//}
//
//void VisualState::RecvHP(PMSG_REFILL* Data)
//{
//	if (Data->IPos == 0xFF)
//	{
//		this->Life = Data->Life;
//		this->Sd = Data->Shield;
//	}
//
//	else if (Data->IPos == 0xFE)
//	{
//		this->MaxLife = Data->Life;
//		this->MaxSd = Data->Shield;
//	}
//
//	if (this->Life > this->MaxLife)
//	{
//		this->Life = this->MaxLife;
//	}
//
//	if (this->Kill == TRUE)
//	{
//		this->Life = 0;
//		this->Sd = 0;
//	}
//}
//
//void VisualState::RecvMP(PMSG_MANASEND* Data)
//{
//	if (Data->IPos == 0xFF)
//	{
//		this->Mana = Data->Mana;
//		this->Ag = Data->BP;
//	}
//
//	else if (Data->IPos == 0xFE)
//	{
//		this->MaxMana = Data->Mana;
//		this->MaxAg = Data->BP;
//	}
//
//	if (this->Mana > this->MaxMana)
//	{
//		this->Mana = this->MaxMana;
//	}
//}
//
//void VisualState::Respawn()
//{
//	this->Kill = FALSE;
//
//	this->Life =this->Life; //this->MaxLife;
//	this->Mana =this->Mana; //this->MaxMana;
//	this->Ag =this->Ag;// this->MaxAg;
//	this->Sd =this->Sd; //this->MaxSd;
//}
//
//void VisualState::Killed(PMSG_DIEPLAYER* Data)
//{
//	short Index = MAKE_NUMBERW(Data->NumberH, Data->NumberL);
//
//	if (this->aIndex == Index)
//	{
//		this->Life = 0;
//		this->Sd = 0;
//		this->Kill = TRUE;
//	}
//
//}
//
//void VisualState::DrawHP(float PosX, float PosY, int Value, float ScaleX, float ScaleY)
//{
//	DrawBar(PosX - 15.0f, PosY, Visual.Life, ScaleX, ScaleY);
//}
//
//void VisualState::DrawMP(float PosX, float PosY, int Value, float ScaleX, float ScaleY)
//{
//	DrawBar(PosX - 15.0f, PosY, Visual.Mana, ScaleX, ScaleY);
//}
//
//void VisualState::DrawAG(float PosX, float PosY, int Value, float ScaleX, float ScaleY)
//{
//	DrawBar(PosX - 10.0f, PosY, Visual.Ag, ScaleX, ScaleY);
//}
//
//void VisualState::DrawSD(float PosX, float PosY, int Value, float ScaleX, float ScaleY)
//{
//	DrawBar(PosX - 15.0f, PosY, Visual.Sd, ScaleX, ScaleY);
//}
//
//void VisualState::DrawTabHP(int PosX, int PosY, LPCSTR lpString)
//{
//	sprintf_s(Visual.Buffer[0], "Life: %d/%d", Visual.Life, Visual.MaxLife);
//
//	DrawTab(PosX, PosY, Visual.Buffer[0]);
//}
//
//void VisualState::DrawTabMP(int PosX, int PosY, LPCSTR lpString)
//{
//	sprintf_s(Visual.Buffer[1], "Mana: %d/%d", Visual.Mana, Visual.MaxMana);
//
//	DrawTab(PosX, PosY, Visual.Buffer[1]);
//}
//
//void VisualState::DrawTabAG(int PosX, int PosY, LPCSTR lpString)
//{
//	sprintf_s(Visual.Buffer[2], "AG: %d/%d", Visual.Ag, Visual.MaxAg);
//
//	DrawTab(PosX, PosY, Visual.Buffer[2]);
//}
//
//void VisualState::DrawTabSD(int PosX, int PosY, LPCSTR lpString)
//{
//	sprintf_s(Visual.Buffer[3], "SD: %d/%d", Visual.Sd, Visual.MaxSd);
//
//	DrawTab(PosX, PosY, Visual.Buffer[3]);
//}
//
//void VisualState::InfoHP(int a1, int a2, LPCSTR a3, int a4, char a5, int a6)
//{
//	sprintf_s(Visual.Buffer[4], "Life: %d/%d", Visual.Life, Visual.MaxLife);
//
//	DrawInfo(a1, a2, Visual.Buffer[4], a4, a5, a6);
//}
//
//void VisualState::InfoMP(int a1, int a2, LPCSTR a3, int a4, char a5, int a6)
//{
//	sprintf_s(Visual.Buffer[5], "Mana: %d/%d", Visual.Mana, Visual.MaxMana);
//
//	DrawInfo(a1, a2, Visual.Buffer[5], a4, a5, a6);
//}
//
//__declspec(naked) void VisualState::CheckMP1()
//{
//	static DWORD Local = 0x5A6ADC;
//
//	__asm
//	{
//		MOV ECX, Visual.Mana;
//		CMP ECX, EDX;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckMP2()
//{
//	static DWORD Local = 0x5A739A;
//
//	__asm
//	{
//		MOV EAX, Visual.Mana;
//		MOV ECX, DWORD PTR SS : [ESP + 0x60];
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckMP3()
//{
//	static DWORD Local = 0x5A85E6;
//
//	__asm
//	{
//		MOV EDX, Visual.Mana;
//		CMP EDX, ECX;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckMP4()
//{
//	static DWORD Local = 0x5AFF70;
//
//	__asm
//	{
//		MOV EDX, Visual.Mana;
//		CMP EDX, DWORD PTR SS : [ESP + 0x2D0];
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckMP5()
//{
//	static DWORD Local = 0x5D0552;
//
//	__asm
//	{
//		MOV ECX, Visual.Mana;
//		CMP EDX, ECX;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckMP6()
//{
//	static DWORD Local = 0x5D3A35;
//
//	__asm
//	{
//		MOV ECX, Visual.Mana;
//		CMP EDX, ECX;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckMP7()
//{
//	static DWORD Local = 0x5D5D6D;
//
//	__asm
//	{
//		XOR EDX, EDX;
//		MOV EDX, Visual.Mana;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckBP1()
//{
//	static DWORD Local = 0x5A72A8;
//
//	__asm
//	{
//		XOR ECX, ECX;
//		MOV ECX, Visual.Ag;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckBP2()
//{
//	static DWORD Local = 0x5B0863;
//
//	__asm
//	{
//		XOR EDX, EDX;
//		MOV EDX, Visual.Ag;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckBP3()
//{
//	static DWORD Local = 0x5D0D53;
//
//	__asm
//	{
//		XOR ECX, ECX;
//		MOV ECX, Visual.Ag;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckBP4()
//{
//	static DWORD Local = 0x5D653A;
//
//	__asm
//	{
//		XOR ECX, ECX;
//		MOV ECX, Visual.Ag;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckBP5()
//{
//	static DWORD Local = 0x5E09BD;
//
//	__asm
//	{
//		XOR EDX, EDX;
//		MOV EDX, Visual.Ag;
//		JMP Local;
//	}
//}
//
//__declspec(naked) void VisualState::CheckBP6()
//{
//	static DWORD Local = 0x5EB2C8;
//
//	__asm
//	{
//		XOR ECX, ECX;
//		MOV ECX, Visual.Ag;
//		JMP Local;
//	}
//}

VisualState Visual;