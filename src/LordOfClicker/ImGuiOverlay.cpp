#include "stdafx.h"
#include "ImGuiOverlay.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "version.h"
#include <cstdio>

// Forward-declare the ImGui Win32 WndProc handler (defined in imgui_impl_win32.cpp).
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// History dialog constants
static const char* ZEN_ENTRY_PREFIX = "Zen '";
static const size_t ZEN_ENTRY_PREFIX_LEN = 5;
static const int ITEMS_PER_PAGE = 10;
static const int MAX_PAGE_BUTTONS = 10;

// ============================================================================
// Character class names (must match CHAR_CLASS_* defines in Settings.h)
// ============================================================================
static const char* g_szClassNames[] =
{
	"Dark Lord",        // CHAR_CLASS_DL = 0
	"Evil Elf",         // CHAR_CLASS_EE = 1
	"Angel Elf",        // CHAR_CLASS_AE = 2
	"Blade Knight",     // CHAR_CLASS_BK = 3
	"Soul Master",      // CHAR_CLASS_SM = 4
	"Magic Gladiator",  // CHAR_CLASS_MG = 5
};
static const int g_nClassCount = sizeof(g_szClassNames) / sizeof(g_szClassNames[0]);

// ============================================================================
// Construction / Destruction
// ============================================================================

CImGuiOverlay::CImGuiOverlay()
	: m_bInitialized(false)
	, m_hwndGame(NULL)
	, m_bShowSettings(false)
	, m_bShowHistory(false)
	, m_bClickerRunning(false)
	, m_bCharSelected(false)
	, m_bGameActive(true)
	, m_pSettings(NULL)
	, m_pfnOnSettings(NULL), m_pSettingsData(NULL)
	, m_pfnOnStartStop(NULL), m_pStartStopData(NULL)
	, m_pfnOnHistory(NULL), m_pHistoryData(NULL)
	, m_pfnOnApply(NULL), m_pApplyData(NULL)
	, m_nSettingsTab(0)
	, m_nHistoryTab(0)
	, m_nHistoryPage(0)
{
	memset(&m_sessionStats, 0, sizeof(m_sessionStats));
}

CImGuiOverlay::~CImGuiOverlay()
{
	Shutdown();
}

// ============================================================================
// Lifecycle
// ============================================================================

bool CImGuiOverlay::Initialize(HWND hwndGame, IDirect3DDevice9* pDevice)
{
	if (m_bInitialized)
		return true;

	m_hwndGame = hwndGame;

	// Create ImGui context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Disable automatic .ini saving — we don't need persistent layout.
	io.IniFilename = NULL;

	// Apply dark-gold MU Online theme.
	SetupMuThemeStyle();

	// Initialize platform + renderer backends.
	ImGui_ImplWin32_Init(hwndGame);
	ImGui_ImplDX9_Init(pDevice);

	m_bInitialized = true;
	return true;
}

void CImGuiOverlay::Shutdown()
{
	if (!m_bInitialized)
		return;

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	m_bInitialized = false;
}

void CImGuiOverlay::OnPreReset()
{
	if (m_bInitialized)
		ImGui_ImplDX9_InvalidateDeviceObjects();
}

void CImGuiOverlay::OnPostReset(HRESULT hr)
{
	if (m_bInitialized && SUCCEEDED(hr))
		ImGui_ImplDX9_CreateDeviceObjects();
}

// ============================================================================
// Render  (called from EndScene hook)
// ============================================================================

void CImGuiOverlay::Render(IDirect3DDevice9* pDevice)
{
	if (!m_bInitialized)
		return;

	// Start the ImGui frame.
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// HUD bar is always visible (if character selected + game active).
	if (m_bCharSelected && m_bGameActive)
		RenderHUD();

	// Popup windows.
	if (m_bShowSettings)
		RenderSettingsWindow();

	if (m_bShowHistory)
		RenderHistoryWindow();

	// Finalize and submit draw data.
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

// ============================================================================
// WndProc forwarding
// ============================================================================

BOOL CImGuiOverlay::WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bInitialized)
		return FALSE;

	// Let ImGui process the message.
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return TRUE;

	// If ImGui wants mouse/keyboard, tell the caller to NOT pass the
	// message to the game's WndProc.
	ImGuiIO& io = ImGui::GetIO();

	switch (uMsg)
	{
	case WM_LBUTTONDOWN: case WM_LBUTTONUP:
	case WM_RBUTTONDOWN: case WM_RBUTTONUP:
	case WM_MBUTTONDOWN: case WM_MBUTTONUP:
	case WM_MOUSEMOVE:   case WM_MOUSEWHEEL:
		if (io.WantCaptureMouse)
			return TRUE;
		break;

	case WM_KEYDOWN: case WM_KEYUP:
	case WM_CHAR:    case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		if (io.WantCaptureKeyboard)
			return TRUE;
		break;
	}

	return FALSE;
}

// ============================================================================
// Visibility
// ============================================================================

