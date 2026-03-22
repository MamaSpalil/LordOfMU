#include "stdafx.h"
#include "AutoKillFilter.h"
#include "CommonPackets.h"
#include <math.h>

/**
 * \brief 
 */
CAutoKillFilter::CAutoKillFilter(CProxy* pProxy) 
	: CPacketFilter(pProxy)
{
	m_fEnabled = FALSE;
	m_fSuspended = false;
	m_dwDelay = 100;
	m_wSkill = 0;
	m_wPlayerId = 0;
	m_bX = 0;
	m_bY = 0;
	m_iDist = 5;

	InitializeCriticalSection(&m_csQueue);

	m_hKillEvent = CreateEvent(0, 1, 0, 0);
	m_hStopEvent = CreateEvent(0, 1, 0, 0);
	m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)KillThreadProc, this, 0, 0);
}


/**
 * \brief 
 */
CAutoKillFilter::~CAutoKillFilter()
{	
	if (m_hThread && m_hThread != INVALID_HANDLE_VALUE)
	{
		SetEvent(m_hStopEvent);

		Sleep(100);

		if (WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT)
			TerminateThread(m_hThread, 0);

		CloseHandle(m_hThread);
	}

	CloseHandle(m_hStopEvent);
	CloseHandle(m_hKillEvent);
	DeleteCriticalSection(&m_csQueue);
}


/**
 * \brief 
 */
int CAutoKillFilter::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	if (pkt == CMeetMonsterPacket::Type())
	{
		CMeetMonsterPacket& pktMMonster = (CMeetMonsterPacket&)pkt;

		for (int i = pktMMonster.GetCount()-1; i >= 0; --i)
		{
			BYTE x=0, y=0, x0=0, y0=0; 
			WORD wId = pktMMonster.GetId(i);
			pktMMonster.GetPos(i, x, y, &x0, &y0);

			m_vMobList.insert(std::pair<WORD,DWORD>(wId, MAKELONG(MAKEWORD(x,y),MAKEWORD(x0,y0))));
		}

		UpdateKillQueue();
	}
	else if (pkt == CForgetObjectPacket::Type())
	{
		CForgetObjectPacket& pkt2 = (CForgetObjectPacket&)pkt;

		for (int i = pkt2.GetObjectCount()-1; i >= 0; --i)
		{
			m_vMobList.erase(pkt2.GetObjectId(i));
		}

		UpdateKillQueue();
	}
	else if (pkt == CObjectDeathPacket::Type())
	{
		CObjectDeathPacket& pkt2 = (CObjectDeathPacket&)pkt;

		if (pkt2.GetObjectId() == m_wPlayerId)
		{
			m_fEnabled = false;
		}

		m_vMobList.erase(pkt2.GetObjectId());
		UpdateKillQueue();
	}
	else if (pkt == CWarpReplyPacket::Type())
	{
		CWarpReplyPacket& pkt2 = (CWarpReplyPacket&)pkt;

		m_fEnabled = FALSE;
		m_bX = pkt2.GetX();
		m_bY = pkt2.GetY();

		CAutoLockQueue autoCS(&m_csQueue);
		m_vKillQueue.clear();
		m_vMobList.clear();
	}
	else if (pkt == CGameServerHelloPacket::Type())
	{
		CGameServerHelloPacket& pktHelo = (CGameServerHelloPacket&)pkt;
		m_wPlayerId = pktHelo.GetPlayerId();
	}
	else if (pkt == CObjectMovedPacket::Type())
	{
		CObjectMovedPacket& pktMove = (CObjectMovedPacket&)pkt;

		if (pktMove.GetId() == m_wPlayerId)
		{
			m_bX = pktMove.GetX();
			m_bY = pktMove.GetY();

			UpdateKillQueue();
		}
		else
		{
			std::map<WORD, DWORD>::iterator it = m_vMobList.find(pktMove.GetId());

			if (it != m_vMobList.end())
			{
				it->second = MAKELONG(MAKEWORD(pktMove.GetX(), pktMove.GetY()), MAKEWORD(pktMove.GetX0(), pktMove.GetY0()));
				UpdateKillQueue();
			}
		}
	}
	else if (pkt == CCharRespawnPacket::Type())
	{
		CCharRespawnPacket& pkt2 = (CCharRespawnPacket&)pkt;

		m_bX = pkt2.GetX();
		m_bY = pkt2.GetY();

		m_fEnabled = false;

		CAutoLockQueue autoCS(&m_csQueue);
		m_vKillQueue.clear();
		m_vMobList.clear();
	}
	else if (pkt == CCharStatsPacket::Type())
	{
		CCharStatsPacket& pkt2= (CCharStatsPacket&)pkt;

		m_fEnabled = FALSE;
		m_bX = pkt2.GetX();
		m_bY = pkt2.GetY();

		CAutoLockQueue autoCS(&m_csQueue);
		m_vKillQueue.clear();
		m_vMobList.clear();
	}
	else if (pkt == CUpdatePosSTCPacket::Type())
	{
		CUpdatePosSTCPacket& pkt2 = (CUpdatePosSTCPacket&)pkt;

		if (pkt2.GetId() == m_wPlayerId)
		{
			m_bX = pkt2.GetX();
			m_bY = pkt2.GetY();

			UpdateKillQueue();
		}
		else
		{
			std::map<WORD, DWORD>::iterator it = m_vMobList.find(pkt2.GetId());

			if (it != m_vMobList.end())
			{
				it->second = (DWORD)MAKEWORD(pkt2.GetX(), pkt2.GetY());
				UpdateKillQueue();
			}
		}
	}

	return 0;
}


