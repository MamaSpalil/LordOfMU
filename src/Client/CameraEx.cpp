#include "stdafx.h"
#include "CameraEx.h"
#include "Other.h"
#include "TMemory.h"
#include "Protocol.h"



CCamera * gCamera = new CCamera();

void CCamera::Init()
{
	this->m_Enable = 0;
	this->m_IsMove = 0;
	this->m_CursorX = 0;
	this->m_CursorY = 0;
	this->m_Zoom.MinPercent = 50.0;
	this->m_Zoom.MaxPercent = 220.0;
	this->m_Zoom.Precision = 5.0;

	this->m_Address.Zoom = (float*)0x006CFA70;
	this->m_Address.RotY = (float*)0x007B5920;
	this->m_Address.RotZ = (float*)0x07EB3838;
	this->m_Address.PosZ = (float*)0x007B3720;
	this->m_Address.ClipX = (float*)0x007B5834;
	this->m_Address.ClipY = (float*)0x00676D8E;
	this->m_Address.ClipGL = (float*)0x007B532C;

	this->m_Default.IsLoad = 0;

	//SetCompleteHook(0xE9,0x006B94E2,&RotateDamageAngle);
}

void __declspec(naked) CCamera::RotateDamageAngle() // OK
{
	/*static DWORD RotateDamageAngleAddress1 = 0x006B9404;

	_asm
	{
		Lea Eax,[Ebp-0x8]
		Lea Ecx,[Ebp-0xC]
		Push Dword Ptr[Ebp+0x18]
		Push Eax
		Push Ecx
		Call CalcDamageAngle
		Add Esp,0xC
		Jmp[RotateDamageAngleAddress1]
	}*/
}
void CCamera::SetCurrentValue() // OK
{
	SetFloat((LPVOID)this->m_Address.ClipX, (4190.0f + (((float)abs(((*this->m_Address.PosZ) - 150.0f)))*3.0f)));
	SetFloat((LPVOID)this->m_Address.ClipY, (5400.0f + (((float)abs(((*this->m_Address.PosZ) - 150.0f)))*3.0f)));
	SetFloat((LPVOID)this->m_Address.ClipGL, (2100.0f + (((float)abs(((*this->m_Address.PosZ) - 150.0f)))*3.0f)));
	//SetFloat((DWORD)this->m_Address.ClipX[0],(4290.0f+(abs(((*this->m_Address.PosZ)-150.0f))*3.0f)));

	//SetFloat((DWORD)this->m_Address.ClipX[1],(4190.0f+(abs(((*this->m_Address.PosZ)-150.0f))*3.0f)));

	//SetFloat((DWORD)this->m_Address.ClipY[0],(5500.0f+(((float)abs(((*this->m_Address.PosZ)-150.0f)))*3.0f)));

	//SetFloat((DWORD)this->m_Address.ClipY[1],(5500.0f+(((float)abs(((*this->m_Address.PosZ)-150.0f)))*3.0f)));

	//SetFloat((DWORD)this->m_Address.ClipGL,(5100.0f+(((float)abs(((*this->m_Address.PosZ)-150.0f)))*3.0f)));
}

void CCamera::ForceResetAndShutdown()
{
	if (this->m_Default.IsLoad != 0)
	{
		SetFloat((LPVOID)this->m_Address.Zoom, this->m_Default.Zoom);
		SetFloat((LPVOID)this->m_Address.RotZ, this->m_Default.RotZ);
		SetFloat((LPVOID)this->m_Address.RotY, this->m_Default.RotY);
		SetFloat((LPVOID)this->m_Address.PosZ, this->m_Default.PosZ);
		SetFloat((LPVOID)this->m_Address.ClipX, this->m_Default.ClipX);
		SetFloat((LPVOID)this->m_Address.ClipY, this->m_Default.ClipY);
		SetFloat((LPVOID)this->m_Address.ClipGL, this->m_Default.ClipGL);
	}

}

bool CCamera::CheckEnable()
{


	if (MAIN_STATEEx != 5)
	{
		this->ForceResetAndShutdown();
		return 0;
	}


	
	return 1;
}

void CCamera::Reset()
{
	if (this->m_Enable == 0)
	{
		return;
	}

	if (this->m_Default.IsLoad != 0)
	{
		SetFloat((LPVOID)this->m_Address.Zoom, this->m_Default.Zoom);
		SetFloat((LPVOID)this->m_Address.RotZ, this->m_Default.RotZ);
		SetFloat((LPVOID)this->m_Address.RotY, this->m_Default.RotY);
		SetFloat((LPVOID)this->m_Address.PosZ, this->m_Default.PosZ);
		SetFloat((LPVOID)this->m_Address.ClipX, this->m_Default.ClipX);
		SetFloat((LPVOID)this->m_Address.ClipY, this->m_Default.ClipY);
		SetFloat((LPVOID)this->m_Address.ClipGL, this->m_Default.ClipGL);
	}

	this->m_CursorX = 0;
	this->m_CursorY = 0;
}

