#include "stdafx.h"
#include "Structure.h"
#include "User.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Interface.h"
#include "Protocol.h"
#include "Tray.h"
#include "Reconnect.h"
#include "Other.h"
#include "ResetSystem.h"
// ----------------------------------------------------------------------------------------------

User	gObjUser;
// ----------------------------------------------------------------------------------------------

void AddEffect(int Struct,float a2,int a3)
{
 
   
	lpViewObj gPreview	= &*(ObjectPreview*)Struct;


	
	 // v116 = 1156 * *((_DWORD *)v117 + 7) + dword_754F52C;
	//if ( (LODWORD(a2 ) + 188) != 1 || (LODWORD(a2) + 2) != 627|| (LODWORD(a2) + 4) != 191 )
    //   {
	//
	//}
	//else
	//{
	//  int v21 = (LODWORD(a2) + 168) & 0x4000;
    // if ( v21 != 16384 )
	// {
	//int v177 = 0;
	//int v178 = 0;
	//int v94 = 0;
	//int v250 = 0;
	//int v87 = 0;
	//char v239 = 1;
	//float v25 = flt_587C6C0 * flt_7B3AA8;
	//float v188 = v25 * flt_7B3A1C + flt_7B389C;
	//float v181 = v188 * v181;
	//float v28 = flt_587C6C0 * flt_7B3944;
    //   pSetEffect(2387,(float)a2  + 16,(float)a2 + 20,1.5,1.5,(int)v181,v28,3.0,5.0);
	//
	//     sub_698A05(ItemID, (LODWORD(a2) + 4), 0, ItemID + 1012, (LODWORD(a2) + 424), 0, 0, 0, 0, 0, v239, a3, 2);
	//  }
	//
	//  sub_6B839A(v21);
	//
	//
	//}

	//if(gPreview->m_Change == 10)
	//{

	//float v212 = 1056964608;
	//v212 = LODWORD(v212);
	int v192;
	v192 = 3205;

	//(a2 + 204) = 1;
	sub_698A05(Struct, 628, 0, Struct + 1012, ((GLfloat )(a2 + 424)), 0, 0, 0, 0, 0, 1, 0, 2);
	//(a2 + 204) = 0;

	// sub_6937C5(240 *(LODWORD(a2) + 2) + dword_587E610,a2,(LODWORD(a2) + 2),(LODWORD(a2) + 424),72,v212,-1);
	//  sub_5305AE(0, 0, 0, Struct, v192, v192, 0, 0, 1, 1, 0);

	//}
	pAddEffect(Struct,a2,a3);
	//g_Console.ConsoleOutput(1,"m_Change: %d ",gPreview->m_Change);
}
/* Не используется?
void  sub_4C00A5(int Strct)
{
	char v1; // al@20
	char v2; // al@22
	char v3; // al@24
	char v4; // al@26
	char v5; // al@28
	char v6; // al@57
	char v7; // al@57
	char v8; // al@57
	char v9; // al@57
	char v10; // al@57
	int v11; // [sp+4h] [bp-1Ch]@1
	signed int v12; // [sp+8h] [bp-18h]@4
	signed int j; // [sp+Ch] [bp-14h]@5
	signed int k; // [sp+Ch] [bp-14h]@10
	signed int i; // [sp+Ch] [bp-14h]@30
	signed int n; // [sp+Ch] [bp-14h]@46
	signed int l; // [sp+Ch] [bp-14h]@57
	signed int m; // [sp+Ch] [bp-14h]@61
	signed int v19; // [sp+10h] [bp-10h]@4
	signed int v20; // [sp+18h] [bp-8h]@4
	signed int v21; // [sp+1Ch] [bp-4h]@4

	v11 = Strct;
	if ((DWORD*)(Strct + 1808))
	{
		if ((BYTE*)(Strct + 648) != (BYTE*)(dword_754F540 + 524))
		{
			(Strct + 648) = (BYTE*)(dword_754F540 + 524);
			sub_53EF98(Strct + 124);
		}
		v19 = 0;
		v12 = 0;
		v20 = 0;
		v21 = 0;
		if (*(_BYTE *)(dword_754F540 + 1059))
		{
			sub_40BD60(&dword_5877B58, *(int *)dword_77AEE0C);
			v1 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
			if (sub_4BFF86(v11 + 724, *(_DWORD *)dword_77AEE0C + 3084, (unsigned __int8)v1 + 8827))
			{
				v2 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
				if (sub_4BFF86(v11 + 756, *(_DWORD *)dword_77AEE0C + 3160, (unsigned __int8)v2 + 8847))
				{
					v3 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
					if (sub_4BFF86(v11 + 788, *(_DWORD *)dword_77AEE0C + 3236, (unsigned __int8)v3 + 8867))
					{
						v4 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
						if (sub_4BFF86(v11 + 820, *(_DWORD *)dword_77AEE0C + 3312, (unsigned __int8)v4 + 8887))
						{
							v5 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
							if (!sub_4BFF86(v11 + 852, *(_DWORD *)dword_77AEE0C + 3388, (unsigned __int8)v5 + 8907))
								v19 = 1;
						}
						else
						{
							v19 = 1;
						}
					}
					else
					{
						v19 = 1;
					}
				}
				else
				{
					v19 = 1;
				}
			}
			else
			{
				v19 = 1;
			}
			for (i = 0; i < 2; ++i)
			{
				if (!sub_4BFF86(v11 + 32 * i + 884, *(_DWORD *)dword_77AEE0C + 76 * i + 2932, -1))
				{
					v12 = 1;
					break;
				}
			}
			if (!sub_4BFF86(v11 + 948, *(_DWORD *)dword_77AEE0C + 3464, -1))
				v20 = 1;
			if (!sub_4BFF86(v11 + 980, *(_DWORD *)dword_77AEE0C + 3540, -1))
				v21 = 1;
			sub_40BE20(&dword_5877B58, *(int *)dword_77AEE0C);
		}
		else
		{
			for (j = 0; j < 6; ++j)
			{
				if (!sub_4BFF33(v11 + 32 * j + 692, dword_754F540 + 32 * j + 568))
				{
					v19 = 1;
					break;
				}
			}
			for (k = 0; k < 2; ++k)
			{
				if (!sub_4BFF33(v11 + 32 * k + 884, dword_754F540 + 32 * k + 760))
				{
					v12 = 1;
					break;
				}
			}
			if (!sub_4BFF33(v11 + 948, dword_754F540 + 824))
				v20 = 1;
			if (!sub_4BFF33(v11 + 980, dword_754F540 + 856))
				v21 = 1;
		}
		if (v19 || v12 || v20 || v21)
		{
			if (*(_BYTE *)(dword_754F540 + 1059))
			{
				if (v19 == 1)
				{
					sub_525E94(v11 + 124, 0, 0);
					sub_40BD60(&dword_5877B58, *(int *)dword_77AEE0C);
					*(_WORD *)(v11 + 692) = (unsigned __int8)sub_545302(*(_BYTE *)(dword_754F540 + 524)) + 8827;
					v6 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
					sub_4C003B(v11 + 724, *(_DWORD *)dword_77AEE0C + 3084, (unsigned __int8)v6 + 8827);
					v7 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
					sub_4C003B(v11 + 756, *(_DWORD *)dword_77AEE0C + 3160, (unsigned __int8)v7 + 8847);
					v8 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
					sub_4C003B(v11 + 788, *(_DWORD *)dword_77AEE0C + 3236, (unsigned __int8)v8 + 8867);
					v9 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
					sub_4C003B(v11 + 820, *(_DWORD *)dword_77AEE0C + 3312, (unsigned __int8)v9 + 8887);
					v10 = sub_545302(*(_BYTE *)(dword_754F540 + 524));
					sub_4C003B(v11 + 852, *(_DWORD *)dword_77AEE0C + 3388, (unsigned __int8)v10 + 8907);
					sub_40BE20(&dword_5877B58, *(int *)dword_77AEE0C);
					for (l = 0; l < 6; ++l)
					{
						*(_DWORD *)(v11 + 32 * l + 716) = 0;
						*(_DWORD *)(v11 + 32 * l + 720) = 0;
						*(_BYTE *)(v11 + 32 * l + 712) = 0;
					}
				}
				if (v12 == 1)
				{
					sub_40BD60(&dword_5877B58, *(int *)dword_77AEE0C);
					for (m = 0; m < 2; ++m)
						sub_4C003B(v11 + 32 * m + 884, *(_DWORD *)dword_77AEE0C + 76 * m + 2932, -1);
					sub_40BE20(&dword_5877B58, *(int *)dword_77AEE0C);
				}
				if (v20 == 1)
				{
					sub_40BD60(&dword_5877B58, *(int *)dword_77AEE0C);
					sub_4C003B(v11 + 948, *(_DWORD *)dword_77AEE0C + 3464, -1);
					sub_40BE20(&dword_5877B58, *(int *)dword_77AEE0C);
				}
				if (v21 == 1)
				{
					sub_40BD60(&dword_5877B58, *(int *)dword_77AEE0C);
					sub_4C003B(v11 + 980, *(_DWORD *)dword_77AEE0C + 3540, -1);
					sub_40BE20(&dword_5877B58, *(int *)dword_77AEE0C);
				}
			}
			else
			{
				if (v19 == 1)
				{
					sub_525E94(v11 + 124, 0, 0);
					sub_7911C0(v11 + 692, dword_754F540 + 568, 192);
					for (n = 0; n < 6; ++n)
					{
						*(_DWORD *)(v11 + 32 * n + 716) = 0;
						*(_DWORD *)(v11 + 32 * n + 720) = 0;
						*(_BYTE *)(v11 + 32 * n + 712) = 0;
					}
				}
				if (v12 == 1)
					sub_7911C0(v11 + 884, dword_754F540 + 760, 64);
				if (v20 == 1)
				{
					sub_7911C0(v11 + 948, dword_754F540 + 824, 32);
					sub_525E94(0, v11 + 124, 0);
				}
				if (v21 == 1)
					sub_7911C0(v11 + 980, dword_754F540 + 856, 32);
			}
			if (v21 == 1)
			{
				sub_4BF4F9(v11, 122, 0);
				*(_DWORD *)(v11 + 1832) = 122;
			}
			else
			{
				sub_4BF4F9(v11, *(_DWORD *)(v11 + 1828), 0);
			}
			if (v21 == 1 || v12 == 1)
			{
				*(_BYTE *)(v11 + 1280) = 0;
				switch (*(_WORD *)(v11 + 980))
				{
				case 7291:
					sub_45DC65(7291, v11 + 140, v11 + 124, v11 + 1280, 0);
					break;
				case 7293:
					sub_45DC65(217, v11 + 140, v11 + 124, v11 + 1280, 0);
					break;
				case 7294:
					sub_45DC65(302, v11 + 140, v11 + 124, v11 + 1280, 0);
					break;
				case 7295:
					sub_45DC65(210, v11 + 140, v11 + 124, v11 + 1280, 0);
					break;
				case 7328:
					switch (*(_BYTE *)(v11 + 983))
					{
					case 1:
						sub_45DC65(360, v11 + 140, v11 + 124, v11 + 1280, 0);
						break;
					case 2:
						sub_45DC65(362, v11 + 140, v11 + 124, v11 + 1280, 0);
						break;
					case 4:
						sub_45DC65(363, v11 + 140, v11 + 124, v11 + 1280, 0);
						break;
					default:
						sub_45DC65(361, v11 + 140, v11 + 124, v11 + 1280, 0);
						break;
					}
					break;
				default:
					break;
				}
				*(_DWORD *)(v11 + 1704) = 0;
				*(float *)(v11 + 1804) = *(float *)(v11 + 1292) * flt_7B3A98 / *(float *)(v11 + 136);
			}
		}
	}
}
*/




