/**
 * \file  AttackHelper.cpp
 * \brief DISABLED — The Client's built-in AttackHelper bot has been replaced
 *        by the LordOfMU AutoClicker (LordOfClicker DLL) which provides a
 *        full-featured ImGui overlay, D3D9 EndScene hook, and cross-DLL
 *        communication.
 *
 *        All bot logic (MainProc, mouse movement, game memory writes) has
 *        been removed.  The class is kept as a stub so that existing code
 *        that references gAttackHelper->State still compiles.
 */
#include "stdafx.h"
#include "AttackHelper.h"

CAttackHelper * gAttackHelper = new CAttackHelper;
BOOL EnableHelperAutoPause;

void CAttackHelper::Init()
{
	// Stub: LordOfMU AutoClicker handles all bot functionality.
	this->State = ATTACKHELPER_STATE_Stop;
	this->m_TargetAttack = -1;
}

void CAttackHelper::SetState(int State)
{
	// Stub: state is kept for compatibility but no game memory is modified.
	// LordOfMU's CClickerJob handles attack skill memory writes.
	this->State = State;
}

void CAttackHelper::MainProc()
{
	// DISABLED: LordOfMU AutoClicker (CClickerJob) handles the attack loop.
}

void CAttackHelper::MouseMove()
{
	// DISABLED: LordOfMU AutoClicker handles mouse movement during clicking.
}

void CAttackHelper::DrawSelectMode()
{
	// DISABLED: LordOfMU's ImGui overlay provides the attack mode selection UI.
}

void CAttackHelper::HUDParam_Init()           {}
float CAttackHelper::HUDParam_GetBottonHeight() { return 0.098f; }
float CAttackHelper::HUDParam_GetX()           { return 0.5f; }
float CAttackHelper::HUDParam_GetY()           { return 0.427083f; }
float CAttackHelper::HUDParam_GetRadius()      { return 100.0f / 500.0f; }
float CAttackHelper::HUDParam_GetPI2()         { return 6.2831853f; }
float CAttackHelper::HUDParam_GetRotateStep()  { return 0.123598775598f; }
void  CAttackHelper::HUDParam_SetInterval(int) {}
bool  CAttackHelper::HUDParam_CheckInterval()  { return false; }