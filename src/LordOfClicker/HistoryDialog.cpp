#include "stdafx.h"
#include "HistoryDialog.h"
#include <commctrl.h>  // SetWindowSubclass


// -----------------------------------------------------------------------
// Button hover subclass: invalidates owner-draw buttons on mouse
// enter/leave so the parent OnDrawItem repaints with correct hover state.
// -----------------------------------------------------------------------
static LRESULT CALLBACK ButtonHoverSubclassProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR /*dwRefData*/)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		{
			TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hWnd, 0 };
			TrackMouseEvent(&tme);
			::InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_MOUSELEAVE:
		::InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, ButtonHoverSubclassProc, uIdSubclass);
		break;
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}


CHistoryDialog::CHistoryDialog()
{
	m_hOldCursor = NULL;
	m_iShowCursor = 0;
	m_bCursorShown = FALSE;
	m_bLButtonDown = FALSE;
	m_hListBrush = NULL;
	m_hTitleBrush = NULL;
}


CHistoryDialog::~CHistoryDialog()
{
	if (m_hListBrush)
		DeleteObject(m_hListBrush);
	if (m_hTitleBrush)
		DeleteObject(m_hTitleBrush);
}


void CHistoryDialog::SetHistory(const std::vector<HistoryEntry>& vHistory)
{
	m_vHistory = vHistory;

	if (IsWindow())
		PopulateList();
}


void CHistoryDialog::PopulateList()
{
	HWND hList = GetDlgItem(IDC_HISTORY_LIST);
	if (!hList) return;

	::SendMessage(hList, LB_RESETCONTENT, 0, 0);

	if (m_vHistory.empty())
	{
		::SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)_T("  No actions recorded"));
		return;
	}

	// Add items in reverse order (newest first)
	for (int i = (int)m_vHistory.size() - 1; i >= 0; --i)
	{
		const HistoryEntry& entry = m_vHistory[i];

		// Format: "[HH:MM:SS]  ItemName"
		TCHAR szLine[256] = {0};
		_sntprintf(szLine, _countof(szLine) - 1, _T("  [%S]  %S"),
			entry.sTime.c_str(), entry.sItem.c_str());
		szLine[_countof(szLine) - 1] = 0;
		::SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)szLine);
	}
}


LRESULT CHistoryDialog::OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	// Initialize theme
	m_cTheme.Initialize(_AtlBaseModule.GetModuleInstance());

	// Center on parent
	CenterWindow(GetParent());

	m_hListBrush = CreateSolidBrush(RGB(20, 17, 12));
	m_hTitleBrush = CreateSolidBrush(RGB(30, 25, 15));

	// Set list box font
	HWND hList = GetDlgItem(IDC_HISTORY_LIST);
	if (hList && m_cTheme.GetBodyFont())
	{
		::SendMessage(hList, WM_SETFONT, (WPARAM)m_cTheme.GetBodyFont(), TRUE);
	}

	// Set button fonts
	HWND hOK = GetDlgItem(IDOK);
	if (hOK && m_cTheme.GetTabFont())
		::SendMessage(hOK, WM_SETFONT, (WPARAM)m_cTheme.GetTabFont(), TRUE);

	// Subclass owner-draw buttons for hover tracking
	if (hOK)
		SetWindowSubclass(hOK, ButtonHoverSubclassProc, 100, 0);

	PopulateList();

	bHandled = TRUE;
	return 1;
}


LRESULT CHistoryDialog::OnShowWindow(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	if ((BOOL)wParam)
	{
		// Re-center over game window
		CenterWindow(GetParent());

		// Show cursor - snapshot current display count first
		HCURSOR hCursor = m_cTheme.GetNormalCursor();
		if (hCursor == NULL)
			hCursor = m_cTheme.GetMuCursor();
		if (hCursor == NULL)
			hCursor = LoadCursor(NULL, IDC_ARROW);
		m_hOldCursor = SetCursor(hCursor);
		int nBefore = ShowCursor(TRUE) - 1;  // undo the probe increment
		ShowCursor(FALSE);
		m_iShowCursor = nBefore;
		while (ShowCursor(TRUE) < 1);

		PopulateList();
	}
	else
	{
		// Restore cursor and display count to the snapshotted value
		if (m_hOldCursor)
			SetCursor(m_hOldCursor);

		while (ShowCursor(FALSE) > m_iShowCursor);
	}

	return 0;
}


