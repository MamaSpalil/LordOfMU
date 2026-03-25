#include "stdafx.h"
#include "Structure.h"
#include "ButtonChar.h"
#include "Interface.h"
#include "InterfaceEx.h"
#include "TMemory.h"



void DrawButtonsCharacters()
{
	//if (GameState == 4)
	//{
		pSetBlend(true);
		//glColor4f(245.0, 222.0, 179.0, 1.0);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		char szTemp[256];
		char szTemp1[256];
		char szTemp2[256];
		char szTemp3[256];
		char szTemp4[256];
		char szTemp5[256];
		char szTemp6[256];
		char szTemp7[256];
		char szTemp8[256];

		int PositionX = 250;
		int PositionY = 28;
		int Width = 20;
		int Height = 20;
		PrintImages(3594,PositionX,PositionY,Width,Height,0.0,0.0,0.52730,0.52730, 1, 1, 0.0);
		if(pCursorX >= PositionX
		&&	(float)pCursorX <= (float)PositionX + Width
		&&	pCursorY >= PositionY - 1 
		&&	pCursorY < PositionY + Height )
		{
			pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf_s (szTemp, " Dark Wizard is a powerful mage with a secret knowledge of ancient spells,");
			sprintf_s(szTemp1 ," which grant him incredible power. With the powers of magic, he can control the elements,");
			sprintf_s (szTemp2, " crushing your enemies, move through space and create a magic barrier around you.");
			sprintf_s (szTemp3, " Thanks to his powerful magic, he can crush characters and monsters,");
			sprintf_s (szTemp4, " inhabiting this magic-infused world, at great distances.");
			MU_DrawTextOut(PositionX-70,PositionY+Height+8,szTemp,0,0,0);
			MU_DrawTextOut(PositionX-70,PositionY+Height+8+10,szTemp1,0,0,0);
			MU_DrawTextOut(PositionX-70,PositionY+Height+8+20,szTemp2,0,0,0);
			MU_DrawTextOut(PositionX-70,PositionY+Height+8+30,szTemp3,0,0,0);
			MU_DrawTextOut(PositionX-70,PositionY+Height+8+40,szTemp4,0,0,0);
		}

		int PositionX1 = 280;

		PrintImages(3595,PositionX1,PositionY,Width,Height,0.0,0.0,0.52730,0.52730, 1, 1, 0.0);
		if(pCursorX >= PositionX1
		&&	(float)pCursorX <= (float)PositionX1 + Width
		&&	pCursorY >= PositionY - 1 
		&&	pCursorY < PositionY + Height )
		{
			pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf_s (szTemp, " Dark Knight is a dark knight, a true warrior, a fighter who has no equal in strength in close combat.");
			sprintf_s (szTemp1 ," He has a powerful weapon and is able to deal crushing blows to enemies,");
			sprintf_s (szTemp2 ," and his powerful armor allows him to survive even in the most difficult situations.");
			sprintf_s (szTemp3, "His blood boils in his veins, turning into fire, giving him great resilience in the face of the enemy.");
			MU_DrawTextOut(PositionX-90,PositionY+Height+8,szTemp,0,0,0);
			MU_DrawTextOut(PositionX-90,PositionY+Height+8+10,szTemp1,0,0,0);
			MU_DrawTextOut(PositionX-90,PositionY+Height+8+20,szTemp2,0,0,0);
			MU_DrawTextOut(PositionX-90,PositionY+Height+8+30,szTemp3,0,0,0);
		}


		int PositionX2 = 310;

		PrintImages(3596,PositionX2,PositionY,Width,Height,0.0,0.0,0.52730,0.52730, 1, 1, 0.0);
		if(pCursorX >= PositionX2
		&&	(float)pCursorX <= (float)PositionX2 + Width
		&&	pCursorY >= PositionY - 1 
		&&	pCursorY < PositionY + Height )
		{
			pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf_s (szTemp, " Fairy Elf is an elven warrior. Elves are skilled with bows and crossbows and are capable of striking ");
			sprintf_s(szTemp1, " your enemies at long distances. They are from birth inextricably linked with the world around them, in their blood ");
			sprintf_s (szTemp2, " the power of Mother earth flows, so in addition to their skillful handling of elven weapons, they are also famous for their ");
			sprintf_s (szTemp3, " magic that can direct the forces of nature to protect, attack, and heal themselves and others.");
			MU_DrawTextOut(PositionX-100,PositionY+Height+8,szTemp,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+10,szTemp1,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+20,szTemp2,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+30,szTemp3,0,0,0);
		}


		int PositionX3 = 340;

		PrintImages(3597,PositionX3,PositionY,Width,Height,0.0,0.0,0.52730,0.52730, 1, 1, 0.0);
		if(pCursorX >= PositionX3
		&&	(float)pCursorX <= (float)PositionX3 + Width
		&&	pCursorY >= PositionY - 1 
		&&	pCursorY < PositionY + Height )
		{
			pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf_s (szTemp, " Magic Gladiator is a versatile fighter who is equally proficient with both weapons and magic. By virtue of its");
			sprintf_s (szTemp1, " busy in combat training, gladiator doesn't have as much time to study secret scrolls as mages, so");
			sprintf_s (szTemp2, " he did not comprehend the full power of ancient spells, but he makes up for it with his combat skills,");
			sprintf_s (szTemp3, " stunning enemies with stunning blows, and using magic when necessary. Magic Gladiator is a rapidly");
			sprintf_s (szTemp4, " develops due to its versatility. Unlike others, he does not hide his face under a mask,");
			sprintf_s (szTemp5, " he prefers that enemies see the face of the one from whose sword they will fall.");
			MU_DrawTextOut(PositionX-100,PositionY+Height+8,szTemp,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+10,szTemp1,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+20,szTemp2,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+30,szTemp3,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+40,szTemp4,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+50,szTemp5,0,0,0);
		}

		int PositionX4 =370;

		PrintImages(3598,PositionX4,PositionY,Width,Height,0.0,0.0,0.52730,0.52730, 1, 1, 0.0);
		if(pCursorX >= PositionX4
		&&	(float)pCursorX <= (float)PositionX4 + Width
		&&	pCursorY >= PositionY - 1 
		&&	pCursorY < PositionY + Height )
		{
			pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf_s (szTemp, " Dark Lord - as befits the lord of the dark lands Mu Online, he owns a horse and a black raven, which are true");
			sprintf_s(szTemp1, " serve him and help him in all his battles. The Dark Lord has incredible power, which he can surpass ");
			sprintf_s (szTemp2, " except for the dark knight, but unlike the latter, the lord can carry out his deadly attack");
			sprintf_s(szTemp3, " remotely, without approaching the enemy. In each of his attacks, he invests what the master of the underworld has given him");
			sprintf_s (szTemp4, " hellfire, and fueled by rage and anger, his attacks become even stronger. It is developing rapidly ");
			sprintf_s (szTemp5, " due to its noble origin and natural talents.");
			MU_DrawTextOut(PositionX-100,PositionY+Height+8,szTemp,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+10,szTemp1,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+20,szTemp2,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+30,szTemp3,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+40,szTemp4,0,0,0);
			MU_DrawTextOut(PositionX-100,PositionY+Height+8+50,szTemp5,0,0,0);
		}

		glColor3f(1.0, 0.0, 0.0); //Red
		pDrawTextBackground = eDrawText::TextBackground::Invisible;
		sprintf_s(szTemp6, " Initially, only 3 heroes will be available to you. ");
		sprintf_s(szTemp7, "Magic Gladiator will be available to you at level 220.");
		sprintf_s(szTemp8, " And finally, at level 250, you can create Dark Lord.");
		MU_DrawTextOut(245,0,szTemp6,0,0,0);
		MU_DrawTextOut(240,7,szTemp7,0,0,0);
		MU_DrawTextOut(230,14,szTemp8,0,0,0);

		glColor4f(1.0, 1.0, 1.0, 1.0);
		MU_ADDSimple();
	//}
}