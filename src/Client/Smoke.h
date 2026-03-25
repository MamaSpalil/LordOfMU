#pragma once
#define MAX_SMOKER		512
#define MAX_SMOKE_EFFECT 100
#define SMOKE_EFFECT_DIR "Data\\Customs\\Configs\\ItemSmoke.bmd"
// ----------------------------------------------------------------------------------------------

struct SmokeEffect_DATA
{
	short Index;
	short Level;
	float Red;
	float Green;
	float Blue;
};

struct SmokerConfig
{
	BYTE ColorR;
	BYTE ColorG;
	BYTE ColorB;
};
class CItemSmoker
{
public:

	CItemSmoker();

	~CItemSmoker();

	void ReadList(char * File);
	void Init();
	BOOL CheckSmokerEnable();
	BOOL GetSmokerColor(int index, FLOAT *r, FLOAT *g, FLOAT *b);
	DWORD SmokeItemType;
	DWORD SmokeRed;
	DWORD SmokeGreen;
	DWORD SmokeBlue;
	// ----
	int SmokeCount;
	SmokerConfig MuSmokerConfig[MAX_SMOKER];
	SmokeEffect_DATA Data[MAX_SMOKE_EFFECT];
};

extern CItemSmoker gItemSmoker;