void CImGuiOverlay::ToggleSettings()
{
	m_bShowSettings = !m_bShowSettings;
	if (m_bShowSettings)
		m_bShowHistory = false;   // Only one panel at a time.
}

void CImGuiOverlay::ToggleHistory()
{
	m_bShowHistory = !m_bShowHistory;
	if (m_bShowHistory)
		m_bShowSettings = false;
}

void CImGuiOverlay::ShowSettings()  { m_bShowSettings = true;  m_bShowHistory = false; }
void CImGuiOverlay::HideSettings()  { m_bShowSettings = false; }
void CImGuiOverlay::ShowHistory()   { m_bShowHistory  = true;  m_bShowSettings = false; }
void CImGuiOverlay::HideHistory()   { m_bShowHistory  = false; }

void CImGuiOverlay::SetHistory(const std::vector<HistoryEntry>& vHistory)
{
	m_vHistory = vHistory;
}

void CImGuiOverlay::SetSessionStats(const SessionStats& stats)
{
	m_sessionStats = stats;
}

// ============================================================================
// MU Online Dark-Gold Theme
// ============================================================================

void CImGuiOverlay::SetupMuThemeStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	// Rounding & spacing (compact game HUD feel)
	style.WindowRounding    = 2.0f;
	style.FrameRounding     = 2.0f;
	style.GrabRounding      = 2.0f;
	style.TabRounding       = 2.0f;
	style.ScrollbarRounding = 2.0f;
	style.WindowPadding     = ImVec2(8, 8);
	style.FramePadding      = ImVec2(6, 3);
	style.ItemSpacing       = ImVec2(6, 4);
	style.WindowBorderSize  = 1.0f;

	// Dark-gold MU Online S3E1 color palette
	ImVec4* colors = style.Colors;

	// Window
	colors[ImGuiCol_WindowBg]        = ImVec4(0.071f, 0.059f, 0.039f, 0.94f); // RGB(18,15,10)
	colors[ImGuiCol_Border]          = ImVec4(0.471f, 0.373f, 0.157f, 1.00f); // RGB(120,95,40)
	colors[ImGuiCol_BorderShadow]    = ImVec4(0.000f, 0.000f, 0.000f, 0.50f);

	// Title
	colors[ImGuiCol_TitleBg]         = ImVec4(0.098f, 0.086f, 0.063f, 1.00f); // RGB(25,22,16)
	colors[ImGuiCol_TitleBgActive]   = ImVec4(0.157f, 0.125f, 0.059f, 1.00f); // Slightly brighter
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.071f, 0.059f, 0.039f, 0.75f);

	// Text
	colors[ImGuiCol_Text]            = ImVec4(0.784f, 0.765f, 0.706f, 1.00f); // RGB(200,195,180)
	colors[ImGuiCol_TextDisabled]    = ImVec4(0.392f, 0.373f, 0.314f, 1.00f); // RGB(100,95,80)

	// Frame (input fields, checkboxes)
	colors[ImGuiCol_FrameBg]         = ImVec4(0.098f, 0.086f, 0.063f, 1.00f); // RGB(25,22,16)
	colors[ImGuiCol_FrameBgHovered]  = ImVec4(0.157f, 0.137f, 0.086f, 1.00f);
	colors[ImGuiCol_FrameBgActive]   = ImVec4(0.216f, 0.188f, 0.118f, 1.00f);

	// Buttons
	colors[ImGuiCol_Button]          = ImVec4(0.157f, 0.137f, 0.086f, 1.00f); // RGB(40,35,22)
	colors[ImGuiCol_ButtonHovered]   = ImVec4(0.216f, 0.188f, 0.118f, 1.00f); // RGB(55,48,30)
	colors[ImGuiCol_ButtonActive]    = ImVec4(0.294f, 0.235f, 0.118f, 1.00f); // RGB(75,60,30)

	// Tab
	colors[ImGuiCol_Tab]             = ImVec4(0.086f, 0.071f, 0.047f, 1.00f); // RGB(22,18,12)
	colors[ImGuiCol_TabHovered]      = ImVec4(0.216f, 0.188f, 0.118f, 1.00f);
	colors[ImGuiCol_TabSelected]     = ImVec4(0.157f, 0.137f, 0.086f, 1.00f); // RGB(40,35,22)
	colors[ImGuiCol_TabDimmed]       = ImVec4(0.071f, 0.059f, 0.039f, 1.00f);
	colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.098f, 0.086f, 0.063f, 1.00f);

	// Header (collapsing headers, menu bar items)
	colors[ImGuiCol_Header]          = ImVec4(0.157f, 0.137f, 0.086f, 1.00f);
	colors[ImGuiCol_HeaderHovered]   = ImVec4(0.216f, 0.188f, 0.118f, 1.00f);
	colors[ImGuiCol_HeaderActive]    = ImVec4(0.294f, 0.235f, 0.118f, 1.00f);

	// Checkbox/Slider
	colors[ImGuiCol_CheckMark]       = ImVec4(0.510f, 0.745f, 0.235f, 1.00f); // RGB(130,190,60) green
	colors[ImGuiCol_SliderGrab]      = ImVec4(0.471f, 0.373f, 0.157f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.765f, 0.647f, 0.275f, 1.00f);

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg]     = ImVec4(0.059f, 0.049f, 0.033f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab]   = ImVec4(0.157f, 0.137f, 0.086f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.216f, 0.188f, 0.118f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.294f, 0.235f, 0.118f, 1.00f);

	// Separator
	colors[ImGuiCol_Separator]       = ImVec4(0.275f, 0.227f, 0.125f, 0.50f); // RGB(70,58,32)
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.471f, 0.373f, 0.157f, 0.75f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.765f, 0.647f, 0.275f, 1.00f);

	// Popup / Tooltip
	colors[ImGuiCol_PopupBg]         = ImVec4(0.071f, 0.059f, 0.039f, 0.94f);
}

