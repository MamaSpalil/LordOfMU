#pragma once

namespace INTERFACE_IMG
{
	enum __T
	{

		NAV_BAR = 3535,
		NAV_BAR_CAM_SWITCH = 3536,
		NAV_BAR_CAM_RESET = 3537,
		NAV_BAR_BOT_START = 3538,
		NAV_BAR_BOT_PAUSE = 3539,


		//// -- Native
		//MESSAGE_BOX_01 = 0xFB,
		//MESSAGE_BOX_02 = 0xFC,

		//BTN_NO_1 = 0x8CF,
		//BTN_NO_2 = 0x8D0,
		//BTN_NO_3 = 0x8D1,

		//BTN_OK_1 = 0x8D2,
		//BTN_OK_2 = 0x8D3,
		//BTN_OK_3 = 0x8D4,

		//BTN_YES_1 = 0x8D5,
		//BTN_YES_2 = 0x8D6,
		//BTN_YES_3 = 0x8D7,

		//BTN_BACK_1 = 0xFE,
		//BTN_BACK_2 = 0xFF,
		//BTN_BACK_3 = 0x100,
		//
		//BTN_ACCEPT_BOX_1 = 0x122,
		//BTN_ACCEPT_BOX_2 = 0x123,

		//BTN_EXIT_01 = 0x118,
		//BTN_EXIT_02 = 0x119,
	};
};



class CExInterface
{
public:
	void Init();
	//static void LoadImages(int a1, int a2, int a3, HDC a4);

	void DrawTimer();
	void DrawNavBar();
	void MouseClickProc();
	DWORD server_timer;
	DWORD server_tick;
};

extern CExInterface * gExInterface;
static void LoadImages(int a1, int a2, int a3, HDC a4);//void LoadImages(HDC ItemID);