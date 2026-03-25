#include "stdafx.h"
#include "Jewels_Wings.h"
#include "TMemory.h"
#include "Structure.h"

//---------------------------------------------------------------------------
DWORD	dwItemDescriptionText_Pointer;
char*   cItemDecriptionText_Line1;
DWORD	dwItemDescription_Line1_Weight;
DWORD	dwItemDescription_Line1_Color;
char*	cItemDecriptionText_Line2;
DWORD	dwItemDescription_Line2_Weight;
DWORD	dwItemDescription_Line2_Color;
char*	cItemDecriptionText_Line3;
DWORD	dwItemDescription_Line3_Weight;
DWORD	dwItemDescription_Line3_Color;
DWORD	dwItemDescription_Line_Count;
WORD	wItemDropBlock_Pointer;
bool	bItemDropBlock_Set;
WORD	wItemTitleColor_Pointer;
DWORD	dwItemTitleColor_Set;

//---------------------------------------------------------------------------


Naked (cItemDescriptionText)//вот оно
{
	cItemDecriptionText_Line1		= 0;
	cItemDecriptionText_Line2		= 0;
	cItemDecriptionText_Line3		= 0;
	dwItemDescription_Line1_Weight	= 0;
	dwItemDescription_Line2_Weight	= 0;
	dwItemDescription_Line2_Weight	= 0;
	dwItemDescription_Line1_Color	= 0;
	dwItemDescription_Line2_Color	= 0;
	dwItemDescription_Line3_Color	= 0;
	dwItemDescription_Line_Count	= 0;
	// ----
	_asm
	{
		mov esi, dword ptr ss:[ebp]
		mov dwItemDescriptionText_Pointer, esi
	}
	// ----
	if( dwItemDescriptionText_Pointer == ITEM(12, 15) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "It is used to combine Chaos items.";
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 101) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to add luck in items and weapons";
	}


