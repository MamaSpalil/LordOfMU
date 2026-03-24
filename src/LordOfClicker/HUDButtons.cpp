#include "stdafx.h"
#include "HUDButtons.h"
#include "MuTheme.h"

#include <windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM

#pragma comment(lib, "msimg32.lib")

// Color key for layered-window transparency (magenta → invisible)
static const COLORREF HUD_TRANSPARENT_KEY = RGB(255, 0, 255);


CHUDButtons::CHUDButtons()
{
	m_hwndParent = NULL;
	m_hInstance = NULL;
	m_bClickerRunning = FALSE;
	m_bEnabled = FALSE;
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


BOOL CHUDButtons::Create(HWND hwndParent, HINSTANCE hInstance)
{
	m_hwndParent = hwndParent;
	m_hInstance = hInstance;

	// Load icon bitmaps
	m_hIcoSettings = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_SETTINGS), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoPlay = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_PLAY), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoStop = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_STOP), IMAGE_BITMAP, 0, 0, 0);
	m_hIcoHistory = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_HISTORY), IMAGE_BITMAP, 0, 0, 0);

	// Calculate bar size
	int barWidth = BAR_PADDING * 2 + BTN_COUNT * BTN_SIZE + (BTN_COUNT - 1) * BTN_SPACING;
	int barHeight = BAR_PADDING * 2 + BTN_SIZE;

	// Position in client coordinates of the parent (right of FPS counter).
	int x = 70;
	int y = 3;

	// Create as a child window of the game window.  WS_CHILD makes the HUD
	// an integral part of the game window: it moves, shows and hides together
	// with the parent automatically, no separate positioning or z-order needed.
	// WS_EX_LAYERED + color-key transparency lets the game scene show through.
	RECT rcPos = { x, y, x + barWidth, y + barHeight };
	HWND hWnd = CWindowImpl<CHUDButtons>::Create(
		hwndParent,                             // parent window
		rcPos,                                  // position in client coords
		NULL,                                   // no title
		WS_CHILD | WS_CLIPSIBLINGS,            // child, initially hidden
		WS_EX_LAYERED                           // layered for color-key transparency
	);

	if (!hWnd)
		return FALSE;

	// Magenta pixels become fully transparent (removes black background).
	::SetLayeredWindowAttributes(m_hWnd, HUD_TRANSPARENT_KEY, 0, LWA_COLORKEY);

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
		DestroyWindow();
	}
}


void CHUDButtons::Show()
{
	if (!IsWindow())
		return;

	m_bEnabled = TRUE;

	if (!IsWindowVisible())
		ShowWindow(SW_SHOWNA);
}


void CHUDButtons::Hide()
{
	if (!IsWindow())
		return;

	if (IsWindowVisible())
		ShowWindow(SW_HIDE);
}


void CHUDButtons::Reset()
{
	m_bEnabled = FALSE;
	m_bClickerRunning = FALSE;
	Hide();
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
	int r = BTN_SIZE / 2;

	// ---- MU Online S3E1 metallic 3D button ----
	// Dark-gold color palette: outer edge dark, centre lighter (radial gradient).
	COLORREF clrOuter, clrMid, clrInner, clrBorder, clrHL;
	if (bPressed) {
		clrOuter  = RGB(20, 16, 6);
		clrMid    = RGB(40, 34, 14);
		clrInner  = RGB(58, 48, 20);
		clrBorder = RGB(150, 120, 45);
		clrHL     = RGB(80, 65, 28);
	} else if (bHover) {
		clrOuter  = RGB(48, 40, 17);
		clrMid    = RGB(95, 78, 33);
		clrInner  = RGB(155, 130, 55);
		clrBorder = RGB(215, 185, 75);
		clrHL     = RGB(205, 178, 82);
	} else {
		clrOuter  = RGB(32, 26, 11);
		clrMid    = RGB(62, 50, 22);
		clrInner  = RGB(95, 76, 32);
		clrBorder = RGB(125, 100, 42);
		clrHL     = RGB(135, 112, 50);
	}

	// 3 concentric ellipses simulate a radial gradient (outer → inner).
	struct { COLORREF c; int ri; } layers[] = {
		{ clrOuter, r - 1 },
		{ clrMid,   r * 2 / 3 },
		{ clrInner, r / 3 }
	};

	for (int i = 0; i < 3; ++i)
	{
		HBRUSH hBr = CreateSolidBrush(layers[i].c);
		HPEN   hPn = CreatePen(PS_SOLID, 1, layers[i].c);
		HGDIOBJ hOB = SelectObject(hDC, hBr);
		HGDIOBJ hOP = SelectObject(hDC, hPn);
		int ri = layers[i].ri;
		Ellipse(hDC, cx - ri, cy - ri, cx + ri, cy + ri);
		SelectObject(hDC, hOB);
		SelectObject(hDC, hOP);
		DeleteObject(hBr);
		DeleteObject(hPn);
	}

	// Gold border ring
	{
		HPEN hPn = CreatePen(PS_SOLID, 1, clrBorder);
		HGDIOBJ hOP = SelectObject(hDC, hPn);
		HGDIOBJ hOB = SelectObject(hDC, GetStockObject(NULL_BRUSH));
		Ellipse(hDC, cx - r + 1, cy - r + 1, cx + r - 1, cy + r - 1);
		SelectObject(hDC, hOB);
		SelectObject(hDC, hOP);
		DeleteObject(hPn);
	}

	// Specular highlight crescent at the top of the button
	if (!bPressed)
	{
		int clipR = r - 2;
		if (clipR > 1)
		{
			HRGN hClip = CreateEllipticRgn(
				cx - clipR, cy - clipR, cx + clipR, cy - 1);
			SelectClipRgn(hDC, hClip);

			HBRUSH hBr = CreateSolidBrush(clrHL);
			HPEN   hPn = CreatePen(PS_SOLID, 1, clrHL);
			HGDIOBJ hOB = SelectObject(hDC, hBr);
			HGDIOBJ hOP = SelectObject(hDC, hPn);
			Ellipse(hDC, cx - clipR + 1, cy - r, cx + clipR - 1, cy);
			SelectObject(hDC, hOB);
			SelectObject(hDC, hOP);
			DeleteObject(hBr);
			DeleteObject(hPn);

			SelectClipRgn(hDC, NULL);
			DeleteObject(hClip);
		}
	}

	// Draw icon bitmap scaled to fit the smaller button
	if (hIcon)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hIcon);

		BITMAP bm = {0};
		GetObject(hIcon, sizeof(bm), &bm);

		int icoSz = BTN_SIZE * 5 / 8; // ~62% of button, fits inscribed circle
		int ix = cx - icoSz / 2;
		int iy = cy - icoSz / 2;

		// Use TransparentBlt to skip the background color
		TransparentBlt(hDC, ix, iy, icoSz, icoSz,
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


LRESULT CHUDButtons::OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&)
{
	// Prevent the HUD from stealing focus when the user clicks on a button.
	// This keeps the game window as the active/foreground window.
	return MA_NOACTIVATE;
}
