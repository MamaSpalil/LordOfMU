#ifndef __ProxyEndpoint_H
#define __ProxyEndpoint_H

#pragma once


/**  
 * \brief 
 */
class CProxyEndpoint
{
public:
	CProxyEndpoint() : m_sock(0) { memset(&m_sockaddr, 0, sizeof(m_sockaddr)); }
	virtual ~CProxyEndpoint() { cleanup(); }

	const sockaddr_in* getAddr(){ return &m_sockaddr; }
	SOCKET getSock() { return m_sock; }

protected:
	void cleanup();

protected:
	struct sockaddr_in m_sockaddr;
	SOCKET m_sock;
};


/**  
 * \brief 
 */
class CClientEndpoint : public CProxyEndpoint
{
public:
	CClientEndpoint() : m_sockListen(0) {}
	virtual ~CClientEndpoint(){}

public:
	bool init();
	void term();
	bool accept();

	int canAccept();

protected:
	bool createSocket();
	bool bindSocket();
	bool listenSocket();

private:
	SOCKET m_sockListen;
};



/**  
 * \brief 
 */
class CServerEndpoint : public CProxyEndpoint
{
public:
	CServerEndpoint(){}
	virtual ~CServerEndpoint(){}

public:
	bool init(const sockaddr_in* sockaddr);
	void term();
};



#endif //__ProxyEndpoint_H