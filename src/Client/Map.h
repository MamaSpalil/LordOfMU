#pragma once

class CCustomMap
{
public:
	// ----
	void	Load();
	// ----
}; extern CCustomMap gCustomMap;


#define pTextLineThis			((LPVOID(*)()) 0x0780525C)//780525C
#define pGetTextLine			((char*(__thiscall*)(LPVOID This, int LineNumber)) 0x00402260)
#define pMapName				((char*(__cdecl*)(signed int mapNumber)) 0x00662DB0)//662DB0


#define sub_461510 ((int(*)(int a1))0x00461510) //ok 
#define sub_408710 ((int(*)(int a1))0x00408710) //ok 