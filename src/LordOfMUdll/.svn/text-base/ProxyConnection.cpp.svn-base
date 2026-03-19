#include "stdafx.h"
#include "ProxyConnection.h"
#include "Proxifier.h"
#include "DebugOut.h"
#include "AutoBuffer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * \brief 
 */
CProxyConnection::CProxyConnection()
{
	m_hConnTread = 0;
	m_hKillEvent = CreateEvent(0, 1, 0, 0);

	if (m_hKillEvent == INVALID_HANDLE_VALUE)
		m_hKillEvent = 0;
}


/**  
 * \brief 
 */
CProxyConnection::~CProxyConnection()
{
	term();

	if (m_hKillEvent)
		CloseHandle(m_hKillEvent);
}


/**
 * \brief 
 */
bool CProxyConnection::initConnection(const sockaddr_in* addr)
{
	if (m_cState.GetState() != CConnState::STATE_IDLE)
		return false;

	m_dwConnTicks = GetTickCount();
	m_cState = CConnState::STATE_ACCEPTING;

	if (!m_cClient.init() || !m_cServer.init(addr) || !startThread())
	{
		term();
		m_cState = CConnState::STATE_IDLE;
		return false;
	}

	return true;
}


/**
 * \brief 
 */
void CProxyConnection::term()
{
	if (m_hConnTread)
	{
		if (m_hKillEvent)
			SetEvent(m_hKillEvent);

		if (WaitForSingleObject(m_hConnTread, 5000) == WAIT_TIMEOUT)
			TerminateThread(m_hConnTread, 0);

		CloseHandle(m_hConnTread);
	}

	m_hConnTread = 0;

	cleanup();
}


void CProxyConnection::cleanup()
{
	m_cClient.term();
	m_cServer.term();
}


/**  
 * \brief 
 */
bool CProxyConnection::startThread()
{
	ResetEvent(m_hKillEvent);
	m_hConnTread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ConnectionThread, this, 0, 0);

	if (!m_hConnTread || m_hConnTread == INVALID_HANDLE_VALUE)
	{
		m_hConnTread = 0;
		return false;
	}

	SetThreadPriority(m_hConnTread, THREAD_PRIORITY_ABOVE_NORMAL);
	return true;
}


/**  
 * \brief 
 */
DWORD CALLBACK CProxyConnection::ConnectionThread(CProxyConnection* pThis)
{
	if (!pThis)
		return 0;

	Sleep(100);

	while (WaitForSingleObject(pThis->m_hKillEvent, 0) == WAIT_TIMEOUT)
	{
		pThis->HandleConnState();
		Sleep(10);
	}

	CDebugOut::PrintInfo("Communication handling thread terminated ...");
	return 0;
}


/**  
 * \brief 
 */
void CProxyConnection::HandleConnState()
{
	switch (m_cState.GetState())
	{
	case CConnState::STATE_ACCEPTING:
		HandleAccepting();
		break;
	case CConnState::STATE_CONNECTING:
		HandleConnecting();
		break;
	case CConnState::STATE_CONNECTED:
		HandleConnection();
		break;
	case CConnState::STATE_ERROR:
		HandleDisconnect();
		break;
	case CConnState::STATE_DISCONNECTING:
		HandleDisconnect();
		break;
	case CConnState::STATE_IDLE:
	default:
		break;
	}
}



/**  
 * \brief 
 */
void CProxyConnection::HandleAccepting()
{
	int res = m_cClient.canAccept();

	if (res == -1)
	{
		CDebugOut::PrintError("CProxyConnection::HandleConnection() error accepting client.");
		m_cState = CConnState::STATE_ERROR;
	}
	else if (res == 1)
	{
		if (!m_cClient.accept())
		{
			CDebugOut::PrintError("CProxyConnection::HandleConnection() error accepting client.");
			m_cState = CConnState::STATE_ERROR;
		}
		else
		{
			CDebugOut::PrintInfo("Client accepted.");

			m_cState = CConnState::STATE_CONNECTING;
			m_dwConnTicks = GetTickCount();
		}
	}
	else if (GetTickCount() - m_dwConnTicks > 15000)
	{
		CDebugOut::PrintError("CProxyConnection::HandleConnection() timeout accepting client.");
		m_cState = CConnState::STATE_ERROR;
	}
}


