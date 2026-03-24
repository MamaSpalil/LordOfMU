#ifndef __MuInstanceManager_H
#define __MuInstanceManager_H

#pragma once

#include "aboutbox.h"
#include "MD5CalcDlg.h"
#include "version.h"
#include "atlstr.h"

#define SHELL_ICON_MESSAGE (WM_APP + 101)
#define WM_UPDATE_LICENSE_STATUS (WM_APP + 301)
#define WM_MUWINDOW_CONTROL (WM_APP + 303)
#define WM_LIC_THREAD_FINISHED (WM_APP + 102)


/**
 * \brief 
 */
class CMuInstanceManager
	: public CWindowImpl<CMuInstanceManager, CWindow, CWinTraits<WS_OVERLAPPED> >
{
public:
	CMuInstanceManager();
	virtual ~CMuInstanceManager();

	BOOL Init(bool fRunAsParasite = true);
	void Term();

#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__) || defined(__INCLUDE_ALL_STUFF__)
	DECLARE_WND_CLASS(_T("__MAC_MESSAGE_WND__"))
#else
	DECLARE_WND_CLASS(_T("__LOM_MESSAGE_WND__"))
#endif

BEGIN_MSG_MAP(CMuInstanceManager)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(SHELL_ICON_MESSAGE, OnShellIcon)
	MESSAGE_HANDLER(WM_MUWINDOW_CONTROL, OnMuWindowControl)
END_MSG_MAP()

protected:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShellIcon(UINT, WPARAM wParam, LPARAM lParam, BOOL&);
	LRESULT OnMuWindowControl(UINT, WPARAM wParam, LPARAM lParam, BOOL&);

	void PrepareDll(TCHAR* pszPath);

protected:
	TCHAR m_szLoaderPath[_MAX_PATH+1];

	HMODULE m_hHookDll;
	bool m_fServer;
	bool m_fProcessing;
	bool m_fParasite;
};



#endif //__MuInstanceManager_H