#include "stdafx.h"
#include "UnifiedSettingsDlg.h"
#include "version.h"
#include <commctrl.h>
#include <uxtheme.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "uxtheme.lib")

static const DWORD s_arrHealTimes[] = {0, 1000, 3000, 5000, 7000, 10000, 15000};

static const int CHECKBOX_BOX_SIZE = 13;  // Default checkbox box size in pixels


/**
 * \brief Dialog proc for tab child pages. Forwards WM_COMMAND and
 *        WM_CTLCOLORxxx messages to the parent (CUnifiedSettingsDlg)
 *        so that a single message map handles all tab controls.
 */
INT_PTR CALLBACK CUnifiedSettingsDlg::TabPageDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		return (INT_PTR)::SendMessage(::GetParent(hDlg), uMsg, wParam, lParam);

	case WM_NOTIFY:
		return (INT_PTR)::SendMessage(::GetParent(hDlg), uMsg, wParam, lParam);

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
		return (INT_PTR)::SendMessage(::GetParent(hDlg), uMsg, wParam, lParam);

	case WM_DRAWITEM:
		return (INT_PTR)::SendMessage(::GetParent(hDlg), uMsg, wParam, lParam);

	case WM_ERASEBKGND:
		return (INT_PTR)::SendMessage(::GetParent(hDlg), uMsg, wParam, lParam);

	case WM_SETCURSOR:
		return (INT_PTR)::SendMessage(::GetParent(hDlg), uMsg, wParam, lParam);
	}

	return FALSE;
}


CUnifiedSettingsDlg::CUnifiedSettingsDlg(CClickerSettings& cSettings)
	: m_cSettings(cSettings)
	, m_cDarkLordSettings(m_cSettings->dl)
	, m_cEElfSettings(m_cSettings->ee)
	, m_cAElfSettings(m_cSettings->ae)
	, m_cBKSettings(m_cSettings->bk)
	, m_cMGSettings(m_cSettings->mg)
	, m_cSMSettings(m_cSettings->sm)
{
	m_hOldCursor = 0;
	m_iShowCursor = 0;
	m_nCurrentTab = 0;
	m_hwndTabGeneral = NULL;
	m_hwndTabClass = NULL;
	m_hwndTabPickup = NULL;
	m_hEditBrush = NULL;
}

CUnifiedSettingsDlg::~CUnifiedSettingsDlg()
{
	if (m_hEditBrush != NULL)
	{
		DeleteObject(m_hEditBrush);
		m_hEditBrush = NULL;
	}
}


