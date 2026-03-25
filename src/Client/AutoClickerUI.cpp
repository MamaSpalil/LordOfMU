/**
 * \file  AutoClickerUI.cpp
 * \brief REPLACED — The Client's built-in AutoClicker UI buttons and history
 *        window have been replaced by the LordOfMU AutoClicker's ImGui
 *        overlay (rendered in D3D9 EndScene hook).
 *
 *        All drawing and mouse handling is disabled.  The button-click
 *        handlers now forward commands to the LordOfMU AutoClicker DLL
 *        via LordOfMUBridge.h (window messages to the game's HWND).
 */
#include "stdafx.h"
#include "AutoClickerUI.h"
#include "WndProc.h"
#include "LordOfMUBridge.h"

CAutoClickerUI gAutoClickerUI;

void CAutoClickerUI::Init()
{
	this->HistoryActive = false;
	this->HistoryCount = 0;
	this->HistoryScrollOffset = 0;
	memset(this->History, 0, sizeof(this->History));
}

void CAutoClickerUI::LoadImages()
{
	// DISABLED: LordOfMU's ImGui overlay renders its own HUD buttons.
	// The TGA button images are no longer needed.
}

void CAutoClickerUI::Draw()
{
	// DISABLED: LordOfMU's ImGui overlay (CImGuiOverlay::RenderHUD) provides
	// the Settings/Start-Stop/History buttons rendered in D3D9 EndScene.
}

void CAutoClickerUI::MouseClickProc()
{
	// DISABLED: LordOfMU's ImGui overlay handles mouse clicks on its own
	// HUD buttons via CImGuiOverlay::WndProcHandler.
}

void CAutoClickerUI::ToggleStartStop()
{
	// Forward to LordOfMU AutoClicker via simulated F5 key press.
	// LordOfMU's WH_KEYBOARD_LL hook intercepts F5 and toggles the clicker.
	LordOfMU_ToggleClicker(MuWindow);
}

void CAutoClickerUI::OpenSettings()
{
	// Forward to LordOfMU AutoClicker — open ImGui settings overlay.
	LordOfMU_OpenSettings(MuWindow);
}

void CAutoClickerUI::ToggleHistory()
{
	// Forward to LordOfMU AutoClicker — toggle ImGui history overlay.
	LordOfMU_ShowHistory(MuWindow);
}

void CAutoClickerUI::CloseHistory()
{
	// No-op: LordOfMU's ImGui overlay handles ESC to close.
	this->HistoryActive = false;
}

void CAutoClickerUI::AddHistoryEntry(const char* itemName)
{
	// No-op: LordOfMU's AutoPickupFilter logs pickup history internally
	// and the ImGui history overlay reads it via GetPickupHistory export.
}

void CAutoClickerUI::DrawHistoryWindow()
{
	// DISABLED: LordOfMU's ImGui overlay renders the history window.
}
