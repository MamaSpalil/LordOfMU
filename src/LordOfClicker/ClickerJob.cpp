#include "stdafx.h"
#include "ClickerJob.h"
#include "MuWindow.h"
#include "CommonPackets.h"
#include "EncDec.h"
#include "ClickerLogger.h"


/**
 * \brief 
 */
CClickerJob::CClickerJob(HWND inHwnd, ULONG ulVersion, const ClickerSettings& inSettings)
	: m_cHudParams(inHwnd, ulVersion), m_ulVersion(ulVersion)
{
	m_hWnd = inHwnd;
	memcpy(&m_tSettings, &inSettings, sizeof(ClickerSettings));

	m_hThread = 0;
	m_hStopEvent = CreateEvent(0, TRUE, 0, 0);
}


/**
 * \brief 
 */
CClickerJob::~CClickerJob()
{
	if (m_hThread)
	{
		DWORD dwStartWait = GetTickCount();

		while (WaitForSingleObject(m_hThread, 10) == WAIT_TIMEOUT)
		{
			MSG msg = {0};
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) != 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if ((int)GetTickCount() - (int)dwStartWait > 7000)
			{
				TerminateThread(m_hThread, 0);
				break;
			}
		}

		CloseHandle(m_hThread);
	}

	if (m_hStopEvent)
		CloseHandle(m_hStopEvent);
}


/**
 * \brief 
 */
BOOL CClickerJob::Start()
{
	if (m_hThread != 0)
		return FALSE; // already started

	DWORD dwId = 0;
	m_hThread = CreateThread(0, 0, ThreadProc, (void*)this, 0, &dwId);
	
	if (!m_hThread || m_hThread == INVALID_HANDLE_VALUE)
		return FALSE;

	return TRUE;
}


/**
 * \brief 
 */
void CClickerJob::Stop()
{
	if (!m_hThread)
		return; //not started

	SetEvent(m_hStopEvent);
	Sleep(0);
}


/**
 * \brief 
 */
DWORD WINAPI CClickerJob::ThreadProc(void* pData)
{
	CClickerJob* pThis = (CClickerJob*)pData;
	
	if (!pThis)
		return (DWORD)-1;

	pThis->InitClicker();

	while (WaitForSingleObject(pThis->m_hStopEvent, 0) == WAIT_TIMEOUT)
	{
		pThis->DoClicker();
		Sleep(0);
	}

	pThis->TermClicker();

	PostMessage(pThis->m_hWnd, WM_CLICKER_JOB_FINISHED, 0, 0);
	return 0;
}


/**
 * \brief 
 */
