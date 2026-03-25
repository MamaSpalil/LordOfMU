#ifndef __MU_WINDOW_H
#define __MU_WINDOW_H

#pragma once

#include "SettingsDlg.h"
#include "ClickerJob.h"
#include "LaunchMuDialog.h"
#include "AdvSettingsDialog.h"
#include "D3D9Hook.h"
#include "ImGuiOverlay.h"
#include "ApiHooker.h"

#define WM_IS_CLICKER_INSTALLED (WM_APP + 402)

#define WM_MUWND_INITIALIZE     (WM_APP + 501)
#define WM_SHOW_SETTINGS_GUI    (WM_APP + 502)
#define WM_START_CLICKER        (WM_APP + 503)
#define WM_STOP_CLICKER         (WM_APP + 504)
#define WM_LAUNCH_MU            (WM_APP + 505)
#define WM_SWITCH_INSTANCE      (WM_APP + 506)
#define WM_SHOW_MU              (WM_APP + 507)
#define WM_HIDE_MU              (WM_APP + 508)
#define WM_SET_GAME_VERSION     (WM_APP + 509)
#define WM_CHAR_SELECTED        (WM_APP + 510)
#define WM_CHAR_DESELECTED      (WM_APP + 511)
#define WM_SHOW_HISTORY         (WM_APP + 512)


/**
 * \brief
 */
class CMuWindow 
	: public CWindowImpl<CMuWindow>
{
protected:
	CMuWindow();
	virtual ~CMuWindow();

public:
	static BOOL Init(HWND hwndMuWindow, int iInstanceNumber);
	static void Term();
	static CMuWindow* GetInstance() { return s_pInstance; }
	
	ULONG GetVersion() { return m_ulVersion; }
	void SayToServer(const char* buf);

protected:
	BEGIN_MSG_MAP(CMuWindow)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		MESSAGE_RANGE_HANDLER(WM_KEYFIRST, WM_KEYLAST, OnKeyboardMessage)
		MESSAGE_HANDLER(WM_ACTIVATEAPP, OnActivateApp)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNCActivate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_MUWND_INITIALIZE, OnInitMuWindow)
		MESSAGE_HANDLER(WM_IS_CLICKER_INSTALLED, OnIsClickerInstalled)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnErasebkgnd)
		MESSAGE_HANDLER(WM_SHOW_SETTINGS_GUI, OnShowSettingsGUI)
		MESSAGE_HANDLER(WM_START_CLICKER, OnStartClicker)
		MESSAGE_HANDLER(WM_STOP_CLICKER, OnStopClicker)
		MESSAGE_HANDLER(WM_CLICKER_JOB_FINISHED, OnClickerJobFinished)
		MESSAGE_HANDLER(WM_CLICKER_JOB_MOUSEMOVE, OnClickerMouseMove)
		MESSAGE_HANDLER(WM_CLICKER_JOB_RBUTTONDOWN, OnClickerRButtonDown)
		MESSAGE_HANDLER(WM_CLICKER_JOB_RBUTTONUP,   OnClickerRButtonUp)
		MESSAGE_HANDLER(WM_CLICKER_JOB_LBUTTONDOWN, OnClickerLButtonDown)
		MESSAGE_HANDLER(WM_CLICKER_JOB_LBUTTONUP,   OnClickerLButtonUp)
		MESSAGE_HANDLER(WM_LAUNCH_MU, OnLaunchMu)
		MESSAGE_HANDLER(WM_SWITCH_INSTANCE, OnSwitchMuInstance)
		MESSAGE_HANDLER(WM_SHOW_MU, OnShowMu)
		MESSAGE_HANDLER(WM_HIDE_MU, OnHideMu)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER(WM_SET_GAME_VERSION, OnSetVersion)
		MESSAGE_HANDLER(WM_MOVE, OnGameWindowChanged)
		MESSAGE_HANDLER(WM_SIZE, OnGameWindowChanged)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CHAR_SELECTED, OnCharSelected)
		MESSAGE_HANDLER(WM_CHAR_DESELECTED, OnCharDeselected)
		MESSAGE_HANDLER(WM_SHOW_HISTORY, OnShowHistory)
	END_MSG_MAP()

