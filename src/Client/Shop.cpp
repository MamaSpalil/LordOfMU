#include "stdafx.h"
#include "Shop.h"

//#include "Item.h"
#include "ItemValue.h"
#include "TMemory.h"

SHOP_INFO gShopPrice[MAX_ITEM_SHOP_PRICE];

DWORD ShopValueType=0;

void InitShop() // OK
{
	SetCompleteHook(0xE8,0x006154C0,&GetBuyPrice);//6154C0

	SetCompleteHook(0xE8,0x0061558F,&GetBuyPrice);//61558F

	SetCompleteHook(0xE8,0x006155D5,&GetSellPrice);//6155D5

	//SetCompleteHook(0xFF,0x006155C5,&ShowBuyPrice);//6155C5
}

int GetBuyPrice(DWORD address) // OK
{
	if(ShopValueType != 0)
	{
		for(int n=0;n<MAX_ITEM_SHOP_PRICE;n++)
		{
			if(gShopPrice[n].ItemIndex != *(WORD*)(address+0x00))
			{
				continue;
			}

			if(gShopPrice[n].ItemLevel != GET_ITEM_OPT_LEVEL(*(DWORD*)(address+0x04)))
			{
				continue;
			}

			if(gShopPrice[n].ItemDur != *(BYTE*)(address+0x1A))
			{
				continue;
			}

			if(gShopPrice[n].ItemNewOption != GET_ITEM_OPT_EXC(*(BYTE*)(address+0x1B)))
			{
				continue;
			}

			return gShopPrice[n].ItemPrice;
		}

		return 0;
	}

	int value = 0;

	if(gItemValue.GetItemValue(address,&value) != 0)
	{
		value = ((value>=100)?((value/10)*10):value);

		value = ((value>=1000)?((value/100)*100):value);

		return value;
	}

	return ((int(*)(DWORD,int))0x00599217)(address,0);
}

int GetSellPrice(DWORD address) // OK
{
	/*if(ShopValueType != 0)
	{
		for(int n=0;n<MAX_ITEM_SHOP_PRICE;n++)
		{
			if(gShopPrice[n].ItemIndex != *(WORD*)(address+0x00))
			{
				continue;
			}

			if(gShopPrice[n].ItemLevel != GET_ITEM_OPT_LEVEL(*(DWORD*)(address+0x04)))
			{
				continue;
			}

			if(gShopPrice[n].ItemDur != *(BYTE*)(address+0x1A))
			{
				continue;
			}

			if(gShopPrice[n].ItemNewOption != GET_ITEM_OPT_EXC(*(BYTE*)(address+0x1B)))
			{
				continue;
			}

			return gShopPrice[n].ItemPrice;
		}

		return 0;
	}*/
	int value1 = 0;

	if(gItemValue.GetItemValue1(address,&value1) != 0)
	{
		value1 = (value1);///3);

		value1 = ((value1>=100)?((value1/10)*10):value1);

		value1 = ((value1>=1000)?((value1/100)*100):value1);

		return value1;
	}

	return ((int(*)(DWORD,int))0x00599217)(address,1);//
}

void ShowBuyPrice(char* a,char* b,char* c) // OK
{
	if(ShopValueType==0)
	{
		wsprintf(a,b,c);
	}
	else
	{
		wsprintf(a,"TEST",c);
	}
}

void ClearShopPriceList() // OK
{
	for(int n=0;n < MAX_ITEM_SHOP_PRICE;n++)
	{
		gShopPrice[n].ItemIndex = -1;
		gShopPrice[n].ItemLevel = -1;
		gShopPrice[n].ItemDur = -1;
		gShopPrice[n].ItemNewOption = -1;
		gShopPrice[n].ItemPrice = -1;
	}
}

void InsertShopPrice(DWORD index,DWORD level,DWORD dur,DWORD exc,DWORD price) // OK
{
	for(int n=0;n < MAX_ITEM_SHOP_PRICE;n++)
	{
		if(gShopPrice[n].ItemIndex == -1)
		{
			gShopPrice[n].ItemIndex = index;
			gShopPrice[n].ItemLevel = level;
			gShopPrice[n].ItemDur = dur;
			gShopPrice[n].ItemNewOption = exc;
			gShopPrice[n].ItemPrice = price;
			return;
		}
	}
}