#include "stdafx.h"
#include "HUDButtons.h"
#include "MuTheme.h"
#include <windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM

#pragma comment(lib, "msimg32.lib")

// Color key for layered-window transparency (magenta → invisible)
static const COLORREF HUD_TRANSPARENT_KEY = RGB(255, 0, 255);

// Background color used in icon bitmap generation; must match the .bmp icon background.
static const COLORREF HUD_ICON_BG_COLOR = RGB(25, 22, 16);


CHUDButtons::CHUDButtons()
{
	m_hwndParent = NULL;
	m_hInstance = NULL;
	m_bClickerRunning = FALSE;
	m_bEnabled = FALSE;
	m_bGameActive = FALSE;
	m_hIcoSettings = NULL;
	m_hIcoPlay = NULL;
	m_hIcoStop = NULL;
	m_hIcoHistory = NULL;
	m_iHoverBtn = -1;
	m_iPressedBtn = -1;
	m_bTracking = FALSE;
	m_bTimerActive = FALSE;
	m_hMuCursor = NULL;
	m_hLinkCursor = NULL;
}


CHUDButtons::~CHUDButtons()
{
	Destroy();
}


BOOL CHUDButtons::Create(HWND hwndParent, HINSTANCE hInstance)
{
	m_hwndParent = hwndParent;
	m_hInstance = hInstance;

	// Load icon bitmaps
	m_hIcoSettings = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_SETTINGS), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoPlay = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_PLAY), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoStop = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_STOP), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoHistory = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_HISTORY), IMAGE_BITMAP, 0, 0, 0);

	// Validate that all icon bitmaps loaded successfully
	if (!m_hIcoSettings || !m_hIcoPlay || !m_hIcoStop || !m_hIcoHistory)
	{
		OutputDebugStringA("HUD: Failed to load icon bitmaps from hInstance, trying NULL\n");
		if (!m_hIcoSettings)
			m_hIcoSettings = (HBITMAP)LoadImage(NULL, MAKEINTRESOURCE(IDB_ICO_SETTINGS), IMAGE_BITMAP, 0, 0, 0);
		if (!m_hIcoPlay)
			m_hIcoPlay = (HBITMAP)LoadImage(NULL, MAKEINTRESOURCE(IDB_ICO_PLAY), IMAGE_BITMAP, 0, 0, 0);
		if (!m_hIcoStop)
			m_hIcoStop = (HBITMAP)LoadImage(NULL, MAKEINTRESOURCE(IDB_ICO_STOP), IMAGE_BITMAP, 0, 0, 0);
		if (!m_hIcoHistory)
			m_hIcoHistory = (HBITMAP)LoadImage(NULL, MAKEINTRESOURCE(IDB_ICO_HISTORY), IMAGE_BITMAP, 0, 0, 0);
	}

	// Calculate bar size
	int barWidth = BAR_PADDING * 2 + BTN_COUNT * BTN_SIZE + (BTN_COUNT - 1) * BTN_SPACING;
	int barHeight = BAR_PADDING * 2 + BTN_SIZE;

	// Initial rect — actual screen position is set by Reposition() after creation.
	RECT rcPos = { 0, 0, barWidth, barHeight };

	// Create as owned popup window (stays on top of game), initially hidden.
	// WS_EX_LAYERED enables color-key transparency (no black background).
	HWND hWnd = CWindowImpl<CHUDButtons>::Create(
		hwndParent,                             // owner window
		rcPos,                                  // initial size (repositioned later)
		NULL,                                   // no title
		WS_POPUP,                               // popup overlay, initially hidden
		WS_EX_LAYERED | WS_EX_NOACTIVATE  // layered + no-activate (owned popup stays above game)
	);

	if (!hWnd)
		return FALSE;

	// Magenta pixels become fully transparent (removes black background).
	// Add slight alpha transparency so buttons blend with the game scene.
	::SetLayeredWindowAttributes(m_hWnd, HUD_TRANSPARENT_KEY, 220, LWA_COLORKEY | LWA_ALPHA);

	// Load MU-themed cursor
	m_hMuCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_MU_CURSOR),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	m_hLinkCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_LINK_CURSOR),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);

	return TRUE;
}


