#include "stdafx.h"
#include "Texture.h"
#include "CustomFenrir.h"
#include "Structure.h"
#include "Other.h"
#include "TMemory.h"


void cTexture::Load()
{
	memset(this->TextStruct, 0, sizeof(this->TextStruct));

	//this->SetTexture();

	Count = MAX_TEXTURE;

	for (int i = 0; i < 21; i++)
	{
		*(DWORD*)(this->TexRef0x0[i], (DWORD)this->TextStruct);
	}

	for (int i = 0; i < 13; i++)
	{
		*(DWORD*)(this->TexRef0x20[i], (DWORD)this->TextStruct + 0x20);
	}

	for (int i = 0; i < 14; i++)
	{
		*(DWORD*)(this->TexRef0x24[i], (DWORD)this->TextStruct + 0x24);
	}

	for (int i = 0; i < 13; i++)
	{
		*(DWORD*)(this->TexRef0x28[i], (DWORD)this->TextStruct + 0x28);
	}

	*(DWORD*)(this->TexRef0x2E, (DWORD)this->TextStruct + 0x2E);

	for (int i = 0; i < 2; i++)
	{
		*(DWORD*)(this->TexRef0x6C4[i], (DWORD)this->TextStruct + 0x6C4);
	}

	for (int i = 0; i < 2; i++)
	{
		*(DWORD*)(this->TexRef0x6C8[i], (DWORD)this->TextStruct + 0x6C8);
	}

	for (int i = 0; i < 2; i++)
	{
		*(DWORD*)(this->TexRef0x6CC[i], (DWORD)this->TextStruct + 0x6CC);
	}

	for (int i = 0; i < 2; i++)
	{
		*(DWORD*)(this->TexRef0x6CD[i], (DWORD)this->TextStruct + 0x6CD);
	}

	for (int i = 0; i < 4; i++)
	{
		*(DWORD*)(this->TexRef0x6D2[i], (DWORD)this->TextStruct + 0x6D2);
	}

	*(DWORD*)(this->TexRef0x6FE, (DWORD)this->TextStruct + 0x6FE);

	//SetCompleteHook(0xE8, 0x69B673, cTexture::CheckTexture);//0069B673
	SetCompleteHook(0xE8, 0x6B392E, cTexture::TexturePlayerLoad);//006B392E
	SetCompleteHook(0xE8, 0x6B3958, cTexture::TextureItemLoad);//006B3958

}

