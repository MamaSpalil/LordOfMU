#include "stdafx.h"
#include "ItemRank.h"
#include "Items.h"
#include "Structure.h"

ItemRank g_ItemRank;

//ItemRank::ItemRank()
//{
//	this->Init();
//}
//
//ItemRank::~ItemRank()
//{
//}
//
//void ItemRank::Init()
//{
//	this->m_Data.clear();
//	this->m_ItemList.clear();
//}
//
//void ItemRank::Load()
//{
//	this->Init();
//	SetOp((LPVOID)0x4FA5E8, this->SetOptionText, ASM::CALL);//425191
//	SetOp((LPVOID)0x7E568B, this->SetOptionText, ASM::CALL);//416BF4
////61AE88
////61B03A
//}
//
//int ItemRank::IsRankItem(WORD ItemType, BYTE ItemLevel)
//{
//	//for(int i = 0; i < this->m_ItemList.size(); i++)
//	//{
//	//	if(this->m_ItemList[i].ItemType != ItemType)
//	//	{
//	//		continue;
//	//	}
//
//	//	if(!this->IsDataRank(this->m_ItemList[i].RankID)) 
//	//	{
//	//		continue;
//	//	}
//
//	//	if(this->m_ItemList[i].MinLevel > ItemLevel)
//	//	{
//	//		continue;
//	//	}
//
//	//	return i;
//	//}
//
//	//return -1;
//
//	int iID = -1;
//	int iMaxLevel = 0;
//
//	for(int i = 0; i < this->m_ItemList.size(); i++)
//	{
//		if(this->m_ItemList[i].ItemType != ItemType)
//		{
//			continue;
//		}
//
//		if(!this->IsDataRank(this->m_ItemList[i].RankID)) 
//		{
//			continue;
//		}
//
//		if(this->m_ItemList[i].MinLevel > ItemLevel)
//		{
//			continue;
//		}
//
//		if(this->m_ItemList[i].MinLevel >= iMaxLevel)
//		{
//			iMaxLevel = this->m_ItemList[i].MinLevel;
//			iID = i;
//		}
//	}
//
//	if(iID == -1)
//	{
//		return -1;
//	}
//
//	return iID;
//}
//
//bool ItemRank::IsDataRank(BYTE RankID)
//{
//	for(int i = 0; i < this->m_Data.size(); i++)
//	{
//		if(this->m_Data[i].RankID == RankID)
//		{
//			return true;
//		}
//	}
//
//	return false;
//}
//
//int ItemRank::GetRankValueOption(int id, BYTE OptionType)
//{
//	int iRankID = -1;
//	int iResult = -1;
//
//	if(id < 0)
//	{
//		iResult = 0;
//		return iResult;
//	}
//
//	iRankID = this->m_ItemList[id].RankID;
//
//	if(iRankID <= 0)
//	{
//		iResult = 0;
//		return iResult;
//	}
//
//	for(int i = 0; i < this->m_Data.size(); i++)
//	{
//		if(this->m_Data[i].RankID == iRankID)
//		{
//			iResult = this->m_Data[i].OptionValue[OptionType];
//			break;
//		}
//	}
//
//	return iResult;
//}
//
//void ItemRank::SetOptionText(int ItemID, int Line, WORD OptionID, BYTE Arg4, int Arg5)	//bool Arg4
//{
//	pSetItemOptionText(ItemID, Line, OptionID, Arg4, Arg5);
//
//	//if(gItem.IsWings2_5(ItemID,false))
//	//{
//	//	switch(OptionID)
//	//	{
//	//		case eItemOption::IgnorOpDefense: pSetItemTextLine(Line, g_ExText.GetText(84), 4);	break;
//	//		case eItemOption::ReturnAttack:   pSetItemTextLine(Line, g_ExText.GetText(85), 4);	break;
//	//		case eItemOption::CompleteLife:   pSetItemTextLine(Line, g_ExText.GetText(86), 4);	break;
//	//		case eItemOption::CompleteMana:   pSetItemTextLine(Line, g_ExText.GetText(87), 4);	break;
//	//	}
//
//	//	return;
//	//}
//	//else if(gItem.IsWings4(ItemID,false))
//	//{
//	//	switch(OptionID)
//	//	{
//	//		case eItemOption::IgnorOpDefense: pSetItemTextLine(Line, g_ExText.GetText(84), 8);	break;
//	//		case eItemOption::ReturnAttack:   pSetItemTextLine(Line, g_ExText.GetText(85), 8);	break;
//	//		case eItemOption::CompleteLife:   pSetItemTextLine(Line, g_ExText.GetText(86), 8);	break;
//	//		case eItemOption::CompleteMana:   pSetItemTextLine(Line, g_ExText.GetText(87), 8);	break;
//	//	}
//
//	//	return;
//	//}
//	//else if(gItem.IsWings5(ItemID,false))
//	//{
//	//	switch(OptionID)
//	//	{
//	//		case eItemOption::IgnorOpDefense: pSetItemTextLine(Line, g_ExText.GetText(84), 8);	break;
//	//		case eItemOption::ReturnAttack:   pSetItemTextLine(Line, g_ExText.GetText(85), 8);	break;
//	//		case eItemOption::CompleteLife:   pSetItemTextLine(Line, g_ExText.GetText(86), 8);	break;
//	//		case eItemOption::CompleteMana:   pSetItemTextLine(Line, g_ExText.GetText(87), 8);	break;
//	//	}
//
//	//	return;
//	//}
//	//else if(gItem.IsWings6(ItemID,false))
//	//{
//	//	switch(OptionID)
//	//	{
//	//		case eItemOption::IgnorOpDefense: pSetItemTextLine(Line, g_ExText.GetText(84), 8);	break;
//	//		case eItemOption::ReturnAttack:   pSetItemTextLine(Line, g_ExText.GetText(85), 8);	break;
//	//		case eItemOption::CompleteLife:   pSetItemTextLine(Line, g_ExText.GetText(86), 8);	break;
//	//		case eItemOption::CompleteMana:   pSetItemTextLine(Line, g_ExText.GetText(87), 8);	break;
//	//	}
//
//	//	return;
//	//}
//
//	int iRankID = g_ItemRank.IsRankItem(ItemID, gItemPrice.ShowItemLevel);
//
//	if(iRankID < 0)
//	{
//		return;
//	}
//
//	switch(OptionID)
//	{
//    case eItemOption::LifeIncrease:
//       pSetItemTextLine(Line, "Increase Max HP +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 0));
//        break;
//    case eItemOption::ExcellentDamage:
//        pSetItemTextLine(Line, "Excellent Damage rate +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 0));
//        break;
//    case eItemOption::ManaIncrease:
//       pSetItemTextLine(Line, "Increase Max Mana +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 1));
//        break;
//    case eItemOption::DamageByLevel    :
//        pSetItemTextLine(Line, "Increase Damage +level/%d",
//            g_ItemRank.GetRankValueOption(iRankID, 1));
//        break;
//    case eItemOption::IncreaseWizardry:
//        pSetItemTextLine(Line, "Increase Wizardry Dmg +level/%d",
//            g_ItemRank.GetRankValueOption(iRankID, 1));
//        break;
//    case eItemOption::DamageDecrease:
//        pSetItemTextLine(Line, "Damage Decrease +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 2));
//        break;
//    case eItemOption::IncreaseDamage:
//        pSetItemTextLine(Line, "Increase Damage +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 2));
//       break;
//   case eItemOption::WizardrySucces:
//        pSetItemTextLine(Line, "Increase Wizardry Dmg +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 2));
//       break;
//    case eItemOption::Reflect:
//        pSetItemTextLine(Line, "Reflect Damage +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 3));
//        break;
//    case eItemOption::IncreaseSpeed:
//        pSetItemTextLine(Line, "Increase Attacking(Wizardy)speed +%d",
//            g_ItemRank.GetRankValueOption(iRankID, 3));
//        break;
//    case eItemOption::DefenseSuccess:
//        pSetItemTextLine(Line, "Defense success rate +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 4));
//        break;
//    case eItemOption::IgnorOpDefenseMana:
//        pSetItemTextLine(Line, "Increases acquisition rate of Life after hunting monsters +life/%d",
//            g_ItemRank.GetRankValueOption(iRankID, 4));
//        break;
//    case eItemOption::IncreaseZen:
//        pSetItemTextLine(Line, "Increases acquisition rate of Zen after hunting monsters +%d%%",
//            g_ItemRank.GetRankValueOption(iRankID, 5));
//        break;
//    case eItemOption::ManaPerMob:
//        pSetItemTextLine(Line, "Increases acquisition rate of Mana after hunting monsters +Mana/%d",
//            g_ItemRank.GetRankValueOption(iRankID, 5));
//        break;
//    }
//}
//
//void ItemRank::GCRankInit(PMSG_INITRANK* aRecv)
//{
//	if(aRecv->Result)
//	{
//		this->Init();
//	}
//}
//
//void ItemRank::GCRankData(PMSG_DATARANK* aRecv)
//{
//	ItemRank_Data lpData;
//	lpData.RankID = aRecv->RankID;
//	lpData.OptionValue[0] = aRecv->OptionValue[0];
//	lpData.OptionValue[1] = aRecv->OptionValue[1];
//	lpData.OptionValue[2] = aRecv->OptionValue[2];
//	lpData.OptionValue[3] = aRecv->OptionValue[3];
//	lpData.OptionValue[4] = aRecv->OptionValue[4];
//	lpData.OptionValue[5] = aRecv->OptionValue[5];
//	this->m_Data.push_back(lpData);
//}
//
//void ItemRank::GCRankList(PMSG_LISTRANK* aRecv)
//{
//	ItemRank_List lpData;
//	lpData.RankID = aRecv->RankID;
//	lpData.MinLevel = aRecv->MinLevel;
//	lpData.ItemType = aRecv->ItemType;
//
//	if(!this->IsDataRank(lpData.RankID))
//	{
//		return;
//	}
//
//	this->m_ItemList.push_back(lpData);
//}