// ============================================================================
// HUD Button Bar
// ============================================================================

void CImGuiOverlay::RenderHUD()
{
	// Position: top-left area, to the right of the FPS counter.
	// Matches HUD_OFFSET_X=90, HUD_OFFSET_Y=5 from the old CHUDButtons.
	ImGui::SetNextWindowPos(ImVec2(90.0f, 5.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(0, 0));  // Auto-size

	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing;

	// Semi-transparent background so game scene is visible behind.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.04f, 0.03f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.47f, 0.37f, 0.16f, 0.80f));

	if (ImGui::Begin("##HUD", NULL, flags))
	{
		ImVec2 btnSize(22, 22);

		// Settings button (gear icon: "S")
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.82f, 0.71f, 0.27f, 1.0f));
		if (ImGui::Button("S##Settings", btnSize))
		{
			if (m_pfnOnSettings)
				m_pfnOnSettings(m_pSettingsData);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Settings (F9)");

		ImGui::SameLine();

		// Start / Stop button
		if (m_bClickerRunning)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.50f, 0.15f, 0.10f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.20f, 0.12f, 1.0f));
			if (ImGui::Button("X##Stop", btnSize))
			{
				if (m_pfnOnStartStop)
					m_pfnOnStartStop(m_pStartStopData);
			}
			ImGui::PopStyleColor(2);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Stop Clicker (F5)");
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.35f, 0.10f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.50f, 0.15f, 1.0f));
			if (ImGui::Button(">##Start", btnSize))
			{
				if (m_pfnOnStartStop)
					m_pfnOnStartStop(m_pStartStopData);
			}
			ImGui::PopStyleColor(2);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Start Clicker (F5)");
		}

		ImGui::SameLine();

		// History button
		if (ImGui::Button("H##History", btnSize))
		{
			if (m_pfnOnHistory)
				m_pfnOnHistory(m_pHistoryData);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Pickup History (Shift+F9)");

		ImGui::PopStyleColor(); // text color
	}
	ImGui::End();

	ImGui::PopStyleColor(2); // WindowBg, Border
	ImGui::PopStyleVar(2);   // Padding, Spacing
}

// ============================================================================
// Settings Window
// ============================================================================

