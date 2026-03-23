#include "stdafx.h"
#include "AutoPickupFilter.h"
#include "CommonPackets.h"
#include "DebugOut.h"
#include "ClickerLogger.h"
#include "BufferUtil.h"
#include <math.h>

/**
 * \brief 
 */
CAutoPickupFilter::CAutoPickupFilter(CProxy* pProxy) 
	: CPacketFilter(pProxy)
{
	m_ulBlessFlags = 0;
	m_ulSoulFlags = 0;
	m_ulChaosFlags = 0;
	m_ulLifeFlags = 0;
	m_ulCreationFlags = 0;
	m_ulGuardianFlags = 0;
	m_ulExlFlags = 0;
	m_ulZenFlags = 0;
	m_ulCustomFlags = 0;
	m_iDist = PICKUP_DIST_DEFAULT;

	m_fEnabled = FALSE;
	m_fDisplayCode = FALSE;

	m_fSuspended = false;
	m_fSuspPick = false;
	m_fSuspZen = false;
	m_fSuspMove = false;
	m_fWalking = false;
	m_fDebugMoveTo = false;
	m_fRunMode = false;
	m_fAutoRunMode = false;
	m_bCharClass = CHAR_CLASS_UNSET;

	InitializeCriticalSection(&m_csQueue);

	m_hPickEvent = CreateEvent(0, 1, 0, 0);
	m_hStopEvent = CreateEvent(0, 1, 0, 0);
	m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PickThreadProc, this, 0, 0);
}


/**
 * \brief 
 */
CAutoPickupFilter::~CAutoPickupFilter()
{	
	if (m_hThread && m_hThread != INVALID_HANDLE_VALUE)
	{
		SetEvent(m_hStopEvent);

		Sleep(100);

		if (WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT)
			TerminateThread(m_hThread, 0);

		CloseHandle(m_hThread);
	}

	CloseHandle(m_hStopEvent);
	CloseHandle(m_hPickEvent);
	DeleteCriticalSection(&m_csQueue);
}


/**
 * \brief 
 */