void CClickerJob::InitClicker()
{
	WriteClickerLogFmt("CLICKER", "=== Auto-clicker starting ===");

	// Always enable Clicker Debug Mode when autoclicker starts (F5).
	// This opens a dedicated debug console window for tracking all clicker actions.
	if (!CDebugMode::IsEnabled())
	{
		CDebugMode::SetEnabled(true);
		CDebugMode::SaveToRegistry();
		WriteClickerLogFmt("CLICKER", "Clicker Debug Mode auto-enabled on autoclicker start");
	}

	// Set console title to distinguish clicker debug window
	HWND hConsole = GetConsoleWindow();
	if (hConsole != NULL)
		SetConsoleTitleA("LordOfMU - Clicker Debug Console");

	// Propagate debug mode to the DLL so packet logging and debug console work
	WriteClickerLog("Debug mode active - propagating to DLL");
	CMuWindow::GetInstance()->SayToServer("//debugmode on");

	// Log all settings for diagnostics
	WriteClickerLogFmt("CLICKER", "Settings: fAutoPick=%d fAutoRepair=%d fAutoLife=%d fAutoReOff=%d",
		(int)m_tSettings.all.fAutoPick, (int)m_tSettings.all.fAutoRepair,
		(int)m_tSettings.all.fAutoLife, (int)m_tSettings.all.fAutoReOff);
	WriteClickerLogFmt("CLICKER", "Settings: dwPickTime=%d dwRepairTime=%d dwHealTime=%d dwClass=%d",
		(int)m_tSettings.all.dwPickTime, (int)m_tSettings.all.dwRepairTime,
		(int)m_tSettings.all.dwHealTime, (int)m_tSettings.all.dwClass);
	WriteClickerLogFmt("CLICKER", "Settings: fAdvAutoPick=%d fExitAtLvl400=%d fAntiAFKProtect=%d fPickRunMode=%d",
		(int)m_tSettings.all.fAdvAutoPick, (int)m_tSettings.all.fExitAtLvl400,
		(int)m_tSettings.all.fAntiAFKProtect, (int)m_tSettings.all.fPickRunMode);
	WriteClickerLogFmt("CLICKER", "Settings: fAdvPickBless=%d fAdvPickSoul=%d fAdvPickChaos=%d fAdvPickLife=%d",
		(int)m_tSettings.all.fAdvPickBless, (int)m_tSettings.all.fAdvPickSoul,
		(int)m_tSettings.all.fAdvPickChaos, (int)m_tSettings.all.fAdvPickLife);
	WriteClickerLogFmt("CLICKER", "Settings: fAdvPickCreation=%d fAdvPickGuardian=%d fAdvPickExl=%d fAdvPickZen=%d",
		(int)m_tSettings.all.fAdvPickCreation, (int)m_tSettings.all.fAdvPickGuardian,
		(int)m_tSettings.all.fAdvPickExl, (int)m_tSettings.all.fAdvPickZen);
	WriteClickerLogFmt("CLICKER", "Settings: fAdvPickCustom=%d fAdvPickCustomMove=%d wPickCustomCode=0x%04X",
		(int)m_tSettings.all.fAdvPickCustom, (int)m_tSettings.all.fAdvPickCustomMove,
		(int)m_tSettings.all.wPickCustomCode);

	if (m_tSettings.all.fAutoReOff)
	{
		WriteClickerLog("Sending /request off");
		CMuWindow::GetInstance()->SayToServer("/request off");
	}

	if (m_tSettings.all.fAntiAFKProtect & 1)
	{
		WriteClickerLog("Anti-AFK autosay enabled");
		CMuWindow::GetInstance()->SayToServer("//autosay on");
	}
	else
	{
		CMuWindow::GetInstance()->SayToServer("//autosay off");
	}

	if (m_tSettings.all.fAntiAFKProtect & 2)
	{
		WriteClickerLog("Stealth option: susp_zen_pick enabled");
		CMuWindow::GetInstance()->SayToServer("//set_stealth_opt susp_zen_pick on");
	}
	else
	{
		CMuWindow::GetInstance()->SayToServer("//set_stealth_opt susp_zen_pick off");
	}

	if (m_tSettings.all.fAntiAFKProtect & 4)
	{
		WriteClickerLog("Stealth option: susp_move_pick enabled");
		CMuWindow::GetInstance()->SayToServer("//set_stealth_opt susp_move_pick on");
	}
	else
	{
		CMuWindow::GetInstance()->SayToServer("//set_stealth_opt susp_move_pick off");
	}

	if (m_tSettings.all.fAntiAFKProtect & 8)
	{
		WriteClickerLog("Stealth option: susp_pick enabled");
		CMuWindow::GetInstance()->SayToServer("//set_stealth_opt susp_pick on");
	}
	else
	{
		CMuWindow::GetInstance()->SayToServer("//set_stealth_opt susp_pick off");
	}

	if (m_tSettings.all.fAdvAutoPick)
	{
		WriteClickerLog("Advanced auto-pickup enabled - setting options");
		SetAdvancedAutopickOptions();

		if (CDebugMode::IsEnabled())
			WriteClickerLogFmt("CLICKER", "Sending //autopick on to DLL");

		CMuWindow::GetInstance()->SayToServer("//autopick on");

		// Send character class for auto run-mode determination
		{
			char szMsg[256] = {0};
			_snprintf(szMsg, 255, "//pickclass %d", m_tSettings.all.dwClass);

			if (CDebugMode::IsEnabled())
				WriteClickerLogFmt("CLICKER", "Sending %s to DLL", szMsg);

			CMuWindow::GetInstance()->SayToServer(szMsg);
		}

		// Enable auto run-mode: walk/run determined by boots enchantment level
		WriteClickerLog("Auto run mode enabled (boots level detection)");
		CMuWindow::GetInstance()->SayToServer("//pickautorunmode on");

		if (m_tSettings.all.fPickRunMode)
		{
			WriteClickerLog("Pick-up run mode enabled");
			CMuWindow::GetInstance()->SayToServer("//pickrunmode on");
		}
		else
		{
			if (CDebugMode::IsEnabled())
				WriteClickerLogFmt("CLICKER", "Pick-up run mode disabled (fPickRunMode=%d)", (int)m_tSettings.all.fPickRunMode);

			CMuWindow::GetInstance()->SayToServer("//pickrunmode off");
		}

		// Enable pickup debug logging when debug mode is active
		if (CDebugMode::IsEnabled())
		{
			WriteClickerLog("Pick-up debug logging enabled");
			CMuWindow::GetInstance()->SayToServer("//pickdebug on");
		}
	}
	else
	{
		WriteClickerLogFmt("CLICKER", "Advanced auto-pickup DISABLED (fAdvAutoPick=%d), skipping pickup commands",
			(int)m_tSettings.all.fAdvAutoPick);
	}

	if (m_tSettings.all.fExitAtLvl400)
	{
		WriteClickerLog("Exit at level 400 enabled");
		CMuWindow::GetInstance()->SayToServer("//exit400 on");
	}

	GetCursorPos(&m_ptMouseStartPos);
	ScreenToClient(m_hWnd, &m_ptMouseStartPos);

	RECT rc = {0};
	GetClientRect(m_hWnd, &rc);

	if (m_ptMouseStartPos.x < 0 || m_ptMouseStartPos.x >= rc.right)
		m_ptMouseStartPos.x = rc.right/2;

	if (m_ptMouseStartPos.y < 0 || m_ptMouseStartPos.y >= rc.bottom)
		m_ptMouseStartPos.y = rc.bottom/2;


	m_dwHealTicks = GetTickCount();
	m_dwPickUpTicks = GetTickCount();
	m_dwRepairTicks = GetTickCount();
	m_dwClickTicks = GetTickCount() - RCLICK_TIMEOUT - 10;
}


