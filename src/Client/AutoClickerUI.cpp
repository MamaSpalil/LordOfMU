#include "stdafx.h"
#include "AutoClickerUI.h"
#include "Interface.h"
#include "InterfaceEx.h"
#include "Other.h"
#include "AttackHelper.h"
#include "Structure.h"
#include "SelectChar.h"

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
	LoadImagesTGA("Customs\\Interface\\btn_ac_settings.tga", AUTOCLICKER_IMG_SETTINGS, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\btn_ac_startstop.tga", AUTOCLICKER_IMG_STARTSTOP, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\btn_ac_stop.tga", AUTOCLICKER_IMG_STOP, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\btn_ac_history.tga", AUTOCLICKER_IMG_HISTORY, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\btn_ac_history_bg.tga", AUTOCLICKER_IMG_HISTORY_BG, 9729, 10496, 0, 1);
}

void CAutoClickerUI::Draw()
{
	if (pCheckWindow(27) && pCheckWindow(26))
	{
		return;
	}

	// FPS background is at pos_x=0, pos_y=0/0.5, width=66, height=28
	// Place buttons right after FPS display
	float btn_x = 68.0f;
	float btn_y = 3.0f;
	float btn_w = 22.0f;
	float btn_h = 22.0f;
	float btn_spacing = 2.0f;

	EnableAlphaTest(true);
	glColor3f(1.0f, 1.0f, 1.0f);

	// Button 1: Settings (gear)
	float settings_x = btn_x;
	if (MouseLeftButtonDown == 1 && CheckZone(settings_x, btn_y, btn_w, btn_h))
	{
		RenderBitmap(AUTOCLICKER_IMG_SETTINGS, settings_x + 0.5f, btn_y + 0.5f, btn_w, btn_h, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, 1.0f);
	}
	else
	{
		RenderBitmap(AUTOCLICKER_IMG_SETTINGS, settings_x, btn_y, btn_w, btn_h, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, 1.0f);
	}

	// Button 2: Start/Stop
	float startstop_x = settings_x + btn_w + btn_spacing;
	int startstop_img = (gAttackHelper->State == ATTACKHELPER_STATE_Running)
		? AUTOCLICKER_IMG_STOP
		: AUTOCLICKER_IMG_STARTSTOP;

	if (MouseLeftButtonDown == 1 && CheckZone(startstop_x, btn_y, btn_w, btn_h))
	{
		RenderBitmap(startstop_img, startstop_x + 0.5f, btn_y + 0.5f, btn_w, btn_h, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, 1.0f);
	}
	else
	{
		RenderBitmap(startstop_img, startstop_x, btn_y, btn_w, btn_h, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, 1.0f);
	}

	// Button 3: History action
	float history_x = startstop_x + btn_w + btn_spacing;
	if (MouseLeftButtonDown == 1 && CheckZone(history_x, btn_y, btn_w, btn_h))
	{
		RenderBitmap(AUTOCLICKER_IMG_HISTORY, history_x + 0.5f, btn_y + 0.5f, btn_w, btn_h, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, 1.0f);
	}
	else
	{
		RenderBitmap(AUTOCLICKER_IMG_HISTORY, history_x, btn_y, btn_w, btn_h, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, 1.0f);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	DisableAlphaBlend();

	// Tooltips
	if (CheckZone(settings_x, btn_y, btn_w, btn_h))
		pDrawToolTipEx((int)(settings_x), (int)(btn_y + btn_h + 2), (char*)"Settings (F9)");

	if (CheckZone(startstop_x, btn_y, btn_w, btn_h))
	{
		if (gAttackHelper->State == ATTACKHELPER_STATE_Running)
			pDrawToolTipEx((int)(startstop_x), (int)(btn_y + btn_h + 2), (char*)"Stop (F5)");
		else
			pDrawToolTipEx((int)(startstop_x), (int)(btn_y + btn_h + 2), (char*)"Start (F5)");
	}

	if (CheckZone(history_x, btn_y, btn_w, btn_h))
		pDrawToolTipEx((int)(history_x), (int)(btn_y + btn_h + 2), (char*)"History Action");

	// Draw history window if active
	if (this->HistoryActive)
	{
		this->DrawHistoryWindow();
	}
}

void CAutoClickerUI::MouseClickProc()
{
	float btn_x = 68.0f;
	float btn_y = 3.0f;
	float btn_w = 22.0f;
	float btn_h = 22.0f;
	float btn_spacing = 2.0f;

	float settings_x = btn_x;
	float startstop_x = settings_x + btn_w + btn_spacing;
	float history_x = startstop_x + btn_w + btn_spacing;

	// Settings button click
	if (CheckZone(settings_x, btn_y, btn_w, btn_h))
	{
		this->OpenSettings();
		return;
	}

	// Start/Stop button click
	if (CheckZone(startstop_x, btn_y, btn_w, btn_h))
	{
		this->ToggleStartStop();
		return;
	}

	// History button click
	if (CheckZone(history_x, btn_y, btn_w, btn_h))
	{
		this->ToggleHistory();
		return;
	}

	// Close history if clicking outside the history window
	if (this->HistoryActive)
	{
		float hw_x = (float)(MAIN_WIDTH / 2) - 100.0f;
		float hw_y = 40.0f;
		float hw_w = 200.0f;
		float hw_h = 250.0f;

		// Close button in history window title bar
		if (CheckZone(hw_x + hw_w - 18.0f, hw_y + 2.0f, 16.0f, 16.0f))
		{
			this->CloseHistory();
			return;
		}
	}
}

void CAutoClickerUI::ToggleStartStop()
{
	// Same as F5 - toggle attack helper
	if (gAttackHelper->State == ATTACKHELPER_STATE_Stop)
	{
		pSendChatTextEx((const char*)"", (const char*)"Attack Helper - Select Mode", Blue);
		gAttackHelper->SetState(ATTACKHELPER_STATE_SelectMode);
	}
	else
	{
		pSendChatTextEx((const char*)"", (const char*)"Attack Helper Off", Red);
		gAttackHelper->SetState(ATTACKHELPER_STATE_Stop);
	}
}

void CAutoClickerUI::OpenSettings()
{
	// Simulate F9 key press to open AVANTA+ELITE autoclicker menu (LordOfMU.dll)
	keybd_event(VK_F9, 0, 0, 0);
	keybd_event(VK_F9, 0, KEYEVENTF_KEYUP, 0);
}

void CAutoClickerUI::ToggleHistory()
{
	this->HistoryActive = !this->HistoryActive;
}

void CAutoClickerUI::CloseHistory()
{
	this->HistoryActive = false;
}

void CAutoClickerUI::AddHistoryEntry(const char* itemName)
{
	if (gAttackHelper->State != ATTACKHELPER_STATE_Running)
	{
		return;
	}

	// Shift entries if full
	if (this->HistoryCount >= AUTOCLICKER_HISTORY_MAX)
	{
		for (int i = 0; i < AUTOCLICKER_HISTORY_MAX - 1; i++)
		{
			memcpy(&this->History[i], &this->History[i + 1], sizeof(AutoClickerHistoryEntry));
		}
		this->HistoryCount = AUTOCLICKER_HISTORY_MAX - 1;
	}

	// Get current time
	SYSTEMTIME st;
	GetLocalTime(&st);

	AutoClickerHistoryEntry* entry = &this->History[this->HistoryCount];
	sprintf_s(entry->TimeStamp, sizeof(entry->TimeStamp), "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	strncpy(entry->ItemName, itemName, sizeof(entry->ItemName) - 1);
	entry->ItemName[sizeof(entry->ItemName) - 1] = '\0';

	this->HistoryCount++;
}

void CAutoClickerUI::DrawHistoryWindow()
{
	float hw_x = (float)(MAIN_WIDTH / 2) - 100.0f;
	float hw_y = 40.0f;
	float hw_w = 200.0f;
	float hw_h = 250.0f;

	EnableAlphaTest(true);
	glColor3f(1.0f, 1.0f, 1.0f);

	// Background
	RenderBitmap(AUTOCLICKER_IMG_HISTORY_BG, hw_x, hw_y, hw_w, hw_h, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, 0.8f);

	DisableAlphaBlend();

	// Title
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawTextOut((int)(hw_x + 50), (int)(hw_y + 4), "Action History", 0, 0, 0);

	// Close button text [X]
	DrawTextOut((int)(hw_x + hw_w - 16), (int)(hw_y + 4), "X", 0, 0, 0);

	// Column headers
	DrawTextOut((int)(hw_x + 8), (int)(hw_y + 22), "Time", 0, 0, 0);
	DrawTextOut((int)(hw_x + 65), (int)(hw_y + 22), "Item Picked Up", 0, 0, 0);

	// Draw separator line
	glColor4f(0.6f, 0.5f, 0.3f, 0.8f);
	DrawBarForm(hw_x + 4, hw_y + 32, hw_w - 8, 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw history entries (most recent first)
	int maxVisible = 17;
	int startIdx = this->HistoryCount - 1 - this->HistoryScrollOffset;
	int drawn = 0;

	for (int i = startIdx; i >= 0 && drawn < maxVisible; i--, drawn++)
	{
		float entry_y = hw_y + 36.0f + (float)(drawn * 12);

		DrawTextOut((int)(hw_x + 8), (int)entry_y, (LPCSTR)this->History[i].TimeStamp, 0, 0, 0);
		DrawTextOut((int)(hw_x + 65), (int)entry_y, (LPCSTR)this->History[i].ItemName, 0, 0, 0);
	}

	// Show empty message if no entries
	if (this->HistoryCount == 0)
	{
		DrawTextOut((int)(hw_x + 40), (int)(hw_y + 120), "No actions yet", 0, 0, 0);
	}

	EnableAlphaBlend();
	DisableAlphaBlend();
}