int CAutoPickupFilter::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	if (pkt == CMeetItemPacket::Type())
	{
		CMeetItemPacket& pktMItem = (CMeetItemPacket&)pkt;

		if (!m_fEnabled)
		{
			WriteClickerLogFmt("PICKUP", "RECV CMeetItemPacket: %d item(s) dropped but autopick DISABLED (m_fEnabled=0) - items ignored",
				pktMItem.GetItemCount());
			CDebugOut::PrintAlways("[PICKUP] RECV CMeetItemPacket: %d item(s) but autopick DISABLED - items ignored",
				pktMItem.GetItemCount());
			return 0;
		}

		CStringA szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());
		WriteClickerLogFmt("PICKUP", "RECV CMeetItemPacket: %d item(s) dropped on ground | Len=%d | %s",
			pktMItem.GetItemCount(), pkt.GetDecryptedLen(), (const char*)szHex);
		CDebugOut::PrintAlways("[PICKUP] RECV CMeetItemPacket: %d item(s) | Len=%d | %s",
			pktMItem.GetItemCount(), pkt.GetDecryptedLen(), (const char*)szHex);

		if (m_fSuspended && m_fSuspPick)
		{
			WriteClickerLogFmt("PICKUP", "Item packet ignored: picking is suspended (suspended=%d, suspPick=%d)", (int)m_fSuspended, (int)m_fSuspPick);

			if (m_fDebugMoveTo)
				CDebugOut::PrintAlways("[MOVETO-DBG] Item packet ignored: picking is suspended (m_fSuspended=%d, m_fSuspPick=%d)", (int)m_fSuspended, (int)m_fSuspPick);
			return 0;
		}

		ULONG ulZenFlags = (m_fSuspended && m_fSuspZen) ? 0 : m_ulZenFlags;

		struct { WORD wType; ULONG* pFlags; const char* pszName; } _map[] =
			{
				{TYPE_BLESS, &m_ulBlessFlags, "Bless"}, {TYPE_SOUL, &m_ulSoulFlags, "Soul"}, {TYPE_CHAOS, &m_ulChaosFlags, "Chaos"},
				{TYPE_LIFE, &m_ulLifeFlags, "Life"}, {TYPE_JOC, &m_ulCreationFlags, "Creation"}, {TYPE_JOG, &m_ulGuardianFlags, "Guardian"},  
				{TYPE_ZEN, &ulZenFlags, "Zen"}
			};

		BYTE bPlX = 0;
		BYTE bPlY = 0;

		CPacketFilter* pFilter = GetProxy()->GetFilter("CharInfoFilter");
		if (pFilter)
		{
			pFilter->GetParam("X", (void*)&bPlX);
			pFilter->GetParam("Y", (void*)&bPlY);
		}
		else if (m_fDebugMoveTo)
		{
			CDebugOut::PrintAlways("[MOVETO-DBG] CharInfoFilter not found, player position unknown (0,0)");
		}

		if (m_fDebugMoveTo)
			CDebugOut::PrintAlways("[MOVETO-DBG] Processing %d item(s), player at (%d,%d), pdist=%d", 
				pktMItem.GetItemCount(), (int)bPlX, (int)bPlY, m_iDist);

		for (int i = pktMItem.GetItemCount()-1; i >= 0; --i)
		{
			BYTE x = 0, y = 0;
			pktMItem.GetItemPos(i, x, y);

			WORD wType = pktMItem.GetItemType(i);
			WORD wMask = 1 << (wType >> 12);
			WORD wId = pktMItem.GetItemId(i);

			wType &= 0x0FFF;

			int xdiff = abs((int)bPlX - (int)x);
			int ydiff = abs((int)bPlY - (int)y);

			std::map<WORD, WORD>::iterator it = m_vItemList.find(wType);

			if (it != m_vItemList.end() && (wMask & it->second) != 0)
			{
				bool fMoveTo = (m_ulCustomFlags & 2) != 0;
				bool fPickEnabled = (m_ulCustomFlags & 1) != 0;
				bool fInRange = (xdiff <= m_iDist && ydiff <= m_iDist);

				if (fMoveTo && fPickEnabled && fInRange)
				{
					// Move To mode: queue item for walking pickup within radius
					CAutoLockQueue autoCS(&m_csQueue);
					m_vPickQueue.insert((ULONG)wId | (ULONG)x << 16 | (ULONG)y << 24);
					SetEvent(m_hPickEvent);

					WriteClickerLogFmt("PICKUP", "Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for Move To pickup",
						wType, wId, (int)x, (int)y, xdiff, ydiff);

					if (m_fDebugMoveTo)
						CDebugOut::PrintAlways("[MOVETO-DBG] Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for Move To pickup", 
							wType, wId, (int)x, (int)y, xdiff, ydiff);
				}
				else if (fPickEnabled && fInRange)
				{
					// Immediate pickup: item is within range and pickup enabled
					WriteClickerLogFmt("PICKUP", "Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) picking up immediately",
						wType, wId, (int)x, (int)y, xdiff, ydiff);

					PickItem(wId);

					if (m_fDebugMoveTo)
						CDebugOut::PrintAlways("[MOVETO-DBG] Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) picked up immediately (no Move To)", 
							wType, wId, (int)x, (int)y, xdiff, ydiff);
				}
				else
				{
					WriteClickerLogFmt("PICKUP", "Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) SKIPPED: pick=%d moveTo=%d inRange=%d",
						wType, wId, (int)x, (int)y, xdiff, ydiff, (int)fPickEnabled, (int)fMoveTo, (int)fInRange);

					if (m_fDebugMoveTo)
					{
						CDebugOut::PrintAlways("[MOVETO-DBG] Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) SKIPPED: pickEnabled=%d, moveTo=%d, inRange=%d",
							wType, wId, (int)x, (int)y, xdiff, ydiff, (int)fPickEnabled, (int)fMoveTo, (int)fInRange);
					}
				}
			}
			else
			{
				BOOL fFound = FALSE;
				ULONG ulFlags = 0;
				const char* pszItemName = "Unknown";

				for (int j=0; j < sizeof(_map)/sizeof(_map[0]); j++)
				{
					if (wType == _map[j].wType)
					{
						ulFlags = *(_map[j].pFlags);
						pszItemName = _map[j].pszName;
						fFound = TRUE;
						break;
					}
				}


				BYTE* pItemCode = pktMItem.GetItemData(i) + 4;

				if (!fFound && (m_ulExlFlags & 1)
						&& ((pItemCode[3] & 0x3F) || (pItemCode[4] & 0x01)))
				{
					ulFlags = m_ulExlFlags;
					pszItemName = "Excellent";
					fFound = TRUE;
				}

				if (fFound && (ulFlags & 1))
				{
					if ((ulFlags & 2) != 0)
					{
						if (xdiff <= m_iDist && ydiff <= m_iDist)
						{
							// Move To mode: queue item for walking pickup if within range
							CAutoLockQueue autoCS(&m_csQueue);
							m_vPickQueue.insert((ULONG)wId | (ULONG)x << 16 | (ULONG)y << 24);
							SetEvent(m_hPickEvent);

							WriteClickerLogFmt("PICKUP", "%s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for Move To pickup",
								pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff);

							if (m_fDebugMoveTo)
								CDebugOut::PrintAlways("[MOVETO-DBG] %s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for Move To pickup",
									pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff);
						}
						else if (m_fDebugMoveTo)
						{
							CDebugOut::PrintAlways("[MOVETO-DBG] %s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) SKIPPED Move To: out of range (pdist=%d)",
								pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff, m_iDist);
						}
					}
					else if (xdiff <= m_iDist && ydiff <= m_iDist)
					{
						m_vNoMovePickQueue.push_back(CPickInfo(wId));

						WriteClickerLogFmt("PICKUP", "%s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for no-move pickup",
							pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff);

						if (m_fDebugMoveTo)
							CDebugOut::PrintAlways("[MOVETO-DBG] %s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for no-move pickup",
								pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff);
					}
					else if (m_fDebugMoveTo)
					{
						CDebugOut::PrintAlways("[MOVETO-DBG] %s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) SKIPPED no-move pickup: out of range (pdist=%d)",
							pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff, m_iDist);
					}
				}
				else if (m_fDebugMoveTo && fFound)
				{
					CDebugOut::PrintAlways("[MOVETO-DBG] %s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) SKIPPED: flags=0x%02X (pickup=%d, moveTo=%d)",
						pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff, ulFlags, (int)(ulFlags & 1), (int)((ulFlags & 2) != 0));
				}
				else if (m_fDebugMoveTo)
				{
					CDebugOut::PrintAlways("[MOVETO-DBG] Item 0x%04X (id=0x%04X) at (%d,%d) not in any pickup list, ignored",
						wType, wId, (int)x, (int)y);
				}
			}
		}
	}
	else if (pkt == CPutInventoryPacket::Type())
	{
		CPutInventoryPacket& pkt2 = (CPutInventoryPacket&)pkt;

		WORD wType = pkt2.GetItemType();
		WORD wMask = 1 << (wType >> 12);
		BYTE bPos = pkt2.GetInvPos();

		wType &= 0x0FFF;

		CStringA szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());
		WriteClickerLogFmt("PICKUP", "RECV CPutInventoryPacket: item type=0x%04X placed in inventory slot=%d | Len=%d | %s",
			wType, (int)bPos, pkt.GetDecryptedLen(), (const char*)szHex);
		CDebugOut::PrintAlways("[PICKUP] RECV CPutInventoryPacket: type=0x%04X slot=%d | Len=%d | %s",
			wType, (int)bPos, pkt.GetDecryptedLen(), (const char*)szHex);

		if (m_fEnabled)
		{
			if (m_fSuspended && m_fSuspPick)
				return 0;

			std::map<WORD, WORD>::iterator it = m_vDropList.find(wType);

			if (it != m_vDropList.end() && (wMask & it->second) != 0)
				m_vDropQueue.push_back(CDropInfo(bPos));
		}
		else if (m_fDisplayCode)
		{
			CServerMessagePacket pktMsg(">> Item code: %d %d %d", HIBYTE(wType) & 0x0F, LOBYTE(wType), HIBYTE(wType) >> 4);
			GetProxy()->recv_direct(pktMsg);
		}
	}
	else if (m_fDisplayCode && pkt == CMoveToInventoryPacket::Type())
	{
		CMoveToInventoryPacket& pkt2 = (CMoveToInventoryPacket&)pkt;

		WORD wType = pkt2.GetItemType();
		BYTE bPos = pkt2.GetInvPos();

		CServerMessagePacket pktMsg(">> Item code: %d %d %d", HIBYTE(wType) & 0x0F, LOBYTE(wType), HIBYTE(wType) >> 4);
		GetProxy()->recv_direct(pktMsg);
	}
	else if (pkt == CForgetItemPacket::Type())
	{
		// Items disappeared from the ground (picked by others, despawned, etc.)
		// Remove them from pickup queues to avoid picking non-existent items
		CForgetItemPacket& pktForget = (CForgetItemPacket&)pkt;
		int iCount = pktForget.GetItemCount();

		CStringA szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());
		WriteClickerLogFmt("PICKUP", "RECV CForgetItemPacket: %d item(s) disappeared from ground | Len=%d | %s",
			iCount, pkt.GetDecryptedLen(), (const char*)szHex);
		CDebugOut::PrintAlways("[PICKUP] RECV CForgetItemPacket: %d item(s) disappeared | Len=%d | %s",
			iCount, pkt.GetDecryptedLen(), (const char*)szHex);

		for (int i = 0; i < iCount; ++i)
		{
			WORD wId = pktForget.GetItemId(i);

			// Remove from move-to pickup queue
			{
				CAutoLockQueue autoCS(&m_csQueue);

				for (std::set<ULONG>::iterator it = m_vPickQueue.begin(); it != m_vPickQueue.end(); )
				{
					if (LOWORD(*it) == wId)
					{
						it = m_vPickQueue.erase(it);
					}
					else
					{
						++it;
					}
				}

				if (m_vPickQueue.empty())
					ResetEvent(m_hPickEvent);
			}

			// Remove from no-move pickup queue
			for (std::deque<CPickInfo>::iterator it = m_vNoMovePickQueue.begin(); it != m_vNoMovePickQueue.end(); )
			{
				if (it->wItemId == wId)
				{
					it = m_vNoMovePickQueue.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}
	else if (pkt == CWarpReplyPacket::Type() || pkt == CCharRespawnPacket::Type())
	{
		// Clear stale pickup queues on warp or respawn, but keep m_fEnabled
		// so auto-pickup continues working after warp/respawn
		CStringA szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());
		WriteClickerLogFmt("PICKUP", "RECV %s: clearing pickup queues | Len=%d | %s",
			(pkt == CWarpReplyPacket::Type()) ? "CWarpReplyPacket" : "CCharRespawnPacket",
			pkt.GetDecryptedLen(), (const char*)szHex);
		CDebugOut::PrintAlways("[PICKUP] RECV %s: clearing queues | Len=%d | %s",
			(pkt == CWarpReplyPacket::Type()) ? "CWarpReplyPacket" : "CCharRespawnPacket",
			pkt.GetDecryptedLen(), (const char*)szHex);
		{
			CAutoLockQueue autoCS(&m_csQueue);
			m_vPickQueue.clear();
			ResetEvent(m_hPickEvent);
		}
		m_vNoMovePickQueue.clear();
		m_vDropQueue.clear();
	}


	ProcessDropQueue();
	ProcessNoMovePickupQueue();
	return 0;
}


