#include "stdafx.h"
#include "protocol.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Other.h"
#include "HPBar.h"
#include "Structure.h"
#include "Reconnect.h"
#include "Packet.h"
#include <process.h>
#include "Reconnect.h"
#include "Visual.h"
#include "ObjectManager.h"
#include "Interface.h"
#include "AttackHelper.h"
#include "InterfaceEx.h"
#include "User.h"
#include "AntiCheatPlus.h"
#include "ResetSystem.h"
#include "Ranking.h"
#include "RageSystem.h"
#include "CustomMenu.h"
#include "JewelsBank.h"
#include "AdvancedStats.h"
#include "SelectChar.h"
#include "Fog.h"
#include "DungeonSiege.h"
#include "Shop.h"
//#include "Controller.h"
#include "LuckyWheel.h"

DWORD *GameIndex = (DWORD*)0x587C314;//
int *CMAIN_STATE2 = (int*)0x007E8088;
DWORD EXP		= NULL;
DWORD Experience = 0;
DWORD DAMAGE	= NULL;
DWORD SHIELD	= NULL;
DWORD MAGIC		= NULL;
DWORD dwDmgValue = NULL;
BYTE byXorFilter[32];


void DataSend(LPBYTE Data, int Size)
{
		byXorFilter[0] = 0xE7;
		byXorFilter[1] = 0x6D;
		byXorFilter[2] = 0x3A;
		byXorFilter[3] = 0x89;
		byXorFilter[4] = 0xBC;
		byXorFilter[5] = 0xB2;
		byXorFilter[6] = 0x9F;
		byXorFilter[7] = 0x73;
		byXorFilter[8] = 0x23;
		byXorFilter[9] = 0xA8;
		byXorFilter[10] = 0xFE;
		byXorFilter[11] = 0xB6;
		byXorFilter[12] = 0x49;
		byXorFilter[13] = 0x5D;
		byXorFilter[14] = 0x39;
		byXorFilter[15] = 0x5D;
		byXorFilter[16] = 0x8A;
		byXorFilter[17] = 0xCB;
		byXorFilter[18] = 0x63;
		byXorFilter[19] = 0x8D;
		byXorFilter[20] = 0xEA;
		byXorFilter[21] = 0x7D;
		byXorFilter[22] = 0x2B;
		byXorFilter[23] = 0x5F;
		byXorFilter[24] = 0xC3;
		byXorFilter[25] = 0xB1;
		byXorFilter[26] = 0xE9;
		byXorFilter[27] = 0x83;
		byXorFilter[28] = 0x29;
		byXorFilter[29] = 0x51;
		byXorFilter[30] = 0xE8;
		byXorFilter[31] = 0x56;

	int StartPos = 0;
	// ----
	if( Data[0] == 0xC1 || Data[0] == 0xC3 )
	{
		StartPos = 3;
	}
	else if( Data[0] == 0xC2 || Data[0] == 0xC4 )
	{
		StartPos = 4;
	}
	// ----
	for( int i = StartPos; i < Size; i++ )
	{
		Data[i] ^= Data[i - 1] ^ byXorFilter[i%32];
	}
	// ----
	send(pActiveSocket, (char*)Data, Size, 0);
}
//---------------------------------------------------------------------------


void cProtocolLoad()
{
	
	
	// ----
	SetOp( (LPVOID)iDataRecvCustom, (LPVOID)cProtocol::cDataRecv, ASM::CALL );
	HookJump(0x4F5F42, cProtocol::UpdateCharacterNoEffect);
	HookJump(0x4F5CBD, cProtocol::UpdateCharacterEffect);
	HookJump(0x662470, cProtocol::LevelUpPoints);
	FixDMG();
	//SpeedFix();//Ver 2.0
	// ----

}
//---------------------------------------------------------------------------
cProtocol Protocol;

