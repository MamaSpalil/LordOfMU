#ifndef __Proxifier_H
#define __Proxifier_H

#pragma once

#include <map>
#include "ApiHooker.h"
#include "version.h"

class CProxy;

typedef std::map<SOCKET, CProxy*> CProxyList;


/**
 *
 */
class CProxifier
{
public:
	CProxifier();
	virtual ~CProxifier();

public:
	static bool StartUp();
	static void CleanUp();

	static CProxifier* GetInstance();

public:	
	static int WINAPI connect2(SOCKET, const struct sockaddr*, int);

public:
	static CAcceptPtr accept;
	static CWSAGetLastErrorPtr WSAGetLastError;
	static CWSASendDisconnectPtr WSASendDisconnect;
	static CCloseSocketPtr closesocket;

	static CShutdownPtr shutdown;
	static CSendPtr send;
	static CRecvPtr recv;
	static CSocketConnectTramp connect;
	static CSelectPtr select;
	static CListenPtr listen;
	static CBindPtr bind;
	static CSocketPtr socket;
	static CIoctlSocketPtr ioctlsocket;


protected:
	bool InternalInit();
	void InternalClean();

protected:
	static DWORD WINAPI PatchMonitor(CProxifier*);
	void StartPatchMonitor();
	void StopPatchMonitor();
	void CheckWinsockDir();
	void CheckPatches();
	void CheckMyPatch(const char* szDll, const char* szFunc, DWORD_PTR dwHookProc);
	void CheckNotPatched(const char* szDll, const char* szFunc);
	void CheckForIATHook(const char* szDll, const char* szFunc);
	static BOOL CALLBACK CheckForHastyProc(HWND hwnd, LPARAM lParam);

	void cleanDead();

protected:
	HANDLE m_hPatchMonitor;
	HANDLE m_hRunMonitor;

protected:
	HANDLE m_hBypassEvent;
	CProxyList m_vProxies;

private:
	static CProxifier* m_sInstance;
};


#endif //__Proxifier_H