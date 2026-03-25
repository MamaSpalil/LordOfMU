#include "StdAfx.h"
#include "InterfaceEx.h"
#include "Interface.h"
#include "AttackHelper.h"
#include "TMemory.h"
#include "Other.h"
#include "CameraEx.h"
#include "Structure.h"
#include "Protocol.h"
#include "Interface.h"
//#include "LoadImages.h"
#include "CameraEx.h"
//#include "JewelsBank.h"
#include "Ranking.h"
#include "ResetSystem.h"
#include "RageSystem.h"
#include "CustomMenu.h"
#include "AdvancedStats.h"
//#include "WndProc.h"

#include "AutoClickerUI.h"

CExInterface * gExInterface = new CExInterface;
ServerTime g_ServerTime;

void CExInterface::Init()
{
	//this->server_timer = 0;
	//this->server_tick = 0;
	//SetOp((LPVOID)0x006BB741, (LPVOID)this->LoadImages, ASM::CALL);
	SetOp((LPVOID)oLoadSomeForm_Call,(LPVOID)LoadImages, ASM::CALL);

}
void LoadImages(int a1, int a2, int a3, HDC a4)//(HDC ItemID)
{

	LoadImagesTGA("Customs\\Interface\\buttonbank.tga", 3695, 9729, 10496, 0, 1); // Guild Bank

	LoadImagesJPG("Customs\\Interface\\navbar.jpg", INTERFACE_IMG::NAV_BAR, 9728, 10496, 0, 1);

	LoadImagesTGA("Customs\\Interface\\navbar_camera_switch.tga", INTERFACE_IMG::NAV_BAR_CAM_SWITCH, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\navbar_camera_reset.tga", INTERFACE_IMG::NAV_BAR_CAM_RESET, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\navbar_helper_start.tga", INTERFACE_IMG::NAV_BAR_BOT_START, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\navbar_helper_pause.tga", INTERFACE_IMG::NAV_BAR_BOT_PAUSE, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\JewelBank.tga", 3548, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\Top100.tga", 3590, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\Reset.tga", 3595, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\top.tga", 3591, 9729, 10496, 0, 1);

	LoadImagesTGA("Customs\\Interface\\MenuTable.tga", 3592, 9729, 10496, 0, 1);

	//LoadImagesTGA("Customs\\Interface\\vault.tga", 3596, 9729, 10496, 0, 1);
	LoadImagesTGA("Customs\\Interface\\newui_gens_rankback.tga", 3549, 9729, 10496, 0, 1);
	
	LoadImagesTGA("Customs\\Interface\\ingame_pack_back01.tga", 3551, 9729, 10496, 0, 1);
	//LoadImagesTGA("Customs\\Interface\\ingame_pack_back03.tga", 2551, 9729, 10496, 0, 1);
	//LoadImagesTGA("Customs\\Interface\\Add_1.tga", 2552, 9729, 10496, 0, 1);
	//LoadImagesTGA("Customs\\Interface\\Add_2.tga", 2553, 9729, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button1.jpg", 3554, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button1_1.jpg", 3555, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button2.jpg", 3556, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button2_1.jpg", 3557, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button3.jpg", 3558, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button3_1.jpg", 3559, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button4.jpg", 3560, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button4_1.jpg", 3561, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button5.jpg", 3562, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button5_1.jpg", 3563, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button6.jpg", 3564, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button6_1.jpg", 3565, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button7.jpg", 3566, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button7_1.jpg", 3567, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button8.jpg", 3568, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button8_1.jpg", 3569, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button9.jpg", 3570, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button9_1.jpg", 3571, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button10.jpg", 3572, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button10_1.jpg", 3573, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button11.jpg", 3574, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button11_1.jpg", 3575, 9728, 10496, 0, 1);

	LoadImagesJPG("Customs\\Interface\\button12.jpg", 3576, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\button12_1.jpg", 3577, 9728, 10496, 0, 1);
//
	//LoadImagesJPG("Customs\\Interface\\Skill1.jpg", 2544, 9728, 10496, 0, 1);
	//LoadImagesJPG("Customs\\Interface\\Skill2.jpg", 2545, 9728, 10496, 0, 1);
	//LoadImagesJPG("Customs\\Interface\\Skill3.jpg", 2546, 9728, 10496, 0, 1);
	//LoadImagesJPG("Customs\\Interface\\Skill4.jpg", 2547, 9728, 10496, 0, 1);

//	LoadImagesTGA("Customs\\Interface\\Next.tga", 3700, 9729, 10496, 0, 1);
	LoadImagesJPG("Interface\\menu_custom.jpg", 3700, 9728, 10496, 0, 1);
//#ifdef PROPENSITY_SYSTEM
//
//	  LoadImagesTGA("Customs\\Interface\\element_fire.tga", 3529, 9729, 10496, 0, 1);
//	  LoadImagesTGA("Customs\\Interface\\element_water.tga", 3530, 9729, 10496, 0, 1);
//	  LoadImagesTGA("Customs\\Interface\\element_wind.tga", 3531, 9729, 10496, 0, 1);
//	  LoadImagesTGA("Customs\\Interface\\element_land.tga", 3532, 9729, 10496, 0, 1);
//#endif
//
	 LoadImagesTGA("Customs\\Interface\\page0.tga", 3578, 9729, 10496, 0, 1);
	 LoadImagesTGA("Customs\\Interface\\page3.tga", 3579, 9729, 10496, 0, 1);

	 LoadImagesTGA("Customs\\Interface\\maplist1.tga", 3674, 9729, 10496, 0, 1); // maplist
	LoadImagesTGA("Customs\\Interface\\cross.tga", 3677, 9729, 10496, 0, 1); // maplist

	LoadImagesJPG("Customs\\Interface\\DW.jpg", 3594, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\DK.jpg", 3595, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\EF.jpg", 3596, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\MG.jpg", 3597, 9728, 10496, 0, 1);
	LoadImagesJPG("Customs\\Interface\\DL.jpg", 3598, 9728, 10496, 0, 1);

	LoadImagesTGA("Customs\\Interface\\newui_option_check.tga", 152, 9729, 10496, 0, 1);

	/* Personal Store */
	LoadImagesJPG( "Customs\\Interface\\b_command01.jpg", 223, 9728, 10496, 0, 1);
	LoadImagesJPG( "Customs\\Interface\\b_command02.jpg", 224, 9728, 10496, 0, 1);
	
	 LoadImagesTGA("Customs\\Interface\\LoginBtn.tga", 151, 9729, 10496, 0, 1);

	LoadImagesTGA("Customs\\Interface\\fps_bg.tga", 334, 9729, 10496, 0, 1);

	gAutoClickerUI.LoadImages();

	LoadImagesJPG( "Customs\\Interface\\left.jpg", 2000, 9728, 10496, 0, 1);
	LoadImagesJPG( "Customs\\Interface\\right.jpg", 1999, 9728, 10496, 0, 1);
	//LoadImagesTGA("Customs\\Interface\\mulogo_01.tga", 3699, 9729, 10496, 0, 1);
//	//  LoadImagesTGA("Customs\\Interface\\top.tga", 3580, 9729, 10496, 0, 1);
//
//
//#ifdef MULTI_CHARACTERS
//	  LoadImagesTGA("Customs\\Interface\\page0.tga", 2578, 9729, 10496, 0, 1);
//	  LoadImagesTGA("Customs\\Interface\\page1.tga", 2579, 9729, 10496, 0, 1);
//	  LoadImagesTGA("Customs\\Interface\\page2.tga", 2580, 9729, 10496, 0, 1);
//	  LoadImagesTGA("Customs\\Interface\\page3.tga", 2581, 9729, 10496, 0, 1);
//	  LoadImagesTGA("Customs\\Interface\\page4.tga", 2582, 9729, 10496, 0, 1);
//	  LoadImagesTGA("Customs\\Interface\\page5.tga", 2583, 9729, 10496, 0, 1);
//#endif
//	//  LoadImagesTGA("Customs\\Interface\\RageTable.tga", 3585, 9729, 10496, 0, 1);
//	
	gRageSystem.LoadImage();
//
	g_StatsAdvance.Load();
	//pLoadSomeForm(ItemID);
	((void(*)(int, int, int, HDC))0x006B0B0D)(a1,a2,a3,a4);
}
// ----------------------------------------------------------------------------------------------
//void CExInterface::LoadImages(int a1, int a2, int a3, HDC a4)
//{
////	//((void(*)(char *, int, int, int, int, int)) 0x006D5B1B)((char*)"Customs\\Interface\\LifeBar.jpg", INTERFACE_IMG::LIFE_BAR, 9728, 10496, 0, 1);
////	//((void(*)(char *, int, int, int, int, int)) 0x006D5B1B)((char*)"Customs\\Interface\\LifeBar_Scroll.jpg", INTERFACE_IMG::LIFE_BAR_SCROLL, 9728, 10496, 0, 1);
////	//((void(*)(char *, int, int, int, int, int)) 0x006D5B1B)((char*)"Customs\\Interface\\navbar.jpg", INTERFACE_IMG::NAV_BAR, 9728, 10496, 0, 1);
////
////
////	
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\navbar_camera_switch.tga", INTERFACE_IMG::NAV_BAR_CAM_SWITCH, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\navbar_camera_reset.tga", INTERFACE_IMG::NAV_BAR_CAM_RESET, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\navbar_helper_start.tga", INTERFACE_IMG::NAV_BAR_BOT_START, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\navbar_helper_pause.tga", INTERFACE_IMG::NAV_BAR_BOT_PAUSE, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\JewelBank.tga", 3548, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\Top100.tga", 3590, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\Reset.tga", 3595, 9729, 10496, 0, 1);
//////
//////	#ifdef MONSTER_LIFE_BAR
//////	//LoadJpgImg((char*)"Customs\\Interface\\LifeBar.jpg",  2525, 9728, 10496, 0, 1);
//////	//LoadJpgImg((char*)"Customs\\Interface\\LifeBar_Scroll.jpg", 2526, 9728, 10496, 0, 1);
//////
//////	  LoadImagesJPG("Customs\\Interface\\InterfaceUI_Frame.jpg", 3525, 9728, 10496, 0, 1);
//////	  LoadImagesJPG("Customs\\Interface\\InterfaceUI_StatusHP.jpg", 3526, 9728, 10496, 0, 1);
//////	//   LoadImagesJPG("Customs\\Interface\\in_bar2.jpg", 2530, 9728, 10496, 0, 1);
////////
//////#endif
//////	LoadImagesJPG("Customs\\Interface\\Skill1.jpg", 2544, 9728, 10496, 0, 1);
//////	LoadImagesJPG("Customs\\Interface\\Skill2.jpg", 2545, 9728, 10496, 0, 1);
//////	LoadImagesJPG("Customs\\Interface\\Skill3.jpg", 2546, 9728, 10496, 0, 1);
//////	LoadImagesJPG("Customs\\Interface\\Skill4.jpg", 2547, 9728, 10496, 0, 1);
//////
//////	#ifdef PROPENSITY_SYSTEM
//////
//////	  LoadImagesTGA("Customs\\Interface\\chaoss.tga", 2529, 9729, 10496, 0, 1);
//////	  LoadImagesTGA("Customs\\Interface\\darks.tga", 2530, 9729, 10496, 0, 1);
//////	  LoadImagesTGA("Customs\\Interface\\lights.tga", 2531, 9729, 10496, 0, 1);
//////	  LoadImagesTGA("Customs\\Interface\\sumers.tga", 2532, 9729, 10496, 0, 1);
//////#endif
////
////	  LoadImagesTGA("Customs\\Interface\\ingame_pack_back01.tga", 2550, 9729, 10496, 0, 1);
////	  LoadImagesTGA("Customs\\Interface\\ingame_pack_back03.tga", 2551, 9729, 10496, 0, 1);
////	  LoadImagesTGA("Customs\\Interface\\Add_1.tga", 2552, 9729, 10496, 0, 1);
////	  LoadImagesTGA("Customs\\Interface\\Add_2.tga", 2553, 9729, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button1.jpg", 3554, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button1_1.jpg", 3555, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button2.jpg", 3556, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button2_1.jpg", 3557, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button3.jpg", 3558, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button3_1.jpg", 3559, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button4.jpg", 3560, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button4_1.jpg", 3561, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button5.jpg", 3562, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button5_1.jpg", 3563, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button6.jpg", 3564, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button6_1.jpg", 3565, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button7.jpg", 3566, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button7_1.jpg", 3567, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button8.jpg", 3568, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button8_1.jpg", 3569, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button9.jpg", 3570, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button9_1.jpg", 3571, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button10.jpg", 3572, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button10_1.jpg", 3573, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button11.jpg", 3574, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button11_1.jpg", 3575, 9728, 10496, 0, 1);
////
////	  LoadImagesJPG("Customs\\Interface\\button12.jpg", 3576, 9728, 10496, 0, 1);
////	  LoadImagesJPG("Customs\\Interface\\button12_1.jpg", 3577, 9728, 10496, 0, 1);
//////
//////#ifdef MULTI_CHARACTERS
//////	  LoadImagesTGA("Customs\\Interface\\page0.tga", 2578, 9729, 10496, 0, 1);
//////	  LoadImagesTGA("Customs\\Interface\\page1.tga", 2579, 9729, 10496, 0, 1);
//////	  LoadImagesTGA("Customs\\Interface\\page2.tga", 2580, 9729, 10496, 0, 1);
//////	  LoadImagesTGA("Customs\\Interface\\page3.tga", 2581, 9729, 10496, 0, 1);
//////	  LoadImagesTGA("Customs\\Interface\\page4.tga", 2582, 9729, 10496, 0, 1);
//////	  LoadImagesTGA("Customs\\Interface\\page5.tga", 2583, 9729, 10496, 0, 1);
//////#endif
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\top.tga", 3591, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\MenuTable.tga", 3592, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\vault.tga", 3596, 9729, 10496, 0, 1);
////	((void(*)(char *, int, int, int, int, int)) 0x006D606C)((char*)"Customs\\Interface\\newui_gens_rankback.tga", 3549, 9729, 10496, 0, 1);
//
//	((void(*)(int, int, int, HDC))0x006B0B0D)(a1,a2,a3,a4);
//}

void GCServerTime(PMSG_SERVER_TIME* lpMsg)
{
	g_ServerTime.m_Hour = lpMsg->Hour;
	g_ServerTime.m_Minute = lpMsg->Minute;
	g_ServerTime.m_Second = lpMsg->Second;
}

void CExInterface::DrawTimer()
{

	time_t TimeServer, TimeLocal;
	struct tm * ServerT, * LocalT;
	time(&TimeServer);
	time(&TimeLocal);
	ServerT = gmtime(&TimeServer);
	LocalT = localtime(&TimeLocal); 
	char szTemp[256];
	char szTemp2[256];


	sprintf(szTemp, "Server %2d:%02d:%02d", g_ServerTime.m_Hour, g_ServerTime.m_Minute, g_ServerTime.m_Second);
	sprintf(szTemp2, " Local %2d:%02d:%02d", LocalT->tm_hour, LocalT->tm_min, LocalT->tm_sec);
	//pSetBlend(true);
	//	 pDrawTextColor = eDrawText::TextColor::Red;
	//	 pDrawTextBackground = eDrawText::TextBackground::Invisible;
	//	 MU_DrawTextOut(StartX + 80, 190, "Relogin...", 0, 0, 0);
	//	 pGLSwitch();
	//pSetBlend(true);
	// FF000000 Golden
	//pSetBlend(true);
	//pDrawTextColor = 	eDrawText::TextColor::Orange;
	//pDrawTextBackground = eDrawText::TextBackground::Invisible;
	//glColor4f(1.0, 1.0, 1.0, 1.0);
	MU_DrawTextOut(7.95, 399.9, szTemp, 0, 0, 0);
	//MU_DrawTextOut(9,400,szTemp,0,0,0);
	MU_DrawTextOut(8, 416.25, szTemp2, 0, 0, 0);
	//MU_DrawTextOut(9,416,szTemp2,0,0,0);
	//sDrawText(9,416,szTemp2,0,0,1,0,640);
	//	pGLSwitch();
		//glColor3f(1.0f, 1.0f, 1.0f);//Green

	//pSetBlend(true);
	//p1CheckWindows(0x1B))
		
		//glEnd();
}

void CExInterface::DrawNavBar()
{
	//if(MAIN_MAPNUMBER == 30)
	//{
	//	return;
	//}
	
	float pos_x = 0.0;
	float pos_y = 0.0;

	//if (MAIN_RESOLUTION_WIDTH > 800) pos_y = 2.5;

	pBlendSwitch(1);
	//((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(INTERFACE_IMG::NAV_BAR, pos_x, pos_y, 58.0, 16.0, 0.0, 0.0, 0.9, 0.7, 1, 1, 0.0);
	//((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(INTERFACE_IMG::NAV_BAR, pos_x+58, pos_y, 58.0, 16.0, 0.0, 0.0, 0.9, 0.7, 1, 1, 0.0);
	//if (MouseLeftButtonDown == 1 && CheckZone(pos_x + 3.0f, pos_y + 1.5, 16.0, 12.0))
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(INTERFACE_IMG::NAV_BAR_CAM_SWITCH, pos_x + 3.2f, pos_y + 2.0, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);
	//else
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(INTERFACE_IMG::NAV_BAR_CAM_SWITCH, pos_x + 3.0f, pos_y + 1.5, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);

	//if (MouseLeftButtonDown == 1 && CheckZone(pos_x + 21.0f, pos_y + 1.5, 16.0, 12.0))
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(INTERFACE_IMG::NAV_BAR_CAM_RESET, pos_x + 21.2f, pos_y + 2.0, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);
	//else
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(INTERFACE_IMG::NAV_BAR_CAM_RESET, pos_x + 21.0f, pos_y + 1.5, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);

	//int interface_index = (gAttackHelper->State == ATTACKHELPER_STATE_Stop) ? INTERFACE_IMG::NAV_BAR_BOT_START : INTERFACE_IMG::NAV_BAR_BOT_PAUSE;

	//if (MouseLeftButtonDown == 1 && CheckZone(pos_x + 39.0f, pos_y + 1.5, 16.0, 12.0))
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(interface_index, pos_x + 39.2f, pos_y + 2.0, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);
	//else
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(interface_index, pos_x + 39.0f, pos_y + 1.5, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);

	//if (MouseLeftButtonDown == 1 && CheckZone(pos_x + 61.5f, pos_y + 1.5, 16.0, 12.0))
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3548, pos_x + 61.7f, pos_y + 2.0, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);
	//else
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3548, pos_x + 61.5f, pos_y +1.5, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);

	//if (MouseLeftButtonDown == 1 && CheckZone(pos_x + 79.5f, pos_y + 1.5, 16.0, 12.0))
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3595, pos_x + 79.7f, pos_y + 2.0, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);
	//else
	//((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3595, pos_x + 79.5f, pos_y +1.5, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);
	//	//
	//if (MouseLeftButtonDown == 1 && CheckZone(pos_x + 97.5f, pos_y + 1.5, 16.0, 12.0))
	//	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3595, pos_x + 97.7f, pos_y + 2.0, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);
	//else
	//((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3595, pos_x + 97.5f, pos_y +1.5, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);

	////if (MouseLeftButtonDown == 1 && CheckZone(pos_x + 97.5f, pos_y + 1.5, 16.0, 12.0))
	////	((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3700, pos_x + 97.7f, pos_y + 2.0, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);
	////else
	////((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3700, pos_x + 97.5f, pos_y +1.5, 16.0, 12.0, 0.0, 0.0, 0.9, 0.75, 1, 1, 0.0);

	float MainWidth			= 70.0;
	float StartY			= MAX_WIN_HEIGHT - 28.7;//(MAX_WIN_HEIGHT / 2) - (MainWidth / 2);
	if (MouseLeftButtonDown == 1 && CheckZone(pos_x + 175.0f, pos_y + StartY+0.5, 23.0, 22.0))
		((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3700, pos_x + 176.2f, pos_y + StartY, 23.0, 22.0, 0.0, 0.0, 0.73, 0.75, 1, 1, 0.0);
	else
		((int(__cdecl*)(int, float, float, float, float, float, float, float, float, BYTE, BYTE, float)) 0x006B97A7)(3700, pos_x + 176.0f, pos_y + StartY+0.5, 23.0, 22.0, 0.0, 0.0, 0.73, 0.75, 1, 1, 0.0);


	//if (CheckZone(pos_x + 3.0f, pos_y + 1.5, 16.0, 12.0))
	//	pDrawToolTipEx(pos_x + 3.0f, pos_y + 19.5, (char*)"Camera 3D");

	//if (CheckZone(pos_x + 21.0f, pos_y + 1.5, 16.0, 12.0))
	//	pDrawToolTipEx(pos_x + 21.0f, pos_y + 19.5, (char*)"Camera 3D [Reset]");

	//if (CheckZone(pos_x + 39.0f, pos_y + 1.5, 16.0, 12.0))
	//	pDrawToolTipEx(pos_x + 39.0f, pos_y + 19.5, (char*)"Attack Helper - Attack / Pause");

	//if (CheckZone(pos_x + 61.5f, pos_y + 1.5, 16.0, 12.0))
	//	pDrawToolTipEx(pos_x + 61.5f, pos_y + 19.5, (char*)"Jewel's Bank");

	//if (CheckZone(pos_x + 79.5f, pos_y + 1.5, 16.0, 12.0))
	//	pDrawToolTipEx(pos_x + 79.5f, pos_y + 19.5, (char*)"Reset System");
	////
	//if (CheckZone(pos_x + 97.5f, pos_y + 1.5, 16.0, 12.0))
	//	pDrawToolTipEx(pos_x + 97.5f, pos_y + 19.5, (char*)"Grand Reset System");

	if (CheckZone(pos_x + 175.0f, pos_y + StartY+0.5, 23.0, 22.0))
		pDrawToolTipEx(pos_x + 177.0f, pos_y + StartY-14, (char*)"Menu (O)");
	if(pCheckWindow(5)) //inventory
		{
			if(pCheckWindow(0x1A)
			|| pCheckWindow(7))
			{
				return;
			}

			//glColor3f(1.0, 1.0, 1.0);
			pDrawImage2(3695, 584,395, 26, 26, 0.0, 0.0, 0.830, 0.830, 1, 1, 0.0);

			if(pCursorX >= 584
			&&	(float)pCursorX <= 584 + 26
			&&	pCursorY >= 395 - 1 
			&&	pCursorY < 395 + 26 )
			{
				//char JewelBankText[50];
				//pDrawTextBackground = eDrawText::TextBackground::Black;
				//glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
				//pDrawBarForm(550,382, 70, 10);
				//pSetBlend(true);
				//glColor3f(1.0, 1.0, 1.0);
				//sprintf(JewelBankText, "Open Jewel Bank (B)");
				//MU_DrawTextOut(550, 382,JewelBankText,GetRealWidth(70), 1, 0);
				MU_DrawTextOut(580, 382, "Guild Bank", 0, 0, 0);
				//glEnd();
			}

		}

//		float StartXe = (float)(MAIN_WIDTH / 2) - 60.0f;
//	float StartYe = 100.0f;
//
//if (CheckZone(StartXe +163, StartYe - 80, 16.0, 12.0))
//		pDrawToolTipEx(StartXe +143, StartYe - 92, (char*)"Advance Stats");



	//pGLSwitch();
}
//struct PMSG_GUILD_BANK
//{
//	PBMSG_HEAD2 h;
//	BYTE Result;
//};
//struct PMSG_ANS_VALUE_MENU
//{
//	PBMSG_HEAD2 h;
//	int ValueOpen;
//	int ValueNumber;
//};
//void tests()
//{
//				PMSG_ANS_VALUE_MENU pMsg;
//			pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0xDE, sizeof(pMsg));
//			pMsg.ValueOpen = 2;
//			pMsg.ValueNumber = 0;
//			DataSend((LPBYTE)&pMsg, pMsg.h.size);
//}
void CExInterface::MouseClickProc()
{
	/*if(MAIN_MAPNUMBER == 30)
	{
		return;
	}*/

	float pos_x = 0.0;
	float pos_y = 0.0;
	float StartY			= MAX_WIN_HEIGHT - 28.7;


	if (MAIN_RESOLUTION_WIDTH > 800) pos_y = 2.5;

	//if (CheckZone(pos_x + 3.0f, pos_y + 1.5, 16.0, 12.0))
	//{
	//	gCamera->Switch();
	//}

	//if (CheckZone(pos_x + 21.0f, pos_y + 1.5, 16.0, 12.0))
	//{
	//	gCamera->Reset();
	//}

	//if (CheckZone(pos_x + 39.0f, pos_y + 1.5, 16.0, 12.0))
	//{
	//	if (gAttackHelper->State == ATTACKHELPER_STATE_Stop)
	//	{
	//		pSendChatTextEx((const char *)"", (const char *)"Attack Helper - Select Mode", Blue);
	//		gAttackHelper->SetState(ATTACKHELPER_STATE_SelectMode);
	//	}
	//	else
	//	{
	//		pSendChatTextEx((const char *)"", (const char *)"Attack Helper Off", Red);
	//		gAttackHelper->SetState(ATTACKHELPER_STATE_Stop);
	//	}
	//}
	//if (CheckZone(pos_x + 61.5f, pos_y + 1.5, 16.0, 12.0))
	//{

	//	

	//	//Reset
	//	if (gRanking.Active == 0)//gRanking.Active)
	//	{
	//		gRanking.Active = 1;

	//	}
	//	else
	//	{
	//		gRanking.Active = 0;
	//	}
	//	//if(gJewelsBank.Active)
	//	//		{
	//	//			gJewelsBank.Active = false;
	//	//			/*pSetCursorFocus= false; 
	//	//			pSetCursorFocus2 = false; */
	//	//		}
	//	//		else
	//	//		{
	//	//			gJewelsBank.Active = true;
	//	//			/*pSetCursorFocus= true; 
	//	//			pSetCursorFocus2 = true; */
	//	//		}
	//}

	//if (CheckZone(pos_x + 78.0f, pos_y + 1.5, 16.0, 12.0))
	//{
	//	//Reset
	//	if (g_ResetSystem.State == 0)//gRanking.Active)
	//	{
	//		g_ResetSystem.State = 1;

	//	}
	//	else
	//	{
	//		g_ResetSystem.State = 0;
	//	}
	//}
	////Reset
	//if (CheckZone(pos_x + 94.5f, pos_y + 1.5, 16.0, 12.0))
	//{
	//	
	//	//Reset
	//	if (gInterfaces->Data[eRageTable].OnShow==0)
	//	{
	//		gInterfaces->Data[eRageTable].OnShow = 1;

	//	}
	//	else
	//	{
	//		gInterfaces->Data[eRageTable].OnShow = 0;
	//	}
	//	//if(g_ResetSystem.StateGR == 0)//gRanking.Active)
	//	//		{
	//	//			//gRanking.Active = false;
	//	//			//g_ResetSystem.Active = true;
	//	//			g_ResetSystem.StateGR = 1;
	//	//			//pSetCursorFocus= false; 
	//	//			//pSetCursorFocus2 = false; 
	//	//			//MouseLeftButtonDown == 1;

	//	//		}
	//	//		else
	//	//		{
	//	//			//gRanking.Active = true;
	//	//			//g_ResetSystem.Active = false;
	//	//			g_ResetSystem.StateGR = 0;

	//	//			/*pSetCursorFocus= true; 
	//	//			pSetCursorFocus2 = true; */
	//	//		}
	//	//gRanking.Show = !gRanking.Show;
	//}

	if (CheckZone(pos_x + 175.0f, pos_y + StartY+0.5, 23.0, 22.0))
	{
		if(gMenuSystem.Active == 0)
		{

			gMenuSystem.Active = 1;

		}
		else
		{
			gMenuSystem.Active = 0;
		}

	}
	if(pCheckWindow(5)) //inventory
	{
		if (CheckZone(584, 395, 23.0, 22.0))
		{
			if(gInterfaces->StateWarex == 0)
		{
			gInterfaces->StateWarex = 1;
			
		}
		else
		{
			gInterfaces->StateWarex = 0;
		}
			/*if(pCursorX >= 584
			&&	(float)pCursorX <= 584 + 26
			&&	pCursorY >= 395 - 1 ...
			&&	pCursorY < 395 + 26 )*/
			//gInterfaces->CloseWindow(5);
			/*PMSG_ANS_VALUE_MENU pMsg;
			pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0xDE, sizeof(pMsg));
			pMsg.ValueOpen = 2;
			pMsg.ValueNumber = 0;
			DataSend((LPBYTE)&pMsg, pMsg.h.size);*/
			//tests();
			/*PMSG_GUILD_BANK pMsg;
			pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0xD8, sizeof(pMsg));
			pMsg.Result = 1;
			DataSend((LPBYTE)&pMsg, pMsg.h.size);*/
			//gMenuSystem.Active = 1;
		}
	}
}