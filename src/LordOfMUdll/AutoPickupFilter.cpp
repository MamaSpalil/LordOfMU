#include "stdafx.h"
#include "AutoPickupFilter.h"
#include "CommonPackets.h"
#include "DebugOut.h"
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
	m_iDist = -1;

	m_fEnabled = FALSE;
	m_fDisplayCode = FALSE;

	m_fSuspended = false;
	m_fSuspPick = false;
	m_fSuspZen = false;
	m_fSuspMove = false;
	m_fWalking = false;
	m_fDebugMoveTo = false;

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
	if (m_fEnabled && pkt == CMeetItemPacket::Type())
	{
		if (m_fSuspended && m_fSuspPick)
		{
			if (m_fDebugMoveTo)
				CDebugOut::PrintAlways("[MOVETO-DBG] Item packet ignored: picking is suspended (m_fSuspended=%d, m_fSuspPick=%d)", (int)m_fSuspended, (int)m_fSuspPick);
			return 0;
		}

		CMeetItemPacket& pktMItem = (CMeetItemPacket&)pkt;

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
				bool fInRange = (m_iDist < 0 || (xdiff <= m_iDist && ydiff <= m_iDist));

				if (fMoveTo && fPickEnabled)
				{
					// Move To mode: queue item for walking pickup regardless of distance
					CAutoLockQueue autoCS(&m_csQueue);
					m_vPickQueue.insert((ULONG)wId | (ULONG)x << 16 | (ULONG)y << 24);
					SetEvent(m_hPickEvent);

					if (m_fDebugMoveTo)
						CDebugOut::PrintAlways("[MOVETO-DBG] Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for Move To pickup", 
							wType, wId, (int)x, (int)y, xdiff, ydiff);
				}
				else if (fPickEnabled && fInRange)
				{
					// Immediate pickup: item is within range and pickup enabled
					PickItem(wId);

					if (m_fDebugMoveTo)
						CDebugOut::PrintAlways("[MOVETO-DBG] Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) picked up immediately (no Move To)", 
							wType, wId, (int)x, (int)y, xdiff, ydiff);
				}
				else if (m_fDebugMoveTo)
				{
					CDebugOut::PrintAlways("[MOVETO-DBG] Custom item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) SKIPPED: pickEnabled=%d, moveTo=%d, inRange=%d",
						wType, wId, (int)x, (int)y, xdiff, ydiff, (int)fPickEnabled, (int)fMoveTo, (int)fInRange);
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
						// Move To mode: queue item for walking pickup regardless of distance
						{
							CAutoLockQueue autoCS(&m_csQueue);
							m_vPickQueue.insert((ULONG)wId | (ULONG)x << 16 | (ULONG)y << 24);
							SetEvent(m_hPickEvent);
						}

						if (m_fDebugMoveTo)
							CDebugOut::PrintAlways("[MOVETO-DBG] %s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for Move To pickup",
								pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff);
					}
					else
					{
						m_vNoMovePickQueue.push_back(CPickInfo(wId));

						if (m_fDebugMoveTo)
							CDebugOut::PrintAlways("[MOVETO-DBG] %s item 0x%04X (id=0x%04X) at (%d,%d) dist=(%d,%d) queued for no-move pickup",
								pszItemName, wType, wId, (int)x, (int)y, xdiff, ydiff);
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
	else if (pkt == CWarpReplyPacket::Type())
	{
		// Clear stale pickup queues from previous map, but keep m_fEnabled
		// so auto-pickup continues working after warp
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
		if (pkt == CNormalAttackPacket::Type()
			|| pkt == CSingleSkillPacket::Type()
			|| pkt == CMassiveSkillPacket::Type()
			|| pkt == CCharMoveCTSPacket::Type())
		{
			return -1;
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
			*_map[i].pFlags = *((ULONG*)pData);
			return true;
		}
	}

	if (_stricmp(pszParam, "autopick") == 0)
	{
		m_fEnabled = *((BOOL*)pData);
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
	}
	else if (_stricmp(pszParam, "pick_clear") == 0)
	{
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
	}
	else if (_stricmp(pszParam, "drop_clear") == 0)
	{
		m_vDropList.clear();
	}
	else if (_stricmp(pszParam, "suspended") == 0)
	{
		m_fSuspended = *((bool*)pData);
	}
	else if (_stricmp(pszParam, "itemcode") == 0)
	{
		m_fDisplayCode = *((bool*)pData);	
	}
	else if (_stricmp(pszParam, "susp_move_pick") == 0)
	{
		m_fSuspMove = *((bool*)pData);
	}
	else if (_stricmp(pszParam, "susp_pick") == 0)
	{
		m_fSuspPick = *((bool*)pData);
	}
	else if (_stricmp(pszParam, "susp_zen_pick") == 0)
	{
		m_fSuspZen = *((bool*)pData);
	}
	else if (_stricmp(pszParam, "pdist") == 0)
	{
		m_iDist = *((int*)pData);
	}
	else if (_stricmp(pszParam, "pickdebug") == 0)
	{
		m_fDebugMoveTo = *((bool*)pData);
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
		Sleep(350);

		CDebugOut::PrintAlways("[AUTOPICK] Walking to item at (%d,%d) from (%d,%d), item=0x%04X", 
			(int)x, (int)y, (int)xOld, (int)yOld, wItem);

		// Walk to item step by step (emulates player walking)
		WalkTo(wPlayerId, xOld, yOld, x, y);
	}
	else if (m_fDebugMoveTo)
	{
		CDebugOut::PrintAlways("[MOVETO-DBG] GoPickNextItem: walking SKIPPED (both suspended=%d and suspMove=%d are true), picking in place",
			(int)fSuspended, (int)fSuspMove);
	}

	PickItem(wItem);

	if (!fSuspended || !fSuspMove)
	{
		Sleep(500);

		// Walk back to original position step by step
		CDebugOut::PrintAlways("[AUTOPICK] Walking back to original position (%d,%d)", (int)xOld, (int)yOld);
		WalkTo(wPlayerId, x, y, xOld, yOld);

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
	CDebugOut::PrintAlways("[AUTOPICK] Picking up item ID=0x%04X", wId);
	CPickItemPacket pkt(wId);
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
		return;
	}

	CDropItemPacket pkt(x, y, pos);
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

		// Update server position (bypass filter chain to avoid being blocked)
		CUpdatePosCTSPacket pktMoveCTS((BYTE)currX, (BYTE)currY);
		GetProxy()->send_direct(pktMoveCTS);

		// Wait per tile to emulate walking speed
		Sleep(WALK_MS_PER_TILE);
	}

	if (m_fDebugMoveTo)
		CDebugOut::PrintAlways("[MOVETO-DBG] WalkTo: completed in %d steps (%d ms)", steps, steps * WALK_MS_PER_TILE);
}


/**
 * \brief Teleports character to position instantly (legacy method,
 *        kept for compatibility but no longer used by GoPickNextItem)
 */
void CAutoPickupFilter::TeleportTo(WORD wPlayerId, BYTE x, BYTE y)
{
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
		PickItem(m_vNoMovePickQueue.front().wItemId);
		m_vNoMovePickQueue.pop_front();
	}
}
