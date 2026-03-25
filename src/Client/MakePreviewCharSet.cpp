#include "stdafx.h"
#include "TMemory.h"
#include "MakePreviewCharSet.h"
#include "Structure.h"
#include "Other.h"

void PreviewCharSet(int ObjectIndex, BYTE * CharSet, LPBYTE Object, int Mode)
{
	((void(*)(int, LPBYTE, LPBYTE, int))0x0053FD2F)(ObjectIndex, CharSet, Object, Mode);


	BYTE WingCharSet = CharSet[16] >> 2;

	if (WingCharSet > 0)
	{
		*(WORD*)(*(int*)PREVIEW_STRUCT + (PREVIEW_STRUCT_SIZE * ObjectIndex) + 0x338) = ITEMGET2(12, 41) + (WingCharSet - 1);
	}
}
//---------------------------------------------------------------------------
void cMakePreviewCharSetLoad()
{
	//SetOp((LPVOID)0x004C0BD4, (LPVOID)PreviewCharSet, ASM::CALL);
	//SetOp((LPVOID)0x004E2EC7, (LPVOID)PreviewCharSet, ASM::CALL);
	//SetOp((LPVOID)0x004E9135, (LPVOID)PreviewCharSet, ASM::CALL);
	//SetOp((LPVOID)0x004EA2E0, (LPVOID)PreviewCharSet, ASM::CALL);
	//SetOp((LPVOID)0x004EAC11, (LPVOID)PreviewCharSet, ASM::CALL);
	//SetOp((LPVOID)0x00507F68, (LPVOID)PreviewCharSet, ASM::CALL);
	//SetOp((LPVOID)0x0050A7DF, (LPVOID)PreviewCharSet, ASM::CALL);
}