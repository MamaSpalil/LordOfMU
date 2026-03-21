#ifndef __MuTheme_H
#define __MuTheme_H

#pragma once

#include "resource.h"
#include <windows.h>

class CMuTheme
{
public:
	CMuTheme();
	~CMuTheme();

	BOOL Initialize(HINSTANCE hInstance);
	void Cleanup();

	// Backgrounds
	HBITMAP GetBgPanel()       const { return m_hBgPanel; }
	HBITMAP GetTitleBar()      const { return m_hTitleBar; }

	// Frame (9-patch)
	HBITMAP GetFrameTL()       const { return m_hFrameTL; }
	HBITMAP GetFrameTR()       const { return m_hFrameTR; }
	HBITMAP GetFrameBL()       const { return m_hFrameBL; }
	HBITMAP GetFrameBR()       const { return m_hFrameBR; }
	HBITMAP GetFrameTop()      const { return m_hFrameTop; }
	HBITMAP GetFrameBottom()   const { return m_hFrameBottom; }
	HBITMAP GetFrameLeft()     const { return m_hFrameLeft; }
	HBITMAP GetFrameRight()    const { return m_hFrameRight; }

	// Buttons
	HBITMAP GetBtnNormal()     const { return m_hBtnNormal; }
	HBITMAP GetBtnHover()      const { return m_hBtnHover; }
	HBITMAP GetBtnPressed()    const { return m_hBtnPressed; }

	// Tabs
	HBITMAP GetTabActive()     const { return m_hTabActive; }
	HBITMAP GetTabInactive()   const { return m_hTabInactive; }

	// Checkboxes
	HBITMAP GetChkUnchecked()     const { return m_hChkUnchecked; }
	HBITMAP GetChkChecked()       const { return m_hChkChecked; }
	HBITMAP GetChkUncheckedDis()  const { return m_hChkUncheckedDis; }
	HBITMAP GetChkCheckedDis()    const { return m_hChkCheckedDis; }

	// Fonts
	HFONT GetTitleFont()       const { return m_hTitleFont; }
	HFONT GetTabFont()         const { return m_hTabFont; }
	HFONT GetSectionFont()     const { return m_hSectionFont; }
	HFONT GetBodyFont()        const { return m_hBodyFont; }
	HFONT GetSmallFont()       const { return m_hSmallFont; }

	// Brushes
	HBRUSH GetBgBrush()        const { return m_hBgBrush; }
	HBRUSH GetPanelBrush()     const { return m_hPanelBrush; }

	// Colors
	static COLORREF ClrWindowBg()     { return RGB(15, 15, 25); }
	static COLORREF ClrPanelBg()      { return RGB(20, 22, 35); }
	static COLORREF ClrFrameGold()    { return RGB(85, 75, 45); }
	static COLORREF ClrFrameBright()  { return RGB(180, 155, 80); }
	static COLORREF ClrTitleText()    { return RGB(200, 170, 80); }
	static COLORREF ClrBodyText()     { return RGB(210, 210, 210); }
	static COLORREF ClrActiveText()   { return RGB(255, 230, 150); }
	static COLORREF ClrDisabledText() { return RGB(120, 120, 140); }
	static COLORREF ClrCheckMark()    { return RGB(120, 200, 80); }
	static COLORREF ClrSeparator()    { return RGB(60, 55, 45); }
	static COLORREF ClrBtnBg()        { return RGB(40, 38, 55); }
	static COLORREF ClrBtnHover()     { return RGB(55, 50, 70); }
	static COLORREF ClrBtnPressed()   { return RGB(70, 60, 45); }
	static COLORREF ClrTabActive()    { return RGB(35, 30, 50); }
	static COLORREF ClrTabInactive()  { return RGB(20, 18, 30); }
	static COLORREF ClrError()        { return RGB(200, 50, 50); }

	// GDI fallback drawing helpers
	static void DrawMuGradientBg(HDC hDC, const RECT& rc);
	static void DrawMuFrame(HDC hDC, const RECT& rc);
	static void DrawMuButton(HDC hDC, const RECT& rc, LPCTSTR text, BOOL pressed, BOOL hover);
	static void DrawSectionHeader(HDC hDC, int x, int y, int width, LPCTSTR text, HFONT hFont);

private:
	void LoadBitmapSafe(HBITMAP& hBmp, HINSTANCE hInst, int nID);

	HBITMAP m_hBgPanel, m_hTitleBar;
	HBITMAP m_hFrameTL, m_hFrameTR, m_hFrameBL, m_hFrameBR;
	HBITMAP m_hFrameTop, m_hFrameBottom, m_hFrameLeft, m_hFrameRight;
	HBITMAP m_hBtnNormal, m_hBtnHover, m_hBtnPressed;
	HBITMAP m_hTabActive, m_hTabInactive;
	HBITMAP m_hChkUnchecked, m_hChkChecked, m_hChkUncheckedDis, m_hChkCheckedDis;

	HFONT m_hTitleFont, m_hTabFont, m_hSectionFont, m_hBodyFont, m_hSmallFont;
	HBRUSH m_hBgBrush, m_hPanelBrush;
};

#endif //__MuTheme_H