/**
 * \brief 
 */
int CAutoPickupFilter::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	if (m_fWalking)
	{
		// Allow injected packets (auto-pickup position updates & pick commands)
		// through the filter chain so they reach PacketEncryptFilter for proper
		// encryption.  Only block player-generated movement and attack packets.
		if (pkt.GetInjected())
		{
			if (m_fDebugMoveTo)
			{
				CDebugOut::PrintAlways("[MOVETO-DBG] FilterSendPacket: ALLOWED injected %s during walk",
					pkt.GetType().GetDescription());
			}
		}
		else if (pkt == CNormalAttackPacket::Type()
			|| pkt == CSingleSkillPacket::Type()
			|| pkt == CMassiveSkillPacket::Type()
			|| pkt == CCharMoveCTSPacket::Type()
			|| pkt == CUpdatePosCTSPacket::Type())
		{
			// Notify PacketEncryptFilter about blocked C3/C4 packets so it can
			// adjust its sequence counter.  Without this, blocked encrypted
			// packets cause a counter desync that leads to server disconnect.
			if (pkt.GetPktClass() == 0xC3 || pkt.GetPktClass() == 0xC4)
			{
				CPacketFilter* pEncrypt = GetProxy()->GetFilter("PacketEncryptFilter");
				if (pEncrypt)
					pEncrypt->SetParam("BlockedC3Packet", NULL);
			}

			WriteClickerLogFmt("PICKUP", "FilterSendPacket: BLOCKED %s during auto-pickup walk",
				pkt.GetType().GetDescription());
			CDebugOut::PrintAlways("[PICKUP] FilterSendPacket: BLOCKED %s during walk",
				pkt.GetType().GetDescription());
			return -1;
		}
		else if (m_fDebugMoveTo)
		{
			CDebugOut::PrintAlways("[MOVETO-DBG] FilterSendPacket: ALLOWED %s during walk (not in block list)",
				pkt.GetType().GetDescription());
		}
	}

	ProcessDropQueue();
	ProcessNoMovePickupQueue();
	return 0;
}