LRESULT CUnifiedSettingsDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CUnifiedSettingsDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	bHandled = TRUE;

	// Initialize theme
	m_cTheme.Initialize(_AtlBaseModule.GetModuleInstance());

	// Center dialog within parent
	RECT rc = {0};
	RECT rcParent = {0};

	CWindow wndParent = GetParent();

	GetWindowRect(&rc);
	wndParent.GetClientRect(&rcParent);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int xPos = ((rcParent.right - rcParent.left) - width) / 2;
	int yPos = ((rcParent.bottom - rcParent.top) - height) / 2;

	MoveWindow(xPos, yPos, width, height, TRUE);

	// Setup Tab Control
	HWND hwndTab = GetDlgItem(IDC_SETTINGS_TAB);
	if (hwndTab != NULL)
	{
		TCITEM tci;
		memset(&tci, 0, sizeof(tci));
		tci.mask = TCIF_TEXT;

		tci.pszText = (LPTSTR)_T("General");
		TabCtrl_InsertItem(hwndTab, 0, &tci);

		tci.pszText = (LPTSTR)_T("Class");
		TabCtrl_InsertItem(hwndTab, 1, &tci);

		tci.pszText = (LPTSTR)_T("Pickup");
		TabCtrl_InsertItem(hwndTab, 2, &tci);

		if (m_cTheme.GetTabFont() != NULL)
			::SendMessage(hwndTab, WM_SETFONT, (WPARAM)m_cTheme.GetTabFont(), TRUE);

		ThemeTabControl(hwndTab);
	}

	// Get tab display area for child positioning
	RECT rcTab = {0};
	if (hwndTab != NULL)
	{
		::GetWindowRect(hwndTab, &rcTab);
		ScreenToClient(&rcTab);
		TabCtrl_AdjustRect(hwndTab, FALSE, &rcTab);
	}

	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();

	// Create the General child panel
	m_hwndTabGeneral = CreateDialogParam(hInst,
		MAKEINTRESOURCE(IDD_TAB_GENERAL), m_hWnd, TabPageDlgProc, 0);

	if (m_hwndTabGeneral != NULL)
	{
		::SetWindowPos(m_hwndTabGeneral, NULL,
			rcTab.left, rcTab.top,
			rcTab.right - rcTab.left, rcTab.bottom - rcTab.top,
			SWP_NOZORDER);
	}

	// Setup heal time combo on General tab
	HWND hwndHealTime = NULL;
	if (m_hwndTabGeneral != NULL)
		hwndHealTime = ::GetDlgItem(m_hwndTabGeneral, IDC_AUTOHEALTIME);

	if (hwndHealTime != NULL)
	{
		for (int i = 0; i < (int)(sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0])); ++i)
		{
			TCHAR szText[256] = {0};
			_stprintf(szText, _T("%d"), s_arrHealTimes[i] / 1000);
			::SendMessage(hwndHealTime, CB_ADDSTRING, 0, (LPARAM)szText);
		}
	}

	// Create the Class child panel
	m_hwndTabClass = CreateDialogParam(hInst,
		MAKEINTRESOURCE(IDD_TAB_CLASS), m_hWnd, TabPageDlgProc, 0);

	if (m_hwndTabClass != NULL)
	{
		::SetWindowPos(m_hwndTabClass, NULL,
			rcTab.left, rcTab.top,
			rcTab.right - rcTab.left, rcTab.bottom - rcTab.top,
			SWP_NOZORDER);

		// Setup class combobox and child class panels
		HWND hwndClassCombo = ::GetDlgItem(m_hwndTabClass, IDC_CHARCLASS);
		HWND hwndClassStatic = ::GetDlgItem(m_hwndTabClass, IDC_STATIC_CLASS);

		if (hwndClassStatic != NULL)
		{
			RECT rcCont = {0};
			::GetWindowRect(hwndClassStatic, &rcCont);
			::ScreenToClient(m_hwndTabClass, (LPPOINT)&rcCont);
			::ScreenToClient(m_hwndTabClass, ((LPPOINT)&rcCont) + 1);

			m_cDarkLordSettings.Create(m_hwndTabClass);
			m_cEElfSettings.Create(m_hwndTabClass);
			m_cAElfSettings.Create(m_hwndTabClass);
			m_cBKSettings.Create(m_hwndTabClass);
			m_cMGSettings.Create(m_hwndTabClass);
			m_cSMSettings.Create(m_hwndTabClass);

			CWindow* pDlgs[] = {&m_cDarkLordSettings, &m_cEElfSettings, &m_cAElfSettings, &m_cBKSettings, &m_cSMSettings, &m_cMGSettings, 0};
			const TCHAR* arrStrings[] = {_T("Dark Lord"), _T("Energy Elf"), _T("Agility Elf"), _T("Blade Knight"), _T("Dark Wizard"), _T("Magic Gladiator"), 0};

			HWND hwndAfter = hwndClassCombo;
			for (int i = 0; pDlgs[i] != 0; ++i)
			{
				pDlgs[i]->SetWindowPos(hwndAfter, &rcCont, 0);
				::SendMessage(hwndClassCombo, CB_ADDSTRING, 0, (LPARAM)arrStrings[i]);
			}
		}
	}

	// Create the Pickup child panel
	m_hwndTabPickup = CreateDialogParam(hInst,
		MAKEINTRESOURCE(IDD_TAB_PICKUP), m_hWnd, TabPageDlgProc, 0);

	if (m_hwndTabPickup != NULL)
	{
		::SetWindowPos(m_hwndTabPickup, NULL,
			rcTab.left, rcTab.top,
			rcTab.right - rcTab.left, rcTab.bottom - rcTab.top,
			SWP_NOZORDER);
	}

	// Create edit brush for themed edit controls
	m_hEditBrush = CreateSolidBrush(CMuTheme::ClrBtnBg());

	// Subclass all checkboxes and separators in the entire dialog hierarchy
	// for dark-gold themed drawing (EnumChildWindows is recursive)
	SubclassCheckboxes(m_hWnd);
	SubclassSeparators(m_hWnd);

	// Disable visual themes on controls so WM_CTLCOLORxxx colors take effect
	DisableChildThemes(m_hWnd);

	// Set version label
	HWND hwndVersion = GetDlgItem(IDC_VERSION_LABEL);
	if (hwndVersion != NULL)
	{
		::SetWindowText(hwndVersion, _T("LordOfMU AutoClicker v2.x"));
		if (m_cTheme.GetSmallFont() != NULL)
			::SendMessage(hwndVersion, WM_SETFONT, (WPARAM)m_cTheme.GetSmallFont(), TRUE);
	}

	// Show Pickup tab by default (F9 opens directly to Pickup settings)
	m_nCurrentTab = 2;
	ShowTab(2);

	return 1;
}


void CUnifiedSettingsDlg::ShowTab(int nTab)
{
	m_nCurrentTab = nTab;

	if (m_hwndTabGeneral != NULL)
		::ShowWindow(m_hwndTabGeneral, nTab == 0 ? SW_SHOWNOACTIVATE : SW_HIDE);
	if (m_hwndTabClass != NULL)
		::ShowWindow(m_hwndTabClass, nTab == 1 ? SW_SHOWNOACTIVATE : SW_HIDE);
	if (m_hwndTabPickup != NULL)
		::ShowWindow(m_hwndTabPickup, nTab == 2 ? SW_SHOWNOACTIVATE : SW_HIDE);
}


LRESULT CUnifiedSettingsDlg::OnTabSelChange(int, LPNMHDR pNMHDR, BOOL&)
{
	int nSel = TabCtrl_GetCurSel(pNMHDR->hwndFrom);
	ShowTab(nSel);
	return 0;
}


LRESULT CUnifiedSettingsDlg::OnShowWindow(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	if (wParam != 0)
	{
		// Re-center within parent each time the dialog is shown
		CWindow wndParent = GetParent();
		if (wndParent.IsWindow())
		{
			RECT rc = {0};
			RECT rcParent = {0};

			GetWindowRect(&rc);
			wndParent.GetClientRect(&rcParent);

			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;

			int xPos = ((rcParent.right - rcParent.left) - width) / 2;
			int yPos = ((rcParent.bottom - rcParent.top) - height) / 2;

			MoveWindow(xPos, yPos, width, height, TRUE);
		}

		InitGeneralValues();
		InitClassValues();
		InitPickupValues();

		HCURSOR hCursor = m_cTheme.GetMuCursor();
		if (hCursor == NULL)
			hCursor = LoadCursor(0, IDC_ARROW);
		m_hOldCursor = SetCursor(hCursor);

		for (m_iShowCursor = 0; ShowCursor(TRUE) < 1 && m_iShowCursor < 100; ++m_iShowCursor);

		// Open to Pickup tab by default (F9 → Pickup → Advanced Pick-up Settings)
		HWND hwndTab = GetDlgItem(IDC_SETTINGS_TAB);
		if (hwndTab != NULL)
			TabCtrl_SetCurSel(hwndTab, 2);
		ShowTab(2);
	}
	else
	{
		SetCursor(m_hOldCursor);
		for (m_iShowCursor; m_iShowCursor >= 0; m_iShowCursor--)
			ShowCursor(FALSE);
	}

	return 0;
}


