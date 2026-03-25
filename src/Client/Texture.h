#pragma once

#define ITEM(X,Y)               (X * 512 + Y) + 0x27B//0x27B
#define MAX_TEXTURE				2182//2120
#define MAX_PLUS_TEXTURE		768
#define TextureCount			*(DWORD*)(0x07EB366C)//7EB366C
#define TextureLoad  			((void(*)(int, char*, char*, int)) 0x69BA93)//0069BA93
#define TextureCall  			((void(*)(int, char*, int, int, int)) 0x69B617)//0069B617

struct sTexture
{
	char* Name[50];
};

class cTexture
{
public:
	void Load();
	void SetTexture();
	static void TexturePlayerLoad();
	static void TextureItemLoad();
	static DWORD CheckTexture(char* Name, DWORD index);
	void LoadItemModel(int index, char* folder, char* name);
	void LoadItemTexture(int index, char* folder);
	void FenrirLoadModel(int index, char* folder, char* name, int value);
	void FenrirLoadTexture(int index, char* folder, char* name, int value);

public:
	int TextureType;
	int TextureOverFlow;
	DWORD TexRef0x0[21];
	DWORD TexRef0x20[13];
	DWORD TexRef0x24[14];
	DWORD TexRef0x28[13];
	DWORD TexRef0x2E;
	DWORD TexRef0x6C4[2];
	DWORD TexRef0x6C8[2];
	DWORD TexRef0x6CC[2];
	DWORD TexRef0x6CD[2];
	DWORD TexRef0x6D2[4];
	DWORD TexRef0x6FE;
	DWORD Count;
	char _Buffer[30];

public:
	sTexture TextStruct[MAX_TEXTURE + MAX_PLUS_TEXTURE];
};

extern cTexture pTexture;
