#include "stdafx.h"
#include "MuTheme.h"

#pragma comment(lib, "msimg32.lib")  // for GradientFill

CMuTheme::CMuTheme()
{
	m_hBgPanel = NULL;
	m_hTitleBar = NULL;
	m_hFrameTL = NULL;
	m_hFrameTR = NULL;
	m_hFrameBL = NULL;
	m_hFrameBR = NULL;
	m_hFrameTop = NULL;
	m_hFrameBottom = NULL;
	m_hFrameLeft = NULL;
	m_hFrameRight = NULL;
	m_hBtnNormal = NULL;
	m_hBtnHover = NULL;
	m_hBtnPressed = NULL;
	m_hTabActive = NULL;
	m_hTabInactive = NULL;
	m_hChkUnchecked = NULL;
	m_hChkChecked = NULL;
	m_hChkUncheckedDis = NULL;
	m_hChkCheckedDis = NULL;
	m_hMuCursor = NULL;
	m_hTitleFont = NULL;
	m_hTabFont = NULL;
	m_hSectionFont = NULL;
	m_hBodyFont = NULL;
	m_hSmallFont = NULL;
	m_hBgBrush = NULL;
	m_hPanelBrush = NULL;
}

CMuTheme::~CMuTheme()
{
	Cleanup();
}

void CMuTheme::LoadBitmapSafe(HBITMAP& hBmp, HINSTANCE hInst, int nID)
{
	hBmp = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(nID), IMAGE_BITMAP, 0, 0, 0);
}

BOOL CMuTheme::Initialize(HINSTANCE hInst)
{
	// Load all bitmap textures
	LoadBitmapSafe(m_hBgPanel, hInst, IDB_BG_PANEL);
	LoadBitmapSafe(m_hTitleBar, hInst, IDB_TITLEBAR);
	LoadBitmapSafe(m_hFrameTL, hInst, IDB_FRAME_TL);
	LoadBitmapSafe(m_hFrameTR, hInst, IDB_FRAME_TR);
	LoadBitmapSafe(m_hFrameBL, hInst, IDB_FRAME_BL);
	LoadBitmapSafe(m_hFrameBR, hInst, IDB_FRAME_BR);
	LoadBitmapSafe(m_hFrameTop, hInst, IDB_FRAME_TOP);
	LoadBitmapSafe(m_hFrameBottom, hInst, IDB_FRAME_BOTTOM);
	LoadBitmapSafe(m_hFrameLeft, hInst, IDB_FRAME_LEFT);
	LoadBitmapSafe(m_hFrameRight, hInst, IDB_FRAME_RIGHT);
	LoadBitmapSafe(m_hBtnNormal, hInst, IDB_BTN_NORMAL);
	LoadBitmapSafe(m_hBtnHover, hInst, IDB_BTN_HOVER);
	LoadBitmapSafe(m_hBtnPressed, hInst, IDB_BTN_PRESSED);
	LoadBitmapSafe(m_hTabActive, hInst, IDB_TAB_ACTIVE);
	LoadBitmapSafe(m_hTabInactive, hInst, IDB_TAB_INACTIVE);
	LoadBitmapSafe(m_hChkUnchecked, hInst, IDB_CHK_UNCHECKED);
	LoadBitmapSafe(m_hChkChecked, hInst, IDB_CHK_CHECKED);
	LoadBitmapSafe(m_hChkUncheckedDis, hInst, IDB_CHK_UNCHECKED_DIS);
	LoadBitmapSafe(m_hChkCheckedDis, hInst, IDB_CHK_CHECKED_DIS);

	// Load MU Online game cursor
	m_hMuCursor = (HCURSOR)LoadImage(hInst, MAKEINTRESOURCE(IDC_MU_CURSOR),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);

	// Create fonts (Tahoma - standard MU Online font)
	m_hTitleFont = CreateFont(-15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));

	m_hTabFont = CreateFont(-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));

	m_hSectionFont = CreateFont(-11, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));

	m_hBodyFont = CreateFont(-11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));

	m_hSmallFont = CreateFont(-9, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));

	// Create brushes
	m_hBgBrush = CreateSolidBrush(ClrWindowBg());
	m_hPanelBrush = CreateSolidBrush(ClrPanelBg());

	return TRUE;
}