/**
 * \brief 
 */
void CClickerJob::TermClicker()
{
	WriteClickerLogFmt("CLICKER", "=== Auto-clicker stopping ===");

	if (m_tSettings.all.fAntiAFKProtect)
	{
		WriteClickerLog("Anti-AFK autosay disabled");
		CMuWindow::GetInstance()->SayToServer("//autosay off");
	}

	if (m_tSettings.all.fAutoReOff)
	{
		WriteClickerLog("Sending /request on");
		CMuWindow::GetInstance()->SayToServer("/request on");
	}

	if (m_tSettings.all.fAdvAutoPick)
	{
		WriteClickerLog("Advanced auto-pickup disabled");

		if (CDebugMode::IsEnabled())
			WriteClickerLogFmt("CLICKER", "Sending //pickdebug off and //autopick off to DLL");

		CMuWindow::GetInstance()->SayToServer("//pickdebug off");
		CMuWindow::GetInstance()->SayToServer("//autopick off");
	}

	// Keep debug mode active in DLL after clicker stops (debug mode is always on)
	WriteClickerLogFmt("CLICKER", "Debug mode remains active in DLL after clicker stop");

	// Restore console title from clicker-specific to general debug
	HWND hConsole = GetConsoleWindow();
	if (hConsole != NULL)
		SetConsoleTitleA("LordOfMU - Debug Console");

	if (m_tSettings.all.fExitAtLvl400)
	{
		WriteClickerLog("Exit at level 400 disabled");
		CMuWindow::GetInstance()->SayToServer("//exit400 off");
	}

	if (IsWindow(m_hWnd))
		SendMessage(m_hWnd, WM_RBUTTONUP, (WPARAM)0, (LPARAM)MAKELONG(m_ptMouseStartPos.x, m_ptMouseStartPos.y));

	WriteClickerLogFmt("CLICKER", "=== Auto-clicker stopped ===");
}


/**
 * \brief 
 */
