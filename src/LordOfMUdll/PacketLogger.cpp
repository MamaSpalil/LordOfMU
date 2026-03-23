#include "stdafx.h"
#include "PacketLogger.h"
#include "CommonPackets.h"
#include "DebugOut.h"
#include "BufferUtil.h"
#include "ClickerLogger.h"

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
 * \brief Logs only AVANTA+ELITE relevant server-to-client packets
 */
int CPacketLogger::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	if (!IsRelevantRecvPacket(pkt))
		return 0;

	const char* szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());

	CDebugOut::PrintAlways("[PACKET] SERVER -> CLIENT | %s | Len=%d | %s", 
				pkt.GetType().GetDescription(),
				pkt.GetDecryptedLen(),
				szHex);

	WritePacketLog("SERVER -> CLIENT", pkt.GetType().GetDescription(), 
				   pkt.GetDecryptedLen(), szHex);

	return 0;
}

/**
 * \brief Logs only AVANTA+ELITE relevant client-to-server packets
 */
int CPacketLogger::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	if (!IsRelevantSendPacket(pkt))
		return 0;

	BYTE* buf = pkt.GetDecryptedPacket();
	const char* szHex = CBufferUtil::BufferToHex((char*)buf, pkt.GetDecryptedLen());

	CDebugOut::PrintAlways("[PACKET] CLIENT -> SERVER | %s | Len=%d | %s", 
				pkt.GetType().GetDescription(),
				pkt.GetDecryptedLen(),
				szHex);

	WritePacketLog("CLIENT -> SERVER", pkt.GetType().GetDescription(),
				   pkt.GetDecryptedLen(), szHex);

	return 0;
}
