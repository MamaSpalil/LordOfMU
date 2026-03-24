#ifndef __AdvSettingsDialog_H
#define __AdvSettingsDialog_H

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>
#include "Settings.h"
#include "MuTheme.h"

class CAdvSettingsDialog
	: public CAxDialogImpl<CAdvSettingsDialog>
{
public:
	CAdvSettingsDialog(CClickerSettings& cSettings) 
		: m_cSettings(cSettings), m_hOldCursor(0), m_iShowCursor(0), m_fResult(FALSE) 
	{}

	enum { IDD = IDD_ADV_SETTINGS };

BEGIN_MSG_MAP(CAdvSettingsDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	MESSAGE_HANDLER(WM_NCPAINT, OnNCPaint)
	COMMAND_HANDLER(IDC_ADV_PICKUP, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_BLESS, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_SOUL, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_LIFE, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_JOG, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_JOC, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_CHAOS, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_EXL, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_ZEN, BN_CLICKED, ApplyState)
	COMMAND_HANDLER(IDC_CUSTOM1, BN_CLICKED, ApplyState)
	CHAIN_MSG_MAP(CAxDialogImpl<CAdvSettingsDialog>)
END_MSG_MAP()

protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowWindow(UINT, WPARAM wParam, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNCPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT ApplyState(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

protected:
	void InitValues();
	void Apply();

public:
	BOOL m_fResult;

protected:
	CClickerSettings& m_cSettings;
	CMuTheme m_cTheme;
	HCURSOR m_hOldCursor;
	int m_iShowCursor;
};

#endif //__AdvSettingsDialog_H