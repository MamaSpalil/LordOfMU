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
}

/**
 *
 */
CPacketLogger::~CPacketLogger()
{
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
	CDebugOut::PrintInfo("[RECV] %s -> %s", 
				pkt.GetType().GetDescription(),
				CBufferUtil::BufferToHex((char*)pkt.GetDecryptedPacket(), pkt.GetDecryptedLen()));

	return 0;
}

/**
 *
 */
int CPacketLogger::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	BYTE* buf = pkt.GetDecryptedPacket();

	CDebugOut::PrintInfo("[SEND] %s -> %s", 
				pkt.GetType().GetDescription(),
				CBufferUtil::BufferToHex((char*)buf, pkt.GetDecryptedLen()));

	return 0;
}