void CImGuiOverlay::RenderSettingsWindow()
{
	if (!m_pSettings)
		return;

	ImGui::SetNextWindowSize(ImVec2(480, 420), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(100, 50), ImGuiCond_FirstUseEver);

	char szTitle[128];
	sprintf_s(szTitle, "MU AutoClicker Settings  [%s]###Settings", __SOFTWARE_VERSION_STR);

	bool bOpen = m_bShowSettings;
	if (ImGui::Begin(szTitle, &bOpen, ImGuiWindowFlags_NoSavedSettings))
	{
		// Tab bar
		if (ImGui::BeginTabBar("##SettingsTabs"))
		{
			if (ImGui::BeginTabItem("General"))
			{
				m_nSettingsTab = 0;
				RenderTabGeneral();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Class"))
			{
				m_nSettingsTab = 1;
				RenderTabClass();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Pickup"))
			{
				m_nSettingsTab = 2;
				RenderTabPickup();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::Separator();

		// Apply / Cancel
		float btnWidth = 80.0f;
		float spacing  = 10.0f;
		float totalW   = btnWidth * 2 + spacing;
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalW) * 0.5f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.35f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.45f, 0.20f, 1.0f));
		if (ImGui::Button("Apply", ImVec2(btnWidth, 0)))
		{
			// Settings are edited in-place, so just fire the callback
			if (m_pfnOnApply)
				m_pfnOnApply(m_pApplyData);
			m_bShowSettings = false;
		}
		ImGui::PopStyleColor(2);

		ImGui::SameLine(0, spacing);

		if (ImGui::Button("Cancel", ImVec2(btnWidth, 0)))
		{
			m_bShowSettings = false;
		}
	}
	ImGui::End();

	if (!bOpen)
		m_bShowSettings = false;
}

// ============================================================================
// General Tab
// ============================================================================

void CImGuiOverlay::RenderTabGeneral()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);

	ImGui::Spacing();
	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Autoclicker Timing");
	ImGui::Separator();

	// Auto Heal
	bool bAutoHeal = (s->all.fAutoLife != FALSE);
	if (ImGui::Checkbox("Auto Heal", &bAutoHeal))
		s->all.fAutoLife = bAutoHeal ? TRUE : FALSE;

	if (bAutoHeal)
	{
		ImGui::SameLine(200);
		int healTime = (int)s->all.dwHealTime;
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##HealTime", &healTime, 500, 1000))
		{
			if (healTime < 500) healTime = 500;
			if (healTime > 60000) healTime = 60000;
			s->all.dwHealTime = (DWORD)healTime;
		}
		ImGui::SameLine();
		ImGui::TextDisabled("ms");
	}

	// Auto Pick
	bool bAutoPick = (s->all.fAutoPick != FALSE);
	if (ImGui::Checkbox("Auto Pick", &bAutoPick))
		s->all.fAutoPick = bAutoPick ? TRUE : FALSE;

	if (bAutoPick)
	{
		ImGui::SameLine(200);
		int pickTime = (int)s->all.dwPickTime;
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##PickTime", &pickTime, 200, 500))
		{
			if (pickTime < 200) pickTime = 200;
			if (pickTime > 30000) pickTime = 30000;
			s->all.dwPickTime = (DWORD)pickTime;
		}
		ImGui::SameLine();
		ImGui::TextDisabled("ms");
	}

	// Auto Repair
	bool bAutoRepair = (s->all.fAutoRepair != FALSE);
	if (ImGui::Checkbox("Auto Repair", &bAutoRepair))
		s->all.fAutoRepair = bAutoRepair ? TRUE : FALSE;

	if (bAutoRepair)
	{
		ImGui::SameLine(200);
		int repairTime = (int)s->all.dwRepairTime;
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputInt("##RepairTime", &repairTime, 10000, 60000))
		{
			if (repairTime < 10000) repairTime = 10000;
			if (repairTime > 600000) repairTime = 600000;
			s->all.dwRepairTime = (DWORD)repairTime;
		}
		ImGui::SameLine();
		ImGui::TextDisabled("ms");
	}

	// Stop picking when inventory full
	bool bAutoReOff = (s->all.fAutoReOff != FALSE);
	if (ImGui::Checkbox("Stop Pick on Inventory Full", &bAutoReOff))
		s->all.fAutoReOff = bAutoReOff ? TRUE : FALSE;

	ImGui::Spacing();
	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Miscellaneous");
	ImGui::Separator();

	// Anti-AFK
	int antiAFK = s->all.fAntiAFKProtect;
	if (ImGui::Checkbox("Anti-AFK Protection", (bool*)&antiAFK))
		s->all.fAntiAFKProtect = antiAFK;
}

// ============================================================================
// Class Tab
// ============================================================================

void CImGuiOverlay::RenderTabClass()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);

	ImGui::Spacing();

	// Character class combo
	int iClass = (int)s->all.dwClass;
	if (iClass < 0 || iClass > CHAR_CLASS_LAST) iClass = 0;

	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Character Class");
	ImGui::SetNextItemWidth(200);
	if (ImGui::Combo("##CharClass", &iClass, g_szClassNames, g_nClassCount))
	{
		s->all.dwClass = (DWORD)iClass;
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	switch (iClass)
	{
	case CHAR_CLASS_DL: RenderClassDarkLord();      break;
	case CHAR_CLASS_EE: RenderClassEvilElf();        break;
	case CHAR_CLASS_AE: RenderClassAngelElf();       break;
	case CHAR_CLASS_BK: RenderClassBladeKnight();    break;
	case CHAR_CLASS_SM: RenderClassSoulMaster();     break;
	case CHAR_CLASS_MG: RenderClassMagicGladiator(); break;
	}
}

// ============================================================================
// Pickup Tab
// ============================================================================