void CClickerJob::DoClicker()
{
	DWORD dwTick = GetTickCount();

	if ((int)dwTick - (int)m_dwClickTicks > RCLICK_TIMEOUT)
	{
		int x = m_cHudParams.x0;
		int y = m_cHudParams.y0;

		if (IsWindow(m_hWnd))
		{
			if (CDebugMode::IsEnabled())
				WriteClickerLogFmt("CLICKER", "Attack: right-click at (%d,%d)", x, y);

			SendMessage(m_hWnd, WM_CLICKER_JOB_RBUTTONUP, (WPARAM)0, (LPARAM)MAKELONG(x, y));
			SendMessage(m_hWnd, WM_CLICKER_JOB_RBUTTONDOWN, (WPARAM)(MK_RBUTTON | MK_SHIFT), (LPARAM)MAKELONG(x, y));
		}

		ReleaseCapture();
		ClipCursor(0);

		m_dwClickTicks = dwTick;
	}

	if ((int)dwTick - (int)m_dwHealTicks > (int)m_tSettings.all.dwHealTime)
	{
		if (m_tSettings.all.fAutoLife)
		{
			if (CDebugMode::IsEnabled())
				WriteClickerLogFmt("CLICKER", "Auto-heal triggered (interval=%d ms)", (int)m_tSettings.all.dwHealTime);

			DoHeal();
		}

		m_dwHealTicks = dwTick;
	}

	if ((int)dwTick - (int)m_dwPickUpTicks > (int)m_tSettings.all.dwPickTime)
	{
		// SPACE key pickup (DoPickUp) should only trigger for basic auto-pick mode.
		// When Advanced Pick-up (fAdvAutoPick) is enabled, the DLL handles item pickup
		// via packet injection (//autopick on) — pressing SPACE would interfere by
		// picking up unfiltered items and conflicting with DLL movement commands.
		if (m_tSettings.all.fAutoPick)
		{
			if (CDebugMode::IsEnabled())
				WriteClickerLogFmt("CLICKER", "Auto-pickup SPACE triggered (interval=%d ms, fAutoPick=%d)",
					(int)m_tSettings.all.dwPickTime, (int)m_tSettings.all.fAutoPick);

			DoPickUp();
		}

		m_dwPickUpTicks = dwTick;
	}

	if ((int)dwTick - (int)m_dwRepairTicks > (int)m_tSettings.all.dwRepairTime)
	{
		if (m_tSettings.all.fAutoRepair)
		{
			if (CDebugMode::IsEnabled())
				WriteClickerLogFmt("CLICKER", "Auto-repair triggered (interval=%d ms)", (int)m_tSettings.all.dwRepairTime);

			DoRepair();
		}

		m_dwRepairTicks = dwTick;
	}
}


/**
 * \brief 
 */
void CClickerJob::DoHeal()
{
	WriteClickerLog("Auto-heal triggered");
	CKeySender::SendAsync('Q');
}


/**
 * \brief 
 */
void CClickerJob::DoPickUp()
{
	WriteClickerLog("Auto-pickup triggered");
	CKeySender::SendAsync(VK_SPACE);
}


/**
 * \brief 
 */
void CClickerJob::DoRepair()
{
	WriteClickerLog("Auto-repair triggered");
	CKeySender::PressKey(VK_LSHIFT);

	if (!CKeySender::SendAsync('V', TRUE, 3000))
		return;

	Sleep(200);

	if (IsWindow(m_hWnd))
	{
		SendMessage(m_hWnd, WM_CLICKER_JOB_MOUSEMOVE, (WPARAM)0, (LPARAM)MAKELONG(m_cHudParams.width, 0));
		Sleep(50);
		CKeySender::SendAsync(VK_LBUTTON, TRUE, 5000);
		SendMessage(m_hWnd, WM_CLICKER_JOB_LBUTTONUP, (WPARAM)0, (LPARAM)MAKELONG(m_cHudParams.width, 0));
		Sleep(50);
	}

	char chRep = 'R';

	if (m_ulVersion >= 105)
	{
		chRep = 'L';
	}

	if (!CKeySender::SendAsync(chRep, TRUE, 3000))
		return;

	Sleep(500);

	POINTF* pCoords = GetRepairCoords();
	
	for (int i=0; pCoords[i].x != 0.0f; ++i)
	{
		int x = (int)(pCoords[i].x*(float)m_cHudParams.width);
		int y = (int)(pCoords[i].y*(float)m_cHudParams.height);

		if (m_ulVersion >= 105)
		{
			x += (int)((4.0f/1024.0f)*(float)m_cHudParams.width);
			y -= (int)((20.0f/768.0f)*(float)m_cHudParams.height);
		}

		if (IsWindow(m_hWnd))
		{
			SendMessage(m_hWnd, WM_CLICKER_JOB_MOUSEMOVE, (WPARAM)0, (LPARAM)MAKELONG(x, y));
			SendMessage(m_hWnd, WM_CLICKER_JOB_LBUTTONDOWN, (WPARAM)(MK_LBUTTON | MK_SHIFT), (LPARAM)MAKELONG(x, y));
		}

		CKeySender::SendAsync(VK_LBUTTON, TRUE, 1200);
		Sleep(pCoords[i].dwWait);

		if (IsWindow(m_hWnd))
		{
			SendMessage(m_hWnd, WM_CLICKER_JOB_LBUTTONUP, (WPARAM)0, (LPARAM)MAKELONG(x, y));
		}
	}

	CKeySender::ReleaseKey(VK_LSHIFT);
	Sleep(1000);
	CKeySender::SendAsync('V', TRUE, 3000);
	Sleep(2000);
	SendMessage(m_hWnd, WM_CLICKER_JOB_LBUTTONUP, (WPARAM)0, (LPARAM)0);
}


