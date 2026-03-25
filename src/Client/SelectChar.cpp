#include "stdafx.h"
#include "SelectChar.h"
#include "Structure.h"
#include "TMemory.h"
#include "Other.h"
#include "ButtonChar.h"
#include "ObjectManager.h"

bool SEASON3B_CheckMouseIn(int a1, int a2, int a3, int a4)
{
  return MouseXs >= a1 && MouseXs < a1 + a3 && MouseYs >= a2 && MouseYs < a4 + a2;
}

void CSetInfo(float X, float Y, int Class)
{
	int cHero = *(DWORD *)((sub_463272() + 36584) + 4) + 368 * Class;
	int m_szName = cHero + 176;
	int m_szClass = sub_532400(*(BYTE *)(*(DWORD *)(cHero + 168) + 524));
	char m_szLevel[256];
	char m_szReset[256];
	wsprintf(m_szLevel, "Lvl %d", *(WORD *)(*(DWORD *)(cHero + 168) + 526));
	//wsprintf(m_szReset, "Gr %d|",gObjectManager->GetResets());
	int m_szGuild = cHero + 240;

	pSetBlend(true);
	pDrawTextColor = eDrawText::TextColor::Orange;
	pDrawTextBackground = eDrawText::TextBackground::Invisible;
	MU_DrawTextOut(X + 85, Y + 17, (char *)m_szName, 100, 0, 0);

	MU_DrawTextOut(X + 85, Y + 7, (char *)m_szClass, 100, 0, 0);

	MU_DrawTextOut(X +10 , Y + 17, (char *)m_szLevel, 100, 0, 0);
	//MU_DrawTextOut(X +40 , Y + 20, (char *)m_szReset, 100, 0, 0);
	MU_DrawTextOut(X +10, Y + 7, (char *)m_szGuild, 100,  0, 0);
	pGLSwitch();

}

void CharacterTeleport(int Index, float X, float Y, float Scale, float Rotate)
{
	int Model;
	for(int i = 0 ; i < MAX_CHARACTER ; i++)
	{
		Model = 1156 * i + *(int*)0x754F4BC;//Model = 1156 * i + *(int*)0x754F4BC;
		if(Model)
		{
			if(Index == i)
			{
				*(float *)(Model + 16) = X;
				*(float *)(Model + 20) = Y;
				*(float *)(Model + 36) = Rotate;
				*(float *)(Model + 12) = Scale;
			}
			else
			{
				*(float *)(Model + 16) = 0;
				*(float *)(Model + 20) = 0;
				*(float *)(Model + 36) = 0;
				*(float *)(Model + 12) = Scale;
			}
		}
	}
}

void CharacterSelect360()
{
	int Model;
	for (int i = 0; i < MAX_CHARACTER; i++)
	{
		Model = 1156 * i + *(int*)0x754F4BC;
		if(SelectedCharacter == i)
		{	
			*(float *)(Model + 36) = *(float *)(Model + 36) + 5.0;  //AQUI É A VELOCIDADE DO GIRO DA 360
		}
		else
		{
			*(float *)(Model + 36) = 50;
		}
	}
}


//INFO_DATANEWS n_info;

void ButtonCharacter()
{
	DrawButtonsCharacters();
	CharacterSelect360();
	BeginBitmap();
	glColor3f(1.0, 1.0, 1.0);





	float X = 471, Y = 51;

	if ( !*(BYTE *)(Instance() + 34244) )
	{
		for ( int i = 0; i < MAX_CHARACTER; ++i )
		{

			//Textura de los Botones
			RenderBitmap(240, X, 37*i+Y, 150, 35, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			if (SEASON3B_CheckMouseIn(X, 37*i+Y, 150, 35) || SelectedHero == i)
			{
				glColor3f(0.8, 0.6, 0.4);
				EnableAlphaBlend();
				RenderBitmap(240, X, 37*i+Y, 150, 35, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
				glColor3f(1.0, 1.0, 1.0);
				DisableAlphaBlend();
			}
			//
			//Informacion de los Botones
			if ( *(BYTE *)(*(int*)0x754F4BC + 1156 * i) )
			{	
				if (SEASON3B_CheckMouseIn(X, 37*i+Y, 150, 35))
				{
					SelectedCharacter = i;
					{
						if (PressKey(1))
						{
							PlayBuffer(25, 0, 0);
							SelectedHero = i;
							CharacterTeleport(i,12974,15425,1.5,50);
						}
					}
				}
				CSetInfo(X,37*i+Y,i);
			}
			else
			{
				if (SEASON3B_CheckMouseIn(X, 37*i+Y, 150, 35) && PressKey(1))
				{
					PlayBuffer(25, 0, 0);
					ShowWin(Instance(), Instance() + 34232);
				}	
				pSetBlend(true);
				pDrawTextColor = eDrawText::TextColor::Red;
				pDrawTextBackground = eDrawText::TextBackground::Invisible;
				MU_DrawTextOut(X-29, 37*i+(Y+13), "Empty Character Slot", 210, 3, 0);
				pGLSwitch();
			}
			//
		}
	}

	//NEWS
	
	//INFO_DATANEWS * res = &n_info;
	//char szTemp1[256];
	//sprintf(szTemp1 ,"%s",res->News1);
	//pDrawTextOut((int)30, (int)55, (char*)szTemp1, 0, 0, 0);
	//res->News1[50];

	/*SetBlend(1);
	glColor4f(0.f, 0.f, 0.f, 0.8f);
	DrawBarForm((float)(3), (float)(30), 150.f + (float)(0), 300.f);
	GLSwitchBlend();
	glColor4f(1.f, 1.f, 1.f, 1.f);*/
	DrawButtonsCharacters();
}
//void NewsRecv(INFO_NEWS * aRecv)
//{
//	n_info.Result = aRecv->Result;
//	n_info.News1[50] = aRecv-> News1[50];
//	n_info.News2[50] = aRecv-> News2[50];
//	n_info.News3[50] = aRecv-> News3[50];
//	n_info.News4[50] = aRecv-> News4[50];
//	n_info.News5[50] = aRecv-> News5[50];
//	n_info.News6[50] = aRecv-> News6[50];
//	n_info.News7[50] = aRecv-> News7[50];
//	n_info.News8[50] = aRecv-> News8[50];
//	n_info.News9[50] = aRecv-> News9[50];
//	n_info.News10[50] = aRecv-> News10[50];
//}
void CSelectedHero()
{
	((int(*)())0x006F52A1)();//6F52A1
	for ( int i = 0; i < MAX_CHARACTER; ++i )
	{
		if ( *(BYTE *)(*(int*)0x754F4BC + 1156 * i) )
		{
			SelectedCharacter = i;
			SelectedHero = i;
			CharacterTeleport(i,12974,15425,1.5,50);
			break;
		}
	}
}

void CCreateCharacterPointer(int a1, int a2, char a3, int a4, int a5, int a6)
{
	((int(__cdecl*)(int a1, int a2, char a3, int a4, int a5, int a6))0x00544C71)(a1,a2,a3,0,0,0);//544C71
}



void CButtonCharacterS13()
{
	SetCompleteHook(0xE8,0x006CE89B, &ButtonCharacter);//6CE89B
	SetCompleteHook(0xE8,0x006CE147, &CSelectedHero);//6CE147
	SetCompleteHook(0xE8,0x004E2FE8, &CCreateCharacterPointer);//4E2FE8
	SetByte(0x006D7665+6, 0);//Hide character_ex.ozt //6D7665
	SetDword(0x0045FD6F+3, 0x3fc00000);//Scale Pet//45FD6F
	//CharacterSelect360();
}