void cTexture::SetTexture()
{
	//__asm
	//{
	//	Mov pTexture.TexRef0x0[4 * 0], 0x004203BC + 3;
	//	Mov pTexture.TexRef0x0[4 * 1], 0x0042048D + 3;
	//	Mov pTexture.TexRef0x0[4 * 2], 0x00421AF6 + 3;
	//	Mov pTexture.TexRef0x0[4 * 3], 0x00421DA4 + 3;
	//	Mov pTexture.TexRef0x0[4 * 4], 0x005FA38D + 3;
	//	Mov pTexture.TexRef0x0[4 * 5], 0x00606967 + 1;
	//	Mov pTexture.TexRef0x0[4 * 6], 0x00607A8D + 2;
	//	Mov pTexture.TexRef0x0[4 * 7], 0x0062B242 + 2;
	//	Mov pTexture.TexRef0x0[4 * 8], 0x0062B25F + 1;
	//	Mov pTexture.TexRef0x0[4 * 9], 0x0062B32C + 2;
	//	Mov pTexture.TexRef0x0[4 * 10], 0x0062B4DF + 1;
	//	Mov pTexture.TexRef0x0[4 * 11], 0x006366DA + 1;
	//	Mov pTexture.TexRef0x0[4 * 12], 0x00642025 + 2;
	//	Mov pTexture.TexRef0x0[4 * 13], 0x00642087 + 1;
	//	Mov pTexture.TexRef0x0[4 * 14], 0x0064C350 + 1;
	//	Mov pTexture.TexRef0x0[4 * 15], 0x006F28D3 + 1;
	//	Mov pTexture.TexRef0x0[4 * 16], 0x006F3193 + 1;
	//	Mov pTexture.TexRef0x0[4 * 17], 0x006F31A1 + 2;
	//	Mov pTexture.TexRef0x0[4 * 18], 0x006F9E06 + 1;
	//	Mov pTexture.TexRef0x0[4 * 19], 0x006FA2FB + 1;
	//	Mov pTexture.TexRef0x0[4 * 20], 0x006FA832 + 1;

	//	Mov pTexture.TexRef0x20[4 * 0], 0x004074EB + 2;
	//	Mov pTexture.TexRef0x20[4 * 1], 0x00414D88 + 2;
	//	Mov pTexture.TexRef0x20[4 * 2], 0x00414DBE + 2;
	//	Mov pTexture.TexRef0x20[4 * 3], 0x00414E00 + 2;
	//	Mov pTexture.TexRef0x20[4 * 4], 0x00414E42 + 2;
	//	Mov pTexture.TexRef0x20[4 * 5], 0x00414ECF + 2;
	//	Mov pTexture.TexRef0x20[4 * 6], 0x00415296 + 2;
	//	Mov pTexture.TexRef0x20[4 * 7], 0x004152EC + 2;
	//	Mov pTexture.TexRef0x20[4 * 8], 0x005FA386 + 3;
	//	Mov pTexture.TexRef0x20[4 * 9], 0x006ED616 + 2;
	//	Mov pTexture.TexRef0x20[4 * 10], 0x006EE1D9 + 2;
	//	Mov pTexture.TexRef0x20[4 * 11], 0x006EE329 + 2;
	//	Mov pTexture.TexRef0x20[4 * 12], 0x006EEA3A + 2;

	//	Mov pTexture.TexRef0x24[4 * 0], 0x00414DA3 + 2;
	//	Mov pTexture.TexRef0x24[4 * 1], 0x00414DDF + 2;
	//	Mov pTexture.TexRef0x24[4 * 2], 0x00414E21 + 2;
	//	Mov pTexture.TexRef0x24[4 * 3], 0x00414E5D + 2;
	//	Mov pTexture.TexRef0x24[4 * 4], 0x00414EFA + 2;
	//	Mov pTexture.TexRef0x24[4 * 5], 0x00415358 + 2;
	//	Mov pTexture.TexRef0x24[4 * 6], 0x004153AE + 2;
	//	Mov pTexture.TexRef0x24[4 * 7], 0x0048C08B + 2;
	//	Mov pTexture.TexRef0x24[4 * 8], 0x00606F24 + 2;
	//	Mov pTexture.TexRef0x24[4 * 9], 0x006E9FFA + 2;
	//	Mov pTexture.TexRef0x24[4 * 10], 0x006ED62E + 2;
	//	Mov pTexture.TexRef0x24[4 * 11], 0x006EE1F7 + 2;
	//	Mov pTexture.TexRef0x24[4 * 12], 0x006EE347 + 2;
	//	Mov pTexture.TexRef0x24[4 * 13], 0x006EEA50 + 2;

	//	Mov pTexture.TexRef0x28[4 * 0], 0x004F915B + 3;
	//	Mov pTexture.TexRef0x28[4 * 1], 0x004F9228 + 3;
	//	Mov pTexture.TexRef0x28[4 * 2], 0x004FA3A3 + 3;
	//	Mov pTexture.TexRef0x28[4 * 3], 0x004FA445 + 3;
	//	Mov pTexture.TexRef0x28[4 * 4], 0x00503B6E + 3;
	//	Mov pTexture.TexRef0x28[4 * 5], 0x00503BB4 + 3;
	//	Mov pTexture.TexRef0x28[4 * 6], 0x00504CBD + 3;
	//	Mov pTexture.TexRef0x28[4 * 7], 0x00504D03 + 3;
	//	Mov pTexture.TexRef0x28[4 * 8], 0x00506361 + 3;
	//	Mov pTexture.TexRef0x28[4 * 9], 0x00506391 + 3;
	//	Mov pTexture.TexRef0x28[4 * 10], 0x006430D8 + 3;
	//	Mov pTexture.TexRef0x28[4 * 11], 0x006ED643 + 3;
	//	Mov pTexture.TexRef0x28[4 * 12], 0x006F42B9 + 3;

	//	Mov pTexture.TexRef0x2E, 0x00606F11 + 2;

	//	Mov pTexture.TexRef0x6C4[4 * 0], 0x005FA173 + 2;
	//	Mov pTexture.TexRef0x6C4[4 * 1], 0x005FA71C + 2;

	//	Mov pTexture.TexRef0x6C8[4 * 0], 0x005FA182 + 2;
	//	Mov pTexture.TexRef0x6C8[4 * 1], 0x005FA72A + 2;

	//	Mov pTexture.TexRef0x6CC[4 * 0], 0x005FA2F3 + 3;
	//	Mov pTexture.TexRef0x6CC[4 * 1], 0x005FA7CF + 3;

	//	Mov pTexture.TexRef0x6CD[4 * 0], 0x005FA2DC + 1;
	//	Mov pTexture.TexRef0x6CD[4 * 1], 0x005FA7B8 + 1;

	//	Mov pTexture.TexRef0x6D2[4 * 0], 0x005FA193 + 2;
	//	Mov pTexture.TexRef0x6D2[4 * 1], 0x005FA2ED + 2;
	//	Mov pTexture.TexRef0x6D2[4 * 2], 0x005FA73B + 2;
	//	Mov pTexture.TexRef0x6D2[4 * 3], 0x005FA7C9 + 2;

	//	Mov pTexture.TexRef0x6FE, 0x005A3832 + 3;
	//}
}