void cProtocol::cDataRecv(DWORD protoNum, BYTE * aRecv, int aLen, int aIndex)
{
	BYTE ProtocolType = aRecv[0];
	BYTE Head = aRecv[(aRecv[0] == 0xC2 || aRecv[0] == 0xC4) ? 3 : 2];
	BYTE SubCode = aRecv[(aRecv[0] == 0xC2 || aRecv[0] == 0xC4) ? 4 : 3];
	// ----

	if( aRecv[2] == 0xF4 && aRecv[3] == 3)
	{
		PMSG_ANS_SERVER_INFO * lpDef = (PMSG_ANS_SERVER_INFO *)aRecv;
		sprintf(gConnectEx.m_GSIP, "%s", lpDef->Ip);
		gConnectEx.m_GSPort = lpDef->Port;
		DataRecv(protoNum, aRecv, aLen, aIndex);
		return;
	}

	gRanking.RecvGS(protoNum, aRecv, aLen, aIndex);

	//switch(ProtocolType)
	switch (Head)
	{

		case 0x9C:
		case 0x16:
		
			KillExpRecv((PMSG_KILLPLAYER*)aRecv, aIndex, protoNum);//Head);
			return;
		break;
		case 0xDC:
		{
		
			AttackRecv((PMSG_ATTACKRESULT*)aRecv, aIndex);
			return;
		}
		break;
		case 0x17:
			UserDieRecv((PMSG_DIEPLAYER*)aRecv);
		break;
		/*case 0x2D:
		{
			GCPeriodicEffectRecv((PMSG_PERIODIC_EFFECT_RECV*)aRecv);

			return;
		}*/
		/*case 0x1D:
			gObjectManager->ObjectMove((PMSG_MOVE_SEND*)aRecv);
		break;*/
		
		case 0xF5:
			{
switch (SubCode)
				{
					case 0x02:
					{	
						if (gInterfaces->StateD == 1)//gRanking.Active)
						{
							gInterfaces->StateD = 0;

						}
						else
						{
							gInterfaces->StateD = 1;
						}
						if (gInterfaces->StateFog == 1)//gRanking.Active)
						{
							gInterfaces->StateFog = 0;
							//FogOff();

						}
						else
						{
							gInterfaces->StateFog = 1;
							//FogOn();
						}
						//gInterfaces->StateD = 0;
						//MessageBoxA(NULL,"Client Hacked !","Check EvilTwin",MB_OK);
						Protocol.Agility();
					}
					break;
				}
			}
			break;
		case 0xF1:
		{
			switch (SubCode)
			{
			case 0x00:
			{
				//Visual.Index((PMSG_JOINRESULT*)aRecv);

				//CONNECTEX_LOGIN pRequest;
				//pRequest.h.set((LPBYTE)&pRequest, 0xFB, 12, sizeof(CONNECTEX_LOGIN));
				//memcpy(pRequest.AccountID, "1234", 10);
				//memcpy(pRequest.Password, "1234", 10);
				//memcpy(pRequest.Name, "22222", 10);
				//pRequest.TickCount = GetTickCount();
				//DataSend((LPBYTE)&pRequest, pRequest.h.size);
#ifdef AUTO_LOGIN
						Protocol.AutoLogin();
#endif
#if(EVENT_DUNGEON_SIEGE)
						g_DungeonSiege.CGHWID();
#endif
#if(ANTI_CHEAT_PLUS==TRUE)
						g_AntiCheatPlus.CG_HddSend();
#endif
			}
			break;
			}
		}
		break;
			//case 0x03: // Login
			//	{

			//	}
			//	break;
				// ----
			case 0xF3:
				{
					switch (SubCode)
					{
					case 0x00:
					{
#ifdef MULTI_CHARACTERS
						gInterface.Allow_DrawCharactersWindow();
#endif
#ifdef CheckClient
						ClientAccept();
#endif	
					
					}
					break;
					case 0x03: //Character selected (join map)
						{
							//CharacterJoinMapResultRecv((PMSG_CHARMAPJOINRESULT *)aRecv, aIndex);
							TpList = false;
							#if(DEV_DAMAGE_TABLE)
							gObjUser.m_SecondDamageMax = 0;
							gObjUser.m_SecondDamage = 0;
							gObjUser.m_SecondDefence = 0;
							gObjUser.m_SecondReflect = 0;
							#endif
							//Visual.Respawn();
							gConnectEx.RestoreGame();
							gAttackHelper->SetState(ATTACKHELPER_STATE_Stop);
							Protocol.Agility();
						}
						break;
					case 0x04: //Respawn
						{
							//Visual.Respawn();
						}
					 break;
					 case 0x05:
						{
							Protocol.Update = false;
							//EXP--;
						}
					break;
					case 0x06:
						{
							Protocol.Update = false;
							//Protocol.RealLevelUpPoint--;
						}
					break;
					case 0x07:
						{
							//Visual.GCMonsterDamageRecv((PMSG_MONSTER_DAMAGE_RECV*)aRecv);
						
							DamageRecv((PMSG_DAMAGE*)aRecv, aIndex);
						}
						break;
					case 0xE2:
					{
						gHPBar.Clear();

						PMSG_HEALTHBAR* lpMsg = (PMSG_HEALTHBAR*)(aRecv);

						for (int i = 0; i < lpMsg->Count; i++)
						{
							PMSG_HEALTHBAR_INFO* Info = (PMSG_HEALTHBAR_INFO*)(&aRecv[sizeof(PMSG_HEALTHBAR) + (sizeof(PMSG_HEALTHBAR_INFO) * i)]);
							gHPBar.Insert(Info->Index, Info->Rate);
						}
					}
					break;
					/*case 0xE7:
					{
						GCShopPriceListRecv((PMSG_SHOP_PRICE_LIST_RECV*)aRecv);
						return;
					}
					break;*/
					
					}
				}
				break;
				// ----


		

	case 0xFD:
	{
		switch (SubCode)
		{
		case 0x01:
		{
			PMSG_LEVELUP Result;
			PMSG_UPDATECHARACTER* Info = (PMSG_UPDATECHARACTER*)(aRecv);
			
			Protocol.Update = true;
			Protocol.Level = Info->Level;
			Protocol.LevelUpPoint = Info->LevelUpPoint;
			Protocol.RealLevelUpPoint = Info->LevelUpPoint;
			Protocol.Experience = Info->Experience;
			Protocol.NextExperience = Info->NextExperience;
			Protocol.Strength = Info->Strength;
			Protocol.Dexterity = Info->Dexterity;
			Protocol.Vitality = Info->Vitality;
			Protocol.Energy = Info->Energy;
			Protocol.Leadership = Info->Leadership;
			Protocol.Life = Info->Life;
			Protocol.MaxLife = Info->MaxLife;
			Protocol.Mana = Info->Mana;
			Protocol.MaxMana = Info->MaxMana;
			Protocol.Shield = Info->Shield;
			Protocol.MaxShield = Info->MaxShield;
			Protocol.BP = Info->BP;
			Protocol.MaxBP = Info->MaxBP;
			Visual.Life = Info->MaxLife;
			Visual.Mana = Info->MaxMana;
			Visual.Ag = Info->MaxBP;
			Visual.Sd = Info->MaxShield;
			Visual.ResetCount = Info->ResetCount;

			Result.h.c = 0xC1;
			Result.h.size = sizeof(Result);
			Result.h.headcode = 0xF3;
			Result.subcode = 0x05;

			gObjectManager->GetResets();
			gObjectManager->GetGResets();

			LevelUpRecv(&Result);
		}
		break;
		case 0x02:
		{
			PMSG_LVPOINTADD Result;
			PMSG_UPDATECHARACTER* Info = (PMSG_UPDATECHARACTER*)(aRecv);

			Protocol.Update = true;
			Protocol.LevelUpPoint = Info->LevelUpPoint + 1;
			Protocol.RealLevelUpPoint = Info->LevelUpPoint;
			Protocol.Strength = Info->Strength - 1;
			Protocol.Dexterity = Info->Dexterity;
			Protocol.Vitality = Info->Vitality;
			Protocol.Energy = Info->Energy;
			Protocol.Leadership = Info->Leadership;
			Protocol.Life = Info->Life;
			Protocol.MaxLife = Info->MaxLife;
			Protocol.Mana = Info->Mana;
			Protocol.MaxMana = Info->MaxMana;
			Protocol.Shield = Info->Shield;
			Protocol.MaxShield = Info->MaxShield;
			Protocol.BP = Info->BP;
			Protocol.MaxBP = Info->MaxBP;
			Visual.Life = Info->MaxLife;
			Visual.Mana = Info->MaxMana;
			Visual.Ag = Info->MaxBP;
			Visual.Sd = Info->MaxShield;
			Visual.ResetCount = Info->ResetCount;
			
			Result.h.c = 0xC1;
			Result.h.size = sizeof(Result);
			Result.h.headcode = 0xF3;
			Result.subcode = 0x06;
			Result.ResultType = 0x10;
			Result.MaxLifeAndMana = Info->MaxLife;
			Result.wMaxShield = Info->MaxShield;
			Result.MaxBP = Info->MaxBP;
			
			gObjectManager->GetResets();
			gObjectManager->GetGResets();
			LevelUpAddPoint(&Result);
		}
		break;
		case 0x07://tyt bag
		{
			
			PMSG_CHARINFOMAIN* Info = (PMSG_CHARINFOMAIN*)(aRecv);

			Protocol.Class = Info->Class;
			Protocol.Dexterity = Info->Dexterity;
			Protocol.AttackSpeed = Info->AttackSpeed;
			Protocol.MagicSpeed = Info->MagicSpeed;
			Protocol.RealLevelUpPoint = Info->LevelUpPoint;
			//Protocol.ResetCount = Info->ResetCount;
			Visual.ResetCount = Info->ResetCount;
		
			Protocol.Agility();
		}
		break;
		case 0x08:
		{
			PMSG_UPDATEPOINTS* Info = (PMSG_UPDATEPOINTS*)(aRecv);

			Protocol.RealLevelUpPoint = Info->LevelUpPoint;
			Visual.ResetCount = Info->ResetCount;
		}
		break;
		}
	}
	break;
	case 0xFB:
			{
				PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2*)aRecv;
				// ----
				switch(lpDef->subcode)
				{

				case 0x00:
					gRageSystem.RecvConfig((sConfigStruct *)aRecv);
					break;
				case 0x01:
					gRageSystem.RageRecv((ClickSend*)aRecv);
					break;
				case 0x02:
					gRageSystem.RecvStart((sRageUserSend*)aRecv);
					break;
				case 0x03:
					gRageSystem.RecvExp((RageExpSend *)aRecv);
					break;

					case 0xC:
					{

						gConnectEx.m_ConnectState = ConnectExType::OnForceDisconnect;
						gConnectEx.Disconnect();

					}
					break;
					
					case 0xAD:
					{
	
						gConnectEx.SetAccInformation((SDHP_RECONNECT_IDPASS*)aRecv);

					}
					case 0x05:
						gObjectManager->ObjectResetRecv((GC_RESET_DATA1*)aRecv);
					break;
					case 0x06:
						gObjectManager->ObjectUpPointsRecv((GC_UpPoints_DATA*)aRecv);
					break;
					case 0x07:
						gObjectManager->ObjectGResetRecv((GC_GRESET_DATA*)aRecv);
					break;
					/*case 0x08:
						gObjectManager->ObjectExpRecv((GC_Exp_DATA*)aRecv);
					break;*/
					case 0x70:
						GCServerTime((PMSG_SERVER_TIME*)aRecv);
					break;
					//case 13://propensity
					//{
					//	Propensity((Propensity_*)aRecv);
					//}
					//break;
#if(_JEWELS_BANK_)
					case 14:
					{
						gJewelsBank.JewelsBankRecv((EXINFO_UPDATE*)aRecv);
					}
					break;
#endif
					case 15:
					{
						gMenuSystem.InfoRecv((INFO_UPDATE*)aRecv);
					}
					break;
					/*case 18:
						{
							 NewsRecv((INFO_NEWS*)aRecv);
						}
						break;*/
		case 0x16:
			UpdateLuckyWheel((ITEM_WIN_SEND*)aRecv);
			break;
		case 0xFB://fb
			GCLuckyWheelRecv((PMSG_LUCKYWGEEL_LIST__RECV*)aRecv);
			break;
		case 0xFC://fc
			gLuckyWheel.RecvInfo((CG_LUCKYWHEEL_RECV*)aRecv);
			break;
					case 0x43:
						g_ResetSystem.GCDialogInfo((GC_RESET_DATAEx*)aRecv);
					break;
					case 0xA1:
						g_ResetSystem.GCGrandDialogInfo((PMSG_GRAND_DIALOG*)aRecv);
					break;
#if(EVENT_DUNGEON_SIEGE)
				case 0x44:
					g_DungeonSiege.GCNpcInfo((PMSG_GC_DUNGEON_SIEGE_NPC*)aRecv);
					break;
				case 0x54:
					g_DungeonSiege.GCGuildNumber((PMSG_GC_GUILD_INDEX*)aRecv);
					break;
				case 0x55:
					g_DungeonSiege.GCSiegeScore((PMSG_DUNGEON_SIEGE_SCORE*)aRecv);
					break;
				case 0x56:
					g_DungeonSiege.GCPlayerConnect((PMSG_DUNGEON_SIEGE_PLAYER_CONNECT*)aRecv);
					break;
				case 0x57:
					g_DungeonSiege.GCEventState((PMSG_DUNGEON_SIEGE_STATE*)aRecv);
					break;
#endif
#if(DEV_STATS_ADVANCE)
				case 0xDA:
					g_StatsAdvance.Recv((PMSG_STATS_ADVANCE*)aRecv);
					break;
#endif
#if(DEV_DAMAGE_TABLE)
				case 0xDC:
						RecvDamageTable((PMSG_DAMAGE_TABLE*)aRecv);
					break;
#endif
#if(ANTI_CHEAT_PLUS==TRUE)
				case 0xFF:
					g_AntiCheatPlus.GC_Recv(aRecv);
					break;
#if(ANTI_CHEAT_PLUS_CRC)
				case 0xFE:
					g_AntiCheatPlus.GC_CRCRecv((PMSG_CRCBase*)aRecv);
					break;
				case 0xFD:
				case 0xFC:
				case 0xFB:
					g_AntiCheatPlus.CGAnsSendFile((PMSG_FILESEND*)aRecv);
					break;
#endif
#endif
#if(CUSTOM_VALUE_MENU)
				case 0xDE:
					GCRecvValueMenu((PMSG_REQ_VALUE_MENU*)aRecv);
					break;
				#endif
				case 0xC1:
					gObjUser.WarehouseReq((CHANGEWAREHOUSE*)aRecv);
					break;
				}
			}
			break;


			case 0xF9: // Custom
				switch(aRecv[3])
					{
				/*case 0x01:
					{
						GCGetRenasLeftInfo((PMSG_ARCHER_INFO_LEFT_RENAS *)aRecv);
						break;
					}
				case 0x02:
					{
						GCGetRenasStatInfo((PMSG_ARCHER_INFO_STAT_RENAS *)aRecv);
						break;
					}*/
					case 0x04:
					{
						GCGetRecupationCharacter((PMSG_SEND_CHARACTER_INFO *)aRecv);
					break;
					}
					/*case 0x05:
				{
				GCGetServerTime((PBMSG_CLOCK *)aRecv);
				  PBMSG_CLOCK * gObj = (PBMSG_CLOCK *) aRecv;
				break;

					}*/
			case 0x06:
					{
						GCGetPostMessage((PMSG_CHATPOSTDATA *)aRecv);
					break;
					}
					  
				}
				break;

			/*case 0xF7:
				{				
#ifdef PROPENSITY_SYSTEM
				GCGetVpPropensitySystem((PMSG_PROPENSITY_VIEWPORT_NOTIFY *)aRecv);
#endif
				}
				break;*/

          
		

		case 0xFE:
		{
#ifdef MULTI_WAREHOUSE
			BANK_START_INFO * pMsg = (BANK_START_INFO*)aRecv;
			gInterfaces->Allow_DrawBankCountWindow();
			gInterfaces->Zen = pMsg->Zen;
			gInterfaces->TotalBanks = pMsg->TotalWarehouseHave;
#endif
		}
		break;
			//}
	
	

			// --
	
		//	}
		case 0xC:
		{
			gConnectEx.m_ConnectState = ConnectExType::OnForceDisconnect;
		}
		break;
	
	
		//}
		//break;
	}

		
	//if (aRecv[2] == 0x16)
	//{
	//	dwDmgValue = MAKE_NUMBERDW(MAKE_NUMBERW(aRecv[7], aRecv[9]), MAKE_NUMBERW(aRecv[8], aRecv[10]));

	//	char tmp[5000] = { 0 };

	//	for (int i = 0; i<aLen; i++)
	//	{
	//		char sym[10];
	//		sprintf(sym, "%02X ", aRecv[i]);
	//		strcat(tmp, sym);
	//	}

	//	if (dwDmgValue == 0)
	//		dwDmgValue = -1;

	//	aLen -= 2;
	//	aRecv[1] -= 2;
	//}

	// ----
	DataRecv(protoNum, aRecv, aLen, aIndex);




}
//---------------------------------------------------------------------------
//void GCGetServerTime(PBMSG_CLOCK * gObj)
//{
//	ServerTime ST;
//
//	ST.Hour = (int)gObj->Hour;
//	ST.Minute = (int)gObj->Minute;
//	ST.Second = (int)gObj->Second;
//}
void GCShopPriceListRecv(PMSG_SHOP_PRICE_LIST_RECV* lpMsg) // OK
{
	ClearShopPriceList();

	ShopValueType = lpMsg->type;

	for (int n = 0; n<lpMsg->count; n++)
	{
		PMSG_SHOP_PRICE_RECV* lpInfo = (PMSG_SHOP_PRICE_RECV*)(((BYTE*)lpMsg) + sizeof(PMSG_SHOP_PRICE_LIST_RECV) + (sizeof(PMSG_SHOP_PRICE_RECV)*n));

		InsertShopPrice(lpInfo->ItemIndex, lpInfo->ItemLevel, lpInfo->ItemDur, lpInfo->ItemNewOption, lpInfo->ItemPrice);
	}
}




