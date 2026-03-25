#include "stdafx.h"
#pragma once
#define MU_WND_PROC_HOOK 0x004DBC6D
#define MU_WND_PROC 0x004DA056

extern WNDPROC muWndProc;
extern HWND MuWindow;
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

