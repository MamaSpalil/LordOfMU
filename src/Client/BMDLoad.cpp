#include "stdafx.h"
#include "BMDLoad.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Interface.h"
#include "Structure.h"
#include "CustomFenrir.h"

//---------------------------------------------------------------------------

char * ModelsFolder				= "Data\\Customs\\Item\\";
char * TexFolder				= "Customs\\Item\\";
char * ModelsFolders			= "Data\\Item\\";
char * TexFolder1				= "Item\\";
char * SkillFolder				= "Skill\\";
char * ModelsFolderJewels		= "Data\\Customs\\Item\\Jewels\\";
char * TexFolderJewels			= "Customs\\Item\\Jewels\\";
char * ModelsFolderAmulet		= "Data\\Customs\\Item\\Amulet\\";
char * TexFolderAmulet			= "Customs\\Item\\Amulet\\";
//---------------------------------------------------------------------------

pLoadModel Model		= (pLoadModel) (iLoadModel);
pLoadTexture Texture	= (pLoadTexture) (iLoadTexture);
pLoadTextureOther TextureOther = (pLoadTextureOther)(iLoadTexture);
//pDrawPartyHP_Interface		MU_PartyHP_Interface = (pDrawPartyHP_Interface)(iDrawPartyHP_Interface);

// ----
DWORD dwModelFloat_Pointer;
float fModelFloat_Float;
DWORD dwModelFloat_Stack;
//---------------------------------------------------------------------------

void cBMDLoad()
{
	SetHook((LPVOID)cTextureLoad, (LPVOID)iLoadTexture_Call, ASM::CALL);
	SetHook((LPVOID)cModelLoad, (LPVOID)iLoadModel_Call, ASM::CALL);
	SetNop(0x0065092A,12);//
	HookOffset((DWORD)&ModelsFloat, 0x0065092A, 0xE9); 
}
//---------------------------------------------------------------------------



__inline __declspec(naked) void ModelsFloat()
{
 _asm
    {
		CMP ESI,0x1EB6
		JE Float
		/*CMP ESI,ITEM2(14,157)
		JE Float*/
		CMP ESI,ITEM2(14,102)
		JE Float
		CMP ESI,ITEM2(14,103)
		JE Float
		CMP ESI,ITEM2(14,104)
		JE Float
		CMP ESI,ITEM2(12,45)
		JE Float
		CMP ESI,ITEM2(12,41)
		JE Float
		CMP ESI,ITEM2(12,43)
		JE Float
		//CMP ESI,ITEM2(0,0)
		//JE Float
		//CMP ESI,ITEM2(14,153)
		//JE Float
		//CMP ESI,ITEM2(14,154)
		//JE Float
		//CMP ESI,ITEM2(14,155)
		//JE Float
		/*CMP ESI,ITEM2(14,157)
		JE Float*/

		mov eax,0x00650936
		JMP eax

		Float:
		mov eax,0x00650b79
		JMP eax
	 }
} 



//---------------------------------------------------------------------------

