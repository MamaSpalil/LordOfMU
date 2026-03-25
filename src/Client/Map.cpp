#include "stdafx.h"
#include "Map.h"
#include "TMemory.h"


CCustomMap gCustomMap;


//char * Map51 = "Interface\\Elbeland.tga";
//char * Map56 = "Interface\\lorencia.tga";
char * Map57 = "Interface\\devias.tga";
//char * Map58 = "Interface\\RaklionBoss.tga";
//char * Map62 = "Interface\\santatown.tga";
//char * Map63 = "Interface\\Vulcanus.tga";
//char * Map64 = "Interface\\duelarena.tga";
//
//char * Map83 = "Interface\\Acheron.tga";
//char * Map84 = "Interface\\Debenter.tga";
char * Map85 = "Interface\\lorencia.tga";
//char * Map86 = "Interface\\Ferea.tga";
//char * Map87 = "Interface\\lorencia.tga";
//char * Map86 = "Interface\\lorencia.tga";
//char * Map89 = "Interface\\NixiesLake.tga";
//char * Map90 = "Interface\\DeepDungeon.tga";
//char * Map91 = "Interface\\DeepDungeon.tga";
//char * Map92 = "Interface\\DeepDungeon.tga";
//char * Map93 = "Interface\\DeepDungeon.tga";
//char * Map94 = "Interface\\DeepDungeon.tga";
//char * Map95 = "Interface\\SwampOfDarkness.tga";
//char * Map96 = "Interface\\KuberaMine.tga";

char* LoadMapName(signed int MapNumber)
{
	//if (MapNumber >= 56 && MapNumber <= 100)
	//{
	//	//return;// pGetTextLine(pTextLineThis, (2532 + MapNumber - 56));
	//}
	//return pMapName(MapNumber);
	if(MapNumber == 0x55)
	{
		return "TvT Map";
	}
	if(MapNumber == 0x39)
	{
		return "Raklion";
	}
	return pMapName(MapNumber);
}



void __declspec(naked) LoadNewInterfaceMapName()
{
	_asm
	{

		MOV DWORD PTR SS : [EBP - 0x0B4], 0x32 // MOV DWORD PTR SS:[EBP-0x0D0],0x32 // 0x32 = 50 IT Final (Hook place)
		PUSH 0x007E95F4 //      7E95F4              // .PUSH 0x0076C05C                      // Arg1 = ASCII "Interface\IllusionTemple.tga"
		LEA EAX, DWORD PTR SS : [EBP - 0x0B4]
		PUSH EAX
		MOV ECX, DWORD PTR SS : [EBP - 0x0B8]
		ADD ECX, 0x4
		CALL InterfaceLoad1
		MOV ECX, EAX
		CALL InterfaceLoad2


		//      MOV DWORD PTR SS:[EBP-0x0B4],0x33
		//      PUSH Map51                 
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		/*MOV DWORD PTR SS:[EBP-0x0B4],0x38
		      PUSH Map56                 
		      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		      PUSH EAX
		      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		      ADD ECX,0x4
		      CALL InterfaceLoad1
		      MOV ECX,EAX
		      CALL InterfaceLoad2*/

		MOV DWORD PTR SS:[EBP-0x0B4],0x39
		      PUSH Map57                 
		      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		      PUSH EAX
		      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		      ADD ECX,0x4
		      CALL InterfaceLoad1
		      MOV ECX,EAX
		      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x3A
		//      PUSH Map58                
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x3E
		//      PUSH Map62                
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x3F
		//      PUSH Map63                
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x40
		//      PUSH Map64                
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2


		//MOV DWORD PTR SS:[EBP-0x0B4],0x53
		//      PUSH Map83                 
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2



		//MOV DWORD PTR SS:[EBP-0x0B4],0x54
		//      PUSH Map84                 
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		MOV DWORD PTR SS:[EBP-0x0B4],0x55
		      PUSH Map85                 
		      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		      PUSH EAX
		      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		      ADD ECX,0x4
		      CALL InterfaceLoad1
		      MOV ECX,EAX
		      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x56
		//      PUSH Map86                 
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x57
		//      PUSH Map87                 
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x58
		//      PUSH Map88                 
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x59
		//      PUSH Map89               
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x5A
		//      PUSH Map90               
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x5B
		//      PUSH Map91               
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x5C
		//      PUSH Map92               
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x5D
		//      PUSH Map93               
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x5E
		//      PUSH Map94               
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x5F
		//      PUSH Map95               
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2

		//MOV DWORD PTR SS:[EBP-0x0B4],0x60
		//      PUSH Map96              
		//      LEA EAX,DWORD PTR SS:[EBP-0x0B4]
		//      PUSH EAX
		//      MOV ECX,DWORD PTR SS:[EBP-0x0B8]
		//      ADD ECX,0x4
		//      CALL InterfaceLoad1
		//      MOV ECX,EAX
		//      CALL InterfaceLoad2




		MOV ESP, EBP
		POP EBP
		RETN


		InterfaceLoad1 :
		MOV EDI, 0x006F8C80
			JMP EDI
			// ---
			InterfaceLoad2 :
		MOV EDI, 0x00414670
			JMP EDI

	}
}
//===============================================================
//		Load New Map no Wellcome
//===============================================================
DWORD MAPNUMBER	= 0;
DWORD NEWMAP_JMP = 0x006F88CF;
DWORD OLDMAP_JMP = 0x006F88DE;

__declspec(naked)void NewMapLoader()
{
	_asm
	{
		MOV EAX, DWORD PTR DS:[0x7D60C8]
		MOV MAPNUMBER, EAX
	}
	if(MAPNUMBER == 40)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 80)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 81)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 82)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 83)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 84)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 85)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 86)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 87)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 88)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 89)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else if(MAPNUMBER == 90)	//Original
	{
		_asm{JMP NEWMAP_JMP}
	}
	else
	{
		_asm{JMP OLDMAP_JMP}
	}
}

void CCustomMap::Load()
{

	SetOp((LPVOID)0x006631B6, (LPVOID)LoadMapName, ASM::CALL);//6631B6
	SetOp((LPVOID)0x004E574E, (LPVOID)LoadMapName, ASM::CALL);//4E574E
	SetOp((LPVOID)0x004E899D, (LPVOID)LoadMapName, ASM::CALL);//4E899D
	//SetNop(0x006F88C6,16);
	//WriteJmp(0x006F88C6, (DWORD)&NewMapLoader);//6F88C6

	WriteJmp(0x006F8823, (DWORD)&LoadNewInterfaceMapName);//6F8823

														  //Bypass in terrains files
	SetByte2((PVOID)0x006AE2D6, 0xEB);//6AE2D6
	SetByte2((PVOID)0x006AE2DC, 0xEB);//6AE2DC
	SetByte2((PVOID)0x006AE4C9, 0xEB);//6AE4C9
	SetByte2((PVOID)0x006AE4CF, 0xEB);//6AE4CF
	SetByte2((PVOID)0x006AE56C, 0xEB);//6AE56C
	SetByte2((PVOID)0x006AE572, 0xEB);//6AE572

									  //Increase terrains amount
	SetByte2((PVOID)0x006AE2D5, 0x69);//6AE2D5
	SetByte2((PVOID)0x006AE4C8, 0x69);//6AE4C8
	SetByte2((PVOID)0x006AE56B, 0x69);//6AE56B

}