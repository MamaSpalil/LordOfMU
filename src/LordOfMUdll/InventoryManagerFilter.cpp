#include "stdafx.h"
#include "InventoryManagerFilter.h"
#include "CommonPackets.h"
#include "version.h"



/**  
 * \brief 
 */
CInventoryManagerFilter::CInventoryManagerFilter(CProxy* pProxy)
	: CPacketFilter(pProxy)
{
}


/**  
 * \brief 
 */
CInventoryManagerFilter::~CInventoryManagerFilter()
{

}


/**  
 * \brief 
 */
int CInventoryManagerFilter::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	if (pkt == CInitInventoryPacket::Type())
	{
		m_vInventory.clear();

		CInitInventoryPacket& pkt2 = (CInitInventoryPacket&)pkt;
		int iCount = pkt2.GetCount();
		
		for (int i=0; i < iCount; ++i)
		{
			BYTE bPos = pkt2.GetPosition(i);
			WORD wCode = pkt2.GetItemType(i);
			BYTE* pData = pkt2.GetItemData(i);

			m_vInventory.insert(std::pair<BYTE,CItemInfo>(bPos, CItemInfo(wCode, pData)));
		}
	}
	else if (pkt == CRemoveFromInventoryPacket::Type())
	{
		CRemoveFromInventoryPacket& pkt2 = (CRemoveFromInventoryPacket&)pkt;

		int iCount = pkt2.GetCount();
		for (int i=0; i < iCount; ++i)
		{
			BYTE bPos = pkt2.GetPosition(i);
			m_vInventory.erase(bPos);
		}
	}
	else if (pkt == CPutInventoryPacket::Type())
	{
		CPutInventoryPacket& pkt2 = (CPutInventoryPacket&)pkt;

		BYTE bPos = pkt2.GetInvPos();
		WORD wCode = pkt2.GetItemType();
		BYTE* pData = pkt2.GetItemData();
		
		m_vInventory.insert(std::pair<BYTE,CItemInfo>(bPos, CItemInfo(wCode, pData)));
	}
	else if (pkt == CMoveToInventoryPacket::Type())
	{
		CMoveToInventoryPacket& pkt2 = (CMoveToInventoryPacket&)pkt;

		BYTE bPos = pkt2.GetInvPos();
		WORD wCode = pkt2.GetItemType();
		BYTE* pData = pkt2.GetItemData();

		m_vInventory.insert(std::pair<BYTE,CItemInfo>(bPos, CItemInfo(wCode, pData)));
	}

	return 0;
}


/**  
 * \brief 
 */
int CInventoryManagerFilter::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	if (pkt == CMoveItemReqPacket::Type())
	{
		CMoveItemReqPacket& pkt2 = (CMoveItemReqPacket&)pkt;

		if (pkt2.GetSource() == CMoveItemReqPacket::PLACE_INVENTORY)
		{
			BYTE bPos = pkt2.GetInvPos();
			m_vInventory.erase(bPos);
		}
	}

	return 0;
}


/**  
 * \brief 
 */
bool CInventoryManagerFilter::GetParam(const char* pszParam, void* pData)
{
	if (!pszParam || !pData)
		return false;

	if (_stricmp(pszParam, "boots_level") == 0)
	{
		static const BYTE EQUIP_SLOT_BOOTS = 6;
		std::map<BYTE,CItemInfo>::iterator it = m_vInventory.find(EQUIP_SLOT_BOOTS);

		if (it != m_vInventory.end())
		{
			// Item level is stored in bits 3-6 of byte 1 of the 7-byte item data
			*((int*)pData) = (it->second.vbData[1] >> 3) & 0x0F;
		}
		else
		{
			// No boots equipped
			*((int*)pData) = -1;
		}

		return true;
	}

	return false;
}


/**  
 * \brief 
 */
bool CInventoryManagerFilter::SetParam(const char* pszParam, void* pData)
{
	if (_stricmp(pszParam, "throw") == 0)
	{
		DWORD dwData = *((DWORD*)pData);

		WORD wCode = LOWORD(dwData);
		WORD wMask = HIWORD(dwData);

		int iPos = FindItem(wCode, wMask);
		
		if (iPos == -1)
			return false;

		DropItem((BYTE)iPos);
	}
	else if (_stricmp(pszParam, "use") == 0)
	{
		DWORD dwData = *((DWORD*)pData);

		WORD wCode = LOWORD(dwData);
		WORD wMask = HIWORD(dwData);

		int iPos = FindItem(wCode, wMask);

		if (iPos == -1)
			return false;

		UseItem((BYTE)iPos);
	}
	else if (_stricmp(pszParam, "moveitem") == 0)
	{
		WORD wData = *((WORD*)pData);

		BYTE src = LOBYTE(wData);
		WORD dst = HIBYTE(wData);

		std::map<BYTE,CItemInfo>::iterator it = m_vInventory.find(src);

		if (it == m_vInventory.end())
			return false;

		if (dst >= 76)
			dst = MAKEWORD(dst, 4);

		GetProxy()->send_packet(CMoveItemReqPacket(src, dst, it->second.vbData));
		GetProxy()->recv_packet(CRemoveFromInventoryPacket(src));
	}
	else
		return false;

	return true;
}


/**  
 * \brief 
 */
int CInventoryManagerFilter::FindItem(WORD wCode, WORD wMask)
{
	for (std::map<BYTE,CItemInfo>::iterator it = m_vInventory.begin(); it != m_vInventory.end(); ++it)
	{
		WORD wItem = it->second.wCode;
		if (wCode == (wItem & 0x0FFF) && (wMask & (1 << (wItem >> 12))) != 0)
			return (int)it->first;
	}

	return -1;
}

/**
 * \brief 
 */
void CInventoryManagerFilter::DropItem(BYTE pos)
{
	CPacketFilter* pCharInfo = GetProxy()->GetFilter("CharInfoFilter");

	BYTE x = 0;
	BYTE y = 0;

	if (!pCharInfo 
		|| !pCharInfo->GetParam("X", &x)
		|| !pCharInfo->GetParam("Y", &y))
	{
		return;
	}

	CDropItemPacket pkt(x+1, y+1, pos);
	GetProxy()->send_packet(pkt);
}

/**
 * \brief 
 */
void CInventoryManagerFilter::UseItem(BYTE pos)
{
	CUseItemPacket pkt(pos);
	GetProxy()->send_packet(pkt);
}