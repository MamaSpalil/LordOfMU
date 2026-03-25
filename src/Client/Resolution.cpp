#include "stdafx.h"
#include "Resolution.h"
#include "TMemory.h"
//#include "Util.h"

float Resolution_ClipX1 = 2000.0f;
float Resolution_ClipX2 = 2000.0f;

void InitResolution() // OK
{
	SetCompleteHook(0xE9,0x004DC208,&ResolutionSwitch);//DONE

	SetCompleteHook(0xE9,0x004DD546,&ResolutionSwitchFont);

	//SetCompleteHook(0xE9,0x00468368,&ResolutionMoveList);

	//SetCompleteHook(0xE9,0x00467D23,&ResolutionMoveList2);

	SetCompleteHook(0xE8,0x006F4FFF,&ResolutionKanturu);
}

void ResolutionKanturu() // OK
{
	if(*(DWORD*)(MAIN_RESOLUTION) >= 4)
	{
		*(WORD*)(*(DWORD*)(0x07ED9E68)+0x0C) = 0xD7;
	}

	((bool(__thiscall*)(void*))0x00434DBD)((void*)*(DWORD*)(0x07ED9E68));
}
//bool ResolutionKanturu()
//{
//    if (m_Resolution >= 4)
//    {
//        *(WORD*)(*(DWORD*)(0x007ED9E68) + 0x0C) = 0xD7;//
//	}
//
//    return ((bool(__stdcall*)(int Code)) 0x006F5BD6)(30);//
//}
__declspec(naked) void ResolutionSwitch() // OK
{
	static DWORD ResolutionSwitchAddress1 = 0x004DC296;

	_asm
	{
		Mov Edx,Dword Ptr Ds:[MAIN_RESOLUTION]
		Mov Dword Ptr Ss:[Ebp-0x338],Edx
		Cmp Dword Ptr Ss:[Ebp-0x338],0x00
		Jnz NEXT1
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],640
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],480
		Jmp EXIT
		NEXT1:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x01
		Jnz NEXT2
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],800
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],600
		Jmp EXIT
		NEXT2:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x02
		Jnz NEXT3
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1024
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],768
		Jmp EXIT
		NEXT3:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x03
		Jnz NEXT4
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1280
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],1024
		Jmp EXIT
		NEXT4:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x04
		Jnz NEXT5
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1360
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],768
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],41
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],-121
		Jmp EXIT
		NEXT5:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x05
		Jnz NEXT6
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1440
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],900
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],81
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],-55
		Jmp EXIT
		NEXT6:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x06
		Jnz NEXT7
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1600
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],900
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],161
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],-55
		Jmp EXIT
		NEXT7:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x07
		Jnz NEXT8
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1680
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],1050
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],200
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],20
		Jmp EXIT
		NEXT8:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x08
		Jnz EXIT
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1920
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],1080
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],320
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],36
		EXIT:
		Jmp [ResolutionSwitchAddress1]
	}
}

__declspec(naked) void ResolutionSwitchFont() // OK
{
	static DWORD ResolutionSwitchFontAddress1 = 0x004DD5C0;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[MAIN_RESOLUTION_X]
		Mov Dword Ptr Ss:[Ebp-0xEE8],Eax
		Cmp Dword Ptr Ss:[Ebp-0xEE8],640
		Jnz NEXT1
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0C
		Jmp EXIT
		NEXT1:
		Cmp Dword Ptr Ss:[Ebp-0xEE8],800
		Jnz NEXT2
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0D
		Jmp EXIT
		NEXT2:
		Cmp Dword Ptr Ss:[Ebp-0xEE8],1024
		Jnz NEXT3
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0E
		Jmp EXIT
		NEXT3:
		Cmp Dword Ptr Ss:[Ebp-0xEE8],1280
		Jnz NEXT4
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0F
		Jmp EXIT
		NEXT4:
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0F
		Lea Ecx,Resolution_ClipX1
		Mov Edx,Dword Ptr Ds:[Ecx+0x00]
		Mov Eax,Dword Ptr Ds:[Ecx+0x04]
		Push Eax
		Push Edx
		Push 0x007B5834
		Call [SetDouble]
		Add Esp,0x08
		Lea Ecx,Resolution_ClipX2
		Mov Edx,Dword Ptr Ds:[Ecx+0x00]
		Mov Eax,Dword Ptr Ds:[Ecx+0x04]
		Push Eax
		Push Edx
		Push 0x007B5844
		Call [SetDouble]
		Add Esp,0x08
		EXIT:
		Jmp [ResolutionSwitchFontAddress1]
	}
}

