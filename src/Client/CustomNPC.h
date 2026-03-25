#pragma once

//struct MONSTER_DATA
//{
//	int Index;
//	int Type;
//	char Name[25];
//	char Folder[MAX_PATH];
//	char BMDFile[MAX_PATH];
//	float Size;
//};
//
//class CMonsterEx
//{
//public:
//	CMonsterEx();
//	virtual ~CMonsterEx();
//
//	void Init();
//	//void Load();
//	void Read(char* filename);
//
//	void InitMonster();
//	static DWORD MonsterRender(int index,int x,int y,int key);
//	static DWORD MonsterSetup(DWORD ViewportAddress,int index);
//	int Count;
//	MONSTER_DATA m_Data[512];
//};
//
//extern CMonsterEx g_MonsterEx;
//
//
//
//#define	pLoadItemModel			((void(*)(int,char*,char*,int))0x0062B803)
//#define	pLoadItemTexture		((void(*)(int,char*,int,int,int))0x0062B387)
void NpcManagerInit();
void CreateNpc(int a1, int a2, int a3, int a4);

#define pLoadMonsterModel									((char(__cdecl*)(int)) 0x006A3817)

#define pCreateMonster										((int(__cdecl*)(int Key, int ModelId, DWORD PositionX, DWORD PositionY, int Unk)) 0x0053E757)
#define pCreateJoint										((void(__cdecl*)(signed int a1, int a2, int a3, int a4, int a5, int a6, float a7, __int16 a8, unsigned __int16 a9, char a10, int a11, int a12)) 0x00566220)

