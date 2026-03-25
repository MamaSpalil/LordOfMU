#pragma once

class CLoginWin
{
public:
	void Load();
	static void __thiscall cSetPosition(BYTE *This, int nXCoord, int nYCoord, int eChangedPram);

private:
};
extern CLoginWin gCLoginWin;

#define g_fScreenRate_x			*(float *)0x00587786C
#define g_fScreenRate_y			*(float *)0x005877870
float TransForX(float num);
#define TransFormX(a)			(a / g_fScreenRate_x)
#define TransFormY(a)			(a / g_fScreenRate_y)
#define TX(x)					(TransForX(TransFormX(x)))
#define TY(x)					(TransForX(TransFormY(x)))
#define Sprite_SetPosition		((void(__thiscall*)(BYTE * CButton, int cx, int cy, int eChangedPram)) 0x00006E57DE)//
//#define pSetBlend				((void(__cdecl*)(BYTE))0x00642288)
#define pSetBlend                ((void(__cdecl*)(BYTE Mode)) 0x6B8239) //ok
//#define CUIRenderText_SetFont	((int(__thiscall*)(int This, int a2)) 0x0041FFF9)
//#define pTextThis				((LPVOID(*)())0x0041FD65)
#define g_fScreenRate_x * (float*)0x00587786C
#define TransFormX(a)			(a / g_fScreenRate_x)
//#define pBackgroundTextColor	((void(__thiscall*)(LPVOID This, int r, int g, int b, int h)) 0x0041FF97)
//#define pSetTextColor			((void(__thiscall*)(LPVOID, BYTE, BYTE, BYTE, BYTE))0x0041FF35)//7D4C84
#define InputBox_SetText		((void(__thiscall*)(int thisa, const char* pszText)) 0x488BEF)//00
#define RequestLogin			((int(__thiscall*)(int This)) 0x006DEF32)//00

#define RenderBitmap            ((void(__cdecl*)(int ImageId,  float cordX, float cordY, float Width, float Height, float a7, float a8, float a9, float a10, char a11, char a12, float a13))  0x006B97A7)
#define pDrawToolTip			((int(__cdecl*)(int PosX, int PosY, LPCSTR lpText)) 0x5A0950)//00
#define pDrawTextOut			((int(__thiscall*)(LPVOID This, int PosX, int PosY, LPCTSTR Text, int size, int nTabPositions, LPINT lpnTabStopPositions, LPSIZE nTabOrigin)) 0x005A0790)//0x000x005A0790

#define pIsWorkZone				((int(__cdecl*)(int PosX, int PosY, int Width, int Height, int arg5))0x0047CEA4)//00
#define LeftClick				*(BYTE *)0x7EB383C
#define pPlayBuffer				((int(__cdecl*)(int,int,int))0x004207F5)//00
bool CheckMouseIn(int PosX, int PosY, int Width, int Height);

typedef struct
{
	char	FileName[32];
	/*+32*/	float	Width;
	/*+36*/	float	Height;
	/*+40*/	char	Components;
	/*+41*/	GLuint	TextureNumber;
	/*+46*/	BYTE* Buffer;

} BITMAP_t;