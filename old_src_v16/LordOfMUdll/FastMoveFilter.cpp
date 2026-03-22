#include "stdafx.h"
#include "FastMoveFilter.h"
#include "CommonPackets.h"
#include "DebugOut.h"


/**
 * \brief 
 */
CFastMoveFilter::CFastMoveFilter(CProxy* pProxy) 
	: CPacketFilter(pProxy)
{
	m_fEnabled = FALSE;
	m_fSuspended = false;
}

/**
 * \brief 
 */
int CFastMoveFilter::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	if (m_fEnabled && !m_fSuspended && pkt.GetInjected() == 0 
			&& (pkt == CObjectMovedPacket::Type()))
	{
		CObjectMovedPacket pktMoved(pkt);

		CPacketFilter *pFilter = GetProxy()->GetFilter("CharInfoFilter");
		if (!pFilter)
		{
			CDebugOut::PrintError("Bad software configuration. CharInfoFilter filter object not found.");
			return 0;
		}

		WORD wId = 0;
		pFilter->GetParam("PlayerId", &wId);

		if (wId == pktMoved.GetId())
			return -1;
	}

	return 0;
}


/**
 * \brief 
 */
int CFastMoveFilter::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	if (m_fEnabled && !m_fSuspended && pkt.GetInjected() == 0
			&& (pkt == CCharMoveCTSPacket::Type()))
	{
		CCharMoveCTSPacket pkt1(pkt);

		BYTE x = pkt1.GetX();
		BYTE y = pkt1.GetY();
		BYTE dir = pkt1.GetDir();
		
		int coef[8][2] = 
		{ 
			{-1, -1},
			{ 0, -2},
			{ 1, -1},
			{ 2,  0},
			{ 1,  1},
			{ 0,  2},
			{-1,  1},
			{-2,  0}
		};

		x = (BYTE)((int)x + coef[dir][0]);
		y = (BYTE)((int)y + coef[dir][1]);

		CPacketFilter *pFilter = GetProxy()->GetFilter("CharInfoFilter");
		if (!pFilter)
		{
			CDebugOut::PrintError("Bad software configuration. CharInfoFilter filter object not found.");
			return 0;
		}

		WORD wId = 0;
		pFilter->GetParam("PlayerId", &wId);

		CUpdatePosSTCPacket pktSTC(wId, x, y);
		CUpdatePosCTSPacket pktCTS(x, y);

		GetProxy()->send_packet(pktCTS);
		GetProxy()->recv_packet(pktSTC);
		return -1;
	}

	return 0;
}


/**
 * \brief 
 */
bool CFastMoveFilter::GetParam(const char* pszParam, void* pData)
{
	return false;
}


/**
 * \brief 
 */
bool CFastMoveFilter::SetParam(const char* pszParam, void* pData)
{
	if (_stricmp(pszParam, "enabled") == 0)
	{
		m_fEnabled = *((BOOL*)pData);
	}
	else if (_stricmp(pszParam, "suspended") == 0)
	{
		m_fSuspended = *((bool*)pData);
	}

	return true;
}
