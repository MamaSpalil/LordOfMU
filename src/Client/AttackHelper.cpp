#include "stdafx.h"
#include "AttackHelper.h"
#include "WndProc.h"
#include "Interface.h"
#include "InterfaceEx.h"
#include "Other.h"
#include "User.h"

CAttackHelper * gAttackHelper = new CAttackHelper;
BOOL EnableHelperAutoPause;
void CAttackHelper::Init()
{
	this->SetState(ATTACKHELPER_STATE_Stop);
	this->m_TargetAttack = -1;
}

void CAttackHelper::SetState(int State)
{
	this->State = State;

	switch (this->State)
	{
		case ATTACKHELPER_STATE_Running:
			{
				this->HUDParam_Init();

				RECT rc = { 0 };
				GetClientRect(MuWindow, &rc);
				this->ProcMouseStartPoint.x = rc.right / 2;
				this->ProcMouseStartPoint.y = rc.bottom / 2;
			}
			break;
		default:
			{
				//-----------------------
				// --  Stop Attack Skill
				//----------------------
				//1.02.11
				//*(DWORD*)0X05878034 = 0;//0X05878034
				//*(BYTE*)0X07EB9678 = 0;//0X07EB9678
				//if (*(BYTE*)0X07EB9644)//0X07EB9644
				//*(BYTE*)0X07EB9628 = 1;//0X07EB9628
				//*(BYTE*)0X07EB9644 = 0;//0X07EB9644
				//1.02.19
				*(DWORD*)0x05877FC4 = 0;
				*(BYTE*)0x07EB3848 = 0;
				if (*(BYTE*)0x07EB3814)
				*(BYTE*)0x7EB37F8 = 1;
				*(BYTE*)0x7EB3814 = 0;
			}
			break;
	}
}