//Scorpion Mobility
	else if( dwItemDescriptionText_Pointer == ITEM(14, 89) )
	{
		dwItemDescription_Line_Count = 2;
		// ----
		cItemDecriptionText_Line1 = "Magic stone will appear when you throw it on the ground ";
		dwItemDescription_Line1_Color = ItemColor::White;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Use this item to move to Scorpion";
		dwItemDescription_Line2_Color = ItemColor::WhiteBlue;
	}
	//TEST
	else if( dwItemDescriptionText_Pointer == ITEM(14, 102) )
	{
		dwItemDescription_Line_Count = 2;
		// ----
		cItemDecriptionText_Line1 = "Sell/Buy Price 100000000 Zen. ";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		// ---- 
		cItemDecriptionText_Line2 =  "Coins A2K";
		dwItemDescription_Line2_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 103) )
	{
		dwItemDescription_Line_Count = 2;
		// ----
		cItemDecriptionText_Line1 = "Sell/Buy Price 500000000 Zen. ";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		// ---- 
		cItemDecriptionText_Line2 =  "Coins A2K";
		dwItemDescription_Line2_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 104) )
	{
		dwItemDescription_Line_Count = 2;
		// ----
		cItemDecriptionText_Line1 = "Sell/Buy Price 1000000000 Zen. ";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		// ---- 
		cItemDecriptionText_Line2 =  "Coins A2K";
		dwItemDescription_Line2_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 105) )
	{
		dwItemDescription_Line_Count = 2;
		// ----
		cItemDecriptionText_Line1 = "Prize for Blood Castle Event";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		// ---- 
		cItemDecriptionText_Line2 =  "BC Box";
		dwItemDescription_Line2_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 106) )
	{
		dwItemDescription_Line_Count = 2;
		// ----
		cItemDecriptionText_Line1 = "Prize for Chaos Castle Event";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		// ---- 
		cItemDecriptionText_Line2 =  "CC Box";
		dwItemDescription_Line2_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 107) )
	{
		dwItemDescription_Line_Count = 2;
		// ----
		cItemDecriptionText_Line1 = "Prize for Devil Square Event";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		// ---- 
		cItemDecriptionText_Line2 =  "DS Box";
		dwItemDescription_Line2_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(12, 41) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Wizardy Damage + 15%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Absorb Damage + 10%%";
		dwItemDescription_Line2_Color = ItemColor::DarkViolet;

		// ---- 
		cItemDecriptionText_Line3 =  "Wing 4 Lvl A2K";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(12, 42) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Critical Damage + 10%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Exellent Damage + 13%%";
		dwItemDescription_Line2_Color = ItemColor::DarkViolet;

		// ---- 
		cItemDecriptionText_Line3 =  "Wing 4 Lvl A2K";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(12, 43) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Absord Damage + 10%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Double Damage + 10%%";
		dwItemDescription_Line2_Color = ItemColor::DarkViolet;

		// ---- 
		cItemDecriptionText_Line3 =  "Wing 4 Lvl A2K";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(12, 44) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Wizardy Damage + 10%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Exellent Damage + 10%%";
		dwItemDescription_Line2_Color = ItemColor::DarkViolet;

		// ---- 
		cItemDecriptionText_Line3 =  "Wing 4 Lvl A2K";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(12, 45) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Critical Damage + 10%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Exellent Damage + 10%%";
		dwItemDescription_Line2_Color = ItemColor::DarkViolet;

		// ---- 
		cItemDecriptionText_Line3 =  "Wing 4 Lvl A2K";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	else if( dwItemDescriptionText_Pointer == ITEM(14, 90) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to add Exc option in items and weapons";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 91) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to get Exc option in Wings";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 92) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to add Luck option in items,weapons and Wings";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 93) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to add Skill option in weapons";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 100) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to add Level in items,weapons and Wings";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 95) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to get Exc option item status";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 96) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to get Anc option item status";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 97) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to get Add option in items,weapons and Wings";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 98) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to get PvP option in items and weapons";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 99) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Used to add Level in Rings and Pendants";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 150) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Double Damage + 8%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Absorb Damage + 10%%";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "[A2K] Amulet active when in inventory!";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 151) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Reflect Damage + 7%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Wizardy Damage + 10%%";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "[A2K] Amulet active when in inventory!";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 152) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Critical Damage + 9%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Exellent Damage + 7%%";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "[A2K] Amulet active when in inventory!";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 153) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Min Defence + 15%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Ignore Defence + 11%%";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "[A2K] Amulet active when in inventory!";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 154) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Damage + 10%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Critical Damage + 5%%";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "[A2K] Amulet active when in inventory!";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 155) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Zen + 7%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Exp + 15%%";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "[A2K] Amulet active when in inventory!";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 156) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "SD Recovery + 7%%";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Life Recovery + 15%%";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "[A2K] Amulet active when in inventory!";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(14, 147) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Auto pick up Zen";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Auto pick up Jewels";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "[A2K] Amulet active when in inventory!";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(13, 0) )
	{
		dwItemDescription_Line_Count = 1;
		// ----
		cItemDecriptionText_Line1 = "Auto Pick Up Zen";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;
	}
	else if( dwItemDescriptionText_Pointer == ITEM(13, 1) )
	{
		dwItemDescription_Line_Count = 3;
		// ----
		cItemDecriptionText_Line1 = "Auto pick up Zen";
		dwItemDescription_Line1_Color = ItemColor::Green;
		dwItemDescription_Line1_Weight = ItemWeight::Normal;

		// ---- 
		cItemDecriptionText_Line2 =  "Auto pick up Jewels";
		dwItemDescription_Line2_Color = ItemColor::Blue;

		// ---- 
		cItemDecriptionText_Line3 =  "A2K Custom Option";
		dwItemDescription_Line3_Color = ItemColor::GreenBlue;
	}
	if( dwItemDescription_Line_Count == 0 )
	{
		_asm
		{
			mov esi, iItemDescriptionText_Next
			jmp esi
		}
	}
	else
	{
		if( dwItemDescription_Line_Count == 3 )
        {
            _asm
            {
				lea eax, [eax * 0x4 + eax]
				push cItemDecriptionText_Line1
				lea edx, [eax * 0x4 + eax]
				lea eax, [edx * 0x4 + iItemDescriptionText_Point]
				push eax
				mov esi, iItemDescriptionText_Crypt
				call esi
				mov eax, dword ptr ds:[iItemDescriptionText_Line]
				add esp, 0x8
				mov esi, dwItemDescription_Line1_Color
				mov dword ptr ds:[eax * 0x4 + iItemDescriptionText_Color], esi
				mov esi, dwItemDescription_Line1_Weight
				mov dword ptr ds:[eax * 0x4 + iItemDesctiptionText_Weight], esi
				inc eax
				mov dword ptr ds:[iItemDescriptionText_Line], eax
				// ----
				lea eax, [eax * 0x4 + eax]
				push cItemDecriptionText_Line2
				lea edx, [eax * 0x4 + eax]
				lea eax, [edx * 0x4 + iItemDescriptionText_Point]
				push eax
				mov esi, iItemDescriptionText_Crypt
				call esi
				mov eax, dword ptr ds:[iItemDescriptionText_Line]
				add esp, 0x8
				mov esi, dwItemDescription_Line2_Color
				mov dword ptr ds:[eax * 0x4 + iItemDescriptionText_Color], esi
				mov esi, dwItemDescription_Line2_Weight
				mov dword ptr ds:[eax * 0x4 + iItemDesctiptionText_Weight], esi
				inc eax
				mov dword ptr ds:[iItemDescriptionText_Line], eax
				// ----
				lea eax, [eax * 0x4 + eax]
				push cItemDecriptionText_Line3
				lea edx, [eax * 0x4 + eax]
				lea eax, [edx * 0x4 + iItemDescriptionText_Point]
				push eax
				mov esi, iItemDescriptionText_Crypt
				call esi
				mov eax, dword ptr ds:[iItemDescriptionText_Line]
				add esp, 0x8
				mov esi, dwItemDescription_Line3_Color
				mov dword ptr ds:[eax * 0x4 + iItemDescriptionText_Color], esi
				mov esi, dwItemDescription_Line3_Weight
				mov dword ptr ds:[eax * 0x4 + iItemDesctiptionText_Weight], esi
				inc eax
				mov dword ptr ds:[iItemDescriptionText_Line], eax
				// ----
				mov esi, iItemDescriptionText_Next
				jmp esi
            }
        }
        else if( dwItemDescription_Line_Count == 2 )
        {
            _asm
            {
				lea eax, [eax * 0x4 + eax]
				push cItemDecriptionText_Line1
				lea edx, [eax * 0x4 + eax]
				lea eax, [edx * 0x4 + iItemDescriptionText_Point]
				push eax
				mov esi, iItemDescriptionText_Crypt
				call esi
				mov eax, dword ptr ds:[iItemDescriptionText_Line]
				add esp, 0x8
				mov esi, dwItemDescription_Line1_Color
				mov dword ptr ds:[eax * 0x4 + iItemDescriptionText_Color], esi
				mov esi, dwItemDescription_Line1_Weight
				mov dword ptr ds:[eax * 0x4 + iItemDesctiptionText_Weight], esi
				inc eax
				mov dword ptr ds:[iItemDescriptionText_Line], eax
				// ----
				lea eax, [eax * 0x4 + eax]
				push cItemDecriptionText_Line2
				lea edx, [eax * 0x4 + eax]
				lea eax, [edx * 0x4 + iItemDescriptionText_Point]
				push eax
				mov esi, iItemDescriptionText_Crypt
				call esi
				mov eax, dword ptr ds:[iItemDescriptionText_Line]
				add esp, 0x8
				mov esi, dwItemDescription_Line2_Color
				mov dword ptr ds:[eax * 0x4 + iItemDescriptionText_Color], esi
				mov esi, dwItemDescription_Line2_Weight
				mov dword ptr ds:[eax * 0x4 + iItemDesctiptionText_Weight], esi
				inc eax
				mov dword ptr ds:[iItemDescriptionText_Line], eax
				// ----
				mov esi, iItemDescriptionText_Next
				jmp esi
            }
        }
        else if( dwItemDescription_Line_Count == 1 )
        {
            _asm
            {
				lea eax, [eax * 0x4 + eax]
				push cItemDecriptionText_Line1
				lea edx, [eax * 0x4 + eax]
				lea eax, [edx * 0x4 + iItemDescriptionText_Point]
				push eax
				mov esi, iItemDescriptionText_Crypt
				call esi
				mov eax, dword ptr ds:[iItemDescriptionText_Line]
				add esp, 0x8
				mov esi, dwItemDescription_Line1_Color
				mov dword ptr ds:[eax * 0x4 + iItemDescriptionText_Color], esi
				mov esi, dwItemDescription_Line1_Weight
				mov dword ptr ds:[eax * 0x4 + iItemDesctiptionText_Weight], esi
				inc eax
				mov dword ptr ds:[iItemDescriptionText_Line], eax
				// ----
				mov esi, iItemDescriptionText_Next
				jmp esi
            }
        }
	}
}
//---------------------------------------------------------------------------
Naked (cItemDropBlock)
{
	bItemDropBlock_Set = false;
	// ----
	_asm
	{
		mov wItemDropBlock_Pointer, ax
	}
	// ----
	if( wItemDropBlock_Pointer >= ITEM(14,90) && wItemDropBlock_Pointer <= ITEM(14,101) )
	{
		bItemDropBlock_Set = true;
	}
#ifdef CUSTOM_WINGS
	else 
		if( wItemDropBlock_Pointer >= ITEM(12, 41) && wItemDropBlock_Pointer <= ITEM(12, 45) )
	{
		bItemDropBlock_Set = true;
	}
#endif
	/*else if( wItemDropBlock_Pointer >= ITEM(14, 105) && wItemDropBlock_Pointer <= ITEM(14, 109) )
	{
		bItemDropBlock_Set = true;
	}*/
	else if( wItemDropBlock_Pointer >= ITEM(14, 145) && wItemDropBlock_Pointer <= ITEM(14, 157) )
	{
		bItemDropBlock_Set = true;
	}
	// ----
	if( bItemDropBlock_Set == true )
	{
		_asm
		{
			mov esi, 0x006476BE
			jmp esi
		}
	}
	else
	{
		_asm
		{
			mov esi, 0x006476BA
			jmp esi
		}
	}
}
//---------------------------------------------------------------------------
Naked (cItemTitleColor)
{
	dwItemTitleColor_Set = ItemColor::White;
	// ----
	_asm
	{
		mov wItemTitleColor_Pointer, ax
	}
	// ----
	if ( wItemTitleColor_Pointer == ITEM(12, 15) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 90) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 91) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 92) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 93) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 100) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 95) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 96) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 97) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 98) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 99) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