void CHUDButtons::Destroy()
{
	if (m_hIcoSettings) { DeleteObject(m_hIcoSettings); m_hIcoSettings = NULL; }
	if (m_hIcoPlay) { DeleteObject(m_hIcoPlay); m_hIcoPlay = NULL; }
	if (m_hIcoStop) { DeleteObject(m_hIcoStop); m_hIcoStop = NULL; }
	if (m_hIcoHistory) { DeleteObject(m_hIcoHistory); m_hIcoHistory = NULL; }
	if (m_hMuCursor) { DestroyCursor(m_hMuCursor); m_hMuCursor = NULL; }
	if (m_hLinkCursor) { DestroyCursor(m_hLinkCursor); m_hLinkCursor = NULL; }

	if (IsWindow())
	{
		DestroyWindow();
	}
}


void CHUDButtons::Show()
{
	if (!IsWindow())
		return;

	m_bEnabled = TRUE;

	// When Show() is called (character selected), the game is necessarily
	// the foreground window, so mark it active.
	m_bGameActive = TRUE;

	// Start periodic reposition timer to track game window movement.
	if (!m_bTimerActive)
	{
		if (SetTimer(TIMER_REPOSITION, TIMER_REPOSITION_INTERVAL))
			m_bTimerActive = TRUE;
	}

	Reposition();
}


void CHUDButtons::Hide()
{
	if (!IsWindow())
		return;

	if (m_bTimerActive)
	{
		KillTimer(TIMER_REPOSITION);
		m_bTimerActive = FALSE;
	}

	if (IsWindowVisible())
		ShowWindow(SW_HIDE);
}


void CHUDButtons::Reset()
{
	m_bEnabled = FALSE;
	m_bClickerRunning = FALSE;
	m_bGameActive = FALSE;
	if (IsWindow())
	{
		if (m_bTimerActive)
		{
			KillTimer(TIMER_REPOSITION);
			m_bTimerActive = FALSE;
		}
		if (IsWindowVisible())
			ShowWindow(SW_HIDE);
		InvalidateRect(NULL, FALSE);
	}
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


void CHUDButtons::SetGameActive(BOOL bActive)
{
	m_bGameActive = bActive;

	if (!IsWindow())
		return;

	if (!bActive)
	{
		// Game lost foreground - hide HUD immediately
		if (IsWindowVisible())
			ShowWindow(SW_HIDE);
	}
	else
	{
		// Game regained foreground - show HUD if character was selected
		if (m_bEnabled && !IsWindowVisible())
		{
			Reposition();
			InvalidateRect(NULL, FALSE);
		}
	}
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

		// Rectangular hit test matching the new rounded-rectangle button shape
		if (x >= rc.left && x < rc.right && y >= rc.top && y < rc.bottom)
			return i;
	}
	return -1;
}


