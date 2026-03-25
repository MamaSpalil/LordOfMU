#pragma once

#define MAX_ITEM_SHOP_PRICE 120

struct SHOP_INFO
{
	DWORD ItemIndex;
	DWORD ItemLevel;
	DWORD ItemDur;
	DWORD ItemNewOption;
	DWORD ItemPrice;
};

void InitShop();
int GetBuyPrice(DWORD address);
int GetSellPrice(DWORD address);
void ShowBuyPrice(char* a, char* b, char* c);
void ClearShopPriceList();
void InsertShopPrice(DWORD index,DWORD level,DWORD dur,DWORD exc,DWORD price);

extern DWORD ShopValueType;