protected:
	LRESULT OnInitMuWindow(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnIsClickerInstalled(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseMessage(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnKeyboardMessage(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnActivateApp(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnErasebkgnd(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShowSettingsGUI(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnStartClicker(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnStopClicker(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClickerJobFinished(UINT, WPARAM, LPARAM, BOOL&);

	LRESULT OnClickerMouseMove(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClickerRButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClickerRButtonUp(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClickerLButtonDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClickerLButtonUp(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnLaunchMu(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSwitchMuInstance(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShowMu(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnHideMu(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnNCActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCaptureChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetVersion(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnGameWindowChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCharSelected(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCharDeselected(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShowHistory(UINT, WPARAM, LPARAM, BOOL&);

	// ImGui overlay static callbacks (D3D9Hook -> CMuWindow)
	static void OnEndSceneCallback(IDirect3DDevice9* pDevice);
	static void OnPreResetCallback(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPP);
	static void OnPostResetCallback(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPP, HRESULT hr);

	// ImGui overlay UI callbacks (button clicks)
	static void OnOverlaySettingsClicked(void* pData);
	static void OnOverlayStartStopClicked(void* pData);
	static void OnOverlayHistoryClicked(void* pData);
	static void OnOverlayApplyClicked(void* pData);

	// History data query helper
	void QueryPickupHistory();

	// Session statistics helpers
	void QuerySessionStats();
	void ResetSessionStatsCall();
	void StopSessionCall();

protected:
	BOOL OnKeyboardEvent(UINT vkCode, UINT uMsg, BOOL fCheckFgWnd = TRUE);
	void LaunchMU();
	void PatchProcessMemory(HANDLE hProcess, char patchByte, const char* szMutexName);

public:
	static LRESULT CALLBACK KeyboardProcLL(int code, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK MyEnumWndProc(HWND hWnd, LPARAM pData);

	static SHORT WINAPI MyGetAsyncKeyState(int);
	static LONG WINAPI MyChangeDisplaySettingsA(LPDEVMODEA, DWORD);
	static HWND WINAPI MyGetForegroundWindow(VOID);
	static HWND WINAPI MyGetActiveWindow(VOID);
	

	static CGetAsyncKeyStateTramp GetAsyncKeyState;
	static CChangeDisplaySettingsATramp ChangeDisplaySettingsA;
	static CGetForegroundWindowTramp GetForegroundWindowTr;
	static CGetActiveWindowTramp GetActiveWindowTr;

protected:	
	BOOL m_fIsWndActive;
	BOOL m_fBlockInput;
	BOOL m_fGuiActive;     // TRUE when ImGui overlay is showing a dialog
	bool m_fShiftWasDown;  // BUG-4 fix: Shift state captured on F9 KEYDOWN
	BOOL m_fWasLastActiveInstance;

	int m_iInstanceNumber;

	CSettingsDlg m_cSettingsDlg;
	CLaunchMuDialog m_cLaunchMuDlg;

	// ImGui-based overlay that renders directly into the D3D EndScene.
	// Replaces CUnifiedSettingsDlg, CHistoryDialog, and CHUDButtons.
	CImGuiOverlay m_cOverlay;

private:
	HHOOK m_hKbdHook;
	static CMuWindow* s_pInstance;
	
	ULONG m_ulVersion;
	bool m_fWindow;

	CClickerJob* m_pClicker;
	char* m_buffDevmode[sizeof(DEVMODEA)+2048];

	struct FN_KEYS_T { int vk; bool fPressed; } m_vFnKeys[10];

	HANDLE m_hEvent;
};

#endif //__MU_WINDOW_H