LRESULT CUnifiedSettingsDlg::OnSetCursor(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (LOWORD(lParam) == HTCLIENT && m_cTheme.GetMuCursor() != NULL)
	{
		SetCursor(m_cTheme.GetMuCursor());
		bHandled = TRUE;
		return TRUE;
	}

	bHandled = FALSE;
	return FALSE;
}


void CUnifiedSettingsDlg::InitGeneralValues()
{
	if (m_hwndTabGeneral == NULL)
		return;

	::CheckDlgButton(m_hwndTabGeneral, IDC_AUTOPICK, m_cSettings->all.fAutoPick ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabGeneral, IDC_AUTOHEAL, m_cSettings->all.fAutoLife ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabGeneral, IDC_AUTOREPAIR, m_cSettings->all.fAutoRepair ? BST_CHECKED : BST_UNCHECKED);

	::CheckDlgButton(m_hwndTabGeneral, IDC_AUTOREOFF, m_cSettings->all.fAutoReOff ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabGeneral, IDC_EXITAT400, m_cSettings->all.fExitAtLvl400 ? BST_CHECKED : BST_UNCHECKED);

	::CheckDlgButton(m_hwndTabGeneral, IDC_AUTOSPEAK, (m_cSettings->all.fAntiAFKProtect & 1) ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabGeneral, IDC_STOPZEN, (m_cSettings->all.fAntiAFKProtect & 2) ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabGeneral, IDC_STOPMOVE, (m_cSettings->all.fAntiAFKProtect & 4) ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabGeneral, IDC_STOPPICK, (m_cSettings->all.fAntiAFKProtect & 8) ? BST_CHECKED : BST_UNCHECKED);

	BOOL fTemp = 0;
	OnCheckStopPick(0, 0, 0, fTemp);

	int nSel = 0;
	int nMinDelta = INT_MAX;

	for (int i = 0; i < (int)(sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0])); ++i)
	{
		int iEps = abs((int)s_arrHealTimes[i] - (int)m_cSettings->all.dwHealTime);
		if (iEps < nMinDelta)
		{
			nSel = i;
			nMinDelta = iEps;
		}
	}

	HWND hwndHealTime = ::GetDlgItem(m_hwndTabGeneral, IDC_AUTOHEALTIME);
	if (hwndHealTime != NULL)
	{
		::SendMessage(hwndHealTime, CB_SETCURSEL, (WPARAM)nSel, 0);
		::EnableWindow(hwndHealTime, m_cSettings->all.fAutoLife);
	}
}


void CUnifiedSettingsDlg::InitClassValues()
{
	if (m_hwndTabClass == NULL)
		return;

	m_cDarkLordSettings.InitValues();
	m_cAElfSettings.InitValues();
	m_cEElfSettings.InitValues();
	m_cBKSettings.InitValues();
	m_cSMSettings.InitValues();
	m_cMGSettings.InitValues();

	HWND hwndClassCombo = ::GetDlgItem(m_hwndTabClass, IDC_CHARCLASS);
	if (hwndClassCombo != NULL)
	{
		::SendMessage(hwndClassCombo, CB_SETCURSEL, (WPARAM)m_cSettings->all.dwClass, 0);
	}

	BOOL fHandled = TRUE;
	OnSelectCharClass(0, 0, 0, fHandled);
}


void CUnifiedSettingsDlg::InitPickupValues()
{
	if (m_hwndTabPickup == NULL)
		return;

	::CheckDlgButton(m_hwndTabPickup, IDC_ADV_PICKUP, m_cSettings->all.fAdvAutoPick ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_BLESS, m_cSettings->all.fAdvPickBless ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_SOUL, m_cSettings->all.fAdvPickSoul ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_LIFE, m_cSettings->all.fAdvPickLife ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_JOG, m_cSettings->all.fAdvPickGuardian ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_JOC, m_cSettings->all.fAdvPickCreation ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_CHAOS, m_cSettings->all.fAdvPickChaos ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_EXL, m_cSettings->all.fAdvPickExl ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_ZEN, m_cSettings->all.fAdvPickZen ? BST_CHECKED : BST_UNCHECKED);

	::CheckDlgButton(m_hwndTabPickup, IDC_BLESS1, m_cSettings->all.fAdvPickBlessMove ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_SOUL1, m_cSettings->all.fAdvPickSoulMove ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_LIFE1, m_cSettings->all.fAdvPickLifeMove ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_JOG1, m_cSettings->all.fAdvPickGuardianMove ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_JOC1, m_cSettings->all.fAdvPickCreationMove ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_CHAOS1, m_cSettings->all.fAdvPickChaosMove ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_EXL1, m_cSettings->all.fAdvPickExlMove ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_ZEN1, m_cSettings->all.fAdvPickZenMove ? BST_CHECKED : BST_UNCHECKED);

	::CheckDlgButton(m_hwndTabPickup, IDC_CUSTOM1, m_cSettings->all.fAdvPickCustom ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_CUSTOM2, m_cSettings->all.fAdvPickCustomMove ? BST_CHECKED : BST_UNCHECKED);
	::CheckDlgButton(m_hwndTabPickup, IDC_PICKRUNMODE, m_cSettings->all.fPickRunMode ? BST_CHECKED : BST_UNCHECKED);

	TCHAR szCustom[256] = {0};
	_sntprintf(szCustom, 255, _T("%d %d"), HIBYTE(m_cSettings->all.wPickCustomCode), LOBYTE(m_cSettings->all.wPickCustomCode));
	::SetDlgItemText(m_hwndTabPickup, IDC_EDIT_CUSTOM, szCustom);

	ApplyPickupState();
}


