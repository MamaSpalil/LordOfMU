// AutoClicker.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "AutoClicker.h"
#include "MuWindow.h"
#include "MuWindowUtil.h"

HANDLE g_hInitThread = 0;


/**
 * \brief 
 */
BOOL CAutoClickerModule::InitClicker()
{
	m_hMuWindow = 0;
	m_vMuInstances.clear();

	EnumWindows(MyEnumWndProc, (LPARAM)this);

	if (!m_hMuWindow)
		return FALSE;

	int iInstanceNumber = 1;
	for (iInstanceNumber; iInstanceNumber < 32000; ++iInstanceNumber)
	{
		if (m_vMuInstances.find(iInstanceNumber) == m_vMuInstances.end())
			break;
	}

	return CMuWindow::Init(m_hMuWindow, iInstanceNumber);
}


/**
 * \brief 
 */
void CAutoClickerModule::TermClicker()
{
	CMuWindow::Term();
}


/**
 * \brief 
 */
BOOL CALLBACK CAutoClickerModule::MyEnumWndProc(HWND hWnd, LPARAM pData)
{
	CAutoClickerModule* pThis = (CAutoClickerModule*)pData;

	if (CMuWindowUtil::IsMuWindow(hWnd))
	{
		DWORD_PTR dwRes = 0;
		if (SendMessageTimeout(hWnd, WM_IS_CLICKER_INSTALLED, 0, 0, SMTO_BLOCK, 500, &dwRes) != 0)
		{
			pThis->m_vMuInstances.insert((int)dwRes);
		}

		DWORD dwProcId = 0;
		GetWindowThreadProcessId(hWnd, &dwProcId);

		if (dwProcId == GetCurrentProcessId())
		{
			pThis->m_hMuWindow = hWnd;
		}
	}

	return TRUE;
}

CAutoClickerModule _AtlModule;


/**
 * \brief 
 */
DWORD CALLBACK InitThread(void*)
{
	DWORD dwStart = GetTickCount();

	while (!_AtlModule.InitClicker())
	{
		Sleep(500);

		if (GetTickCount() - dwStart > 5000)
			break;
	}

	return 0;
}


/**
 * \brief DLL Entry Point
 */
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (!_AtlModule.DllMain(dwReason, lpReserved))
		return FALSE;
	
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInstance);

		g_hInitThread = CreateThread(0, 0, InitThread, 0, 0, 0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		_AtlModule.TermClicker();
	}

    return TRUE; 
}