//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void GCGetRenasStatInfo(PMSG_ARCHER_INFO_STAT_RENAS * gObj)
{

	// ----
	char szTemp[256];
	if(gObj->Result == 1)
	{
	sprintf(szTemp, "You need many Rena's.Please register them to Golden Archer in Lorencia.");
	}
	else if (gObj->Result == 2)
	{

	sprintf(szTemp, "Please choose the item. 1 item you can choose for %d Rena's",gObj->NeedRenas);
	}
	// ----
	//MU_DrawInfoBox(szTemp);

}
//---------------------------------------------------------------------------
void GCGetRenasLeftInfo(PMSG_ARCHER_INFO_LEFT_RENAS * gObj)
{

	// ----
	char szTemp[256];
	sprintf(szTemp, "You have left %d rena's.", gObj->LeftRenas);
	// ----
	//MU_DrawInfoBox(szTemp);

}

//---------------------------------------------------------------------------
void GCGetPostMessage(PMSG_CHATPOSTDATA * gObj)
{

	// ----
	char szTemp[256];
	//sprintf(szTemp, "You have left %d rena's.", gObj->chatmsg);
	sprintf(szTemp, "%s %s",gObj->chatid,gObj->chatmsg);
	
	

	// ----
	//MU_DrawInfoBox(szTemp);


	 //MU_SendChatText((const char *)"", (const char *)szTemp, ChatColor::Red);

}



//--

void BankNumberOpen(int BankNum)
{

	BANK_OPEN_NUM pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFE, 0x01, sizeof(BANK_OPEN_NUM));
	pRequest.BankNum = BankNum;
//	pRequest.TickCount = GetTickCount();
	DataSend((BYTE*)&pRequest, pRequest.h.size);


}

void PageCharacterSet(int PageNum)
{
	SDHP_GETNUMPAGE_MULTICHAR pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFA, sizeof(SDHP_GETNUMPAGE_MULTICHAR));
	pRequest.Page = PageNum;
	//	pRequest.TickCount = GetTickCount();
	DataSend((BYTE*)&pRequest, pRequest.h.size);

}
void CreateBank(int BankHave)
{
	if (BankHave >= 12)
	{
		return;
	}
	CREATE_BANK pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFE, 0x02, sizeof(CREATE_BANK));
	pRequest.BankNum = 1;
	//	pRequest.TickCount = GetTickCount();
	DataSend((BYTE*)&pRequest, pRequest.h.size);


}
void CharacterJoinMapResultRecv(PMSG_CHARMAPJOINRESULT * lpRecv, int key)
{
	//gAttackHelper->SetState(ATTACKHELPER_STATE_Stop);
	//gObjectManager->SetUpPoints(lpRecv->LevelUpPoint);
	//gObjectManager->SetLife(lpRecv->Life, lpRecv->wShield, 0xFF);
	//gObjectManager->SetLife(lpRecv->MaxLife, lpRecv->wMaxShield, 0xFE);
	//gObjectManager->SetMana(lpRecv->Mana, lpRecv->BP, 0xFF);
	//gObjectManager->SetMana(lpRecv->MaxMana, lpRecv->MaxBP, 0xFE);
	//gObjectManager->SetPosition(lpRecv->MapX, lpRecv->MapY);


	//if (gServerInfo->MuConfig.FixAttackSpeed == 1)
	//{
		BYTE value = 0x0F;

		int Class = lpRecv->Class;

		if (Class != 1)
		{ 
			value = 0x02;
		}

		SetByte((LPVOID)(0x00524850), value);
	//}

	PMSG_CHARMAPJOINRESULT_PRIMARY pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xF3, sizeof(PMSG_CHARMAPJOINRESULT_PRIMARY));
	pMsg.subcode = lpRecv->subcode;
	pMsg.MapX = lpRecv->MapX;
	pMsg.MapY = lpRecv->MapY;
	pMsg.MapNumber = lpRecv->MapNumber;
	pMsg.Dir = lpRecv->Dir;
	pMsg.Exp = lpRecv->Exp;
	pMsg.NextExp = lpRecv->NextExp;
	pMsg.LevelUpPoint = GET_MAX_WORD_VALUE(lpRecv->LevelUpPoint);
	pMsg.Str = lpRecv->Str;
	pMsg.Dex = lpRecv->Dex;
	pMsg.Vit = lpRecv->Vit;
	pMsg.Energy = lpRecv->Energy;
	pMsg.Life = GET_MAX_WORD_VALUE(lpRecv->Life);
	pMsg.MaxLife = GET_MAX_WORD_VALUE(lpRecv->MaxLife);
	pMsg.Mana = GET_MAX_WORD_VALUE(lpRecv->Mana);
	pMsg.MaxMana = GET_MAX_WORD_VALUE(lpRecv->MaxMana);
	pMsg.wShield = GET_MAX_WORD_VALUE(lpRecv->wShield);
	pMsg.wMaxShield = GET_MAX_WORD_VALUE(lpRecv->wMaxShield);
	pMsg.BP = GET_MAX_WORD_VALUE(lpRecv->BP);
	pMsg.MaxBP = GET_MAX_WORD_VALUE(lpRecv->MaxBP);
	pMsg.Money = lpRecv->Money;
	pMsg.PkLevel = lpRecv->PkLevel;
	pMsg.CtlCode = lpRecv->CtlCode;
	pMsg.AddPoint = lpRecv->AddPoint;
	pMsg.MaxAddPoint = lpRecv->MaxAddPoint;
	pMsg.Leadership = lpRecv->Leadership;
	pMsg.wMinusPoint = lpRecv->wMinusPoint;
	pMsg.wMaxMinusPoint = lpRecv->wMaxMinusPoint;

	pProtocolCores(0xF3, (LPBYTE)&pMsg, pMsg.h.size, key);
}
void AttackRecv(PMSG_ATTACKRESULT*lpRecv, int key)
{
	gObjectManager->SetDamage(lpRecv->DamageHP, lpRecv->DamageSD);
	gObjectManager->CheckDamage(MAKE_NUMBERW(lpRecv->NumberH, lpRecv->NumberL), lpRecv->DamageHP, lpRecv->DamageSD);

	PMSG_ATTACKRESULT_PRIMARY pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xDC, sizeof(PMSG_ATTACKRESULT_PRIMARY));
	pMsg.NumberH = lpRecv->NumberH;
	pMsg.NumberL = lpRecv->NumberL;
	pMsg.DamageType = lpRecv->DamageType;
	pMsg.DamageH = SET_NUMBERH(GET_MAX_WORD_VALUE(lpRecv->DamageHP));
	pMsg.DamageL = SET_NUMBERL(GET_MAX_WORD_VALUE(lpRecv->DamageHP));
	pMsg.btShieldDamageH = SET_NUMBERH(GET_MAX_WORD_VALUE(lpRecv->DamageSD));
	pMsg.btShieldDamageL = SET_NUMBERL(GET_MAX_WORD_VALUE(lpRecv->DamageSD));
	//pProtocolCore(0xDC, (LPBYTE)&pMsg, pMsg.h.size, key);
	pProtocolCores(0xDC, (LPBYTE)&pMsg, pMsg.h.size, key);
	//DataSend((BYTE*)&pMsg, pMsg.h.size);
}
void UserDieRecv(PMSG_DIEPLAYER * lpRecv)
{
	gObjectManager->ObjectDie(MAKE_NUMBERW(lpRecv->NumberH, lpRecv->NumberL));
}
void KillExpRecv(PMSG_KILLPLAYER * lpRecv, int key, int Protonum)
{
	gObjectManager->SetDamage(lpRecv->DamageHP, 0);
	gObjectManager->SetExperience(lpRecv->Experience);
	gObjectManager->ObjectDie(MAKE_NUMBERW(lpRecv->NumberH, lpRecv->NumberL));

	PMSG_KILLPLAYER_PRIMARY pMsg;
	pMsg.h.set((LPBYTE)&pMsg, Protonum, sizeof(PMSG_KILLPLAYER_PRIMARY));
	pMsg.NumberH = lpRecv->NumberH;
	pMsg.NumberL = lpRecv->NumberL;
	pMsg.ExpH = SET_NUMBERH(GET_MAX_WORD_VALUE(lpRecv->Experience));
	pMsg.ExpL = SET_NUMBERL(GET_MAX_WORD_VALUE(lpRecv->Experience));
	pMsg.DamageH = SET_NUMBERH(GET_MAX_WORD_VALUE(lpRecv->DamageHP));
	pMsg.DamageL = SET_NUMBERL(GET_MAX_WORD_VALUE(lpRecv->DamageHP));
	
	pProtocolCores(Protonum, (LPBYTE)&pMsg, pMsg.h.size, key);
}
void DamageRecv(PMSG_DAMAGE*lpRecv, int key)
{
	gObjectManager->SetDamage(lpRecv->DamageHP, lpRecv->DamageSD);
	gObjectManager->SetLife(lpRecv->Life, lpRecv->Shield, 0xFF);

	PMSG_DAMAGE_PRIMARY pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xF3, sizeof(PMSG_DAMAGE_PRIMARY));
	pMsg.subcode = 0x07;
	pMsg.DamageH = SET_NUMBERH(GET_MAX_WORD_VALUE(lpRecv->DamageHP));
	pMsg.DamageL = SET_NUMBERL(GET_MAX_WORD_VALUE(lpRecv->DamageHP));
	pMsg.btShieldDamageH = SET_NUMBERH(GET_MAX_WORD_VALUE(lpRecv->DamageSD));
	pMsg.btShieldDamageL = SET_NUMBERL(GET_MAX_WORD_VALUE(lpRecv->DamageSD));
	//pProtocolCore(0xF3, (LPBYTE)&pMsg, pMsg.h.size, key);
	DataSend((BYTE*)&pMsg, pMsg.h.size);
	pProtocolCores(0xF3, (LPBYTE)&pMsg, pMsg.h.size, key);
}
//---------------------------------------------------------------------------
void GCGetVpPropensitySystem(PMSG_PROPENSITY_VIEWPORT_NOTIFY * pMsg)
{

//	  DWORD tIndex = ((pMsg->NumberH << 8) + pMsg->NumberL) & 0x7FFF;
//	  float Number;
//		
//      memcpy(&Number, &pMsg->Number, sizeof(float));
//
//	  VPPROPOBJ mp_gObj = &VpPropObj[tIndex];
//////������������
//     	mp_gObj->Number = (int)Number;
////////////

}

