#include "stdafx.h"
#include "WndProc.h"
#include "Reconnect.h"
#include "InterfaceEx.h"
#include "AttackHelper.h"
#include "InterfaceEx.h"
#include "Interface.h"
#include "Other.h"
#include "CameraEx.h"
#include "ResetSystem.h"
#include "Interface.h"
#include "Ranking.h"
#include "RageSystem.h"
#include "CustomMenu.h"
#include "Controller.h"
#include "DungeonSiege.h"
#include "ExTeleport.h"
#include "AutoClickerUI.h"

BOOL MouseWellButtonDown;
BOOL MouseLeftButtonDown;
BOOL MouseRightButtonDown;

bool bMButton = false;
int iX = 0, iY = 0;

int g_UsedResolution;

WNDPROC muWindowProc = (WNDPROC)MU_WND_PROC;
HWND MuWindow;

#pragma optimize("g", on)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*if (MAIN_STATEEx == 5)
	{*/
	MuWindow = hwnd;
	switch (uMsg)
	{
		
	case WM_LBUTTONDOWN:
	{
		if (TpList == true)
		{
			return true;
		}
		if (gAttackHelper->State == ATTACKHELPER_STATE_SelectMode)
		{
			return true;
		}

		if(g_ResetSystem.State == 1)
		{
			return true;
		}
		if(gInterfaces->StateWare == 1)
		{
			return true;
		}
		if(gInterfaces->StateWarex == 1)
		{
			return true;
		}
		if (g_ResetSystem.StateGR == 1)
		{
			return true;
		}

		if(gRanking.Active == 1)
		{
			return true;
		}
		if(gMenuSystem.Active == 1)
		{
			return true;
		}
		if(g_DungeonSiege.Active == 1)
		{
			return true;
		}
#ifdef MULTI_WAREHOUSE
		if(gInterfaces->StateBank == 1)
		{
			return true;
		}
#endif
		if(gInterfaces->Data[eRageTable].OnShow == 1)
		{
			return true;
		}
		
		MouseLeftButtonDown = true;

#if(QUEST_WORLD_ENABLE == 1)
		if (gQuestWorld.MouseLock() == 1)
		{
			return 1;
		}
#endif

		/*if (MAIN_MAPNUMBER != 30 && CheckZone(0.0f, 0.0f, 65.0, 21.0))
		{
			return true;
		}*/
	}
	break;
	case WM_MBUTTONDOWN:
		gCamera->UpdateCursor();
		//MouseWellButtonDown = true;
		break;
	case WM_MBUTTONUP:
		//MouseWellButtonDown = false;
		break;
	case WM_LBUTTONUP:
		MouseLeftButtonDown = false;
		gAutoClickerUI.MouseClickProc();
		gInterfaces->MouseClickProc();
		gExInterface->MouseClickProc();
		MouseClickTP();
#if(QUEST_WORLD_ENABLE == 1)
		gQuestWorld.MouseEventClick();
#endif
		break;
	case WM_RBUTTONDOWN:
		MouseRightButtonDown = true;
		break;
	case WM_RBUTTONUP:
		MouseRightButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		gAttackHelper->MouseMove();
		break;
	case WM_MOUSEWHEEL:
		
		gCamera->Zoom((int)(SHIWORD(wParam) / 120));
		break;
	case WM_TIMER:
		{
			switch (wParam)
			{
			case AUTOCLICK_LEFT:
				{
					if (AutoClick[0] == true)
					{
						SendMessage(MuWindow, (AutoClick[2] == true) ? WM_LBUTTONUP : WM_LBUTTONDOWN, NULL, NULL);
						AutoClick[2] = !AutoClick[2];
					}
				}
				break;
			case AUTOCLICK_RIGHT:
				{
					if (AutoClick[1] == true)
					{
						SendMessage(MuWindow, (AutoClick[2] == true) ? WM_RBUTTONUP : WM_RBUTTONDOWN, NULL, NULL);
						AutoClick[2] = !AutoClick[2];
					}
				}
				break;
			}
		}
		break;
	}
	
	//return muWindowProc(hwnd, uMsg, wParam, lParam);
	return CallWindowProc(WndProcEx, hwnd, uMsg, wParam, lParam);
	//}
}