/**
 * \brief 
 */
int CAutoKillFilter::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	return 0;
}


/**
 * \brief 
 */
bool CAutoKillFilter::GetParam(const char* pszParam, void* pData)
{
	if (_stricmp(pszParam, "autokill") == 0)
	{
		CAutoLockQueue autoCS(&m_csQueue);
		*((BOOL*)pData) = m_fEnabled;

		return true;
	}
	else if (_stricmp(pszParam, "targets_count") == 0)
	{
		CAutoLockQueue autoCS(&m_csQueue);
		*((int*)pData) = (int)m_vKillQueue.size();

		return true;
	}

	return false;
}


/**
 * \brief 
 */
bool CAutoKillFilter::SetParam(const char* pszParam, void* pData)
{
	if (_stricmp(pszParam, "autokill") == 0)
	{
		CAutoLockQueue autoCS(&m_csQueue);

		m_fEnabled = *((BOOL*)pData);
		if (!m_fEnabled)
			m_vKillQueue.clear();
	}
	else if (_stricmp(pszParam, "hdist") == 0)
	{
		m_iDist = *((int*)pData);
		UpdateKillQueue();
	}
	else if (_stricmp(pszParam, "toggle") == 0)
	{
		CAutoLockQueue autoCS(&m_csQueue);

		m_fEnabled = !m_fEnabled;
		if (!m_fEnabled)
			m_vKillQueue.clear();
	}
	else if (_stricmp(pszParam, "setskill") == 0)
	{
		char* szSkill = (char*)pData;

		struct { const char* szSkillName; WORD wSkill; } _map[] =
		{
			{"Normal"		, 0x0000},
			{"FireBurst"	, 0x193D},
			{"Force"		, 0x193C}, 
			{"TwistingSlash", 0x1E29},
			{"DeathStab"	, 0x192B},
			{"FallingSlash"	, 0x1913},
			{"Cyclone"		, 0x1916},
			{"Lunge"		, 0x1914},
			{"EnergyBall"	, 0x1911},
			{"Poison"		, 0x1901},
			{"Meteorite"	, 0x1902},
			{"Lightning"	, 0x1903},
			{"Fireball"		, 0x1904},
			{"PowerWave"	, 0x190B},
			{"Ice"			, 0x1907},
			{"IceArrow"		, 0x1933},
			{"ChainLight"	, 0x19D6},
			{"Sleep"		, 0x19DB},
			{"DrainLife"	, 0x1ED6},
			{"EvilSpirit"	, 0x1E09},
			{"DarkHorse"	, 0x1E3E},
		};

		for (int i=sizeof(_map)/sizeof(_map[0])-1; i >=0; --i)
		{
			if (0 == _stricmp(szSkill, _map[i].szSkillName))
			{
				CAutoLockQueue autoCS(&m_csQueue);

				m_wSkill = _map[i].wSkill;
				return true;
			}
		}

		return false;
	}
	else if (_stricmp(pszParam, "suspended") == 0)
	{
		CAutoLockQueue autoCS(&m_csQueue);
		m_fSuspended = *((bool*)pData);
	}
	else if (_stricmp(pszParam, "hitdelay") == 0)
	{
		CAutoLockQueue cs(&m_csQueue);
		m_dwDelay = *((DWORD*)pData);

		if (m_dwDelay > 2000)
			m_dwDelay = 2000;
	}

	return true;
}


