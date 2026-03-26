#include "stdafx.h"
#include "ImGuiOverlay.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_impl_win32.h"
#include "version.h"
#include <cstdio>

// Forward-declare the ImGui Win32 WndProc handler (defined in imgui_impl_win32.cpp).
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// History dialog constants
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
	, m_hInstance(NULL)
	, m_hNormalCursor(NULL)
	, m_hTextCursor(NULL)
	, m_hLinkCursor(NULL)
	, m_hBusyCursor(NULL)
	, m_hWibCursor(NULL)
	, m_bCursorForced(false)
	, m_nSavedCursorCount(0)
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
	, m_dwHistoryFilter(HISTORY_FILTER_ALL)
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

bool CImGuiOverlay::Initialize(HWND hwndGame, HINSTANCE hInstance)
{
	if (m_bInitialized)
		return true;

	m_hwndGame = hwndGame;
	m_hInstance = hInstance;

	// Create ImGui context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Disable automatic .ini saving — we don't need persistent layout.
	io.IniFilename = NULL;
	// Tell ImGui NOT to change the OS cursor itself — we handle it.
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	// Apply dark-gold MU Online theme.
	SetupMuThemeStyle();

	// Initialize platform + renderer backends.
	ImGui_ImplWin32_Init(hwndGame);
	ImGui_ImplOpenGL2_Init();

	// Load MU Online game cursors from embedded resources.
	if (hInstance != NULL)
	{
		m_hNormalCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_NORMAL_CURSOR),
			IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
		m_hTextCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_TEXT_CURSOR),
			IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
		m_hLinkCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_LINK_CURSOR),
			IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
		m_hBusyCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_BUSY_CURSOR),
			IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
		m_hWibCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_WIB_CURSOR),
			IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	}

	m_bInitialized = true;
	return true;
}

void CImGuiOverlay::Shutdown()
{
	if (!m_bInitialized)
		return;

	// Restore hardware cursor display count if we forced it visible.
	if (m_bCursorForced)
	{
		while (::ShowCursor(FALSE) > m_nSavedCursorCount)
			;
		m_bCursorForced = false;
	}

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Destroy loaded game cursors.
	if (m_hNormalCursor) { DestroyCursor(m_hNormalCursor); m_hNormalCursor = NULL; }
	if (m_hTextCursor)   { DestroyCursor(m_hTextCursor);   m_hTextCursor   = NULL; }
	if (m_hLinkCursor)   { DestroyCursor(m_hLinkCursor);   m_hLinkCursor   = NULL; }
	if (m_hBusyCursor)   { DestroyCursor(m_hBusyCursor);   m_hBusyCursor   = NULL; }
	if (m_hWibCursor)    { DestroyCursor(m_hWibCursor);    m_hWibCursor    = NULL; }

	m_bInitialized = false;
}

void CImGuiOverlay::Render()
{
	if (!m_bInitialized)
		return;

	// Start the ImGui frame.
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// HUD bar is always visible when the game window is active.
	// Note: m_bCharSelected is not checked here because the packet-based
	// detection (WM_CHAR_SELECTED) may not fire if the DLL is loaded after
	// the character was already selected.  The overlay itself is only
	// initialized during SwapBuffers, so the game is already rendering.
	if (m_bGameActive)
		RenderHUD();

	// Popup windows.
	if (m_bShowSettings)
		RenderSettingsWindow();

	if (m_bShowHistory)
		RenderHistoryWindow();

	// Manage hardware cursor visibility.  Must be called after all
	// ImGui windows are submitted so that io.WantCaptureMouse reflects
	// the current hover state, but before EndFrame.
	UpdateCursorVisibility();

	// Finalize and submit draw data.
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
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

	// Override OS cursor with MU Online game cursor when the overlay is
	// capturing the mouse (HUD buttons or Settings/History windows open).
	if (uMsg == WM_SETCURSOR && LOWORD(lParam) == HTCLIENT)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse)
		{
			// Ensure the cursor display counter is non-negative so the
			// hardware cursor is visible.  MU Online calls ShowCursor(FALSE)
			// which can decrement the global counter below zero, hiding the
			// cursor even when a valid shape is set via SetCursor.
			// Note: UpdateCursorVisibility() handles the initial force-visible
			// on state transition; this probe handles the game re-hiding the
			// cursor between frames (single decrement).
			int nCount = ::ShowCursor(TRUE);   // probe: returns count after increment
			if (nCount > 1)
				::ShowCursor(FALSE);           // already visible, undo the probe

			ApplyGameCursor();
			return TRUE;
		}
	}

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
// Game Cursor Mapping
// ============================================================================