static unsigned char bBuxCode[3] = { 0xFC, 0xCF, 0xAB };	// Xox Key for some interesthing things :)

void BuxConvert(char* buf, int size)
{
	int n;

	for (n = 0; n<size; n++)
	{
		buf[n] ^= bBuxCode[n % 3];		// Nice trick from WebZen
	}
}
__declspec(naked) void cProtocol::UpdateCharacterNoEffect()
{
	static DWORD UpdateCharacterNoEffectBack = 0x4F5F47;

	__asm PUSHAD;

	if (Protocol.Update == true)
	{
		__asm
		{
			POPAD;
			MOV EAX, DWORD PTR DS : [0x77A8FD8];

			MOV CX, Protocol.Strength;
			MOV WORD PTR DS : [EAX + 0x18], CX;

			MOV CX, Protocol.Dexterity;
			MOV WORD PTR DS : [EAX + 0x1A], CX;

			MOV CX, Protocol.Vitality;
			MOV WORD PTR DS : [EAX + 0x1C], CX;

			MOV CX, Protocol.Energy;
			MOV WORD PTR DS : [EAX + 0x1E], CX;

			MOV CX, Protocol.Leadership;
			MOV WORD PTR DS : [EAX + 0x20], CX;

			MOV CX, Protocol.Life;
			MOV WORD PTR DS : [EAX + 0x22], CX;

			MOV CX, Protocol.MaxLife;
			MOV WORD PTR DS : [EAX + 0x26], CX;

			MOV CX, Protocol.Mana;
			MOV WORD PTR DS : [EAX + 0x24], CX;

			MOV CX, Protocol.MaxMana;
			MOV WORD PTR DS : [EAX + 0x28], CX;

			MOV CX, Protocol.BP;
			MOV WORD PTR DS : [EAX + 0x40], CX;

			MOV CX, Protocol.MaxBP;
			MOV WORD PTR DS : [EAX + 0x2C], CX;

			MOV CX, Protocol.Shield;
			MOV WORD PTR DS : [EAX + 0x42], CX;

			MOV CX, Protocol.MaxShield;
			MOV WORD PTR DS : [EAX + 0x4E], CX;

			MOV WORD PTR DS : [EAX + 0x50], 0;
			MOV WORD PTR DS : [EAX + 0x52], 0;

			MOV CX, Protocol.LevelUpPoint;
			MOV WORD PTR DS : [EAX + 0x70], CX;


			JMP UpdateCharacterNoEffectBack;
		}
	}

	__asm
	{
		POPAD;
		MOV EAX, DWORD PTR DS : [0x77A8FD8];
		JMP UpdateCharacterNoEffectBack;
	}
}

__declspec(naked) void cProtocol::UpdateCharacterEffect()
{
	static DWORD UpdateCharacterEffectBack[2] = { 0x4F5D1A, 0x4F5CC2 };

	__asm PUSHAD;

	if (Protocol.Update == true)
	{
		__asm
		{
			POPAD;
			MOV EAX, DWORD PTR DS : [0x77A8FD8];

			MOV CX, Protocol.Level;
			MOV WORD PTR DS : [EAX + 0x0E], CX;

			MOV EDX, Protocol.Experience;
			MOV DWORD PTR DS : [EAX + 0x10], EDX;

			MOV EDX, Protocol.NextExperience;
			MOV DWORD PTR DS : [EAX + 0x14], EDX;

			MOV CX, Protocol.Strength;
			MOV WORD PTR DS : [EAX + 0x18], CX;

			MOV CX, Protocol.Dexterity;
			MOV WORD PTR DS : [EAX + 0x1A], CX;

			MOV CX, Protocol.Vitality;
			MOV WORD PTR DS : [EAX + 0x1C], CX;

			MOV CX, Protocol.Energy;
			MOV WORD PTR DS : [EAX + 0x1E], CX;

			MOV CX, Protocol.Leadership;
			MOV WORD PTR DS : [EAX + 0x20], CX;

			MOV CX, Protocol.Life;
			MOV WORD PTR DS : [EAX + 0x22], CX;

			MOV CX, Protocol.MaxLife;
			MOV WORD PTR DS : [EAX + 0x26], CX;

			MOV CX, Protocol.Mana;
			MOV WORD PTR DS : [EAX + 0x24], CX;

			MOV CX, Protocol.MaxMana;
			MOV WORD PTR DS : [EAX + 0x28], CX;

			MOV CX, Protocol.BP;
			MOV WORD PTR DS : [EAX + 0x40], CX;

			MOV CX, Protocol.MaxBP;
			MOV WORD PTR DS : [EAX + 0x2C], CX;

			MOV CX, Protocol.Shield;
			MOV WORD PTR DS : [EAX + 0x42], CX;

			MOV CX, Protocol.MaxShield;
			MOV WORD PTR DS : [EAX + 0x4E], CX;

			MOV WORD PTR DS : [EAX + 0x50], 0;
			MOV WORD PTR DS : [EAX + 0x52], 0;

			MOV CX, Protocol.LevelUpPoint;
			MOV WORD PTR DS : [EAX + 0x70], CX;

			JMP UpdateCharacterEffectBack[0];
		}
	}

	__asm
	{
		POPAD;
		MOV EAX, DWORD PTR DS : [0x77A8FD8];
		JMP UpdateCharacterEffectBack[4];
	}
}

__declspec(naked) void cProtocol::LevelUpPoints()
{
	static DWORD Local = 0x662475;

	__asm
	{
		MOV EDX, Protocol.RealLevelUpPoint;
		PUSH EDX;
		JMP Local;
	}
}
bool oneLogin = true;
BYTE Key[19] = { 0x29, 0x23, 0xBE, 0x00, 0xE1, 0x6C, 0xD6,0xAE, 0x52, 0x90, 0x49, 0xF1, 0xF1, 0xBB, 0xE9, 0xEB, 0xB3, 0xA6, 0xDB };
void Encode32(BYTE *buf)
{
	for (int i = 3; i<buf[1]; i++)
		buf[i] ^= buf[i - 1] ^ Key[i % 32];
}
void PacketArgumentEncrypt(char* out_buff, char* in_buff, int size) // OK
{
	BYTE XorTable[3] = { 0xFC,0xCF,0xAB };

	for (int n = 0; n < size; n++)
	{
		out_buff[n] = in_buff[n] ^ XorTable[n % 3];
	}
}
void cProtocol::AutoLogin()
{
#if ENABLE_AUTOLOGIN ==	FALSE
	if(oneLogin == true)
	{
		return;
	}

	int Enable = GetPrivateProfileIntA("ExTeam","Enable",1,".\\AutoLogin.ini");

	if(!Enable) 
	{
		return;
	}



	oneLogin = true;
#endif

	//static bool AutoLoginStart = true;

#if ENABLE_AUTOLOGIN ==	TRUE

	char acc[11];
	char pas[11];

	ZeroMemory(&acc,sizeof(acc));
	ZeroMemory(&pas,sizeof(pas));

	strcpy(acc,LOGIN);
	strcpy(pas,PASSWORD);

#else
	char acc[11];
	char pas[11];

	ZeroMemory(&acc,sizeof(acc));
	ZeroMemory(&pas,sizeof(pas));

	GetPrivateProfileStringA("ExTeam", "Account", "1234", acc, sizeof(acc), ".\\AutoLogin.ini");
	GetPrivateProfileStringA("ExTeam", "Password", "1234", pas, sizeof(pas), ".\\AutoLogin.ini");
#endif

	PMSG_IDPASS pSend;
	ZeroMemory(&pSend,sizeof(pSend));
	pSend.h.set((LPBYTE)&pSend, 0xF1, sizeof(pSend));
	pSend.subcode = 0x01;

	BuxConvert(acc,sizeof(acc));
	BuxConvert(pas,sizeof(pas));

	memcpy(pSend.Id,acc,sizeof(pSend.Id)); 
	memcpy(pSend.Pass,pas,sizeof(pSend.Pass)); 

	pSend.TickCount = 0;

	char Seiral[17];
	char Version[6];

////#if(IN_DLL==1)

		char VersionEx[6];
		memcpy(Seiral,MAIN_SERIAL,sizeof(Seiral));
		memcpy(VersionEx,MAIN_VERSION,sizeof(VersionEx));
		for (int i = 0; i < 5; ++i )
		{
			Version[i] =  ( VersionEx[i] - (i + 1) );	
		}
////#else
		//for (int i = 0; i < 16; ++i )
		//{
		//	Seiral[i] =  *(BYTE*)(0x007D3B5C + i);	
		//}
		//for (int i = 0; i < 5; ++i )
		//{
		//	Version[i] =  ( *(BYTE*)(0x007D3B54 + i) - (i + 1) );	
		//}
//#endif

	memcpy(pSend.CliSerial,Seiral,sizeof(pSend.CliSerial));
	memcpy(pSend.CliVersion,Version,sizeof(pSend.CliVersion));

	DataSend((LPBYTE)&pSend, pSend.h.size);
}