///Scorpion Moblility
	else if ( wItemTitleColor_Pointer == ITEM(14, 89) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
#ifdef CUSTOM_WINGS
	else if( wItemTitleColor_Pointer == ITEM(12, 41))// && wItemTitleColor_Pointer <= ITEM(12, 45) )
	{
		dwItemTitleColor_Set =  ItemColor::Violet;
	}
		else if( wItemTitleColor_Pointer == ITEM(12, 42))// && wItemTitleColor_Pointer <= ITEM(12, 45) )
	{
		dwItemTitleColor_Set =  ItemColor::Violet;
	}
		else if( wItemTitleColor_Pointer == ITEM(12, 43))// && wItemTitleColor_Pointer <= ITEM(12, 45) )
	{
		dwItemTitleColor_Set =  ItemColor::Violet;
	}
			else if( wItemTitleColor_Pointer == ITEM(12, 44))// && wItemTitleColor_Pointer <= ITEM(12, 45) )
	{
		dwItemTitleColor_Set =  ItemColor::Violet;
	}
			else if( wItemTitleColor_Pointer == ITEM(12, 45))// && wItemTitleColor_Pointer <= ITEM(12, 45) )
	{
		dwItemTitleColor_Set =  ItemColor::Violet;
	}
#endif
	// ----
	else if ( wItemTitleColor_Pointer == ITEM(14, 102) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 103) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 104) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	//#ifdef CUSTOM_WINGS
	else if( wItemTitleColor_Pointer == ITEM(14, 150))// && wItemTitleColor_Pointer <= ITEM(14,156) )
	{
		dwItemTitleColor_Set =  ItemColor::DarkViolet;
	}
	else if( wItemTitleColor_Pointer == ITEM(14, 151))// && wItemTitleColor_Pointer <= ITEM(14,156) )
	{
		dwItemTitleColor_Set =  ItemColor::DarkViolet;
	}
	else if( wItemTitleColor_Pointer == ITEM(14, 152))// && wItemTitleColor_Pointer <= ITEM(14,156) )
	{
		dwItemTitleColor_Set =  ItemColor::DarkViolet;
	}
	else if( wItemTitleColor_Pointer == ITEM(14, 153))// && wItemTitleColor_Pointer <= ITEM(14,156) )
	{
		dwItemTitleColor_Set =  ItemColor::DarkViolet;
	}
	else if( wItemTitleColor_Pointer == ITEM(14, 154))// && wItemTitleColor_Pointer <= ITEM(14,156) )
	{
		dwItemTitleColor_Set =  ItemColor::DarkViolet;
	}
	else if( wItemTitleColor_Pointer == ITEM(14, 155))// && wItemTitleColor_Pointer <= ITEM(14,156) )
	{
		dwItemTitleColor_Set =  ItemColor::DarkViolet;
	}
	else if( wItemTitleColor_Pointer == ITEM(14, 156))// && wItemTitleColor_Pointer <= ITEM(14,156) )
	{
		dwItemTitleColor_Set =  ItemColor::DarkViolet;
	}
