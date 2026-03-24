#ifndef __HistoryDialog_H
#define __HistoryDialog_H

#pragma once

#include "resource.h"
#include <atlhost.h>
#include "MuTheme.h"
#include <vector>
#include <string>

/**
 * \brief Dialog displaying autoclicker pickup history.
 *        Shows timestamps and item names for items picked up
 *        while the autoclicker was running.
 */
class CHistoryDialog
	: public CAxDialogImpl<CHistoryDialog>
{
public:
	CHistoryDialog();
	virtual ~CHistoryDialog();

	enum { IDD = IDD_HISTORY_DLG };

	struct HistoryEntry
	{
		std::string sTime;
		std::string sItem;
	};

	void SetHistory(const std::vector<HistoryEntry>& vHistory);

BEGIN_MSG_MAP(CHistoryDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
	MESSAGE_HANDLER(WM_NCPAINT, OnNCPaint)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColor)
	MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColor)
	MESSAGE_HANDLER(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CHistoryDialog>)
END_MSG_MAP()

protected:
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShowWindow(UINT, WPARAM wParam, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		m_bLButtonDown = TRUE;
		UpdateCursorForLMBState();
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		m_bLButtonDown = FALSE;
		UpdateCursorForLMBState();
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&)
	{
		// Allow the dialog to be activated on click so that controls
		// (buttons, listbox) receive mouse input.
		return MA_ACTIVATE;
	}
	LRESULT OnNCPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCtlColor(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCtlColorListBox(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDrawItem(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClickedOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnClickedCancel(WORD, WORD, HWND, BOOL&);

private:
	void PopulateList();
	void UpdateCursorForLMBState()
	{
		HCURSOR hCursor = m_bLButtonDown ? m_cTheme.GetLinkCursor() : NULL;
		if (hCursor == NULL) hCursor = m_cTheme.GetNormalCursor();
		if (hCursor == NULL) hCursor = m_cTheme.GetMuCursor();
		if (hCursor == NULL) hCursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCursor);
	}

	CMuTheme m_cTheme;
	std::vector<HistoryEntry> m_vHistory;

	HCURSOR m_hOldCursor;
	int m_iShowCursor;
	BOOL m_bCursorShown;   // track whether we added cursor visibility
	BOOL m_bLButtonDown;   // track LMB state for cursor switching
	HBRUSH m_hListBrush;
	HBRUSH m_hTitleBrush;  // cached brush for OnNCPaint title bar
};

#endif //__HistoryDialog_H