void cProtocol::Agility()
{
	switch (Protocol.Class)
	{
	case 0:
	{
		if (Protocol.MagicSpeed >= 455 && Protocol.MagicSpeed <= 479)
		{
			*MainSpeedFrame2 = 0.0024700f;
		}
		else if (Protocol.MagicSpeed >= 605 && Protocol.MagicSpeed <= 636)
		{
			*MainSpeedFrame2 = 0.0019000f;
		}
		else if (Protocol.MagicSpeed >= 637 && Protocol.MagicSpeed <= 668)
		{
			*MainSpeedFrame2 = 0.0018000f;
		}
		else if (Protocol.MagicSpeed >= 669 && Protocol.MagicSpeed <= 688)
		{
			*MainSpeedFrame2 = 0.0017000f;
		}
		else if (Protocol.MagicSpeed >= 855 && Protocol.MagicSpeed <= 1040)
		{
			*MainSpeedFrame2 = 0.0016300f;
		}
		else if (Protocol.MagicSpeed >= 1041 && Protocol.MagicSpeed <= 1104)
		{
			*MainSpeedFrame2 = 0.0015500f;
		}
		else if (Protocol.MagicSpeed >= 1301 && Protocol.MagicSpeed <= 1500)
		{
			*MainSpeedFrame2 = 0.0017500f;
		}
		else if (Protocol.MagicSpeed >= 1501 && Protocol.MagicSpeed <= 1524)
		{
			*MainSpeedFrame2 = 0.0015000f;
		}
		else if (Protocol.MagicSpeed >= 1525 && Protocol.MagicSpeed <= 1800)
		{
			*MainSpeedFrame2 = 0.0014500f;
		}
		else if (Protocol.MagicSpeed >= 1801 && Protocol.MagicSpeed <= 1999)
		{
			*MainSpeedFrame2 = 0.0013000f;
		}
		else if (Protocol.MagicSpeed >= 2000 && Protocol.MagicSpeed <= 2167)
		{
			*MainSpeedFrame2 = 0.0012500f;
		}
		else if (Protocol.MagicSpeed >= 2168 && Protocol.MagicSpeed <= 2354)
		{
			*MainSpeedFrame2 = 0.0011500f;
		}
		else if (Protocol.MagicSpeed >= 2855 && Protocol.MagicSpeed <= 3011)
		{
			*MainSpeedFrame2 = 0.0009000f;
		}
		else if (Protocol.MagicSpeed >= 3011 && Protocol.MagicSpeed <= 3340)
		{
			*MainSpeedFrame2 = 0.0008100f;
		}
		else if (Protocol.MagicSpeed >= 3341 && Protocol.MagicSpeed <= 3380)
		{
			*MainSpeedFrame2 = 0.0008000f;
		}
		else if (Protocol.MagicSpeed >= 3381 && Protocol.MagicSpeed <= 3610)
		{
			*MainSpeedFrame2 = 0.0007500f;
		}
		else if (Protocol.MagicSpeed >= 3611 && Protocol.MagicSpeed <= 3860)
		{
			*MainSpeedFrame2 = 0.0007000f;
		}
		else if (Protocol.MagicSpeed >= 3861 && Protocol.MagicSpeed <= 4160)
		{
			*MainSpeedFrame2 = 0.0006500f;
		}
		else if (Protocol.MagicSpeed >= 4161 && Protocol.MagicSpeed <= 4480)
		{
			*MainSpeedFrame2 = 0.0006000f;
		}
		else if (Protocol.MagicSpeed >= 4481 && Protocol.MagicSpeed <= 4920)
		{
			*MainSpeedFrame2 = 0.0005500f;
		}
		else if (Protocol.MagicSpeed >= 4921 && Protocol.MagicSpeed <= 5410)
		{
			*MainSpeedFrame2 = 0.0005000f;
		}
		else if (Protocol.MagicSpeed >= 5411 && Protocol.MagicSpeed <= 6020)
		{
			*MainSpeedFrame2 = 0.0004500f;
		}
		else if (Protocol.MagicSpeed >= 6021)
		{
			*MainSpeedFrame2 = 0.0004000f;
		}
		else
		{
			*MainSpeedFrame2 = 0.0020000f;
		}
	}
	break;
	case 1:
	{
		if (Protocol.Dexterity >= 11001 && Protocol.Dexterity <= 12000)
		{
			*MainSpeedFrame1 = 0.001800000f;
		}
		else if (Protocol.Dexterity >= 16000 && Protocol.Dexterity <= 17000)
		{
			*MainSpeedFrame1 = 0.001800000f;
		}
		else if (Protocol.Dexterity >= 17001 && Protocol.Dexterity <= 19000)
		{
			*MainSpeedFrame1 = 0.001700000f;
		}
		else if (Protocol.Dexterity >= 19001 && Protocol.Dexterity <= 20000)
		{
			*MainSpeedFrame1 = 0.001600000f;
		}
		else if (Protocol.Dexterity >= 20001 && Protocol.Dexterity <= 21000)
		{
			*MainSpeedFrame1 = 0.001500000f;
		}
		else if (Protocol.Dexterity >= 21001 && Protocol.Dexterity <= 23000)
		{
			*MainSpeedFrame1 = 0.001400000f;
		}
		else if (Protocol.Dexterity >= 26000 && Protocol.Dexterity <= 27000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 27001 && Protocol.Dexterity <= 29000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 29001 && Protocol.Dexterity <= 30000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 30001 && Protocol.Dexterity <= 32767)
		{
			*MainSpeedFrame1 = 0.001600000f;
		}
		else
		{
			*MainSpeedFrame1 = 0.004000000f;
		}
	}
	break;
	case 2:
	{
		if (Protocol.AttackSpeed >= 509 && Protocol.AttackSpeed <= 549)
		{
			*MainSpeedFrame1 = 0.0026000f;
		}
		else if (Protocol.AttackSpeed >= 550 && Protocol.AttackSpeed <= 750)
		{
			*MainSpeedFrame1 = 0.0017000f;
		}
		else
		{
			*MainSpeedFrame1 = 0.0040000f;
		}
	}
	break;
	case 3:
	{
		if (Protocol.Dexterity >= 11001 && Protocol.Dexterity <= 12000)
		{
			*MainSpeedFrame1 = 0.001800000f;
		}
		else if (Protocol.Dexterity >= 16000 && Protocol.Dexterity <= 17000)
		{
			*MainSpeedFrame1 = 0.001800000f;
		}
		else if (Protocol.Dexterity >= 17001 && Protocol.Dexterity <= 19000)
		{
			*MainSpeedFrame1 = 0.001700000f;
		}
		else if (Protocol.Dexterity >= 19001 && Protocol.Dexterity <= 20000)
		{
			*MainSpeedFrame1 = 0.001600000f;
		}
		else if (Protocol.Dexterity >= 20001 && Protocol.Dexterity <= 21000)
		{
			*MainSpeedFrame1 = 0.001500000f;
		}
		else if (Protocol.Dexterity >= 21001 && Protocol.Dexterity <= 23000)
		{
			*MainSpeedFrame1 = 0.001400000f;
		}
		else if (Protocol.Dexterity >= 26000 && Protocol.Dexterity <= 27000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 27001 && Protocol.Dexterity <= 29000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 29001 && Protocol.Dexterity <= 30000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 30001 && Protocol.Dexterity <= 32767)
		{
			*MainSpeedFrame1 = 0.001600000f;
		}
		else
		{
			*MainSpeedFrame1 = 0.004000000f;
		}

		if (Protocol.MagicSpeed >= 455 && Protocol.MagicSpeed <= 479)
		{
			*MainSpeedFrame2 = 0.0024700f;
		}
		else if (Protocol.MagicSpeed >= 605 && Protocol.MagicSpeed <= 636)
		{
			*MainSpeedFrame2 = 0.0019000f;
		}
		else if (Protocol.MagicSpeed >= 637 && Protocol.MagicSpeed <= 668)
		{
			*MainSpeedFrame2 = 0.0018000f;
		}
		else if (Protocol.MagicSpeed >= 669 && Protocol.MagicSpeed <= 688)
		{
			*MainSpeedFrame2 = 0.0017000f;
		}
		else if (Protocol.MagicSpeed >= 855 && Protocol.MagicSpeed <= 1040)
		{
			*MainSpeedFrame2 = 0.0016300f;
		}
		else if (Protocol.MagicSpeed >= 1041 && Protocol.MagicSpeed <= 1104)
		{
			*MainSpeedFrame2 = 0.0015500f;
		}
		else if (Protocol.MagicSpeed >= 1301 && Protocol.MagicSpeed <= 1500)
		{
			*MainSpeedFrame2 = 0.0017500f;
		}
		else if (Protocol.MagicSpeed >= 1501 && Protocol.MagicSpeed <= 1524)
		{
			*MainSpeedFrame2 = 0.0015000f;
		}
		else if (Protocol.MagicSpeed >= 1525 && Protocol.MagicSpeed <= 1800)
		{
			*MainSpeedFrame2 = 0.0014500f;
		}
		else if (Protocol.MagicSpeed >= 1801 && Protocol.MagicSpeed <= 1999)
		{
			*MainSpeedFrame2 = 0.0013000f;
		}
		else if (Protocol.MagicSpeed >= 2000 && Protocol.MagicSpeed <= 2167)
		{
			*MainSpeedFrame2 = 0.0012500f;
		}
		else if (Protocol.MagicSpeed >= 2168 && Protocol.MagicSpeed <= 2354)
		{
			*MainSpeedFrame2 = 0.0011500f;
		}
		else if (Protocol.MagicSpeed >= 2855 && Protocol.MagicSpeed <= 3011)
		{
			*MainSpeedFrame2 = 0.0009000f;
		}
		else if (Protocol.MagicSpeed >= 3011 && Protocol.MagicSpeed <= 3340)
		{
			*MainSpeedFrame2 = 0.0008100f;
		}
		else if (Protocol.MagicSpeed >= 3341 && Protocol.MagicSpeed <= 3380)
		{
			*MainSpeedFrame2 = 0.0008000f;
		}
		else if (Protocol.MagicSpeed >= 3381 && Protocol.MagicSpeed <= 3610)
		{
			*MainSpeedFrame2 = 0.0007500f;
		}
		else if (Protocol.MagicSpeed >= 3611 && Protocol.MagicSpeed <= 3860)
		{
			*MainSpeedFrame2 = 0.0007000f;
		}
		else if (Protocol.MagicSpeed >= 3861 && Protocol.MagicSpeed <= 4160)
		{
			*MainSpeedFrame2 = 0.0006500f;
		}
		else if (Protocol.MagicSpeed >= 4161 && Protocol.MagicSpeed <= 4480)
		{
			*MainSpeedFrame2 = 0.0006000f;
		}
		else if (Protocol.MagicSpeed >= 4481 && Protocol.MagicSpeed <= 4920)
		{
			*MainSpeedFrame2 = 0.0005500f;
		}
		else if (Protocol.MagicSpeed >= 4921 && Protocol.MagicSpeed <= 5410)
		{
			*MainSpeedFrame2 = 0.0005000f;
		}
		else if (Protocol.MagicSpeed >= 5411 && Protocol.MagicSpeed <= 6020)
		{
			*MainSpeedFrame2 = 0.0004500f;
		}
		else if (Protocol.MagicSpeed >= 6021)
		{
			*MainSpeedFrame2 = 0.0004000f;
		}
		else
		{
			*MainSpeedFrame2 = 0.0020000f;
		}
	}
	break;
	case 4:
	{
		/*if (Protocol.Dexterity >= 2000 && Protocol.Dexterity < 4900)
		{
			*MainSpeedFrame1 = 0.0038000f;
		}
		else if (Protocol.Dexterity >= 4900 && Protocol.Dexterity < 5050)
		{
			*MainSpeedFrame1 = 0.0037000f;
		}
		else if (Protocol.Dexterity >= 5050 && Protocol.Dexterity < 5200)
		{
			*MainSpeedFrame1 = 0.0036000f;
		}
		else if (Protocol.Dexterity >= 5200 && Protocol.Dexterity < 5350)
		{
			*MainSpeedFrame1 = 0.0035000f;
		}
		else if (Protocol.Dexterity >= 5350 && Protocol.Dexterity < 5550)
		{
			*MainSpeedFrame1 = 0.0034000f;
		}
		else if (Protocol.Dexterity >= 5550 && Protocol.Dexterity < 5700)
		{
			*MainSpeedFrame1 = 0.0033000f;
		}
		else if (Protocol.Dexterity >= 5700 && Protocol.Dexterity < 5900)
		{
			*MainSpeedFrame1 = 0.0032000f;
		}
		else if (Protocol.Dexterity >= 5900 && Protocol.Dexterity < 6100)
		{
			*MainSpeedFrame1 = 0.0031000f;
		}
		else if (Protocol.Dexterity >= 6100 && Protocol.Dexterity < 6300)
		{
			*MainSpeedFrame1 = 0.0030000f;
		}
		else if (Protocol.Dexterity >= 6300 && Protocol.Dexterity < 6500)
		{
			*MainSpeedFrame1 = 0.0029000f;
		}
		else if (Protocol.Dexterity >= 6500 && Protocol.Dexterity < 6750)
		{
			*MainSpeedFrame1 = 0.0028000f;
		}
		else if (Protocol.Dexterity >= 6750 && Protocol.Dexterity < 7000)
		{
			*MainSpeedFrame1 = 0.0027000f;
		}
		else if (Protocol.Dexterity >= 7000 && Protocol.Dexterity < 7300)
		{
			*MainSpeedFrame1 = 0.0026000f;
		}
		else if (Protocol.Dexterity >= 7300 && Protocol.Dexterity < 7600)
		{
			*MainSpeedFrame1 = 0.0025000f;
		}
		else if (Protocol.Dexterity >= 7600 && Protocol.Dexterity < 7900)
		{
			*MainSpeedFrame1 = 0.0024000f;
		}
		else if (Protocol.Dexterity >= 7900 && Protocol.Dexterity < 8250)
		{
			*MainSpeedFrame1 = 0.0023000f;
		}
		else if (Protocol.Dexterity >= 8250 && Protocol.Dexterity < 8650)
		{
			*MainSpeedFrame1 = 0.0022000f;
		}
		else if (Protocol.Dexterity >= 8650 && Protocol.Dexterity < 9050)
		{
			*MainSpeedFrame1 = 0.0021000f;
		}
		else if (Protocol.Dexterity >= 9050 && Protocol.Dexterity < 9500)
		{
			*MainSpeedFrame1 = 0.0020000f;
		}
		else if (Protocol.Dexterity >= 9500 && Protocol.Dexterity < 10000)
		{
			*MainSpeedFrame1 = 0.0019000f;
		}
		else if (Protocol.Dexterity >= 10000 && Protocol.Dexterity < 10550)
		{
			*MainSpeedFrame1 = 0.0018000f;
		}
		else if (Protocol.Dexterity >= 10550 && Protocol.Dexterity < 11150)
		{
			*MainSpeedFrame1 = 0.0017000f;
		}
		else if (Protocol.Dexterity >= 11150 && Protocol.Dexterity < 11850)
		{
			*MainSpeedFrame1 = 0.0016000f;
		}
		else if (Protocol.Dexterity >= 11850 && Protocol.Dexterity < 12600)
		{
			*MainSpeedFrame1 = 0.0015000f;
		}
		else if (Protocol.Dexterity >= 12600 && Protocol.Dexterity < 13450)
		{
			*MainSpeedFrame1 = 0.0014000f;
		}
		else if (Protocol.Dexterity >= 13450 && Protocol.Dexterity < 16600)
		{
			*MainSpeedFrame1 = 0.0010000f;
		}
		else if (Protocol.Dexterity >= 16600)
		{
			*MainSpeedFrame1 = 0.0002000f;
		}
		else
		{
			*MainSpeedFrame1 = 0.0040000f;
		}*/
if (Protocol.Dexterity >= 11001 && Protocol.Dexterity <= 12000)
		{
			*MainSpeedFrame1 = 0.001800000f;
		}
		else if (Protocol.Dexterity >= 16000 && Protocol.Dexterity <= 17000)
		{
			*MainSpeedFrame1 = 0.001800000f;
		}
		else if (Protocol.Dexterity >= 17001 && Protocol.Dexterity <= 19000)
		{
			*MainSpeedFrame1 = 0.001700000f;
		}
		else if (Protocol.Dexterity >= 19001 && Protocol.Dexterity <= 20000)
		{
			*MainSpeedFrame1 = 0.001600000f;
		}
		else if (Protocol.Dexterity >= 20001 && Protocol.Dexterity <= 21000)
		{
			*MainSpeedFrame1 = 0.001500000f;
		}
		else if (Protocol.Dexterity >= 21001 && Protocol.Dexterity <= 23000)
		{
			*MainSpeedFrame1 = 0.001400000f;
		}
		else if (Protocol.Dexterity >= 26000 && Protocol.Dexterity <= 27000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 27001 && Protocol.Dexterity <= 29000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 29001 && Protocol.Dexterity <= 30000)
		{
			*MainSpeedFrame1 = 0.001900000f;
		}
		else if (Protocol.Dexterity >= 30001 && Protocol.Dexterity <= 32767)
		{
			*MainSpeedFrame1 = 0.001600000f;
		}
		else
		{
			*MainSpeedFrame1 = 0.004000000f;
		}

		if (Protocol.MagicSpeed >= 455 && Protocol.MagicSpeed <= 479)
		{
			*MainSpeedFrame2 = 0.0024700f;
		}
		else if (Protocol.MagicSpeed >= 605 && Protocol.MagicSpeed <= 636)
		{
			*MainSpeedFrame2 = 0.0019000f;
		}
		else if (Protocol.MagicSpeed >= 637 && Protocol.MagicSpeed <= 668)
		{
			*MainSpeedFrame2 = 0.0018000f;
		}
		else if (Protocol.MagicSpeed >= 669 && Protocol.MagicSpeed <= 688)
		{
			*MainSpeedFrame2 = 0.0017000f;
		}
		else if (Protocol.MagicSpeed >= 855 && Protocol.MagicSpeed <= 1040)
		{
			*MainSpeedFrame2 = 0.0016300f;
		}
		else if (Protocol.MagicSpeed >= 1041 && Protocol.MagicSpeed <= 1104)
		{
			*MainSpeedFrame2 = 0.0015500f;
		}
		else if (Protocol.MagicSpeed >= 1301 && Protocol.MagicSpeed <= 1500)
		{
			*MainSpeedFrame2 = 0.0017500f;
		}
		else if (Protocol.MagicSpeed >= 1501 && Protocol.MagicSpeed <= 1524)
		{
			*MainSpeedFrame2 = 0.0015000f;
		}
		else if (Protocol.MagicSpeed >= 1525 && Protocol.MagicSpeed <= 1800)
		{
			*MainSpeedFrame2 = 0.0014500f;
		}
		else if (Protocol.MagicSpeed >= 1801 && Protocol.MagicSpeed <= 1999)
		{
			*MainSpeedFrame2 = 0.0013000f;
		}
		else if (Protocol.MagicSpeed >= 2000 && Protocol.MagicSpeed <= 2167)
		{
			*MainSpeedFrame2 = 0.0012500f;
		}
		else if (Protocol.MagicSpeed >= 2168 && Protocol.MagicSpeed <= 2354)
		{
			*MainSpeedFrame2 = 0.0011500f;
		}
		else if (Protocol.MagicSpeed >= 2855 && Protocol.MagicSpeed <= 3011)
		{
			*MainSpeedFrame2 = 0.0009000f;
		}
		else if (Protocol.MagicSpeed >= 3011 && Protocol.MagicSpeed <= 3340)
		{
			*MainSpeedFrame2 = 0.0008100f;
		}
		else if (Protocol.MagicSpeed >= 3341 && Protocol.MagicSpeed <= 3380)
		{
			*MainSpeedFrame2 = 0.0008000f;
		}
		else if (Protocol.MagicSpeed >= 3381 && Protocol.MagicSpeed <= 3610)
		{
			*MainSpeedFrame2 = 0.0007500f;
		}
		else if (Protocol.MagicSpeed >= 3611 && Protocol.MagicSpeed <= 3860)
		{
			*MainSpeedFrame2 = 0.0007000f;
		}
		else if (Protocol.MagicSpeed >= 3861 && Protocol.MagicSpeed <= 4160)
		{
			*MainSpeedFrame2 = 0.0006500f;
		}
		else if (Protocol.MagicSpeed >= 4161 && Protocol.MagicSpeed <= 4480)
		{
			*MainSpeedFrame2 = 0.0006000f;
		}
		else if (Protocol.MagicSpeed >= 4481 && Protocol.MagicSpeed <= 4920)
		{
			*MainSpeedFrame2 = 0.0005500f;
		}
		else if (Protocol.MagicSpeed >= 4921 && Protocol.MagicSpeed <= 5410)
		{
			*MainSpeedFrame2 = 0.0005000f;
		}
		else if (Protocol.MagicSpeed >= 5411 && Protocol.MagicSpeed <= 6020)
		{
			*MainSpeedFrame2 = 0.0004500f;
		}
		else if (Protocol.MagicSpeed >= 6021)
		{
			*MainSpeedFrame2 = 0.0004000f;
		}
		else
		{
			*MainSpeedFrame2 = 0.0020000f;
		}
	}
	break;
	}
}