void CUnifiedSettingsDlg::ApplyGeneral()
{
	if (m_hwndTabGeneral == NULL)
		return;

	m_cSettings->all.fAutoPick = ::IsDlgButtonChecked(m_hwndTabGeneral, IDC_AUTOPICK) == BST_CHECKED;
	m_cSettings->all.fAutoLife = ::IsDlgButtonChecked(m_hwndTabGeneral, IDC_AUTOHEAL) == BST_CHECKED;
	m_cSettings->all.fAutoRepair = ::IsDlgButtonChecked(m_hwndTabGeneral, IDC_AUTOREPAIR) == BST_CHECKED;
	m_cSettings->all.fAutoReOff = ::IsDlgButtonChecked(m_hwndTabGeneral, IDC_AUTOREOFF) == BST_CHECKED;
	m_cSettings->all.fExitAtLvl400 = ::IsDlgButtonChecked(m_hwndTabGeneral, IDC_EXITAT400) == BST_CHECKED;

	m_cSettings->all.fAntiAFKProtect =
		((::IsDlgButtonChecked(m_hwndTabGeneral, IDC_AUTOSPEAK) == BST_CHECKED) ? 1 : 0)
		| ((::IsDlgButtonChecked(m_hwndTabGeneral, IDC_STOPZEN) == BST_CHECKED) ? 2 : 0)
		| ((::IsDlgButtonChecked(m_hwndTabGeneral, IDC_STOPMOVE) == BST_CHECKED) ? 4 : 0)
		| ((::IsDlgButtonChecked(m_hwndTabGeneral, IDC_STOPPICK) == BST_CHECKED) ? 8 : 0);

	HWND hwndHealTime = ::GetDlgItem(m_hwndTabGeneral, IDC_AUTOHEALTIME);
	int nIdx = 0;
	if (hwndHealTime != NULL)
		nIdx = (int)::SendMessage(hwndHealTime, CB_GETCURSEL, 0, 0);

	if (nIdx >= (int)(sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0])))
		nIdx = (int)(sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0])) - 1;

	if (nIdx < 0)
		nIdx = 0;

	m_cSettings->all.dwHealTime = s_arrHealTimes[nIdx];
}


void CUnifiedSettingsDlg::ApplyClass()
{
	if (m_hwndTabClass == NULL)
		return;

	m_cDarkLordSettings.Apply();
	m_cAElfSettings.Apply();
	m_cEElfSettings.Apply();
	m_cBKSettings.Apply();
	m_cSMSettings.Apply();
	m_cMGSettings.Apply();

	HWND hwndClassCombo = ::GetDlgItem(m_hwndTabClass, IDC_CHARCLASS);
	int nIdx2 = 0;
	if (hwndClassCombo != NULL)
		nIdx2 = (int)::SendMessage(hwndClassCombo, CB_GETCURSEL, 0, 0);

	if (nIdx2 > CHAR_CLASS_LAST)
		nIdx2 = 0;

	if (nIdx2 < 0)
		nIdx2 = 0;

	m_cSettings->all.dwClass = nIdx2;
}


void CUnifiedSettingsDlg::ApplyPickup()
{
	if (m_hwndTabPickup == NULL)
		return;

	m_cSettings->all.fAdvAutoPick = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_ADV_PICKUP) == BST_CHECKED;
	m_cSettings->all.fAdvPickBless = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_BLESS) == BST_CHECKED;
	m_cSettings->all.fAdvPickSoul = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_SOUL) == BST_CHECKED;
	m_cSettings->all.fAdvPickLife = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_LIFE) == BST_CHECKED;
	m_cSettings->all.fAdvPickGuardian = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_JOG) == BST_CHECKED;
	m_cSettings->all.fAdvPickCreation = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_JOC) == BST_CHECKED;
	m_cSettings->all.fAdvPickChaos = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_CHAOS) == BST_CHECKED;
	m_cSettings->all.fAdvPickExl = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_EXL) == BST_CHECKED;
	m_cSettings->all.fAdvPickZen = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_ZEN) == BST_CHECKED;

	m_cSettings->all.fAdvPickBlessMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_BLESS1) == BST_CHECKED;
	m_cSettings->all.fAdvPickSoulMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_SOUL1) == BST_CHECKED;
	m_cSettings->all.fAdvPickLifeMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_LIFE1) == BST_CHECKED;
	m_cSettings->all.fAdvPickGuardianMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_JOG1) == BST_CHECKED;
	m_cSettings->all.fAdvPickCreationMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_JOC1) == BST_CHECKED;
	m_cSettings->all.fAdvPickChaosMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_CHAOS1) == BST_CHECKED;
	m_cSettings->all.fAdvPickExlMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_EXL1) == BST_CHECKED;
	m_cSettings->all.fAdvPickZenMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_ZEN1) == BST_CHECKED;

	m_cSettings->all.fAdvPickCustom = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_CUSTOM1) == BST_CHECKED;
	m_cSettings->all.fAdvPickCustomMove = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_CUSTOM2) == BST_CHECKED;
	m_cSettings->all.fPickRunMode = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_PICKRUNMODE) == BST_CHECKED;

	TCHAR szCustom[256] = {0};
	::GetDlgItemText(m_hwndTabPickup, IDC_EDIT_CUSTOM, szCustom, 255);

	TCHAR* p1 = szCustom;
	TCHAR* p2 = 0;

	while (*p1 != 0 && *p1 == _T(' ')) p1++;
	p2 = p1;

	while (*p2 != 0 && *p2 != _T(' ')) p2++;
	while (*p2 != 0 && *p2 == _T(' ')) p2++;

	m_cSettings->all.wPickCustomCode = (WORD)(((_ttoi(p1) & 0xFF) << 8) | (_ttoi(p2) & 0xFF));
}