__declspec(naked) void ResolutionMoveList() // OK
{
	static DWORD ResolutionMoveListAddress1 = 0x00468410;
	static DWORD ResolutionMoveListAddress2 = 0x00468373;

	_asm
	{
		Mov Ecx,Dword Ptr Ds:[MAIN_RESOLUTION_X]
		Mov Dword Ptr Ss:[Ebp-0x1B4],Ecx
		Cmp Dword Ptr Ss:[Ebp-0x1B4],0x500
		Jbe EXIT
		Jmp [ResolutionMoveListAddress1]
		EXIT:
		Jmp [ResolutionMoveListAddress2]
	}
}

__declspec(naked) void ResolutionMoveList2() // OK
{
	static DWORD ResolutionMoveListAddress1 = 0x00467DCB;
	static DWORD ResolutionMoveListAddress2 = 0x00467D2E;

	_asm
	{
		Mov Ecx,Dword Ptr Ds:[MAIN_RESOLUTION_X]
		Mov Dword Ptr Ss:[Ebp-0x1B4],Ecx
		Cmp Dword Ptr Ss:[Ebp-0x1B4],0x500
		Jbe EXIT
		Jmp [ResolutionMoveListAddress1]
		EXIT:
		Jmp [ResolutionMoveListAddress2]
	}
}

