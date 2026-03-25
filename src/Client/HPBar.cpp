#include "StdAfx.h"
#include "HPBar.h"
#include "Other.h"
#include "TMemory.h"

bool MonsterHPBarMode = 1;

HPBar::HPBar() : HP()
{
}

HPBar::~HPBar()
{
}

void HPBar::Load()
{

}

void HPBar::Clear()
{
	this->HP.clear();
}

void HPBar::Insert(short Index, BYTE Percent)
{
	this->HP[Index] = Percent;
}

BYTE HPBar::GetPercent(short Index) const
{
	try
	{
		return this->HP.at(Index);
	}
	catch (...)
	{
		return 255;
	}
}

void HPBar::Draw()
{
	SelectObject(*(HDC*)(0x5877F38), *(HFONT*)(0x5877FA0));

	if(MonsterHPBarMode != 0)
	{
		return;
	}

	int X = 0, Y = 0, Width = 80;
	Angle Angle;
	SIZE Size;
	ViewportInfo* Viewport;
	BYTE Percent;

	for (int i = 0; i < 400; ++i)
	{
		Viewport = (ViewportInfo*)(*(DWORD*)(0x754F4BC) + sizeof(ViewportInfo) * i);

		if (!Viewport->Live)
		{
			continue;
		}

		if (Viewport->Type == 2)
		{
			Percent = gHPBar.GetPercent(Viewport->Index);

			if (Percent == (BYTE)(-1))
			{
				continue;
			}

			Angle.X = Viewport->Angle1.X;
			Angle.Y = Viewport->Angle1.Y;
			Angle.Z = Viewport->Angle1.Z + Viewport->Angle2.Z + 100.f;

			GetPosFromAngle(&Angle, &X, &Y);

			X -= (int)(floor(Width / 2.f));

			SetBlend(1);
			glColor4f(0.f, 0.f, 0.f, 0.5f);
			DrawBarForm((float)(X - 1), (float)(Y - 12), 2.f + (float)(Width), 17.f);

			glColor3f(0.9f, 0.f, 0.f);
			DrawBarForm((float)(X), 1.f + Y, (float)((Percent * Width) / 100.f), 3.f);
			GLSwitchBlend();

			GetTextExtentPoint(*(HDC*)(0x5877F38), Viewport->Name, strlen(Viewport->Name), &Size);
			Size.cx = 80 - (LONG)((float)(Size.cx) / *(float*)(0x587786C));

			if (Size.cx < 0)
			{
				Size.cx = 0;
			}
			else
			{
				Size.cx /= 2;
			}

			glColor4f(1.f, 1.f, 1.f, 1.f);

			X += Size.cx;
			Y -= 9;

			if ((X >= 0 && X <= (640 - (Size.cx / 2))) && (Y >= 0 && Y <= 480))
			{
				DrawTextOut(X, Y, Viewport->Name, 0, 0, 0);
			}
		}

		//if (Viewport->Type == 1)
		//{
		//	Percent = gHPBar.GetPercent(Viewport->Index);

		//	if (Percent == (BYTE)(-1))
		//	{
		//		continue;
		//	}

		//	Angle.X = Viewport->Angle1.X;
		//	Angle.Y = Viewport->Angle1.Y;
		//	Angle.Z = Viewport->Angle1.Z + Viewport->Angle2.Z + 100.f;

		//	GetPosFromAngle(&Angle, &X, &Y);

		//	X -= (int)(floor(Width / 2.f));

		//	SetBlend(1);
		//	glColor4f(0.f, 0.f, 0.f, 0.5f);
		//	DrawBarForm((float)(X - 1), (float)(Y - 6), 2.f + (float)(Width), 10.f);

		//	glColor3f(0.9f, 0.f, 0.f);
		//	DrawBarForm((float)(X), 1.f + Y, (float)((Percent * Width) / 100.f), 3.f);
		//	GLSwitchBlend();

		//	GetTextExtentPoint(*(HDC*)(0x5877F38), Viewport->Name, strlen(Viewport->Name), &Size);
		//	Size.cx = 80 - (LONG)((float)(Size.cx) / *(float*)(0x587786C));

		//	if (Size.cx < 0)
		//	{
		//		Size.cx = 0;
		//	}
		//	else
		//	{
		//		Size.cx /= 2;
		//	}

		//	glColor4f(1.f, 1.f, 1.f, 1.f);

		//	X += Size.cx;
		//	Y -= 9;

		//	/*if ((X >= 0 && X <= (640 - (Size.cx / 2))) && (Y >= 0 && Y <= 480))
		//	{
		//		DrawTextOut(X, Y, Viewport->Name, 0, 0, 0);
		//	}*/
		//}
	}

	GLSwitch();
	glColor3f(1.f, 1.f, 1.f);

	((void(*)())(0x602B50))();
}

HPBar gHPBar;