//===============================================================
//		Attack HP
//===============================================================
DWORD DMG_FUNCTION = 0x005931D0;
DWORD RETURN_DMG_JMP = 0x004ED932;
__declspec(naked) void FixVisualDamageHP()
{
	_asm
	{
		PUSH 1
		MOV EAX,ESP//EAX
		ADD EAX,0x38//EAX
		PUSH EDI
		AND EBP,0x0000FFFF
		PUSH EAX
		MOV ESI,EBX
		ADD ESI,0x10//0x10
		PUSH  DAMAGE	//EBP
		PUSH ESI                                 ; |Arg1
		CALL DMG_FUNCTION                        ; \Main.005931D0

		JMP RETURN_DMG_JMP
	}
}
//#define pDrawDamage				((void(__cdecl*)(int a1, int a2, int a3, float a4, char a5, char a6)) 0x005931D0)
//void PrintDamageHPOnScreen(int a1, int a2, int a3, float a4, char a5, char a6)
//{
//	//if (gObjectManager->GetDamageHP() > 60000)
//	//{
//	//	a2 = gObjectManager->GetDamageHP();
//	//}
//	//if (a2 > 0)
//	//{
//	//	a2 =  Visual.AttackHP;
//	//}
//
//	//return ((int(__cdecl*)(int, int, int, int, int)) 0x005931D0)(a1, a2, a3, a4, a5);
//	pDrawDamage(a1, gObjUser.m_SecondDamage, a3, a4, a5, a6);
//}
//
//
//void PrintDamageSDOnScreen(int a1, int a2, int a3, float a4, char a5, char a6)
//{
//	//if (gObjectManager->GetDamageHP() > 60000)
//	//{
//	//	a2 = gObjectManager->GetDamageHP();
//	//}
//	//if (a2 > 0)
//	//{
//	//	a2 =  Visual.AttackHP;
//	//}
//
//	//return ((int(__cdecl*)(int, int, int, int, int)) 0x005931D0)(a1, a2, a3, a4, a5);
//	pDrawDamage(a1, gObjUser.m_SecondDamage, a3, a4, a5, a6);
//}