/**
 * \brief 
 */
void CClickerJob::SetAdvancedAutopickOptions()
{
	if (CDebugMode::IsEnabled())
		WriteClickerLogFmt("CLICKER", "SetAdvancedAutopickOptions: sending all pickup options to DLL");

	SetAutopickOption("bless", m_tSettings.all.fAdvPickBless, m_tSettings.all.fAdvPickBlessMove);
	SetAutopickOption("soul", m_tSettings.all.fAdvPickSoul, m_tSettings.all.fAdvPickSoulMove);
	SetAutopickOption("chaos", m_tSettings.all.fAdvPickChaos, m_tSettings.all.fAdvPickChaosMove);
	SetAutopickOption("jol", m_tSettings.all.fAdvPickLife, m_tSettings.all.fAdvPickLifeMove);
	SetAutopickOption("joc", m_tSettings.all.fAdvPickCreation, m_tSettings.all.fAdvPickCreationMove);
	SetAutopickOption("jog", m_tSettings.all.fAdvPickGuardian, m_tSettings.all.fAdvPickGuardianMove);
	SetAutopickOption("exl", m_tSettings.all.fAdvPickExl, m_tSettings.all.fAdvPickExlMove);
	SetAutopickOption("zen", m_tSettings.all.fAdvPickZen, m_tSettings.all.fAdvPickZenMove);
	SetAutopickOption("custom", m_tSettings.all.fAdvPickCustom, m_tSettings.all.fAdvPickCustomMove);

	if (CDebugMode::IsEnabled())
		WriteClickerLogFmt("CLICKER", "Sending //pick clear to DLL");

	CMuWindow::GetInstance()->SayToServer("//pick clear");

	if (m_tSettings.all.fAdvPickCustom && m_tSettings.all.wPickCustomCode != 0)
	{
		char szMsg[256] = {0};
		_snprintf(szMsg, 255, "//pick %d %d", HIBYTE(m_tSettings.all.wPickCustomCode), LOBYTE(m_tSettings.all.wPickCustomCode));

		if (CDebugMode::IsEnabled())
			WriteClickerLogFmt("CLICKER", "Sending custom pick command: %s", szMsg);

		CMuWindow::GetInstance()->SayToServer(szMsg);
	}

	if (CDebugMode::IsEnabled())
		WriteClickerLogFmt("CLICKER", "SetAdvancedAutopickOptions: all pickup options sent");
}


/**
 * \brief 
 */
void CClickerJob::SetAutopickOption(const char* opt, BOOL fPick, BOOL fMove)
{
	char szMsg[256] = {0};
	_snprintf(szMsg, 255, "//set_pick_opt %s %s %s", opt, fPick ? "on" : "off", fMove ? "on" : "off");

	if (CDebugMode::IsEnabled())
		WriteClickerLogFmt("CLICKER", "Sending to DLL: %s", szMsg);

	CMuWindow::GetInstance()->SayToServer(szMsg);
}
