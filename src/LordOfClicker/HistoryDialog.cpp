#include "stdafx.h"
#include "HistoryDialog.h"


CHistoryDialog::CHistoryDialog()
{
	m_hOldCursor = NULL;
	m_iShowCursor = 0;
	m_bCursorShown = FALSE;
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

		// Show cursor (guard against double-show)
		if (!m_bCursorShown)
		{
			m_hOldCursor = SetCursor(LoadCursor(NULL, IDC_ARROW));

			// ShowCursor increments/decrements an internal counter.
			// Loop up to 100 times as a safety limit (MU Online hides the
			// cursor deeply; normal games need at most 2-3 increments).
			m_iShowCursor = 0;
			while (ShowCursor(TRUE) < 1 && m_iShowCursor < 100)
				++m_iShowCursor;

			m_bCursorShown = TRUE;
		}

		PopulateList();
	}
	else
	{
		// Restore cursor
		if (m_bCursorShown)
		{
			if (m_hOldCursor)
				SetCursor(m_hOldCursor);

			for (int i = m_iShowCursor; i >= 0; --i)
				ShowCursor(FALSE);

			m_iShowCursor = 0;
			m_bCursorShown = FALSE;
		}
	}

	return 0;
}


LRESULT CHistoryDialog::OnSetCursor(UINT, WPARAM, LPARAM, BOOL&)
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));
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
		BOOL bHover = FALSE;

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
