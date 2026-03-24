#ifndef __HUDButtons_H
#define __HUDButtons_H

#pragma once

#include <atlbase.h>
#include <atlwin.h>

// Messages sent from HUD buttons to the parent (CMuWindow)
#define WM_HUD_SETTINGS     WM_APP + 601
#define WM_HUD_STARTSTOP    WM_APP + 602
#define WM_HUD_HISTORY      WM_APP + 603

/**
 * \brief Floating HUD button bar overlaid on the game window.
 *        Three icon buttons: Settings (gear), Start/Stop, History.
 *        Created as an owned popup window so it stays on top of the game.
 */
class CHUDButtons
	: public CWindowImpl<CHUDButtons>
{
public:
	DECLARE_WND_CLASS(_T("MU_HUDButtons"))

	CHUDButtons();
	~CHUDButtons();

	BOOL Create(HWND hwndOwner, HINSTANCE hInstance);
	void Destroy();
	void Show();

	void SetClickerRunning(BOOL bRunning);
	void Reposition();

BEGIN_MSG_MAP(CHUDButtons)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
END_MSG_MAP()

protected:
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseLeave(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);

private:
	enum { BTN_COUNT = 3 };
	enum { BTN_SIZE = 16 };
	enum { BTN_SPACING = 2 };
	enum { BAR_PADDING = 1 };
	enum { REPOSITION_TIMER_ID = 2020 };

	int HitTest(int x, int y);
	void DrawButton(HDC hDC, int idx, HBITMAP hIcon, BOOL bHover, BOOL bPressed);
	RECT GetButtonRect(int idx);

	HWND m_hwndOwner;
	HINSTANCE m_hInstance;
	BOOL m_bClickerRunning;

	HBITMAP m_hIcoSettings;
	HBITMAP m_hIcoPlay;
	HBITMAP m_hIcoStop;
	HBITMAP m_hIcoHistory;

	int m_iHoverBtn;    // -1 = none
	int m_iPressedBtn;  // -1 = none
	BOOL m_bTracking;   // mouse tracking active
};

#endif //__HUDButtons_H
