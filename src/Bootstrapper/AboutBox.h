// AboutBox.h : Declaration of the CAboutBox

#pragma once

#include "resource.h"       // main symbols


/**
 * \brief 
 */
class CAboutBox : 
	public CAxDialogImpl<CAboutBox>
{
public:
	CAboutBox()
	{
	}

	virtual ~CAboutBox()
	{
	}

	enum { IDD = IDD_ABOUTBOX };

BEGIN_MSG_MAP(CAboutBox)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CAboutBox>)
END_MSG_MAP()

protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

protected:
	static HWND s_hwndAboutBox;
};


