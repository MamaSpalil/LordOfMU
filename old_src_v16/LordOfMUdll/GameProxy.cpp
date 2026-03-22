#include "stdafx.h"
#include "GameProxy.h"
#include "Proxifier.h"
#include "DebugOut.h"
#include "BufferUtil.h"
#include "CommonPackets.h"


static CGameProxy* g_pGameProxy = 0;

extern "C" __declspec(dllexport) bool SendCommand(const char* buf)
{
//	char* buf = (char*)lParam;

	if (!buf || !g_pGameProxy)
		return false;

	CPacketFilter* pFilter = g_pGameProxy->GetFilter("CharInfoFilter");

	if (!pFilter)
		return false;

	char* pszCharName = 0;
	pFilter->GetParam("CharName", &pszCharName);

	CCharacterSayPacket pkt(pszCharName, buf);
	g_pGameProxy->send_packet(pkt);

	return true;
}


/**
 *
 */
CGameProxy::CGameProxy(SOCKET s)
	: CPassThroughProxy(s), m_ioBuffer(io_buffer_size+8), m_ioBuffer2(io_buffer_size + io_buffer_size + 16)
{
	g_pGameProxy = this;
}

/**
 *
 */
CGameProxy::~CGameProxy()
{
	g_pGameProxy = 0;

	ClearFilters();	
}


/**  
 * \brief 
 */
void CGameProxy::HandleConnection()
{
	int buf_size = io_buffer_size;
	CCharAutoBuffer& buf = m_ioBuffer;
	CCharAutoBuffer& buf2 = m_ioBuffer2;

	if (recvBuffer(m_cClient.getSock(), buf, buf_size))
	{
		ProcessSendStream(buf, buf2, buf_size);

		if (buf_size > 0)
			sendBuffer(m_cServer.getSock(), buf2, buf_size);
	}

	ProcessSendQueue();

	buf_size = io_buffer_size;
	if (recvBuffer(m_cServer.getSock(), buf, buf_size))
	{
		ProcessRecvStream(buf, buf2, buf_size);

		if (buf_size > 0)
			sendBuffer(m_cClient.getSock(), buf2, buf_size);
	}

	ProcessRecvQueue();
}



/**
 * \brief 
 */
void CGameProxy::ProcessRecvStream(char* lpBuffer, char* newBuff, int& iLen)
{
	m_cRecvPacketParser.AppendStream((BYTE*)lpBuffer, iLen);
	int offs = 0;

	while (1)
	{
		while (m_cRecvPacketParser.Next());

		CPacket pkt;
		if (m_cRecvPacketParser.GetPacket(pkt))
		{
			if (!FilterRecvPacket(pkt))
			{
				memcpy(newBuff + offs, pkt.GetRawPacket(), pkt.GetPktLen());
				offs += pkt.GetPktLen();
			}

			continue;
		}
		else if (m_cRecvPacketParser.HasError())
		{
			CDebugOut::PrintError("Error parsing received packet.");
		}

		break;
	}

	iLen = offs;
}


/**
 * \brief 
 */
void CGameProxy::ProcessSendStream(char* lpBuffer, char* newBuff, int& iLen)
{
	m_cSendPacketParser.AppendStream((BYTE*)lpBuffer, iLen);
	int offs = 0;

	while (1)
	{
		while (m_cSendPacketParser.Next());

		CPacket pkt;
		if (m_cSendPacketParser.GetPacket(pkt))
		{
			if (!FilterSendPacket(pkt))
			{
				memcpy(newBuff + offs, pkt.GetRawPacket(), pkt.GetPktLen());
				offs += pkt.GetPktLen();
			}

			continue;
		}
		else if (m_cSendPacketParser.HasError())
		{
			CDebugOut::PrintError("Error parsing send packet.");
		}

		break;
	}

	iLen = offs;
}


/**
 *
 */
bool CGameProxy::FilterRecvPacket(CPacket& pkt)
{
	int iCount = (int)m_cFilterChain.size();
	int iLvl = INT_MAX;

	CFilterContext context;

	for (int i=0; i < iCount; ++i)
	{
		CPacketFilter* pFilter = m_cFilterChain[i];

		if (pFilter && (int)pFilter->GetLevel() < iLvl)
		{
			int iRet = pFilter->FilterRecvPacket(pkt, context);

			if (iRet == -1)
				return true;

			if (iRet > 0)
				iLvl = iRet;
		}
	}

	return false;
}


/**
 *
 */