int PrintDamageHPOnScreen(int a1, int a2, int a3, int a4, int a5)
{
	if (gObjectManager->GetDamageHP() > 60000)
	{
		a2 = gObjectManager->GetDamageHP();
	}


	return ((int(__cdecl*)(int, int, int, int, int)) 0x005931D0)(a1, a2, a3, a4, a5);
}

int PrintDamageSDOnScreen(int a1, int a2, int a3, int a4, int a5)
{
	if (gObjectManager->GetDamageSD() > 60000)
	{
		a2 = gObjectManager->GetDamageSD();
	}

	return ((int(__cdecl*)(int, int, int, int, int)) 0x005931D0)(a1, a2, a3, a4, a5);
}

//===============================================================
//		Attack SD
//===============================================================
DWORD RETURN_SD_JMP = 0x004ED98C;
__declspec(naked) void FixVisualDamageSD()
{
	_asm
	{
		PUSH 1                                   ; /Arg5 = 1
		MOV ECX,ESP
		ADD ECX,0x38
		MOV DWORD PTR SS:[ESP+0x54],EDX            ; |//MOV DWORD PTR SS:[ESP+0x54],EDX
		PUSH 0x41700000                          ; |Arg4 = 41700000
		AND EAX,0x0000FFFF                         ; |
		PUSH ECX                                 ; |Arg3 => OFFSET LOCAL.8
		FSTP DWORD PTR SS:[ESP+0x60]                ; |//FSTP DWORD PTR SS:[ESP+0x60] 
		MOV EDX,ESP
		ADD EDX,0x58//ADD EDX,0x58
		PUSH  SHIELD//EAX                                 ; |Arg2
		PUSH EDX                                 ; |Arg1 => OFFSET LOCAL.2
		CALL DMG_FUNCTION                            ; \Main.006BA450//; \Main.005931D0
		JMP RETURN_SD_JMP
	}
}

void FixDMG()
{
	//WriteJmp(0x004ED91A, (DWORD)FixVisualDamageHP);//
	//WriteJmp(0x004ED968, (DWORD)FixVisualDamageSD);//
	//SetCompleteHook(0xE8,0x0067DF10,&PrintDamageHPOnScreen);

	//SetCompleteHook(0xE8,0x004EC6E7, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004ECAAC, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004ECBBA, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004ECC11, &PrintDamageSDOnScreen);//SD
	//SetCompleteHook(0xE8,0x004ED987, &PrintDamageSDOnScreen);//SD
	//SetCompleteHook(0xE8,0x004ED7C5, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004ED820, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004ED8B7, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004ED912, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004ED92D, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004F1B69, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004F24EC, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004F2522, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004F300E, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x004F3044, &PrintDamageHPOnScreen);
	//SetCompleteHook(0xE8,0x00513351, &PrintDamageHPOnScreen);


	SetOp((LPVOID)0x004EC6E7, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004ECAAC, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004ECBBA, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004ECC11, (LPVOID)PrintDamageSDOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004ED7C5, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004ED820, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004ED8B7, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004ED912, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004ED92D, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004F1B69, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004F24EC, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004F2522, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004F300E, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x004F3044, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);
	SetOp((LPVOID)0x00513351, (LPVOID)PrintDamageHPOnScreen, ASM::CALL);

	SetOp((LPVOID)0x004ED987, (LPVOID)PrintDamageSDOnScreen, ASM::CALL);

}
//===============================================================
//		Attack Speed
//===============================================================
static DWORD ATTACK_RET = 0x0051F143;//0x0051F154
static WORD CLASS = 0;
static WORD STR_SPEED = 0;
static WORD MAG_SPEED = 0;
__declspec(naked) void FixAttackSpeed()
{
	_asm
	{
		MOV EAX,DWORD PTR DS:[0x77A8FD8]//

		MOV CX,WORD PTR DS:[EAX+0xB]
		MOV CLASS,CX

		MOV CX,WORD PTR DS:[EAX+0x54]
		MOV STR_SPEED,CX

		MOV CX,WORD PTR DS:[EAX+0x60]
		MOV MAG_SPEED,CX
	}
	
	if( CLASS == DarkWizard || CLASS == SoulMaster || CLASS == GrandMaster )
	{
		if( MAGIC == SKILL_EVIL ||
			MAGIC == 445 ||
			MAGIC == 446 ||
			MAGIC == 447 ||
			MAGIC == 448 ||
			MAGIC == 449 ||
			MAGIC == SKILL_FLAME)
		{
			if(MAG_SPEED > 450 && MAG_SPEED < 480)
			{
				MAG_SPEED = 450;
			}
			else if(MAG_SPEED > 600 && MAG_SPEED < 690)
			{
				MAG_SPEED = 600;
			}
			else if(MAG_SPEED > 850 && MAG_SPEED < 1105)
			{
				MAG_SPEED = 850;
			}
			else if(MAG_SPEED > 1350 && MAG_SPEED < 2355)
			{
				MAG_SPEED = 1350;
			}
			else if(MAG_SPEED > 2850)
			{
				MAG_SPEED = 2850;
			}
		}
		else if(MAGIC == SKILL_INFERNO)
		{
			if(MAG_SPEED > 3276)
			{
				MAG_SPEED = 3276;
			}
		}
	}
	else if( CLASS == DarkKnight  || CLASS == BladeKnight || CLASS == BladeMaster  )
	{

	}
	else if( CLASS == Elf || CLASS == MuseElf || CLASS == HightElf )
	{
		if(MAGIC == SKILL_CROSSBOW ||
		   MAGIC == 490 ||
		   MAGIC == 491 ||
		   MAGIC == 492 ||
		   MAGIC == 493 ||
		   MAGIC == 494 )
		{
			if(STR_SPEED > 508 && STR_SPEED < 550)
			{
				STR_SPEED = 508;
			}
			else if (STR_SPEED > 799 && STR_SPEED < 1150)
			{
				STR_SPEED = 799;
			}
		}
	}
	else 
	if( CLASS == MagicGladiator || CLASS == DuelMaster )
	{
		if( MAGIC == 55 ||
			//SKILL_EVIL ||
			//MAGIC == 530 ||
			//MAGIC == 531 ||
			//MAGIC == 532 ||
			//MAGIC == 533 ||
			//MAGIC == 534 || 
			MAGIC == 56 )//||
		
		{
			if(STR_SPEED > 1368)
			{
				STR_SPEED = 1368;
			}
		}
		if(MAGIC == SKILL_EVIL ||
			MAGIC == 8 ||
			MAGIC == 13||
			MAGIC == 5)
		{
			if(MAG_SPEED > 450 && MAG_SPEED < 480)
			{
				MAG_SPEED = 450;
			}
			else if(MAG_SPEED > 600 && MAG_SPEED < 690)
			{
				MAG_SPEED = 600;
			}
			else if(MAG_SPEED > 850 && MAG_SPEED < 1105)
			{
				MAG_SPEED = 850;
			}
			else if(MAG_SPEED > 1350 && MAG_SPEED < 2355)
			{
				MAG_SPEED = 1350;
			}
			else if(MAG_SPEED > 2850)
			{
				MAG_SPEED = 2850;
			}
		}
		if (MAGIC == 14 )
			{
			if(MAG_SPEED > 3276)
			{
				MAG_SPEED = 3276;
			}
		}
		if (MAGIC == 56  || MAGIC == 55 )
			{
			if(STR_SPEED > 1368 && STR_SPEED < 1368)
			{
				STR_SPEED = 1368;
			}
		}
			//MAGIC == SKILL_FLAME )
		//{
		//	if(MAG_SPEED > 450 && MAG_SPEED < 480)
		//	{
		//		MAG_SPEED = 450;
		//	}
		//	else if(MAG_SPEED > 600 && MAG_SPEED < 690)
		//	{
		//		MAG_SPEED = 600;
		//	}
		//	else if(MAG_SPEED > 850 && MAG_SPEED < 1105)
		//	{
		//		MAG_SPEED = 850;
		//	}
		//	else if(MAG_SPEED > 1350 && MAG_SPEED < 2355)
		//	{
		//		MAG_SPEED = 1350;
		//	}
		//	else if(MAG_SPEED > 2850)
		//	{
		//		MAG_SPEED = 1850;
		//	}
		//	else if(STR_SPEED > 1350 && STR_SPEED < 2850)
		//	{
		//		STR_SPEED = 508;
		//	}
		//}
	/*	else if(MAGIC == SKILL_INFERNO || MAGIC == 56 )
		{
			if(MAG_SPEED > 3276)
			{
				MAG_SPEED = 1276;
			}
		}*/
	}
	else if( CLASS == DarkLord || CLASS == LordEmperor)
	{
		if(MAGIC == SKILL_FIRESCREAM ||
		   MAGIC == 525 ||
		   MAGIC == 526 ||
		   MAGIC == 527 ||
		   MAGIC == 528 ||
		   MAGIC == 529)
		{
			if(STR_SPEED > 249 && STR_SPEED < 264)
			{
				STR_SPEED = 249;
			}
			else if(STR_SPEED > 324 && STR_SPEED < 367)
			{
				STR_SPEED = 324;
			}
			else if(STR_SPEED > 449 && STR_SPEED < 575)
			{
				STR_SPEED = 449;
			}
			else if(STR_SPEED > 699 && STR_SPEED < 1200)
			{
				STR_SPEED = 699;
			}
			else if(STR_SPEED > 1449)
			{
				STR_SPEED = 1449;
			}
		}
	}
	//1.02.19
	_asm
	{

		MOV DX,WORD PTR DS:[STR_SPEED]
		MOV DWORD PTR SS:[EBP-0x14],EDX
		FILD DWORD PTR SS:[EBP-0x14]
		FMUL DWORD PTR DS:[0x7B3A4C]
		FSTP DWORD PTR SS:[EBP-0x8] //1
		MOV CX,WORD PTR DS:[MAG_SPEED]
		MOV DWORD PTR SS:[EBP-0x18],ECX
		FILD DWORD PTR SS:[EBP-0x18]
		FMUL DWORD PTR DS:[0x7B3A4C]
		FSTP DWORD PTR SS:[EBP-0x0C]//2
		MOV AX,WORD PTR DS:[MAG_SPEED]
		MOV DWORD PTR SS:[EBP-0x1C],EAX
		FILD DWORD PTR SS:[EBP-0x1C]
		FMUL DWORD PTR DS:[0x7B3A20]
		FSTP DWORD PTR SS:[EBP-0x4]
		JMP ATTACK_RET
		/* MOV ECX,DWORD PTR DS:[0x77A8FD8]
		 XOR EDX,EDX
		 MOV DX,WORD PTR DS:[STR_SPEED]
		 MOV DWORD PTR SS:[EBP-0x14],EDX
		 FILD DWORD PTR SS:[EBP-0x14]
		 FMUL DWORD PTR DS:[0x7B3A4C]               ; FLOAT 0.004000000
		 FSTP DWORD PTR SS:[EBP-0x8]
		 MOV EAX,DWORD PTR DS:[0x77A8FD8]
		 XOR ECX,ECX
		 MOV CX,WORD PTR DS:[MAG_SPEED]
		 MOV DWORD PTR SS:[EBP-0x18],ECX
		 FILD DWORD PTR SS:[EBP-0x18]
		 FMUL DWORD PTR DS:[0x7B3A4C]               ; FLOAT 0.004000000
		 FSTP DWORD PTR SS:[EBP-0x0C]
		 MOV EDX,DWORD PTR DS:[0x77A8FD8]
		 XOR EAX,EAX
		 MOV AX,WORD PTR DS:[MAG_SPEED]
		 MOV DWORD PTR SS:[EBP-0x1C],EAX
		 FILD DWORD PTR SS:[EBP-0x1C]
		 FMUL DWORD PTR DS:[0x7B3A20]               ; FLOAT 0.002000000
		 FSTP DWORD PTR SS:[EBP-0x4]
		 JMP ATTACK_RET*/
	}
	/*_asm
	{
		MOV EAX,DWORD PTR DS:[0x7AB351C]
		XOR ECX,ECX
		MOV CX,WORD PTR DS:[STR_SPEED]
		MOV DWORD PTR SS:[EBP-0x14],ECX
		FILD DWORD PTR SS:[EBP-0x14]
		FMUL DWORD PTR DS:[0x88B700]               ; FLOAT 0.004000000
		FSTP DWORD PTR SS:[EBP-0x8]
		MOV EDX,DWORD PTR DS:[0x7AB351C]
		XOR EAX,EAX
		MOV AX,WORD PTR DS:[MAG_SPEED]
		MOV DWORD PTR SS:[EBP-0x18],EAX
		FILD DWORD PTR SS:[EBP-0x18]
		FMUL DWORD PTR DS:[0x88B700]               ; FLOAT 0.004000000
		FSTP DWORD PTR SS:[EBP-0x0C]
		MOV ECX,DWORD PTR DS:[0x7AB351C]
		XOR EDX,EDX
		MOV DX,WORD PTR DS:[MAG_SPEED]
		MOV DWORD PTR SS:[EBP-0x1C],EDX
		FILD DWORD PTR SS:[EBP-0x1C]
		FMUL DWORD PTR DS:[0x88B494]               ; FLOAT 0.002000000
		FSTP DWORD PTR SS:[EBP-0x4]
		JMP ATTACK_RET*/
			//1.03k
//004D5E05  |.  A1 1C35AB07   MOV EAX,DWORD PTR DS:[7AB351C]
//004D5E0A  |.  33C9          XOR ECX,ECX
//004D5E0C  |.  66:8B48 54    MOV CX,WORD PTR DS:[EAX+54]
//004D5E10  |.  894D EC       MOV DWORD PTR SS:[LOCAL.5],ECX
//004D5E13  |.  DB45 EC       FILD DWORD PTR SS:[LOCAL.5]
//004D5E16  |.  D80D 00B78800 FMUL DWORD PTR DS:[88B700]               ; FLOAT 0.004000000
//004D5E1C  |.  D95D F8       FSTP DWORD PTR SS:[LOCAL.2]
//004D5E1F  |.  8B15 1C35AB07 MOV EDX,DWORD PTR DS:[7AB351C]
//004D5E25  |.  33C0          XOR EAX,EAX
//004D5E27  |.  66:8B42 60    MOV AX,WORD PTR DS:[EDX+60]
//004D5E2B  |.  8945 E8       MOV DWORD PTR SS:[LOCAL.6],EAX
//004D5E2E  |.  DB45 E8       FILD DWORD PTR SS:[LOCAL.6]
//004D5E31  |.  D80D 00B78800 FMUL DWORD PTR DS:[88B700]               ; FLOAT 0.004000000
//004D5E37  |.  D95D F4       FSTP DWORD PTR SS:[LOCAL.3]
//004D5E3A  |.  8B0D 1C35AB07 MOV ECX,DWORD PTR DS:[7AB351C]
//004D5E40  |.  33D2          XOR EDX,EDX
//004D5E42  |.  66:8B51 60    MOV DX,WORD PTR DS:[ECX+60]
//004D5E46  |.  8955 E4       MOV DWORD PTR SS:[LOCAL.7],EDX
//004D5E49  |.  DB45 E4       FILD DWORD PTR SS:[LOCAL.7]
//004D5E4C  |.  D80D 94B48800 FMUL DWORD PTR DS:[88B494]               ; FLOAT 0.002000000
//004D5E52  |.  D95D FC       FSTP DWORD PTR SS:[LOCAL.1]

	//}
}