/**
 * \brief 
 */
bool CAutoPickupFilter::GetParam(const char* pszParam, void* pData)
{
	return false;
}


/**
 * \brief 
 */
bool CAutoPickupFilter::SetParam(const char* pszParam, void* pData)
{
	struct { const char* pszOpt; ULONG* pFlags; } _map[] = 
		{
			{"bless", &m_ulBlessFlags},
			{"soul", &m_ulSoulFlags},
			{"chaos", &m_ulChaosFlags},
			{"jol", &m_ulLifeFlags},
			{"joc", &m_ulCreationFlags},
			{"jog", &m_ulGuardianFlags},
			{"exl", &m_ulExlFlags},
			{"zen", &m_ulZenFlags},
			{"custom", &m_ulCustomFlags},
		};

	for (int i=0; i < sizeof(_map)/sizeof(_map[0]); i++)
	{
		if (_stricmp(pszParam, _map[i].pszOpt) == 0)
		{
			ULONG ulOld = *_map[i].pFlags;
			*_map[i].pFlags = *((ULONG*)pData);

			WriteClickerLogFmt("PICKUP", "SetParam: %s flags 0x%02X -> 0x%02X (pick=%d, moveTo=%d)",
				pszParam, ulOld, *_map[i].pFlags,
				(int)(*_map[i].pFlags & 1), (int)((*_map[i].pFlags & 2) != 0));

			return true;
		}
	}

	if (_stricmp(pszParam, "autopick") == 0)
	{
		m_fEnabled = *((BOOL*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: autopick %s (m_fEnabled=%d)",
			m_fEnabled ? "ENABLED" : "DISABLED", (int)m_fEnabled);
	}
	else if (_stricmp(pszParam, "pick") == 0)
	{
		DWORD dwData = *((DWORD*)pData);
		WORD wCode = LOWORD(dwData);
		WORD wMask = HIWORD(dwData);

		std::map<WORD,WORD>::iterator  it = m_vItemList.find(wCode);

		if (it != m_vItemList.end())
			it->second = it->second | wMask;
		else
			m_vItemList.insert(std::pair<WORD,WORD>(wCode, wMask));

		WriteClickerLogFmt("PICKUP", "SetParam: pick item code=0x%04X mask=0x%04X (total pick items=%d)",
			wCode, wMask, (int)m_vItemList.size());
	}
	else if (_stricmp(pszParam, "pick_clear") == 0)
	{
		WriteClickerLogFmt("PICKUP", "SetParam: pick list cleared (was %d items)", (int)m_vItemList.size());

		m_vItemList.clear();
	}
	else if (_stricmp(pszParam, "drop") == 0)
	{
		DWORD dwData = *((DWORD*)pData);
		WORD wCode = LOWORD(dwData);
		WORD wMask = HIWORD(dwData);

		std::map<WORD,WORD>::iterator  it = m_vDropList.find(wCode);

		if (it != m_vDropList.end())
			it->second = it->second | wMask;
		else
			m_vDropList.insert(std::pair<WORD,WORD>(wCode, wMask));

		WriteClickerLogFmt("PICKUP", "SetParam: drop item code=0x%04X mask=0x%04X (total drop items=%d)",
			wCode, wMask, (int)m_vDropList.size());
	}
	else if (_stricmp(pszParam, "drop_clear") == 0)
	{
		WriteClickerLogFmt("PICKUP", "SetParam: drop list cleared (was %d items)", (int)m_vDropList.size());

		m_vDropList.clear();
	}
	else if (_stricmp(pszParam, "suspended") == 0)
	{
		m_fSuspended = *((bool*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: suspended=%d", (int)m_fSuspended);
	}
	else if (_stricmp(pszParam, "itemcode") == 0)
	{
		m_fDisplayCode = *((bool*)pData);	
	}
	else if (_stricmp(pszParam, "susp_move_pick") == 0)
	{
		m_fSuspMove = *((bool*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: susp_move_pick=%d", (int)m_fSuspMove);
	}
	else if (_stricmp(pszParam, "susp_pick") == 0)
	{
		m_fSuspPick = *((bool*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: susp_pick=%d", (int)m_fSuspPick);
	}
	else if (_stricmp(pszParam, "susp_zen_pick") == 0)
	{
		m_fSuspZen = *((bool*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: susp_zen_pick=%d", (int)m_fSuspZen);
	}
	else if (_stricmp(pszParam, "pdist") == 0)
	{
		m_iDist = *((int*)pData);

		// Enforce min/max for pickup radius
		if (m_iDist < PICKUP_DIST_MIN) m_iDist = PICKUP_DIST_MIN;
		if (m_iDist > PICKUP_DIST_MAX) m_iDist = PICKUP_DIST_MAX;

		WriteClickerLogFmt("PICKUP", "SetParam: pdist=%d", m_iDist);
	}
	else if (_stricmp(pszParam, "pickdebug") == 0)
	{
		m_fDebugMoveTo = *((bool*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: pickdebug=%d", (int)m_fDebugMoveTo);
	}
	else if (_stricmp(pszParam, "pickrunmode") == 0)
	{
		m_fRunMode = *((bool*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: pickrunmode=%d", (int)m_fRunMode);
	}
	else if (_stricmp(pszParam, "charclass") == 0)
	{
		m_bCharClass = *((BYTE*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: charclass=%d", (int)m_bCharClass);
	}
	else if (_stricmp(pszParam, "autorunmode") == 0)
	{
		m_fAutoRunMode = *((bool*)pData);

		WriteClickerLogFmt("PICKUP", "SetParam: autorunmode=%d", (int)m_fAutoRunMode);
	}

	return true;
}


/**
 * \brief 
 */
DWORD CAutoPickupFilter::PickThreadProc(CAutoPickupFilter* pThis)
{
	while (1)
	{
		HANDLE objs[] = {pThis->m_hStopEvent, pThis->m_hPickEvent};
		DWORD dwRes = WaitForMultipleObjects(2, objs, 0, INFINITE);

		if (dwRes == WAIT_OBJECT_0)
			break;
		
		pThis->GoPickNextItem();
	}

	return 0;
}


/**
 * \brief 
 */
void CAutoPickupFilter::GoPickNextItem()
{
	WORD wItem = 0;
	BYTE x = 0;
	BYTE y = 0;
	bool fSuspended = false;
	bool fSuspMove = false;
	bool fHasItem = false;

	EnterCriticalSection(&m_csQueue);

	if (m_vPickQueue.size() != 0)
	{
		ULONG ulItemInfo = *(m_vPickQueue.begin());
		m_vPickQueue.erase(ulItemInfo);

		wItem = LOWORD(ulItemInfo);
		x = LOBYTE(HIWORD(ulItemInfo));
		y = HIBYTE(HIWORD(ulItemInfo));
		fHasItem = true;
	}

	if (m_vPickQueue.size() == 0)
		ResetEvent(m_hPickEvent);

	fSuspended = m_fSuspended;
	fSuspMove = m_fSuspMove;
	LeaveCriticalSection(&m_csQueue);

	if (!fHasItem)
	{
		if (m_fDebugMoveTo)
			CDebugOut::PrintAlways("[MOVETO-DBG] GoPickNextItem: queue is empty, nothing to pick");
		return;
	}

	WriteClickerLogFmt("PICKUP", "GoPickNextItem: dequeued item 0x%04X at (%d,%d), suspended=%d, suspMove=%d",
		wItem, (int)x, (int)y, (int)fSuspended, (int)fSuspMove);

	if (m_fDebugMoveTo)
		CDebugOut::PrintAlways("[MOVETO-DBG] GoPickNextItem: dequeued item 0x%04X at (%d,%d), suspended=%d, suspMove=%d",
			wItem, (int)x, (int)y, (int)fSuspended, (int)fSuspMove);

	CPacketFilter* pCharInfo = GetProxy()->GetFilter("CharInfoFilter");
	CPacketFilter* pScript = GetProxy()->GetFilter("ScriptProcessorFilter");
	CPacketFilter* pAutoKill = GetProxy()->GetFilter("AutoKillFilter");

	BYTE xOld = 0;
	BYTE yOld = 0;
	WORD wPlayerId = 0;

	if (!pCharInfo 
			|| !pCharInfo->GetParam("X", &xOld)
			|| !pCharInfo->GetParam("Y", &yOld)
			|| !pCharInfo->GetParam("PlayerId", &wPlayerId))
	{
		if (m_fDebugMoveTo)
			CDebugOut::PrintAlways("[MOVETO-DBG] GoPickNextItem: FAILED - CharInfoFilter unavailable or could not get player position/ID (charInfo=%p)",
				pCharInfo);
		return;
	}

	if (m_fDebugMoveTo)
		CDebugOut::PrintAlways("[MOVETO-DBG] GoPickNextItem: player at (%d,%d) id=0x%04X, target item at (%d,%d)",
			(int)xOld, (int)yOld, wPlayerId, (int)x, (int)y);

	if (!fSuspended || !fSuspMove)
	{
		bool fTemp = true;
		if (!fSuspended)
		{
			if (pScript)
				pScript->SetParam("suspended", (void*)&fTemp);
			if (pAutoKill)
				pAutoKill->SetParam("suspended", (void*)&fTemp);
		}

		m_fWalking = true;
		Sleep(100);

		// Auto-determine run mode based on equipment and character class.
		// Only auto-detect if user hasn't explicitly enabled run mode (pickrunmode).
		// When user sets pickrunmode=on, always use run mode regardless of auto-detection.
		//
		// Run method rules:
		//   Magic Gladiator (class 5) and Dark Lord (class 0) → always Run
		//   Other classes (DK/BK, DW/SM, Elf/Muse):
		//     Run if boots_level >= 4, OR has wings, OR has pet equipped
		//     Walk otherwise (boots < +4, no wings, no pet)
		if (m_fAutoRunMode && !m_fRunMode)
		{
			CPacketFilter* pInvFilter = GetProxy()->GetFilter("InventoryManagerFilter");
			if (pInvFilter)
			{
				static const BYTE CLASS_DL = 0;
				static const BYTE CLASS_MG = 5;

				int iBootsLevel = -1;
				int iHasWings = 0;
				int iHasPet = 0;

				pInvFilter->GetParam("boots_level", &iBootsLevel);
				pInvFilter->GetParam("has_wings", &iHasWings);
				pInvFilter->GetParam("has_pet", &iHasPet);

				if (m_bCharClass == CLASS_MG || m_bCharClass == CLASS_DL)
				{
					// MG and DL always run
					m_fRunMode = true;
				}
				else
				{
					// Run if boots >= +4, or wings equipped, or pet equipped
					m_fRunMode = (iBootsLevel >= 4) || (iHasWings != 0) || (iHasPet != 0);
				}

				if (m_fDebugMoveTo)
					CDebugOut::PrintAlways("[MOVETO-DBG] Auto run mode: boots_level=%d, wings=%d, pet=%d, class=%d -> %s",
						iBootsLevel, iHasWings, iHasPet, (int)m_bCharClass, m_fRunMode ? "RUN" : "WALK");

				if (CDebugMode::IsEnabled())
					WriteClickerLogFmt("PICKUP", "Auto run mode: class=%d, boots=%d, wings=%d, pet=%d -> %s",
						(int)m_bCharClass, iBootsLevel, iHasWings, iHasPet, m_fRunMode ? "RUN" : "WALK");
			}
			else
			{
				// Fallback: InventoryManagerFilter unavailable (e.g. MUEliteClicker build)
				// Use class-based defaults — MG and DL always run, others walk
				static const BYTE CLASS_DL = 0;
				static const BYTE CLASS_MG = 5;
				m_fRunMode = (m_bCharClass == CLASS_MG || m_bCharClass == CLASS_DL);

				if (m_fDebugMoveTo)
					CDebugOut::PrintAlways("[MOVETO-DBG] Auto run mode fallback (no InventoryManagerFilter): class=%d -> %s",
						(int)m_bCharClass, m_fRunMode ? "RUN" : "WALK");

				if (CDebugMode::IsEnabled())
					WriteClickerLogFmt("PICKUP", "Auto run mode: InventoryManagerFilter unavailable, using class-based fallback (class=%d -> %s)",
						(int)m_bCharClass, m_fRunMode ? "RUN" : "WALK");
			}
		}

		if (m_fRunMode)
		{
			CDebugOut::PrintAlways("[AUTOPICK] Running to item at (%d,%d) from (%d,%d), item=0x%04X", 
				(int)x, (int)y, (int)xOld, (int)yOld, wItem);
			WriteClickerLogFmt("PICKUP", "GoPickNextItem: RUNNING to item 0x%04X at (%d,%d) from (%d,%d)",
				wItem, (int)x, (int)y, (int)xOld, (int)yOld);

			// Run to item step by step (emulates player running)
			RunTo(wPlayerId, xOld, yOld, x, y);
		}
		else
		{
			CDebugOut::PrintAlways("[AUTOPICK] Walking to item at (%d,%d) from (%d,%d), item=0x%04X", 
				(int)x, (int)y, (int)xOld, (int)yOld, wItem);
			WriteClickerLogFmt("PICKUP", "GoPickNextItem: WALKING to item 0x%04X at (%d,%d) from (%d,%d)",
				wItem, (int)x, (int)y, (int)xOld, (int)yOld);

			// Walk to item step by step (emulates player walking)
			WalkTo(wPlayerId, xOld, yOld, x, y);
		}

		// Settling delay: give the server time to process the final
		// position update before we send the pickup request
		Sleep(WALK_BASE_DELAY_MS);
	}
	else if (m_fDebugMoveTo)
	{
		CDebugOut::PrintAlways("[MOVETO-DBG] GoPickNextItem: walking SKIPPED (both suspended=%d and suspMove=%d are true), picking in place",
			(int)fSuspended, (int)fSuspMove);
	}

	// Check if autopick was disabled during movement (race condition:
	// user stopped the clicker while we were walking to the item).
	// Skip sending PickItem to avoid sending packets after clicker stop.
	if (!m_fEnabled)
	{
		WriteClickerLogFmt("PICKUP", "GoPickNextItem: ABORTED pickup of item 0x%04X - autopick disabled during movement, returning to origin",
			wItem);
		CDebugOut::PrintAlways("[AUTOPICK] GoPickNextItem: ABORTED - autopick disabled during movement");
	}
	else
	{
		PickItem(wItem);
	}

	if (!fSuspended || !fSuspMove)
	{
		Sleep(200);

		// Move back to original position using the same mode (walk or run)
		if (m_fRunMode)
		{
			CDebugOut::PrintAlways("[AUTOPICK] Running back to original position (%d,%d)", (int)xOld, (int)yOld);
			WriteClickerLogFmt("PICKUP", "GoPickNextItem: RUNNING back to original position (%d,%d)", (int)xOld, (int)yOld);
			RunTo(wPlayerId, x, y, xOld, yOld);
		}
		else
		{
			CDebugOut::PrintAlways("[AUTOPICK] Walking back to original position (%d,%d)", (int)xOld, (int)yOld);
			WriteClickerLogFmt("PICKUP", "GoPickNextItem: WALKING back to original position (%d,%d)", (int)xOld, (int)yOld);
			WalkTo(wPlayerId, x, y, xOld, yOld);
		}

		// Settling delay: let the server finish processing the return
		// movement before resuming filters (AutoKill, Script, etc.)
		Sleep(WALK_BASE_DELAY_MS);

		m_fWalking = false;

		if (m_fDebugMoveTo)
			CDebugOut::PrintAlways("[MOVETO-DBG] GoPickNextItem: walk complete, m_fWalking=false, resuming filters");

		bool fTemp = false;
		if (!fSuspended)
		{
			if (pScript)
				pScript->SetParam("suspended", (void*)&fTemp);
			if (pAutoKill)
				pAutoKill->SetParam("suspended", (void*)&fTemp);
		}
	}
}


/**
 * \brief 
 */
void CAutoPickupFilter::PickItem(WORD wId)
{
	CPickItemPacket pkt(wId);

	CStringA szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());
	WriteClickerLogFmt("PICKUP", "SEND CPickItemPacket: picking up item ID=0x%04X (walking=%d) | Len=%d | %s",
		wId, (int)m_fWalking, pkt.GetDecryptedLen(), (const char*)szHex);
	CDebugOut::PrintAlways("[PICKUP] SEND CPickItemPacket: ID=0x%04X walking=%d | Len=%d | %s",
		wId, (int)m_fWalking, pkt.GetDecryptedLen(), (const char*)szHex);

	// Always use send_packet() so the packet goes through the full filter chain
	// including PacketEncryptFilter.  FilterSendPacket allows injected packets
	// through even during walking, so this will not be blocked.
	GetProxy()->send_packet(pkt);
}


/**
 * \brief 
 */
void CAutoPickupFilter::DropItem(BYTE pos)
{
	CPacketFilter* pCharInfo = GetProxy()->GetFilter("CharInfoFilter");

	BYTE x = 0;
	BYTE y = 0;

	if (!pCharInfo 
		|| !pCharInfo->GetParam("X", &x)
		|| !pCharInfo->GetParam("Y", &y))
	{
		WriteClickerLogFmt("PICKUP", "SEND CDropItemPacket FAILED: cannot get player position for slot=%d", (int)pos);
		return;
	}

	CDropItemPacket pkt(x, y, pos);

	CStringA szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());
	WriteClickerLogFmt("PICKUP", "SEND CDropItemPacket: dropping item from slot=%d at (%d,%d) | Len=%d | %s",
		(int)pos, (int)x, (int)y, pkt.GetDecryptedLen(), (const char*)szHex);
	CDebugOut::PrintAlways("[PICKUP] SEND CDropItemPacket: slot=%d pos=(%d,%d) | Len=%d | %s",
		(int)pos, (int)x, (int)y, pkt.GetDecryptedLen(), (const char*)szHex);

	GetProxy()->send_packet(pkt);
}


/**
 * \brief Moves character to position step-by-step (tile by tile),
 *        emulating real player walking. The character walks diagonally
 *        or straight towards the target one tile at a time.
 */
void CAutoPickupFilter::WalkTo(WORD wPlayerId, BYTE xFrom, BYTE yFrom, BYTE xTo, BYTE yTo)
{
	int currX = (int)xFrom;
	int currY = (int)yFrom;
	int destX = (int)xTo;
	int destY = (int)yTo;

	WriteClickerLogFmt("PICKUP", "WalkTo: from (%d,%d) to (%d,%d), playerId=0x%04X",
		currX, currY, destX, destY, wPlayerId);

	if (m_fDebugMoveTo)
		CDebugOut::PrintAlways("[MOVETO-DBG] WalkTo: from (%d,%d) to (%d,%d), playerId=0x%04X", 
			currX, currY, destX, destY, wPlayerId);

	int steps = 0;

	while (currX != destX || currY != destY)
	{
		int dx = (destX > currX) ? 1 : (destX < currX) ? -1 : 0;
		int dy = (destY > currY) ? 1 : (destY < currY) ? -1 : 0;

		currX += dx;
		currY += dy;
		steps++;

		// Update client position (emulate server telling client where character is)
		CUpdatePosSTCPacket pktMoveSTC(wPlayerId, (BYTE)currX, (BYTE)currY);
		GetProxy()->recv_packet(pktMoveSTC);

		// Update server position (send through filter chain for proper encryption;
		// FilterSendPacket allows injected packets through during walking)
		CUpdatePosCTSPacket pktMoveCTS((BYTE)currX, (BYTE)currY);
		GetProxy()->send_packet(pktMoveCTS);

		if (steps == 1 || (currX == destX && currY == destY))
		{
			CStringA szHexSTC = CBufferUtil::BufferToHex((char*)pktMoveSTC.GetDecryptedPacket(), pktMoveSTC.GetDecryptedLen());
			CStringA szHexCTS = CBufferUtil::BufferToHex((char*)pktMoveCTS.GetDecryptedPacket(), pktMoveCTS.GetDecryptedLen());
			WriteClickerLogFmt("PICKUP", "WalkTo step %d: RECV STC | %s | SEND CTS | %s | pos=(%d,%d)",
				steps, (const char*)szHexSTC, (const char*)szHexCTS, currX, currY);
			CDebugOut::PrintAlways("[PICKUP] WalkTo step %d: STC | %s | CTS | %s | pos=(%d,%d)",
				steps, (const char*)szHexSTC, (const char*)szHexCTS, currX, currY);
		}
		else if (m_fDebugMoveTo)
		{
			WriteClickerLogFmt("PICKUP", "WalkTo step %d: pos=(%d,%d)", steps, currX, currY);
		}

		// Wait per tile to emulate walking speed
		Sleep(WALK_MS_PER_TILE);
	}

	WriteClickerLogFmt("PICKUP", "WalkTo: completed in %d steps (%d ms)", steps, steps * WALK_MS_PER_TILE);

	if (m_fDebugMoveTo)
		CDebugOut::PrintAlways("[MOVETO-DBG] WalkTo: completed in %d steps (%d ms)", steps, steps * WALK_MS_PER_TILE);
}


/**
 * \brief Moves character to position step-by-step (tile by tile),
 *        emulating player running. Uses RUN_MS_PER_TILE for faster
 *        movement compared to WalkTo.
 */
void CAutoPickupFilter::RunTo(WORD wPlayerId, BYTE xFrom, BYTE yFrom, BYTE xTo, BYTE yTo)
{
	int currX = (int)xFrom;
	int currY = (int)yFrom;
	int destX = (int)xTo;
	int destY = (int)yTo;

	WriteClickerLogFmt("PICKUP", "RunTo: from (%d,%d) to (%d,%d), playerId=0x%04X",
		currX, currY, destX, destY, wPlayerId);

	if (m_fDebugMoveTo)
		CDebugOut::PrintAlways("[MOVETO-DBG] RunTo: from (%d,%d) to (%d,%d), playerId=0x%04X", 
			currX, currY, destX, destY, wPlayerId);

	int steps = 0;

	while (currX != destX || currY != destY)
	{
		int dx = (destX > currX) ? 1 : (destX < currX) ? -1 : 0;
		int dy = (destY > currY) ? 1 : (destY < currY) ? -1 : 0;

		currX += dx;
		currY += dy;
		steps++;

		// Update client position (emulate server telling client where character is)
		CUpdatePosSTCPacket pktMoveSTC(wPlayerId, (BYTE)currX, (BYTE)currY);
		GetProxy()->recv_packet(pktMoveSTC);

		// Update server position (send through filter chain for proper encryption;
		// FilterSendPacket allows injected packets through during walking)
		CUpdatePosCTSPacket pktMoveCTS((BYTE)currX, (BYTE)currY);
		GetProxy()->send_packet(pktMoveCTS);

		if (steps == 1 || (currX == destX && currY == destY))
		{
			CStringA szHexSTC = CBufferUtil::BufferToHex((char*)pktMoveSTC.GetDecryptedPacket(), pktMoveSTC.GetDecryptedLen());
			CStringA szHexCTS = CBufferUtil::BufferToHex((char*)pktMoveCTS.GetDecryptedPacket(), pktMoveCTS.GetDecryptedLen());
			WriteClickerLogFmt("PICKUP", "RunTo step %d: RECV STC | %s | SEND CTS | %s | pos=(%d,%d)",
				steps, (const char*)szHexSTC, (const char*)szHexCTS, currX, currY);
			CDebugOut::PrintAlways("[PICKUP] RunTo step %d: STC | %s | CTS | %s | pos=(%d,%d)",
				steps, (const char*)szHexSTC, (const char*)szHexCTS, currX, currY);
		}
		else if (m_fDebugMoveTo)
		{
			WriteClickerLogFmt("PICKUP", "RunTo step %d: pos=(%d,%d)", steps, currX, currY);
		}

		// Wait per tile to emulate running speed (faster than walking)
		Sleep(RUN_MS_PER_TILE);
	}

	WriteClickerLogFmt("PICKUP", "RunTo: completed in %d steps (%d ms)", steps, steps * RUN_MS_PER_TILE);

	if (m_fDebugMoveTo)
		CDebugOut::PrintAlways("[MOVETO-DBG] RunTo: completed in %d steps (%d ms)", steps, steps * RUN_MS_PER_TILE);
}


/**
 * \brief Teleports character to position instantly (legacy method,
 *        kept for compatibility but no longer used by GoPickNextItem)
 */
void CAutoPickupFilter::TeleportTo(WORD wPlayerId, BYTE x, BYTE y)
{
	WriteClickerLogFmt("PICKUP", "TeleportTo: teleporting to (%d,%d), playerId=0x%04X", (int)x, (int)y, wPlayerId);

	CUpdatePosSTCPacket pktMoveSTC(wPlayerId, x, y);
	GetProxy()->recv_packet(pktMoveSTC);
	Sleep(10);

	CUpdatePosCTSPacket pktMoveCTS(x, y);
	GetProxy()->send_packet(pktMoveCTS);

	Sleep(250);

	GetProxy()->recv_packet(pktMoveSTC);
	Sleep(10);

	GetProxy()->send_packet(pktMoveCTS);
	Sleep(10);
}


/**  
 * \brief 
 */
void CAutoPickupFilter::ProcessDropQueue()
{
	while (!m_vDropQueue.empty() && (GetTickCount() - m_vDropQueue.front().dwTimestamp) > 700)
	{
		WriteClickerLogFmt("PICKUP", "ProcessDropQueue: dropping item from inventory slot=%d (queued %d ms ago)",
			(int)m_vDropQueue.front().bInvPos, (int)(GetTickCount() - m_vDropQueue.front().dwTimestamp));
		CDebugOut::PrintAlways("[PICKUP] ProcessDropQueue: dropping slot=%d (queued %d ms ago)",
			(int)m_vDropQueue.front().bInvPos, (int)(GetTickCount() - m_vDropQueue.front().dwTimestamp));

		DropItem(m_vDropQueue.front().bInvPos);
		m_vDropQueue.pop_front();
	}
}


/**  
 * \brief 
 */
void CAutoPickupFilter::ProcessNoMovePickupQueue()
{
	while (!m_vNoMovePickQueue.empty() && (GetTickCount() - m_vNoMovePickQueue.front().dwTimestamp) > 2000)
	{
		WriteClickerLogFmt("PICKUP", "ProcessNoMovePickupQueue: picking item ID=0x%04X (queued %d ms ago)",
			m_vNoMovePickQueue.front().wItemId, (int)(GetTickCount() - m_vNoMovePickQueue.front().dwTimestamp));
		CDebugOut::PrintAlways("[PICKUP] ProcessNoMovePickupQueue: picking ID=0x%04X (queued %d ms ago)",
			m_vNoMovePickQueue.front().wItemId, (int)(GetTickCount() - m_vNoMovePickQueue.front().dwTimestamp));

		PickItem(m_vNoMovePickQueue.front().wItemId);
		m_vNoMovePickQueue.pop_front();
	}
}
