#include "stdafx.h"
#include "PacketLogger.h"
#include "CommonPackets.h"
#include "DebugOut.h"
#include "BufferUtil.h"
#include "ClickerLogger.h"
#include "HistoryCategory.h"

// Defined in AutoPickupFilter.cpp — adds an entry to the shared pickup history buffer.
extern void AddPickupHistoryEntry(const char* pszItemName, int nCategory);

/**
 * \brief Check if a recv packet is relevant to the AVANTA+ELITE autoclicker
 *
 * Only these server-to-client packets matter for autoclicker operation:
 * - CMeetItemPacket:        Item drops detection (autopickup)
 * - CForgetItemPacket:      Item disappear tracking (autopickup)
 * - CUpdatePosSTCPacket:    Server position sync (movement/pickup walk)
 * - CWarpReplyPacket:       Warp detection (autopickup reset)
 * - CObjectDeathPacket:     Monster death (pickup timing)
 * - CLevelUpPacket:         Level tracking (exit at lvl 400)
 * - CServerMessagePacket:   DLL command responses
 * - CGMMessagePacket:       GM announcements
 * - CCharSetupPacket:       Character setup (class detection)
 * - CCharStatsPacket:       Character stats (HP/level tracking)
 * - CCharRespawnPacket:     Character respawn (state reset)
 */
static bool IsRelevantRecvPacket(CPacket& pkt)
{
	if (pkt == CMeetItemPacket::Type())        return true;
	if (pkt == CForgetItemPacket::Type())      return true;
	if (pkt == CUpdatePosSTCPacket::Type())    return true;
	if (pkt == CWarpReplyPacket::Type())       return true;
	if (pkt == CObjectDeathPacket::Type())     return true;
	if (pkt == CLevelUpPacket::Type())         return true;
	if (pkt == CServerMessagePacket::Type())   return true;
	if (pkt == CGMMessagePacket::Type())       return true;
	if (pkt == CCharSetupPacket::Type())       return true;
	if (pkt == CCharStatsPacket::Type())       return true;
	if (pkt == CCharRespawnPacket::Type())     return true;
	if (pkt == CPickItemResultFailPacket::Type()) return true;
	if (pkt == CZenUpdatePacket::Type())       return true;

	return false;
}

/**
 * \brief Check if a send packet is relevant to the AVANTA+ELITE autoclicker
 *
 * Only these client-to-server packets matter for autoclicker operation:
 * - CCharacterSayPacket:    DLL command interface (//autopick, //set_pick_opt, etc.)
 * - CUpdatePosCTSPacket:    Position update (movement/pickup walk)
 * - CCharMoveCTSPacket:     Character walk (movement to item)
 * - CPickItemPacket:        Item pickup request
 * - CMassiveSkillPacket:    Skill usage (blocked during pickup walk)
 * - CUseItemPacket:         Item use (heal potion, repair)
 * - CNormalAttackPacket:    Normal attack (autoclicker right-click)
 */
static bool IsRelevantSendPacket(CPacket& pkt)
{
	if (pkt == CCharacterSayPacket::Type())    return true;
	if (pkt == CCharacterSayPacket2::Type())   return true;
	if (pkt == CCharacterSayPacket3::Type())   return true;
	if (pkt == CUpdatePosCTSPacket::Type())    return true;
	if (pkt == CCharMoveCTSPacket::Type())     return true;
	if (pkt == CPickItemPacket::Type())        return true;
	if (pkt == CMassiveSkillPacket::Type())    return true;
	if (pkt == CUseItemPacket::Type())         return true;
	if (pkt == CNormalAttackPacket::Type())    return true;

	return false;
}

/**
 * \brief Flood protection: rate-limit logging for high-frequency packet types.
 *        Returns true if this packet type should be throttled (skip logging).
 *        Packets that repeat very often (position updates, attacks, skills)
 *        are logged at most once per THROTTLE_INTERVAL_MS to prevent log
 *        flood that causes I/O bottleneck and server disconnection.
 */
static const DWORD THROTTLE_INTERVAL_MS = 1000;

enum ThrottleCategory
{
	THROTTLE_NONE = -1,
	THROTTLE_UPDATE_POS_STC = 0,
	THROTTLE_UPDATE_POS_CTS = 1,
	THROTTLE_MASSIVE_SKILL = 2,
	THROTTLE_NORMAL_ATTACK = 3,
	THROTTLE_OBJECT_DEATH = 4,
	THROTTLE_CATEGORY_COUNT = 5
};

static ThrottleCategory GetThrottleCategory(CPacket& pkt)
{
	if (pkt == CUpdatePosSTCPacket::Type())   return THROTTLE_UPDATE_POS_STC;
	if (pkt == CUpdatePosCTSPacket::Type())   return THROTTLE_UPDATE_POS_CTS;
	if (pkt == CMassiveSkillPacket::Type())   return THROTTLE_MASSIVE_SKILL;
	if (pkt == CNormalAttackPacket::Type())   return THROTTLE_NORMAL_ATTACK;
	if (pkt == CObjectDeathPacket::Type())    return THROTTLE_OBJECT_DEATH;
	return THROTTLE_NONE;
}

static bool IsThrottled(ThrottleCategory cat, DWORD* pLastLogTime, DWORD* pSkipCount)
{
	if (cat == THROTTLE_NONE)
		return false;

	DWORD dwNow = GetTickCount();
	DWORD dwLast = pLastLogTime[cat];

	if (dwNow - dwLast < THROTTLE_INTERVAL_MS)
	{
		pSkipCount[cat]++;
		return true;
	}

	pLastLogTime[cat] = dwNow;
	pSkipCount[cat] = 0;
	return false;
}


