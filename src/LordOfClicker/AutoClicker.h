#ifndef __AutoClicker_H
#define __AutoClicker_H

#pragma once

#include <set>

class CMuWindow;

/**
 * \brief Global ATL Module Class
 */
class CAutoClickerModule : public CAtlDllModuleT< CAutoClickerModule >
{
public:
	CAutoClickerModule() 
		: m_hMuWindow(0)
	{
	}

	BOOL InitClicker();
	void TermClicker();

protected:
	static BOOL CALLBACK MyEnumWndProc(HWND hWnd, LPARAM pData);

protected:
	std::set<int> m_vMuInstances;
	HWND m_hMuWindow;
};

#endif //__AutoClicker_H