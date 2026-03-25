#include "stdafx.h"
#include "Items.h"
#include "CustomFenrir.h"

void cTexture::Models()
{
	((void(*)())0x69ECB7)();

	for (int i = 0; i < 512; i++)
	{
		if (i > 23 && i < 31 || i > 31)
		{
			sprintf_s(Texture._Buffer, "Sword%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((0 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);

		}

		if (i > 8)
		{
			sprintf_s(Texture._Buffer, "Axe%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((1 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);
		}

		if (i > 14)
		{
			sprintf_s(Texture._Buffer, "Mace%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((2 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);
		}

		if (i > 10)
		{
			sprintf_s(Texture._Buffer, "Spear%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((3 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);
		}

		if (i > 12)
		{
			sprintf_s(Texture._Buffer, "Staff%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((5 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);
		}

		if (i > 16)
		{
			sprintf_s(Texture._Buffer, "Shield%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((6 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);
		}

		if (i >= 41 && i <= 66)
		{
			sprintf_s(Texture._Buffer, "Wing%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((12 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);
		}

		if (i >= 84 && i <= 200)
		{
			sprintf_s(Texture._Buffer, "Jewel%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((14 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);
		}

		if (i > 32)
		{
			sprintf_s(Texture._Buffer, "HelmMale%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((7 * 512) + i + 0x27B, "Data\\Player\\", Texture._Buffer, -1);
		}

		if (i > 33)
		{
			sprintf_s(Texture._Buffer, "ArmorMale%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((8 * 512) + i + 0x27B, "Data\\Player\\", Texture._Buffer, -1);
		}

		if (i > 33)
		{
			sprintf_s(Texture._Buffer, "PantMale%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((9 * 512) + i + 0x27B, "Data\\Player\\", Texture._Buffer, -1);
		}

		if (i > 33)
		{
			sprintf_s(Texture._Buffer, "GloveMale%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((10 * 512) + i + 0x27B, "Data\\Player\\", Texture._Buffer, -1);
		}

		if (i > 33)
		{
			sprintf_s(Texture._Buffer, "BootMale%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((11 * 512) + i + 0x27B, "Data\\Player\\", Texture._Buffer, -1);
		}
		if (i > 17)
		{
			sprintf_s(Texture._Buffer, "Book%d", i + 1);
			((void(*)(int, char*, char*, int))0x69BA93)((5 * 512) + i + 0x27B, "Data\\Item\\", Texture._Buffer, -1);
		}

	}
	/*for (int n = 0; n < MAX_FENRIR_INFO; n++)
	{
		if (gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != -1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 0 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 2 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 4)
		{
			Texture.FenrirLoadModel(gCustomFenrir.m_CustomFenrirInfo[n].Texture, "Skill\\", gCustomFenrir.m_CustomFenrirInfo[n].ModelName, -1);
		}
	}*/
}

void cTexture::Textures()
{
	((void(*)())0x6A10BD)();

	for (int i = 0; i < 512; i++)
	{
		if (i > 23 && i < 31 || i > 31)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((0 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 8)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((1 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 14)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((2 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 10)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((3 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 12)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((5 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 16)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((6 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i >= 41 && i <= 66)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((12 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i >= 84 && i <= 200)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((14 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 33)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((7 * 512) + i + 0x27B, "Player\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 33)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((8 * 512) + i + 0x27B, "Player\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 33)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((9 * 512) + i + 0x27B, "Player\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 33)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((10 * 512) + i + 0x27B, "Player\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}

		if (i > 33)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((11 * 512) + i + 0x27B, "Player\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}
		if (i > 17)
		{
			((void(*)(int, char*, int, int, int))0x69B617)((15 * 512) + i + 0x27B, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}
	}

	//for (int n = 0; n < MAX_FENRIR_INFO; n++)
	//{
	//	if (gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != -1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 0 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 2 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 4)
	//	{
	//		Texture.FenrirLoadTexture(gCustomFenrir.m_CustomFenrirInfo[n].Texture, "Skill\\", gCustomFenrir.m_CustomFenrirInfo[n].ModelName, -1);
	//	}
	//}
}

//void cTexture::FenrirLoadModel(int index, char* folder, char* name, int value) // OK
//{
//	if (name[0] == 0)
//	{
//		return;
//	}
//
//	char path[MAX_PATH] = { 0 };
//
//	wsprintf(path, "Data\\%s", folder);
//
//	((void(*)(int, char*, char*, int))0x0069BA93)(index, path, name, -1);
//}
//void cTexture::FenrirLoadTexture(int index, char* folder, char* name, int value)
//{
//	if (name[0] == 0)
//	{
//		return;
//	}
//
//	((void(*)(int, char*, int, int, int))0x0069B617)(index, folder, GL_REPEAT, GL_NEAREST, GL_TRUE);
//}

cTexture Texture;