void CImGuiOverlay::ApplyGameCursor()
{
	// Map the current ImGui logical cursor to the matching MU Online game cursor.
	// Falls back to Windows system cursors if the .cur resource was not loaded.
	ImGuiMouseCursor imCur = ImGui::GetMouseCursor();

	HCURSOR hCur = NULL;
	switch (imCur)
	{
	case ImGuiMouseCursor_Arrow:
		hCur = m_hNormalCursor;
		break;
	case ImGuiMouseCursor_TextInput:
		hCur = m_hTextCursor;
		break;
	case ImGuiMouseCursor_Hand:
		hCur = m_hLinkCursor;
		break;
	case ImGuiMouseCursor_ResizeAll:
	case ImGuiMouseCursor_ResizeEW:
	case ImGuiMouseCursor_ResizeNS:
	case ImGuiMouseCursor_ResizeNESW:
	case ImGuiMouseCursor_ResizeNWSE:
		hCur = m_hWibCursor;
		break;
	case ImGuiMouseCursor_NotAllowed:
		hCur = m_hBusyCursor;
		break;
	default:
		hCur = m_hNormalCursor;
		break;
	}

	if (hCur != NULL)
		::SetCursor(hCur);
	else
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
}


// ============================================================================
// Cursor visibility management
// ============================================================================

/**
 * \brief Manage the hardware cursor display counter around ImGui mouse capture.
 *
 *        MU Online hides the hardware cursor via ShowCursor(FALSE) and
 *        renders its own cursor through OpenGL.  When the ImGui overlay
 *        captures the mouse (io.WantCaptureMouse) the game's software
 *        cursor is drawn behind our overlay, making it invisible.
 *
 *        On transition to captured:
 *          - Snapshot the current ShowCursor display count.
 *          - Force the count to >= 0 (visible) so the hardware cursor set
 *            by ApplyGameCursor() is actually shown.
 *
 *        On transition to released:
 *          - Restore the display count to the snapshotted value so the
 *            game's own cursor management is not disrupted.
 *
 *        This follows the same pattern used by CHistoryDialog and
 *        CUnifiedSettingsDlg (ShowCursor force-visible / restore loops).
 */