//#endif
	/*else if ( wItemTitleColor_Pointer == ITEM(14, 105) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 106) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
    else if ( wItemTitleColor_Pointer == ITEM(14, 107) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 108) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;

	}
	else if ( wItemTitleColor_Pointer == ITEM(14, 109) )
	{
		dwItemTitleColor_Set = ItemColor::Gold;
	}*/
	_asm
	{
		je ColorSet
		mov edx, iItemTitleColor_Next
		jmp edx
		// ----
		ColorSet:
		mov ecx, dwItemTitleColor_Set
		mov dword ptr ss:[esp+0x1c], ecx
		mov edx, iItemTitleColor_Stack
		jmp edx
	}
}
//---------------------------------------------------------------------------
//DWORD	dwJewelsPricesOnShop_Pointer;
//DWORD	dwJewelsPricesOnShop_Set;
static DWORD dwJewelsPricesOnShop_Pointer;
static DWORD dwJewelsPricesOnShop_Buffer;
static DWORD dwJewelsPricesOnShop_Set;
Naked (cItemPrice)
{
	dwJewelsPricesOnShop_Set = 0;
	// ----
	_asm
	{
		mov ecx, dword ptr ss:[ebp+0x8]
		movsx edx, word ptr ds:[ecx]
		mov dwJewelsPricesOnShop_Pointer, edx
	}
	// ----
  if( dwJewelsPricesOnShop_Pointer == ITEM(12,15)) // Jewel of Chaos
  {
    dwJewelsPricesOnShop_Set = 150000;
  }
  else 
	if( dwJewelsPricesOnShop_Pointer == ITEM(14,13)) // Jewel of Bless
	{
		dwJewelsPricesOnShop_Set = 1500000;
	}
  else if( dwJewelsPricesOnShop_Pointer == ITEM(14,14)) // Jewel of Soul
  {
    dwJewelsPricesOnShop_Set = 1000000;
  }
  else if( dwJewelsPricesOnShop_Pointer == ITEM(14,16)) // Jewel of Life
  {
    dwJewelsPricesOnShop_Set = 4000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,22)) // Jewel of Creation
  {
    dwJewelsPricesOnShop_Set = 3000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,31)) // Jewel of Guardian
  {
    dwJewelsPricesOnShop_Set = 2500000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,42)) // Jewel of Harmony
  {
    dwJewelsPricesOnShop_Set = 5000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,41)) // GAMESTONE
  {
    dwJewelsPricesOnShop_Set = 200000;                                                            
  }                                                                                        	
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,90)) //	Jewel Of Exellent                       	
  {                                                                                        	
    dwJewelsPricesOnShop_Set = 10000000;                                                            	
  }                                                                                        	
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,91)) //	Jewel Of Wings                          
  {                                                                                        	
    dwJewelsPricesOnShop_Set = 10000000;                                                            
  }                                                                                        		
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,92)) //	Jewel Of Luck	                          	
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,93)) //	Jewel Of Skill
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,100)) //	Jewel Of Level
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,95)) //	Jewel Of Evalution
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,96)) //	Jewel Of Ancent
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,97)) //	Jewel Of Option	
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,98)) //	Jewel Of PvP
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,99)) //	Jewel Of Mistic                                 	
  {                                                                                                 		
    dwJewelsPricesOnShop_Set = 10000000;                                                                   	
  }                                                                                                 
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,0)) // Wings of Elf
  {
    dwJewelsPricesOnShop_Set = 5000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,1)) // Wings of Heaven
  {
    dwJewelsPricesOnShop_Set = 5000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,2)) // Wings of Satan
  {
    dwJewelsPricesOnShop_Set = 5000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,3)) // Wings of Spirits
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,4)) // Wings of Soul
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,5)) // Wings of Dragon
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,6)) // Wings of Darkness
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }  
	else if( dwJewelsPricesOnShop_Pointer == ITEM(12,30)) // Bundled Jewel of Bless
  {
    dwJewelsPricesOnShop_Set = 15000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,31)) // Bundled Jewel of Soul
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,36)) //  Wings of Storm	                       
  {                                                                                      
    dwJewelsPricesOnShop_Set = 20000000;                                                          	 
  }                                                                                            
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,37)) // Wings of Space-Time                        	 
  {
    dwJewelsPricesOnShop_Set = 20000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,38)) // Wings of Illusion
  {
    dwJewelsPricesOnShop_Set = 20000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,39)) // Wings of Doom
  {
    dwJewelsPricesOnShop_Set = 20000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,40)) // Mantle of Monarch
  {
    dwJewelsPricesOnShop_Set = 20000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,41)) //  Wings of Angel                     	  
  {                                                                                      
    dwJewelsPricesOnShop_Set = 50000000;                                                          	 
  }                                                                                           
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,42)) // Wings of Power	                       	 
  {
    dwJewelsPricesOnShop_Set = 50000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,43)) // Wings of Butterfly
  {
    dwJewelsPricesOnShop_Set = 50000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,44)) // Wings of Dream
  {
    dwJewelsPricesOnShop_Set = 50000000;
  }
    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,45)) // Mantle of Darkness
  {
    dwJewelsPricesOnShop_Set = 50000000;
  }  
      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,4)) // Dark Horse                
  {                                                                                    
    dwJewelsPricesOnShop_Set = 3000000;
  }  
      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,5)) // Dark Raven
  {
    dwJewelsPricesOnShop_Set = 1500000;
  }  
      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,30)) // Cape of Lord
  {
    dwJewelsPricesOnShop_Set = 10000000;
  }  
      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,52)) // Flame of Condor
  {
    dwJewelsPricesOnShop_Set = 1000000;
  }  
      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,53)) // Feather of Condor
  {
    dwJewelsPricesOnShop_Set = 1000000;
  }  
	/*if( dwJewelsPricesOnShop_Pointer == ITEM(14, 13) ) 
	{
		dwJewelsPricesOnShop_Set = 9000000;
	}*/
#ifdef ANTI_ANCIENT_JEWEL
	else if (dwJewelsPricesOnShop_Pointer == ITEM(14, 102))
	{
		dwJewelsPricesOnShop_Set = 50000000;
	}
	else if (dwJewelsPricesOnShop_Pointer == ITEM(14, 103))
	{
		dwJewelsPricesOnShop_Set = 50000000;
	}