void  SetFenrir(int Structure, int a2)
{
	////lpViewObj
	//float v2; // ST14_4@3
	//float v3; // ST10_4@3
	//char v5; // [sp+17h] [bp-15h]@3
	//int v6; // [sp+18h] [bp-14h]@21
	//float v7; // [sp+1Ch] [bp-10h]@24
	//float v8; // [sp+20h] [bp-Ch]@24
	//float v9; // [sp+24h] [bp-8h]@24
	//float v10; // [sp+28h] [bp-4h]@23
	//
	//lpViewObj gPreview = &*(ObjectPreview*)Structure;
	//
	//if (Structure)
	//{
	//	if (a2)
	//	{
	//		v5 = 1;
	//	}
	//	else
	//	{
	//		v2 = (gPreview->VecPosZ) * flt_7B3944;
	//		v3 = (gPreview->VecPosX) * flt_7B3944;
	//		v5 = sub_6771F8(v3, v2, -20.0);
	//	}
	//	gPreview->Unknown416 = v5;
	//	if (gPreview->Unknown416)
	//	{
	//		//if (/*((gPreview->Unknow316) + 2) != 627 && */gPreview->MauntType != 7291)
	//		//	return 1;
	//		if (a2)
	//		{
	//			gPreview->ModelScale = 1060823368;
	//		}
	//		else if (dword_7E8088 == 2)
	//		{
	//			gPreview->ModelScale = 1070386381;
	//		}
	//		else if (dword_7E8088 == 4)
	//		{
	//			gPreview->ModelScale = 1065353216;
	//		}
	//		else if (gPreview->MauntType == 360
	//			|| gPreview->MauntType == 362
	//			|| gPreview->MauntType == 361
	//			|| gPreview->MauntType == 363
	//			|| gPreview->MauntType == 364 )
	//		{
	//			gPreview->ModelScale = 1065353216;
	//		}
	//		else
	//		{
	//			gPreview->ModelScale = 1065353216;
	//		}
	//		v6 = 0;
	//	//	if ((((gPreview->Unknow316) + 168) & 0x4000) == 0x4000)
	//			v6 = 10;
	//		sub_682783(Structure, 0, 0, v6);
	//		v10 = (double)(sub_791979() % 30 + 70) * flt_7B3944;
	//
	//		if (gPreview->MauntType == 7291)
	//		{
	//			v7 = v10 * flt_7B389C;
	//			v8 = v10 * flt_7B3A90;
	//			v9 = v10 * flt_7B3A70;
	//			pAddEffectSet2(1957, gPreview->VecPosX, 1065353216, (int)&v7, Structure, 0, 0);
	//		}
	//	}
	//}
	//return 1;
	lpViewObj gPreview = &*(ObjectPreview*)Structure;
	if (gPreview->MauntType == 364 )
	{
		gPreview->ModelScale = 1065353216;
	}
	sub_45FC93(Structure, a2);
}
void LoadFunction()
{
	SetHook((LPVOID)SetFenrir, (LPVOID)0x0045FEBF, ASM::CALL);
	SetHook((LPVOID)SetFenrir, (LPVOID)0x004BF49F, ASM::CALL);
}

