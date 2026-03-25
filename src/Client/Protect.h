// Protect.h: interface for the CProtect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.h"
//#include "CustomItem.h"
//#include "CustomItemBow.h"
//#include "CustomJewel.h"
//#include "CustomMap.h"
//#include "CustomMessage.h"
//#include "CustomMonster.h"
//#include "CustomTooltip.h"
//#include "CustomWing.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "CustomFenrir.h"

struct MAIN_FILE_INFO
{
	//char CustomerName[32];
	BYTE LauncherStart;
	char LauncherMutex[32];
	//char IpAddress[32];
	////WORD IpAddressPort;
	///*char ClientVersion[8];
	//char ClientSerial[17];
	//char WindowName[32];
	//char ScreenShotPath[50];*/
	char ClientName[32];
	DWORD ClientNameCRC;
	char PluginName[3][32];
	DWORD PluginNameCRC[4];
	///*BYTE SelectServerCustom;
	//BYTE SelectCharCustom;
	//DWORD MaxAttackSpeed[7];
	//BYTE AttackAnimationValue[7];
	//BYTE RemoveLimitFPS;
	//DWORD ReconnectTime;*/
	///*CUSTOM_ITEM_INFO CustomItemInfo[MAX_CUSTOM_ITEM];
	//CUSTOM_ITEM_BOW_INFO CustomItemBowInfo[MAX_CUSTOM_ITEM_BOW];
	//CUSTOM_JEWEL_INFO CustomJewelInfo[MAX_CUSTOM_JEWEL];
	//CUSTOM_MAP_INFO CustomMapInfo[MAX_CUSTOM_MAP];
	//CUSTOM_MESSAGE_INFO EngCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	//CUSTOM_MESSAGE_INFO PorCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	//CUSTOM_MESSAGE_INFO SpnCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	//CUSTOM_MONSTER_INFO CustomMonsterInfo[MAX_CUSTOM_MONSTER];
	//CUSTOM_TOOLTIP_INFO CustomTooltipInfo[MAX_CUSTOM_TOOLTIP];
	//CUSTOM_WING_INFO CustomWingInfo[MAX_CUSTOM_WING];*/
	ITEM_STACK_INFO ItemStackInfo[MAX_ITEM_STACK_INFO];
	ITEM_VALUE_INFO ItemValueInfo[MAX_ITEM_VALUE_INFO];
	CUSTOM_FENRIR_INFO			CustomFenrirInfo[MAX_FENRIR_INFO];
	////BYTE EnableSkillSiegeAllMaps;
	////BYTE RemoveRecuo;

	//char Patente[5][32];
};

class CProtect
{
public:
	CProtect();
	virtual ~CProtect();
	bool ReadMainFile(char* name);
	void CheckLauncher();
	void CheckInstance();
	void CheckClientFile();
	void CheckPluginFile();
public:
	MAIN_FILE_INFO m_MainInfo;
	DWORD m_ClientFileCRC;
};

extern CProtect gProtect;