#endif
	//else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 99) )
	//{
	//	dwJewelsPricesOnShop_Set = 50000000;
	//}
	//else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 100) )
	//{
	//	dwJewelsPricesOnShop_Set = 50000000;
	//}
	//else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 101) )
	//{
	//	dwJewelsPricesOnShop_Set = 50000000;
	//}

	else
		if( dwJewelsPricesOnShop_Pointer == ITEM(14, 102) )
	{
		dwJewelsPricesOnShop_Set = 100000000;
	}
	else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 103) )
	{
		dwJewelsPricesOnShop_Set = 500000000;
	}
	else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 104) )
	{
		dwJewelsPricesOnShop_Set = 1000000000;
	}
	// ----
	static DWORD Offset01 = 0x0059A0D3;
	if( dwJewelsPricesOnShop_Set == 0 )
	{
		_asm
		{
			//mov edx, iItemPrice_Next
			//jmp edx
			MOV dwJewelsPricesOnShop_Buffer,iItemPrice_Next
			JMP dwJewelsPricesOnShop_Buffer
		}
		
	}
	else
	{
		_asm
		{
			/*mov eax, dwJewelsPricesOnShop_Set
			mov dword ptr ss:[ebp-0x8], eax
			mov dword ptr ss:[ebp-0x4], 0x0
			mov eax, iItemPrice_Stack
			jmp eax*/
			MOV EAX,dwJewelsPricesOnShop_Set
			MOV DWORD PTR SS:[EBP-0x8],EAX
			MOV DWORD PTR SS:[EBP-0x4],0x0
			//MOV dwJewelsPricesOnShop_Buffer,0x0059A0D3
			//JMP dwJewelsPricesOnShop_Buffer//EAX,0x0059A0D3
			// ----
			MOV EAX,iItemPrice_Stack
			JMP EAX

			//0059A4B0
			
				
		}
		/**(BYTE*)(0x0059A531 + 1) = 0x01;
		*(BYTE*)(0x007D2790 + 2) = 0x01;*/
	}
	

}
//---------------------------------------------------------------------------
void test()
{
	//if( dwJewelsPricesOnShop_Pointer == ITEM(12,15)) // Jewel of Chaos
 // {
 //   dwJewelsPricesOnShop_Set = 150000;
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }
 // else if( dwJewelsPricesOnShop_Pointer == ITEM(14,13)) // Jewel of Bless
 // {
 //   dwJewelsPricesOnShop_Set = 1500000;
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }
 // else if( dwJewelsPricesOnShop_Pointer == ITEM(14,14)) // Jewel of Soul
 // {
 //   dwJewelsPricesOnShop_Set = 1000000;
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }
 // else if( dwJewelsPricesOnShop_Pointer == ITEM(14,16)) // Jewel of Life
 // {
 //   dwJewelsPricesOnShop_Set = 4000000;
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }
 //   else if( dwJewelsPricesOnShop_Pointer == ITEM(14,22)) // Jewel of Creation
 // {
 //   dwJewelsPricesOnShop_Set = 3000000;
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }
 //   else if( dwJewelsPricesOnShop_Pointer == ITEM(14,31)) // Jewel of Guardian
 // {
 //   dwJewelsPricesOnShop_Set = 2500000;
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }
 //   else if( dwJewelsPricesOnShop_Pointer == ITEM(14,42)) // Jewel of Harmony
 // {
 //   dwJewelsPricesOnShop_Set = 5000000;
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }
 //   else if( dwJewelsPricesOnShop_Pointer == ITEM(14,41)) // GAMESTONE
 // {
 //   dwJewelsPricesOnShop_Set = 200000;  
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }                                                                                        	
 //   else if( dwJewelsPricesOnShop_Pointer == ITEM(14,90)) //	Jewel Of Exellent                       	
 // {                                                                                        	
 //   dwJewelsPricesOnShop_Set = 10000000;  
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;
 // }                                                                                        	
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,91)) //	Jewel Of Wings                          
//  {                                                                                        	
//    dwJewelsPricesOnShop_Set = 10000000;                                                            
//  }                                                                                        		
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,92)) //	Jewel Of Luck	                          	
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,93)) //	Jewel Of Skill
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,94)) //	Jewel Of Level
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,95)) //	Jewel Of Evalution
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,96)) //	Jewel Of Ancent
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,97)) //	Jewel Of Option	
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,98)) //	Jewel Of PvP
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(14,99)) //	Jewel Of Mistic                                 	
//  {                                                                                                 		
//    dwJewelsPricesOnShop_Set = 10000000;                                                                   	
//  }                                                                                                 
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,0)) // Wings of Elf
//  {
//    dwJewelsPricesOnShop_Set = 5000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,1)) // Wings of Heaven
//  {
//    dwJewelsPricesOnShop_Set = 5000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,2)) // Wings of Satan
//  {
//    dwJewelsPricesOnShop_Set = 5000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,3)) // Wings of Spirits
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,4)) // Wings of Soul
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,5)) // Wings of Dragon
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,6)) // Wings of Darkness
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }  
//	else if( dwJewelsPricesOnShop_Pointer == ITEM(12,30)) // Bundled Jewel of Bless
//  {
//    dwJewelsPricesOnShop_Set = 15000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,31)) // Bundled Jewel of Soul
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,36)) //  Wings of Storm	                       
//  {                                                                                      
//    dwJewelsPricesOnShop_Set = 20000000;                                                          	 
//  }                                                                                            
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,37)) // Wings of Space-Time                        	 
//  {
//    dwJewelsPricesOnShop_Set = 20000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,38)) // Wings of Illusion
//  {
//    dwJewelsPricesOnShop_Set = 20000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,39)) // Wings of Doom
//  {
//    dwJewelsPricesOnShop_Set = 20000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,40)) // Mantle of Monarch
//  {
//    dwJewelsPricesOnShop_Set = 20000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,41)) //  Wings of Angel                     	  
//  {                                                                                      
//    dwJewelsPricesOnShop_Set = 50000000;                                                          	 
//  }                                                                                           
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,42)) // Wings of Power	                       	 
//  {
//    dwJewelsPricesOnShop_Set = 50000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,43)) // Wings of Butterfly
//  {
//    dwJewelsPricesOnShop_Set = 50000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,44)) // Wings of Dream
//  {
//    dwJewelsPricesOnShop_Set = 50000000;
//  }
//    else if( dwJewelsPricesOnShop_Pointer == ITEM(12,45)) // Mantle of Darkness
//  {
//    dwJewelsPricesOnShop_Set = 50000000;
//  }  
//      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,4)) // Dark Horse                
//  {                                                                                    
//    dwJewelsPricesOnShop_Set = 3000000;
//  }  
//      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,5)) // Dark Raven
//  {
//    dwJewelsPricesOnShop_Set = 1500000;
//  }  
//      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,30)) // Cape of Lord
//  {
//    dwJewelsPricesOnShop_Set = 10000000;
//  }  
//      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,52)) // Flame of Condor
//  {
//    dwJewelsPricesOnShop_Set = 1000000;
//  }  
//      else if( dwJewelsPricesOnShop_Pointer == ITEM(13,53)) // Feather of Condor
//  {
//    dwJewelsPricesOnShop_Set = 1000000;
//  }  
//	/*if( dwJewelsPricesOnShop_Pointer == ITEM(14, 13) ) 
//	{
//		dwJewelsPricesOnShop_Set = 9000000;
//	}*/
//#ifdef ANTI_ANCIENT_JEWEL
//	else if (dwJewelsPricesOnShop_Pointer == ITEM(14, 102))
//	{
//		dwJewelsPricesOnShop_Set = 50000000;
//	}
//	else if (dwJewelsPricesOnShop_Pointer == ITEM(14, 103))
//	{
//		dwJewelsPricesOnShop_Set = 50000000;
//	}
//#endif
//	//else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 99) )
//	//{
//	//	dwJewelsPricesOnShop_Set = 50000000;
//	//}
//	//else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 100) )
//	//{
//	//	dwJewelsPricesOnShop_Set = 50000000;
//	//}
//	//else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 101) )
//	//{
//	//	dwJewelsPricesOnShop_Set = 50000000;
//	//}

	//else 
