#include "stdafx.h"
#include "PacketLogger.h"
#include "DebugOut.h"
#include "BufferUtil.h"
#include "ClickerLogger.h"

/**
 *
 */
CPacketLogger::CPacketLogger(CProxy* pProxy)
	: CPacketFilter(pProxy)
{
	CDebugOut::PrintAlways("[PACKET_LOG] Packet logger filter initialized.");
	WriteClickerLogFmt("PACKET", "Packet logger filter initialized - monitoring all traffic");
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
 * \brief Logs server-to-client packets and writes to both debug log and ClickerLog.txt
 */
int CPacketLogger::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	const char* szHex = CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());

	CDebugOut::PrintAlways("[PACKET] SERVER -> CLIENT | %s | Len=%d | %s", 
				pkt.GetType().GetDescription(),
				pkt.GetDecryptedLen(),
				szHex);

	// Also write to ClickerLog.txt for comprehensive logging
	WritePacketLog("SERVER -> CLIENT", pkt.GetType().GetDescription(), 
				   pkt.GetDecryptedLen(), szHex);

	return 0;
}

/**
 * \brief Logs client-to-server packets and writes to both debug log and ClickerLog.txt
 */
int CPacketLogger::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	BYTE* buf = pkt.GetDecryptedPacket();
	const char* szHex = CBufferUtil::BufferToHex((char*)buf, pkt.GetDecryptedLen());

	CDebugOut::PrintAlways("[PACKET] CLIENT -> SERVER | %s | Len=%d | %s", 
				pkt.GetType().GetDescription(),
				pkt.GetDecryptedLen(),
				szHex);

	// Also write to ClickerLog.txt for comprehensive logging
	WritePacketLog("CLIENT -> SERVER", pkt.GetType().GetDescription(),
				   pkt.GetDecryptedLen(), szHex);

	return 0;
}
