#include "stdafx.h"
#include "MD5CalcDlg.h"
#include "MD5.h"


HWND CMD5CalcDlg::s_hwndDlg = 0;


/**
 * \brief 
 */
LRESULT CMD5CalcDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CMD5CalcDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	bHandled = TRUE;

	if (s_hwndDlg != 0)
	{
		SetForegroundWindow(s_hwndDlg);
		EndDialog(IDCANCEL);
	}
	else
	{
		s_hwndDlg = m_hWnd;
	}

	return 1;  // Let the system set the focus

}


/**
 * \brief 
 */
LRESULT CMD5CalcDlg::OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR szFileName[_MAX_PATH+1] = {0};

	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFilter = _T("All Files (*.*)\0*.*\0");
	ofn.nMaxFile = _MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_LONGNAMES;

	if (GetOpenFileName(&ofn))
	{
		SetDlgItemText(IDC_FILEPATH, ofn.lpstrFile);
		
		FILE* f = _tfopen(ofn.lpstrFile, _T("rb"));

		if (f)
		{
			MD5 md5;

			BYTE buff[4096] = {0};
			size_t ulRead = 0;
			
			while ((ulRead = fread(buff, 1, 4096, f)) != 0)
			{
				md5.update(buff, (MD5::size_type)ulRead);
			}

			md5.finalize();
			SetDlgItemText(IDC_MD5, CA2CT(md5.hexdigest().c_str()));

			fclose(f);
		}
		else
		{
			SetDlgItemText(IDC_MD5, _T("Cannot open file!"));
		}
	}
	
	return 0;
}


/**
 * \brief 
 */
LRESULT CMD5CalcDlg::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	s_hwndDlg = 0;
	EndDialog(IDCANCEL);
	return 0;
}

