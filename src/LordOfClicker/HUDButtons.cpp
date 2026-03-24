#include "stdafx.h"
#include "HUDButtons.h"
#include "MuTheme.h"

#pragma comment(lib, "msimg32.lib")


CHUDButtons::CHUDButtons()
{
	m_hwndOwner = NULL;
	m_hInstance = NULL;
	m_bClickerRunning = FALSE;
	m_hIcoSettings = NULL;
	m_hIcoPlay = NULL;
	m_hIcoStop = NULL;
	m_hIcoHistory = NULL;
	m_iHoverBtn = -1;
	m_iPressedBtn = -1;
	m_bTracking = FALSE;
}


CHUDButtons::~CHUDButtons()
{
	Destroy();
}


BOOL CHUDButtons::Create(HWND hwndOwner, HINSTANCE hInstance)
{
	m_hwndOwner = hwndOwner;
	m_hInstance = hInstance;

	// Load icon bitmaps
	m_hIcoSettings = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_SETTINGS), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoPlay = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_PLAY), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoStop = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_STOP), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoHistory = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_HISTORY), IMAGE_BITMAP, 0, 0, 0);

	// Calculate bar size
	int barWidth = BAR_PADDING * 2 + BTN_COUNT * BTN_SIZE + (BTN_COUNT - 1) * BTN_SPACING;
	int barHeight = BAR_PADDING * 2 + BTN_SIZE;

	// Get game window position for initial placement
	RECT rcOwner = {0};
	::GetWindowRect(hwndOwner, &rcOwner);

	RECT rcClient = {0};
	::GetClientRect(hwndOwner, &rcClient);
	POINT ptClient = {0, 0};
	::ClientToScreen(hwndOwner, &ptClient);

	// Position to the right of the FPS counter in the game client area.
	// FPS label is approximately 70px wide at top-left, so place HUD at x=90, y=48.
	int x = ptClient.x + 90;
	int y = ptClient.y + 48;

	// Create as owned popup window (stays on top of game), initially hidden.
	// Show() must be called later (after character selection) to make it visible.
	HWND hWnd = CWindowImpl<CHUDButtons>::Create(
		hwndOwner,                              // owner window
		CWindow::rcDefault,                     // position (set below)
		NULL,                                   // no title
		WS_POPUP,                               // popup, initially hidden
		WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST  // no taskbar, no focus steal, topmost
	);

	if (!hWnd)
		return FALSE;

	// Position and size the window (still hidden)
	SetWindowPos(NULL, x, y, barWidth, barHeight, SWP_NOZORDER | SWP_NOACTIVATE);

	return TRUE;
}


void CHUDButtons::Destroy()
{
	if (m_hIcoSettings) { DeleteObject(m_hIcoSettings); m_hIcoSettings = NULL; }
	if (m_hIcoPlay) { DeleteObject(m_hIcoPlay); m_hIcoPlay = NULL; }
	if (m_hIcoStop) { DeleteObject(m_hIcoStop); m_hIcoStop = NULL; }
	if (m_hIcoHistory) { DeleteObject(m_hIcoHistory); m_hIcoHistory = NULL; }

	if (IsWindow())
	{
		KillTimer(REPOSITION_TIMER_ID);
		DestroyWindow();
	}
}


void CHUDButtons::Show()
{
	if (!IsWindow())
		return;

	if (IsWindowVisible())
		return;

	ShowWindow(SW_SHOWNOACTIVATE);
	SetTimer(REPOSITION_TIMER_ID, 200, NULL);
	Reposition();
}


void CHUDButtons::SetClickerRunning(BOOL bRunning)
{
	if (m_bClickerRunning != bRunning)
	{
		m_bClickerRunning = bRunning;
		if (IsWindow())
			InvalidateRect(NULL, FALSE);
	}
}