void cTexture::TexturePlayerLoad()
{
	TextureCount = 302;

	pTexture.TextureType = 1;

	((void(*)())0x69E12E)();//0069E12E

	

	for (int n = 0; n < MAX_FENRIR_INFO; n++)
	{
		if (gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != -1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 0 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 2 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 4)
		{
			pTexture.FenrirLoadModel(gCustomFenrir.m_CustomFenrirInfo[n].Texture, "Skill\\", gCustomFenrir.m_CustomFenrirInfo[n].ModelName, -1);
		}
	}

	if (pTexture.TextureOverFlow)
	{
		pTexture.TextureOverFlow = 0;

		pTexture.Count = TextureCount;
	}

	pTexture.TextureType = 0;
}

void cTexture::TextureItemLoad()
{
	TextureCount = 757;

	pTexture.TextureType = 2;

	((void(*)())0x6A10BD)();//006A10BD

	

	for (int n = 0; n < MAX_FENRIR_INFO; n++)
	{
		if (gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != -1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 0 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 2 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 4)
		{
			pTexture.FenrirLoadTexture(gCustomFenrir.m_CustomFenrirInfo[n].Texture, "Skill\\", gCustomFenrir.m_CustomFenrirInfo[n].ModelName, -1);
		}
	}

	if (pTexture.TextureOverFlow)
	{
		pTexture.TextureOverFlow = 0;

		pTexture.Count = TextureCount;
	}

	pTexture.TextureType = 0;
}

DWORD cTexture::CheckTexture(char* Name, DWORD index)
{
	if (pTexture.TextureOverFlow == 0)
	{
		if (pTexture.TextureType == 1)
		{
			if (TextureCount >= 742)
			{
				pTexture.TextureOverFlow = 1;
			}
		}
		else if (pTexture.TextureType == 2)
		{
			if (TextureCount >= 1107)
			{
				pTexture.TextureOverFlow = 1;
			}
		}
	}

	if (pTexture.TextureOverFlow == 1)
	{
		TextureCount = pTexture.Count;
		pTexture.TextureOverFlow = 2;
	}

	return ((DWORD(*)(char*, DWORD))0x69B497)(Name, index);
}

void cTexture::LoadItemModel(int index, char* folder, char* name) // OK
{
	//if (name[0] == 0)
	//{
	//	return;
	//}

	//char path[MAX_PATH] = { 0 };

	//wsprintf(path, "Data\\%s", folder);

	//pLoadItemModel(index, path, name, -1);
}

void cTexture::LoadItemTexture(int index, char* folder) // OK
{
	//pLoadItemTexture(index, folder, GL_REPEAT, GL_NEAREST, GL_TRUE);
}

void cTexture::FenrirLoadModel(int index, char* folder, char* name, int value) // OK
{
	if (name[0] == 0)
	{
		return;
	}

	char path[MAX_PATH] = { 0 };

	wsprintf(path, "Data\\%s", folder);

	((void(*)(int, char*, char*, int))0x69BA93)(index, path, name, -1);
}

void cTexture::FenrirLoadTexture(int index, char* folder, char* name, int value)
{
	if (name[0] == 0)
	{
		return;
	}

	((void(*)(int, char*, int, int, int))0x69B617)(index, folder, GL_REPEAT, GL_NEAREST, GL_TRUE);
}

cTexture pTexture;