void CImGuiOverlay::RenderTabPickup()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);

	ImGui::Spacing();

	bool bAdvPick = (s->all.fAdvAutoPick != FALSE);
	if (ImGui::Checkbox("Advanced Auto Pickup", &bAdvPick))
		s->all.fAdvAutoPick = bAdvPick ? TRUE : FALSE;

	if (!bAdvPick)
	{
		ImGui::TextDisabled("Enable Advanced Auto Pickup to configure item filters.");
		return;
	}

	ImGui::Spacing();
	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Item Filters");
	ImGui::Separator();

	// Table: Item | Pick | Move To
	if (ImGui::BeginTable("##PickupItems", 3, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("Item", ImGuiTableColumnFlags_WidthFixed, 180);
		ImGui::TableSetupColumn("Pick", ImGuiTableColumnFlags_WidthFixed, 50);
		ImGui::TableSetupColumn("Move", ImGuiTableColumnFlags_WidthFixed, 50);
		ImGui::TableHeadersRow();

		// Helper macro to reduce repetition.
		#define PICKUP_ROW(label, fPick, fMove) do { \
			ImGui::TableNextRow(); \
			ImGui::TableNextColumn(); ImGui::TextUnformatted(label); \
			ImGui::TableNextColumn(); { bool b = (fPick != FALSE); if (ImGui::Checkbox("##pick_" label, &b)) fPick = b ? TRUE : FALSE; } \
			ImGui::TableNextColumn(); { bool b = (fMove != FALSE); if (ImGui::Checkbox("##move_" label, &b)) fMove = b ? TRUE : FALSE; } \
		} while(0)

		PICKUP_ROW("Jewel of Bless",    s->all.fAdvPickBless,     s->all.fAdvPickBlessMove);
		PICKUP_ROW("Jewel of Soul",     s->all.fAdvPickSoul,      s->all.fAdvPickSoulMove);
		PICKUP_ROW("Jewel of Life",     s->all.fAdvPickLife,       s->all.fAdvPickLifeMove);
		PICKUP_ROW("Jewel of Guardian", s->all.fAdvPickGuardian,   s->all.fAdvPickGuardianMove);
		PICKUP_ROW("Jewel of Creation", s->all.fAdvPickCreation,   s->all.fAdvPickCreationMove);
		PICKUP_ROW("Jewel of Chaos",    s->all.fAdvPickChaos,      s->all.fAdvPickChaosMove);
		PICKUP_ROW("Excellent Items",   s->all.fAdvPickExl,        s->all.fAdvPickExlMove);
		PICKUP_ROW("Zen",               s->all.fAdvPickZen,        s->all.fAdvPickZenMove);
		PICKUP_ROW("Custom Item",       s->all.fAdvPickCustom,     s->all.fAdvPickCustomMove);

		#undef PICKUP_ROW

		ImGui::EndTable();
	}

	// Custom item code input
	if (s->all.fAdvPickCustom)
	{
		ImGui::Spacing();
		char szCode[16];
		sprintf_s(szCode, "0x%04X", (unsigned)s->all.wPickCustomCode);
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputText("Custom Item Code", szCode, sizeof(szCode),
			ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
		{
			unsigned val = 0;
			if (sscanf_s(szCode, "%x", &val) == 1)
				s->all.wPickCustomCode = (WORD)(val & 0xFFFF);
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Pickup distance
	int dist = (int)s->all.dwPickDist;
	if (dist < PICKUP_DIST_MIN) dist = PICKUP_DIST_MIN;
	if (dist > PICKUP_DIST_MAX) dist = PICKUP_DIST_MAX;
	ImGui::SetNextItemWidth(100);
	if (ImGui::SliderInt("Pickup Distance", &dist, PICKUP_DIST_MIN, PICKUP_DIST_MAX))
		s->all.dwPickDist = (DWORD)dist;

	// Pickup run mode
	bool bRunMode = (s->all.fPickRunMode != FALSE);
	if (ImGui::Checkbox("Pickup Run Mode", &bRunMode))
		s->all.fPickRunMode = bRunMode ? TRUE : FALSE;
}

// ============================================================================
// Class-Specific Panels
// ============================================================================

void CImGuiOverlay::RenderClassDarkLord()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);
	DLSettings& dl = s->dl;

	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Dark Lord Settings");

	int attackSlot = (int)dl.dwAttackSlot;
	ImGui::SetNextItemWidth(80);
	if (ImGui::InputInt("Attack Skill Slot", &attackSlot, 1, 1))
	{
		if (attackSlot < 1) attackSlot = 1;
		if (attackSlot > 9) attackSlot = 9;
		dl.dwAttackSlot = (DWORD)attackSlot;
	}

	bool bCritDmg = (dl.fUseCritDmg != FALSE);
	if (ImGui::Checkbox("Use Critical Damage", &bCritDmg))
		dl.fUseCritDmg = bCritDmg ? TRUE : FALSE;

	if (bCritDmg)
	{
		int critSlot = (int)dl.dwCriticalDmgSlot;
		ImGui::SetNextItemWidth(80);
		if (ImGui::InputInt("Critical Dmg Skill Slot", &critSlot, 1, 1))
		{
			if (critSlot < 1) critSlot = 1;
			if (critSlot > 9) critSlot = 9;
			dl.dwCriticalDmgSlot = (DWORD)critSlot;
		}
	}

	bool bDarkHorse = (dl.fUseDarkHorse != FALSE);
	if (ImGui::Checkbox("Use Dark Horse", &bDarkHorse))
		dl.fUseDarkHorse = bDarkHorse ? TRUE : FALSE;

	if (bDarkHorse)
	{
		int horseSlot = (int)dl.dwDarkHorseSlot;
		ImGui::SetNextItemWidth(80);
		if (ImGui::InputInt("Dark Horse Skill Slot", &horseSlot, 1, 1))
		{
			if (horseSlot < 1) horseSlot = 1;
			if (horseSlot > 9) horseSlot = 9;
			dl.dwDarkHorseSlot = (DWORD)horseSlot;
		}
	}

	int margin = (int)dl.dwNoClickMargin;
	ImGui::SetNextItemWidth(100);
	if (ImGui::InputInt("No-Click Margin (px)", &margin, 10, 50))
	{
		if (margin < 0) margin = 0;
		if (margin > 500) margin = 500;
		dl.dwNoClickMargin = (DWORD)margin;
	}
}