void cModelLoad()
{
	LoadModel();
	// ----
	Model(ITEM2(14, 89), ModelsFolder, "MasterSealB", -1);
//Scorpion Mobility
//	Model(ITEM2(14, 189),	ModelsFolder, "MasterSealB",  -1);
//Jewels
	Model(ITEM2(14, 90),	ModelsFolderJewels, "Jewel01",  -1);
	Model(ITEM2(14, 91),	ModelsFolderJewels, "Jewel02",  -1);
	Model(ITEM2(14, 92),	ModelsFolderJewels, "Jewel03",  -1);
	Model(ITEM2(14, 93),	ModelsFolderJewels, "Jewel04",  -1);
	Model(ITEM2(14, 100),	ModelsFolderJewels, "Jewel13",  -1);
	Model(ITEM2(14, 95),	ModelsFolderJewels, "Jewel06",  -1);
	Model(ITEM2(14, 96),	ModelsFolderJewels, "Jewel07",  -1);
	Model(ITEM2(14, 97),	ModelsFolderJewels, "Jewel14",  -1);
	Model(ITEM2(14, 98),	ModelsFolderJewels, "Jewel09",  -1);
	Model(ITEM2(14, 99),	ModelsFolderJewels, "Jewel10",  -1);

//Moss the Gambler
	Model(ITEM2(14, 84),	ModelsFolder, "gamble_bowx01",  -1);
	Model(ITEM2(14, 85),	ModelsFolder, "gamble_scyderx01",  -1);
	Model(ITEM2(14, 86),	ModelsFolder, "gamble_stickx01",  -1);
	Model(ITEM2(14, 87),	ModelsFolder, "gamble_wand01",  -1);
	Model(ITEM2(14, 88),	ModelsFolder, "gamble_safterx01",  -1);
//New Wings
#ifdef CUSTOM_WINGS
	Model(ITEM2(12, 41),	ModelsFolder, "WingOfAngel",  -1);   //SM
	Model(ITEM2(12, 42),	ModelsFolder, "WingsOfPower",  -1);   //BK
	Model(ITEM2(12, 43),	ModelsFolder, "WingsOfButterfly",  -1);   //Elf
	Model(ITEM2(12, 44),	ModelsFolder, "WingsOfDream",  -1);   //MG
	Model(ITEM2(12, 45),	ModelsFolder, "MantleOfDarkness",  -1);   //DL
	//Model(ITEM2(12, 45), ModelsFolder, "Cloak_of_Death", -1);   //DL
#endif
	Model(ITEM2(14, 102),	ModelsFolder, "PlatinaCoin",  -1);
	Model(ITEM2(14, 103),	ModelsFolder, "BronzeCoin",  -1);
	Model(ITEM2(14, 104),	ModelsFolder, "GoldenCoin",  -1);

	Model(ITEM2(14, 105),	ModelsFolder, "roboxgreen",  -1);
	Model(ITEM2(14, 106),	ModelsFolder, "roboxpurple",  -1);
	Model(ITEM2(14, 107),	ModelsFolder, "roboxred",  -1);
	//
	Model(ITEM2(14, 147),	ModelsFolder, "heywoodpet_evol",  -1);
	//
	//Model(ITEM2(14, 105),	ModelsFolder, "PackOfLife",  -1);
	//Model(ITEM2(14, 106),	ModelsFolder, "PackOfChaos",  -1);
	//Model(ITEM2(14, 107),	ModelsFolder, "PackOfCreation",  -1);
	//Model(ITEM2(14, 108),	ModelsFolder, "PackOfGuardian",  -1);
	//Model(ITEM2(14, 109),	ModelsFolder, "PackOfHarmony",  -1);

	Model(ITEM2(2, 16), ModelsFolder, "gamble_scyder01", -1);

	//Model(ITEM2(15, 19), ModelsFolder, "Book19", -1);

	//Model(ITEM2(14, 150), ModelsFolderAmulet, "penta01", -1);
	//Model(ITEM2(14, 151), ModelsFolderAmulet, "penta02", -1);
	//Model(ITEM2(14, 152), ModelsFolderAmulet, "PentaCintamani", -1);
	//Model(ITEM2(14, 153), ModelsFolderAmulet, "pentaexdan", -1);
	//Model(ITEM2(14, 154), ModelsFolderAmulet, "pentaexshi", -1);
	//Model(ITEM2(14, 155), ModelsFolderAmulet, "pentaharp", -1);
	//Model(ITEM2(14, 156), ModelsFolderAmulet, "pentaliquid", -1);

	Model(ITEM2(14, 150), ModelsFolderAmulet, "sd_rune", -1);
	Model(ITEM2(14, 151), ModelsFolderAmulet, "sd_rune01", -1);
	Model(ITEM2(14, 152), ModelsFolderAmulet, "sd_rune02", -1);
	Model(ITEM2(14, 153), ModelsFolderAmulet, "sd_rune03", -1);
	Model(ITEM2(14, 154), ModelsFolderAmulet, "sd_rune04", -1);
	Model(ITEM2(14, 155), ModelsFolderAmulet, "sd_rune05", -1);
	Model(ITEM2(14, 156), ModelsFolderAmulet, "sd_rune06", -1);

	//Model(ITEM2(15, 18), ModelsFolderAmulet, "sd_rune", -1);
	Model(ITEM2(15, 19), ModelsFolders, "Book19", -1);

	for (int n = 0; n < MAX_FENRIR_INFO; n++)
	{
		if (gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != -1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 0 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 2 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 4)
		{
			FenrirLoadModel(gCustomFenrir.m_CustomFenrirInfo[n].Texture, "Skill\\", gCustomFenrir.m_CustomFenrirInfo[n].ModelName, -1);
		}
	}
}

