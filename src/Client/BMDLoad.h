#pragma once
//---------------------------------------------------------------------------
#define iLoadModel			0x0069BA93
#define iLoadTexture		0x0069B617

// ----
#define iLoadModel_Call		0x006B3943
#define iLoadTexture_Call	0x006B3958
// ----
#define iLoadModel_Void		0x0069ECB7
#define iLoadTexture_Void	0x006A10BD
#define iLoadOther      	0x006B396D
#define iLoadOtherFunc      0x006A727E

//---------------------------------------------------------------------------

#define LoadModel	( ( void(*) () ) iLoadModel_Void)
#define LoadTexture ( ( void(*) () ) iLoadTexture_Void)
#define LoadOther   ((int(*)(int a1, int a2, int a3)) iLoadOtherFunc)

// ----
typedef void (* pLoadTexture) (int TextureID, char * Folder, int GLREPEAT, int GLNEAREST, int GLTRUE);
extern pLoadTexture Texture;

typedef void(*pLoadTextureOther) (int a1, int a2, int a3, int TextureID, char * Folder, int GLREPEAT, int GLNEAREST, int GLTRUE);
extern pLoadTextureOther TextureOther;

// ----
typedef void (* pLoadModel) (int ModelID, char * Folder, char * Name, int ModelType);
extern pLoadModel Model;

//---------------------------------------------------------------------------

void cBMDLoad();
#ifdef NEW_FENRIR
void cLoadFenrirModel();
#endif
void ModelsFloat();
void cTextureLoad();
void cModelLoad();
void cModelFloat();
void LoadCustomConfig();
void FenrirLoadTexture(int index, char* folder, char* name, int value);
void FenrirLoadModel(int index, char* folder, char* name, int value);
//---------------------------------------------------------------------------