#ifndef __AutoKillFilter_H
#define __AutoKillFilter_H

#pragma once

#include "Proxy.h"
#include "PacketFilter.h"
#include <set>
#include <map>


/**
 * \brief 
 */
class CAutoKillFilter
	: public CPacketFilter
{
	struct CAutoLockQueue
	{
		CAutoLockQueue(CRITICAL_SECTION* pCS){ EnterCriticalSection(m_pCS = pCS); }
		virtual ~CAutoLockQueue(){ LeaveCriticalSection(m_pCS); }

		CRITICAL_SECTION* m_pCS;
	};

public:
	CAutoKillFilter(CProxy* pProxy);
	virtual ~CAutoKillFilter();

public:
	// Filter Interface Methods
	virtual const char* GetName(){ return "AutoKillFilter"; }
	virtual WORD GetLevel(){ return FILTER_LEVEL_CLICKER; }
	virtual int FilterRecvPacket(CPacket& pkt, CFilterContext& context);
	virtual int FilterSendPacket(CPacket& pkt, CFilterContext& context);

	// Parameter interface
	virtual bool GetParam(const char* pszParam, void* pData);
	virtual bool SetParam(const char* pszParam, void* pData);

private:
	static DWORD KillThreadProc(CAutoKillFilter* pThis);

	void KillNext();
	void KillObject(WORD wId, WORD wSkill);
	void UpdateKillQueue();

private:
	std::set<WORD> m_vKillQueue;
	std::map<WORD, DWORD> m_vMobList;

private:
	BOOL m_fEnabled;
	bool m_fSuspended;
	DWORD m_dwDelay;
	WORD m_wSkill;
	int  m_iDist;

	BYTE m_bX;
	BYTE m_bY;
	WORD m_wPlayerId;

private:
	HANDLE m_hThread;
	HANDLE m_hKillEvent;
	HANDLE m_hStopEvent;
	CRITICAL_SECTION m_csQueue;
};

#endif //__AutoKillFilter_H