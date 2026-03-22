#include "stdafx.h"
#include "ProxyEndpoint.h"
#include "DebugOut.h"
#include "Proxifier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**  
 * \brief 
 */
void CProxyEndpoint::cleanup()
{
	if (m_sock && m_sock != INVALID_SOCKET)
	{
		u_long param = 0; // set blocking i/o
		CProxifier::ioctlsocket(m_sock, FIONBIO, &param);

		CProxifier::WSASendDisconnect(m_sock, 0);

		if (SOCKET_ERROR == CProxifier::shutdown(m_sock, SD_BOTH))
			CDebugOut::PrintLastError("shutdown()", CProxifier::WSAGetLastError());

		if (SOCKET_ERROR == CProxifier::closesocket(m_sock))
			CDebugOut::PrintLastError("closesocket()", CProxifier::WSAGetLastError());
	}

	m_sock = 0;
	memset(&m_sockaddr, 0, sizeof(m_sockaddr));
}


/**  
 * \brief 
 */
bool CClientEndpoint::init()
{
	return createSocket()
			 && bindSocket()
			 && listenSocket();
}


/**  
 * \brief 
 */
void CClientEndpoint::term()
{
	cleanup();

	if (m_sockListen != 0 && m_sockListen != INVALID_SOCKET)
		CProxifier::closesocket(m_sockListen);

	m_sockListen = 0;
}



/**  
 * \brief 
 */
bool CClientEndpoint::createSocket()
{
	m_sockListen = CProxifier::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (!m_sockListen || m_sockListen == INVALID_SOCKET)
	{
		CDebugOut::PrintError("Error creating listening socket ...");
		return false;
	}

	return true;
}


/**  
 * \brief 
 */
bool CClientEndpoint::bindSocket()
{
	u_short port = (u_short)2772;

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = MAKEWORD(HIBYTE(port), LOBYTE(port));
	m_sockaddr.sin_addr.S_un.S_addr = 0x0100007F;

	for (int i=0; i < 1000; i++)
	{
		if (0 == CProxifier::bind(m_sockListen, (const sockaddr*)&m_sockaddr, sizeof(sockaddr_in)))
		{
			CDebugOut::PrintInfo("Socket bound to %d.%d.%d.%d on port %d ...", m_sockaddr.sin_addr.S_un.S_un_b.s_b1, 
				m_sockaddr.sin_addr.S_un.S_un_b.s_b2, m_sockaddr.sin_addr.S_un.S_un_b.s_b3, 
				m_sockaddr.sin_addr.S_un.S_un_b.s_b4, port);

			return true;
		}

		DWORD dwErr = CProxifier::WSAGetLastError();

		if (WSAEADDRINUSE != dwErr)
		{
			CDebugOut::PrintLastError("bind()", dwErr);
			return false;
		}

		port++;
		m_sockaddr.sin_port = MAKEWORD(HIBYTE(port), LOBYTE(port));
	}

	return false;
}


/**  
 * \brief 
 */
bool CClientEndpoint::listenSocket()
{
	// set socket to non-blocking i/o mode
	u_long param = 1;
	if (0 != CProxifier::ioctlsocket(m_sockListen, FIONBIO, &param))
	{
		CDebugOut::PrintLastError("ioctlsocket()", CProxifier::WSAGetLastError());
		return false;
	}

	int res = CProxifier::listen(m_sockListen, 5);

	if (res != 0)
	{
		CDebugOut::PrintLastError("listen()", CProxifier::WSAGetLastError());
	}
	else
	{
		CDebugOut::PrintInfo("Listening ...");
	}

	return (res == 0);
}


/**  
 * \brief 
 */
bool CClientEndpoint::accept()
{
	m_sock = CProxifier::accept(m_sockListen, 0, 0);

	if (m_sock == INVALID_SOCKET)
	{
		m_sock = 0;

		CDebugOut::PrintError("CClientEndpoint::accept() failed.");
		CDebugOut::PrintLastError("accept()", CProxifier::WSAGetLastError());

		return false;
	}

	return true;
}


/**  
 * \brief 
 */
int CClientEndpoint::canAccept()
{
	if (!m_sockListen || m_sockListen == INVALID_SOCKET)
	{
		CDebugOut::PrintError("CClientEndpoint::accept() -> Error: proxy not initialized.");
		return -1;
	}

	fd_set rfd = {0};
	rfd.fd_count = 1;
	rfd.fd_array[0] = m_sockListen;

	TIMEVAL tv = {0};

	int res = CProxifier::select(0, &rfd, 0, 0, &tv);

	if (res == 0)
	{
		return 0; // timeout expired
	}
	else if (res == SOCKET_ERROR)
	{
		CDebugOut::PrintError("CServerListeningSocket::accept() failed.");
		CDebugOut::PrintLastError("select()", CProxifier::WSAGetLastError());
		return -1;
	}

	return 1;
}


/**  
 * \brief 
 */
bool CServerEndpoint::init(const sockaddr_in* sockaddr)
{
	memcpy(&m_sockaddr, sockaddr, sizeof(sockaddr_in));

	m_sock = CProxifier::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (!m_sock || m_sock == INVALID_SOCKET)
	{
		CDebugOut::PrintLastError("socket()", CProxifier::WSAGetLastError());
		return false;
	}

	u_long param = 1; // set non-blocking i/o

	if (SOCKET_ERROR == CProxifier::ioctlsocket(m_sock, FIONBIO, &param))
	{
		CDebugOut::PrintLastError("ioctlsocket()", CProxifier::WSAGetLastError());
		return false;
	}


	DWORD dwErr = 0;

	if (0 != CProxifier::connect(m_sock, (const struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr))
			&& WSAEWOULDBLOCK != (dwErr = CProxifier::WSAGetLastError()))
	{
		CDebugOut::PrintLastError("connect()", dwErr);
		return false;
	}

	return true;
}


/**  
 * \brief 
 */
void CServerEndpoint::term()
{
	cleanup();
}
