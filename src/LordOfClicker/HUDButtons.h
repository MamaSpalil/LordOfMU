#ifndef __HUDButtons_H
#define __HUDButtons_H

#pragma once

#include <atlbase.h>
#include <atlwin.h>

// Messages sent from HUD buttons to the parent (CMuWindow)
#define WM_HUD_SETTINGS     (WM_APP + 601)
#define WM_HUD_STARTSTOP    (WM_APP + 602)
#define WM_HUD_HISTORY      (WM_APP + 603)

/**
 * \brief HUD button bar shown as a topmost popup overlay on the game window.
 *        Three icon buttons: Settings (gear), Start/Stop, History.
 *        Created as WS_POPUP | WS_EX_TOPMOST so it renders above the
 *        DirectDraw/Direct3D surface of the game.  Visibility is managed
 *        explicitly: hidden on ALT+TAB / minimize, shown when the game
 *        regains focus.  A periodic timer keeps the overlay positioned
 *        correctly relative to the game window's client area.
 */
class CHUDButtons
	: public CWindowImpl<CHUDButtons>
{
public:
	DECLARE_WND_CLASS(_T("MU_HUDButtons"))

	CHUDButtons();
	~CHUDButtons();

	BOOL Create(HWND hwndParent, HINSTANCE hInstance);
	void Destroy();
	void Show();
	void Hide();
	void Reset();

	void SetClickerRunning(BOOL bRunning);
	void SetGameActive(BOOL bActive);
	void Reposition();

BEGIN_MSG_MAP(CHUDButtons)
	MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
	MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
END_MSG_MAP()

protected:
	LRESULT OnNCHitTest(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseLeave(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);

private:
	enum { BTN_COUNT = 3 };
	enum { BTN_SIZE = 24 };
	enum { BTN_SPACING = 3 };
	enum { BAR_PADDING = 2 };
	enum { TIMER_REPOSITION = 2020 };
	enum { TIMER_REPOSITION_INTERVAL = 200 };
	enum { HUD_OFFSET_X = 90 };
	enum { HUD_OFFSET_Y = 48 };

	int HitTest(int x, int y);
	void DrawButton(HDC hDC, int idx, HBITMAP hIcon, BOOL bHover, BOOL bPressed);
	RECT GetButtonRect(int idx);

	HWND m_hwndParent;
	HINSTANCE m_hInstance;
	BOOL m_bClickerRunning;
	BOOL m_bEnabled;       // TRUE after Show() called (character selected)
	BOOL m_bGameActive;    // TRUE when game window is foreground

	HBITMAP m_hIcoSettings;
	HBITMAP m_hIcoPlay;
	HBITMAP m_hIcoStop;
	HBITMAP m_hIcoHistory;

	int m_iHoverBtn;    // -1 = none
	int m_iPressedBtn;  // -1 = none
	BOOL m_bTracking;   // mouse tracking active
	BOOL m_bTimerActive; // reposition timer running
	HCURSOR m_hMuCursor; // MU-themed cursor for HUD buttons
	HCURSOR m_hLinkCursor; // Link/select cursor for HUD button hover
};

#endif //__HUDButtons_H