void CUnifiedSettingsDlg::ApplyPickupState()
{
	if (m_hwndTabPickup == NULL)
		return;

	BOOL fEnable = ::IsDlgButtonChecked(m_hwndTabPickup, IDC_ADV_PICKUP) == BST_CHECKED;
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_BLESS), fEnable);
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_SOUL), fEnable);
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_LIFE), fEnable);
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_JOG), fEnable);
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_JOC), fEnable);
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_CHAOS), fEnable);
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_EXL), fEnable);
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_ZEN), fEnable);
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_CUSTOM1), fEnable);

	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_BLESS1), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_BLESS) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_SOUL1), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_SOUL) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_LIFE1), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_LIFE) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_JOG1), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_JOG) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_JOC1), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_JOC) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_CHAOS1), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_CHAOS) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_EXL1), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_EXL) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_ZEN1), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_ZEN) == BST_CHECKED));

	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_CUSTOM2), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_CUSTOM1) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_EDIT_CUSTOM), fEnable && (::IsDlgButtonChecked(m_hwndTabPickup, IDC_CUSTOM1) == BST_CHECKED));
	::EnableWindow(::GetDlgItem(m_hwndTabPickup, IDC_PICKRUNMODE), fEnable);
}


LRESULT CUnifiedSettingsDlg::OnClickedOK(WORD, WORD, HWND, BOOL&)
{
	ApplyGeneral();
	ApplyClass();
	ApplyPickup();
	m_cSettings.Save();

	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CUnifiedSettingsDlg::OnClickedCancel(WORD, WORD, HWND, BOOL&)
{
	ShowWindow(SW_HIDE);
	return 0;
}


LRESULT CUnifiedSettingsDlg::OnCheckAutoHeal(WORD, WORD, HWND, BOOL&)
{
	if (m_hwndTabGeneral != NULL)
	{
		HWND hwndHealTime = ::GetDlgItem(m_hwndTabGeneral, IDC_AUTOHEALTIME);
		if (hwndHealTime != NULL)
			::EnableWindow(hwndHealTime, ::IsDlgButtonChecked(m_hwndTabGeneral, IDC_AUTOHEAL) == BST_CHECKED);
	}
	return 0;
}

LRESULT CUnifiedSettingsDlg::OnCheckStopPick(WORD, WORD, HWND, BOOL&)
{
	if (m_hwndTabGeneral != NULL)
	{
		BOOL fCheck = ::IsDlgButtonChecked(m_hwndTabGeneral, IDC_STOPPICK) != BST_CHECKED;
		::EnableWindow(::GetDlgItem(m_hwndTabGeneral, IDC_STOPZEN), fCheck);
		::EnableWindow(::GetDlgItem(m_hwndTabGeneral, IDC_STOPMOVE), fCheck);
	}
	return 0;
}

LRESULT CUnifiedSettingsDlg::OnSelectCharClass(WORD, WORD, HWND, BOOL&)
{
	if (m_hwndTabClass == NULL)
		return 0;

	HWND hwndClassCombo = ::GetDlgItem(m_hwndTabClass, IDC_CHARCLASS);
	int nSel = 0;
	if (hwndClassCombo != NULL)
		nSel = (int)::SendMessage(hwndClassCombo, CB_GETCURSEL, 0, 0);

	CWindow* pDlgs[] = {&m_cDarkLordSettings, &m_cEElfSettings, &m_cAElfSettings, &m_cBKSettings, &m_cSMSettings, &m_cMGSettings, 0};

	for (int i = 0; pDlgs[i] != 0; ++i)
	{
		pDlgs[i]->ShowWindow(i == nSel ? SW_SHOWNOACTIVATE : SW_HIDE);
	}

	return 0;
}

LRESULT CUnifiedSettingsDlg::OnPickupStateChange(WORD, WORD, HWND, BOOL&)
{
	ApplyPickupState();
	return 0;
}


LRESULT CUnifiedSettingsDlg::OnEraseBkgnd(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	HDC hDC = (HDC)wParam;
	if (hDC == NULL)
	{
		bHandled = FALSE;
		return 0;
	}

	RECT rc;
	GetClientRect(&rc);

	// Use bitmap texture if available, otherwise fallback to gradient
	if (m_cTheme.GetBgPanel() != NULL)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		if (hMemDC != NULL)
		{
			HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, m_cTheme.GetBgPanel());

			BITMAP bm;
			GetObject(m_cTheme.GetBgPanel(), sizeof(bm), &bm);
			for (int y = rc.top; y < rc.bottom; y += bm.bmHeight)
				for (int x = rc.left; x < rc.right; x += bm.bmWidth)
					BitBlt(hDC, x, y, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

			SelectObject(hMemDC, hOldBmp);
			DeleteDC(hMemDC);
		}
	}
	else
	{
		CMuTheme::DrawMuGradientBg(hDC, rc);
	}

	bHandled = TRUE;
	return 1;
}


LRESULT CUnifiedSettingsDlg::OnNCPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HDC hDC = GetWindowDC();
	if (hDC == NULL)
		return 0;

	RECT rcWnd;
	RECT rcClient;
	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);

	GetWindowRect(&rcWnd);

	int iXOffs = rcClient.left - rcWnd.left;
	int iCapSize = rcClient.top - rcWnd.top;

	GetClientRect(&rcClient);
	OffsetRect(&rcClient, iXOffs, iCapSize);
	ExcludeClipRect(hDC, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

	RECT rc = {0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top};

	// Fill NC area with MU Online dark theme color
	HBRUSH hBrush = CreateSolidBrush(CMuTheme::ClrWindowBg());
	FillRect(hDC, &rc, hBrush);
	DeleteObject(hBrush);

	// Draw gold frame
	CMuTheme::DrawMuFrame(hDC, rc);

	// Draw title text
	RECT rcTitle = {4, 4, rc.right - 4, iCapSize};

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, CMuTheme::ClrTitleText());

	HFONT hOldFont = NULL;
	if (m_cTheme.GetTitleFont() != NULL)
		hOldFont = (HFONT)SelectObject(hDC, m_cTheme.GetTitleFont());

	TCHAR szCaption[256] = {0};
	GetWindowText(szCaption, 255);
	DrawText(hDC, szCaption, (int)_tcslen(szCaption), &rcTitle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (hOldFont != NULL)
		SelectObject(hDC, hOldFont);

	ReleaseDC(hDC);
	return 0;
}


