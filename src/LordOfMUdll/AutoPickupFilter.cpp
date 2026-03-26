#include "stdafx.h"
#include "AutoPickupFilter.h"
#include "CommonPackets.h"
#include "DebugOut.h"
#include "ClickerLogger.h"
#include "BufferUtil.h"
#include <math.h>
#include <time.h>

// ---- Pickup History tracking (global, thread-safe) ----

static const int MAX_HISTORY_ENTRIES = 200;

struct PickupHistoryEntry
{
	char szTime[20];   // "HH:MM:SS"
	char szItem[128];  // Item display name
};

static PickupHistoryEntry g_vPickupHistory[MAX_HISTORY_ENTRIES];
static int g_nHistoryCount = 0;
static int g_nHistoryHead = 0; // circular buffer write position
static CRITICAL_SECTION g_csHistory;
static BOOL g_bHistoryInit = FALSE;

// ---- Session Statistics (global, thread-safe via g_csHistory) ----

// Key names used in GetSessionStats text format (must match consumer in MuWindow.cpp)
static const char* STAT_KEY_KILLS      = "kills";
static const char* STAT_KEY_ITEMS      = "items";
static const char* STAT_KEY_ZEN        = "zen";
static const char* STAT_KEY_ZENCOUNT   = "zencount";
static const char* STAT_KEY_EXP        = "exp";
static const char* STAT_KEY_RUNTIME    = "runtime";

static int g_nSessionKillCount = 0;
static int g_nSessionItemCount = 0;
static int g_nSessionZenPickupCount = 0;
static unsigned __int64 g_ullSessionZenTotal = 0;
static unsigned __int64 g_ullSessionExpGained = 0;
static DWORD g_dwSessionStartTick = 0;
static BOOL g_bSessionActive = FALSE;

static void InitHistoryCS()
{
	if (!g_bHistoryInit)
	{
		InitializeCriticalSection(&g_csHistory);
		g_bHistoryInit = TRUE;
	}
}

static void AddPickupHistoryEntry(const char* pszItemName)
{
	InitHistoryCS();
	EnterCriticalSection(&g_csHistory);

	time_t t = time(NULL);
	struct tm tm_storage = {0};
	localtime_s(&tm_storage, &t);

	PickupHistoryEntry& entry = g_vPickupHistory[g_nHistoryHead];
	strftime(entry.szTime, sizeof(entry.szTime), "%H:%M:%S", &tm_storage);
	strncpy(entry.szItem, pszItemName, sizeof(entry.szItem) - 1);
	entry.szItem[sizeof(entry.szItem) - 1] = '\0';

	g_nHistoryHead= (g_nHistoryHead + 1) % MAX_HISTORY_ENTRIES;
	if (g_nHistoryCount < MAX_HISTORY_ENTRIES)
		g_nHistoryCount++;

	LeaveCriticalSection(&g_csHistory);
}

/**
 * \brief Exported function: copies pickup history entries to caller buffer.
 *        Returns number of entries copied.
 *        Each entry is "HH:MM:SS|ItemName\n" format.
 *        Buffer should be large enough (e.g., 32KB).
 */
extern "C" __declspec(dllexport) int GetPickupHistory(char* pszBuffer, int nBufSize)
{
	if (!pszBuffer || nBufSize <= 0)
		return 0;

	InitHistoryCS();
	EnterCriticalSection(&g_csHistory);

	pszBuffer[0] = '\0';
	int nWritten = 0;
	int nPos = 0;

	// Read entries from oldest to newest
	int nStart = (g_nHistoryCount < MAX_HISTORY_ENTRIES)
		? 0
		: g_nHistoryHead;

	for (int i = 0; i < g_nHistoryCount; ++i)
	{
		int idx = (nStart + i) % MAX_HISTORY_ENTRIES;
		const PickupHistoryEntry& entry = g_vPickupHistory[idx];

		int nNeeded = (int)strlen(entry.szTime) + 1 + (int)strlen(entry.szItem) + 1; // "time|item\n"
		if (nPos + nNeeded >= nBufSize - 1)
			break;

		int nRet = _snprintf(pszBuffer + nPos, nBufSize - nPos - 1,
			"%s|%s\n", entry.szTime, entry.szItem);
		if (nRet < 0)
			break;
		nPos += nRet;
		nWritten++;
	}

	pszBuffer[nPos] = '\0';

	LeaveCriticalSection(&g_csHistory);
	return nWritten;
}


/**
 * \brief Exported function: returns session statistics as a text buffer.
 *        Format: "kills=NNN\nitems=NNN\nzen=NNN\nexp=NNN\nruntime=NNN\n"
 *        runtime is in seconds since session start.
 */
extern "C" __declspec(dllexport) int GetSessionStats(char* pszBuffer, int nBufSize)
{
	if (!pszBuffer || nBufSize <= 0)
		return 0;

	InitHistoryCS();
	EnterCriticalSection(&g_csHistory);

	DWORD dwRuntime = 0;
	if (g_bSessionActive && g_dwSessionStartTick != 0)
		dwRuntime = (GetTickCount() - g_dwSessionStartTick) / 1000;

	_snprintf(pszBuffer, nBufSize - 1,
		"%s=%d\n%s=%d\n%s=%I64u\n%s=%d\n%s=%I64u\n%s=%lu\n",
		STAT_KEY_KILLS, g_nSessionKillCount,
		STAT_KEY_ITEMS, g_nSessionItemCount,
		STAT_KEY_ZEN, g_ullSessionZenTotal,
		STAT_KEY_ZENCOUNT, g_nSessionZenPickupCount,
		STAT_KEY_EXP, g_ullSessionExpGained,
		STAT_KEY_RUNTIME, (unsigned long)dwRuntime);
	pszBuffer[nBufSize - 1] = '\0';

	LeaveCriticalSection(&g_csHistory);
	return 1;
}


/**
 * \brief Exported function: resets session statistics and pickup history.
 *        Called when the autoclicker starts a new session.
 */
extern "C" __declspec(dllexport) void ResetSessionStats()
{
	InitHistoryCS();
	EnterCriticalSection(&g_csHistory);

	g_nSessionKillCount = 0;
	g_nSessionItemCount = 0;
	g_nSessionZenPickupCount = 0;
	g_ullSessionZenTotal = 0;
	g_ullSessionExpGained = 0;
	g_dwSessionStartTick = GetTickCount();
	g_bSessionActive = TRUE;

	// Also clear pickup history for the new session
	g_nHistoryCount = 0;
	g_nHistoryHead = 0;

	LeaveCriticalSection(&g_csHistory);
}


