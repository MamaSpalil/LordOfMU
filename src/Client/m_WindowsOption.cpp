#include "stdafx.h"
#include "m_WindowsOption.h"
#include "Other.h"
#include "Interface.h"
#include "InterfaceEx.h"
#include "TMemory.h"
#include "m_Disable.h"
#include "Structure.h"
#include "SelectChar.h"
#include "define.h"
#include "Resolution.h"
#include "CameraEx.h"
#include "HPBar.h"

COptionsMenu gOptionsMenu;

float CalculateCenterPosX(float size)
{
	const float screenCenterX = 320.0f;
	return (screenCenterX - (size / 2.0f));
}

float CalculateCenterPosY(float size)
{
	const float screenCenterY = 216.0f;
	return (screenCenterY - (size / 2.0f));
}

void RenderText(HFONT font,int x,int y,char* text,bool centered = false)
{
	SIZE sz;

	//GetTextExtentPoint(pGetFontDC(pTextThis()), text, strlen(text), &sz);
	GetTextExtentPoint(*(HDC*)(0x5877F38), text, strlen(text), &sz);

	int px = (x - (640 * sz.cx / *(DWORD*)MAIN_RESOLUTION_X >> 1));

	int py = (y - (480 * sz.cy / *(DWORD*)MAIN_RESOLUTION_Y >> 1));

	EnableAlphaTest(true);

	//pSetTextFont(pTextThis(),font);
	//pSetBGTextColor(pTextThis(), 0, 0, 0, 0);
	//pSetTextColor(pTextThis(),255,255,255,255);
	//pDrawText(pTextThis(),((centered)?px:x),py,text,0,0,0,0);
	pDrawTextOut((int)((centered)?px:x),py,text, 0, 0, 0);
	//pDrawTextOut(pTextThis(), X + PosXMove + TransFormX(32), PosYMoveB, iUser1, TransFormX(57), 0, (LPINT)3, 0);
	//pSetTextFont(pTextThis(),pFontNormal);
}

void PushButton()
{
	MouseLButtonPush = 0;

	MouseUpdateTime = 0;
	MouseUpdateTimeMax = 6;

	pPlayBuffer(25,0,0);
}

COptionsMenu::COptionsMenu()
	: DeleteFloor(0),
	  DeleteMapObjects(0),
	  DeleteObjects(0),
	  DeleteShadows(0),
	  DeleteEffects(0),
	  DeleteSkills(0),
	  DeleteAttackBar(0),
	  MainWidth(340.0f),
	  MainHeight(270.0f),
	  MainPosX(CalculateCenterPosX(MainWidth)),
	  MainPosY(CalculateCenterPosY(MainHeight)),
	  SectionWidth(100),
	  SectionPosY(MainPosY + 30.0f),
	  AntilagPosX(160.0f),
	  GeneralPosX(265.0f),
	  ResolutionPosX(375.0f),
	  FontPosX(455.0f)
{
	for (int i = 0; i < MAX_FONTS; i++)
	{
		Resolutions[i] = 0;
		Fonts[i] = (i * 2) + 9;
	}

	const char* resolutionsText[MAX_RESOLUTIONS] = {
		"640x480",
		"800x600","1024x768","1280x1024",
		"1360x768","1440x900","1600x900","1680x1050","1920x1080"
	};

	for (int n = 0; n < MAX_RESOLUTIONS; n++)
	{
		sprintf_s(ResolutionsText[n], "%s", resolutionsText[n]);
	}
}

COptionsMenu::~COptionsMenu()
{

}

void COptionsMenu::Init()
{
	/*if(gProtect.m_MainInfo.AdvancedOptionMenuSwitch == 0)
	{
		return;
	}*/

	SetCompleteHook(0xE9,0x006C78CE,&DrawOptionsMenuUI);//006C78CE

	SetCompleteHook(0xE9,0x006BDEA8,&HandleOptionsMenuControl);//006BDEA8
}

__declspec(naked) void COptionsMenu::DrawOptionsMenuUI()
{
	static DWORD originalCodeAddress = 0x006C8328 ;//006C8328 

	_asm
	{
		PushAd
	}

	gOptionsMenu.DrawOptionsMenuMain();
	gOptionsMenu.DrawOptionsMenuRender();
	gOptionsMenu.DrawOptionsMenuGeneral();
	gOptionsMenu.DrawOptionsMenuResolution();
	gOptionsMenu.DrawOptionsMenuFont();
	gOptionsMenu.DrawOptionsMenuClose();

	_asm
	{
		PopAd
		Jmp[originalCodeAddress]
	}
}

