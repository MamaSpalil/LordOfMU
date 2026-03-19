#ifndef __ProxyConnection_H
#define __ProxyConnection_H

#pragma once

#include "ProxyEndpoint.h"


/**
 * \brief 
 */
class CProxyConnection
{
	/**
	 * \brief 
	 */
	class CConnState
	{
	public:
		CConnState() : m_iState(0) { InitializeCriticalSection(&m_cs); }
		virtual ~CConnState() { DeleteCriticalSection(&m_cs); }

		void operator=(int iState) 
		{
			EnterCriticalSection(&m_cs);
			m_iState = iState;
			LeaveCriticalSection(&m_cs);
		}

		int GetState()
		{ 
			EnterCriticalSection(&m_cs);
			int iRes = m_iState;
			LeaveCriticalSection(&m_cs);

			return iRes;
		}

		static const int STATE_IDLE = 0;
		static const int STATE_DISCONNECTED = 0;
		static const int STATE_CONNECTING = 1;
		static const int STATE_CONNECTED = 2;
		static const int STATE_DISCONNECTING = 3;
		static const int STATE_ERROR = 21;
		static const int STATE_ACCEPTING = 4;

	private:
		CRITICAL_SECTION m_cs;
		int m_iState;
	};

public:
	CProxyConnection();
	virtual ~CProxyConnection();

	bool isDead() { return m_cState.GetState() == CConnState::STATE_DISCONNECTED; }

protected:
	bool initConnection(const sockaddr_in* addr);

	virtual void HandleConnState();

	virtual void HandleAccepting();
	virtual void HandleConnecting();
	virtual void HandleConnection();
	virtual void HandleDisconnect();

	bool recvBuffer(SOCKET s, char* buf, int& buf_size);
	bool sendBuffer(SOCKET s, char* buf, int buf_size);


private:
	void term();
	void cleanup();
	bool startThread();

	static DWORD CALLBACK ConnectionThread(CProxyConnection* pThis);

protected:
	CServerEndpoint m_cServer;
	CClientEndpoint m_cClient;

	CConnState m_cState;
	DWORD m_dwConnTicks;

	HANDLE m_hConnTread;
	HANDLE m_hKillEvent;
};


#endif //__ProxyConnection_H