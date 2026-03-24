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

	// Cursors
	HCURSOR GetMuCursor()      const { return m_hMuCursor; }
	HCURSOR GetNormalCursor()  const { return m_hNormalCursor; }
	HCURSOR GetTextCursor()    const { return m_hTextCursor; }
	HCURSOR GetLinkCursor()    const { return m_hLinkCursor; }
	HCURSOR GetBusyCursor()    const { return m_hBusyCursor; }
	HCURSOR GetWibCursor()     const { return m_hWibCursor; }

	// Fonts
	HFONT GetTitleFont()       const { return m_hTitleFont; }
	HFONT GetTabFont()         const { return m_hTabFont; }
	HFONT GetSectionFont()     const { return m_hSectionFont; }
	HFONT GetBodyFont()        const { return m_hBodyFont; }
	HFONT GetSmallFont()       const { return m_hSmallFont; }

	// Brushes
	HBRUSH GetBgBrush()        const { return m_hBgBrush; }
	HBRUSH GetPanelBrush()     const { return m_hPanelBrush; }

	// Dark-gold MU Online S3E1 color scheme
	static COLORREF ClrWindowBg()     { return RGB(18, 15, 10); }
	static COLORREF ClrPanelBg()      { return RGB(25, 22, 16); }
	static COLORREF ClrFrameGold()    { return RGB(120, 95, 40); }
	static COLORREF ClrFrameBright()  { return RGB(195, 165, 70); }
	static COLORREF ClrTitleText()    { return RGB(210, 180, 70); }
	static COLORREF ClrBodyText()     { return RGB(200, 195, 180); }
	static COLORREF ClrActiveText()   { return RGB(255, 225, 130); }
	static COLORREF ClrDisabledText() { return RGB(100, 95, 80); }
	static COLORREF ClrCheckMark()    { return RGB(130, 190, 60); }
	static COLORREF ClrSeparator()    { return RGB(70, 58, 32); }
	static COLORREF ClrBtnBg()        { return RGB(40, 35, 22); }
	static COLORREF ClrBtnHover()     { return RGB(55, 48, 30); }
	static COLORREF ClrBtnPressed()   { return RGB(75, 60, 30); }
	static COLORREF ClrTabActive()    { return RGB(40, 35, 22); }
	static COLORREF ClrTabInactive()  { return RGB(22, 18, 12); }
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

	HCURSOR m_hMuCursor;
	HCURSOR m_hNormalCursor;
	HCURSOR m_hTextCursor;
	HCURSOR m_hLinkCursor;
	HCURSOR m_hBusyCursor;
	HCURSOR m_hWibCursor;

	HFONT m_hTitleFont, m_hTabFont, m_hSectionFont, m_hBodyFont, m_hSmallFont;
	HBRUSH m_hBgBrush, m_hPanelBrush;
};

#endif //__MuTheme_H