void CImGuiOverlay::UpdateCursorVisibility()
{
	ImGuiIO& io = ImGui::GetIO();
	bool bNeedVisible = io.WantCaptureMouse;

	if (bNeedVisible && !m_bCursorForced)
	{
		// Snapshot the current ShowCursor display count.
		// ShowCursor(TRUE) returns the count AFTER the increment, so
		// (return - 1) gives the count before the probe.
		int nProbe = ::ShowCursor(TRUE);
		::ShowCursor(FALSE);               // undo the probe
		m_nSavedCursorCount = nProbe - 1;  // count before our intervention

		// Force cursor visible (display count >= 0).
		// The loop exits when ShowCursor(TRUE) returns >= 1, meaning the
		// count after increment is at least 1 — cursor is visible.
		while (::ShowCursor(TRUE) < 1)
			;

		m_bCursorForced = true;
	}
	else if (!bNeedVisible && m_bCursorForced)
	{
		// Restore the display count to the saved value.
		while (::ShowCursor(FALSE) > m_nSavedCursorCount)
			;

		m_bCursorForced = false;
	}
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

	// Semi-transparent background matching MuHelper bar panel.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3, 3));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.05f, 0.03f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.47f, 0.39f, 0.16f, 0.85f));

	if (ImGui::Begin("##HUD", NULL, flags))
	{
		ImDrawList* dl = ImGui::GetWindowDrawList();
		ImVec2 btnSize(24, 24);

		// === MuHelper-style button colors ===
		// Dark metallic base with gold accents, matching MU Online S3E1.
		ImVec4 clrBtnBg       = ImVec4(0.086f, 0.071f, 0.031f, 1.0f); // RGB(22,18,8)
		ImVec4 clrBtnHover    = ImVec4(0.150f, 0.125f, 0.058f, 1.0f); // RGB(38,32,15)
		ImVec4 clrBtnActive   = ImVec4(0.059f, 0.047f, 0.020f, 1.0f); // RGB(15,12,5)
		ImVec4 clrBorder      = ImVec4(0.471f, 0.392f, 0.165f, 1.0f); // RGB(120,100,42) gold
		ImVec4 clrBorderHover = ImVec4(0.863f, 0.745f, 0.314f, 1.0f); // RGB(220,190,80) bright gold

		// ---- Settings Button (gear) ----
		ImGui::PushStyleColor(ImGuiCol_Button, clrBtnBg);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, clrBtnHover);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, clrBtnActive);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.82f, 0.71f, 0.27f, 1.0f)); // gold text

		ImVec2 posBeforeBtn = ImGui::GetCursorScreenPos();
		bool bSettingsClicked = ImGui::Button("S##Settings", btnSize);

		// Draw gold border around button (MuHelper style outer glow)
		{
			ImVec2 pMin = posBeforeBtn;
			ImVec2 pMax = ImVec2(pMin.x + btnSize.x, pMin.y + btnSize.y);
			bool bHov = ImGui::IsItemHovered();
			ImU32 borderCol = bHov
				? IM_COL32(220, 190, 80, 255)
				: IM_COL32(120, 100, 42, 200);
			dl->AddRect(pMin, pMax, borderCol, 3.0f, 0, 1.0f);
			// Inner highlight on hover (specular top edge)
			if (bHov)
				dl->AddLine(ImVec2(pMin.x + 2, pMin.y + 1),
				            ImVec2(pMax.x - 2, pMin.y + 1),
				            IM_COL32(180, 155, 65, 160));
		}

		if (bSettingsClicked)
		{
			if (m_pfnOnSettings)
				m_pfnOnSettings(m_pSettingsData);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Settings (F9)");

		ImGui::PopStyleColor(4);
		ImGui::SameLine();

		// ---- Start / Stop Button ----
		if (m_bClickerRunning)
		{
			// Stop state — red-tinted metallic
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.35f, 0.10f, 0.06f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  ImVec4(0.50f, 0.15f, 0.08f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,   ImVec4(0.25f, 0.07f, 0.04f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text,           ImVec4(1.0f, 0.45f, 0.35f, 1.0f));

			posBeforeBtn = ImGui::GetCursorScreenPos();
			bool bStopClicked = ImGui::Button("X##Stop", btnSize);
			{
				ImVec2 pMin = posBeforeBtn;
				ImVec2 pMax = ImVec2(pMin.x + btnSize.x, pMin.y + btnSize.y);
				bool bHov = ImGui::IsItemHovered();
				dl->AddRect(pMin, pMax,
					bHov ? IM_COL32(220, 80, 60, 255) : IM_COL32(140, 50, 35, 200),
					3.0f, 0, 1.0f);
				if (bHov)
					dl->AddLine(ImVec2(pMin.x + 2, pMin.y + 1),
					            ImVec2(pMax.x - 2, pMin.y + 1),
					            IM_COL32(200, 100, 80, 140));
			}
			if (bStopClicked)
			{
				if (m_pfnOnStartStop)
					m_pfnOnStartStop(m_pStartStopData);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Stop Clicker (F5)");

			ImGui::PopStyleColor(4);
		}
		else
		{
			// Start state — green-tinted metallic
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.06f, 0.22f, 0.06f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  ImVec4(0.10f, 0.35f, 0.10f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,   ImVec4(0.04f, 0.15f, 0.04f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text,           ImVec4(0.45f, 0.90f, 0.35f, 1.0f));

			posBeforeBtn = ImGui::GetCursorScreenPos();
			bool bStartClicked = ImGui::Button(">##Start", btnSize);
			{
				ImVec2 pMin = posBeforeBtn;
				ImVec2 pMax = ImVec2(pMin.x + btnSize.x, pMin.y + btnSize.y);
				bool bHov = ImGui::IsItemHovered();
				dl->AddRect(pMin, pMax,
					bHov ? IM_COL32(80, 200, 60, 255) : IM_COL32(40, 120, 35, 200),
					3.0f, 0, 1.0f);
				if (bHov)
					dl->AddLine(ImVec2(pMin.x + 2, pMin.y + 1),
					            ImVec2(pMax.x - 2, pMin.y + 1),
					            IM_COL32(100, 180, 80, 140));
			}
			if (bStartClicked)
			{
				if (m_pfnOnStartStop)
					m_pfnOnStartStop(m_pStartStopData);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Start Clicker (F5)");

			ImGui::PopStyleColor(4);
		}

		ImGui::SameLine();

		// ---- History Button ----
		ImGui::PushStyleColor(ImGuiCol_Button, clrBtnBg);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, clrBtnHover);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, clrBtnActive);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.82f, 0.71f, 0.27f, 1.0f));

		posBeforeBtn = ImGui::GetCursorScreenPos();
		bool bHistoryClicked = ImGui::Button("H##History", btnSize);
		{
			ImVec2 pMin = posBeforeBtn;
			ImVec2 pMax = ImVec2(pMin.x + btnSize.x, pMin.y + btnSize.y);
			bool bHov = ImGui::IsItemHovered();
			ImU32 borderCol = bHov
				? IM_COL32(220, 190, 80, 255)
				: IM_COL32(120, 100, 42, 200);
			dl->AddRect(pMin, pMax, borderCol, 3.0f, 0, 1.0f);
			if (bHov)
				dl->AddLine(ImVec2(pMin.x + 2, pMin.y + 1),
				            ImVec2(pMax.x - 2, pMin.y + 1),
				            IM_COL32(180, 155, 65, 160));
		}

		if (bHistoryClicked)
		{
			if (m_pfnOnHistory)
				m_pfnOnHistory(m_pHistoryData);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Pickup History (Shift+F9)");

		ImGui::PopStyleColor(4);
	}
	ImGui::End();

	ImGui::PopStyleColor(2); // WindowBg, Border
	ImGui::PopStyleVar(3);   // Padding, Spacing, FrameRounding
}