LRESULT CUnifiedSettingsDlg::OnCtlColorStatic(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hDC = (HDC)wParam;
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, CMuTheme::ClrBodyText());

	if (m_cTheme.GetBodyFont() != NULL)
		SelectObject(hDC, m_cTheme.GetBodyFont());

	bHandled = TRUE;
	return (LRESULT)m_cTheme.GetPanelBrush();
}

LRESULT CUnifiedSettingsDlg::OnCtlColorBtn(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hDC = (HDC)wParam;
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, CMuTheme::ClrBodyText());

	bHandled = TRUE;
	return (LRESULT)m_cTheme.GetPanelBrush();
}

LRESULT CUnifiedSettingsDlg::OnCtlColorDlg(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	return (LRESULT)m_cTheme.GetBgBrush();
}


LRESULT CUnifiedSettingsDlg::OnCtlColorEdit(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hDC = (HDC)wParam;
	SetBkMode(hDC, OPAQUE);
	SetBkColor(hDC, CMuTheme::ClrBtnBg());
	SetTextColor(hDC, CMuTheme::ClrBodyText());

	bHandled = TRUE;
	return (LRESULT)m_hEditBrush;
}


LRESULT CUnifiedSettingsDlg::OnCtlColorListBox(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hDC = (HDC)wParam;
	SetBkMode(hDC, OPAQUE);
	SetBkColor(hDC, CMuTheme::ClrBtnBg());
	SetTextColor(hDC, CMuTheme::ClrBodyText());

	bHandled = TRUE;
	return (LRESULT)m_hEditBrush;
}


LRESULT CUnifiedSettingsDlg::OnDrawItem(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
	if (lpDIS == NULL)
	{
		bHandled = FALSE;
		return 0;
	}

	// Handle owner-draw tab items
	if (lpDIS->CtlType == ODT_TAB)
	{
		DrawThemedTab(lpDIS);
		bHandled = TRUE;
		return TRUE;
	}

	// Handle owner-draw buttons (Apply/Cancel)
	if (lpDIS->CtlType == ODT_BUTTON)
	{
		BOOL bPressed = (lpDIS->itemState & ODS_SELECTED) != 0;
		BOOL bFocused = (lpDIS->itemState & ODS_FOCUS) != 0;

		TCHAR szText[64] = {0};
		::GetWindowText(lpDIS->hwndItem, szText, 63);

		if (m_cTheme.GetBodyFont() != NULL)
			SelectObject(lpDIS->hDC, m_cTheme.GetBodyFont());

		CMuTheme::DrawMuButton(lpDIS->hDC, lpDIS->rcItem, szText, bPressed, bFocused);

		bHandled = TRUE;
		return TRUE;
	}

	bHandled = FALSE;
	return 0;
}


// -----------------------------------------------------------------------
// Theme helpers
// -----------------------------------------------------------------------

void CUnifiedSettingsDlg::ThemeTabControl(HWND hwndTab)
{
	if (hwndTab == NULL) return;

	// Disable visual styles on the tab control so owner-draw works cleanly
	SetWindowTheme(hwndTab, L"", L"");

	// Add TCS_OWNERDRAWTABS style so WM_DRAWITEM is sent for each tab
	LONG lStyle = ::GetWindowLong(hwndTab, GWL_STYLE);
	::SetWindowLong(hwndTab, GWL_STYLE, lStyle | TCS_OWNERDRAWFIXED);

	// Subclass tab control to paint its background/border
	SetWindowSubclass(hwndTab, TabSubclassProc, 1, (DWORD_PTR)&m_cTheme);
}


static BOOL CALLBACK EnumSubclassCheckboxesProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClass[64] = {0};
	GetClassName(hWnd, szClass, 63);
	if (_tcsicmp(szClass, _T("Button")) != 0)
		return TRUE;

	LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	LONG lType = lStyle & BS_TYPEMASK;

	// Only subclass auto-checkboxes and 3-state checkboxes
	if (lType != BS_AUTOCHECKBOX && lType != BS_CHECKBOX && lType != BS_AUTO3STATE)
		return TRUE;

	CMuTheme* pTheme = (CMuTheme*)lParam;
	SetWindowSubclass(hWnd, CUnifiedSettingsDlg::CheckboxSubclassProc, 2, (DWORD_PTR)pTheme);
	return TRUE;
}


void CUnifiedSettingsDlg::SubclassCheckboxes(HWND hwndParent)
{
	if (hwndParent == NULL) return;
	EnumChildWindows(hwndParent, EnumSubclassCheckboxesProc, (LPARAM)&m_cTheme);
}