bool CGameProxy::FilterSendPacket(CPacket& pkt)
{
	int iCount = (int)m_cFilterChain.size();
	int iLvl = INT_MAX;

	CFilterContext context;

	for (int i=0; i < iCount; ++i)
	{
		CPacketFilter* pFilter = m_cFilterChain[i];

		if (pFilter && (int)pFilter->GetLevel() < iLvl)
		{
			int iRet = pFilter->FilterSendPacket(pkt, context);

			if (iRet == -1)
				return true;

			if (iRet > 0)
				iLvl = iRet;
		}
	}

	return false;
}

/**
 *
 */
bool CGameProxy::send_packet(CPacket& pkt)
{
	m_cSendQueue.QueuePacket(pkt);
	return true;
}


/**
 * \brief 
 */
bool CGameProxy::send_lop_packet(CPacket& pkt)
{
	m_cSendQueue.QueuePacket(pkt);
	return true;
}


/**
 *
 */
bool CGameProxy::recv_packet(CPacket& pkt)
{
	m_cRecvQueue.QueuePacket(pkt);	
	return true;
}


/**
 *
 */
bool CGameProxy::send_direct(CPacket& pkt)
{
	m_cSendQueue.QueuePacket(pkt, QUEUE_FLAG_DIRECT);
	return true;
}

/**
 *
 */
bool CGameProxy::recv_direct(CPacket& pkt)
{
	m_cRecvQueue.QueuePacket(pkt, QUEUE_FLAG_DIRECT);
	return true;
}

/**
 *
 */
bool CGameProxy::AddFilter(CPacketFilter* pFilter)
{
	if (!pFilter)
		return false;

	if (GetFilter(pFilter->GetName()) != 0)
		return false; // already exists

	m_cFilterChain.push_back(pFilter);
	return true;
}

/**
 *
 */
CPacketFilter* CGameProxy::GetFilter(const char* szName)
{
	for (int i=(int)m_cFilterChain.size()-1; i >= 0; --i)
	{
		if (m_cFilterChain[i] != 0 && _stricmp(m_cFilterChain[i]->GetName(), szName) == 0)
			return m_cFilterChain[i];
	}

	return 0;
}

/**
 *
 */
void CGameProxy::ClearFilters()
{
	for (int i=(int)m_cFilterChain.size()-1; i >= 0; --i)
	{
		if (m_cFilterChain[i])
			m_cFilterChain[i]->Term();
	}

	for (int i=(int)m_cFilterChain.size()-1; i >= 0; --i)
	{
		if (m_cFilterChain[i])
		{
			delete m_cFilterChain[i];
			m_cFilterChain[i] = 0;
		}
	}

	m_cFilterChain.clear();
}


/**
 * \brief 
 */
bool CGameProxy::GetParam(const char* pszParam, void* pData)
{
	if (!pszParam)
		return false;

	if (_stricmp(pszParam, "IP") == 0)
	{
		*((DWORD*)pData) = m_cServer.getAddr()->sin_addr.S_un.S_addr;
		return true;
	}

	return false;
}


/**
 * \brief 
 */
bool CGameProxy::SetParam(const char* pszParam, void* pData)
{
	return false;
}


/**
 * \brief 
 */
void CGameProxy::ProcessSendQueue()
{
	int iCount = 25;
	std::vector<CPacket> vPackets; 
	std::vector<ULONG> vFlags;

	m_cSendQueue.GetPackets(vPackets, vFlags, iCount);

	for (int i = 0; i < iCount; i++)
	{
		CPacket& pkt = vPackets[i];
		ULONG    ulFlags = vFlags[i];

		if (ulFlags == QUEUE_FLAG_DIRECT || !FilterSendPacket(pkt))
		{
			char* buf = (char*)pkt.GetRawPacket();
			int len = (int)pkt.GetPktLen();

			if (!sendBuffer(m_cServer.getSock(), buf, len))
				break;
		}
	}
}


/**
 * \brief 
 */
void CGameProxy::ProcessRecvQueue()
{
	int iCount = 25;
	std::vector<CPacket> vPackets; 
	std::vector<ULONG> vFlags;

	m_cRecvQueue.GetPackets(vPackets, vFlags, iCount);

	for (int i = 0; i < iCount; i++)
	{
		CPacket& pkt = vPackets[i];
		ULONG    ulFlags = vFlags[i];

		if (ulFlags == QUEUE_FLAG_DIRECT || !FilterRecvPacket(pkt))
		{
			char* buf = (char*)pkt.GetRawPacket();
			int len = (int)pkt.GetPktLen();

			if (!sendBuffer(m_cClient.getSock(), buf, len))
				break;
		}
	}
}

