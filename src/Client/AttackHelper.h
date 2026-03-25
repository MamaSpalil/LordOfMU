#pragma once


#define ATTACKHELPER_STATE_Stop				0
#define ATTACKHELPER_STATE_SelectMode		1
#define ATTACKHELPER_STATE_Running			2
#define ATTACKHELPER_STATE_Paused			3

class CAttackHelper
{
public:
	void Init();
	void SetState(int State);
	void MainProc();
	void MouseMove();
	void DrawSelectMode();
	// -- HUD Params
	void HUDParam_Init();
	float HUDParam_GetBottonHeight();
	float HUDParam_GetX();
	float HUDParam_GetY();
	float HUDParam_GetRadius();
	float HUDParam_GetPI2();
	float HUDParam_GetRotateStep();
	void HUDParam_SetInterval(int delay);
	bool HUDParam_CheckInterval();

	int State;

	POINT ProcMouseStartPoint;
	POINT ProcMousePoint;

	// -- HUD Params
	int HUDParam_X;
	int HUDParam_Y;
	int HUDParam_Width;
	int HUDParam_Height;
	int HUDParam_PlayWidth;
	int HUDParam_PlayHeight;
	float HUDParam_CurrentAngle;
	DWORD HUDParam_Interval;

	// -- Target
	int m_TargetAttack;
	int AttackType;
};

extern CAttackHelper * gAttackHelper;