#pragma once

#define MAX_FENRIR_INFO 64

struct CUSTOM_FENRIR_INFO
{
	int ExeOption;
	char PetName[32];
	int Texture;
	char ModelName[32];
	float ModelScale;
	int SkillID;
	float SkillR;
	float SkillG;
	float SkillB;
	int Effect;
};

class CCustomFenrir
{
public:

	CCustomFenrir();

	~CCustomFenrir();

	void Load(CUSTOM_FENRIR_INFO* info);

	void SetInfo(CUSTOM_FENRIR_INFO info);

	void Init();

	static void CustomFenrirSkin();

	static void CustomFenrirVisualBug();

	static void CustomFenrirChangeFix();

	static void CustomFenrirName();

	static void CustomFenrirOptions();

	static void CustomFenrirMovement();

	static void CustomFenrirScale1();

	static void CustomFenrirScale2();

	static void CustomFenrirSkillColor();

	static void CustomFenrirEffect();

	static void ChangeCharacterExt(int Key, BYTE* Equipment, DWORD pCharacter, DWORD pHelper);

	static void FenrirMakePreviewCharSet();

public:

	CUSTOM_FENRIR_INFO m_CustomFenrirInfo[MAX_FENRIR_INFO];
};

extern CCustomFenrir gCustomFenrir;