void LoadUser()
{
SetHook((LPVOID)AddEffect, (LPVOID)0x004BF4B1, ASM::CALL);
SetHook((LPVOID)AddEffect, (LPVOID)0x0053D324, ASM::CALL);
SetHook((LPVOID)AddEffect, (LPVOID)0x0053D338, ASM::CALL);
SetHook((LPVOID)AddEffect, (LPVOID)0x006D91CC, ASM::CALL);
SetHook((LPVOID)AddEffect, (LPVOID)0x00714BB9, ASM::CALL);
}
void User::Load()//tet
{
	//ZeroMemory(&this->lpPlayer, sizeof(this->lpPlayer));
	this->lpViewPlayer = &*(ObjectPreview*)oUserPreviewStruct;
	ZeroMemory(&this->lpViewPlayer, sizeof(this->lpViewPlayer));
	ZeroMemory(&this->lpViewTarget, sizeof(this->lpViewTarget));
	// ----
	this->m_TargetUpdateTick	= 0;
	this->m_TargetType			= false;
	this->m_CursorX				= -1;
	this->m_CursorY				= -1;
	this->m_MapNumber			= -1;
#if(DEV_DAMAGE_TABLE)
	this->m_SecondDamageMax = 0;
	this->m_SecondDamage = 0;
	this->m_SecondDefence = 0;
	this->m_SecondReflect = 0;
#endif
	this->WarehouseNumber = 0;

	// ----
	//SetOp((LPVOID)0x005B96E8, (LPVOID)this->DrawPartyHP, ASM::CALL);
	// ----
#ifdef __NOVUS__
	SetOp((LPVOID)0x00588511, (LPVOID)this->SetEventEffect, ASM::CALL);
#endif
}
void User::WarehouseReq(CHANGEWAREHOUSE * aRecv)
{
	this->WarehouseNumber = aRecv->Number;

	#if(CUSTOM_VALUE_MENU)
	if(g_ResetSystem.Active = 1)
	{
		g_ResetSystem.Active = 0;
	}
	//if(gInterface.CheckWindowEx(ObjWindowsEx::exWinValueMenu))//TRUE
	//{
	//	gInterface.CloseWindowEx(ObjWindowsEx::exWinValueMenu);//FALSE
	//}

	//if(gInterface.CheckWindowEx(ObjWindowsEx::exWinGuildValueMenu))
	//{
	//	gInterface.CloseWindowEx(ObjWindowsEx::exWinGuildValueMenu);
	//}
	#endif
}
// ----------------------------------------------------------------------------------------------