void CHUDButtons::Reposition()
{
	if (!IsWindow() || !::IsWindow(m_hwndOwner))
		return;

	// Only show when the game window is visible
	if (!::IsWindowVisible(m_hwndOwner))
	{
		ShowWindow(SW_HIDE);
		return;
	}

	RECT rcClient = {0};
	::GetClientRect(m_hwndOwner, &rcClient);
	POINT ptClient = {0, 0};
	::ClientToScreen(m_hwndOwner, &ptClient);

	// Position to the right of the FPS counter
	int x = ptClient.x + 90;
	int y = ptClient.y + 48;

	// Keep TOPMOST so the HUD stays visible above the game's
	// DirectDraw/Direct3D rendering surface.
	SetWindowPos(HWND_TOPMOST, x, y, 0, 0,
		SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
}


RECT CHUDButtons::GetButtonRect(int idx)
{
	RECT rc;
	rc.left = BAR_PADDING + idx * (BTN_SIZE + BTN_SPACING);
	rc.top = BAR_PADDING;
	rc.right = rc.left + BTN_SIZE;
	rc.bottom = rc.top + BTN_SIZE;
	return rc;
}


int CHUDButtons::HitTest(int x, int y)
{
	for (int i = 0; i < BTN_COUNT; ++i)
	{
		RECT rc = GetButtonRect(i);

		// Circular hit test
		int cx = (rc.left + rc.right) / 2;
		int cy = (rc.top + rc.bottom) / 2;
		int dx = x - cx;
		int dy = y - cy;
		int r = BTN_SIZE / 2;

		if (dx * dx + dy * dy <= r * r)
			return i;
	}
	return -1;
}


void CHUDButtons::DrawButton(HDC hDC, int idx, HBITMAP hIcon, BOOL bHover, BOOL bPressed)
{
	RECT rc = GetButtonRect(idx);
	int cx = (rc.left + rc.right) / 2;
	int cy = (rc.top + rc.bottom) / 2;
	int r = BTN_SIZE / 2 - 1;

	// Draw circular button background
	COLORREF clrBg = bPressed ? CMuTheme::ClrBtnPressed() :
					 bHover ? CMuTheme::ClrBtnHover() : CMuTheme::ClrBtnBg();
	HBRUSH hBr = CreateSolidBrush(clrBg);
	HPEN hPen = CreatePen(PS_SOLID, 2,
		bHover ? CMuTheme::ClrFrameBright() : CMuTheme::ClrFrameGold());
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, hBr);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	Ellipse(hDC, cx - r, cy - r, cx + r, cy + r);

	SelectObject(hDC, hOldBr);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBr);
	DeleteObject(hPen);

	// Draw icon bitmap centered in button
	if (hIcon)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hIcon);

		BITMAP bm = {0};
		GetObject(hIcon, sizeof(bm), &bm);

		int ix = cx - bm.bmWidth / 2;
		int iy = cy - bm.bmHeight / 2;

		// Use TransparentBlt to skip the background color
		TransparentBlt(hDC, ix, iy, bm.bmWidth, bm.bmHeight,
			hMemDC, 0, 0, bm.bmWidth, bm.bmHeight,
			RGB(25, 22, 16)); // BG_COLOR used in icon generation

		SelectObject(hMemDC, hOldBmp);
		DeleteDC(hMemDC);
	}
}


LRESULT CHUDButtons::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(&ps);

	RECT rc;
	GetClientRect(&rc);

	// Double buffer
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBmp = CreateCompatibleBitmap(hDC, rc.right, rc.bottom);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

	// Fill background with semi-transparent dark
	HBRUSH hBgBr = CreateSolidBrush(RGB(10, 8, 5));
	FillRect(hMemDC, &rc, hBgBr);
	DeleteObject(hBgBr);

	// Draw thin gold border around the bar
	HPEN hBorderPen = CreatePen(PS_SOLID, 1, CMuTheme::ClrFrameGold());
	HPEN hOldPen = (HPEN)SelectObject(hMemDC, hBorderPen);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hMemDC, GetStockObject(NULL_BRUSH));
	RoundRect(hMemDC, 0, 0, rc.right, rc.bottom, 6, 6);
	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBr);
	DeleteObject(hBorderPen);

	// Draw each button
	// Button 0: Settings
	DrawButton(hMemDC, 0, m_hIcoSettings,
		m_iHoverBtn == 0, m_iPressedBtn == 0);

	// Button 1: Start/Stop (icon changes based on state)
	HBITMAP hStartStopIcon = m_bClickerRunning ? m_hIcoStop : m_hIcoPlay;
	DrawButton(hMemDC, 1, hStartStopIcon,
		m_iHoverBtn == 1, m_iPressedBtn == 1);

	// Button 2: History
	DrawButton(hMemDC, 2, m_hIcoHistory,
		m_iHoverBtn == 2, m_iPressedBtn == 2);

	// Blit to screen
	BitBlt(hDC, 0, 0, rc.right, rc.bottom, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);

	EndPaint(&ps);
	return 0;
}


LRESULT CHUDButtons::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
{
	return 1; // Handled in OnPaint
}


LRESULT CHUDButtons::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	int btn = HitTest(x, y);

	if (btn >= 0)
	{
		m_iPressedBtn = btn;
		SetCapture();
		InvalidateRect(NULL, FALSE);
	}

	return 0;
}


LRESULT CHUDButtons::OnLButtonUp(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	if (m_iPressedBtn >= 0)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		int btn = HitTest(x, y);

		if (btn == m_iPressedBtn && m_hwndOwner)
		{
			// Send appropriate message to parent
			switch (btn)
			{
			case 0:
				::PostMessage(m_hwndOwner, WM_HUD_SETTINGS, 0, 0);
				break;
			case 1:
				::PostMessage(m_hwndOwner, WM_HUD_STARTSTOP, 0, 0);
				break;
			case 2:
				::PostMessage(m_hwndOwner, WM_HUD_HISTORY, 0, 0);
				break;
			}
		}

		m_iPressedBtn = -1;
		ReleaseCapture();
		InvalidateRect(NULL, FALSE);
	}

	return 0;
}


LRESULT CHUDButtons::OnMouseMove(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	int btn = HitTest(x, y);

	if (btn != m_iHoverBtn)
	{
		m_iHoverBtn = btn;
		InvalidateRect(NULL, FALSE);
	}

	// Track mouse leave
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme = {0};
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		TrackMouseEvent(&tme);
		m_bTracking = TRUE;
	}

	return 0;
}


LRESULT CHUDButtons::OnMouseLeave(UINT, WPARAM, LPARAM, BOOL&)
{
	m_iHoverBtn = -1;
	m_bTracking = FALSE;
	InvalidateRect(NULL, FALSE);
	return 0;
}


LRESULT CHUDButtons::OnTimer(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if (wParam == REPOSITION_TIMER_ID)
	{
		Reposition();
		bHandled = TRUE;
	}
	else
	{
		bHandled = FALSE;
	}

	return 0;
}
