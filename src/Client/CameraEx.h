// Camera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#pragma once

struct CAMERA_ZOOM
{
	float MinPercent;
	float MaxPercent;
	float MinLimit;
	float MaxLimit;
	float Precision;
};

struct CAMERA_ADDR
{
	float* Zoom;
	float* RotY;
	float* RotZ;
	float* PosZ;
	float* ClipX;
	float* ClipY;
	float* ClipGL;
};


struct CAMERA_INFO
{
	int IsLoad;
	float Zoom;
	float RotY;
	float RotZ;
	float PosZ;
	float ClipX;
	float ClipY;
	float ClipGL;
};



class CCamera
{
public:
	void Init();
	bool CheckEnable();
	void Reset();
	void ForceResetAndShutdown();
	void Switch();
	static void RotateDamageAngle();
	void Update();
	void UpdateCursor();
	void MainProc();
	void Zoom(int dir);
	void SetCurrentValue();
	BOOL m_Enable;
	BOOL m_IsMove;
	int TempCursorX;
	int TempCursorY;
	LONG m_CursorX;
	LONG m_CursorY;
	CAMERA_ZOOM m_Zoom;
	CAMERA_ADDR m_Address;
	CAMERA_INFO m_Default;
};

extern CCamera * gCamera;