void CUnifiedSettingsDlg::DrawThemedTab(LPDRAWITEMSTRUCT lpDIS)
{
	HDC hDC = lpDIS->hDC;
	RECT rc = lpDIS->rcItem;
	BOOL bSelected = (lpDIS->itemState & ODS_SELECTED) != 0;

	// Fill tab background
	COLORREF clrBg = bSelected ? CMuTheme::ClrTabActive() : CMuTheme::ClrTabInactive();
	HBRUSH hBr = CreateSolidBrush(clrBg);
	FillRect(hDC, &rc, hBr);
	DeleteObject(hBr);

	// Draw tab border
	COLORREF clrBorder = bSelected ? CMuTheme::ClrFrameBright() : CMuTheme::ClrFrameGold();
	HPEN hPen = CreatePen(PS_SOLID, 1, clrBorder);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

	// Draw top and sides (no bottom for selected tab)
	MoveToEx(hDC, rc.left, rc.bottom - 1, NULL);
	LineTo(hDC, rc.left, rc.top);
	LineTo(hDC, rc.right - 1, rc.top);
	LineTo(hDC, rc.right - 1, rc.bottom - 1);
	if (!bSelected)
		LineTo(hDC, rc.left, rc.bottom - 1);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBr);
	DeleteObject(hPen);

	// Draw tab text
	TCHAR szText[64] = {0};
	TCITEM tci;
	memset(&tci, 0, sizeof(tci));
	tci.mask = TCIF_TEXT;
	tci.pszText = szText;
	tci.cchTextMax = 63;
	TabCtrl_GetItem(lpDIS->hwndItem, lpDIS->itemID, &tci);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, bSelected ? CMuTheme::ClrActiveText() : CMuTheme::ClrBodyText());

	if (m_cTheme.GetTabFont() != NULL)
		SelectObject(hDC, m_cTheme.GetTabFont());

	DrawText(hDC, szText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


static BOOL CALLBACK EnumSubclassSeparatorsProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClass[64] = {0};
	GetClassName(hWnd, szClass, 63);
	if (_tcsicmp(szClass, _T("Static")) != 0)
		return TRUE;

	LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	LONG lType = lStyle & SS_TYPEMASK;

	// Only subclass SS_ETCHEDHORZ static controls
	if (lType != SS_ETCHEDHORZ)
		return TRUE;

	CMuTheme* pTheme = (CMuTheme*)lParam;
	SetWindowSubclass(hWnd, CUnifiedSettingsDlg::SeparatorSubclassProc, 3, (DWORD_PTR)pTheme);
	return TRUE;
}


void CUnifiedSettingsDlg::SubclassSeparators(HWND hwndParent)
{
	if (hwndParent == NULL) return;
	EnumChildWindows(hwndParent, EnumSubclassSeparatorsProc, (LPARAM)&m_cTheme);
}


static BOOL CALLBACK EnumDisableThemesProc(HWND hWnd, LPARAM)
{
	TCHAR szClass[64] = {0};
	GetClassName(hWnd, szClass, 63);

	if (_tcsicmp(szClass, _T("ComboBox")) == 0 ||
		_tcsicmp(szClass, _T("Edit")) == 0 ||
		_tcsicmp(szClass, _T("msctls_updown32")) == 0 ||
		_tcsicmp(szClass, _T("Button")) == 0 ||
		_tcsicmp(szClass, _T("Static")) == 0 ||
		_tcsicmp(szClass, _T("ListBox")) == 0)
	{
		SetWindowTheme(hWnd, L"", L"");
	}

	return TRUE;
}


void CUnifiedSettingsDlg::DisableChildThemes(HWND hwndParent)
{
	if (hwndParent == NULL) return;
	EnumChildWindows(hwndParent, EnumDisableThemesProc, 0);
}


// -----------------------------------------------------------------------
// Separator subclass: paints gold line instead of white etched line
// -----------------------------------------------------------------------
LRESULT CALLBACK CUnifiedSettingsDlg::SeparatorSubclassProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	CMuTheme* pTheme = (CMuTheme*)dwRefData;

	switch (uMsg)
	{
	case WM_ERASEBKGND:
		return 1;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = ::BeginPaint(hWnd, &ps);

			RECT rc;
			::GetClientRect(hWnd, &rc);

			// Fill background with panel color
			FillRect(hDC, &rc, pTheme->GetPanelBrush());

			// Draw gold separator line at vertical center
			int yMid = (rc.top + rc.bottom) / 2;
			HPEN hPen = CreatePen(PS_SOLID, 1, CMuTheme::ClrSeparator());
			HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
			MoveToEx(hDC, rc.left, yMid, NULL);
			LineTo(hDC, rc.right, yMid);
			SelectObject(hDC, hOldPen);
			DeleteObject(hPen);

			::EndPaint(hWnd, &ps);
			return 0;
		}

	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, SeparatorSubclassProc, uIdSubclass);
		break;
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}


// -----------------------------------------------------------------------
// Tab control subclass: paints background and border in dark-gold theme
// -----------------------------------------------------------------------
LRESULT CALLBACK CUnifiedSettingsDlg::TabSubclassProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	CMuTheme* pTheme = (CMuTheme*)dwRefData;

	switch (uMsg)
	{
	case WM_ERASEBKGND:
		{
			HDC hDC = (HDC)wParam;
			RECT rc;
			::GetClientRect(hWnd, &rc);

			// Fill entire tab control area with panel background
			FillRect(hDC, &rc, pTheme->GetPanelBrush());

			// Draw a gold border around the tab display area
			RECT rcDisplay = rc;
			TabCtrl_AdjustRect(hWnd, FALSE, &rcDisplay);
			rcDisplay.left -= 2;
			rcDisplay.top -= 2;
			rcDisplay.right += 2;
			rcDisplay.bottom += 2;

			HPEN hPen = CreatePen(PS_SOLID, 1, CMuTheme::ClrFrameGold());
			HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
			HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
			Rectangle(hDC, rcDisplay.left, rcDisplay.top, rcDisplay.right, rcDisplay.bottom);
			SelectObject(hDC, hOldPen);
			SelectObject(hDC, hOldBr);
			DeleteObject(hPen);

			return 1;
		}

	case WM_PAINT:
		{
			// Let the default handler draw the tab items, then paint over the body border
			LRESULT lr = DefSubclassProc(hWnd, uMsg, wParam, lParam);

			HDC hDC = ::GetDC(hWnd);
			if (hDC != NULL)
			{
				RECT rc;
				::GetClientRect(hWnd, &rc);

				// Paint over the default body border with dark-gold border
				RECT rcDisplay = rc;
				TabCtrl_AdjustRect(hWnd, FALSE, &rcDisplay);
				rcDisplay.left -= 2;
				rcDisplay.top -= 2;
				rcDisplay.right += 2;
				rcDisplay.bottom += 2;

				HPEN hPen = CreatePen(PS_SOLID, 1, CMuTheme::ClrFrameGold());
				HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
				HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
				Rectangle(hDC, rcDisplay.left, rcDisplay.top, rcDisplay.right, rcDisplay.bottom);
				SelectObject(hDC, hOldPen);
				SelectObject(hDC, hOldBr);
				DeleteObject(hPen);

				::ReleaseDC(hWnd, hDC);
			}
			return lr;
		}

	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, TabSubclassProc, uIdSubclass);
		break;
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}