/**
 * \brief Exported function: marks the session as stopped (runtime freezes).
 */
extern "C" __declspec(dllexport) void StopSession()
{
	InitHistoryCS();
	EnterCriticalSection(&g_csHistory);
	g_bSessionActive = FALSE;
	LeaveCriticalSection(&g_csHistory);
}


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

	m_dwLastZen = 0;
	m_fZenTracked = false;

	// Try to initialize Zen from game memory so that session tracking
	// starts with the correct baseline even before the first packet.
	{
		DWORD dwMemZen = 0;
		if (ReadZenFromMemory(&dwMemZen))
		{
			m_dwLastZen = dwMemZen;
			m_fZenTracked = true;
			WriteClickerLogFmt("PICKUP", "Constructor: initialized m_dwLastZen=%lu from game memory",
				(unsigned long)m_dwLastZen);
		}
	}

	InitializeCriticalSection(&m_csQueue);

	m_hPickEvent = CreateEvent(0, 1, 0, 0);
	m_hStopEvent = CreateEvent(0, 1, 0, 0);
	m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PickThreadProc, this, 0, 0);
}


/**
 * \brief Read current Zen from game memory using the known ObjectCharacter offset.
 *        Returns TRUE if the value was read successfully.
 */
BOOL CAutoPickupFilter::ReadZenFromMemory(DWORD* pdwZen)
{
	if (!pdwZen)
		return FALSE;

	__try
	{
		DWORD dwCharBase = *(DWORD*)ZEN_CHAR_STRUCT_PTR;
		if (dwCharBase == 0)
		{
			WriteClickerLogFmt("PICKUP", "ReadZenFromMemory: char struct pointer is NULL (0x%08X)",
				ZEN_CHAR_STRUCT_PTR);
			return FALSE;
		}

		// Validate that the money offset is within readable memory
		DWORD* pdwMoney = (DWORD*)(dwCharBase + ZEN_MONEY_OFFSET);
		if (IsBadReadPtr(pdwMoney, sizeof(DWORD)))
		{
			WriteClickerLogFmt("PICKUP", "ReadZenFromMemory: bad read ptr at 0x%08X (base=0x%08X + %d)",
				(DWORD)pdwMoney, dwCharBase, ZEN_MONEY_OFFSET);
			return FALSE;
		}

		DWORD dwZen = *pdwMoney;

		// Sanity check: Zen should not exceed MAX_ZEN
		if (dwZen > MAX_ZEN)
		{
			WriteClickerLogFmt("PICKUP", "ReadZenFromMemory: value %lu exceeds MAX_ZEN, ignoring",
				(unsigned long)dwZen);
			return FALSE;
		}

		*pdwZen = dwZen;
		WriteClickerLogFmt("PICKUP", "ReadZenFromMemory: read zen=%lu from [0x%08X]+%d (base=0x%08X)",
			(unsigned long)dwZen, ZEN_CHAR_STRUCT_PTR, ZEN_MONEY_OFFSET, dwCharBase);
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		WriteClickerLogFmt("PICKUP", "ReadZenFromMemory: exception reading memory (0x%08X+%d)",
			ZEN_CHAR_STRUCT_PTR, ZEN_MONEY_OFFSET);
		return FALSE;
	}
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

			int xdiff= abs((int)bPlX - (int)x);
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
	else if (pkt == CPickItemResultFailPacket::Type())
	{
		CPickItemResultFailPacket& pktFail = (CPickItemResultFailPacket&)pkt;
		// Mask to 12-bit item type (strip upper group bits used for drop masks)
		WORD wFailType = pktFail.GetItemType() & 0x0FFF;

		if (m_fEnabled)
		{
			if (wFailType == TYPE_ZEN)
			{
				// Zen has its own separate vault (max 2,000,000,000) and does
				// not occupy inventory slots.  Do NOT show "Inventory is Full"
				// for Zen - the CZenUpdatePacket handler will display
				// "[PICKUP] - {amount} Zen Obtained" with the actual amount.
				WriteClickerLogFmt("PICKUP", "RECV CPickItemResultFailPacket for Zen (type=0x%04X): skipping inventory full notification", wFailType);
			}
			else
			{
				WriteClickerLogFmt("PICKUP", "RECV CPickItemResultFailPacket: inventory is full, cannot pick item type=0x%04X", wFailType);
				CDebugOut::PrintAlways("[PICKUP] RECV CPickItemResultFailPacket: inventory is full");

				CServerMessagePacket pktFull("[PICKUP] - Inventory is Full");
				GetProxy()->recv_direct(pktFull);
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

			// Zen pickup: identified by item type OR inventory slot=254
			// (Zen has its own vault, it does not occupy normal inventory slots).
			// Amount notification is deferred to the CZenUpdatePacket handler
			// which calculates the exact delta.
			bool bIsZen = (wType == TYPE_ZEN) || (bPos == INVENTORY_SLOT_ZEN);

			if (!bIsZen)
			{
				// Display "[PICKUP] - ItemName Obtained" notification
				const char* pszName = GetItemDisplayName(wType);
				CServerMessagePacket pktObtained("[PICKUP] - %s Obtained", pszName);
				GetProxy()->recv_direct(pktObtained);

				// Record to pickup history for the History dialog
				AddPickupHistoryEntry(pszName);

				// Increment session item count (only non-Zen items)
				InitHistoryCS();
				EnterCriticalSection(&g_csHistory);
				g_nSessionItemCount++;
				LeaveCriticalSection(&g_csHistory);
			}

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
	else if (pkt == CZenUpdatePacket::Type())
	{
		CZenUpdatePacket& pktZen = (CZenUpdatePacket&)pkt;
		DWORD dwNewZen = pktZen.GetZenAmount();

		WriteClickerLogFmt("PICKUP", "RECV CZenUpdatePacket: zen=%lu (prev=%lu, tracked=%d)",
			(unsigned long)dwNewZen, (unsigned long)m_dwLastZen, (int)m_fZenTracked);

		// If Zen is not yet tracked, try to initialize baseline from game memory.
		// This handles the case where the DLL was loaded after character selection
		// and the first CZenUpdatePacket arrives before we have a baseline.
		if (!m_fZenTracked)
		{
			DWORD dwMemZen = 0;
			if (ReadZenFromMemory(&dwMemZen) && dwMemZen > 0)
			{
				// Use the memory value as baseline if it is <= the packet value.
				// Equality is valid (no zen gained between memory read and packet).
				// If memory > packet, it's stale/inconsistent so skip it and
				// let the packet value become the baseline via m_dwLastZen below.
				if (dwMemZen <= dwNewZen)
				{
					m_dwLastZen = dwMemZen;
					m_fZenTracked = true;
					WriteClickerLogFmt("PICKUP", "First CZenUpdatePacket: baseline set from memory zen=%lu",
						(unsigned long)dwMemZen);
				}
			}
		}

		if (m_fZenTracked && dwNewZen > m_dwLastZen)
		{
			DWORD dwDelta = dwNewZen - m_dwLastZen;

			WriteClickerLogFmt("PICKUP", "Zen increased by %lu (new total: %lu)",
				(unsigned long)dwDelta, (unsigned long)dwNewZen);
			CDebugOut::PrintAlways("[PICKUP] Zen increased by %lu (total: %lu)",
				(unsigned long)dwDelta, (unsigned long)dwNewZen);

			// Display "[PICKUP] - Zen 'amount' Obtained" notification
			// Show when autopickup is enabled OR when the clicker session
			// is active, so the player always sees Zen pickup feedback.
			if (m_fEnabled || g_bSessionActive)
			{
				CServerMessagePacket pktMsg("[PICKUP] - Zen '%lu' Obtained", (unsigned long)dwDelta);
				GetProxy()->recv_direct(pktMsg);
			}

			// Record to pickup history for the History dialog
			char szHistory[64];
			sprintf_s(szHistory, sizeof(szHistory), "Zen '%lu'", (unsigned long)dwDelta);
			AddPickupHistoryEntry(szHistory);

			// Accumulate session zen total and pickup count when session
			// (clicker F5) is active, regardless of autopickup state.
			InitHistoryCS();
			EnterCriticalSection(&g_csHistory);
			if (g_bSessionActive)
			{
				g_ullSessionZenTotal += (unsigned __int64)dwDelta;
				g_nSessionZenPickupCount++;
			}
			LeaveCriticalSection(&g_csHistory);
		}

		m_dwLastZen = dwNewZen;
		m_fZenTracked = true;
	}
	else if (pkt == CKillExpPacket::Type())
	{
		// Track kills and experience gained for session statistics.
		// CKillExpPacket (C1:16) is sent when the player kills a monster.
		// Only count when the clicker session is active (clicker F5 running).
		CKillExpPacket& pktKill = (CKillExpPacket&)pkt;
		DWORD dwExp = pktKill.GetExperience();

		InitHistoryCS();
		EnterCriticalSection(&g_csHistory);
		if (g_bSessionActive)
		{
			g_nSessionKillCount++;
			g_ullSessionExpGained += (unsigned __int64)dwExp;
		}
		LeaveCriticalSection(&g_csHistory);
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
	if (_stricmp(pszParam, "zen_memory") == 0 && pData)
	{
		return ReadZenFromMemory((DWORD*)pData) != FALSE;
	}
	if (_stricmp(pszParam, "zen_last") == 0 && pData)
	{
		*((DWORD*)pData) = m_dwLastZen;
		return m_fZenTracked;
	}
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

		// When autopick is enabled, refresh Zen baseline from game memory
		// so that session tracking starts with the correct value.
		if (m_fEnabled)
		{
			DWORD dwMemZen = 0;
			if (ReadZenFromMemory(&dwMemZen))
			{
				m_dwLastZen = dwMemZen;
				m_fZenTracked = true;
				WriteClickerLogFmt("PICKUP", "SetParam: autopick ON -> refreshed m_dwLastZen=%lu from game memory",
					(unsigned long)m_dwLastZen);
			}
		}
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


/**
 * \brief Returns a human-readable display name for a MU Online item type code.
 *
 * Item type encoding: bits 0-7 = item index, bits 8-11 = item group (0-15).
 * Item database covers MU Online Season 3 Episode 1 items.
 */
const char* CAutoPickupFilter::GetItemDisplayName(WORD wType)
{
	BYTE bGroup = (wType >> 8) & 0x0F;
	BYTE bIndex = wType & 0xFF;

	switch (bGroup)
	{
	// ---------------------------------------------------------------
	// Group 0 - Swords
	// ---------------------------------------------------------------
	case 0:
		switch (bIndex)
		{
		case 0x01: return "Short Sword";
		case 0x02: return "Rapier";
		case 0x03: return "Katana";
		case 0x04: return "Sword of Assassin";
		case 0x05: return "Blade";
		case 0x06: return "Gladius";
		case 0x07: return "Falchion";
		case 0x08: return "Serpent Sword";
		case 0x09: return "Sword of Salamander";
		case 0x0A: return "Light Saber";
		case 0x0B: return "Legendary Sword";
		case 0x0C: return "Heliacal Sword";
		case 0x0D: return "Double Blade";
		case 0x0E: return "Lightning Sword";
		case 0x0F: return "Giant Sword";
		case 0x10: return "Sword of Destruction";
		case 0x11: return "Dark Breaker";
		case 0x12: return "Thunder Blade";
		case 0x13: return "Divine Sword of Archangel";
		case 0x14: return "Knight Blade";
		case 0x15: return "Dark Reign Blade";
		case 0x16: return "Bone Blade";
		case 0x17: return "Explosion Blade";
		case 0x18: return "Daybreak";
		case 0x19: return "Sword Dancer";
		case 0x1A: return "Flameberge";
		case 0x1B: return "Sword Breaker";
		case 0x1C: return "Rune Bastard Sword";
		case 0x1D: return "Sonic Blade";
		case 0x1E: return "Asura";
		case 0x1F: return "Rune Blade";
		case 0x20: return "Sacred Glove";
		case 0x21: return "Holy Storm Claw";
		case 0x22: return "Piercing Blade Glove";
		case 0x23: return "Phoenix Soul Star";
		case 0x24: return "Cyclone Sword";
		case 0x25: return "Blast Break";
		case 0x26: return "Prickle Glove";
		}
		return "Sword";

	// ---------------------------------------------------------------
	// Group 1 - Axes
	// ---------------------------------------------------------------
	case 1:
		switch (bIndex)
		{
		case 0x00: return "Small Axe";
		case 0x01: return "Hand Axe";
		case 0x02: return "Double Axe";
		case 0x03: return "Tomahawk";
		case 0x04: return "Elven Axe";
		case 0x05: return "Battle Axe";
		case 0x06: return "Nikea Axe";
		case 0x07: return "Larkan Axe";
		case 0x08: return "Crescent Axe";
		}
		return "Axe";

	// ---------------------------------------------------------------
	// Group 2 - Maces & Scepters
	// ---------------------------------------------------------------
	case 2:
		switch (bIndex)
		{
		case 0x00: return "Mace";
		case 0x01: return "Morning Star";
		case 0x02: return "Flail";
		case 0x03: return "Great Hammer";
		case 0x04: return "Crystal Morning Star";
		case 0x05: return "Crystal Sword";
		case 0x06: return "Chaos Dragon Axe";
		case 0x07: return "Elemental Mace";
		case 0x08: return "Battle Scepter";
		case 0x09: return "Master Scepter";
		case 0x0A: return "Great Scepter";
		case 0x0B: return "Lord Scepter";
		case 0x0C: return "Great Lord Scepter";
		case 0x0D: return "Divine Scepter of Archangel";
		case 0x0E: return "Soleil Scepter";
		case 0x0F: return "Shining Scepter";
		case 0x10: return "Frost Mace";
		case 0x11: return "Absolute Scepter";
		case 0x12: return "Striker Scepter";
		case 0x13: return "Thunderbolt";
		case 0x14: return "Horn of Steel";
		}
		return "Mace";

	// ---------------------------------------------------------------
	// Group 3 - Spears
	// ---------------------------------------------------------------
	case 3:
		switch (bIndex)
		{
		case 0x00: return "Light Spear";
		case 0x01: return "Spear";
		case 0x02: return "Dragon Lance";
		case 0x03: return "Giant Trident";
		case 0x04: return "Serpent Spear";
		case 0x05: return "Double Poleaxe";
		case 0x06: return "Halberd";
		case 0x07: return "Berdysh";
		case 0x08: return "Great Scythe";
		case 0x09: return "Bill of Balrog";
		case 0x0A: return "Dragon Spear";
		case 0x0B: return "Brova";
		case 0x0C: return "Magmus Peer";
		}
		return "Spear";

	// ---------------------------------------------------------------
	// Group 4 - Bows & Crossbows
	// ---------------------------------------------------------------
	case 4:
		switch (bIndex)
		{
		case 0x00: return "Short Bow";
		case 0x01: return "Bow";
		case 0x02: return "Elven Bow";
		case 0x03: return "Battle Bow";
		case 0x04: return "Tiger Bow";
		case 0x05: return "Silver Bow";
		case 0x06: return "Chaos Nature Bow";
		case 0x07: return "Bolt";
		case 0x08: return "Crossbow";
		case 0x09: return "Golden Crossbow";
		case 0x0A: return "Arquebus";
		case 0x0B: return "Light Crossbow";
		case 0x0C: return "Serpent Crossbow";
		case 0x0D: return "Bluewing Crossbow";
		case 0x0E: return "Aquagold Crossbow";
		case 0x0F: return "Arrow";
		case 0x10: return "Saint Crossbow";
		case 0x11: return "Celestial Bow";
		case 0x12: return "Divine Crossbow of Archangel";
		case 0x13: return "Great Reign Crossbow";
		case 0x14: return "Arrow Viper Bow";
		case 0x15: return "Sylph Wind Bow";
		case 0x16: return "Albatross Bow";
		case 0x17: return "Dark Stinger";
		case 0x18: return "Aileen Bow";
		case 0x19: return "Angelic Bow";
		case 0x1A: return "Devil Crossbow";
		}
		return "Bow";

	// ---------------------------------------------------------------
	// Group 5 - Staffs & Sticks & Books
	// ---------------------------------------------------------------
	case 5:
		switch (bIndex)
		{
		case 0x00: return "Skull Staff";
		case 0x01: return "Angelic Staff";
		case 0x02: return "Serpent Staff";
		case 0x03: return "Thunder Staff";
		case 0x04: return "Gorgon Staff";
		case 0x05: return "Legendary Staff";
		case 0x06: return "Staff of Resurrection";
		case 0x07: return "Chaos Lighting Staff";
		case 0x08: return "Staff of Destruction";
		case 0x09: return "Dragon Soul Staff";
		case 0x0A: return "Divine Staff of Archangel";
		case 0x0B: return "Staff of Kundun";
		case 0x0C: return "Grand Viper Staff";
		case 0x0D: return "Platina Staff";
		case 0x0E: return "Mystery Stick";
		case 0x0F: return "Violent Wind Stick";
		case 0x10: return "Red Wing Stick";
		case 0x11: return "Ancient Stick";
		case 0x12: return "Demonic Stick";
		case 0x13: return "Storm Blitz Stick";
		case 0x14: return "Eternal Wing Stick";
		case 0x15: return "Book of Samut";
		case 0x16: return "Book of Neil";
		case 0x17: return "Book of Lagle";
		case 0x1E: return "Deadly Staff";
		case 0x1F: return "Imperial Staff";
		case 0x20: return "Summon Spirit Stick";
		case 0x21: return "Chrome Staff";
		case 0x22: return "Raven Stick";
		case 0x23: return "Miracle Staff";
		case 0x24: return "Divine Stick of Archangel";
		case 0x25: return "Spite Staff";
		}
		return "Staff";

	// ---------------------------------------------------------------
	// Group 6 - Shields
	// ---------------------------------------------------------------
	case 6:
		switch (bIndex)
		{
		case 0x00: return "Small Shield";
		case 0x01: return "Horn Shield";
		case 0x02: return "Kite Shield";
		case 0x03: return "Elven Shield";
		case 0x04: return "Buckler";
		case 0x05: return "Dragon Slayer Shield";
		case 0x06: return "Skull Shield";
		case 0x07: return "Spiked Shield";
		case 0x08: return "Tower Shield";
		case 0x09: return "Plate Shield";
		case 0x0A: return "Large Round Shield";
		case 0x0B: return "Serpent Shield";
		case 0x0C: return "Bronze Shield";
		case 0x0D: return "Dragon Shield";
		case 0x0E: return "Legendary Shield";
		case 0x0F: return "Grand Soul Shield";
		case 0x10: return "Elemental Shield";
		case 0x11: return "Crimson Glory";
		case 0x12: return "Salamander Shield";
		case 0x13: return "Frost Barrier";
		case 0x14: return "Guardian Shield";
		case 0x15: return "Cross Shield";
		case 0x16: return "Lazy Wind Shield";
		case 0x17: return "Light Lord Shield";
		case 0x18: return "Dark Devil Shield";
		case 0x19: return "Magic Knight Shield";
		case 0x1A: return "Ambition Shield";
		}
		return "Shield";

	// ---------------------------------------------------------------
	// Group 7 - Helms
	// ---------------------------------------------------------------
	case 7:
		switch (bIndex)
		{
		case 0x00: return "Bronze Helm";
		case 0x01: return "Dragon Helm";
		case 0x02: return "Pad Helm";
		case 0x03: return "Legendary Helm";
		case 0x04: return "Bone Helm";
		case 0x05: return "Leather Helm";
		case 0x06: return "Scale Helm";
		case 0x07: return "Sphinx Mask";
		case 0x08: return "Brass Helm";
		case 0x09: return "Plate Helm";
		case 0x0A: return "Vine Helm";
		case 0x0B: return "Silk Helm";
		case 0x0C: return "Wind Helm";
		case 0x0D: return "Spirit Helm";
		case 0x0E: return "Guardian Helm";
		case 0x10: return "Black Dragon Helm";
		case 0x11: return "Dark Phoenix Helm";
		case 0x12: return "Grand Soul Helm";
		case 0x13: return "Holy Spirit Helm";
		case 0x15: return "Great Dragon Helm";
		case 0x16: return "Dark Soul Helm";
		case 0x18: return "Red Spirit Helm";
		case 0x19: return "Light Plate Mask";
		case 0x1A: return "Adamantine Mask";
		case 0x1B: return "Dark Steel Mask";
		case 0x1C: return "Dark Master Mask";
		case 0x1D: return "Dragon Knight Helm";
		case 0x1E: return "Venom Mist Helm";
		case 0x1F: return "Sylphid Ray Helm";
		case 0x21: return "Sunlight Mask";
		case 0x22: return "Ashcrow Helm";
		case 0x23: return "Eclipse Helm";
		case 0x24: return "Iris Helm";
		case 0x26: return "Glorious Mask";
		case 0x27: return "Violent Wind Helm";
		case 0x28: return "Red Wing Helm";
		case 0x29: return "Ancient Helm";
		case 0x2A: return "Demonic Helm";
		case 0x2B: return "Storm Blitz Helm";
		case 0x2C: return "Eternal Wing Helm";
		case 0x2D: return "Titan Helm";
		case 0x2E: return "Brave Helm";
		case 0x31: return "Seraphim Helm";
		case 0x32: return "Divine Helm";
		case 0x33: return "Royal Mask";
		case 0x34: return "Hades Helm";
		case 0x35: return "Succubus Helm";
		case 0x3B: return "Sacred Fire Helm";
		case 0x3C: return "Storm Jahad Helm";
		case 0x3D: return "Piercing Helm";
		case 0x49: return "Phoenix Soul Helmet";
		}
		return "Helm";

	// ---------------------------------------------------------------
	// Group 8 - Armors
	// ---------------------------------------------------------------
	case 8:
		switch (bIndex)
		{
		case 0x00: return "Bronze Armor";
		case 0x01: return "Dragon Armor";
		case 0x02: return "Pad Armor";
		case 0x03: return "Legendary Armor";
		case 0x04: return "Bone Armor";
		case 0x05: return "Leather Armor";
		case 0x06: return "Scale Armor";
		case 0x07: return "Sphinx Armor";
		case 0x08: return "Brass Armor";
		case 0x09: return "Plate Armor";
		case 0x0A: return "Vine Armor";
		case 0x0B: return "Silk Armor";
		case 0x0C: return "Wind Armor";
		case 0x0D: return "Spirit Armor";
		case 0x0E: return "Guardian Armor";
		case 0x0F: return "Storm Crow Armor";
		case 0x10: return "Black Dragon Armor";
		case 0x11: return "Dark Phoenix Armor";
		case 0x12: return "Grand Soul Armor";
		case 0x13: return "Holy Spirit Armor";
		case 0x14: return "Thunder Hawk Armor";
		case 0x15: return "Great Dragon Armor";
		case 0x16: return "Dark Soul Armor";
		case 0x17: return "Hurricane Armor";
		case 0x18: return "Red Spirit Armor";
		case 0x19: return "Light Plate Armor";
		case 0x1A: return "Adamantine Armor";
		case 0x1B: return "Dark Steel Armor";
		case 0x1C: return "Dark Master Armor";
		case 0x1D: return "Dragon Knight Armor";
		case 0x1E: return "Venom Mist Armor";
		case 0x1F: return "Sylphid Ray Armor";
		case 0x20: return "Volcano Armor";
		case 0x21: return "Sunlight Armor";
		case 0x22: return "Ashcrow Armor";
		case 0x23: return "Eclipse Armor";
		case 0x24: return "Iris Armor";
		case 0x25: return "Valiant Armor";
		case 0x26: return "Glorious Armor";
		case 0x27: return "Violent Wind Armor";
		case 0x28: return "Red Wing Armor";
		case 0x29: return "Ancient Armor";
		case 0x2A: return "Demonic Armor";
		case 0x2B: return "Storm Blitz Armor";
		case 0x2C: return "Eternal Wing Armor";
		case 0x2D: return "Titan Armor";
		case 0x2E: return "Brave Armor";
		case 0x2F: return "Phantom Armor";
		case 0x30: return "Destroy Armor";
		case 0x31: return "Seraphim Armor";
		case 0x32: return "Divine Armor";
		case 0x33: return "Royal Armor";
		case 0x34: return "Hades Armor";
		case 0x35: return "Succubus Armor";
		case 0x3B: return "Sacred Fire Armor";
		case 0x3C: return "Storm Jahad Armor";
		case 0x3D: return "Piercing Armor";
		case 0x49: return "Phoenix Soul Armor";
		}
		return "Armor";

	// ---------------------------------------------------------------
	// Group 9 - Pants
	// ---------------------------------------------------------------
	case 9:
		switch (bIndex)
		{
		case 0x00: return "Bronze Pants";
		case 0x01: return "Dragon Pants";
		case 0x02: return "Pad Pants";
		case 0x03: return "Legendary Pants";
		case 0x04: return "Bone Pants";
		case 0x05: return "Leather Pants";
		case 0x06: return "Scale Pants";
		case 0x07: return "Sphinx Pants";
		case 0x08: return "Brass Pants";
		case 0x09: return "Plate Pants";
		case 0x0A: return "Vine Pants";
		case 0x0B: return "Silk Pants";
		case 0x0C: return "Wind Pants";
		case 0x0D: return "Spirit Pants";
		case 0x0E: return "Guardian Pants";
		case 0x0F: return "Storm Crow Pants";
		case 0x10: return "Black Dragon Pants";
		case 0x11: return "Dark Phoenix Pants";
		case 0x12: return "Grand Soul Pants";
		case 0x13: return "Holy Spirit Pants";
		case 0x14: return "Thunder Hawk Pants";
		case 0x15: return "Great Dragon Pants";
		case 0x16: return "Dark Soul Pants";
		case 0x17: return "Hurricane Pants";
		case 0x18: return "Red Spirit Pants";
		case 0x19: return "Light Plate Pants";
		case 0x1A: return "Adamantine Pants";
		case 0x1B: return "Dark Steel Pants";
		case 0x1C: return "Dark Master Pants";
		case 0x1D: return "Dragon Knight Pants";
		case 0x1E: return "Venom Mist Pants";
		case 0x1F: return "Sylphid Ray Pants";
		case 0x20: return "Volcano Pants";
		case 0x21: return "Sunlight Pants";
		case 0x22: return "Ashcrow Pants";
		case 0x23: return "Eclipse Pants";
		case 0x24: return "Iris Pants";
		case 0x25: return "Valiant Pants";
		case 0x26: return "Glorious Pants";
		case 0x27: return "Violent Wind Pants";
		case 0x28: return "Red Wing Pants";
		case 0x29: return "Ancient Pants";
		case 0x2A: return "Demonic Pants";
		case 0x2B: return "Storm Blitz Pants";
		case 0x2C: return "Eternal Wing Pants";
		case 0x2D: return "Titan Pants";
		case 0x2E: return "Brave Pants";
		case 0x2F: return "Phantom Pants";
		case 0x30: return "Destroy Pants";
		case 0x31: return "Seraphim Pants";
		case 0x32: return "Divine Pants";
		case 0x33: return "Royal Pants";
		case 0x34: return "Hades Pants";
		case 0x35: return "Succubus Pants";
		case 0x3B: return "Sacred Fire Pants";
		case 0x3C: return "Storm Jahad Pants";
		case 0x3D: return "Piercing Pants";
		case 0x49: return "Phoenix Soul Pants";
		}
		return "Pants";

	// ---------------------------------------------------------------
	// Group 10 - Gloves
	// ---------------------------------------------------------------
	case 10:
		switch (bIndex)
		{
		case 0x00: return "Bronze Gloves";
		case 0x01: return "Dragon Gloves";
		case 0x02: return "Pad Gloves";
		case 0x03: return "Legendary Gloves";
		case 0x04: return "Bone Gloves";
		case 0x05: return "Leather Gloves";
		case 0x06: return "Scale Gloves";
		case 0x07: return "Sphinx Gloves";
		case 0x08: return "Brass Gloves";
		case 0x09: return "Plate Gloves";
		case 0x0A: return "Vine Gloves";
		case 0x0B: return "Silk Gloves";
		case 0x0C: return "Wind Gloves";
		case 0x0D: return "Spirit Gloves";
		case 0x0E: return "Guardian Gloves";
		case 0x0F: return "Storm Crow Gloves";
		case 0x10: return "Black Dragon Gloves";
		case 0x11: return "Dark Phoenix Gloves";
		case 0x12: return "Grand Soul Gloves";
		case 0x13: return "Holy Spirit Gloves";
		case 0x14: return "Thunder Hawk Gloves";
		case 0x15: return "Great Dragon Gloves";
		case 0x16: return "Dark Soul Gloves";
		case 0x17: return "Hurricane Gloves";
		case 0x18: return "Red Spirit Gloves";
		case 0x19: return "Light Plate Gloves";
		case 0x1A: return "Adamantine Gloves";
		case 0x1B: return "Dark Steel Gloves";
		case 0x1C: return "Dark Master Gloves";
		case 0x1D: return "Dragon Knight Gloves";
		case 0x1E: return "Venom Mist Gloves";
		case 0x1F: return "Sylphid Ray Gloves";
		case 0x20: return "Volcano Gloves";
		case 0x21: return "Sunlight Gloves";
		case 0x22: return "Ashcrow Gloves";
		case 0x23: return "Eclipse Gloves";
		case 0x24: return "Iris Gloves";
		case 0x25: return "Valiant Gloves";
		case 0x26: return "Glorious Gloves";
		case 0x27: return "Violent Wind Gloves";
		case 0x28: return "Red Wing Gloves";
		case 0x29: return "Ancient Gloves";
		case 0x2A: return "Demonic Gloves";
		case 0x2B: return "Storm Blitz Gloves";
		case 0x2C: return "Eternal Wing Gloves";
		case 0x2D: return "Titan Gloves";
		case 0x2E: return "Brave Gloves";
		case 0x2F: return "Phantom Gloves";
		case 0x30: return "Destroy Gloves";
		case 0x31: return "Seraphim Gloves";
		case 0x32: return "Divine Gloves";
		case 0x33: return "Royal Gloves";
		case 0x34: return "Hades Gloves";
		case 0x35: return "Succubus Gloves";
		}
		return "Gloves";

	// ---------------------------------------------------------------
	// Group 11 - Boots
	// ---------------------------------------------------------------
	case 11:
		switch (bIndex)
		{
		case 0x00: return "Bronze Boots";
		case 0x01: return "Dragon Boots";
		case 0x02: return "Pad Boots";
		case 0x03: return "Legendary Boots";
		case 0x04: return "Bone Boots";
		case 0x05: return "Leather Boots";
		case 0x06: return "Scale Boots";
		case 0x07: return "Sphinx Boots";
		case 0x08: return "Brass Boots";
		case 0x09: return "Plate Boots";
		case 0x0A: return "Vine Boots";
		case 0x0B: return "Silk Boots";
		case 0x0C: return "Wind Boots";
		case 0x0D: return "Spirit Boots";
		case 0x0E: return "Guardian Boots";
		case 0x0F: return "Storm Crow Boots";
		case 0x10: return "Black Dragon Boots";
		case 0x11: return "Dark Phoenix Boots";
		case 0x12: return "Grand Soul Boots";
		case 0x13: return "Holy Spirit Boots";
		case 0x14: return "Thunder Hawk Boots";
		case 0x15: return "Great Dragon Boots";
		case 0x16: return "Dark Soul Boots";
		case 0x17: return "Hurricane Boots";
		case 0x18: return "Red Spirit Boots";
		case 0x19: return "Light Plate Boots";
		case 0x1A: return "Adamantine Boots";
		case 0x1B: return "Dark Steel Boots";
		case 0x1C: return "Dark Master Boots";
		case 0x1D: return "Dragon Knight Boots";
		case 0x1E: return "Venom Mist Boots";
		case 0x1F: return "Sylphid Ray Boots";
		case 0x20: return "Volcano Boots";
		case 0x21: return "Sunlight Boots";
		case 0x22: return "Ashcrow Boots";
		case 0x23: return "Eclipse Boots";
		case 0x24: return "Iris Boots";
		case 0x25: return "Valiant Boots";
		case 0x26: return "Glorious Boots";
		case 0x27: return "Violent Wind Boots";
		case 0x28: return "Red Wing Boots";
		case 0x29: return "Ancient Boots";
		case 0x2A: return "Demonic Boots";
		case 0x2B: return "Storm Blitz Boots";
		case 0x2C: return "Eternal Wing Boots";
		case 0x2D: return "Titan Boots";
		case 0x2E: return "Brave Boots";
		case 0x2F: return "Phantom Boots";
		case 0x30: return "Destroy Boots";
		case 0x31: return "Seraphim Boots";
		case 0x32: return "Divine Boots";
		case 0x33: return "Royal Boots";
		case 0x34: return "Hades Boots";
		case 0x35: return "Succubus Boots";
		case 0x3B: return "Sacred Fire Boots";
		case 0x3C: return "Storm Jahad Boots";
		case 0x3D: return "Piercing Boots";
		case 0x49: return "Phoenix Soul Boots";
		}
		return "Boots";

	// ---------------------------------------------------------------
	// Group 12 - Wings / Capes / Orbs / Special
	// ---------------------------------------------------------------
	case 12:
		switch (bIndex)
		{
		case 0x00: return "Wings of Elf";
		case 0x01: return "Wings of Heaven";
		case 0x02: return "Wings of Satan";
		case 0x03: return "Wings of Spirits";
		case 0x04: return "Wings of Soul";
		case 0x05: return "Wings of Dragon";
		case 0x06: return "Wings of Darkness";
		case 0x07: return "Cape of Lord";
		case 0x08: return "Healing Orb";
		case 0x09: return "Orb of Greater Fortitude";
		case 0x0A: return "Dinorant";
		case 0x0B: return "Dark Horse";
		case 0x0C: return "Dark Raven";
		case 0x0D: return "Horn of Uniria";
		case 0x0E: return "Spirit of Guardian";
		case 0x0F: return "Jewel of Chaos";
		case 0x10: return "Orb of Fire Slash";
		case 0x11: return "Orb of Penetration";
		case 0x12: return "Orb of Ice Arrow";
		case 0x13: return "Orb of Death Stab";
		case 0x15: return "Scroll of FireBurst";
		case 0x16: return "Scroll of Summon";
		case 0x17: return "Scroll of Critical Damage";
		case 0x18: return "Scroll of Electric Spark";
		case 0x1A: return "Gem of Secret";
		case 0x1E: return "Bundle of Jewel of Bless";
		case 0x1F: return "Bundle of Jewel of Soul";
		case 0x23: return "Scroll of Fire Scream";
		case 0x24: return "Wings of Storm";
		case 0x25: return "Wings of Eternal";
		case 0x26: return "Wings of Illusion";
		case 0x27: return "Wings of Ruin";
		case 0x28: return "Cape of Emperor";
		case 0x29: return "Wings of Dimension";
		case 0x2A: return "Cape of Fighter";
		case 0x2B: return "Wing of Curse";
		case 0x2C: return "Crystal of Destruction";
		case 0x2D: return "Crystal of Multi-Shot";
		case 0x2E: return "Crystal of Recovery";
		case 0x2F: return "Crystal of Flame Strike";
		case 0x30: return "Small Cape of Lord";
		case 0x31: return "Small Wings of Mystery";
		case 0x32: return "Small Wings of Elf";
		case 0x33: return "Small Wings of Heaven";
		case 0x34: return "Small Wings of Satan";
		case 0x35: return "Little Warrior's Cloak";
		}
		return "Wings";

	// ---------------------------------------------------------------
	// Group 13 - Misc (Pets, Rings, Pendants, Event)
	// ---------------------------------------------------------------
	case 13:
		switch (bIndex)
		{
		case 0x00: return "Guardian Angel";
		case 0x01: return "Imp";
		case 0x02: return "Horn of Uniria";
		case 0x03: return "Horn of Dinorant";
		case 0x04: return "Dark Horse";
		case 0x05: return "Dark Raven";
		case 0x07: return "Contract of Summon";
		case 0x08: return "Ring of Ice";
		case 0x09: return "Ring of Poison";
		case 0x0A: return "Transformation Ring";
		case 0x0B: return "Order of Guardian/Life Stone";
		case 0x0C: return "Pendant of Lightning";
		case 0x0D: return "Pendant of Fire";
		case 0x0E: return "Loch's Feather";
		case 0x0F: return "Fruits";
		case 0x10: return "Scroll of Archangel";
		case 0x11: return "Blood Bone";
		case 0x12: return "Invisibility Cloak";
		case 0x13: return "Weapon of Archangel";
		case 0x14: return "Wizards Ring";
		case 0x15: return "Ring of Fire";
		case 0x16: return "Ring of Earth";
		case 0x17: return "Ring of Wind";
		case 0x18: return "Ring of Magic";
		case 0x19: return "Pendant of Ice";
		case 0x1A: return "Pendant of Wind";
		case 0x1B: return "Pendant of Water";
		case 0x1C: return "Pendant of Ability";
		case 0x1D: return "Armor of Guardsman";
		case 0x1E: return "Cape of Lord";
		case 0x1F: return "Spirit";
		case 0x20: return "Splinter of Armor";
		case 0x21: return "Bless of Guardian";
		case 0x22: return "Claw of Beast";
		case 0x23: return "Fragment of Horn";
		case 0x24: return "Broken Horn";
		case 0x25: return "Horn of Fenrir";
		case 0x26: return "Moonstone Pendant";
		case 0x2E: return "Devil Square Ticket";
		case 0x2F: return "Blood Castle Ticket";
		case 0x30: return "Kalima Ticket";
		case 0x31: return "Old Scroll";
		case 0x32: return "Illusion Sorcerer Covenant";
		case 0x33: return "Scroll of Blood";
		case 0x34: return "Condor Flame";
		case 0x35: return "Condor Feather";
		case 0x40: return "Demon";
		case 0x41: return "Spirit of Guardian";
		case 0x43: return "Pet Rudolf";
		case 0x45: return "Talisman of Resurrection";
		case 0x50: return "Pet Panda";
		}
		return "Misc";

	// ---------------------------------------------------------------
	// Group 14 - Consumables & Jewels
	// ---------------------------------------------------------------
	case 14:
		switch (bIndex)
		{
		case 0x00: return "Apple";
		case 0x01: return "Small Healing Potion";
		case 0x02: return "Medium Healing Potion";
		case 0x03: return "Large Healing Potion";
		case 0x04: return "Small Mana Potion";
		case 0x05: return "Medium Mana Potion";
		case 0x06: return "Large Mana Potion";
		case 0x07: return "Siege Potion";
		case 0x08: return "Antidote";
		case 0x09: return "Ale";
		case 0x0A: return "Town Portal Scroll";
		case 0x0B: return "Armor of Guardsman";
		case 0x0C: return "Wizards Ring";
		case 0x0D: return "Jewel of Bless";
		case 0x0E: return "Jewel of Soul";
		case 0x0F: return "Zen";
		case 0x10: return "Jewel of Life";
		case 0x11: return "Scroll of Archangel";
		case 0x12: return "Box of Kundun +1";
		case 0x13: return "Box of Kundun +2";
		case 0x14: return "Box of Kundun +3";
		case 0x15: return "Box of Kundun +4";
		case 0x16: return "Jewel of Creation";
		case 0x17: return "Box of Kundun +5";
		case 0x18: return "Broken Sword";
		case 0x19: return "Tear of Elf";
		case 0x1A: return "Symbol of Kundun";
		case 0x1B: return "Lost Map";
		case 0x1C: return "Loch's Feather";
		case 0x1D: return "Gemstone";
		case 0x1E: return "Jewel of Harmony";
		case 0x1F: return "Jewel of Guardian";
		case 0x20: return "Crest of Monarch";
		case 0x21: return "Rena";
		case 0x22: return "Fruits";
		case 0x23: return "Blue Chocolate Box";
		case 0x24: return "Pink Chocolate Box";
		case 0x25: return "Red Chocolate Box";
		case 0x26: return "Firecracker";
		case 0x27: return "Small SD Potion";
		case 0x28: return "SD Potion";
		case 0x29: return "Large SD Potion";
		case 0x2A: return "Cherry Blossom Wine";
		case 0x2B: return "Cherry Blossom Rice Cake";
		case 0x2C: return "Cherry Blossom Flower Petal";
		case 0x2D: return "Summoner Card";
		case 0x2E: return "Scroll of Emperor";
		case 0x2F: return "Lower Refining Stone";
		case 0x30: return "Higher Refining Stone";
		case 0x31: return "Old Scroll";
		case 0x32: return "Illusion Sorcerer Covenant";
		case 0x33: return "Scroll of Blood";
		case 0x35: return "Condor Feather";
		case 0x36: return "Condor Flame";
		case 0x37: return "Horn of Fenrir";
		case 0x38: return "Broken Horn";
		case 0x39: return "Jewel of Chaos";
		case 0x3A: return "Small Complex Potion";
		case 0x3B: return "Complex Potion";
		case 0x3C: return "Large Complex Potion";
		case 0x46: return "Elite Healing Potion";
		case 0x47: return "Elite Mana Potion";
		case 0x4E: return "Scroll of Quickness";
		case 0x4F: return "Scroll of Defense";
		case 0x50: return "Scroll of Wrath";
		case 0x51: return "Scroll of Wizardry";
		case 0x52: return "Scroll of Health";
		case 0x53: return "Scroll of Mana";
		case 0x64: return "Lucky Coin";
		case 0x6E: return "Sign of Dimensions";
		case 0x6F: return "Mirror of Dimensions";
		case 0x85: return "Elite SD Potion";
		case 0x8C: return "Scroll of Healing";
		case 0xA0: return "Jewel of Extension";
		case 0xA1: return "Jewel of Elevation";
		}
		return "Item";

	// ---------------------------------------------------------------
	// Group 15 - Scrolls & Parchments
	// ---------------------------------------------------------------
	case 15:
		switch (bIndex)
		{
		case 0x00: return "Scroll of Poison";
		case 0x01: return "Scroll of Meteorite";
		case 0x02: return "Scroll of Lightning";
		case 0x03: return "Scroll of Fire Ball";
		case 0x04: return "Scroll of Flame";
		case 0x05: return "Scroll of Teleport";
		case 0x06: return "Scroll of Ice";
		case 0x07: return "Scroll of Twister";
		case 0x08: return "Scroll of Evil Spirit";
		case 0x09: return "Scroll of Hellfire";
		case 0x0A: return "Scroll of Power Wave";
		case 0x0B: return "Scroll of Aqua Beam";
		case 0x0C: return "Scroll of Cometfall";
		case 0x0D: return "Scroll of Inferno";
		case 0x0E: return "Scroll of Teleport Ally";
		case 0x0F: return "Scroll of Soul Barrier";
		case 0x10: return "Scroll of Decay";
		case 0x11: return "Scroll of Ice Storm";
		case 0x12: return "Scroll of Nova";
		case 0x13: return "Chain Lightning Parchment";
		case 0x14: return "Drain Life Parchment";
		case 0x15: return "Lightning Shock Parchment";
		case 0x16: return "Damage Reflection Parchment";
		case 0x17: return "Berserker Parchment";
		case 0x18: return "Sleep Parchment";
		case 0x1A: return "Weakness Parchment";
		case 0x1B: return "Innovation Parchment";
		case 0x1C: return "Scroll of Wizardry Enhance";
		case 0x1D: return "Scroll of Gigantic Storm";
		case 0x1E: return "Chain Drive Parchment";
		case 0x1F: return "Dark Side Parchment";
		case 0x20: return "Dragon Roar Parchment";
		case 0x21: return "Dragon Slasher Parchment";
		case 0x22: return "Ignore Defense Parchment";
		case 0x23: return "Increase Health Parchment";
		case 0x24: return "Increase Block Parchment";
		}
		return "Scroll";

	default:
		break;
	}

	return "Item";
}