void CAttackHelper::MainProc()
{
	if (MAIN_STATEEx != 5)
	{
		return;
	}

	if (this->State != ATTACKHELPER_STATE_Running)
	{
		return;
	}

	if(MAIN_MAPNUMBER == 30)
	{
		this->SetState(ATTACKHELPER_STATE_Stop);
		return;
	}
	
	if(EnableHelperAutoPause == 0)
	{
		int test = MAIN_WIDTH / 2.8;
		pDrawToolTipEx(test,30, (char*)"   [ - PRESS F8 TO DISABLE BOT HELPER - ]   ");
	}
	
	if (this->AttackType == 1 || this->AttackType == 2)
	{
		if (EnableHelperAutoPause == 0 || this->HUDParam_CheckInterval() == true)
		{
			if (this->AttackType == 1)
			{
				this->ProcMousePoint.x = (int)((float)this->HUDParam_X + (float)this->HUDParam_Width*this->HUDParam_GetRadius()*cosf(this->HUDParam_CurrentAngle));
				this->ProcMousePoint.y = (int)((float)this->HUDParam_Y + (float)this->HUDParam_Height*this->HUDParam_GetRadius()*sinf(this->HUDParam_CurrentAngle));

				LPARAM lParam = (LPARAM)MAKELONG(this->ProcMousePoint.x, this->ProcMousePoint.y);

				MAIN_CURSORX = MAIN_WIDTH * (unsigned int)(unsigned __int16)lParam / MAIN_RESOLUTION_WIDTH;
				MAIN_CURSORY = MAIN_HEIGHT * (lParam >> 16) / MAIN_RESOLUTION_HEIGHT;

				if (MAIN_CURSORX < 0)
					MAIN_CURSORX = 0;
				if (MAIN_CURSORX > MAIN_WIDTH)
					MAIN_CURSORX = MAIN_WIDTH;
				if (MAIN_CURSORY < 0)
					MAIN_CURSORY = 0;
				if (MAIN_CURSORY > MAIN_HEIGHT)
					MAIN_CURSORY = MAIN_HEIGHT;

				this->HUDParam_CurrentAngle += this->HUDParam_GetRotateStep();

				if (fabs(this->HUDParam_CurrentAngle - this->HUDParam_GetPI2()) < 0.01)
				{
					this->HUDParam_CurrentAngle = 0.0f;
				}
			}
			else
			{
				this->ProcMousePoint = this->ProcMouseStartPoint;

				LPARAM lParam = (LPARAM)MAKELONG(this->ProcMousePoint.x, this->ProcMousePoint.y);
				MAIN_CURSORX = MAIN_WIDTH * (unsigned int)(unsigned __int16)lParam / MAIN_RESOLUTION_WIDTH;
				MAIN_CURSORY = MAIN_HEIGHT * (lParam >> 16) / MAIN_RESOLUTION_HEIGHT;
				
				if (MAIN_CURSORX < 0)
					MAIN_CURSORX = 0;
				if (MAIN_CURSORX > MAIN_WIDTH)
					MAIN_CURSORX = MAIN_WIDTH;
				if (MAIN_CURSORY < 0)
					MAIN_CURSORY = 0;
				if (MAIN_CURSORY > MAIN_HEIGHT)
					MAIN_CURSORY = MAIN_HEIGHT;
			}
		}
	}
	else
	{
		if (this->m_TargetAttack >= 0 && this->m_TargetAttack < 400)
		{

			DWORD ObjViewPortStruct = *(DWORD*)(PREVIEW_STRUCT)+(this->m_TargetAttack * PREVIEW_STRUCT_SIZE);

			if (*(BYTE*)ObjViewPortStruct == 0)
			{
				MAIN_ATTACK_TARGET = -1;
				this->m_TargetAttack = -1;
			}

			if (*(BYTE*)(ObjViewPortStruct + 0x1A0) == 0)
			{
				MAIN_ATTACK_TARGET = -1;
				this->m_TargetAttack = -1;
			}

			WORD index = *(WORD*)(ObjViewPortStruct + 0x234); // 22C

			if (index < 0 || index >= MAX_OBJECTSTRUCT)
			{
				MAIN_ATTACK_TARGET = -1;
				this->m_TargetAttack = -1;
			}
			else
			{
				OBJECTSTRUCT ObjectData = gObjUser.GetObject(index);

				if (ObjectData.live == 0)
				{
					MAIN_ATTACK_TARGET = -1;
					this->m_TargetAttack = -1;
				}
				else if (gObjUser.CheckDistance(gObjUser.GetIndex(), index, 8) == 0)
				{
					MAIN_ATTACK_TARGET = -1;
					this->m_TargetAttack = -1;
				}
				else
				{
					MAIN_ATTACK_TARGET = this->m_TargetAttack;
				}
			}
		}

		if (this->m_TargetAttack == -1)
		{
			int newTargetViewport = -1;
			int newTargetDis = 9;

			for (int x = 0; x < 400; x++)
			{
				DWORD ObjViewPortStruct = *(DWORD*)(PREVIEW_STRUCT)+(x * PREVIEW_STRUCT_SIZE); // 47c

				if (*(BYTE*)ObjViewPortStruct == 0)
				{
					continue;
				}

				if (*(BYTE*)(ObjViewPortStruct + 0x1A0) == 0)
				{
					continue;
				}

				WORD index = *(WORD*)(ObjViewPortStruct + 0x234); // 22C

				if (index < 0 || index >= MAX_OBJECTSTRUCT)
				{
					continue;
				}

				OBJECTSTRUCT ObjectData = gObjUser.GetObject(index);

				if (ObjectData.live == 0)
				{
					continue;
				}

				if (*(BYTE*)(ObjViewPortStruct + 0xBC) == 2) // monster
				{
					if (gObjUser.CalcDistance(gObjUser.GetIndex(), index) < newTargetDis)
					{
						newTargetViewport = x;
						newTargetDis = gObjUser.CalcDistance(gObjUser.GetIndex(), index);
					}
				}
			}
			
			if (newTargetViewport != -1)
			{
				this->m_TargetAttack = newTargetViewport;
				MAIN_ATTACK_TARGET = newTargetViewport;
			}
		}
	}

	if (EnableHelperAutoPause == 0 || this->HUDParam_CheckInterval() == true)
	{
		//-----------------------
		// -- Attack Skill
		//----------------------
		//1.02.11
		//*(DWORD*)0X05878034 = 0;
		//*(BYTE*)0X07EB9628 = 0;
		//if (!*(BYTE*)0X07EB9644)
		//	*(BYTE*)0X07EB9678 = 1;
		//*(BYTE*)0X07EB9644 = 1;
		//1.02.19
		*(DWORD*)0x05877FC4 = 0;
		*(BYTE*)0x07EB37F8 = 0;
		if (!*(BYTE*)0x07EB3814)
		*(BYTE*)0x07EB3848 = 1;
		*(BYTE*)0x07EB3814 = 1;
		this->HUDParam_SetInterval(500);
	}

	
}

