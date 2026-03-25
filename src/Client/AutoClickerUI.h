#pragma once
#include "stdafx.h"

#define AUTOCLICKER_IMG_SETTINGS    3810
#define AUTOCLICKER_IMG_STARTSTOP   3811
#define AUTOCLICKER_IMG_STOP        3812
#define AUTOCLICKER_IMG_HISTORY     3813
#define AUTOCLICKER_IMG_HISTORY_BG  3814

#define AUTOCLICKER_HISTORY_MAX     50

struct AutoClickerHistoryEntry
{
	char ItemName[64];
	char TimeStamp[32];
};

class CAutoClickerUI
{
public:
	void Init();
	void LoadImages();
	void Draw();
	void MouseClickProc();
	void ToggleStartStop();
	void OpenSettings();
	void ToggleHistory();
	void AddHistoryEntry(const char* itemName);
	void DrawHistoryWindow();
	void CloseHistory();

	bool HistoryActive;
	int  HistoryCount;
	int  HistoryScrollOffset;
	AutoClickerHistoryEntry History[AUTOCLICKER_HISTORY_MAX];
};

extern CAutoClickerUI gAutoClickerUI;