//void InitResolution()
//{
//    SetByte(0x004DC12F, 0x1D);
//
//    SetCompleteHook(0xE9, 0x004DC208, &ResolutionSwitch);//4DC208
//
//    SetCompleteHook(0xE9, 0x004DD546, &ResolutionSwitchFont);//004DD546
//
//    //SetCompleteHook(0xE9, 0x00468592, &ResolutionMoveList);//00468592
//
//    //SetCompleteHook(0xE9, 0x00467E83, &ResolutionMoveList2);//00467E83
//
//    SetCompleteHook(0xE8, 0x006F4FEB, &ResolutionKanturu);//006F4FEB
//}
//
//__declspec(naked) void ResolutionSwitch()
//{
//    static DWORD ResolutionSwitchAddress1 = 0x004DC296;
//
//
//    _asm
//    {
//        Mov Edx, Dword Ptr Ds : [0x05877D58]; //MAIN_RESOLUTION 5877D58
//        Mov Dword Ptr Ss : [Ebp - 0x338], Edx;
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x00;
//        Jnz NEXT1;
//        Mov Dword Ptr Ds : [0x007E7F68], 640; //MAIN_RESOLUTION_X 7E7F68
//        Mov Dword Ptr Ds : [0x007E7F6C], 480; //MAIN_RESOLUTION_Y 7E7F6C
//        Jmp EXIT;
//    NEXT1:
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x01;
//        Jnz NEXT2;
//        Mov Dword Ptr Ds : [0x007E7F68], 800; //MAIN_RESOLUTION_X
//        Mov Dword Ptr Ds : [0x007E7F6C], 600; //MAIN_RESOLUTION_Y
//        Jmp EXIT;
//    NEXT2:
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x02;
//        Jnz NEXT3;
//        Mov Dword Ptr Ds : [0x007E7F68], 1024; //MAIN_RESOLUTION_X
//        Mov Dword Ptr Ds : [0x007E7F6C], 768; //MAIN_RESOLUTION_Y
//        Jmp EXIT;
//    NEXT3:
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x03;
//        Jnz NEXT4;
//        Mov Dword Ptr Ds : [0x007E7F68], 1280; //MAIN_RESOLUTION_X
//        Mov Dword Ptr Ds : [0x007E7F6C], 1024; //MAIN_RESOLUTION_Y
//        Jmp EXIT;
//    NEXT4:
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x04;
//        Jnz NEXT5;
//        Mov Dword Ptr Ds : [0x007E7F68], 1360; //MAIN_RESOLUTION_X
//        Mov Dword Ptr Ds : [0x007E7F6C], 768; //MAIN_RESOLUTION_Y
//        Jmp EXIT;
//    NEXT5:
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x05;
//        Jnz NEXT6;
//        Mov Dword Ptr Ds : [0x007E7F68], 1440; //MAIN_RESOLUTION_X
//        Mov Dword Ptr Ds : [0x007E7F6C], 900; //MAIN_RESOLUTION_Y
//        Jmp EXIT;
//    NEXT6:
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x06;
//        Jnz NEXT7;
//        Mov Dword Ptr Ds : [0x007E7F68], 1600; //MAIN_RESOLUTION_X
//        Mov Dword Ptr Ds : [0x007E7F6C], 900; //MAIN_RESOLUTION_Y
//        Jmp EXIT;
//    NEXT7:
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x07;
//        Jnz NEXT8;
//        Mov Dword Ptr Ds : [0x007E7F68], 1680; //MAIN_RESOLUTION_X
//        Mov Dword Ptr Ds : [0x007E7F6C], 1050; //MAIN_RESOLUTION_Y
//        Jmp EXIT;
//    NEXT8:
//        Cmp Dword Ptr Ss : [Ebp - 0x338], 0x08;
//        Jnz EXIT;
//        Mov Dword Ptr Ds : [0x007E7F68], 1920; //MAIN_RESOLUTION_X
//        Mov Dword Ptr Ds : [0x007E7F6C], 1080; //MAIN_RESOLUTION_Y
//    EXIT:
//        Jmp[ResolutionSwitchAddress1];
//    }
//}
//
//__declspec(naked) void ResolutionSwitchFont()
//{
//    static DWORD ResolutionSwitchFontAddress1 = 0x004DD5C0;
//
//    static DWORD Resolution_ClipX1 = 0x007B5834;
//    static DWORD Resolution_ClipX2 = 0X007B5840;
//    static float Float2000 = 2000.0f;
//
//
//    _asm
//    {
//        Mov Edx, Dword Ptr Ds : [0x007E7F68]; //MAIN_RESOLUTION_X 7E7F68
//        Mov Dword Ptr Ss : [Ebp - 0xEEC], Edx;//EEC
//        Cmp Dword Ptr Ss : [Ebp - 0xEEC], 640;
//        Jnz NEXT1;
//        Mov Dword Ptr Ds : [0x078C5260], 0x0C; //MAIN_FONT_SIZE //78C5260
//        Jmp EXIT;
//    NEXT1:
//        Cmp Dword Ptr Ss : [Ebp - 0xEEC], 800;
//        Jnz NEXT2;
//        Mov Dword Ptr Ds : [0x078C5260], 0x0D; //MAIN_FONT_SIZE
//        Jmp EXIT;
//    NEXT2:
//        Cmp Dword Ptr Ss : [Ebp - 0xEEC], 1024;
//        Jnz NEXT3;
//        Mov Dword Ptr Ds : [0x078C5260], 0x0E; //MAIN_FONT_SIZE
//        Jmp EXIT;
//    NEXT3:
//        Cmp Dword Ptr Ss : [Ebp - 0xEEC], 1280;
//        Jnz NEXT4;
//        Mov Dword Ptr Ds : [0x078C5260], 0x0F; //MAIN_FONT_SIZE
//        Jmp EXIT;
//    NEXT4:
//        Mov Dword Ptr Ds : [0x078C5260], 0x0F; //MAIN_FONT_SIZE
//        Push Float2000;
//        Push Resolution_ClipX1;
//        Call[SetFloat];
//        Add Esp, 0x08;
//        Push Float2000;
//        Push Resolution_ClipX2;
//        Call[SetFloat];
//        Add Esp, 0x08;
//    EXIT:
//        Jmp[ResolutionSwitchFontAddress1];
//    }
//}
//
//__declspec(naked) void ResolutionMoveList()
//{
//    static DWORD ResolutionMoveListAddress1 = 0x000046863A;
//    static DWORD ResolutionMoveListAddress2 = 0x000046859D;
//
//
//    _asm
//    {
//        Mov Eax, Dword Ptr Ds : [0x007E7F68]; //MAIN_RESOLUTION_X 7E7F68
//        Mov Dword Ptr Ss : [Ebp - 0x1D4], Eax;//1D4
//        Cmp Dword Ptr Ss : [Ebp - 0x1D4], 0x500;
//        Jbe EXIT;
//        Jmp[ResolutionMoveListAddress1];
//    EXIT:
//        Jmp[ResolutionMoveListAddress2];
//    }
//}
//
//__declspec(naked) void ResolutionMoveList2()
//{
//    static DWORD ResolutionMoveListAddress3 = 0x00467F2B;
//    static DWORD ResolutionMoveListAddress4 = 0x00467E8E;
//
//
//    _asm
//    {
//        Mov Eax, Dword Ptr Ds : [0x007E7F68]; //MAIN_RESOLUTION_X 7E7F68
//        Mov Dword Ptr Ss : [Ebp - 0x1C8], Eax;//1C8
//        Cmp Dword Ptr Ss : [Ebp - 0x1C8], 0x500;
//        Jbe EXIT;
//        Jmp[ResolutionMoveListAddress3];
//    EXIT:
//        Jmp[ResolutionMoveListAddress4];
//    }
//}
//
//bool ResolutionKanturu()
//{
//    if (m_Resolution >= 4)
//    {
//        *(WORD*)(*(DWORD*)(0x007ED9E68) + 0x0C) = 0xD7;//
//	}
//
//    return ((bool(__stdcall*)(int Code)) 0x006F5BD6)(30);//
//}