// -----------------------------------------------------------------------
// Checkbox subclass: custom paints checkbox with dark-gold theme bitmaps
// -----------------------------------------------------------------------
LRESULT CALLBACK CUnifiedSettingsDlg::CheckboxSubclassProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	CMuTheme* pTheme = (CMuTheme*)dwRefData;

	switch (uMsg)
	{
	case WM_ERASEBKGND:
		return 1;

	case WM_SETCURSOR:
		{
			HCURSOR hCursor = pTheme->GetMuCursor();
			if (hCursor != NULL)
			{
				SetCursor(hCursor);
				return TRUE;
			}
		}
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = ::BeginPaint(hWnd, &ps);

			RECT rc;
			::GetClientRect(hWnd, &rc);

			// Fill background with panel color
			FillRect(hDC, &rc, pTheme->GetPanelBrush());

			// Get checkbox state
			BOOL bChecked = (::SendMessage(hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED);
			BOOL bEnabled = ::IsWindowEnabled(hWnd);

			// Select the appropriate checkbox bitmap
			HBITMAP hBmp = NULL;
			if (bChecked)
				hBmp = bEnabled ? pTheme->GetChkChecked() : pTheme->GetChkCheckedDis();
			else
				hBmp = bEnabled ? pTheme->GetChkUnchecked() : pTheme->GetChkUncheckedDis();

			// Get checkbox text and style for layout
			TCHAR szText[256] = {0};
			::GetWindowText(hWnd, szText, 255);
			LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
			BOOL bLeftText = (lStyle & BS_LEFTTEXT) != 0;

			int boxSize = CHECKBOX_BOX_SIZE;

			if (hBmp != NULL)
			{
				BITMAP bm;
				GetObject(hBmp, sizeof(bm), &bm);
				boxSize = bm.bmWidth;

				HDC hMemDC = CreateCompatibleDC(hDC);
				HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

				int yBox = rc.top + (rc.bottom - rc.top - bm.bmHeight) / 2;
				int xBox;

				if (bLeftText)
					xBox = rc.right - bm.bmWidth;
				else
					xBox = rc.left;

				BitBlt(hDC, xBox, yBox, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

				SelectObject(hMemDC, hOldBmp);
				DeleteDC(hMemDC);
			}
			else
			{
				// Fallback: draw a simple themed checkbox box
				int yBox = rc.top + (rc.bottom - rc.top - boxSize) / 2;
				int xBox = bLeftText ? (rc.right - boxSize) : rc.left;

				RECT rcBox = {xBox, yBox, xBox + boxSize, yBox + boxSize};

				HBRUSH hBoxBr = CreateSolidBrush(CMuTheme::ClrBtnBg());
				FillRect(hDC, &rcBox, hBoxBr);
				DeleteObject(hBoxBr);

				HPEN hPen = CreatePen(PS_SOLID, 1, CMuTheme::ClrFrameGold());
				HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
				HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
				Rectangle(hDC, rcBox.left, rcBox.top, rcBox.right, rcBox.bottom);
				SelectObject(hDC, hOldPen);
				SelectObject(hDC, hOldBr);
				DeleteObject(hPen);

				if (bChecked)
				{
					HPEN hChkPen = CreatePen(PS_SOLID, 2,
						bEnabled ? CMuTheme::ClrCheckMark() : CMuTheme::ClrDisabledText());
					hOldPen = (HPEN)SelectObject(hDC, hChkPen);
					// Draw checkmark
					MoveToEx(hDC, rcBox.left + 2, rcBox.top + boxSize/2, NULL);
					LineTo(hDC, rcBox.left + boxSize/3 + 1, rcBox.bottom - 3);
					LineTo(hDC, rcBox.right - 2, rcBox.top + 2);
					SelectObject(hDC, hOldPen);
					DeleteObject(hChkPen);
				}
			}

			// Draw text if present
			if (szText[0] != 0)
			{
				SetBkMode(hDC, TRANSPARENT);
				SetTextColor(hDC, bEnabled ? CMuTheme::ClrBodyText() : CMuTheme::ClrDisabledText());

				if (pTheme->GetBodyFont() != NULL)
					SelectObject(hDC, pTheme->GetBodyFont());

				RECT rcText = rc;
				if (bLeftText)
				{
					rcText.right -= boxSize + 3;
					DrawText(hDC, szText, -1, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					rcText.left += boxSize + 3;
					DrawText(hDC, szText, -1, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				}
			}

			::EndPaint(hWnd, &ps);
			return 0;
		}

	case WM_ENABLE:
		::InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, CheckboxSubclassProc, uIdSubclass);
		break;
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
