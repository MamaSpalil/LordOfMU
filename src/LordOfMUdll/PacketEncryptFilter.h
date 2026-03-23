#ifndef __PacketEncryptFilter_H
#define __PacketEncryptFilter_H

#pragma once

#include "Proxy.h"
#include "PacketFilter.h"

/**
 * \brief 
 */
class CPacketEncryptFilter
	: public CPacketFilter
{
public:
	CPacketEncryptFilter(CProxy* pProxy);
	virtual ~CPacketEncryptFilter(){}

public:
	// Filter Interface Methods
	virtual const char* GetName() { return "PacketEncryptFilter"; }
	virtual WORD GetLevel() { return FILTER_LEVEL_ENCDEC; }
	virtual int FilterRecvPacket(CPacket& pkt, CFilterContext& context);
	virtual int FilterSendPacket(CPacket& pkt, CFilterContext& context);

	// Parameter interface - allows other filters to notify about blocked C3/C4 packets
	virtual bool SetParam(const char* pszParam, void* pData);

protected:
	void EncryptXOR(CPacket& pkt);

private:
	ULONG m_ulSendC3Counter;
	BYTE m_bLastSendC3Counter;
	ULONG m_ulRecvC3Counter;
	ULONG m_ulBlockedC3Total;
};

#endif //__PacketEncryptFilter_H