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
	LRESULT OnNCPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCtlColor(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCtlColorListBox(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDrawItem(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClickedOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnClickedCancel(WORD, WORD, HWND, BOOL&);

private:
	void PopulateList();

	CMuTheme m_cTheme;
	std::vector<HistoryEntry> m_vHistory;

	HCURSOR m_hOldCursor;
	int m_iShowCursor;
	HBRUSH m_hListBrush;
};

#endif //__HistoryDialog_H
