#include "stdafx.h"
#include "PassThroughProxy.h"
#include "Proxifier.h"
#include "DebugOut.h"

/**
 *
 */
CPassThroughProxy::CPassThroughProxy(SOCKET s)
	: CProxy(s)
{
}


/**
 *
 */
CPassThroughProxy::~CPassThroughProxy()
{
}


/**
 *
 */
int CPassThroughProxy::connect(const struct sockaddr* addr, int len)
{
	if (len != sizeof(sockaddr_in))
		return SOCKET_ERROR;

	CDebugOut::PrintInfo("Connect to server: %d.%d.%d.%d:%d", 
		((sockaddr_in*)addr)->sin_addr.S_un.S_un_b.s_b1, ((sockaddr_in*)addr)->sin_addr.S_un.S_un_b.s_b2, 
		((sockaddr_in*)addr)->sin_addr.S_un.S_un_b.s_b3, ((sockaddr_in*)addr)->sin_addr.S_un.S_un_b.s_b4,
		MAKEWORD(HIBYTE(((sockaddr_in*)addr)->sin_port), LOBYTE(((sockaddr_in*)addr)->sin_port)));


	if (!initConnection((const sockaddr_in*)addr))
		return SOCKET_ERROR;

	addr = (const struct sockaddr*)m_cClient.getAddr();

	CDebugOut::PrintInfo("Connect to proxy: %d.%d.%d.%d:%d", 
		((sockaddr_in*)addr)->sin_addr.S_un.S_un_b.s_b1, ((sockaddr_in*)addr)->sin_addr.S_un.S_un_b.s_b2, 
		((sockaddr_in*)addr)->sin_addr.S_un.S_un_b.s_b3, ((sockaddr_in*)addr)->sin_addr.S_un.S_un_b.s_b4,
		MAKEWORD(HIBYTE(((sockaddr_in*)addr)->sin_port), LOBYTE(((sockaddr_in*)addr)->sin_port)));

	int res = CProxifier::connect(m_id, addr, len);

	if (res == SOCKET_ERROR)
		CDebugOut::PrintLastError("CPassThroughProxy::connect()", CProxifier::WSAGetLastError());

	return res;
}

