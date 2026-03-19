// AboutBox.cpp : Implementation of CAboutBox

#include "stdafx.h"
#include "AboutBox.h"
#include "version.h"
#include "atlstr.h"

HWND CAboutBox::s_hwndAboutBox = 0;


/**
 * \brief 
 */
LRESULT CAboutBox::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CAboutBox>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	bHandled = TRUE;

	if (s_hwndAboutBox != 0)
	{
		SetForegroundWindow(s_hwndAboutBox);
		EndDialog(IDCANCEL);
	}
	else
	{
		SetDlgItemText(IDC_STATIC1, __SOFTWARE_VERSION_ABOUT);
		SetWindowText(CString("About ") + _T(__SOFTWARE_PRODUCT_NAME));
		SetDlgItemText(IDC_STATIC2, _T(__SOFTWARE_COPYRIGHT));
		
		s_hwndAboutBox = m_hWnd;
	}

	return 1;  // Let the system set the focus
}


/**
 * \brief 
 */
LRESULT CAboutBox::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	s_hwndAboutBox = 0;
	EndDialog(wID);
	return 0;
}


/**
 * \brief 
 */
LRESULT CAboutBox::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	s_hwndAboutBox = 0;
	EndDialog(wID);
	return 0;
}