void CHUDButtons::DrawButton(HDC hDC, int idx, HBITMAP hIcon, BOOL bHover, BOOL bPressed)
{
	RECT rc = GetButtonRect(idx);
	int cx = (rc.left + rc.right) / 2;
	int cy = (rc.top + rc.bottom) / 2;
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	int cornerR = 4; // Corner radius for rounded rectangle

	// ---- MU Online S3E1 modern metallic button ----
	// Dark metallic panel with gold trim.  Square-ish with rounded corners,
	// subtle gradient, and a bright gold border that glows on hover.
	COLORREF clrBg, clrBgLight, clrBorder, clrBorderInner, clrHL;
	BOOL bShowHighlight = TRUE;

	if (bPressed) {
		clrBg          = RGB(15, 12, 5);
		clrBgLight     = RGB(28, 23, 10);
		clrBorder      = RGB(160, 130, 50);
		clrBorderInner = RGB(45, 38, 18);
		clrHL          = RGB(0, 0, 0);
		bShowHighlight = FALSE;  // No highlight when pressed
	} else if (bHover) {
		clrBg          = RGB(38, 32, 15);
		clrBgLight     = RGB(65, 55, 25);
		clrBorder      = RGB(220, 190, 80);
		clrBorderInner = RGB(90, 75, 35);
		clrHL          = RGB(180, 155, 65);
	} else {
		clrBg          = RGB(22, 18, 8);
		clrBgLight     = RGB(40, 34, 16);
		clrBorder      = RGB(120, 100, 42);
		clrBorderInner = RGB(55, 46, 22);
		clrHL          = RGB(100, 85, 38);
	}

	// Background fill — vertical gradient from darker (top) to lighter (bottom)
	int gradientSteps = (h > 1) ? (h - 1) : 1;
	for (int y = rc.top; y < rc.bottom; ++y)
	{
		int t = (y - rc.top) * 255 / gradientSteps;
		COLORREF clr = RGB(
			GetRValue(clrBg) + (GetRValue(clrBgLight) - GetRValue(clrBg)) * t / 255,
			GetGValue(clrBg) + (GetGValue(clrBgLight) - GetGValue(clrBg)) * t / 255,
			GetBValue(clrBg) + (GetBValue(clrBgLight) - GetBValue(clrBg)) * t / 255
		);
		HPEN hPn = CreatePen(PS_SOLID, 1, clr);
		HGDIOBJ hOP = SelectObject(hDC, hPn);
		MoveToEx(hDC, rc.left + 2, y, NULL);
		LineTo(hDC, rc.right - 2, y);
		SelectObject(hDC, hOP);
		DeleteObject(hPn);
	}

	// Inner border (subtle dark outline)
	{
		HPEN hPn = CreatePen(PS_SOLID, 1, clrBorderInner);
		HGDIOBJ hOP = SelectObject(hDC, hPn);
		HGDIOBJ hOB = SelectObject(hDC, GetStockObject(NULL_BRUSH));
		RoundRect(hDC, rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1, cornerR, cornerR);
		SelectObject(hDC, hOB);
		SelectObject(hDC, hOP);
		DeleteObject(hPn);
	}

	// Outer gold border
	{
		HPEN hPn = CreatePen(PS_SOLID, 1, clrBorder);
		HGDIOBJ hOP = SelectObject(hDC, hPn);
		HGDIOBJ hOB = SelectObject(hDC, GetStockObject(NULL_BRUSH));
		RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, cornerR + 1, cornerR + 1);
		SelectObject(hDC, hOB);
		SelectObject(hDC, hOP);
		DeleteObject(hPn);
	}

	// Top highlight line (specular) — skip if pressed
	if (bShowHighlight && !bPressed)
	{
		HPEN hPn = CreatePen(PS_SOLID, 1, clrHL);
		HGDIOBJ hOP = SelectObject(hDC, hPn);
		MoveToEx(hDC, rc.left + 3, rc.top + 2, NULL);
		LineTo(hDC, rc.right - 3, rc.top + 2);
		SelectObject(hDC, hOP);
		DeleteObject(hPn);
	}

	// Draw icon bitmap scaled to fit the button
	if (hIcon)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hIcon);

		BITMAP bm = {0};
		GetObject(hIcon, sizeof(bm), &bm);

		int icoSz = BTN_SIZE * 5 / 8; // ~62% of button, fits within borders
		int ix = cx - icoSz / 2;
		int iy = cy - icoSz / 2;

		// Slight offset when pressed (simulates push-in)
		if (bPressed)
		{
			ix += 1;
			iy += 1;
		}

		// Use TransparentBlt to skip the background color
		TransparentBlt(hDC, ix, iy, icoSz, icoSz,
			hMemDC, 0, 0, bm.bmWidth, bm.bmHeight,
			HUD_ICON_BG_COLOR);

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

	// Fill background with the transparent color key (magenta → invisible)
	HBRUSH hBgBr = CreateSolidBrush(HUD_TRANSPARENT_KEY);
	FillRect(hMemDC, &rc, hBgBr);
	DeleteObject(hBgBr);

	// Draw each button (no bar background / border)
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
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int btn = HitTest(x, y);

	if (btn >= 0)
	{
		m_iPressedBtn = btn;
		SetCapture();
		InvalidateRect(NULL, FALSE);
	}
	else
	{
		// Click missed all buttons - forward to game window
		POINT pt = { x, y };
		ClientToScreen(&pt);
		::ScreenToClient(m_hwndParent, &pt);
		::PostMessage(m_hwndParent, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
	}

	return 0;
}


