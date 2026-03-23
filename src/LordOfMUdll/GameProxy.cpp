#include "stdafx.h"
#include "GameProxy.h"
#include "Proxifier.h"
#include "ProxyBuilder.h"
#include "DebugOut.h"
#include "BufferUtil.h"
#include "CommonPackets.h"
#include "ClickerLogger.h"


/**
 * \brief Format raw packet bytes as a hex string for logging.
 *        Outputs up to nMaxBytes bytes in "XX XX XX ..." format.
 */
static void FormatPacketHex(const BYTE* pData, int nLen, char* szOut, int cbOut, int nMaxBytes = 32)
{
	if (!pData || nLen <= 0 || !szOut || cbOut <= 0)
	{
		if (szOut && cbOut > 0) szOut[0] = '\0';
		return;
	}

	int nShow = (nLen < nMaxBytes) ? nLen : nMaxBytes;
	int nPos = 0;

	for (int i = 0; i < nShow && nPos + 4 < cbOut; i++)
	{
		if (i > 0) szOut[nPos++] = ' ';
		int nWritten = _snprintf(szOut + nPos, cbOut - nPos - 1, "%02X", pData[i]);
		if (nWritten > 0) nPos += nWritten; else break;
	}

	if (nLen > nMaxBytes && nPos + 4 < cbOut)
	{
		int nWritten = _snprintf(szOut + nPos, cbOut - nPos - 1, "...");
		if (nWritten > 0) nPos += nWritten;
	}

	szOut[nPos] = '\0';
}


static CGameProxy* g_pGameProxy = 0;
static bool g_bCommandOnlyProxy = false;

extern "C" __declspec(dllexport) bool SendCommand(const char* buf)
{
	if (!buf)
	{
		WriteClickerLogFmt("PICKUP", "SendCommand FAILED: buf is NULL");
		return false;
	}

	if (!g_pGameProxy)
	{
		WriteClickerLogFmt("PICKUP", "SendCommand FAILED: g_pGameProxy is NULL (game not connected?) cmd='%s'", buf);
		return false;
	}

	CPacketFilter* pFilter = g_pGameProxy->GetFilter("CharInfoFilter");

	if (!pFilter)
	{
		WriteClickerLogFmt("PICKUP", "SendCommand FAILED: CharInfoFilter not found in proxy cmd='%s'", buf);
		return false;
	}

	char* pszCharName = 0;
	pFilter->GetParam("CharName", &pszCharName);

	CCharacterSayPacket pkt(pszCharName, buf);
	g_pGameProxy->send_packet(pkt);

	// In command-only mode (no active socket connection), HandleConnection is not
	// running, so ProcessSendQueue must be called explicitly to process the command.
	// Commands starting with "//" are intercepted by CCommandInterface and never
	// reach sendBuffer, so the invalid socket is not used.
	if (g_bCommandOnlyProxy)
	{
		g_pGameProxy->ProcessSendQueue();
	}

	return true;
}


/**
 * \brief Creates a command-only proxy when the game is already connected
 *        and the connect() hook missed the existing connection.
 *        This allows // commands (autopick, set_pick_opt, etc.) to be processed
 *        through the filter chain without an active socket.
 *        Actual packet sending (PickItem) will not work until a real connection
 *        is proxied (e.g., on game reconnect).
 */
extern "C" __declspec(dllexport) bool ForceInitCommandProxy()
{
	if (g_pGameProxy)
	{
		WriteClickerLogFmt("PICKUP", "ForceInitCommandProxy: proxy already exists at 0x%p (command-only=%d)",
			(void*)g_pGameProxy, (int)g_bCommandOnlyProxy);
		return true;
	}

	WriteClickerLogFmt("PICKUP", "ForceInitCommandProxy: Creating command-only proxy (no socket)");

	// Use CProxyBuilder to create a full game proxy with all filters.
	// INVALID_SOCKET means no actual network traffic - only command processing.
	CProxy* pProxy = CProxyBuilder::CreateGameProxy(INVALID_SOCKET);

	if (!pProxy || !g_pGameProxy)
	{
		WriteClickerLogFmt("PICKUP", "ForceInitCommandProxy: FAILED to create proxy");
		return false;
	}

	g_bCommandOnlyProxy = true;

	WriteClickerLogFmt("PICKUP", "ForceInitCommandProxy: Command proxy created at 0x%p, filters initialized",
		(void*)g_pGameProxy);
	return true;
}


