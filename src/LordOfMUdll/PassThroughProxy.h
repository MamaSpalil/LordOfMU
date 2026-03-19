#ifndef __PassThroughProxy_H
#define __PassThroughProxy_H

#pragma once

#include "Proxy.h"
#include "ProxyConnection.h"


/**
 * \brief
 */
class CPassThroughProxy 
		: public CProxy, public CProxyConnection
{
public:
	CPassThroughProxy(SOCKET s);
	virtual ~CPassThroughProxy();

public:
	// Socket Interface
	virtual int connect(const struct sockaddr* addr, int len);
	virtual bool isDead(){ return CProxyConnection::isDead(); };
};

#endif //__PassThroughProxy_H