#ifndef __ImGuiOverlay_H
#define __ImGuiOverlay_H

#pragma once

#include "Settings.h"
#include <d3d9.h>
#include <vector>
#include <string>

/**
 * \brief ImGui-based in-game overlay that replaces all WS_POPUP windows.
 *
 *        Rendered directly into the D3D9 EndScene call, so it is part of
 *        the game frame — no separate HWND, no focus/activation/capture
 *        conflicts.
 *
 *        Provides:
 *        - HUD button bar (Settings / Start-Stop / History)
 *        - Settings window (tabs: General, Class, Pickup)
 *        - History window (pickup item log)
 */
class CImGuiOverlay
{
public:
	CImGuiOverlay();
	~CImGuiOverlay();

	// ----- Lifecycle -----
	bool Initialize(HWND hwndGame, IDirect3DDevice9* pDevice);
	void Shutdown();
	void OnPreReset();
	void OnPostReset(HRESULT hr);

	/// Call once per frame from the EndScene hook to render the overlay.
	void Render(IDirect3DDevice9* pDevice);

	/// Forward WndProc messages so ImGui can handle mouse/keyboard.
	/// Returns TRUE if ImGui consumed the message (game should ignore it).
	BOOL WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ----- UI visibility -----
	void ToggleSettings();
	void ToggleHistory();
	void ShowSettings();
	void HideSettings();
	void ShowHistory();
	void HideHistory();

	bool IsSettingsVisible() const { return m_bShowSettings; }
	bool IsHistoryVisible() const  { return m_bShowHistory; }
	bool IsAnyWindowVisible() const { return m_bShowSettings || m_bShowHistory; }

	// ----- State from CMuWindow -----
	void SetClickerRunning(bool bRunning)   { m_bClickerRunning = bRunning; }
	void SetCharSelected(bool bSelected)    { m_bCharSelected = bSelected; }
	void SetGameActive(bool bActive)        { m_bGameActive = bActive; }

	/// Provide a pointer to the settings struct so the overlay can edit it.
	void SetSettings(CClickerSettings* pSettings) { m_pSettings = pSettings; }

	// History data
	struct HistoryEntry
	{
		std::string sTime;
		std::string sItem;
	};
	void SetHistory(const std::vector<HistoryEntry>& vHistory);

	// Session statistics
	struct SessionStats
	{
		int nKillCount;
		int nItemCount;
		unsigned __int64 ullZenTotal;
		unsigned __int64 ullExpGained;
		unsigned long ulRuntimeSeconds;
	};
	void SetSessionStats(const SessionStats& stats);

	// ----- Callbacks -----
	/// These function pointers are called when HUD buttons are clicked.
	typedef void (*FnCallback)(void* pUserData);
	void SetOnSettingsClicked(FnCallback pfn, void* pData) { m_pfnOnSettings = pfn; m_pSettingsData = pData; }
	void SetOnStartStopClicked(FnCallback pfn, void* pData) { m_pfnOnStartStop = pfn; m_pStartStopData = pData; }
	void SetOnHistoryClicked(FnCallback pfn, void* pData) { m_pfnOnHistory = pfn; m_pHistoryData = pData; }
	void SetOnSettingsApply(FnCallback pfn, void* pData) { m_pfnOnApply = pfn; m_pApplyData = pData; }

	bool IsInitialized() const { return m_bInitialized; }

private:
	// ----- ImGui rendering sections -----
	void RenderHUD();
	void RenderSettingsWindow();
	void RenderHistoryWindow();

	// Settings tabs
	void RenderTabGeneral();
	void RenderTabClass();
	void RenderTabPickup();

	// Class-specific settings panels
	void RenderClassDarkLord();
	void RenderClassEvilElf();
	void RenderClassAngelElf();
	void RenderClassBladeKnight();
	void RenderClassSoulMaster();
	void RenderClassMagicGladiator();

	// Style setup
	void SetupMuThemeStyle();

	// ----- State -----
	bool m_bInitialized;
	HWND m_hwndGame;

	// Visibility
	bool m_bShowSettings;
	bool m_bShowHistory;

	// Game state
	bool m_bClickerRunning;
	bool m_bCharSelected;
	bool m_bGameActive;

	// Settings reference
	CClickerSettings* m_pSettings;

	// History data
	std::vector<HistoryEntry> m_vHistory;

	// Session statistics
	SessionStats m_sessionStats;

	// History dialog state
	int m_nHistoryTab;     // 0 = Items, 1 = Kill Count
	int m_nHistoryPage;    // current page (0-based) for Items tab

	// Callbacks
	FnCallback m_pfnOnSettings;   void* m_pSettingsData;
	FnCallback m_pfnOnStartStop;  void* m_pStartStopData;
	FnCallback m_pfnOnHistory;    void* m_pHistoryData;
	FnCallback m_pfnOnApply;      void* m_pApplyData;

	// Settings tab index
	int m_nSettingsTab;
};

#endif // __ImGuiOverlay_H
