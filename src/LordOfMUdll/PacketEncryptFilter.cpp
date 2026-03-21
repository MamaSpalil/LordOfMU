#include "stdafx.h"
#include "PacketEncryptFilter.h"
#include "CommonPackets.h"
#include "EncDec.h"
#include "BufferUtil.h"
#include "DebugOut.h"


/**
 * \brief 
 */
CPacketEncryptFilter::CPacketEncryptFilter(CProxy* pProxy) 
	: CPacketFilter(pProxy) 
{
	m_ulSendC3Counter = 0;
	m_bLastSendC3Counter = 0;
	m_ulRecvC3Counter = 0;
}

/**
 * \brief 
 */
int CPacketEncryptFilter::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	if (!pkt.IsDecrypted())
		return 0;

	EncryptXOR(pkt);

	if (pkt.GetPktClass() == 0xC3 || pkt.GetPktClass() == 0xC4)
	{
#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__)
		CEncDec::EncryptC3asServer(pkt);
#endif
	}
	else
	{
		pkt = CPacket(pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());
	}
	
	return 0;
}

/**
 * \brief 
 */
int CPacketEncryptFilter::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	if (!pkt.IsDecrypted())
		return 0;


	EncryptXOR(pkt);

	if (pkt.GetPktClass() == 0xC3 || pkt.GetPktClass() == 0xC4)
	{
#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__)

		if (pkt.GetInjected() != 0)
			m_ulSendC3Counter = (m_ulSendC3Counter + 1) % 256;
		else
		{
			BYTE* pDec = pkt.GetDecryptedPacket();
			if (pDec)
				m_bLastSendC3Counter = *(pDec + pkt.GetHdrLen());
		}

		DWORD counter = (m_bLastSendC3Counter + m_ulSendC3Counter) % 256;
		BYTE* pDec2 = pkt.GetDecryptedPacket();
		if (pDec2)
			*(pDec2 + pkt.GetHdrLen()) = (BYTE)counter;

		CEncDec::EncryptC3asClient(pkt);
#endif
	}
	else
	{
		pkt = CPacket(pkt.GetDecryptedPacket(), pkt.GetDecryptedLen());
	}

	return 0;
}

/**
 * \brief 
 */
void CPacketEncryptFilter::EncryptXOR(CPacket& pkt)
{
	const BYTE* xorP = pkt.GetType().GetXorParams();

	if (xorP && (xorP[0] != 0 || xorP[1] != 0))
	{
		int len = pkt.GetDecryptedLen();
		BYTE* buf = pkt.GetDecryptedPacket();

		if (buf)
			CEncDec::EncXor32(buf + xorP[0], xorP[1], len - xorP[0]);
	}
}