void User::Refresh()
{
	lpCharObj lpPlayer			= pUserObjectStruct;
	//this->lpPlayer			= &*(ObjectCharacter*)oUserObjectStruct;
	//this->lpViewPlayer		= &*(ObjectPreview*)oUserPreviewStruct;

	//this->GetTarget();
	// ----
	this->m_CursorX			= pCursorX;
	this->m_CursorY			= pCursorY;
	this->m_MapNumber		= pMapNumber;

//	if( (gObjUser.lpPlayer->Class & 7) != 6 )
//	{
//		SetByte((PVOID)(0x00556C32 + 6), 2);
//	}
//	else
//	{
//		SetByte((PVOID)(0x00556C32 + 6), 15);
//	}
	// ----
	if( gTrayMode.InTray )
	{
		//if( !this->lpViewPlayer->m_Model.Unknown4 && gReconnect.m_ConnectState == 0 )
		//{
		//	gTrayMode.ShowMessage(NIIF_WARNING, "MU Online", "Your character has been killed");
		//}
	}
}
// ----------------------------------------------------------------------------------------------

//bool User::GetTarget()
//{
////	if( pViewNPCTargetID != -1 )
////	{
////		this->lpViewTarget	= &*(ObjectPreview*)pGetPreviewStruct(pPreviewThis(), pViewNPCTargetID);
////		this->m_TargetType	= 1;
////		return true;
////	}
////	else if( pViewAttackTargetID != -1 )
////	{
////		this->lpViewTarget	= &*(ObjectPreview*)pGetPreviewStruct(pPreviewThis(), pViewAttackTargetID);
////		this->m_TargetType	= 2;
////		return true;
////	}
////	// ----
////	if( this->lpViewTarget != 0 )
////	{
////		ZeroMemory(&this->lpViewTarget, sizeof(this->lpViewTarget));
////	}
////	// ----
////	this->m_TargetType = 0;
//..	return false;
//}
// ----------------------------------------------------------------------------------------------

