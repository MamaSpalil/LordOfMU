#ifndef __GameProxy_H
#define __GameProxy_H

#pragma once

#include "Proxy.h"
#include "PassThroughProxy.h"
#include "PacketQueue.h"
#include "PacketParser.h"
#include "AutoBuffer.h"
#include <vector>

/**
 *
 */
class CGameProxy 
	: public CPassThroughProxy
{
protected:
	typedef std::vector<CPacketFilter*> CFilterChain;


public:
	CGameProxy(SOCKET s);
	virtual ~CGameProxy();

public:
	// Packet Interface
	virtual bool send_packet(CPacket& pkt);
	virtual bool send_lop_packet(CPacket& pkt);
	virtual bool recv_packet(CPacket& pkt);
	virtual bool send_direct(CPacket& pkt);
	virtual bool recv_direct(CPacket& pkt);

	// Filter Interface
	virtual bool AddFilter(CPacketFilter* pFilter);
	virtual CPacketFilter* GetFilter(const char* szName);
	virtual void ClearFilters();

	// Queue processing - public so SendCommand can flush commands immediately
	// when running in command-only mode (no active socket connection).
	void ProcessSendQueue();

	// Parameter interface
	virtual bool GetParam(const char* pszParam, void* pData);
	virtual bool SetParam(const char* pszParam, void* pData);

protected:
	virtual void HandleConnection();
	void ProcessRecvStream(char* lpBuffer, char* newBuff, int& iLen);
	void ProcessSendStream(char* lpBuffer, char* newBuff, int& iLen);

	void ProcessRecvQueue();

	bool FilterRecvPacket(CPacket& pkt);
	bool FilterSendPacket(CPacket& pkt);

private:
	CPacketQueue  m_cRecvQueue;
	CPacketQueue  m_cSendQueue;
	CPacketParser m_cRecvPacketParser;
	CPacketParser m_cSendPacketParser;
	CFilterChain  m_cFilterChain;

private:
	static const UINT WM_SEND_COMMAND_INPUT	= WM_APP + 601;
	static const UINT WM_SEND_PACKET		= WM_APP + 602;

	static const UINT QUEUE_FLAG_DIRECT	= 1;

	static const int io_buffer_size	= 65536;

	static const int io_output_buffer_size = io_buffer_size * 2 + 16;

	CCharAutoBuffer m_ioBuffer;
	CCharAutoBuffer m_ioBuffer2;
};

#endif //__GameProxy_H