void CImGuiOverlay::RenderClassEvilElf()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);
	EElfSettings& ee = s->ee;

	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Evil Elf (Cleric) Settings");

	bool bUseDmg = (ee.fUseDmgSkill != FALSE);
	if (ImGui::Checkbox("Use Damage Skill", &bUseDmg))
		ee.fUseDmgSkill = bUseDmg ? TRUE : FALSE;
	if (bUseDmg)
	{
		int slot = (int)ee.dwDmgSkillSlot;
		ImGui::SetNextItemWidth(80);
		if (ImGui::InputInt("Damage Skill Slot", &slot, 1, 1))
		{ if (slot < 1) slot = 1; if (slot > 9) slot = 9; ee.dwDmgSkillSlot = (DWORD)slot; }
	}

	bool bUseDef = (ee.fUseDefSkill != FALSE);
	if (ImGui::Checkbox("Use Defense Skill", &bUseDef))
		ee.fUseDefSkill = bUseDef ? TRUE : FALSE;
	if (bUseDef)
	{
		int slot = (int)ee.dwDefSkillSlot;
		ImGui::SetNextItemWidth(80);
		if (ImGui::InputInt("Defense Skill Slot", &slot, 1, 1))
		{ if (slot < 1) slot = 1; if (slot > 9) slot = 9; ee.dwDefSkillSlot = (DWORD)slot; }
	}

	bool bUseHeal = (ee.fUseHealSkill != FALSE);
	if (ImGui::Checkbox("Use Heal Skill", &bUseHeal))
		ee.fUseHealSkill = bUseHeal ? TRUE : FALSE;
	if (bUseHeal)
	{
		int slot = (int)ee.dwHealSkillSlot;
		ImGui::SetNextItemWidth(80);
		if (ImGui::InputInt("Heal Skill Slot", &slot, 1, 1))
		{ if (slot < 1) slot = 1; if (slot > 9) slot = 9; ee.dwHealSkillSlot = (DWORD)slot; }
	}

	int party = (int)ee.dwPartyMembers;
	ImGui::SetNextItemWidth(80);
	if (ImGui::InputInt("Party Members", &party, 1, 1))
	{ if (party < 1) party = 1; if (party > 5) party = 5; ee.dwPartyMembers = (DWORD)party; }
}

void CImGuiOverlay::RenderClassAngelElf()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);
	AElfSettings& ae = s->ae;

	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Angel Elf (Archer) Settings");

	int shotSlot = (int)ae.dwTShotSkillSlot;
	ImGui::SetNextItemWidth(80);
	if (ImGui::InputInt("Triple Shot Skill Slot", &shotSlot, 1, 1))
	{ if (shotSlot < 1) shotSlot = 1; if (shotSlot > 9) shotSlot = 9; ae.dwTShotSkillSlot = (DWORD)shotSlot; }

	bool bInfArrow = (ae.fUseInfArrow != FALSE);
	if (ImGui::Checkbox("Use Infinite Arrow", &bInfArrow))
		ae.fUseInfArrow = bInfArrow ? TRUE : FALSE;
	if (bInfArrow)
	{
		int slot = (int)ae.dwInfArrowSkillSlot;
		ImGui::SetNextItemWidth(80);
		if (ImGui::InputInt("Infinite Arrow Slot", &slot, 1, 1))
		{ if (slot < 1) slot = 1; if (slot > 9) slot = 9; ae.dwInfArrowSkillSlot = (DWORD)slot; }
	}

	int dirTime = (int)ae.dwChangeDirTime;
	ImGui::SetNextItemWidth(100);
	if (ImGui::InputInt("Direction Change (ms)", &dirTime, 500, 1000))
	{ if (dirTime < 500) dirTime = 500; if (dirTime > 30000) dirTime = 30000; ae.dwChangeDirTime = (DWORD)dirTime; }
}

