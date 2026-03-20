#include "stdafx.h"
#include "PacketLogger.h"
#include "DebugOut.h"
#include "BufferUtil.h"

/**
 *
 */
CPacketLogger::CPacketLogger(CProxy* pProxy)
	: CPacketFilter(pProxy)
{
	CDebugOut::PrintAlways("[PACKET_LOG] Packet logger filter initialized.");
}

/**
 *
 */
CPacketLogger::~CPacketLogger()
{
	CDebugOut::PrintAlways("[PACKET_LOG] Packet logger filter destroyed.");
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
 *
 */
int CPacketLogger::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	CDebugOut::PrintAlways("[PACKET] SERVER -> CLIENT | %s | Len=%d | %s", 
				pkt.GetType().GetDescription(),
				pkt.GetDecryptedLen(),
				CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen()));

	return 0;
}

/**
 *
 */
int CPacketLogger::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	BYTE* buf = pkt.GetDecryptedPacket();

	CDebugOut::PrintAlways("[PACKET] CLIENT -> SERVER | %s | Len=%d | %s", 
				pkt.GetType().GetDescription(),
				pkt.GetDecryptedLen(),
				CBufferUtil::BufferToHex((char*)buf, pkt.GetDecryptedLen()));

	return 0;
}
