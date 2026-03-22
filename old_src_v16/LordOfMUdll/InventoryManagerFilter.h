#ifndef __InventoryManagerFilter_H
#define __InventoryManagerFilter_H

#pragma once


#include "Proxy.h"
#include "PacketFilter.h"
#include <map>
#include <set>


/**  
 * \brief 
 */
class CInventoryManagerFilter
	: public CPacketFilter
{
	struct CItemInfo
	{
	public:
		CItemInfo(){ wCode = 0; memset(vbData, 0, sizeof(vbData)); }
		CItemInfo(const CItemInfo& in)
		{ 
			wCode = in.wCode; 
			memcpy(vbData, in.vbData, sizeof(vbData)); 
		}

		CItemInfo(WORD code, BYTE* pData)
		{ 
			wCode = code; 
			memset(vbData, 0, sizeof(vbData)); 

			if (pData)
				memcpy(vbData, pData, sizeof(vbData)); 
		}

	private:
		void operator=(const CItemInfo& in){}

	public:
		WORD wCode;
		BYTE vbData[7];
	};

public:
	CInventoryManagerFilter(CProxy* pProxy);
	virtual ~CInventoryManagerFilter();

public:
	// Filter Interface Methods
	virtual const char* GetName(){ return "InventoryManagerFilter"; }
	virtual WORD GetLevel(){ return FILTER_LEVEL_CLICKER; }
	virtual int FilterRecvPacket(CPacket& pkt, CFilterContext& context);
	virtual int FilterSendPacket(CPacket& pkt, CFilterContext& context);

	// Parameter interface
	virtual bool GetParam(const char* pszParam, void* pData);
	virtual bool SetParam(const char* pszParam, void* pData);

protected:
	int FindItem(WORD wCode, WORD wMask);
	void DropItem(BYTE pos);
	void UseItem(BYTE pos);

private:
	std::map<BYTE,CItemInfo> m_vInventory;
};


#endif //__InventoryManagerFilter_H