__declspec(naked) void COptionsMenu::HandleOptionsMenuControl()
{
	static DWORD originalCodeAddress = 0x006BE21B;//006BE21B

	_asm
	{
		PushAd
	}

	if (gOptionsMenu.IsDeleteFloorEnabled() ||
		gOptionsMenu.IsDeleteMapObjectsEnabled() ||
		gOptionsMenu.IsDeleteObjectsEnabled() ||
		gOptionsMenu.IsDeleteShadowsEnabled() ||
		gOptionsMenu.IsDeleteEffectsEnabled() ||
		gOptionsMenu.IsDeleteSkillsEnabled() ||
		gOptionsMenu.IsMonsterHealthBarEnabled() ||
		gOptionsMenu.IsAutoAttackBarEnabled() ||
		gOptionsMenu.IsAutomaticAttackEnabled() ||
		gOptionsMenu.IsSoundWhisperEnabled() ||
		gOptionsMenu.IsVolumeEnabled() ||
		gOptionsMenu.IsSlideHelpEnabled() ||
		gOptionsMenu.IsGlowEnabled() ||
		gOptionsMenu.IsResolutionsEnabled() ||
		gOptionsMenu.IsFontSizeEnabled() ||
		gOptionsMenu.IsCloseEnabled())
	{
		originalCodeAddress = 0x006BE216;//006BE216
	}

	_asm
	{
		PopAd
		Jmp[originalCodeAddress]
	}
}

void COptionsMenu::DrawOptionsMenuMain()
{
	EnableAlphaTest(1);
	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	pDrawBarForm(this->MainPosX, this->MainPosY, this->MainWidth, this->MainHeight);

	glColor3f(1.0, 1.0, 1.0);
	DisableAlphaBlend();

	EnableAlphaTest(true);

	//pSetTextFont(pTextThis(),pFontBold);
	//pSetBGTextColor(pTextThis(), 0, 0, 0, 0);
	//pSetTextColor(pTextThis(),255,0,0,255);
	//pDrawText(pTextThis(),320, (int)this->MainPosY + 5, gCustomMessage.GetMessage(6), 0, 0, 8, 0);
	pDrawTextOut((int)280, (int)this->MainPosY + 5,"Advanced Menu Options", 0, 0, 0);
	DisableAlphaBlend();
}

void COptionsMenu::DrawOptionsMenuRender()
{
	RenderText(pFontBold,(int)this->AntilagPosX, (int)this->SectionPosY,"Render Options");

	DrawFloorDeletionOption();
	DrawMapObjectDeletionOption();
	DrawObjectsDeletionOption();
	DrawShadowDeletionOption();
	DrawEffectDeletionOption();
	DrawSkillDeletionOption();
	DrawMonsterHealthBarOption();
	DrawAutoAttackBarOption();
}

void COptionsMenu::DrawOptionsMenuGeneral()
{
	char Text[64] = { 0 };
	sprintf_s(Text, "General");

	RenderText(pFontBold,(int)this->GeneralPosX, (int)this->SectionPosY,Text);

	this->DrawAutomaticAttackOption();
	this->DrawSoundWhisperOption();
	this->DrawVolumeOption();
	this->DrawSlideHelpOption();
	this->DrawGlowOption();
}

void COptionsMenu::DrawOptionsMenuResolution()
{
	char resolutionTitle[64] = { 0 };
	sprintf_s(resolutionTitle, "Resolution");

	RenderText(pFontBold,(int)this->ResolutionPosX, (int)this->SectionPosY, resolutionTitle);

	int optionPosX = (int)this->ResolutionPosX;
	int optionPosY = (int)this->SectionPosY + 20;
	int optionWidth = 67;
	int optionHeight = 19;
	this->Resolutions[*(DWORD*)MAIN_RESOLUTION] = 1;

	for (int i = 0; i < MAX_RESOLUTIONS; i++)
	{
		EnableAlphaTest(true);

		glColor3f(0.2f, 0.2f, 0.2f);
		pDrawBarForm((float)optionPosX, (float)optionPosY, (float)optionWidth, (float)optionHeight);

		if (this->Resolutions[i])
			glColor3f(0.8f, 0.6f, 0.3f);
		else if (pMouseOnZone(optionPosX, optionPosY, optionWidth, optionHeight, 1))
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		else
			glColor3f(0.05f, 0.05f, 0.05f);

		pDrawBarForm((float)optionPosX + 0.75f, (float)optionPosY + 0.75f, (float)optionWidth - 1.5f, (float)optionHeight - 1.5f);

		glColor3f(1.0f, 1.0f, 1.0f);

		glEnable(GL_TEXTURE_2D);

		EnableAlphaTest(false);

		RenderText(pFontNormal,optionPosX+(optionWidth/2),optionPosY+(optionHeight/2),this->ResolutionsText[i],true);

		optionPosY += 20;
	}
}

void COptionsMenu::DrawOptionsMenuFont()
{
	char titleText[64] = { 0 };
	sprintf_s(titleText, "Font");

	RenderText(pFontBold,(int)this->FontPosX, (int)this->SectionPosY, titleText);

	int optionPosX = (int)this->FontPosX;
	int optionPosY = (int)this->SectionPosY + 20;
	int optionWidth = 20;
	int optionHeight = 19;

	for (int i = 0; i < MAX_FONTS; i++)
	{
		EnableAlphaTest(true);

		glColor3f(0.2f, 0.2f, 0.2f);
		pDrawBarForm((float)optionPosX, (float)optionPosY, (float)optionWidth, (float)optionHeight);

		if (this->Fonts[i] == FontSize)
			glColor3f(0.8f, 0.6f, 0.3f);
		else if (pMouseOnZone(optionPosX, optionPosY, optionWidth, optionHeight, 1))
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		else
			glColor3f(0.05f, 0.05f, 0.05f);

		pDrawBarForm((float)optionPosX + 0.75f, (float)optionPosY + 0.75f, (float)optionWidth - 1.5f, (float)optionHeight - 1.5f);

		glColor3f(1.0f, 1.0f, 1.0f);

		glEnable(GL_TEXTURE_2D);

		EnableAlphaTest(false);

		char optionText[5] = { 0 };

		sprintf_s(optionText, "%d", this->Fonts[i]);

		RenderText(pFontNormal,optionPosX+(optionWidth/2),optionPosY+(optionHeight/2),optionText,true);

		optionPosY += 20;
	}
}