void SpeedFix()
{
	///SetNop(0x0051F0FB,80);//
	//WriteJmp(0x0051F0FB, (DWORD)&FixAttackSpeed);
	//SetOp((LPVOID)0x0051F0FB, (LPVOID)FixAttackSpeed, ASM::JMP);//51F0FB
}

#if(DEV_DAMAGE_TABLE)

void RecvDamageTable(PMSG_DAMAGE_TABLE* lpMsg)
{
	//if(!gAdvanceStats)
	//{
	//	return;
	//}

	gObjUser.m_SecondDamage = lpMsg->SecondDamage;
	gObjUser.m_SecondDefence = lpMsg->SecondDefence;
	gObjUser.m_SecondReflect = lpMsg->SecondReflect;

	if(gObjUser.m_SecondDamage > gObjUser.m_SecondDamageMax)
	{
		gObjUser.m_SecondDamageMax = gObjUser.m_SecondDamage;
	}

	gObjUser.m_SecondInfo = true;
}

#endif

void ClientAccept()
{
	PMSG_CLIENT_ACCEPT pMsg;

	//if (*CMAIN_STATE2 == 0x2)
	//{
	//if (sTmp != NULL)
	//{
		pMsg.c = 0xC1;
		pMsg.headcode = 0xFD;
		pMsg.Accept = true;
		pMsg.size = sizeof(pMsg);
		//customDataSend((LPBYTE)&pMsg, pMsg.size);

		//Encode32((BYTE*)&pMsg);
		DataSend((LPBYTE)&pMsg, pMsg.size);
		//send(pActiveSocket, (char*)&pMsg, pMsg.size, 0);
		//wsSend_h(sTmp, (BYTE*)&pMsg, pMsg.size, 0);
		//this->lpViewPlayer = &*(ObjectPreview*)oUserPreviewStruct;
		//_endthread();
	//}
	//	g_Console.ConsoleOutput(2, "sTmps : %d", sTmp);
	/*}
	else
	{
		g_Console.ConsoleOutput(1, "sTmp : %d", sTmp);

	}*/
}

void UpdateLuckyWheel(ITEM_WIN_SEND* lpMsg)
{
	gLuckyWheel.LuckyWheelNumber = lpMsg->number;
	gLuckyWheel.StartRoll = 0;
}

void GCLuckyWheelRecv(PMSG_LUCKYWGEEL_LIST__RECV* lpMsg) // OK
{
	gLuckyWheel.ClearLuckyWheelList();

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_LUCKYWHEEL_LIST* lpInfo = (PMSG_LUCKYWHEEL_LIST*)(((BYTE*)lpMsg) + sizeof(PMSG_LUCKYWGEEL_LIST__RECV) + (sizeof(PMSG_LUCKYWHEEL_LIST) * n));

		gLuckyWheel.InsertLuckyWheel(lpInfo);
	}
}

#if(CUSTOM_VALUE_MENU)

void GCRecvValueMenu(PMSG_REQ_VALUE_MENU* lpMsg)
{
	if(!lpMsg->ValueOpen)
	{
		return;
	}

	if(lpMsg->ValueOpen == 1)
	{
		#if(DELETE_GUILDBANK_BT)	
		gInterface.SetBankHook(false);
		#endif

	if(gInterfaces->StateWare == 1)
	{
		gInterfaces->StateWare = 0;
		return;
	}

		gInterfaces->StateWare = 1;
	
	}

	if(lpMsg->ValueOpen == 2)
	{
		if(gInterfaces->StateWarex == 1)
		{
			gInterfaces->StateWarex = 0;
			return;
		}

			gInterfaces->StateWarex = 1;
	}
}

void CGSendValueMenu(int ValueType, int ValueNum)
{
	PMSG_ANS_VALUE_MENU pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0xDE, sizeof(pMsg));
	pMsg.ValueOpen = ValueType;
	pMsg.ValueNumber = ValueNum;
	DataSend((LPBYTE)&pMsg, pMsg.h.size);
}


#endif