//int User::GetActiveSkill()
//{
//	//return this->lpPlayer->pMagicList[this->lpViewPlayer->ActiveMagic];
//}
// ----------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------

void User::SetTargetData(PMSG_TARGETDATA_ANS * aRecv)
{
	this->m_TargetLifePercent	= 0.0f;
	this->m_TargetLifePercent	= aRecv->TargetLifePercent;
	this->m_TargetUpdateTick	= GetTickCount();
}
// ----------------------------------------------------------------------------------------------

void User::SetEventEffect(int PreviewStruct)
{
//	//SetByte((PVOID)(0x004EFF0D + 2), 3);	//Min event effect number
//	// ----
//	lpViewObj lpPreview = &*(ObjectPreview*)PreviewStruct;
//	// ----
//	if( lpPreview->CtlCode == 32 )
//	{
//		if( *(DWORD*)(PreviewStruct + 672) && lpPreview->Unknown23 != 5 )
//		{
//			pInitEventStatus(PreviewStruct);
//		}
//		// ----
//		lpPreview->Unknown23 = 5;
//	}
//	// ----
//	if( *(DWORD*)(PreviewStruct + 672) != 0 || !lpPreview->Unknown23 )
//	{
//		return;
//	}
//	// ----
//	switch(lpPreview->Unknown23)
//	{
//	case 5:	//-> GM
//		{
//			if( pEventEffectThis(668) )
//			{
//				DWORD ModelID = 349;
//				// ----
//				if( !strncmp(lpPreview->Name, "[GM]", 4) )
//				{
//					ModelID = 406;
//				}
//				else if( !strncmp(lpPreview->Name, "[EM]", 4) )
//				{
//					ModelID = 407;
//				}
//				else if( !strcmp(lpPreview->Name, "Admin") )
//				{
//					ModelID = 408;
//				}
//				// ----
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect(pEventEffectThis(668), ModelID, 20, 1, 70.0, -5.0, 0.0, 0.0, 0.0, 45.0);
//			}
//		}
//		break;
//		// --
//	case 6:
//		{
//			if( pEventEffectThis(668) )
//			{
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 0, 20, 120.0, 0.0, 0.0);
//			}
//		}
//		break;
//		// --
//	case 7:
//		{
//			if( pEventEffectThis(668) )
//			{
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 1, 20, 120.0, 0.0, 0.0);
//			}
//		}
//		break;
//		// --
//	case 8:
//		{
//			if( pEventEffectThis(668) )
//			{
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 2, 20, 120.0, 0.0, 0.0);
//			}
//		}
//		break;
//		// --
//	case 9:
//		{
//			if( pEventEffectThis(668) )
//			{
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 3, 20, 120.0, 0.0, 0.0);
//			}
//		}
//		break;
//		// --
//	case 10:
//		{
//			if( pEventEffectThis(668) )
//			{
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 4, 20, 120.0, 0.0, 0.0);
//			}
//		}
//		break;
//		// --
//	case 11:
//		{
//			if( pEventEffectThis(668) )
//			{
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 5, 20, 120.0, 0.0, 0.0);
//			}
//		}
//		break;
//		// --
//	case 12:
//		{
//			if( pEventEffectThis(668) )
//			{
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 6, 20, 120.0, 0.0, 0.0);
//			}
//		}
//		break;
//		// --
//	case 13:
//		{
//			if( pEventEffectThis(668) )
//			{
//				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 7, 20, 120.0, 0.0, 0.0);
//			}
//		}
//		break;
//		// --
//	default:
//		{
//			lpPreview->Unknown23 = 0;
//		}
//		break;
//	}
}
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------