//_asm
//	{
//		mov ecx, dword ptr ss:[ebp+0x8]
//		movsx edx, word ptr ds:[ecx]
//		mov dwJewelsPricesOnShop_Pointer, edx
//	}
//
//		if( dwJewelsPricesOnShop_Pointer == ITEM(14, 102) )
//	{
//		dwJewelsPricesOnShop_Set = 100000000;
//		*(BYTE*)(0x0059A531 + 1) = 0x01;
//	*(BYTE*)(0x007D2790 + 2) = 0x01;
//	}
//	else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 103) )
//	{
//		dwJewelsPricesOnShop_Set = 500000000;
//		*(BYTE*)(0x0059A531 + 1) = 0x01;
//	*(BYTE*)(0x007D2790 + 2) = 0x01;
//	}
//	else if( dwJewelsPricesOnShop_Pointer == ITEM(14, 104) )
//	{
//		dwJewelsPricesOnShop_Set = 1000000000;
//		*(BYTE*)(0x0059A531 + 1) = 0x01;
//	*(BYTE*)(0x007D2790 + 2) = 0x01;
//	}
	/*if(ITEM(14, 104) && ITEM(14, 103) && (14, 102))
	{
		*(BYTE*)(0x0059A531 + 1) = 0x01;
		*(BYTE*)(0x007D2790 + 2) = 0x01;
	}
*/
	

}
Naked(AllowInsertToItem2)
{
	_asm
	{
		CMP SI, 0x1C2B
		JE Return

			/*CMP SI, ITEM(14, 102)
			JE Return

			CMP SI, ITEM(14, 103)
			JE Return*/

			CMP SI, ITEM(14, 90)
			JE Return
			CMP SI, ITEM(14, 91)
			JE Return
			CMP SI, ITEM(14, 92)
			JE Return
			CMP SI, ITEM(14, 93)
			JE Return
			CMP SI, ITEM(14, 100)
			JE Return
			CMP SI, ITEM(14, 95)
			JE Return
			CMP SI, ITEM(14, 96)
			JE Return
			CMP SI, ITEM(14, 97)
			JE Return
			CMP SI, ITEM(14, 98)
			JE Return
			CMP SI, ITEM(14, 99)
			JE Return

			Return :
			MOV ECX, 0x0064684A
			JMP ECX
	}

}
//---------------------------------------------------------------------------
Naked(AllowInsertToItem)
{
	_asm
	{
		CMP SI, 0x1C2B
		JE Return

			/*CMP SI, ITEM(14, 102)
			JE Return

			CMP SI, ITEM(14, 103)
			JE Return*/

			CMP SI, ITEM(14, 90)
			JE Return
			CMP SI, ITEM(14, 91)
			JE Return
			CMP SI, ITEM(14, 92)
			JE Return
			CMP SI, ITEM(14, 93)
			JE Return
			CMP SI, ITEM(14, 100)
			JE Return
			CMP SI, ITEM(14, 95)
			JE Return
			CMP SI, ITEM(14, 96)
			JE Return
			CMP SI, ITEM(14, 97)
			JE Return
			CMP SI, ITEM(14, 98)
			JE Return
			CMP SI, ITEM(14, 99)
			JE Return

			Return :
		    MOV ECX, 0x006331A3
			JMP ECX
	}

}
//---------------------------------------------------------------------------
void __declspec(naked) CustomWingAllowJewel()
{
	static DWORD WingAllowJewel_Address01 = 0x006468A2;
	static DWORD WingAllowJewel_Address02 = 0x0064689C;

	_asm
	{
		CMP ECX, 0x1828
		JLE EXIT1

		//CMP ECX, 0x1С63//
		//JLE EXIT1
		//CMP ECX, 0x1829
		//JL EXIT2
		//CMP ECX, 0x1864
		//JLE EXIT1

		CMP ECX, 0x1829
		JL EXIT2
		CMP ECX, 0x1864
		JLE EXIT1

		
		JMP[WingAllowJewel_Address02]

		EXIT1:
		JMP[WingAllowJewel_Address01]

		EXIT2 :
		JMP[WingAllowJewel_Address02]
	}

}
//---------------------------------------------------------------------------
void __declspec(naked) CustomWingMoveIcarus01()
{

	static DWORD WingMoveIcarus01_Address01 = 0x0046822F;
	static DWORD WingMoveIcarus01_Address02 = 0x004681E3;

	_asm
	{
		MOV EDX, DWORD PTR SS : [EBP - 0x60]
		MOVSX EAX, WORD PTR DS : [EDX]
		CMP EAX, 0x1828
		JLE EXIT1
		CMP EAX, 0x1829
		JL EXIT2
		CMP EAX, 0x1864
		JLE EXIT1
		JMP[WingMoveIcarus01_Address02]
		EXIT1:
		JMP[WingMoveIcarus01_Address01]
		EXIT2 :
		JMP[WingMoveIcarus01_Address02]
	}
}