void COptionsMenu::DrawFloorDeletionOption()
{
	int PosY = (int)this->SectionPosY + 20;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Floor Map");

	if (!this->DeleteFloor)
		pDrawImage2(152, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(151, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->AntilagPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawMapObjectDeletionOption()
{
	int PosY = (int)this->SectionPosY + 40;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Map Objects");

	if (!this->DeleteMapObjects)
		pDrawImage2(152, this->AntilagPosX, (float)PosY, (float)Width, (float)Height,  0.01, 0.02999999747, 0.5799999952, 0.3700000346, 1, 1, 0);//0.02999999747, 0.8799999952, 0.3700000346, 1, 1, 0);//0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(153, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.01, 0.49999999747, 0.8799999952, 0.3700000346, 1, 1, 0);// 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->AntilagPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawObjectsDeletionOption()
{
	int PosY = (int)this->SectionPosY + 60;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Entities");

	if (!this->DeleteObjects)
		pDrawImage2(152, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(152, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->AntilagPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawShadowDeletionOption()
{
	int PosY = (int)this->SectionPosY + 80;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Shadows");

	if (!this->DeleteShadows)
		pDrawImage2(152, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(152, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->AntilagPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawEffectDeletionOption()
{
	int PosY = (int)this->SectionPosY + 100;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Effects");

	if (!this->DeleteEffects)
		pDrawImage2(152, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(152, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->AntilagPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawSkillDeletionOption()
{
	int PosY = (int)this->SectionPosY + 120;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Skill Effects");

	if (!this->DeleteSkills)
		pDrawImage2(2074, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(2073, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->AntilagPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawMonsterHealthBarOption()
{
	int PosY = (int)this->SectionPosY + 140;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Monster Health Bar");

	if (!MonsterHPBarMode)
		pDrawImage2(2086, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(2071, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->AntilagPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawAutoAttackBarOption()
{
	int PosY = (int)this->SectionPosY + 160;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Auto Attack Bar");

	if (!this->DeleteAttackBar)
		pDrawImage2(2070, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(2069, this->AntilagPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->AntilagPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawAutomaticAttackOption()
{
	int PosY = (int)this->SectionPosY + 20;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Automatic Attack");

	if (*(BYTE*)0x007D4C64)//7D4C64
		pDrawImage2(2068, this->GeneralPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(2067, this->GeneralPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->GeneralPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawSoundWhisperOption()
{
	int PosY = (int)this->SectionPosY + 40;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text,  "Whisper Sounds");

	if (*(BYTE*)0x07960BB4)//7960BB4
		pDrawImage2(2066, this->GeneralPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(2065, this->GeneralPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->GeneralPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawVolumeOption()
{
	int PosY = (int)this->SectionPosY + 80;
	float Height = 10.0f;
	int MAX_VALUE = 9;
	char Text[64] = { 0 };
	sprintf_s(Text, "%s: %d",  "Volume", *(DWORD*)0x07EB71F8);//7EB71F8

	RenderText(pFontNormal,(int)this->GeneralPosX, PosY + ((int)Height / 2) - 15, Text);

	EnableAlphaTest(1);

	glColor3f(0.2f, 0.2f, 0.2f);
	pDrawBarForm(this->GeneralPosX, (float)PosY, (float)this->SectionWidth, Height);

	glColor3f(0.0f, 0.0f, 0.0f);
	pDrawBarForm(this->GeneralPosX + 1.0f, (float)PosY + 1, (float)this->SectionWidth - 2.0f, Height - 2.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	float X = this->GeneralPosX + 1.0f;

	for (int i = 0; i < MAX_VALUE; i++)
	{
		if (i >= *(int*)0x07EB71F8)//7EB71F8
			glColor3f(0.0f, 0.0f, 0.0f);
		else
			glColor3f(0.8f, 0.6f, 0.3f);

		pDrawBarForm(X, (float)PosY + 1.0f, (((float)this->SectionWidth - 2.0f) / MAX_VALUE), Height - 2.0f);

		X += (((float)this->SectionWidth - 2.0f) / MAX_VALUE);
	}

	pTextureBlind();
}

void COptionsMenu::DrawSlideHelpOption()
{
	int PosY = (int)this->SectionPosY + 110;
	int Width = 10;
	int Height = 10;
	char Text[64] = { 0 };
	sprintf_s(Text, "Slide Help");

	if (*(BYTE*)0x007D3048)//7D3048
		pDrawImage2(152, this->GeneralPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.5f, 1.0f, 0.5f, 1, 1, 0.0);
	else
		pDrawImage2(152, this->GeneralPosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 1.0f, 0.5f, 1, 1, 0.0);

	RenderText(pFontNormal,((int)this->GeneralPosX + Width + 5), PosY + 5,Text);
}

void COptionsMenu::DrawGlowOption()
{
	int PosY = (int)this->SectionPosY + 150;
	float Height = 10.0f;
	int MAX_VALUE = 4;
	char Text[64] = { 0 };
	sprintf_s(Text, "%s: %d", "+Effect limitation", (2 * *(BYTE*)0x007D60C7) + 5);//7D60C7

	RenderText(pFontNormal,(int)this->GeneralPosX, PosY + ((int)Height / 2) - 15, Text);

	EnableAlphaTest(1);

	glColor3f(0.2f, 0.2f, 0.2f);
	pDrawBarForm(this->GeneralPosX, (float)PosY, (float)this->SectionWidth, Height);

	glColor3f(0.0f, 0.0f, 0.0f);
	pDrawBarForm(this->GeneralPosX + 1.0f, (float)PosY + 1.0f, (float)this->SectionWidth - 2.0f, Height - 2.0f);

	float X = this->GeneralPosX + 1.0f;

	for (int i = 0; i < MAX_VALUE; i++)
	{
		if (i >= *(BYTE*)0x007D60C7)//7D60C7
			glColor3f(0.0f, 0.0f, 0.0f);
		else
			glColor3f(0.8f, 0.6f, 0.3f);

		pDrawBarForm(X, (float)PosY + 1.0f, (((float)this->SectionWidth - 2.0f) / MAX_VALUE), Height - 2.0f);

		X += (((float)this->SectionWidth - 2.0f) / MAX_VALUE);
	}

	pTextureBlind();
}

void COptionsMenu::DrawOptionsMenuClose()
{
	int PosX = (int)CalculateCenterPosX(34.0f);
	int PosY = (int)this->MainPosY + (int)this->MainHeight - 30;
	int Width = 34;
	int Height = 19;

	if (pMouseOnZone((int)PosX, (int)PosY, (int)Width, (int)Height, 1))
	{
		if (!MouseLButtonPush)
		{
			pDrawImage2(152, (float)PosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.235f, 0.843f, 0.24f, 1, 1, 0.0);
		}
		else
		{
			pDrawImage2(152, (float)PosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.47f, 0.843f, 0.24f, 1, 1, 0.0);
		}
	}
	else
	{
		pDrawImage2(2075, (float)PosX, (float)PosY, (float)Width, (float)Height, 0.0f, 0.0f, 0.843f, 0.24f, 1, 1, 0.0);
	}
}

bool COptionsMenu::IsDeleteFloorEnabled()
{
	int PosY = (int)this->SectionPosY + 20;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->AntilagPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		if((this->DeleteFloor ^= 1))
		{
			BYTE replace[10] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
			MemoryCpy(0x006D124C, replace, sizeof(replace));//6D124C
		}
		else
		{
			//BYTE replace[10] = { 0x6A, 0x00, 0xE8, 0x55, 0x7B, 0xFA, 0xFF, 0x83, 0xC4, 0x04 };
			BYTE replace[10] = { 0x6A, 0x00, 0xE8, 0x21, 0x6B, 0xFA, 0xFF, 0x83, 0xC4, 0x04 };
			MemoryCpy(0x006D124C, replace, sizeof(replace));//6D124C
			//CPU Disasm

		}

		return true;
	}

	return false;
}

bool COptionsMenu::IsDeleteMapObjectsEnabled()
{
	int PosY = (int)this->SectionPosY + 40;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->AntilagPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		if((this->DeleteMapObjects ^= 1))
			SetByte(0x006846D9, 0xC3);//006846D9
		else
			SetByte(0x006846D9, 0x55);//006846D9

		return true;
	}

	return false;
}

bool COptionsMenu::IsDeleteObjectsEnabled()
{
	int PosY = (int)this->SectionPosY + 60;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->AntilagPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		if ((this->DeleteObjects ^= 1))
			SetByte(0x00534E27, 0xC3);//00534E27
		else
			SetByte(0x00534E27, 0x55);//00534E27

		return true;
	}

	return false;
}

bool COptionsMenu::IsDeleteShadowsEnabled()
{
	int PosY = (int)this->SectionPosY + 80;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->AntilagPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		if ((this->DeleteShadows ^= 1))
			SetByte(0x0051A061, 0xC3);//0051A061
		else
			SetByte(0x0051A061, 0x55);//0051A061

		return true;
	}

	return false;
}

bool COptionsMenu::IsDeleteEffectsEnabled()
{
	int PosY = (int)this->SectionPosY + 100;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->AntilagPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		if ((this->DeleteEffects ^= 1))
		{
			SetByte(0x0057F691, 0xC3); // pCreateParticle//0057F691
			SetByte(0x00593928, 0xC3); // pCreateSprite//00593928
		}
		else
		{
			SetByte(0x0057F691, 0x55);//0057F691
			SetByte(0x00593928, 0x55);//00593928
		}

		return true;
	}

	return false;
}

bool COptionsMenu::IsDeleteSkillsEnabled()
{
	int PosY = (int)this->SectionPosY + 120;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->AntilagPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		if ((this->DeleteSkills ^= 1))
		{
			SetByte(0x006A727E, 0xC3);//006A727E
			SetByte(0x00566220, 0xC3);//00566220
			SetByte(0x00547D36, 0);//00547D36
		}
		else
		{
			SetByte(0x006A727E, 0x55);//006A727E
			SetByte(0x00566220, 0x55);//00566220
			SetByte(0x00547D36, 1);//00547D36
		}

		return true;
	}

	return false;
}

bool COptionsMenu::IsMonsterHealthBarEnabled()
{
	int PosY = (int)this->SectionPosY + 140;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->AntilagPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		MonsterHPBarMode ^= 1;

		return true;
	}

	return false;
}

bool COptionsMenu::IsAutoAttackBarEnabled()
{
	int PosY = (int)this->SectionPosY + 160;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->AntilagPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		this->DeleteAttackBar ^= 1;

		return true;
	}

	return false;
}

bool COptionsMenu::IsAutomaticAttackEnabled()
{
	int PosY = (int)this->SectionPosY + 20;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->GeneralPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		*(BYTE*)0x007D4C64 ^= 1;////7D4C64

		return true;
	}

	return false;
}

bool COptionsMenu::IsSoundWhisperEnabled()
{
	int PosY = (int)this->SectionPosY + 40;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->GeneralPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		*(BYTE*)0x07960BB4 ^= 1;////7960BB4

		return true;
	}

	return false;
}

bool COptionsMenu::IsVolumeEnabled()
{
	int PosY = (int)this->SectionPosY + 80;
	float Height = 10.0f;
	int MAX_VALUE = 9;
	int newVolumeLevel = 0;
	int cursorInicial = (int)this->GeneralPosX + 1;

	while (pCursorX < cursorInicial || pCursorX > cursorInicial + (int)(((float)this->SectionWidth - 2.0f) / MAX_VALUE) || pCursorY < (int)PosY + 1 || pCursorY >(int)PosY + ((int)Height - 1) || !MouseLButtonPush)
	{
		cursorInicial += (int)(((float)this->SectionWidth - 2.0f) / MAX_VALUE);
		newVolumeLevel++;

		if (cursorInicial >= (int)(this->GeneralPosX + (float)this->SectionWidth))
			return false;
	}

	*(DWORD*)0x07EB71F8 = newVolumeLevel;////7EB71F8

	static const float _1_div_log_2 = 1 / log(2.0f);

	float volumeSound = 0.30102999566398119802f * log(10.0f / newVolumeLevel) * _1_div_log_2;

	volumeSound *= -2000.0f;

	((int(__cdecl*)(int a1)) 0x00420A7F)((int)volumeSound);//00420A7F

	return true;
}

bool COptionsMenu::IsSlideHelpEnabled()
{
	int PosY = (int)this->SectionPosY + 110;
	int Width = 10;
	int Height = 10;

	if (pMouseOnZone((int)this->GeneralPosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		*(BYTE*)0x007D3048 ^= 1;////7D3048

		return true;
	}

	return false;
}

bool COptionsMenu::IsGlowEnabled()
{
	int PosY = (int)this->SectionPosY + 150;
	float Height = 10.0f;
	int MAX_VALUE = 4;
	int glowLevel = 0;
	int cursorInicial = (int)this->GeneralPosX + 1;

	while (pCursorX < cursorInicial || pCursorX > cursorInicial + (int)(((float)this->SectionWidth - 2.0f) / MAX_VALUE) || pCursorY < (int)PosY + 1 || pCursorY >(int)PosY + ((int)Height - 1) || !MouseLButtonPush)
	{
		cursorInicial += (int)(((float)this->SectionWidth - 2.0f) / MAX_VALUE);
		glowLevel++;

		if (cursorInicial >= (int)(this->GeneralPosX + (float)this->SectionWidth))
			return false;
	}

	*(BYTE*)0x007D60C7 = glowLevel;////7D60C7

	return true;
}

bool COptionsMenu::IsResolutionsEnabled()//tyt
{
	int PosX = (int)this->ResolutionPosX;
	int PosY = (int)this->SectionPosY + 20;
	int Width = 67;
	int Height = 19;

	for (int i = 0; i < MAX_RESOLUTIONS; i++)
	{
		if (pMouseOnZone(PosX, PosY, Width, Height, 1) && MouseLButtonPush)
		{
			if (this->Resolutions[i] == 1)
			{
				return true;
			}

			PushButton();

			this->Resolutions[*(DWORD*)MAIN_RESOLUTION] = 0;

			this->Resolutions[i] = 1;

			HKEY key;

			if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
			{
				if (RegSetValueEx(key, "Resolution", 0, REG_DWORD, (const BYTE*)&i, sizeof(i)) == ERROR_SUCCESS)
				{
					((int(*)())0x00004DBE14)(); //OpenInitFile//004DBE14

					RECT rect;
					rect.left = 0;
					rect.top = 0;
					rect.right = *(DWORD*)MAIN_RESOLUTION_X;
					rect.bottom = *(DWORD*)MAIN_RESOLUTION_Y;
					LONG PosX = ((GetSystemMetrics(SM_CXSCREEN)) / 2) - (*(DWORD*)MAIN_RESOLUTION_X / 2);
					LONG PosY = ((GetSystemMetrics(SM_CYSCREEN)) / 2) - (*(DWORD*)MAIN_RESOLUTION_Y / 2);

					if (!*(BOOL*)0x007D3270)//7D3270
					{
						LONG STYLE = WS_POPUP;
						LONG EXSTYLE = WS_EX_APPWINDOW /*| WS_EX_TOPMOST*/;
						SetWindowLongPtr(*(HWND*)(MAIN_WINDOW), GWL_STYLE, STYLE); // Set the Window Style
						SetWindowLongPtr(*(HWND*)(MAIN_WINDOW), GWL_EXSTYLE, EXSTYLE); // Set the Window Extra Style
						AdjustWindowRect(&rect, STYLE, FALSE); // Adjust the rectangle inside
					}
					else
					{
						LONG STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN;
						SetWindowLongPtr(*(HWND*)(MAIN_WINDOW), GWL_STYLE, STYLE); // Set the Window Style
						AdjustWindowRect(&rect, STYLE, FALSE); // Adjust the rectangle inside
					}

					SetWindowPos(*(HWND*)(MAIN_WINDOW), NULL, PosX, PosY, rect.right - rect.left, rect.bottom - rect.top,
						SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

					MoveWindow(*(HWND*)(MAIN_WINDOW), PosX, PosY, rect.right - rect.left, rect.bottom - rect.top, TRUE); // Change the size

					if (!*(BOOL*)0x007D3270)//7D3270
					{
						this->ApplyFullScreenSettings();
					}

					gCamera->SetCurrentValue(); // Fix the 3d camera position

					ReloadFont();

					// Update the CUIMng saved width and height
					DWORD V1 = ((DWORD(*)()) 0x006FAAE2)(); //CUIMng::Instance()//006FAAE2
					*(DWORD*)(V1 + 40) = *(DWORD*)MAIN_RESOLUTION_X;
					*(DWORD*)(V1 + 44) = *(DWORD*)MAIN_RESOLUTION_Y;

					// Update the CInput saved width and height
					DWORD V2 = ((DWORD(*)()) 0x006DC2E1)(); //CInput::Instance()//006DC2E1
					*(DWORD*)(V2 + 40) = *(DWORD*)MAIN_RESOLUTION_X;
					*(DWORD*)(V2 + 44) = *(DWORD*)MAIN_RESOLUTION_Y;

					// Update the CGameCensorship saved width and height
					DWORD V3 = ((DWORD(*)()) 0x006FAAE2)(); //CGameCensorship::GetInstance()//006FAAE2
					((void(__thiscall*)(DWORD This)) 0x006FAB2F)(V3); //CGameCensorship::Create()//006FAB2F

					SetByte(0x00488C7B, 0x20); // Skip Window Resize Check//00488C7B

					//// Chat and Whisper Input
					((void(__thiscall*)(DWORD This, int iWidth, int iHeight)) 0x00488C6A)(((*(DWORD*)0x05877F3C) + 4), 180, 14);//	//5877F3C
					((void(__thiscall*)(DWORD This, int iWidth, int iHeight)) 0x00488C6A)(((*(DWORD*)0x05877F3C) + 240), 50, 14);//	//5877F3C
					// User and Password Input
					((void(__thiscall*)(DWORD This, int iWidth, int iHeight)) 0x00488C6A)(((*(DWORD*)0x05877F40) + 4), 100, 14);//	//5877F40
					((void(__thiscall*)(DWORD This, int iWidth, int iHeight)) 0x00488C6A)(((*(DWORD*)0x05877F40) + 240), 100, 14);//	//5877F40
					// 
					((void(__thiscall*)(DWORD This, int iWidth, int iHeight)) 0x00488C6A)(((*(DWORD*)0x05877F44) + 4), 100, 14);//	//5877F44
					((void(__thiscall*)(DWORD This, int iWidth, int iHeight)) 0x00488C6A)(((*(DWORD*)0x05877F44) + 240), 100, 14);//	//5877F44
					//
					((void(__thiscall*)(DWORD This, int iWidth, int iHeight)) 0x00488C6A)((*(DWORD*)0x05877F48), 200, 20);//	//5877F48
					//
					((void(__thiscall*)(DWORD This, int iWidth, int iHeight)) 0x00488C6A)((*(DWORD*)0x05877F4C), 200, 20);//	//5877F4C

					SetByte(0x00488C7B, 0x05); // Restore Window Resize Check//00488C7B
				}
				else
				{
					this->Resolutions[*(DWORD*)MAIN_RESOLUTION] = 1;

					this->Resolutions[i] = 0;
				}

				RegCloseKey(key);
			}

			return true;
		}

		PosY += 20;
	}

	return false;
}

bool COptionsMenu::IsFontSizeEnabled()
{
	int PosX = (int)this->FontPosX;
	int PosY = (int)this->SectionPosY + 20;
	int Width = 20;
	int Height = 19;

	for (int i = 0; i < MAX_FONTS; i++)
	{
		if (pMouseOnZone(PosX, PosY, Width, Height, 1) && MouseLButtonPush)
		{
			if (this->Fonts[i] == FontSize)
			{
				return true;
			}

			PushButton();

			FontSize = this->Fonts[i];

			ReloadFont();

			return true;
		}

		PosY += 20;
	}

	return false;
}

bool COptionsMenu::IsCloseEnabled()
{
	int PosX = (int)CalculateCenterPosX(34.0f);
	int PosY = (int)this->MainPosY + (int)this->MainHeight - 30;
	int Width = 34;
	int Height = 19;

	if (pMouseOnZone(PosX, PosY, Width, Height, 1) && MouseLButtonPush)
	{
		PushButton();

		ErrorMessage = *(DWORD*)0x07EB71F4;//7EB71F4

		*(DWORD*)0x07EB71F4 = 0;//7EB71F4

		return true;
	}

	return false;
}

void COptionsMenu::ApplyFullScreenSettings()
{
	DEVMODE DevMode;

	DEVMODE* pDevmodes;

	int nModes = 0;

	while (EnumDisplaySettings(NULL, nModes, &DevMode))
	{
		nModes++;
	}

	pDevmodes = new DEVMODE[nModes + 1]; //. Create mod list

	nModes = 0;

	while (EnumDisplaySettings(NULL, nModes, &pDevmodes[nModes]))
	{
		nModes++;
	}

	DWORD dwBitsPerPel = 16;

	for (int n1 = 0; n1 < nModes; n1++)
	{
		if (pDevmodes[n1].dmBitsPerPel == 16 && *(int*)0x05877D7C == 0)
		{
			dwBitsPerPel = 16;
			break;
		}

		if (pDevmodes[n1].dmBitsPerPel == 24 && *(int*)0x05877D7C == 1)
		{
			dwBitsPerPel = 24;
			break;
		}

		if (pDevmodes[n1].dmBitsPerPel == 32 && *(int*)0x05877D7C == 1)
		{
			dwBitsPerPel = 32;
			break;
		}
	}

	for (int n2 = 0; n2 < nModes; n2++)
	{
		if (pDevmodes[n2].dmPelsWidth == *(int*)MAIN_RESOLUTION_X && pDevmodes[n2].dmPelsHeight == *(int*)MAIN_RESOLUTION_Y && pDevmodes[n2].dmBitsPerPel == dwBitsPerPel)
		{
			ChangeDisplaySettings(&pDevmodes[n2], 0);
			break;
		}
	}

	delete[] pDevmodes; //. remove mod list
}
//
//bool m_DisableObject;
//bool m_DisableWeapons;
//bool m_DisableArmors;
//bool m_DisableWings;
//bool m_DisablePlayer;
//bool m_DisableShadow;
//bool m_DisableEffStatic;
//bool m_DisableEffDynamic;
//bool m_DisableFog;
//
//COptionWindow gOptionWindow;
//
//COptionWindow::COptionWindow() {
//	this->Main();
//}
//
//COptionWindow::~COptionWindow(void) {
//}
//
//void COptionWindow::DrawBarFormex(float PosX, float PosY, float Width, float Height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
//	pSetBlend(1);
//	glColor4f(red, green, blue, alpha);
//	pDrawBarFormex(PosX, PosY, Width, Height, 0.0f, 0);
//	pGLSwitchBlend();
//	pGLSwitch();
//	glColor3f(1.0, 1.0, 1.0);
//}
//
//bool COptionWindow::IsWorkZone(float a1, float a2, float a3, float a4) {
//	return MouseXe <= (a1 + a3) && MouseXe >= a1 && MouseYe <= (a2 + a4) && MouseYe >= a2;
//}
//
//void COptionWindow::RenderButtonsAntiLag(AntiLag_Options id, bool type) {
//	switch (id) {
//	    case m_MapObject:
//			if (type == true) {
//				m_Disable.KG_ActivateInvisibleObjects_1();
//			}else {
//				m_Disable.KG_ActivateInvisibleObjects_2();
//			}
//			break;
//		case m_ItemWeapon:
//			if (type == true) {
//				m_Disable.KG_ActivateInvisibleWeapons_1();
//			}else {
//				m_Disable.KG_ActivateInvisibleWeapons_2();
//			}
//			break;
//		case m_ItemArmors:
//			if (type == true) {
//				m_Disable.KG_ActivateInvisibleArmors_1();
//			}else {
//				m_Disable.KG_ActivateInvisibleArmors_2();
//			}
//			break;
//		case m_ItemWings:
//			if (type == true) {
//				m_Disable.KG_ActivateInvisibleWings_1();
//			}else {
//				m_Disable.KG_ActivateInvisibleWings_2();
//			}
//			break;
//		case m_MapPlayer:
//			if (type == true) {
//				m_Disable.KG_ActivateInvisiblePlayer_1();
//			}else {
//				m_Disable.KG_ActivateInvisiblePlayer_2();
//			}
//			break;
//		case m_MapShadow:
//			if (type == true) {
//				m_Disable.KG_ActiveShadow_1();
//			}else {
//				m_Disable.KG_ActiveShadow_2();
//			}
//			break;
//		case m_EffStatic:
//			if (type == true) {
//				m_Disable.KG_ActivateStaticEffect_1();
//			}else {
//				m_Disable.KG_ActivateStaticEffect_2();
//			}
//			break;
//		case m_EffDynamic:
//			if (type == true) {
//				m_Disable.KG_ActivateDynamicEffect_1();
//			}else {
//				m_Disable.KG_ActivateDynamicEffect_2();
//			}
//			break;
//		case m_Fog:
//			if (type == true) {
//				m_Disable.KG_ActivateFog_1();
//			}else {
//				m_Disable.KG_ActivateFog_2();
//			}
//			break;
//		default:
//			break;
//	}
//}
//
//void COptionWindow::RenderFrame(int a1, float a2, float a3) {
//	glColor4f(0.0, 0.0, 0.0, 0.80000001);
//	pDrawBarFormex(170.f, 40.f, 300.f, 290.f, 0, 0);
//	gOptionWindow.DrawBarFormex(170.f, 40.f, 300.0f, 12.0f, 0.4, 0.0, 0.0, 1.0);
//
//	EnableAlphaTest(true);
//	glColor3f(1.0, 1.0, 1.0);
//
//	RenderText(285.f, 42.f, "Options Panel", 0, 0, 0);
//
//	gOptionWindow.RenderContents();
//	gOptionWindow.RenderButtons();
//
//	EnableAlphaTest(false);
//	return;
//}
//
//void RenderButton(const char* label, bool& option, AntiLag_Options bitmapIndex, float x, float y) {
//	if (CheckMouseIn(x, y, 24.0, 24.0, 1) && MouseLButtonPush) {
//		MouseLButtonPush = 0;
//		MouseUpdateTime = 0;
//		MouseUpdateTimeMax = 6;
//		PlayBuffer(25, 0, 0);
//
//		if (gOptionWindow.IsWorkZone(x, y, 24.0, 24.0) && GetTickCount() >= gOptionWindow.Delay_AntiLag + 250) {
//			MouseLButtonPush = 1;
//			option = !option;
//			gOptionWindow.RenderButtonsAntiLag(bitmapIndex, option);
//			gOptionWindow.Delay_AntiLag = GetTickCount();
//		}
//	}else {
//		if (option) {
//			RenderBitmap(291, x, y, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
//			RenderText(x + 27.5, y + 5.f, &((std::string(label) + " (OFF)")[0]), 0, 0, 0);
//		}else {
//			RenderBitmap(290, x, y, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
//			RenderText(x + 27.5, y + 5.f, &((std::string(label) + " (ON)")[0]), 0, 0, 0);
//		}
//	}
//}
//
//void COptionWindow::RenderContents() {
//	float startX = 172.f;
//	float startY = 60.f;
//
//	RenderButton("Objects", m_DisableObject, m_MapObject, startX, startY);
//	startY += 30;
//
//	RenderButton("Weapons", m_DisableWeapons, m_ItemWeapon, startX, startY);
//	startY += 30;
//
//	RenderButton("Armors", m_DisableArmors, m_ItemArmors, startX, startY);
//	startY += 30;
//
//	RenderButton("Wings", m_DisableWings, m_ItemWings, startX, startY);
//	startY += 30;
//
//	RenderButton("Player", m_DisablePlayer, m_MapPlayer, startX, startY);
//	startY += 30;
//
//	RenderButton("Shadow", m_DisableShadow, m_MapShadow, startX, startY);
//	startY += 30;
//
//	RenderButton("Eff Static", m_DisableEffStatic, m_EffStatic, startX, startY);
//	startY += 30;
//
//	RenderButton("Eff Dynamic", m_DisableEffDynamic, m_EffDynamic, startX, startY);
//	startY += 30;
//
//	RenderButton("Fog", m_DisableFog, m_Fog, startX, startY);
//}
//
//void COptionWindow::RenderButtons() {
//	const int buttonCount = 6;
//	const float buttonSpacing = 30.f;
//
//	for (int i = 0; i < buttonCount; ++i) {
//		float buttonHeight = (i == 4) ? 44.f : 22.f;
//		float buttonY = 60.f + i * (buttonSpacing + ((i == buttonCount - 1) ? 4.f : 0.f));
//
//		EnableAlphaTest(true);
//
//		if (pCursorX >= 260 && pCursorX < 380 && pCursorY >= buttonY && pCursorY < buttonY + buttonHeight) {
//			glColor3f(0.9f, 0.9f, 0.9f);
//		}else {
//			glColor3f(1.0f, 1.0f, 1.0f);
//		}
//
//		RenderBitmap(240, 260.0, buttonY, 120.0, buttonHeight, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
//
//		glColor3f(1.0, 1.0, 1.0);
//		EnableAlphaTest(false);
//	}
//}
//
//
//void COptionWindow::Main() {
//	SetByte(0x006C78E9, 0);//006C78E9
//	SetCompleteHook(0xE8, 0x006C7947, &COptionWindow::RenderFrame);//006C7947
//	MemorySet(0x006C7A3C, 0x90, 5);//006C7A3C
//	MemorySet(0x006C79B6, 0x90, 5);//006C79B6
//}