void CMuTheme::Cleanup()
{
	HBITMAP* bitmaps[] = {
		&m_hBgPanel, &m_hTitleBar,
		&m_hFrameTL, &m_hFrameTR, &m_hFrameBL, &m_hFrameBR,
		&m_hFrameTop, &m_hFrameBottom, &m_hFrameLeft, &m_hFrameRight,
		&m_hBtnNormal, &m_hBtnHover, &m_hBtnPressed,
		&m_hTabActive, &m_hTabInactive,
		&m_hChkUnchecked, &m_hChkChecked, &m_hChkUncheckedDis, &m_hChkCheckedDis,
		NULL
	};

	for (int i = 0; bitmaps[i] != NULL; ++i)
	{
		if (*bitmaps[i] != NULL)
		{
			DeleteObject(*bitmaps[i]);
			*bitmaps[i] = NULL;
		}
	}

	HFONT* fonts[] = {
		&m_hTitleFont, &m_hTabFont, &m_hSectionFont, &m_hBodyFont, &m_hSmallFont,
		NULL
	};

	for (int i = 0; fonts[i] != NULL; ++i)
	{
		if (*fonts[i] != NULL)
		{
			DeleteObject(*fonts[i]);
			*fonts[i] = NULL;
		}
	}

	if (m_hBgBrush != NULL) { DeleteObject(m_hBgBrush); m_hBgBrush = NULL; }
	if (m_hPanelBrush != NULL) { DeleteObject(m_hPanelBrush); m_hPanelBrush = NULL; }
	if (m_hMuCursor != NULL) { DestroyCursor(m_hMuCursor); m_hMuCursor = NULL; }
}


void CMuTheme::DrawMuGradientBg(HDC hDC, const RECT& rc)
{
	// Dark-gold vertical gradient from top RGB(18,15,10) to bottom RGB(25,22,16)
	// Uses GradientFill API for efficient hardware-accelerated rendering.
	TRIVERTEX vtx[2];
	vtx[0].x = rc.left;
	vtx[0].y = rc.top;
	vtx[0].Red   = 18 << 8;
	vtx[0].Green  = 15 << 8;
	vtx[0].Blue   = 10 << 8;
	vtx[0].Alpha  = 0;

	vtx[1].x = rc.right;
	vtx[1].y = rc.bottom;
	vtx[1].Red   = 25 << 8;
	vtx[1].Green  = 22 << 8;
	vtx[1].Blue   = 16 << 8;
	vtx[1].Alpha  = 0;

	GRADIENT_RECT gr = { 0, 1 };
	GradientFill(hDC, vtx, 2, &gr, 1, GRADIENT_FILL_RECT_V);
}


void CMuTheme::DrawMuFrame(HDC hDC, const RECT& rc)
{
	// Outer dark border
	HPEN hDarkPen = CreatePen(PS_SOLID, 1, RGB(50, 42, 22));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hDarkPen);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
	Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);

	// Middle gold border
	HPEN hGoldPen = CreatePen(PS_SOLID, 2, RGB(120, 95, 40));
	SelectObject(hDC, hGoldPen);
	Rectangle(hDC, rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1);

	// Inner bright highlight
	HPEN hBrightPen = CreatePen(PS_SOLID, 1, RGB(155, 130, 55));
	SelectObject(hDC, hBrightPen);
	Rectangle(hDC, rc.left + 3, rc.top + 3, rc.right - 3, rc.bottom - 3);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBr);
	DeleteObject(hDarkPen);
	DeleteObject(hGoldPen);
	DeleteObject(hBrightPen);
}


void CMuTheme::DrawMuButton(HDC hDC, const RECT& rc, LPCTSTR text, BOOL pressed, BOOL hover)
{
	COLORREF clrBg = pressed ? ClrBtnPressed() :
					 hover ? ClrBtnHover() : ClrBtnBg();

	// Button background
	HBRUSH hBr = CreateSolidBrush(clrBg);
	FillRect(hDC, &rc, hBr);
	DeleteObject(hBr);

	// Button border
	HPEN hPen = CreatePen(PS_SOLID, 1, pressed ? RGB(155, 130, 55) : ClrFrameGold());
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
	Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBr);
	DeleteObject(hPen);

	// Button text
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, ClrActiveText());
	RECT rcText = rc;
	DrawText(hDC, text, -1, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


void CMuTheme::DrawSectionHeader(HDC hDC, int x, int y, int width, LPCTSTR text, HFONT hFont)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, ClrFrameGold());
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	// Left separator line
	MoveToEx(hDC, x, y + 6, NULL);
	LineTo(hDC, x + 5, y + 6);

	// Section header text
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, ClrTitleText());
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	SIZE sz;
	GetTextExtentPoint32(hDC, text, (int)_tcslen(text), &sz);
	TextOut(hDC, x + 8, y, text, (int)_tcslen(text));

	// Right separator line
	MoveToEx(hDC, x + 12 + sz.cx, y + 6, NULL);
	LineTo(hDC, x + width, y + 6);

	SelectObject(hDC, hOldFont);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