/**
 * \brief 
 */
DWORD CAutoKillFilter::KillThreadProc(CAutoKillFilter* pThis)
{
	while (1)
	{
		HANDLE objs[] = {pThis->m_hStopEvent, pThis->m_hKillEvent};
		DWORD dwRes = WaitForMultipleObjects(2, objs, 0, INFINITE);

		if (dwRes == WAIT_OBJECT_0)
			break;
		
		pThis->KillNext();
	}

	return 0;
}


/**
 * \brief 
 */
void CAutoKillFilter::KillNext()
{
	WORD wObject = 0;
	bool fSuspended = true;
	DWORD dwDelay = 100;
	WORD wSkill = 0;

	EnterCriticalSection(&m_csQueue);

	if (m_vKillQueue.size() != 0)
	{
		wObject = *(m_vKillQueue.begin());
		fSuspended = m_fSuspended;
		dwDelay = m_dwDelay;
		wSkill = m_wSkill;
	}
	else
		ResetEvent(m_hKillEvent);

	LeaveCriticalSection(&m_csQueue);

	if (!fSuspended)
	{
		KillObject(wObject, wSkill);
		Sleep(dwDelay);
	}
}


/**
 * \brief 
 */
void CAutoKillFilter::KillObject(WORD wId, WORD wSkill)
{
	if (wSkill == 0)
	{
		GetProxy()->send_packet(CNormalAttackPacket(wId));
	}
	else if ((wSkill & 0xFF00) == 0x1900)
	{
		GetProxy()->send_packet(CSingleSkillPacket(LOBYTE(wSkill), wId));
	}
	else if ((wSkill & 0xFF00) == 0x1E00)
	{
		CPacketFilter* pFilter = GetProxy()->GetFilter("CharInfoFilter");

		if (pFilter)
		{
			BYTE x = 0, y = 0;
			pFilter->GetParam("X", &x);
			pFilter->GetParam("Y", &y);

			GetProxy()->send_packet(CMassiveSkillPacket(LOBYTE(wSkill), x, y, 0, wId));
		}
	}
}


/**
 * \brief 
 */
void CAutoKillFilter::UpdateKillQueue()
{
	if (!m_fEnabled)
		return;

	CAutoLockQueue autoCS(&m_csQueue);
	m_vKillQueue.clear();

	for (std::map<WORD, DWORD>::iterator it = m_vMobList.begin();
			it != m_vMobList.end(); ++it)
	{
		int dx = abs((int)LOBYTE(it->second) - (int)m_bX);
		int dy = abs((int)HIBYTE(it->second) - (int)m_bY);
//		int dx0 = abs((int)LOBYTE(HIWORD(it->second)) - (int)m_bX);
//		int dy0 = abs((int)HIBYTE(HIWORD(it->second)) - (int)m_bY);

		if (dx <= m_iDist && dy <= m_iDist)
			m_vKillQueue.insert(it->first);
	}

	if (m_vKillQueue.size() > 0)
		SetEvent(m_hKillEvent);
}
