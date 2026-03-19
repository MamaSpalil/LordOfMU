#ifndef __Proxy_H
#define __Proxy_H

#pragma once

#include "Packet.h"
#include "PacketFilter.h"

/**
 *
 */
class CProxy
{
protected:
	CProxy(SOCKET s) : m_id(s) {}

public:
	virtual ~CProxy(){}

public:
	// Socket Interface
	virtual int connect(const struct sockaddr* addr, int len) = 0;
	virtual bool isDead() = 0;

	// Packet Interface
	virtual bool send_packet(CPacket& pkt){ return false; }
	virtual bool send_lop_packet(CPacket& pkt){ return false; }
	virtual bool recv_packet(CPacket& pkt){ return false; }
	virtual bool send_direct(CPacket& pkt){ return false; }
	virtual bool recv_direct(CPacket& pkt){ return false; }

	// Packet Filter Interface
	virtual bool AddFilter(CPacketFilter* pFilter){ return false; }
	virtual CPacketFilter* GetFilter(const char* szName){ return 0; }
	virtual void ClearFilters(){}

	// Parameter interface
	virtual bool GetParam(const char* pszParam, void* pData){ return false; }
	virtual bool SetParam(const char* pszParam, void* pData){ return false; }

protected:
	SOCKET m_id;
};

#endif //__Proxy_H