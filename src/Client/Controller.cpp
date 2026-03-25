#include "stdafx.h"
#include "Controller.h"
#include "CameraEx.h"
#include "Interface.h"
#include "InterfaceEx.h"
#include "Tray.h"
#include "TDebugLog.h"
#include "Structure.h"
#include "AttackHelper.h"
#include "Protocol.h"
//#include "JewelsBank.h"
#include "Ranking.h"
#include "ResetSystem.h"
#include "RageSystem.h"
#include "CustomMenu.h"
#include "JewelsBank.h"
#include "Fog.h"
#include "WndProc.h"
#include "DungeonSiege.h"
#include "console.h"
#include "AutoClickerUI.h"

// ----------------------------------------------------------------------------------------------
//WPARAM mParam;
Controller	gController;
bool            AutoClick[3]	= { false, false, false };
bool    TpList       = false;
// ----------------------------------------------------------------------------------------------

bool Controller::Load()
{
	gController.wMouse = 0x00;

	gController.wKeyBord = 0x00;

	if( !this->MouseHook )
	{
		this->MouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)this->Mouse, gController.Instance, GetCurrentThreadId());
		// ----
		if( !this->MouseHook )
		{
			return false;
		}
	}
	// ----
	if( !this->KeyboardHook )
	{
		this->KeyboardHook = SetWindowsHookExA(WH_KEYBOARD, (HOOKPROC)this->Keyboard, gController.Instance, GetCurrentThreadId());

		if(!this->KeyboardHook)
		{
			//MessageBox(0,"ERROR","KeyboardHook",0);
			return false;
		}
	}
	return true;
}
//BOOL MouseSetHook(BOOL set_or_remove) 
//{
//	if(set_or_remove == TRUE) 
//	{
//		if(MouseHook == NULL) 
//		{
//			MouseHook = SetWindowsHookExA(WH_MOUSE, gController.Mouse, gController.Instance, GetCurrentThreadId());
//			if(!MouseHook) 
//			{ 
//				//MessageBox(0,"ERROR","Mause",0);
//				return FALSE;
//			}
//		}
//	} 
//	else
//	{ 
//		//MessageBox(0,"ERROR","Mause 2",0);
//		MouseHook = NULL;
//		return UnhookWindowsHookEx(MouseHook); 
//	}
//	return TRUE;
//}
//BOOL KeyboardSetHook(BOOL set_or_remove)
//{
//	if(set_or_remove == TRUE)
//	{
//		if(KeyboardHook == NULL)
//		{
//			KeyboardHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, gController.Instance, GetCurrentThreadId());
//	
//			if(!KeyboardHook) 
//				return FALSE; 
//		}
//	} 
//	else 
//	{
//		return UnhookWindowsHookEx(KeyboardHook);
//		KeyboardHook = NULL;
//	}
//  return TRUE;
//}
//LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
LRESULT Controller::Keyboard(int Code, WPARAM wParam, LPARAM lParam)
{
	
	if(Code == HC_ACTION)
	{
		// F5-F12 keys are managed by LordOfMU AutoClicker's WH_KEYBOARD_LL hook.
		// Pass them through immediately to avoid interference with F9 (Settings),
		// Shift+F9 (History), F5 (Start/Stop), F8 (Stop), etc.
		if (wParam >= VK_F5 && wParam <= VK_F12)
			return CallNextHookEx(gController.KeyboardHook, Code, wParam, lParam);

		if(((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0 && GetForegroundWindow() == *(HWND*)(MAIN_WINDOW))
		{

		gController.wKeyBord = wParam;
		
		if ( *(int *) (0x7D4C90) || *(int *) (0x7960BA5) ) 
		{
		//TpList = false;
			return false;
		}
		switch(wParam)
		{
			case VK_F5:
			{
				// REPLACED: Was controlling client's AttackHelper.
				// Now handled by LordOfMU AutoClicker's WH_KEYBOARD_LL hook.
				// No action needed — LordOfMU intercepts F5 directly.
			}
			break;
			/*case VK_F9:
			{
				// F9 is handled by LordOfMU.dll (AutoClicker ImGui overlay)
			}
			break;*/
			/*case VK_F6:
			{
				if (gInterfaces->StateFog == 1)
						{
							gInterfaces->StateFog = 0;
							pSendChatTextEx((const char *)"", (const char *)"Fog Off", Red);
							

						}
						else
						{
							gInterfaces->StateFog = 1;
							pSendChatTextEx((const char *)"", (const char *)"Fog On", Blue);
							
						}
			}
			break;*/
			/*case VK_F7:
			{
				AntilagSwith();
			}
			break;*/
			case VK_F8:
			{
				// REPLACED: Was controlling client's AttackHelper.
				// Now handled by LordOfMU AutoClicker's WH_KEYBOARD_LL hook.
				// F8 = Stop Clicker in LordOfMU (WM_STOP_CLICKER).
			}
		break;
		/*case VK_F9:
			{
					if(gAttackHelper->State == ATTACKHELPER_STATE_Running)
					{
						gAttackHelper->SetState(ATTACKHELPER_STATE_Stop);
					}
		}
		break;*/
		//case VK_F10:
		//	{
		//		if (AutoClick[1] == false) // Right
		//		{
		//			AutoClick[0] = !AutoClick[0];
		//			AutoClick[2] = false;

		//			if (AutoClick[0] == false)  // Left
		//			{
		//				KillTimer(MuWindow, AUTOCLICK_LEFT);
		//				SendMessage(MuWindow, WM_LBUTTONUP, NULL, NULL);
		//			}
		//			else
		//			{
		//				SetTimer(MuWindow, AUTOCLICK_LEFT, 100, NULL);
		//			}
		//		}
		//	}
		//	break;
		//	case VK_F11:
		//		{
		//			if (AutoClick[0] == false) // Left
		//		{
		//			AutoClick[1] = !AutoClick[1];
		//			AutoClick[2] = false;

		//			if (AutoClick[1] == false)  // Right
		//			{
		//				KillTimer(MuWindow, AUTOCLICK_RIGHT);
		//				SendMessage(MuWindow, WM_RBUTTONUP, NULL, NULL);
		//			}
		//			else
		//			{
		//				SetTimer(MuWindow, AUTOCLICK_RIGHT, 100, NULL);
		//			}
		//		}
		//		}
		//		break;
		//if(wParam == VK_F12)
		//{
		case VK_F12:
		{
			if( gTrayMode.TempWindowProc == NULL )
			{
				gTrayMode.TempWindowProc = SetWindowLong(pGameWindow, GWL_WNDPROC, (long)Controller::Window);
			}
			// ----
			gTrayMode.SwitchState();
			
		}
		break;
		case  0x4D: // Key Num 0 0x60
			{	
				//if (RunningChat == false)
				//{
					if (TpList == false)
					{
						//if (RunningMap == false)
						//{
							//PlaySound(25, 0, 0);
							TpList = true;
						//}
					}
					else
					{
						//PlaySound(25, 0, 0);
						TpList = false;
					}
				//}
			}
			break;
		case 0x4F:	//O
		{
			//if(!gInterface.CheckWindow(ObjWindow::ChatWindow))
			//{
				if(gMenuSystem.Active == 0)
				{
					gMenuSystem.Active = 1;

				}
				else
				{
					gMenuSystem.Active = 0;
				}
			//}
		}
		break;
		//if(wParam == VK_ESCAPE)
		//{
		case VK_ESCAPE:
		{
			TpList = false;
			gJewelsBank.Active = false;
			gRanking.Active = false;
			g_ResetSystem.State = 0;
			g_ResetSystem.StateGR = 0;
			gRanking.Active = 0;
			//pSetCursorFocus= false; 
			//pSetCursorFocus2 = false; 
			gInterfaces->Data[eRageTable].OnShow = false; 
			gMenuSystem.Active=false;
			g_DungeonSiege.Active=false;
			gInterfaces->StateWare = 0;
			// gAutoClickerUI.CloseHistory() removed — LordOfMU's ImGui overlay
			// handles ESC to close its own settings/history windows.
			//gInterfaces->Data[eLuckyWheelMain].OnShow = false;
#ifdef MULTI_WAREHOUSE
			gInterfaces->Close_DrawBankCountWindow();
#endif
		}
			break;

		/*if(wParam == VK_NUMPAD5)
		{
			UpdateCharInfoDEBUG();		
		}*/



	}
	}
	}
	return CallNextHookEx(gController.KeyboardHook, Code, wParam, lParam);
}
// ----------------------------------------------------------------------------------------------

LRESULT Controller::Mouse(int Code, WPARAM wParam, LPARAM lParam)
{
	if(Code == HC_ACTION)
	{
		MOUSEHOOKSTRUCTEX* HookStruct =(MOUSEHOOKSTRUCTEX*)lParam;

		if(GetForegroundWindow() == *(HWND*)(MAIN_WINDOW))
	// ----
	gController.wMouse = wParam;

	switch(wParam)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
		{

//#ifdef MuContinent
//
//#else
//	RunCameraMouse(Mouse, wParam);
//#endif
	//gInterface.EventCameraUI(wParam);
	//gInterface.EventNewsWindow_Main(wParam);
#ifdef MULTI_CHARACTERS
	gInterface.CharactersButtonAddUI(wParam);
#endif
#ifdef MULTI_WAREHOUSE
	gInterfaces->WarehouseButtonAddUI(wParam);
#endif
#if(CUSTOM_VALUE_MENU)
	gInterfaces->ControllerValueMenu(wParam);
	gInterfaces->ControllerGuildValueMenu(wParam);
#endif		
	//test
///////////////////////////////////////////////////////////////////
	g_ResetSystem.Button(wParam);
	gRageSystem.Button(wParam);
	gMenuSystem.Button(wParam);
	gRanking.Button(wParam);
	g_ResetSystem.GrandButton(wParam);
	g_DungeonSiege.Button(wParam);
///////////////////////////////////////////////////////////////////

	//testButtonAddUI(wParam);
	//test();
			}
		break;
	case WM_MOUSEWHEEL:
		{

		}
		break;
	case WM_MBUTTONDOWN:
		{

		}
		break;
	case WM_MBUTTONUP:
		{

		}
		break;
	case WM_MOUSEMOVE:
		{

		}
		break;
	}
	}
	// ----
	return CallNextHookEx(gController.MouseHook, Code, wParam, lParam);
}
// ----------------------------------------------------------------------------------------------

//LRESULT Controller::Keyboard(int Code, WPARAM wParam, LPARAM lParam)
//{
//
//
//
//	if( (Code == HC_ACTION) && (wParam == WM_KEYDOWN))
//	{
//
//		KBDLLHOOKSTRUCT Hook = *((KBDLLHOOKSTRUCT*)lParam);
//		// ----
//		switch(Hook.vkCode)
//		{
//
//		case VK_F7:
//			{
//				
//			}
//			break;
//
////#ifdef MuContinent
////
////#else
////		case VK_F9:
////		{
////			CameraKeyboard(1);
////		}
////				break;	
////#endif
////
////#ifdef MuContinent
////
////#else
////		case VK_F10:
////			{
////				CameraKeyboard(2);
////			}
////			break;
////#endif
//			// --
//		case VK_F12:
//			{
//				if( gTrayMode.TempWindowProc == NULL )
//				{
//					gTrayMode.TempWindowProc = SetWindowLong(pGameWindow, GWL_WNDPROC, (long)gController.Window);
//				}
//				// ----
//				gTrayMode.SwitchState();
//			}
//			break;
//			// --
//		}
//	//	}
//	} 
//	return CallNextHookEx(gController.KeyboardHook, Code, wParam, lParam);
//}
// ----------------------------------------------------------------------------------------------

LRESULT Controller::Window(HWND Window, DWORD Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
	case TRAYMODE_ICON_MESSAGE:
		{
			switch(lParam)
			{
			case WM_LBUTTONDBLCLK:
				{
					gTrayMode.SwitchState();
				}
				break;
			}
		}
		break;
	}
	// ----
	return CallWindowProc((WNDPROC)gTrayMode.TempWindowProc, Window, Message, wParam, lParam);
}
// ----------------------------------------------------------------------------------------------