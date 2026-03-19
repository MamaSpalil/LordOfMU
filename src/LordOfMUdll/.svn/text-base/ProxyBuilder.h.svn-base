#ifndef __ProxyBuilder_H
#define __ProxyBuilder_H

#include "Proxy.h"

/**
 * \brief 
 */
class CProxyBuilder
{
public:
	static CProxy* CreateProxy(SOCKET s, const struct sockaddr* addr, int len);

private:
	static CProxy* CreateBypassProxy(SOCKET s);
	static CProxy* CreateGameProxy(SOCKET s);
};

#endif // __ProxyBuilder_H