void __declspec(naked) CustomWingMoveIcarus02()
{
	static DWORD WingMoveIcarus02_Address01 = 0x004689F7;
	static DWORD WingMoveIcarus02_Address02 = 0x004689AB;

	_asm
	{
		MOV ECX, DWORD PTR SS : [EBP - 0x78]
		MOVSX EDX, WORD PTR DS : [ECX]
		CMP EDX, 0x1828
		JLE EXIT1

		CMP EDX, 0x1829
		JL EXIT2
		CMP EDX, 0x1864
		JLE EXIT1

		JMP[WingMoveIcarus02_Address02]

		EXIT1:
		JMP[WingMoveIcarus02_Address01]

		EXIT2 :
		JMP[WingMoveIcarus02_Address02]
	}
}
// ----------------------------------------------------------------------------------------------
bool IsCustomWings(WORD ItemID, bool Preview)
{
	// ----
	if( Preview )
	{
		ItemID -= ItemStartIndex;
	}
	// ----
	if( ItemID >= ITEM(12, 41) && ItemID <= ITEM(12, 46) )
	{
		return true;
	}
	// ----
	return false;
}
void gObjItemConvert(ObjectItem * lpItem, BYTE Option, BYTE Special, BYTE Value)
{
	ItemConvert(lpItem, Option, Special, Value);
	// ----
	//int ItemAttr		= *(DWORD*)0x78CB064 + 84 * lpItem->m_Type; //item.bmd reader
	int SpecialValue	= GET_ITEMOPT0(Option) + 4 * (GET_ITEMOPT16(Special) >> 6);	
	// ----
	//lpItem->ReqLevel	= *(short*)(ItemAttr + 58);
	// ----
	if( IsCustomWings(lpItem->m_Type, false) )
          {
		//if( Special & MAX_ITEMOPT_BIT >= 10 && Special & MAX_ITEMOPT_BIT <= 15 )
		//{
		//	lpItem->Unknown17 += 1;
		//}
		// ----
		if( GET_ITEMOPT_0(Special) ) //-> Set IgnorOpDefense
		{
			lpItem->SpecialValue[lpItem->SpecialCount]	= 10;
			lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::IncreaseDamage;//IgnorOpDefense;
			lpItem->SpecialCount++;
		}
		// ----
		if( GET_ITEMOPT_1(Special) ) //-> Set ReturnAttack
		{
			lpItem->SpecialValue[lpItem->SpecialCount]	= 13;
			lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::ReturnAttack;
			lpItem->SpecialCount++;
		}
		// ----
		if( GET_ITEMOPT_2(Special) ) //-> Set CompleteLife
		{
			lpItem->SpecialValue[lpItem->SpecialCount]	= 7;
			lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::CompleteLife;
			lpItem->SpecialCount++;
		}
		// ----
		if( GET_ITEMOPT_3(Special) ) //-> Set CompleteMana
		{
			lpItem->SpecialValue[lpItem->SpecialCount]	= 7;
			lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::CompleteMana;
			lpItem->SpecialCount++;
		}
		// ----
		if( GET_ITEMOPT_LUCK(Option) ) //-> Add luck option
		{
			lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::Luck;
			lpItem->SpecialCount++;
		}
		// ----
		if( SpecialValue < 1 )
		{
			return;
		}
		// ----
		switch(lpItem->m_Type) //-> Set add option
		{
		case ITEM(12, 41):	//-> DW
			{
				if( GET_ITEMOPT_4(Special) )
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= 4 * SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::WizardryDamage;
					lpItem->SpecialCount++;
				}
				//else 
					if( GET_ITEMOPT_5(Special) )
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= 5 * SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::Defense;
					lpItem->SpecialCount++;
				}
				else
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::HPRecovery;
					lpItem->SpecialCount++;
				}
			}
			break;
			// --
		case ITEM(12, 44):	//-> MG
			{
				if( GET_ITEMOPT_4(Special) )
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= 4 * SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::Damage;
					lpItem->SpecialCount++;
				}
				//else
				if( GET_ITEMOPT_5(Special) )
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= 5 * SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::WizardryDamage;
					lpItem->SpecialCount++;
				}
				else
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::HPRecovery;
					lpItem->SpecialCount++;
				}
			}
			break;
			// --
		case ITEM(12, 42):	//-> DK
		case ITEM(12, 43):	//-> ELF
		case ITEM(12, 45):	//-> DL
		case ITEM(12, 46):
			{
				if( GET_ITEMOPT_4(Special) )
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= 4 * SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::Damage;
					lpItem->SpecialCount++;
				}
				//else 
					if( GET_ITEMOPT_5(Special) )
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= 5 * SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::Defense;
					lpItem->SpecialCount++;
				}
				else
				{
					lpItem->SpecialValue[lpItem->SpecialCount]	= SpecialValue;
					lpItem->SpecialType[lpItem->SpecialCount]	= eItemOption::HPRecovery;
					lpItem->SpecialCount++;
				}
			}
			break;
			// --
		}
	}


}
DWORD   dwItem;
Naked(AddEffecrToBK1)
{
	/*_asm//white
	{

		MOV EAX, DWORD PTR SS : [EBP + 0x10]
		MOV dwItem, EAX


		CMP dwItem, 0x0E8A
		JE Return
	    CMP dwItem, ITEM2(12, 42) 
	    JE Return



		Return :
	  MOV ECX, 0x00690A2A
	JMP ECX
	}*/
	_asm//red
	{

			CMP ECX, 0x1EBA
			JE Return

			CMP ECX,ITEM2(12, 42) 
			JE Return

			/*CMP ECX,ITEM2(12, 41) 
			JE Return
			CMP ECX,ITEM2(12, 44) 
			JE Return*/

		Return:
		MOV ECX, 0x006945C6
		JMP ECX
	}

	//_asm//invisible
	//{


	//	CMP ECX, 0x1A93
	//	JE Return

	//	CMP ECX,ITEM2(12, 42) 
	//		JE Return

	//		Return :
	//	MOV ECX, 0x006948A6
	//		JMP ECX
	//}



}

Naked(AddEffecrToElf)
{
	_asm
	{


		CMP ECX, 0x22EC
		JE Return

			CMP ECX, ITEM2(12, 43)
			JE Return




			Return :
		MOV ECX, 0x006952F1
			JMP ECX
	}
}
Naked(WingsAddEffectInvis)
{
    _asm
    {
        MOV EAX, DWORD PTR SS:[EBP+0x10]
        MOV dwItem, EAX

        CMP dwItem,ITEM2(12,43) //
        JE Return
		 CMP dwItem,ITEM2(12,36) //
        JE Return


        Return:
        MOV ECX,0x0068CED9
        JMP ECX
    }
}
Naked(CapeAddNormalState)
{
	_asm
	{
	/*MOV DWORD PTR SS : [EBP - 308], EDX
	CMP DWORD PTR SS : [EBP - 308], ITEM2(12, 40)
	JE Return*/

	MOV DWORD PTR SS : [EBP - 308], EDX
	CMP DWORD PTR SS : [EBP - 308], ITEM2(12, 45)
	JE Return


	Return :
	MOV ECX, 0x0053947A
		JMP ECX
	}

}
void AddEffectWings(int ItemID, int ObjectID, int a3, GLfloat alpha, int a5, int a6, int a7, int a8, int a9)
{
	pAddItemEffect(ItemID, ObjectID,a3, alpha,a5,a6,a7, a8,a9);
}
#define ITEMGET(type, index)		(type*512 + index)
void __declspec(naked) CustomItemJewelAllow01()
{
	static DWORD ItemJewelAllow01_Address01 = 0x006331AC;
	static DWORD ItemJewelAllow01_Address02 = 0x006331C3;

	_asm
	{
		CMP SI, ITEMGET(14, 90)
		JL NEXT
		CMP SI, ITEMGET(14, 100)
		JLE EXIT

		NEXT :
		CMP SI, 0x1C2C
		JE EXIT

		JMP[ItemJewelAllow01_Address01]

		EXIT :
		JMP[ItemJewelAllow01_Address02]
	}
}

void __declspec(naked) CustomItemJewelAllow02()
{
	static DWORD ItemJewelAllow02_Address01 = 0x0064683F;
	static DWORD ItemJewelAllow02_Address02 = 0x0064684A;


	_asm
	{
		CMP DI, ITEMGET(14, 90)
		JL NEXT
		CMP DI, ITEMGET(14, 100)
		JLE EXIT

		NEXT :
		CMP DI, 0x1C2B
		JE EXIT

		JMP[ItemJewelAllow02_Address01]

		EXIT :
		JMP[ItemJewelAllow02_Address02]
	}
}