LRESULT CHistoryDialog::OnSetCursor(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
	// Set the themed cursor for ALL areas of the dialog (client and
	// non-client).  Handling all hit tests prevents the message from
	// being forwarded to the owner window (game) through DefWindowProc,
	// where the game's WndProc could call SetCursor(NULL) and make the
	// cursor invisible when transitioning from the game window.
	HCURSOR hCursor = NULL;

	if (m_bLButtonDown)
	{
		hCursor = m_cTheme.GetLinkCursor();
	}

	// Fall back to normal themed cursor
	if (hCursor == NULL)
		hCursor = m_cTheme.GetNormalCursor();
	if (hCursor == NULL)
		hCursor = m_cTheme.GetMuCursor();
	if (hCursor == NULL)
		hCursor = LoadCursor(NULL, IDC_ARROW);

	// Ensure the cursor display counter is non-negative so the system
	// cursor is visible.  The game may call ShowCursor(FALSE) which
	// decrements the global counter below zero, hiding the cursor even
	// when a valid shape is set via SetCursor.
	int nCount = ShowCursor(TRUE);  // probe: returns count after increment
	if (nCount > 1)
		ShowCursor(FALSE);  // was already visible, undo the probe

	SetCursor(hCursor);
	bHandled = TRUE;
	return TRUE;
}


LRESULT CHistoryDialog::OnNCPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	// Custom non-client area painting for gold frame
	HDC hDC = GetWindowDC();
	if (!hDC) return 0;

	RECT rcWindow;
	GetWindowRect(&rcWindow);
	OffsetRect(&rcWindow, -rcWindow.left, -rcWindow.top);

	CMuTheme::DrawMuFrame(hDC, rcWindow);

	// Draw title bar
	RECT rcTitle = rcWindow;
	rcTitle.left += 4;
	rcTitle.top += 4;
	rcTitle.right -= 4;
	rcTitle.bottom = rcTitle.top + 20;

	HBRUSH hTitleBr = m_hTitleBrush ? m_hTitleBrush : (HBRUSH)GetStockObject(BLACK_BRUSH);
	FillRect(hDC, &rcTitle, hTitleBr);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, CMuTheme::ClrTitleText());
	if (m_cTheme.GetTitleFont())
		SelectObject(hDC, m_cTheme.GetTitleFont());
	DrawText(hDC, _T("Pickup History"), -1, &rcTitle,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	ReleaseDC(hDC);
	return 0;
}


LRESULT CHistoryDialog::OnEraseBkgnd(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	HDC hDC = (HDC)wParam;
	RECT rc;
	GetClientRect(&rc);
	CMuTheme::DrawMuGradientBg(hDC, rc);
	return 1;
}


LRESULT CHistoryDialog::OnCtlColor(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	HDC hDC = (HDC)wParam;
	SetTextColor(hDC, CMuTheme::ClrBodyText());
	SetBkColor(hDC, CMuTheme::ClrWindowBg());
	SetBkMode(hDC, TRANSPARENT);
	return (LRESULT)m_cTheme.GetBgBrush();
}


LRESULT CHistoryDialog::OnCtlColorListBox(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	HDC hDC = (HDC)wParam;
	SetTextColor(hDC, CMuTheme::ClrBodyText());
	SetBkColor(hDC, RGB(20, 17, 12));
	return (LRESULT)m_hListBrush;
}


LRESULT CHistoryDialog::OnDrawItem(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
	if (!lpDIS) { bHandled = FALSE; return 0; }

	// Handle owner-drawn buttons (OK/Close)
	if (lpDIS->CtlType == ODT_BUTTON)
	{
		BOOL bPressed = (lpDIS->itemState & ODS_SELECTED) != 0;

		POINT pt;
		GetCursorPos(&pt);
		RECT rcScreen;
		::GetWindowRect(lpDIS->hwndItem, &rcScreen);
		BOOL bHover = PtInRect(&rcScreen, pt);

		TCHAR szText[64] = {0};
		::GetWindowText(lpDIS->hwndItem, szText, 63);

		CMuTheme::DrawMuButton(lpDIS->hDC, lpDIS->rcItem, szText, bPressed, bHover);
		bHandled = TRUE;
		return TRUE;
	}

	bHandled = FALSE;
	return 0;
}


LRESULT CHistoryDialog::OnClickedOK(WORD, WORD, HWND, BOOL&)
{
	ShowWindow(SW_HIDE);
	return 0;
}


LRESULT CHistoryDialog::OnClickedCancel(WORD, WORD, HWND, BOOL&)
{
	ShowWindow(SW_HIDE);
	return 0;
}
