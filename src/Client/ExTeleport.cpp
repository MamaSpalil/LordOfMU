#include "stdafx.h"
#include "ExTeleport.h"
#include "Interface.h"
#include "InterfaceEx.h"
#include "Other.h"
#include "Structure.h"
#include "Controller.h"
#include "WndProc.h"

void TeleportList()
{
	if (pCheckWindow(0x1B)) //inventory
	{
		// keybd_event('V', 0,0,0);
			//keybd_event('V', 0,KEYEVENTF_KEYUP,0);
		//system("PAUSE");
		DrawTextOut(0, 105, "Close, Inventory first (V)",0,0,0);
		return;
		//return;
	}
		/*Seting = false;
		Menu = false;
		RunningBank = false;
		QuestDialog = false;*/
		pSetBlend(true);
		glColor3f(1.0f, 1.0f, 1.0f);//Green
		PrintImages(3674,120,90,400,300,0.0,0.0,0.998125,0.768125, 1, 1, 0.0);//map

		char Location[20][255];
		char Lvl[20][255];
		char Zen[20][255];
		char ResMin[20][255];
		char ResMax[20][255];
		

	

		PrintImages(3677,270,223,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal Arena
		PrintImages(3677,290,225,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal Arena Medium
		PrintImages(3677,296,205,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal Arena Hard
		//
		PrintImages(3677,310,176,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal Lorencia 317 182
		//
		PrintImages(3677,365,208,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//Noria
		//
		PrintImages(3677,275,163,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal devias 
		PrintImages(3677,280,151,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal devias 3
		PrintImages(3677,260,148,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal devias castle
		PrintImages(3677,305,145,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal devias 4
		//
		PrintImages(3677,342,178,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//Dungeon 1
		PrintImages(3677,343,188,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//Dungeon 2
		PrintImages(3677,356,183,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//Dungeon 3
		//
		PrintImages(3677,399,180,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//Atlans1
		PrintImages(3677,411,198,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//Atlans2
		PrintImages(3677,428,191,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//Atlans3
		//
		PrintImages(3677,298,135,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//lt1
		PrintImages(3677,275,134,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//lt4
		PrintImages(3677,256,132,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//lt5
		PrintImages(3677,247,127,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//lt6
		PrintImages(3677,270,122,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//lt7
		//
		PrintImages(3677,442,174,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//tarkan
		PrintImages(3677,451,155,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//tarkan
		//
		PrintImages(3677,268,112,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//icarus
		//
		PrintImages(3677,267,196,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal Loren
		//
		PrintImages(3677,365,236,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal aida 1
		PrintImages(3677,385,238,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal aida 2
		//
		PrintImages(3677,419,140,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal kanturu
		PrintImages(3677,451,222,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal kanturu 2
		PrintImages(3677,456,260,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal kanturu 3
		//
		PrintImages(3677,312,217,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal crywolf
		PrintImages(3677,281,252,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal barraks
		PrintImages(3677,297,242,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal refuge
		

		if(pCursorX >= 310//lorencia
		&&	(float)pCursorX <= (float)310 + 10
		&&	pCursorY >= 176 - 1 
		&&	pCursorY < 176 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Lorencia");
			sprintf(Lvl[0], "Need level: 25");
			sprintf(Zen[0], "Need Zen: 50000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 365//noria
		&&	(float)pCursorX <= (float)365 + 10
		&&	pCursorY >= 208 - 1 
		&&	pCursorY < 208 + 10 )
		{
			/*pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
			pDrawBarForm(pCursorX-40,pCursorY-40, 80, 30);
			glEnd();
			pSetBlend(true);
			pDrawTextBackground = eDrawText::TextBackground::Black;*/
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Noria");
			sprintf(Lvl[0], "Need level: 25");
			sprintf(Zen[0], "Need Zen: 50000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 275//devias
		&&	(float)pCursorX <= (float)275 + 10
		&&	pCursorY >= 163 - 1 
		&&	pCursorY < 163 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Devias");
			sprintf(Lvl[0], "Need level: 50");
			sprintf(Zen[0], "Need Zen: 75000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 260//devias castle
		&&	(float)pCursorX <= (float)260 + 10
		&&	pCursorY >= 148 - 1 
		&&	pCursorY < 148 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Devias Castle");
			sprintf(Lvl[0], "Need level: 200");
			sprintf(Zen[0], "Need Zen: 2000000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 280//devias 3
		&&	(float)pCursorX <= (float)280 + 10
		&&	pCursorY >= 151 - 1 
		&&	pCursorY < 151 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Devias 3");
			sprintf(Lvl[0], "Need level: 60");
			sprintf(Zen[0], "Need Zen: 150000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 305//devias 4
		&&	(float)pCursorX <= (float)305 + 10
		&&	pCursorY >= 145 - 1 
		&&	pCursorY < 145 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Devias 4");
			sprintf(Lvl[0], "Need level: 70");
			sprintf(Zen[0], "Need Zen: 200000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 270
		&&	(float)pCursorX <= (float)270 + 10
		&&	pCursorY >= 223 - 1 
		&&	pCursorY < 223 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Arena");
			sprintf(Lvl[0], "Need level: 200");
			sprintf(Zen[0], "Need Zen: 2000000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 10");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 290
		&&	(float)pCursorX <= (float)290 + 10
		&&	pCursorY >= 225 - 1 
		&&	pCursorY < 225 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Arena Medium");
			sprintf(Lvl[0], "Need level: 200");
			sprintf(Zen[0], "Need Zen: 2500000");
			sprintf(ResMin[0], "Reset Min: 5");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 296
		&&	(float)pCursorX <= (float)296 + 10
		&&	pCursorY >= 205 - 1 
		&&	pCursorY < 205 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Arena Hard");
			sprintf(Lvl[0], "Need level: 200");
			sprintf(Zen[0], "Need Zen: 3500000");
			sprintf(ResMin[0], "Reset Min: 15");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 342
		&&	(float)pCursorX <= (float)342 + 10
		&&	pCursorY >= 178 - 1 
		&&	pCursorY < 178 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Dungeon 1");
			sprintf(Lvl[0], "Need level: 60");
			sprintf(Zen[0], "Need Zen: 100000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 343
		&&	(float)pCursorX <= (float)343 + 10
		&&	pCursorY >= 188 - 1 
		&&	pCursorY < 188 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Dungeon 2");
			sprintf(Lvl[0], "Need level: 70");
			sprintf(Zen[0], "Need Zen: 150000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 356
		&&	(float)pCursorX <= (float)356 + 10
		&&	pCursorY >= 183 - 1 
		&&	pCursorY < 183 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Dungeon 3");
			sprintf(Lvl[0], "Need level: 80");
			sprintf(Zen[0], "Need Zen: 200000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 399
		&&	(float)pCursorX <= (float)399 + 10
		&&	pCursorY >= 180 - 1 
		&&	pCursorY < 180 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Atlans");
			sprintf(Lvl[0], "Need level: 70");
			sprintf(Zen[0], "Need Zen: 150000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 411
		&&	(float)pCursorX <= (float)411 + 10
		&&	pCursorY >= 198 - 1 
		&&	pCursorY < 198 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Atlans 2");
			sprintf(Lvl[0], "Need level: 90");
			sprintf(Zen[0], "Need Zen: 200000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 428
		&&	(float)pCursorX <= (float)428 + 10
		&&	pCursorY >= 191 - 1 
		&&	pCursorY < 191 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Atlans 3");
			sprintf(Lvl[0], "Need level: 110");
			sprintf(Zen[0], "Need Zen: 300000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 298
		&&	(float)pCursorX <= (float)298 + 10
		&&	pCursorY >= 135 - 1 
		&&	pCursorY < 135 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Lost Tower");
			sprintf(Lvl[0], "Need level: 90");
			sprintf(Zen[0], "Need Zen: 250000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 275
		&&	(float)pCursorX <= (float)275 + 10
		&&	pCursorY >= 134 - 1 
		&&	pCursorY < 134 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Lost Tower 3");
			sprintf(Lvl[0], "Need level: 100");
			sprintf(Zen[0], "Need Zen: 400000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 256
		&&	(float)pCursorX <= (float)256 + 10
		&&	pCursorY >= 132 - 1 
		&&	pCursorY < 132 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Lost Tower 5");
			sprintf(Lvl[0], "Need level: 110");
			sprintf(Zen[0], "Need Zen: 550000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 247
		&&	(float)pCursorX <= (float)247 + 10
		&&	pCursorY >= 127 - 1 
		&&	pCursorY < 127 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Lost Tower 6");
			sprintf(Lvl[0], "Need level: 115");
			sprintf(Zen[0], "Need Zen: 650000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 270
		&&	(float)pCursorX <= (float)270 + 10
		&&	pCursorY >= 122 - 1 
		&&	pCursorY < 122 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Lost Tower 7");
			sprintf(Lvl[0], "Need level: 120");
			sprintf(Zen[0], "Need Zen: 750000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 442
		&&	(float)pCursorX <= (float)442 + 10
		&&	pCursorY >= 174 - 1 
		&&	pCursorY < 174 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Tarkan");
			sprintf(Lvl[0], "Need level: 140");
			sprintf(Zen[0], "Need Zen: 1000000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		if(pCursorX >= 451
		&&	(float)pCursorX <= (float)451 + 10
		&&	pCursorY >= 155 - 1 
		&&	pCursorY < 155 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Tarkan 2");
			sprintf(Lvl[0], "Need level: 150");
			sprintf(Zen[0], "Need Zen: 1250000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		////
		//PrintImages(3677,268,112,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//icarus
		if(pCursorX >= 268
		&&	(float)pCursorX <= (float)268 + 10
		&&	pCursorY >= 112 - 1 
		&&	pCursorY < 112 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Icarus");
			sprintf(Lvl[0], "Need level: 170");
			sprintf(Zen[0], "Need Zen: 1500000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		////
		//PrintImages(3677,267,196,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal Loren
		if(pCursorX >= 267
		&&	(float)pCursorX <= (float)267 + 10
		&&	pCursorY >= 196 - 1 
		&&	pCursorY < 196 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Valley of Loren");
			sprintf(Lvl[0], "Need level: 100");
			sprintf(Zen[0], "Need Zen: 100000");
			sprintf(ResMin[0], "Reset Min: 0");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		////
		//PrintImages(3677,365,236,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal aida 1
		if(pCursorX >= 365
		&&	(float)pCursorX <= (float)365 + 10
		&&	pCursorY >= 236 - 1 
		&&	pCursorY < 236 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Aida");
			sprintf(Lvl[0], "Need level: 230");
			sprintf(Zen[0], "Need Zen: 1500000");
			sprintf(ResMin[0], "Reset Min: 2");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		//PrintImages(3677,385,238,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal aida 2
		if(pCursorX >= 385
		&&	(float)pCursorX <= (float)385 + 10
		&&	pCursorY >= 238 - 1 
		&&	pCursorY < 238 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Aida 2");
			sprintf(Lvl[0], "Need level: 250");
			sprintf(Zen[0], "Need Zen: 1750000");
			sprintf(ResMin[0], "Reset Min: 2");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		//PrintImages(3677,419,140,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal kanturu
		if(pCursorX >= 419
		&&	(float)pCursorX <= (float)419 + 10
		&&	pCursorY >= 140 - 1 
		&&	pCursorY < 140 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Kanturu");
			sprintf(Lvl[0], "Need level: 260");
			sprintf(Zen[0], "Need Zen: 3000000");
			sprintf(ResMin[0], "Reset Min: 5");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		//PrintImages(3677,451,222,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal kanturu 2
		if(pCursorX >= 451
		&&	(float)pCursorX <= (float)451 + 10
		&&	pCursorY >= 222 - 1 
		&&	pCursorY < 222 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Kanturu 2");
			sprintf(Lvl[0], "Need level: 270");
			sprintf(Zen[0], "Need Zen: 3500000");
			sprintf(ResMin[0], "Reset Min: 6");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		//PrintImages(3677,456,260,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal kanturu 3
		if(pCursorX >= 456
		&&	(float)pCursorX <= (float)456 + 10
		&&	pCursorY >= 260 - 1 
		&&	pCursorY < 260 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Kanturu 3");
			sprintf(Lvl[0], "Need level: 290");
			sprintf(Zen[0], "Need Zen: 3800000");
			sprintf(ResMin[0], "Reset Min: 10");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		////
		//PrintImages(3677,312,217,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal crywolf
		if(pCursorX >= 312
		&&	(float)pCursorX <= (float)312 + 10
		&&	pCursorY >= 217 - 1 
		&&	pCursorY < 217 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Cry Wolf");
			sprintf(Lvl[0], "Need level: 300");
			sprintf(Zen[0], "Need Zen: 3500000");
			sprintf(ResMin[0], "Reset Min: 5");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		//PrintImages(3677,281,252,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal barraks
		if(pCursorX >= 281
		&&	(float)pCursorX <= (float)281 + 10
		&&	pCursorY >= 252 - 1 
		&&	pCursorY < 252 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Barracks");
			sprintf(Lvl[0], "Need level: 330");
			sprintf(Zen[0], "Need Zen: 4000000");
			sprintf(ResMin[0], "Reset Min: 10");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		//PrintImages(3677,297,242,10,10,0.0,0.0,0.930, 0.930, 1, 1, 0.0);//portal refuge
		if(pCursorX >= 297
		&&	(float)pCursorX <= (float)297 + 10
		&&	pCursorY >= 242 - 1 
		&&	pCursorY < 242 + 10 )
		{
			pSetBlend(true);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			pDrawBarForm(pCursorX+9,pCursorY-40, 102, 50);
			glEnd();
			pSetBlend(true);
			//pDrawTextBackground = eDrawText::TextBackground::Black;
			sprintf(Location[0], "Refuge");
			sprintf(Lvl[0], "Need level: 350");
			sprintf(Zen[0], "Need Zen: Refuge");
			sprintf(ResMin[0], "Reset Min: 15");
			sprintf(ResMax[0], "Reset Max: 100");

			glColor3f(1.0, 1.0, 0.0);
			MU_DrawTextOut(pCursorX+10,pCursorY-40,Location[0],GetRealWidth(100), 1, 0);
			glColor3f(1.0f, 1.0f, 1.0f);//Green
			MU_DrawTextOut(pCursorX+10,pCursorY-30,Lvl[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-20,Zen[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-10,ResMin[0],GetRealWidth(100), 1, 0);
			MU_DrawTextOut(pCursorX+10,pCursorY-0,ResMax[0],GetRealWidth(100), 1, 0);
			pSetBlend(true);
		}
		glColor3f(1.0f, 1.0f, 1.0f);//Green
		//pGLSwitchBlend();
		pGLSwitch();
		glEnd();
}
void TPList(int number)
{
	PMSG_TPLIST pMsg;
	pMsg.h.set(0xFB, 0xDF, sizeof(pMsg));
	pMsg.Result = number;
	DataSend((LPBYTE)&pMsg, pMsg.h.size);
}
void MouseClickTP()
{
	int Width = 25;
	int Height = 25;
	int PositionX1 = 248;
	int PositionY1 = 80;
	if (TpList == true)
	{
		//test
		if(pCursorX >= 270
		&&	(float)pCursorX <= 270 + 15
		&&	pCursorY >= 223 - 1 
		&&	pCursorY < 223 + 15 )
		{
			TPList(1);
			TpList = false;
		}
		if(pCursorX >= 290
		&&	(float)pCursorX <= 290 + 15
		&&	pCursorY >= 225 - 1 
		&&	pCursorY < 225 + 15 )
		{
			TPList(2);
			TpList = false;
		}
		if(pCursorX >= 296
		&&	(float)pCursorX <= 296 + 15
		&&	pCursorY >= 205 - 1 
		&&	pCursorY < 205 + 15 )
		{
			TPList(3);
			TpList = false;
		}
		if(pCursorX >= 310
		&&	(float)pCursorX <= 310 + 15
		&&	pCursorY >= 176 - 1 
		&&	pCursorY < 176 + 15 )
		{
			TPList(4);
			TpList = false;
		}
		
		if(pCursorX >= 365
		&&	(float)pCursorX <= 365 + 15
		&&	pCursorY >= 208 - 1 
		&&	pCursorY < 208 + 15 )
		{
			TPList(5);//noria
			TpList = false;
		}
		if(pCursorX >= 275
		&&	(float)pCursorX <= 275 + 15
		&&	pCursorY >= 163 - 1 
		&&	pCursorY < 163 + 15 )
		{
			TPList(6);
			TpList = false;
		}
		if(pCursorX >= 280
		&&	(float)pCursorX <= 280 + 15
		&&	pCursorY >= 151 - 1 
		&&	pCursorY < 151 + 15 )
		{
			TPList(8);
			TpList = false;
		}
		if(pCursorX >= 260
		&&	(float)pCursorX <= 260 + 15
		&&	pCursorY >= 148 - 1 
		&&	pCursorY < 148 + 15 )
		{
			TPList(7);
			TpList = false;
		}
		if(pCursorX >= 305
		&&	(float)pCursorX <= 305 + 15
		&&	pCursorY >= 145 - 1 
		&&	pCursorY < 145 + 15 )
		{
			TPList(9);
			TpList = false;
		}
		if(pCursorX >= 342
		&&	(float)pCursorX <= 342 + 15
		&&	pCursorY >= 178 - 1 
		&&	pCursorY < 178 + 15 )
		{
			TPList(10);
			TpList = false;
		}
		if(pCursorX >= 343
		&&	(float)pCursorX <= 343 + 15
		&&	pCursorY >= 188 - 1 
		&&	pCursorY < 188 + 15 )
		{
			TPList(11);
			TpList = false;
		}
		if(pCursorX >= 356
		&&	(float)pCursorX <= 356 + 15
		&&	pCursorY >= 183 - 1 
		&&	pCursorY < 183 + 15 )
		{
			TPList(12);//dungeon3
			TpList = false;
		}
		if(pCursorX >= 399
		&&	(float)pCursorX <= 399 + 15
		&&	pCursorY >= 180 - 1 
		&&	pCursorY < 180 + 15 )
		{
			TPList(13);
			TpList = false;
		}
		if(pCursorX >= 411
		&&	(float)pCursorX <= 411 + 15
		&&	pCursorY >= 198 - 1 
		&&	pCursorY < 198 + 15 )
		{
			TPList(14);
			TpList = false;
		}
		if(pCursorX >= 428
		&&	(float)pCursorX <= 428 + 15
		&&	pCursorY >= 191 - 1 
		&&	pCursorY < 191 + 15 )
		{
			TPList(15);//atlans3
			TpList = false;
		}
		if(pCursorX >= 298
		&&	(float)pCursorX <= 298 + 15
		&&	pCursorY >= 135 - 1 
		&&	pCursorY < 135 + 15 )
		{
			TPList(16);
			TpList = false;
		}
		if(pCursorX >= 275
		&&	(float)pCursorX <= 275 + 15
		&&	pCursorY >= 134 - 1 
		&&	pCursorY < 134 + 15 )
		{
			TPList(17);
			TpList = false;
		}
		if(pCursorX >= 256
		&&	(float)pCursorX <= 256 + 15
		&&	pCursorY >= 132 - 1 
		&&	pCursorY < 132 + 15 )
		{
			TPList(18);
			TpList = false;
		}
		if(pCursorX >= 247
		&&	(float)pCursorX <= 247 + 15
		&&	pCursorY >= 127 - 1 
		&&	pCursorY < 127 + 15 )
		{
			TPList(19);
			TpList = false;
		}
		if(pCursorX >= 270
		&&	(float)pCursorX <= 270 + 15
		&&	pCursorY >= 122 - 1 
		&&	pCursorY < 122 + 15 )
		{
			TPList(20);//lt7
			TpList = false;
		}
		if(pCursorX >= 442
		&&	(float)pCursorX <= 442 + 15
		&&	pCursorY >= 174 - 1 
		&&	pCursorY < 174 + 15 )
		{
			TPList(21);
			TpList = false;
		}
		if(pCursorX >= 451
		&&	(float)pCursorX <= 451 + 15
		&&	pCursorY >= 155 - 1 
		&&	pCursorY < 155 + 15 )
		{
			TPList(22);
			TpList = false;
		}
		if(pCursorX >= 268
		&&	(float)pCursorX <= 268 + 15
		&&	pCursorY >= 112 - 1 
		&&	pCursorY < 112 + 15 )
		{
			TPList(23);
			TpList = false;
		}
		if(pCursorX >= 267
		&&	(float)pCursorX <= 267 + 15
		&&	pCursorY >= 196 - 1 
		&&	pCursorY < 196 + 15 )
		{
			TPList(24);//valleyofloren
			TpList = false;
		}
		if(pCursorX >= 365
		&&	(float)pCursorX <= 365 + 15
		&&	pCursorY >= 236 - 1 
		&&	pCursorY < 236 + 15 )
		{
			TPList(25);
			TpList = false;
		}
		if(pCursorX >= 385
		&&	(float)pCursorX <= 385 + 15
		&&	pCursorY >= 238 - 1 
		&&	pCursorY < 238 + 15 )
		{
			TPList(26);//aida2
			TpList = false;
		}
		if(pCursorX >= 419
		&&	(float)pCursorX <= 419 + 15
		&&	pCursorY >= 140 - 1 
		&&	pCursorY < 140 + 15 )
		{
			TPList(27);
			TpList = false;
		}
		if(pCursorX >= 451
		&&	(float)pCursorX <= 451 + 15
		&&	pCursorY >= 222 - 1 
		&&	pCursorY < 222 + 15 )
		{
			TPList(28);
			TpList = false;
		}
		if(pCursorX >= 456
		&&	(float)pCursorX <= 456 + 15
		&&	pCursorY >= 260 - 1 
		&&	pCursorY < 260 + 15 )
		{
			TPList(29);//kanturu3
			TpList = false;
		}
		if(pCursorX >= 312
		&&	(float)pCursorX <= 312 + 15
		&&	pCursorY >= 217 - 1 
		&&	pCursorY < 217 + 15 )
		{
			TPList(30);
			TpList = false;
		}
		if(pCursorX >= 281
		&&	(float)pCursorX <= 281 + 15
		&&	pCursorY >= 252 - 1 
		&&	pCursorY < 252 + 15 )
		{
			TPList(31);
			TpList = false;
		}
		if(pCursorX >= 297
		&&	(float)pCursorX <= 297 + 15
		&&	pCursorY >= 242 - 1 
		&&	pCursorY < 242 + 15 )
		{
			TPList(32);
			TpList = false;
		}
	}
}