void User::DrawPartyHP()
{
	//if( pPartyMemberCount <= 0 )
	//{
	//	return;
	//}
	//// ----
	//float LifeBarWidth = 38.0f;
	//char LifeDisplay[20];
	//VAngle Angle;
	//int PosX, PosY, LifeProgress;
	//// ----
	//for( int PartySlot = 0; PartySlot < pPartyMemberCount; PartySlot++ )
	//{
	//	PartyList PartyMember	= *(PartyList*)((char*)&pPartyListStruct + sizeof(PartyList) * PartySlot);
	//	lpViewObj lpPartyObj	= &*(ObjectPreview*)pGetPreviewStruct(pPreviewThis(), PartyMember.ViewportID);
	//	// ----
	//	if( !lpPartyObj )
	//	{
	//		continue;
	//	}
	//	// ----
	//	Angle.X = lpPartyObj->m_Model.VecPosX;
	//	Angle.Y = lpPartyObj->m_Model.VecPosY;
	//	Angle.Z = lpPartyObj->m_Model.VecPosZ + lpPartyObj->m_Model.Unknown216.Z + 100.0;
	//	// ----
	//	pGetPosFromAngle(&Angle, &PosX, &PosY);
	//	PosX -= (int)floor(LifeBarWidth / (double)2.0);	//T_T
	//	// ----
	//	if(		pCursorX >= PosX
	//		&&	(float)pCursorX <= (float)PosX + LifeBarWidth
	//		&&	pCursorY >= PosY - 2 
	//		&&	pCursorY < PosY + 6 )
	//	{
	//		sprintf(LifeDisplay, "HP : %d0%%", PartyMember.LifePercent);
	//		pSetTextColor(pTextThis(), 0xFF, 0xE6, 0xD2, 0xFF);
	//		pDrawText(pTextThis(), PosX, PosY - 6, LifeDisplay, 0, 0, (LPINT)1, 0);
	//	}
	//	// ----
	//	pSetBlend(true);
	//	// ----
	//	glColor4f(0.0, 0.0, 0.0, 0.5);
	//	pDrawBarForm((float)(PosX + 1), (float)(PosY + 1), LifeBarWidth + 4.0, 5.0, 0.0, 0);
	//	pGLSwitchBlend();
	//	// ----
	//	glColor3f(0.2, 0.0, 0.0);
	//	pDrawBarForm((float)PosX, (float)PosY, LifeBarWidth + 4.0, 5.0, 0.0, 0);
	//	// ----
	//	glColor3f(0.19607843, 0.039215688, 0.0);
	//	pDrawBarForm((float)(PosX + 2), (float)(PosY + 2), LifeBarWidth, 1.0, 0.0, 0);
	//	//-> Very strange method, maybe will be cool LifePercent / 10? :D
	//	if( PartyMember.LifePercent > 10 )	
	//	{
	//		LifeProgress = 10;
	//	}
	//	else
	//	{
	//		LifeProgress = PartyMember.LifePercent;
	//	}
	//	// ----
	//	glColor3f(0.98039216, 0.039215688, 0.0);
	//	// ----
	//	for( int i = 0; i < LifeProgress; i++ )
 //       {
	//		pDrawBarForm((float)(i * 4 + PosX + 2), (float)(PosY + 2), 3.0, 2.0, 0.0, 0);
 //       }
	//	// ----
	//	pGLSwitch();
	//}
	//// ----
	//pGLSwitch();
 //   glColor3f(1.0, 1.0, 1.0);
}
// ----------------------------------------------------------------------------------------------
bool User::CheckDistance(int aIndex, int bIndex, int distance)
{
	if (aIndex < 0 || aIndex > 10000)
	{
		return 0;
	}

	if (bIndex < 0 || bIndex > 10000)
	{
		return 0;
	}

	if (this->m_Object[aIndex].x < (this->m_Object[bIndex].x - distance) || this->m_Object[aIndex].x >(this->m_Object[bIndex].x + distance) || this->m_Object[aIndex].y < (this->m_Object[bIndex].y - distance) || this->m_Object[aIndex].y >(this->m_Object[bIndex].y + distance))
	{
		return 0;
	}

	return 1;
}
DWORD User::GetIndex()
{
	return this->index;
}
BYTE User::CalcDistance(int aIndex, int bIndex)
{
	if (aIndex < 0 || aIndex > 10000)
	{
		return 255;
	}

	if (bIndex < 0 || bIndex > 10000)
	{
		return 255;
	}


	if (this->m_Object[aIndex].x == this->m_Object[bIndex].x && this->m_Object[aIndex].y == this->m_Object[bIndex].y)
	{
		return 0;
	}

	float tx = this->m_Object[aIndex].x - this->m_Object[bIndex].x;
	float ty = this->m_Object[aIndex].y - this->m_Object[bIndex].y;

	return sqrt((tx*tx) + (ty*ty));
}
OBJECTSTRUCT User::GetObject(DWORD index)
{
	if (index < 0 || index > 10000)
	{
		return this->m_Object[0];
	}

	return this->m_Object[index];
}
LPSTR User::GetCharacterName()
{
	this->RefreshUserStruct();
	return this->Name;
}
void User::RefreshUserStruct()
{
	//DWORD StructData = *(int*)0x077A8FDC;
	//LPVOID ClassOffset = (LPVOID)0x5877AE8;
	//int a1 = ((unsigned int(__thiscall*)(LPVOID, int)) 0x00404120)(ClassOffset, *(int*)0x77A8FDC);

	//if (a1 == -1)
	//{
	//	LPVOID v6 = ((LPVOID(__cdecl*)(int)) 0x00790EB2)(3905);

	//	*(BYTE*)(*(int*)v6 + 3904) = 1;
	//	((signed int(__thiscall*)(LPVOID, int, int)) 0x00403FC0)(ClassOffset, *(int*)v6, StructData);
	//}
	//else
	//{
	//	int v3 = ((unsigned int(__thiscall*)(LPVOID, int)) 0x00404120)(ClassOffset, *(int*)0x77A8FDC);

	//	DWORD v4 = 0;

	//	if (v3 != -1)
	//	{
	//		v4 = *(DWORD*)(*(int*)0x05877AEC + 4 * v3);
	//	}

	//	BYTE v5 = *(BYTE*)(v4 + 3904) + 1;
	//	//*(BYTE*)(v4 + 3904) = v5;

	//	if (v5 < 2)
	//	{
	//		((void(__stdcall*)(int, int)) 0x0040C040)(StructData, v4);
	//	}
	//}


	//this->m_CharObj = &*(ObjectCharacter*)(USER_STRUCT);



	/*memset(this->Name, 0, sizeof(this->Name));
	strncpy_s(this->Name, this->m_CharObj->Name, sizeof(this->Name) - 1);*/
}
void User::DrawSome()
{
//	char LifeDisplay[20];
//	VAngle Angle;
//	int PosX, PosY, LifeProgress;
//	// ----
//	float LifeBarWidth	= 38.0f;
//	lpViewObj lpObj		= gObjUser.lpViewTarget;
//	// ----
//	if( !lpObj || lpObj->m_Model.ObjectType != emMonster || !lpObj->m_Model.Unknown4 )
//	{
//		return;
//	}
//	// ----
//	Angle.X = lpObj->m_Model.VecPosX;
//	Angle.Y = lpObj->m_Model.VecPosY;
//	Angle.Z = lpObj->m_Model.VecPosZ + lpObj->m_Model.Unknown216.Z + 100.0;
//	// ----
//	pGetPosFromAngle(&Angle, &PosX, &PosY);
//	PosX -= (int)floor(LifeBarWidth / (double)2.0);
//	// ----
//	sprintf(LifeDisplay, "%s", lpObj->Name);
//	pSetTextColor(pTextThis(), 0xFF, 0xE6, 0xD2, 0xFF);
//	pDrawText(pTextThis(), PosX + 2, PosY - 6, LifeDisplay, 0, 0, (LPINT)1, 0);
//	// ----
//	pSetBlend(true);
//	glColor4f(0.0, 0.0, 0.0, 0.5);
//	pDrawBarForm((float)(PosX + 1), (float)(PosY + 1), LifeBarWidth + 4.0, 5.0, 0.0, 0);
//	pGLSwitchBlend();
//	// ----
//	glColor3f(0.2, 0.0, 0.0);
//	pDrawBarForm((float)PosX, (float)PosY, LifeBarWidth + 4.0, 5.0, 0.0, 0);
//	// ----
//	glColor3f(0.19607843, 0.039215688, 0.0);
//	pDrawBarForm((float)(PosX + 2), (float)(PosY + 2), LifeBarWidth, 1.0, 0.0, 0);
//	// ----
//	if( (int)(gObjUser.m_TargetLifePercent / 10) > 10 )
//	{
//		LifeProgress = 10;
//	}
//	else
//	{
//		LifeProgress = (int)(gObjUser.m_TargetLifePercent / 10);
//	}
//	// ----
//	glColor3f(0.98039216, 0.039215688, 0.0);
//	// ----
//	for( int i = 0; i < LifeProgress; i++ )
//	{
//		pDrawBarForm((float)(i * 4 + PosX + 2), (float)(PosY + 2), 3.0, 2.0, 0.0, 0);
//	}
//	// ----
//	pGLSwitch();
//    glColor3f(1.0, 1.0, 1.0);
}
// ----------------------------------------------------------------------------------------------