LRESULT CHUDButtons::OnLButtonUp(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	if (m_iPressedBtn >= 0)
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		int btn = HitTest(x, y);

		if (btn == m_iPressedBtn && m_hwndParent)
		{
			// Send appropriate message to parent
			switch (btn)
			{
			case 0:
				::PostMessage(m_hwndParent, WM_HUD_SETTINGS, 0, 0);
				break;
			case 1:
				::PostMessage(m_hwndParent, WM_HUD_STARTSTOP, 0, 0);
				break;
			case 2:
				::PostMessage(m_hwndParent, WM_HUD_HISTORY, 0, 0);
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
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
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


LRESULT CHUDButtons::OnNCHitTest(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	// Convert screen coordinates to client coordinates.
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	ScreenToClient(&pt);

	// If the point lands on one of the circular buttons, return HTCLIENT
	// so the click is processed by the HUD.  Otherwise return HTTRANSPARENT
	// so the click passes through to the game window underneath.
	if (HitTest(pt.x, pt.y) >= 0)
		return HTCLIENT;

	return HTTRANSPARENT;
}


LRESULT CHUDButtons::OnSetCursor(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	if (LOWORD(lParam) == HTCLIENT)
	{
		// Consistent cursor: normal MU cursor by default, link/select when
		// LMB is pressed.  This matches the dialog cursor behavior.
		HCURSOR hCursor = NULL;

		if (m_iPressedBtn >= 0 && m_hLinkCursor != NULL)
		{
			hCursor = m_hLinkCursor;
		}
		else
		{
			hCursor = m_hMuCursor;
		}

		if (hCursor != NULL)
		{
			SetCursor(hCursor);
			bHandled = TRUE;
			return TRUE;
		}
	}
	bHandled = FALSE;
	return FALSE;
}


LRESULT CHUDButtons::OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&)
{
	// Prevent the HUD from stealing focus when the user clicks on a button.
	// This keeps the game window as the active/foreground window.
	return MA_NOACTIVATE;
}



void CHUDButtons::Reposition()
{
	if (!IsWindow() || !m_hwndParent || !::IsWindow(m_hwndParent))
		return;

	// Don't show if game window is minimized or not visible.
	if (::IsIconic(m_hwndParent) || !::IsWindowVisible(m_hwndParent))
	{
		if (IsWindowVisible())
			ShowWindow(SW_HIDE);
		return;
	}

	// Only reposition/show if HUD should be visible.
	if (!m_bEnabled || !m_bGameActive)
		return;

	// Convert client-area offset to screen coordinates.
	POINT ptScreen = { HUD_OFFSET_X, HUD_OFFSET_Y };
	::ClientToScreen(m_hwndParent, &ptScreen);

	DWORD dwFlags = SWP_NOSIZE | SWP_NOACTIVATE;
	if (!IsWindowVisible())
		dwFlags |= SWP_SHOWWINDOW;

	::SetWindowPos(m_hWnd, HWND_TOP,
		ptScreen.x, ptScreen.y, 0, 0, dwFlags);
}


LRESULT CHUDButtons::OnTimer(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	if (wParam == TIMER_REPOSITION)
	{
		if (m_bEnabled && m_bGameActive)
			Reposition();
		bHandled = TRUE;
	}
	else
	{
		bHandled = FALSE;
	}
	return 0;
}