/**
 *
 */
CGameProxy::CGameProxy(SOCKET s)
	: CPassThroughProxy(s), m_ioBuffer(io_buffer_size+8), m_ioBuffer2(io_buffer_size + io_buffer_size + 16)
{
	// Cache the old state before making any changes.
	CGameProxy* pOldProxy = g_pGameProxy;
	bool bWasCommandOnly = g_bCommandOnlyProxy;

	// Update the command-only flag BEFORE setting g_pGameProxy to avoid
	// a window where other threads see the new proxy with the old flag.
	if (s != INVALID_SOCKET)
		g_bCommandOnlyProxy = false;

	g_pGameProxy = this;

	// If replacing a command-only proxy with a real proxy (game reconnected),
	// clean up the old one. The check uses the cached state to avoid the
	// flag being already cleared when we get here.
	if (pOldProxy && pOldProxy != this && bWasCommandOnly)
	{
		WriteClickerLogFmt("PICKUP", "CGameProxy: Real proxy replacing command-only proxy at 0x%p",
			(void*)pOldProxy);
		delete pOldProxy;
	}
}

/**
 *
 */
CGameProxy::~CGameProxy()
{
	// Only clear g_pGameProxy if it still points to us (prevents clearing
	// the pointer when a command-only proxy is deleted during replacement).
	if (g_pGameProxy == this)
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
			// Note: Individual packet logging is handled by PacketLogger filter
			// (AVANTA+ELITE filtered) after type classification by PacketDecryptFilter.
			// Raw pre-filter logging removed to avoid duplicate flood.

			if (!FilterRecvPacket(pkt))
			{
				BYTE* pRaw = pkt.GetRawPacket();
				int pktLen = pkt.GetPktLen();

				if (pRaw && pktLen > 0 && offs + pktLen <= io_output_buffer_size)
				{
					memcpy(newBuff + offs, pRaw, pktLen);
					offs += pktLen;
				}
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
			// Note: Individual packet logging is handled by PacketLogger filter
			// (AVANTA+ELITE filtered) after type classification by PacketDecryptFilter.
			// Raw pre-filter logging removed to avoid duplicate flood.

			if (!FilterSendPacket(pkt))
			{
				BYTE* pRaw = pkt.GetRawPacket();
				int pktLen = pkt.GetPktLen();

				if (pRaw && pktLen > 0 && offs + pktLen <= io_output_buffer_size)
				{
					memcpy(newBuff + offs, pRaw, pktLen);
					offs += pktLen;
				}
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
	if (CDebugMode::IsEnabled())
	{
		char szHex[256] = {0};
		FormatPacketHex(pkt.GetRawPacket(), pkt.GetPktLen(), szHex, sizeof(szHex));
		WritePacketLog("DIRECT>SERVER", pkt.GetType().GetDescription(), pkt.GetPktLen(), szHex);
	}
	m_cSendQueue.QueuePacket(pkt, QUEUE_FLAG_DIRECT);
	return true;
}

/**
 *
 */
bool CGameProxy::recv_direct(CPacket& pkt)
{
	if (CDebugMode::IsEnabled())
	{
		char szHex[256] = {0};
		FormatPacketHex(pkt.GetRawPacket(), pkt.GetPktLen(), szHex, sizeof(szHex));
		WritePacketLog("DIRECT>CLIENT", pkt.GetType().GetDescription(), pkt.GetPktLen(), szHex);
	}
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

			if (buf && len > 0 && !sendBuffer(m_cServer.getSock(), buf, len))
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

			if (buf && len > 0 && !sendBuffer(m_cClient.getSock(), buf, len))
				break;
		}
	}
}