/**
 *
 */
CPacketLogger::CPacketLogger(CProxy* pProxy)
	: CPacketFilter(pProxy)
{
	CDebugOut::PrintAlways("[PACKET_LOG] Packet logger filter initialized (AVANTA+ELITE mode).");
	WriteClickerLogFmt("PACKET", "Packet logger filter initialized - AVANTA+ELITE relevant packets only");
}

/**
 *
 */
CPacketLogger::~CPacketLogger()
{
	CDebugOut::PrintAlways("[PACKET_LOG] Packet logger filter destroyed.");
	WriteClickerLogFmt("PACKET", "Packet logger filter destroyed - monitoring stopped");
}

/**
 * 
 */
const char* CPacketLogger::GetName()
{
	return "PacketLogger";
}

/**
 *
 */
WORD CPacketLogger::GetLevel()
{
	return FILTER_LEVEL_LOGGER;
}

/**
 * \brief Logs only AVANTA+ELITE relevant server-to-client packets.
 *        High-frequency non-pickup packet types are rate-limited to prevent
 *        log flood that causes I/O bottleneck and server disconnection.
 *        Pickup-related packets (MeetItem, ForgetItem, etc.) are always logged.
 */
int CPacketLogger::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	if (!IsRelevantRecvPacket(pkt))
		return 0;

	static DWORD s_dwRecvLastLog[THROTTLE_CATEGORY_COUNT] = {0};
	static DWORD s_dwRecvSkipCount[THROTTLE_CATEGORY_COUNT] = {0};

	ThrottleCategory cat = GetThrottleCategory(pkt);
	DWORD dwSkipped = (cat != THROTTLE_NONE) ? s_dwRecvSkipCount[cat] : 0;

	if (IsThrottled(cat, s_dwRecvLastLog, s_dwRecvSkipCount))
		return 0;

	CStringA szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());

	if (dwSkipped > 0)
	{
		CDebugOut::PrintAlways("[PACKET] SERVER -> CLIENT | %s | Len=%d | %s (throttled: %d similar packets skipped)",
					pkt.GetType().GetDescription(),
					pkt.GetDecryptedLen(),
					(const char*)szHex, dwSkipped);

		WriteClickerLogFmt("PACKET", "SERVER -> CLIENT | %s | Len=%d | %s (throttled: %d similar packets skipped)",
			pkt.GetType().GetDescription(), pkt.GetDecryptedLen(), (const char*)szHex, dwSkipped);
	}
	else
	{
		CDebugOut::PrintAlways("[PACKET] SERVER -> CLIENT | %s | Len=%d | %s", 
					pkt.GetType().GetDescription(),
					pkt.GetDecryptedLen(),
					(const char*)szHex);

		WritePacketLog("SERVER -> CLIENT", pkt.GetType().GetDescription(), 
					   pkt.GetDecryptedLen(), (const char*)szHex);
	}

	// Record to history for the Debug Console (Server category)
	{
		char szEntry[192];
		sprintf_s(szEntry, sizeof(szEntry), "S: %s (Len=%d)",
			pkt.GetType().GetDescription(), pkt.GetDecryptedLen());
		AddPickupHistoryEntry(szEntry, HISTORY_CAT_SERVER);
	}

	return 0;
}

/**
 * \brief Logs only AVANTA+ELITE relevant client-to-server packets.
 *        High-frequency non-pickup packet types are rate-limited to prevent
 *        log flood that causes I/O bottleneck and server disconnection.
 *        Pickup-related packets (PickItem, CharacterSay, etc.) are always logged.
 */
int CPacketLogger::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	if (!IsRelevantSendPacket(pkt))
		return 0;

	static DWORD s_dwSendLastLog[THROTTLE_CATEGORY_COUNT] = {0};
	static DWORD s_dwSendSkipCount[THROTTLE_CATEGORY_COUNT] = {0};

	ThrottleCategory cat = GetThrottleCategory(pkt);
	DWORD dwSkipped = (cat != THROTTLE_NONE) ? s_dwSendSkipCount[cat] : 0;

	if (IsThrottled(cat, s_dwSendLastLog, s_dwSendSkipCount))
		return 0;

	CStringA szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());

	if (dwSkipped > 0)
	{
		CDebugOut::PrintAlways("[PACKET] CLIENT -> SERVER | %s | Len=%d | %s (throttled: %d similar packets skipped)",
					pkt.GetType().GetDescription(),
					pkt.GetDecryptedLen(),
					(const char*)szHex, dwSkipped);

		WriteClickerLogFmt("PACKET", "CLIENT -> SERVER | %s | Len=%d | %s (throttled: %d similar packets skipped)",
			pkt.GetType().GetDescription(), pkt.GetDecryptedLen(), (const char*)szHex, dwSkipped);
	}
	else
	{
		CDebugOut::PrintAlways("[PACKET] CLIENT -> SERVER | %s | Len=%d | %s", 
					pkt.GetType().GetDescription(),
					pkt.GetDecryptedLen(),
					(const char*)szHex);

		WritePacketLog("CLIENT -> SERVER", pkt.GetType().GetDescription(),
					   pkt.GetDecryptedLen(), (const char*)szHex);
	}

	// Record to history for the Debug Console (Client category)
	{
		char szEntry[192];
		sprintf_s(szEntry, sizeof(szEntry), "C: %s (Len=%d)",
			pkt.GetType().GetDescription(), pkt.GetDecryptedLen());
		AddPickupHistoryEntry(szEntry, HISTORY_CAT_CLIENT);
	}

	return 0;
}