/**  
 * \brief 
 */
void CProxyConnection::HandleConnecting()
{
	fd_set wfd = {0};
	wfd.fd_count = 1;
	wfd.fd_array[0] = m_cServer.getSock();

	TIMEVAL tvC = {0};

	int res = CProxifier::select(0, 0, &wfd, 0, &tvC);

	if (res != 0)
	{
		if (res != 1)
		{
			CDebugOut::PrintError("CProxyConnection::HandleConnection() SV:connect failed.");
			CDebugOut::PrintLastError("select()", CProxifier::WSAGetLastError());
			m_cState = CConnState::STATE_ERROR;
		}
		else
		{
			CDebugOut::PrintInfo("Connected to server.");
			m_cState = CConnState::STATE_CONNECTED;
		}
	}
	else if (GetTickCount() - m_dwConnTicks > 15000)
	{
		CDebugOut::PrintError("CProxyConnection::HandleConnection() connecting to server timeout.");
		m_cState = CConnState::STATE_ERROR;
	}
}


/**  
 * \brief 
 */
void CProxyConnection::HandleDisconnect()
{
	CDebugOut::PrintInfo("Disconnected from server.");

	cleanup();
	m_cState = CConnState::STATE_DISCONNECTED;
}


/**  
 * \brief 
 */
void CProxyConnection::HandleConnection()
{
	int buf_size = 1024;
	CCharAutoBuffer buf(buf_size);

	if (recvBuffer(m_cServer.getSock(), buf, buf_size))
	{
		sendBuffer(m_cClient.getSock(), buf, buf_size);
	}

	buf_size = 1024;
	if (recvBuffer(m_cClient.getSock(), buf, buf_size))
	{
		sendBuffer(m_cServer.getSock(), buf, buf_size);
	}
}


/**  
 * \brief 
 */
bool CProxyConnection::recvBuffer(SOCKET s, char* buf, int& buf_size)
{
	DWORD dwErr = 0;

	fd_set rfd = {0};
	rfd.fd_count = 1;
	rfd.fd_array[0] = s;

	TIMEVAL tv = {0};

	int res = CProxifier::select(0, &rfd, 0, 0, &tv);

	if (res == 0)
		return false;
	
	if (res != 1)
	{
		CDebugOut::PrintError("CServerProxy::HandleConnection() recvBuffer() failed.");
		CDebugOut::PrintLastError("select()", CProxifier::WSAGetLastError());
		m_cState = CConnState::STATE_ERROR;
		return false;
	}

	int ret = CProxifier::recv(s, buf, buf_size, 0);

	if (ret == 0)
	{
		CDebugOut::PrintDebug("Socket closed.");
		m_cState = CConnState::STATE_DISCONNECTING;
		return false; // socket has been closed
	}

	if (ret == SOCKET_ERROR)
	{
		dwErr = CProxifier::WSAGetLastError();

		if (dwErr != WSAEWOULDBLOCK && dwErr != ERROR_CONNECTION_ABORTED
				&& dwErr != ERROR_SEM_TIMEOUT)
		{
			CDebugOut::PrintLastError("recv()", dwErr);
			m_cState = CConnState::STATE_ERROR;
		}
		else
		{
			CDebugOut::PrintLastError("recv()", dwErr);
		}

		return false;
	}

	buf_size = ret;
	return true;
}


/**  
 * \brief 
 */
bool CProxyConnection::sendBuffer(SOCKET s, char* buf, int buf_size)
{
	DWORD dwErr = 0;

	if (SOCKET_ERROR == CProxifier::send(s, buf, buf_size, 0)
			&& WSAEWOULDBLOCK != (dwErr = CProxifier::WSAGetLastError()))
	{
		CDebugOut::PrintLastError("send()", dwErr);
		m_cState = CConnState::STATE_ERROR;
		return false;
	}

	return true;
}


