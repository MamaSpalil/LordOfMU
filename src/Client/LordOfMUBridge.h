#pragma once
/**
 * \file  LordOfMUBridge.h
 * \brief Cross-DLL communication bridge between the game Client DLL and
 *        the LordOfMU AutoClicker DLL (LordOfClicker).
 *
 * The LordOfMU autoclicker runs as a separate DLL injected into the same
 * process.  It subclasses the game's main HWND and listens for custom
 * window messages (WM_APP + xxx) to start/stop the clicker, open settings,
 * and show pickup history.  Keyboard shortcuts (F5, F9, Shift+F9) are
 * handled by LordOfMU's own WH_KEYBOARD_LL hook and timer-based polling.
 *
 * This bridge provides helper functions so that the Client DLL can:
 *   - Check whether the LordOfMU autoclicker is loaded
 *   - Send start/stop/settings/history commands to it
 *
 * Communication is done via PostMessage to the game's HWND, which the
 * LordOfMU CMuWindow subclass intercepts.
 */

#include <windows.h>

// -----------------------------------------------------------------------
// LordOfMU AutoClicker window-message IDs
// (must match src/LordOfClicker/MuWindow.h)
// -----------------------------------------------------------------------
#define LORDOFMU_WM_IS_CLICKER_INSTALLED (WM_APP + 402)
#define LORDOFMU_WM_SHOW_SETTINGS_GUI    (WM_APP + 502)
#define LORDOFMU_WM_START_CLICKER        (WM_APP + 503)
#define LORDOFMU_WM_STOP_CLICKER         (WM_APP + 504)
#define LORDOFMU_WM_SHOW_HISTORY         (WM_APP + 512)

// -----------------------------------------------------------------------
// Bridge API
// -----------------------------------------------------------------------

/**
 * \brief Check whether the LordOfMU autoclicker DLL is loaded and has
 *        subclassed the game window.
 * \param hGameWnd  The game's main HWND.
 * \return TRUE if the autoclicker responds; FALSE otherwise.
 */
inline BOOL LordOfMU_IsClickerInstalled(HWND hGameWnd)
{
	if (!hGameWnd || !IsWindow(hGameWnd))
		return FALSE;

	// SendMessage is synchronous — returns the instance number (>0) if
	// CMuWindow is present, or 0 if no subclass is installed.
	LRESULT lr = SendMessage(hGameWnd, LORDOFMU_WM_IS_CLICKER_INSTALLED, 0, 0);
	return (lr != 0) ? TRUE : FALSE;
}

/**
 * \brief Tell the LordOfMU autoclicker to start clicking.
 */
inline void LordOfMU_StartClicker(HWND hGameWnd)
{
	if (hGameWnd && IsWindow(hGameWnd))
		PostMessage(hGameWnd, LORDOFMU_WM_START_CLICKER, 0, 0);
}

/**
 * \brief Tell the LordOfMU autoclicker to stop clicking.
 */
inline void LordOfMU_StopClicker(HWND hGameWnd)
{
	if (hGameWnd && IsWindow(hGameWnd))
		PostMessage(hGameWnd, LORDOFMU_WM_STOP_CLICKER, 0, 0);
}

/**
 * \brief Toggle start/stop of the LordOfMU autoclicker.
 *        If the clicker is running, stop it; otherwise, start it.
 *
 * Since we cannot directly query the clicker's running state from the
 * Client DLL, we simulate the F5 key press which LordOfMU's keyboard
 * hook intercepts and toggles accordingly.
 */
inline void LordOfMU_ToggleClicker(HWND hGameWnd)
{
	// Use keybd_event to simulate F5 — LordOfMU's WH_KEYBOARD_LL hook
	// will intercept it and toggle the clicker state.
	keybd_event(VK_F5, 0, 0, 0);
	keybd_event(VK_F5, 0, KEYEVENTF_KEYUP, 0);
}

/**
 * \brief Open the LordOfMU autoclicker settings overlay (F9).
 */
inline void LordOfMU_OpenSettings(HWND hGameWnd)
{
	if (hGameWnd && IsWindow(hGameWnd))
		PostMessage(hGameWnd, LORDOFMU_WM_SHOW_SETTINGS_GUI, 0, 0);
}

/**
 * \brief Open the LordOfMU pickup history overlay (Shift+F9).
 */
inline void LordOfMU_ShowHistory(HWND hGameWnd)
{
	if (hGameWnd && IsWindow(hGameWnd))
		PostMessage(hGameWnd, LORDOFMU_WM_SHOW_HISTORY, 0, 0);
}
