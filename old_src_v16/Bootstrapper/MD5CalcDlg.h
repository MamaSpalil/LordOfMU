#ifndef __MD5CalcDlg_H
#define __MD5CalcDlg_H

#pragma once


#include "resource.h"

/**
 * \brief 
 */
class CMD5CalcDlg
	: public CAxDialogImpl<CMD5CalcDlg>
{
public:
	enum { IDD = IDD_MD5CALC };

	BEGIN_MSG_MAP(CMD5CalcDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BROWSE, BN_CLICKED, OnClickedBrowse)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
		CHAIN_MSG_MAP(CAxDialogImpl<CMD5CalcDlg>)
	END_MSG_MAP()

protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

protected:
	static HWND s_hwndDlg;
};


#endif // __MD5CalcDlg_H