void CCamera::Switch()
{
	if (this->CheckEnable() == 0)
	{
		return;
	}

	if (this->m_Default.IsLoad == 0)
	{
		this->m_Default.IsLoad = 1;

		this->m_Default.Zoom = (*this->m_Address.Zoom);
		this->m_Default.RotY = (*this->m_Address.RotY);
		this->m_Default.RotZ = (*this->m_Address.RotZ);
		this->m_Default.PosZ = (*this->m_Address.PosZ);
		this->m_Default.ClipX = (*this->m_Address.ClipX);
		this->m_Default.ClipY = (*this->m_Address.ClipY);
		this->m_Default.ClipGL = (*this->m_Address.ClipGL);
	}
	
	this->m_Enable ^= 1;

	if (this->m_Enable)
	{
		pSendChatTextEx((const char *)"", (const char *)"[3D Camera On]", Blue);
	}
	else
	{
		pSendChatTextEx((const char *)"", (const char *)"[3D Camera Off]", Red);
	}
	
}

void CCamera::Update()
{
	SetFloat((LPVOID)this->m_Address.ClipX, (4190.0f + (((float)abs(((*this->m_Address.PosZ) - 150.0f)))*3.0f)));
	SetFloat((LPVOID)this->m_Address.ClipY, (5400.0f + (((float)abs(((*this->m_Address.PosZ) - 150.0f)))*3.0f)));
	SetFloat((LPVOID)this->m_Address.ClipGL, (2100.0f + (((float)abs(((*this->m_Address.PosZ) - 150.0f)))*3.0f)));
}

void CCamera::MainProc()
{
	if (this->m_Enable == 0 || this->m_Default.IsLoad != 1 || MouseWellButtonDown == 0 || this->CheckEnable() == 0)
	{
		return;
	}
	
	if ((this->m_CursorX + 5) < MAIN_CURSORX)
	{
		if ((*this->m_Address.RotZ) > 309.0f)
		{
			SetFloat((LPVOID)this->m_Address.RotZ, -45.0f);
		}
		else
		{
			SetFloat((LPVOID)this->m_Address.RotZ, ((*this->m_Address.RotZ) + 6.0f));
		}
	}

	if ((this->m_CursorX - 5) > MAIN_CURSORX)
	{
		if ((*this->m_Address.RotZ) < -417.0f)
		{
			SetFloat((LPVOID)this->m_Address.RotZ, -45.0f);
		}
		else
		{
			SetFloat((LPVOID)this->m_Address.RotZ, ((*this->m_Address.RotZ) - 6.0f));
		}
	}

	if ((this->m_CursorY + 5) < MAIN_CURSORY)
	{
		if ((*this->m_Address.RotY) < -45.0f)
		{
			SetFloat((LPVOID)this->m_Address.RotY, ((*this->m_Address.RotY) + 2.42f));
			SetFloat((LPVOID)this->m_Address.PosZ, ((*this->m_Address.PosZ) - 44.0f));
		}
	}

	if ((this->m_CursorY - 5) > MAIN_CURSORY)
	{
		if ((*this->m_Address.RotY) > -90.0f)
		{
			SetFloat((LPVOID)this->m_Address.RotY, ((*this->m_Address.RotY) - 2.42f));
			SetFloat((LPVOID)this->m_Address.PosZ, ((*this->m_Address.PosZ) + 44.0f));
		}
	}

	this->m_CursorX = MAIN_CURSORX;
	this->m_CursorY = MAIN_CURSORY;

	this->Update();
}

void CCamera::Zoom(int dir)
{
	if (this->m_Enable == 0 || this->m_Default.IsLoad != 1 || this->CheckEnable() == 0)
	{
		return;
	}

	this->m_Zoom.MinLimit = (this->m_Default.Zoom / 100)*this->m_Zoom.MinPercent;

	this->m_Zoom.MaxLimit = (this->m_Default.Zoom / 100)*this->m_Zoom.MaxPercent;

	if (dir > 0)
	{
		if ((*this->m_Address.Zoom) >= this->m_Zoom.MinLimit)
		{
			SetFloat((LPVOID)this->m_Address.Zoom, ((*this->m_Address.Zoom) - this->m_Zoom.Precision));
		}
	}

	if (dir < 0)
	{
		if ((*this->m_Address.Zoom) <= this->m_Zoom.MaxLimit)
		{
			SetFloat((LPVOID)this->m_Address.Zoom, ((*this->m_Address.Zoom) + this->m_Zoom.Precision));
		}
	}

	this->Update();
}

void CCamera::UpdateCursor()
{
	this->m_CursorX = MAIN_CURSORX;
	this->m_CursorY = MAIN_CURSORY;
}