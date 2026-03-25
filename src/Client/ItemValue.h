// ItemValue.h: interface for the CItemValue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//#include "Item.h"

#define MAX_ITEM_VALUE_INFO 2000

struct ITEM_VALUE_INFO
{
	int Index;
	int ItemIndex;
	int Level;
	int Grade;
	int Value;
	int Value1;
};

class CItemValue
{
public:
	CItemValue();
	virtual ~CItemValue();
	void Init();
	void Load(ITEM_VALUE_INFO* info);
	void SetInfo(ITEM_VALUE_INFO info);
	ITEM_VALUE_INFO* GetInfo(int index);
	bool GetItemValue(DWORD address,int* value);
	bool GetItemValue1(DWORD address,int* value1);
private:
	ITEM_VALUE_INFO m_ItemValueInfo[MAX_ITEM_VALUE_INFO];
};

extern CItemValue gItemValue;