// ============================================================================
// Settings Window
// ============================================================================

void CImGuiOverlay::RenderSettingsWindow()
{
	if (!m_pSettings)
		return;

	ImGui::SetNextWindowSize(ImVec2(480, 460), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(100, 50), ImGuiCond_FirstUseEver);

	bool bOpen = m_bShowSettings;
	if (ImGui::Begin("MU AutoClicker Settings###Settings", &bOpen, ImGuiWindowFlags_NoSavedSettings))
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

		// Version footer (matches old dialog: "LordOfMU AutoClicker v2.x (AVANTA+ELITE)")
		ImGui::Spacing();
		ImGui::Separator();
		{
			const char* szFooter = __SOFTWARE_PRODUCT_NAME " v" __SOFTWARE_VERSION_STR;
			float footerW = ImGui::CalcTextSize(szFooter).x;
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - footerW) * 0.5f);
			ImGui::TextDisabled("%s", szFooter);
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
	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Auto Functions");
	ImGui::Separator();

	// Enable auto-pick-up (press 'SPACE')
	bool bAutoPick = (s->all.fAutoPick != FALSE);
	if (ImGui::Checkbox("Enable auto-pick-up (press 'SPACE')", &bAutoPick))
		s->all.fAutoPick = bAutoPick ? TRUE : FALSE;

	// Enable auto-repair
	bool bAutoRepair = (s->all.fAutoRepair != FALSE);
	if (ImGui::Checkbox("Enable auto-repair", &bAutoRepair))
		s->all.fAutoRepair = bAutoRepair ? TRUE : FALSE;

	// Enable auto-heal (press 'Q')
	bool bAutoHeal = (s->all.fAutoLife != FALSE);
	if (ImGui::Checkbox("Enable auto-heal (press 'Q')", &bAutoHeal))
		s->all.fAutoLife = bAutoHeal ? TRUE : FALSE;

	// Auto-heal time
	if (bAutoHeal)
	{
		ImGui::Indent(28.0f);
		// Preset heal times matching old dialog combobox (0,1,3,5,7,10,15 seconds)
		static const int s_nHealPresets[] = { 0, 1000, 3000, 5000, 7000, 10000, 15000 };
		static const char* s_szHealLabels[] = { "0 sec.", "1 sec.", "3 sec.", "5 sec.", "7 sec.", "10 sec.", "15 sec." };
		static const int s_nHealPresetCount = sizeof(s_nHealPresets) / sizeof(s_nHealPresets[0]);

		int iSelected = -1;
		for (int i = 0; i < s_nHealPresetCount; ++i)
		{
			if ((int)s->all.dwHealTime == s_nHealPresets[i])
			{
				iSelected = i;
				break;
			}
		}

		// If current value doesn't match any preset, snap to nearest one
		if (iSelected < 0)
		{
			iSelected = 0;
			int nBestDiff = abs((int)s->all.dwHealTime - s_nHealPresets[0]);
			for (int i = 1; i < s_nHealPresetCount; ++i)
			{
				int d = abs((int)s->all.dwHealTime - s_nHealPresets[i]);
				if (d < nBestDiff) { nBestDiff = d; iSelected = i; }
			}
			s->all.dwHealTime = (DWORD)s_nHealPresets[iSelected];
		}

		ImGui::SetNextItemWidth(100);
		if (ImGui::Combo("Auto-heal time", &iSelected, s_szHealLabels, s_nHealPresetCount))
		{
			s->all.dwHealTime = (DWORD)s_nHealPresets[iSelected];
		}
		ImGui::Unindent(28.0f);
	}

	// Stop picking when inventory full
	bool bAutoReOff = (s->all.fAutoReOff != FALSE);
	if (ImGui::Checkbox("Stop pick on inventory full", &bAutoReOff))
		s->all.fAutoReOff = bAutoReOff ? TRUE : FALSE;

	// Exit game at level 400
	bool bExitAtLvl = (s->all.fExitAtLvl400 != FALSE);
	if (ImGui::Checkbox("Exit game at level 400", &bExitAtLvl))
		s->all.fExitAtLvl400 = bExitAtLvl ? TRUE : FALSE;

	ImGui::Spacing();
	ImGui::TextColored(ImVec4(0.82f, 0.71f, 0.27f, 1.0f), "Stealth Options (if s.o. approaches your character)");
	ImGui::Separator();

	// Anti-AFK / Stealth sub-options stored as bit flags in fAntiAFKProtect:
	//   bit 0 (1) = Auto-speak
	//   bit 1 (2) = Stop zen pick-up
	//   bit 2 (4) = Stop move-to-pick
	//   bit 3 (8) = Stop all pick-up
	int afk = s->all.fAntiAFKProtect;

	bool bAutoSpeak = (afk & 1) != 0;
	bool bStopZen   = (afk & 2) != 0;
	bool bStopMove  = (afk & 4) != 0;
	bool bStopAll   = (afk & 8) != 0;

	// When "Stop all pick-up" is ON, the sub-options Stop-zen and Stop-move
	// are disabled (greyed out) — matching old dialog behavior at
	// UnifiedSettingsDlg.cpp:713-715.
	if (ImGui::Checkbox("Auto-speak", &bAutoSpeak))
	{
		afk = (afk & ~1) | (bAutoSpeak ? 1 : 0);
		s->all.fAntiAFKProtect = afk;
	}
	ImGui::SameLine(260);
	ImGui::BeginDisabled(bStopAll);
	if (ImGui::Checkbox("Stop zen pick-up", &bStopZen))
	{
		afk = (afk & ~2) | (bStopZen ? 2 : 0);
		s->all.fAntiAFKProtect = afk;
	}
	ImGui::EndDisabled();

	ImGui::BeginDisabled(bStopAll);
	if (ImGui::Checkbox("Stop move-to-pick", &bStopMove))
	{
		afk = (afk & ~4) | (bStopMove ? 4 : 0);
		s->all.fAntiAFKProtect = afk;
	}
	ImGui::EndDisabled();
	ImGui::SameLine(260);
	if (ImGui::Checkbox("Stop all pick-up", &bStopAll))
	{
		afk = (afk & ~8) | (bStopAll ? 8 : 0);
		s->all.fAntiAFKProtect = afk;
	}
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
	if (ImGui::Begin("History Action###History", &bOpen,
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar))
	{
		// ---- Menu bar: File > Settings (filter checkboxes) ----
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("Settings"))
				{
					// Human-readable names for each filter category
					static const char* s_szFilterNames[HISTORY_CAT_COUNT] =
					{
						"ItemsPick",      // HISTORY_CAT_ITEMS_PICK
						"ItemsDrop",      // HISTORY_CAT_ITEMS_DROP
						"ZenPick",        // HISTORY_CAT_ZEN_PICK
						"ZenDrop",        // HISTORY_CAT_ZEN_DROP
						"KillMonster",    // HISTORY_CAT_KILL_MONSTER
						"Server",         // HISTORY_CAT_SERVER
						"Client",         // HISTORY_CAT_CLIENT
					};

					for (int i = 0; i < HISTORY_CAT_COUNT; ++i)
					{
						bool bChecked = (m_dwHistoryFilter & (1u << i)) != 0;
						if (ImGui::MenuItem(s_szFilterNames[i], NULL, &bChecked))
						{
							if (bChecked)
								m_dwHistoryFilter |= (1u << i);
							else
								m_dwHistoryFilter &= ~(1u << i);

							// Reset pagination when filter changes
							m_nHistoryPage = 0;
						}
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Select All"))
					{
						m_dwHistoryFilter = HISTORY_FILTER_ALL;
						m_nHistoryPage = 0;
					}
					if (ImGui::MenuItem("Clear All"))
					{
						m_dwHistoryFilter = 0;
						m_nHistoryPage = 0;
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Tab bar: Items | Kill Count
		if (ImGui::BeginTabBar("##HistoryTabs"))
		{
			if (ImGui::BeginTabItem("Items"))
			{
				m_nHistoryTab = 0;

				// ---- Zen Obtained: show both pickup count and total amount ----
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

					// Zen Obtained: show both count and total amount
					ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "Zen Obtained:");
					ImGui::SameLine();
					ImGui::Text("%d", m_sessionStats.nZenPickupCount);
					ImGui::SameLine();
					ImGui::TextDisabled("(Total: %s)", szZen);
				}

				ImGui::Separator();

				// ---- Build item list (filtered by enabled categories) ----
				// Items are displayed newest first
				std::vector<int> vItemIndices;
				for (int i = (int)m_vHistory.size() - 1; i >= 0; --i)
				{
					int cat = m_vHistory[i].nCategory;
					if (cat >= 0 && cat < HISTORY_CAT_COUNT && (m_dwHistoryFilter & (1u << cat)))
						vItemIndices.push_back(i);
				}

				int nTotalItems = (int)vItemIndices.size();
				int nTotalPages = (nTotalItems + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
				if (nTotalPages < 1) nTotalPages = 1;
				if (m_nHistoryPage >= nTotalPages) m_nHistoryPage = nTotalPages - 1;
				if (m_nHistoryPage < 0) m_nHistoryPage = 0;

				if (nTotalItems == 0)
				{
					if (m_dwHistoryFilter == 0)
						ImGui::TextDisabled("All filters are disabled. Use File > Settings to enable.");
					else
						ImGui::TextDisabled("No matching entries yet.");
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
							int nCat = m_vHistory[idx].nCategory;

							ImGui::TableNextRow();
							ImGui::TableNextColumn();

							// Color entries by category
							ImVec4 color;
							switch (nCat)
							{
							case HISTORY_CAT_ZEN_PICK:
							case HISTORY_CAT_ZEN_DROP:
								color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);  // Gold
								break;
							case HISTORY_CAT_KILL_MONSTER:
								color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);   // Red
								break;
							case HISTORY_CAT_ITEMS_DROP:
								color = ImVec4(0.4f, 0.8f, 1.0f, 1.0f);   // Light blue
								break;
							case HISTORY_CAT_SERVER:
								color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);   // Gray
								break;
							case HISTORY_CAT_CLIENT:
								color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);   // Light gray
								break;
							default: // HISTORY_CAT_ITEMS_PICK and others
								color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // White (default)
								break;
							}

							ImGui::TextColored(color, "%s", m_vHistory[idx].sItem.c_str());

							// Hover tooltip: show item name and timestamp
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::TextColored(color, "%s", m_vHistory[idx].sItem.c_str());
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

				// ---- Zen Obtained (count + total) ----
				ImGui::Text("Zen Obtained:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "%d", m_sessionStats.nZenPickupCount);

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