void CImGuiOverlay::RenderClassBladeKnight()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);
	BKSettings& bk = s->bk;

	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Blade Knight Settings");

	int attackSlot = (int)bk.dwAttackSkillSlot;
	ImGui::SetNextItemWidth(80);
	if (ImGui::InputInt("Attack Skill Slot", &attackSlot, 1, 1))
	{ if (attackSlot < 1) attackSlot = 1; if (attackSlot > 9) attackSlot = 9; bk.dwAttackSkillSlot = (DWORD)attackSlot; }

	bool bGrFort = (bk.fUseGrFort != FALSE);
	if (ImGui::Checkbox("Use Great Fortitude", &bGrFort))
		bk.fUseGrFort = bGrFort ? TRUE : FALSE;
	if (bGrFort)
	{
		int slot = (int)bk.dwGrFortSkillSlot;
		ImGui::SetNextItemWidth(80);
		if (ImGui::InputInt("Great Fortitude Slot", &slot, 1, 1))
		{ if (slot < 1) slot = 1; if (slot > 9) slot = 9; bk.dwGrFortSkillSlot = (DWORD)slot; }
	}

	int dist = (int)bk.dwAttackDistance;
	ImGui::SetNextItemWidth(100);
	if (ImGui::InputInt("Attack Distance (px)", &dist, 10, 50))
	{ if (dist < 10) dist = 10; if (dist > 500) dist = 500; bk.dwAttackDistance = (DWORD)dist; }
}

void CImGuiOverlay::RenderClassSoulMaster()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);
	SMSettings& sm = s->sm;

	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Soul Master Settings");

	int attackSlot = (int)sm.dwAttackSkillSlot;
	ImGui::SetNextItemWidth(80);
	if (ImGui::InputInt("Attack Skill Slot", &attackSlot, 1, 1))
	{ if (attackSlot < 1) attackSlot = 1; if (attackSlot > 9) attackSlot = 9; sm.dwAttackSkillSlot = (DWORD)attackSlot; }

	bool bManaShld = (sm.fUseManaShld != FALSE);
	if (ImGui::Checkbox("Use Mana Shield", &bManaShld))
		sm.fUseManaShld = bManaShld ? TRUE : FALSE;
	if (bManaShld)
	{
		int slot = (int)sm.dwManaShldSkillSlot;
		ImGui::SetNextItemWidth(80);
		if (ImGui::InputInt("Mana Shield Slot", &slot, 1, 1))
		{ if (slot < 1) slot = 1; if (slot > 9) slot = 9; sm.dwManaShldSkillSlot = (DWORD)slot; }
	}

	int dist = (int)sm.dwAttackDistance;
	ImGui::SetNextItemWidth(100);
	if (ImGui::InputInt("Attack Distance (px)", &dist, 10, 50))
	{ if (dist < 10) dist = 10; if (dist > 500) dist = 500; sm.dwAttackDistance = (DWORD)dist; }
}

void CImGuiOverlay::RenderClassMagicGladiator()
{
	if (!m_pSettings) return;
	ClickerSettings* s = (*m_pSettings);
	MGSettings& mg = s->mg;

	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Magic Gladiator Settings");

	int dirTime = (int)mg.dwChangeDirTime;
	ImGui::SetNextItemWidth(100);
	if (ImGui::InputInt("Direction Change (ms)", &dirTime, 500, 1000))
	{ if (dirTime < 500) dirTime = 500; if (dirTime > 30000) dirTime = 30000; mg.dwChangeDirTime = (DWORD)dirTime; }
}

// ============================================================================
// History Window
// ============================================================================