void CAttackHelper::MouseMove()
{
	if (this->State != ATTACKHELPER_STATE_Running)
	{
		return;
	}

	if (MAIN_STATEEx != 5)
	{
		return;
	}

	if(EnableHelperAutoPause == 1)
	{
		this->HUDParam_SetInterval(3000);
	}
}

void CAttackHelper::DrawSelectMode()
{

	if (this->State == ATTACKHELPER_STATE_SelectMode)
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
		float StartX = (float)(MAIN_WIDTH / 2) - 60.0f;
		float StartY = 50.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);


		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}


		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}


		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}
		StartY += 30.0f;

		pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);

		if (CheckZone(StartX, StartY, 120.0, 22.0))
		{
			glColor3f((GLfloat)1061997773, (GLfloat)1058642330, (GLfloat)1053609165);
			pBlendDisableEx();
			pDrawImage2(240, StartX, StartY, 120.0, 22.0, 0, 0, 0.83203125, 1.0, 1, 1, 0.0);
			glColor3f(1065353216, 1065353216, 1065353216);
			pGLSwitch();
		}
		pBlendSwitch(1);

		StartY = 57.0f;
		pDrawTextOut((int)StartX + 25, (int)StartY, (char*)"Attack by rotating", 0, 0, 0);
		pDrawTextOut((int)StartX + 24, (int)StartY + 30, (char*)"Attack Cursor Lock", 0, 0, 0);
		pDrawTextOut((int)StartX + 25, (int)StartY + 60, (char*)"Attack Target Lock", 0, 0, 0);
		pDrawTextOut((int)StartX + 25, (int)StartY + 90, (char*)" Cancel Operation ", 0, 0, 0);
		/*GLSwitch();
	glColor3f(1.f, 1.f, 1.f);*/
	}
}

void CAttackHelper::HUDParam_Init()
{
	RECT rcClient;
	GetClientRect(MuWindow, &rcClient);
	this->HUDParam_Width = rcClient.right - rcClient.left;
	this->HUDParam_Height = rcClient.bottom - rcClient.top;
	this->HUDParam_PlayWidth = this->HUDParam_Width;
	this->HUDParam_PlayHeight = (int)((float)this->HUDParam_Height * this->HUDParam_GetBottonHeight());
	this->HUDParam_X = (int)(this->HUDParam_GetX() * (float)this->HUDParam_Width);
	this->HUDParam_Y = (int)(this->HUDParam_GetY() * (float)this->HUDParam_Height);
	this->HUDParam_CurrentAngle = 0.0f;
}

float CAttackHelper::HUDParam_GetBottonHeight()
{
	return 0.098f;
}

float CAttackHelper::HUDParam_GetX()
{
	return 0.5f;
}

float CAttackHelper::HUDParam_GetY()
{
	return 0.427083f; // 0.41927f; // : 0.427083f;
}

float CAttackHelper::HUDParam_GetRadius()
{
	return 100.0f / 500.0f;
}

float CAttackHelper::HUDParam_GetPI2()
{
	return 6.2831853f;
}

float CAttackHelper::HUDParam_GetRotateStep()
{
	return 0.123598775598f;//skorost vrawenia
}

void CAttackHelper::HUDParam_SetInterval(int delay)
{
	this->HUDParam_Interval = GetTickCount() + delay;
}

bool CAttackHelper::HUDParam_CheckInterval()
{
	return ((int)(GetTickCount() - this->HUDParam_Interval) > 0 ? true : false);
}