//---------------------------------------------------------------------------

void cTextureLoad()
{
	LoadTexture();
	// ----

	Texture(ITEM2(14, 84), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 85), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 86), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 87), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 88), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	//Scorpion Mobility
	Texture(ITEM2(14, 89), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	//Jewels

	Texture(ITEM2(14, 90), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 91), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 92), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 93), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 100), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 95), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 96), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 97), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 98), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 99), TexFolderJewels, GL_REPEAT, GL_NEAREST, GL_TRUE );
	//New Wings
	#ifdef CUSTOM_WINGS
	Texture(ITEM2(12, 41), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE ); //SM
	Texture(ITEM2(12, 42), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE ); //BK
	Texture(ITEM2(12, 43), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE ); //Elf
	Texture(ITEM2(12, 44), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE ); //MG.
	Texture(ITEM2(12, 45), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE ); //DL
	#endif

	Texture(ITEM2(14, 102), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 103), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 104), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );

	Texture(ITEM2(14, 105), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	Texture(ITEM2(14, 106), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 107), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	//
	Texture(ITEM2(14, 147), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
		
	//Texture(ITEM2(14, 105), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	//Texture(ITEM2(14, 106), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	//Texture(ITEM2(14, 107), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	//Texture(ITEM2(14, 108), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );
	//Texture(ITEM2(14, 109), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE );

	Texture(ITEM2(2, 16), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE);


	//Texture(ITEM2(15, 19), TexFolder, GL_REPEAT, GL_NEAREST, GL_TRUE);

	Texture(ITEM2(14, 150), TexFolderAmulet, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 151), TexFolderAmulet, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 152), TexFolderAmulet, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 153), TexFolderAmulet, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 154), TexFolderAmulet, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 155), TexFolderAmulet, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 156), TexFolderAmulet, GL_REPEAT, GL_NEAREST, GL_TRUE);
	Texture(ITEM2(14, 156), TexFolderAmulet, GL_REPEAT, GL_NEAREST, GL_TRUE);

	Texture(ITEM2(15, 19), TexFolder1, GL_REPEAT, GL_NEAREST, GL_TRUE );

	for (int n = 0; n < MAX_FENRIR_INFO; n++)
	{
		if (gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != -1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 0 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 1 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 2 && gCustomFenrir.m_CustomFenrirInfo[n].ExeOption != 4)
		{
			FenrirLoadTexture(gCustomFenrir.m_CustomFenrirInfo[n].Texture, "Skill\\", gCustomFenrir.m_CustomFenrirInfo[n].ModelName, -1);
		}
	}
}

void FenrirLoadModel(int index, char* folder, char* name, int value) // OK
{
	if (name[0] == 0)
	{
		return;
	}

	char path[MAX_PATH] = { 0 };

	wsprintf(path, "Data\\%s", folder);

	((void(*)(int, char*, char*, int))0x0069BA93)(index, path, name, -1);
}
void FenrirLoadTexture(int index, char* folder, char* name, int value)
{
	if (name[0] == 0)
	{
		return;
	}

	((void(*)(int, char*, int, int, int))0x0069B617)(index, folder, GL_REPEAT, GL_NEAREST, GL_TRUE);
}

//---------------------------------------------------------------------------