void CImGuiOverlay::RenderHistoryWindow()
{
	ImGui::SetNextWindowSize(ImVec2(480, 420), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(120, 70), ImGuiCond_FirstUseEver);

	bool bOpen = m_bShowHistory;
	if (ImGui::Begin("History Action###History", &bOpen, ImGuiWindowFlags_NoSavedSettings))
	{
		// Tab bar: Items | Kill Count
		if (ImGui::BeginTabBar("##HistoryTabs"))
		{
			if (ImGui::BeginTabItem("Items"))
			{
				m_nHistoryTab = 0;

				// ---- Zen summary at the top ----
				{
					// Format zen total with suffix (k, m, b)
					char szZen[64];
					if (m_sessionStats.ullZenTotal >= 1000000000ULL)
						_snprintf(szZen, sizeof(szZen), "%.1fb", (double)m_sessionStats.ullZenTotal / 1000000000.0);
					else if (m_sessionStats.ullZenTotal >= 1000000ULL)
						_snprintf(szZen, sizeof(szZen), "%.1fm", (double)m_sessionStats.ullZenTotal / 1000000.0);
					else if (m_sessionStats.ullZenTotal >= 1000ULL)
						_snprintf(szZen, sizeof(szZen), "%.1fk", (double)m_sessionStats.ullZenTotal / 1000.0);
					else
						_snprintf(szZen, sizeof(szZen), "%I64u", m_sessionStats.ullZenTotal);
					szZen[sizeof(szZen) - 1] = '\0';

					// Right-align zen display
					char szLabel[80];
					_snprintf(szLabel, sizeof(szLabel), "Zen: %s", szZen);
					szLabel[sizeof(szLabel) - 1] = '\0';
					float textWidth = ImGui::CalcTextSize(szLabel).x;
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - textWidth - ImGui::GetStyle().WindowPadding.x);
					ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "%s", szLabel);
				}

				ImGui::Separator();

				// ---- Build filtered item list (exclude Zen entries) ----
				// Items are displayed newest first
				std::vector<int> vItemIndices;
				for (int i = (int)m_vHistory.size() - 1; i >= 0; --i)
				{
					// Zen entries start with "Zen '"
					if (m_vHistory[i].sItem.compare(0, ZEN_ENTRY_PREFIX_LEN, ZEN_ENTRY_PREFIX) != 0)
						vItemIndices.push_back(i);
				}

				int nTotalItems = (int)vItemIndices.size();
				int nTotalPages = (nTotalItems + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
				if (nTotalPages < 1) nTotalPages = 1;
				if (m_nHistoryPage >= nTotalPages) m_nHistoryPage = nTotalPages - 1;
				if (m_nHistoryPage < 0) m_nHistoryPage = 0;

				if (nTotalItems == 0)
				{
					ImGui::TextDisabled("No items picked up yet.");
				}
				else
				{
					// ---- Item table ----
					if (ImGui::BeginTable("##ItemsTable", 1,
						ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg,
						ImVec2(0, ImGui::GetContentRegionAvail().y - 60)))
					{
						ImGui::TableSetupColumn("Items Obtained", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableHeadersRow();

						int nStart = m_nHistoryPage * ITEMS_PER_PAGE;
						int nEnd = nStart + ITEMS_PER_PAGE;
						if (nEnd > nTotalItems) nEnd = nTotalItems;

						for (int i = nStart; i < nEnd; ++i)
						{
							int idx = vItemIndices[i];
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TextUnformatted(m_vHistory[idx].sItem.c_str());

							// Hover tooltip: show item name (image placeholder)
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "%s", m_vHistory[idx].sItem.c_str());
								ImGui::TextDisabled("Time: %s", m_vHistory[idx].sTime.c_str());
								ImGui::EndTooltip();
							}
						}

						ImGui::EndTable();
					}
				}

				// ---- Pagination ----
				ImGui::Spacing();
				{
					float btnSize = 24.0f;
					float spacing = ImGui::GetStyle().ItemSpacing.x;

					// Page number buttons
					for (int p = 0; p < nTotalPages && p < MAX_PAGE_BUTTONS; ++p)
					{
						if (p > 0) ImGui::SameLine(0, 2.0f);

						char szPage[8];
						_snprintf(szPage, sizeof(szPage), "%d", p + 1);
						szPage[sizeof(szPage) - 1] = '\0';

						bool bSelected = (p == m_nHistoryPage);
						if (bSelected)
							ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

						if (ImGui::Button(szPage, ImVec2(btnSize, 0)))
							m_nHistoryPage = p;

						if (bSelected)
							ImGui::PopStyleColor();
					}

					// Prev / Next / Last buttons
					ImGui::SameLine(0, spacing);
					if (ImGui::Button("Prev"))
					{
						if (m_nHistoryPage > 0) m_nHistoryPage--;
					}
					ImGui::SameLine(0, 2.0f);
					if (ImGui::Button("Next"))
					{
						if (m_nHistoryPage < nTotalPages - 1) m_nHistoryPage++;
					}
					ImGui::SameLine(0, 2.0f);
					if (ImGui::Button("Last"))
					{
						m_nHistoryPage = nTotalPages - 1;
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Kill Count"))
			{
				m_nHistoryTab = 1;
				ImGui::Spacing();

				// ---- Clicker Run Time ----
				{
					unsigned long sec = m_sessionStats.ulRuntimeSeconds;
					unsigned long hrs = sec / 3600;
					unsigned long mins = (sec % 3600) / 60;
					unsigned long secs = sec % 60;

					char szTime[64];
					if (hrs > 0)
						_snprintf(szTime, sizeof(szTime), "%luh %lum %lus", hrs, mins, secs);
					else
						_snprintf(szTime, sizeof(szTime), "%lum %lus", mins, secs);
					szTime[sizeof(szTime) - 1] = '\0';

					ImGui::Text("Clicker Run Time:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%s", szTime);
				}

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				// ---- Monster Killer ----
				ImGui::Text("Monster Killer:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%d", m_sessionStats.nKillCount);

				ImGui::Spacing();

				// ---- Item Obtained ----
				ImGui::Text("Item Obtained:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%d", m_sessionStats.nItemCount);

				ImGui::Spacing();

				// ---- Exp Gained ----
				ImGui::Text("Exp Gained:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "%I64u", m_sessionStats.ullExpGained);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				// ---- Zen Total ----
				ImGui::Text("Zen Gained:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "%I64u", m_sessionStats.ullZenTotal);

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		// Close button at bottom
		ImGui::Spacing();
		float btnWidth = 80.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - btnWidth) * 0.5f);
		if (ImGui::Button("Close", ImVec2(btnWidth, 0)))
			m_bShowHistory = false;
	}
	ImGui::End();

	if (!bOpen)
		m_bShowHistory = false;
}
