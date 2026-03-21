#ifndef __UnifiedSettingsDlg_H
#define __UnifiedSettingsDlg_H

#pragma once

#include "resource.h"
#include <atlhost.h>
#include <commctrl.h>
#include "Settings.h"
#include "MuTheme.h"
#include "DarkLordSettings.h"
#include "EElfSettings.h"
#include "AElfSettings.h"
#include "BKSettings.h"
#include "MGSettings.h"
#include "SMSettings.h"

class CUnifiedSettingsDlg
	: public CAxDialogImpl<CUnifiedSettingsDlg>
{
public:
	CUnifiedSettingsDlg(CClickerSettings& cSettings);
	virtual ~CUnifiedSettingsDlg();

	enum { IDD = IDD_UNIFIED_SETTINGS };

BEGIN_MSG_MAP(CUnifiedSettingsDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	MESSAGE_HANDLER(WM_NCPAINT, OnNCPaint)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorBtn)
	MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	NOTIFY_HANDLER(IDC_SETTINGS_TAB, TCN_SELCHANGE, OnTabSelChange)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	// General tab handlers (forwarded from child dialog)
	COMMAND_HANDLER(IDC_AUTOHEAL, BN_CLICKED, OnCheckAutoHeal)
	COMMAND_HANDLER(IDC_STOPPICK, BN_CLICKED, OnCheckStopPick)
	// Class tab handlers (forwarded from child dialog)
	COMMAND_HANDLER(IDC_CHARCLASS, CBN_SELENDOK, OnSelectCharClass)
	// Pickup tab handlers (forwarded from child dialog)
	COMMAND_HANDLER(IDC_ADV_PICKUP, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_BLESS, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_SOUL, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_LIFE, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_JOG, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_JOC, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_CHAOS, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_EXL, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_ZEN, BN_CLICKED, OnPickupStateChange)
	COMMAND_HANDLER(IDC_CUSTOM1, BN_CLICKED, OnPickupStateChange)
	CHAIN_MSG_MAP(CAxDialogImpl<CUnifiedSettingsDlg>)
END_MSG_MAP()

protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowWindow(UINT, WPARAM wParam, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNCPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEraseBkgnd(UINT, WPARAM wParam, LPARAM, BOOL& bHandled);
	LRESULT OnCtlColorStatic(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCtlColorBtn(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCtlColorDlg(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCtlColorEdit(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTabSelChange(int, LPNMHDR pNMHDR, BOOL&);
	LRESULT OnClickedOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnClickedCancel(WORD, WORD, HWND, BOOL&);
	LRESULT OnCheckAutoHeal(WORD, WORD, HWND, BOOL&);
	LRESULT OnCheckStopPick(WORD, WORD, HWND, BOOL&);
	LRESULT OnSelectCharClass(WORD, WORD, HWND, BOOL&);
	LRESULT OnPickupStateChange(WORD, WORD, HWND, BOOL&);

protected:
	void ShowTab(int nTab);
	void InitGeneralValues();
	void InitClassValues();
	void InitPickupValues();
	void ApplyGeneral();
	void ApplyClass();
	void ApplyPickup();
	void ApplyPickupState();

	void ThemeTabControl(HWND hwndTab);
	void SubclassCheckboxes(HWND hwndParent);
	void SubclassSeparators(HWND hwndParent);
	void DrawThemedTab(LPDRAWITEMSTRUCT lpDIS);

	static INT_PTR CALLBACK TabPageDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK TabSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
public:
	static LRESULT CALLBACK CheckboxSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static LRESULT CALLBACK SeparatorSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

protected:
	CClickerSettings& m_cSettings;
	CMuTheme m_cTheme;

	HCURSOR m_hOldCursor;
	int m_iShowCursor;
	int m_nCurrentTab;

	// Brushes for edit controls
	HBRUSH m_hEditBrush;

	// Child dialogs for class panels (reused from existing code)
	CDarkLordSettings m_cDarkLordSettings;
	CEElfSettings     m_cEElfSettings;
	CAElfSettings     m_cAElfSettings;
	CBKSettings       m_cBKSettings;
	CMGSettings       m_cMGSettings;
	CSMSettings       m_cSMSettings;

	// Tab child dialog page HWNDs
	HWND m_hwndTabGeneral;
	HWND m_hwndTabClass;
	HWND m_hwndTabPickup;
};

#endif //__UnifiedSettingsDlg_H