void __declspec(naked) CustomItemJewelAllow03()
{
	static DWORD ItemJewelAllow03_Address01 = 0x0064688C;
	static DWORD ItemJewelAllow03_Address02 = 0x006468A2;

	_asm
	{
		MOV EDI, DWORD PTR DS : [0x79F5AB8]

		CMP DI, ITEMGET(14, 90)
		JL NEXT
		CMP DI, ITEMGET(14, 100)
		JLE EXIT

		NEXT:
		CMP ECX, ITEMGET(12, 41)
		JL NEXT1
		CMP ECX, ITEMGET(12, 46)
		JLE EXIT

		NEXT1 :
		CMP ECX, 0x1A1E
		JE EXIT
		JMP[ItemJewelAllow03_Address01]

		EXIT :
		JMP[ItemJewelAllow03_Address02]
	}
}
//---------------------------------------------------------------------------
void cItemPropertiesLoad()
{
	// ----
	/*SetRange((LPVOID)0x00690A23, 7, ASM::NOP);
	SetJmp((LPVOID)0x00690A23, AddEffecrToBK1);*///white
	SetRange((LPVOID)0x006945C0, 6, ASM::NOP);
	SetJmp((LPVOID)0x006945C0, AddEffecrToBK1);//red
	//SetRange((LPVOID)0x006948A0, 6, ASM::NOP);
	//SetJmp((LPVOID)0x006948A0, AddEffecrToBK1);//invisible

	//SetRange((LPVOID)0x006952EC, 5, ASM::NOP);
	//SetJmp((LPVOID)0x006952EC, AddEffecrToElf);//perelivaetsa vnutrenn99

	SetRange((LPVOID)0x0068CED2, 7,ASM::NOP);//polnosty prozra4nie
    SetJmp((LPVOID)0x0068CED2, WingsAddEffectInvis);

	SetRange((LPVOID)0x0053946A, 16, ASM::NOP);
	SetJmp((LPVOID)0x0053946A, CapeAddNormalState);


	SetRange((LPVOID)iItemDescriptionText, 60, ASM::NOP);
	SetJmp((LPVOID)iItemDescriptionText, cItemDescriptionText);
	// ----
	SetRange((LPVOID)iItemDropBlock, 6, ASM::NOP);
	SetJmp((LPVOID)iItemDropBlock, cItemDropBlock);
	// ----
	SetRange((LPVOID)iItemTitleColor, 10, ASM::NOP);
	SetJmp((LPVOID)iItemTitleColor, cItemTitleColor);
	// ----
	//SetNop(0x005994BC, 33);
	//WriteJmp(0x005994BC, (DWORD)&cItemPrice);
	//test();
	////////------------------------------------------------------------------------
	//////// Change Price % 
	////////------------------------------------------------------------------------
	////*(BYTE*)(0x006475AD + 2) = 0x3D;
	//*(BYTE*)(0x0059A531 + 1) = 0x01;
	//*(BYTE*)(0x007D2790 + 2) = 0x01;


	// ----
	/*SetRange((LPVOID)0x0063319E, 5, ASM::NOP);
	SetJmp((LPVOID)0x0063319E, AllowInsertToItem);
	SetRange((LPVOID)0x00646838, 5, ASM::NOP);
	SetJmp((LPVOID)0x00646838, AllowInsertToItem2);*/
	SetOp((LPVOID)0x006331A5, (LPVOID)CustomItemJewelAllow01, ASM::JMP);
	SetOp((LPVOID)0x00646838, (LPVOID)CustomItemJewelAllow02, ASM::JMP);
	SetOp((LPVOID)0x00646884, (LPVOID)CustomItemJewelAllow03, ASM::JMP);
	// ----
	BYTE WingsHideExcellentGlow[] = { 0x81, 0xF9,0xA8 };
	WriteMemory( (PVOID) 0x0069806D, & WingsHideExcellentGlow, sizeof( WingsHideExcellentGlow ) );
	// ----
	BYTE WingsHideLeveltGlow[] = { 0xA8 };
	WriteMemory( (PVOID) 0x00693EB0,& WingsHideLeveltGlow, sizeof( WingsHideLeveltGlow ) );
	// ----
	BYTE WingsHideExcellentDropName[] = { 0xA8 };
	WriteMemory( (PVOID) 0x0061F4CF,& WingsHideLeveltGlow, sizeof( WingsHideLeveltGlow ) );
	// ----	
	BYTE WingsAddDurablity[] = { 0x2D };
	WriteMemory( (PVOID) 0x00619DEF,& WingsAddDurablity, sizeof( WingsAddDurablity ) );
	// ----	
	BYTE WingsAddOption[] = { 0x2D };
	WriteMemory( (PVOID) 0x00619551,& WingsAddOption, sizeof( WingsAddOption ) );
	// ----	
	BYTE RemoveNameExe[] = { 0x2D };
	WriteMemory( (PVOID) 0x00616AEE,& RemoveNameExe, sizeof( RemoveNameExe ) );
	// ----		
	BYTE GoldenNameWings[] = { 0x2D };
	WriteMemory( (PVOID) 0x00615433,& GoldenNameWings, sizeof( GoldenNameWings ) );
	// ----		
	BYTE AllowEnterKanturuEvent[] = { 0x2D };
	WriteMemory((PVOID)0x00434F61, &AllowEnterKanturuEvent, sizeof(AllowEnterKanturuEvent));

	SetOp((LPVOID)0x004681D6, (LPVOID)CustomWingMoveIcarus01, ASM::JMP);
	SetOp((LPVOID)0x0046899D, (LPVOID)CustomWingMoveIcarus02, ASM::JMP);
	// ----	
	SetOp((LPVOID)0x00646894, (LPVOID)CustomWingAllowJewel, ASM::JMP);
	// ----	
	SetHook((LPINT)gObjItemConvert, (LPVOID)0x005968D5, ASM::CALL);
	SetHook((LPINT)gObjItemConvert, (LPVOID)0x006202DA, ASM::CALL);

	SetHook((LPINT)gObjItemConvert, (LPVOID)0x00620956, ASM::CALL);
	SetHook((LPINT)gObjItemConvert, (LPVOID)0x00620E8E, ASM::CALL);

	SetHook((LPINT)gObjItemConvert, (LPVOID)0x006212C6, ASM::CALL);
	SetHook((LPINT)gObjItemConvert, (LPVOID)0x00621705, ASM::CALL);

	SetHook((LPINT)gObjItemConvert, (LPVOID)0x00671743, ASM::CALL);
	SetHook((LPINT)gObjItemConvert, (LPVOID)0x00713257, ASM::CALL);


	//SetHook((LPVOID)AddEffectWings, (LPVOID)0x00530DF8, ASM::CALL);
	//SetHook((LPVOID)AddEffectWings, (LPVOID)0x00698F6D, ASM::CALL);
}
