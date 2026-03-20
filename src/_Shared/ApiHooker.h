#ifndef __ApiHooker_H
#define __ApiHooker_H


#include "PEUtil.h"
#include "winsock2.h"


/**
 * \brief 
 */
class CApiHooker
{
public:
	CApiHooker();
	virtual ~CApiHooker() {}

	bool PatchFunction(const char* szDll, const char* szFunc, DWORD_PTR dwHookProc);
	bool UnPatchFunction(const char* szDll, const char* szFunc);

	bool PatchFunction(BYTE* pFunc, DWORD_PTR dwHookProc);
	bool UnPatchFunction(BYTE* pFunc);

	static BYTE* FindPatchPoint(BYTE* pFunc, DWORD_PTR dwHookProc);
	static BYTE* FindPatchedAddress(BYTE* pFunc, DWORD_PTR dwHookProc);

protected:
	BYTE m_abTrampBuff[64];
	DWORD m_dwPatchSize;
	DWORD_PTR m_dwHookProc;
	void* m_pfnApi;
	bool m_fPatched;
};



/**
 * \brief 
 */
class CApiDllMirror
{
public:
	CApiDllMirror(const char* pszDllName);
	virtual ~CApiDllMirror();

	void* GetProcAddress(const char* pszProc);

protected:
	HMODULE m_hDll;
};


/**
 * \brief 
 */
class CWinsockApiMirror 
	: public CApiDllMirror
{
public:
	CWinsockApiMirror();
	virtual ~CWinsockApiMirror();
};


/**
 * \brief 
 */
class CWSASendTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI *WSASendPtr)(SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);

public:
	bool Patch(WSASendPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "WSASend", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "WSASend"); }

	int operator()(SOCKET s, LPWSABUF lpBuffs, DWORD dwCount, LPDWORD lpSent, DWORD flags, LPWSAOVERLAPPED ovlpd, LPWSAOVERLAPPED_COMPLETION_ROUTINE compl)
	{
		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		WSASendPtr pfn = (WSASendPtr)(void*)m_abTrampBuff;
		return pfn(s, lpBuffs, dwCount, lpSent, flags, ovlpd, compl);
	}
};


/**
 * \brief 
 */
class CCloseSocketTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI *CloseSocketPtr)(SOCKET);

public:
	bool Patch(CloseSocketPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "closesocket", PtrToUlong(pfnHookProc)); }
	bool Patch(CloseSocketPtr pfnHookProc, CApiDllMirror& dllMirror)
	{ 
		m_pfnApi = dllMirror.GetProcAddress("closesocket");
		return PatchFunction("ws2_32.dll", "closesocket", PtrToUlong(pfnHookProc)); 
	}
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "closesocket"); }

	int operator()(SOCKET s)
	{
		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		CloseSocketPtr pfn = (CloseSocketPtr)(void*)m_abTrampBuff;
		return pfn(s);
	}
};


/**
 * \brief 
 */
class CShutdownPtr
	: public CApiHooker
{
public:
	typedef int (WINAPI *ShutdownPtr)(SOCKET s, int how);

public:
	CShutdownPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (ShutdownPtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "shutdown")); }

	int operator()(SOCKET s, int how)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, how);
	}

private:
	ShutdownPtr m_pfn;
};


/**
 * \brief 
 */
class CSocketTramp
	: public CApiHooker
{
public:
	typedef SOCKET (WINAPI *SocketPtr)(int af, int type, int protocol);

public:
	bool Patch(SocketPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "socket", PtrToUlong(pfnHookProc)); }
	bool Patch(SocketPtr pfnHookProc, CApiDllMirror& dllMirror)
	{ 
		m_pfnApi = dllMirror.GetProcAddress("socket");
		return PatchFunction("ws2_32.dll", "socket", PtrToUlong(pfnHookProc)); 
	}

	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "socket"); }

	SOCKET operator()(int af, int type, int protocol)
	{
		if (m_pfnApi)
			return ((SocketPtr)m_pfnApi)(af, type, protocol);

		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		SocketPtr pfn = (SocketPtr)(void*)m_abTrampBuff;
		return pfn(af, type, protocol);
	}
};


/**
 * \brief 
 */
class CCloseSocketPtr
	: public CApiHooker
{
public:
	typedef int (WINAPI *CloseSocketPtr)(SOCKET);

public:
	CCloseSocketPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (CloseSocketPtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "closesocket")); }

	int operator()(SOCKET s)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s);
	}

private:
	CloseSocketPtr m_pfn;
};


/**
 * \brief 
 */
class CWSARecvTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI *WSARecvPtr)(SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);

public:
	bool Patch(WSARecvPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "WSARecv", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "WSARecv"); }

	int operator()(SOCKET s, LPWSABUF lpBuffs, DWORD dwCount, LPDWORD lpRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED ovlpd, LPWSAOVERLAPPED_COMPLETION_ROUTINE compl)
	{
		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		WSARecvPtr pfn = (WSARecvPtr)(void*)m_abTrampBuff;
		return pfn(s, lpBuffs, dwCount, lpRecvd, lpFlags, ovlpd, compl);
	}
};


/**
 * \brief 
 */
class CWSAGetLastErrorPtr
	: public CApiHooker
{
public:
	typedef int (WINAPI *WSAGetLastErrorPtr)();

public:
	CWSAGetLastErrorPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (WSAGetLastErrorPtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "WSAGetLastError")); }
	bool Init(CApiDllMirror& dllMirror)
	{
		return 0 != (m_pfn = (WSAGetLastErrorPtr)dllMirror.GetProcAddress("WSAGetLastError")); 
	}

	int operator()()
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn();
	}

private:
	WSAGetLastErrorPtr m_pfn;
};


/**
 * \brief 
 */
class CWSAGetLastErrorTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI *WSAGetLastErrorPtr)();

public:
	bool Patch(WSAGetLastErrorPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "WSAGetLastError", PtrToUlong(pfnHookProc)); }
	bool Patch(WSAGetLastErrorPtr pfnHookProc, CApiDllMirror& dllMirror)
	{ 
		m_pfnApi = dllMirror.GetProcAddress("WSAGetLastError");
		return PatchFunction("ws2_32.dll", "WSAGetLastError", PtrToUlong(pfnHookProc)); 
	}
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "WSAGetLastError"); }

	int operator()()
	{
		if (m_pfnApi)
			return ((WSAGetLastErrorPtr)m_pfnApi)();

		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		WSAGetLastErrorPtr pfn = (WSAGetLastErrorPtr)(void*)m_abTrampBuff;
		return pfn();
	}
};


/**
 * \brief 
 */
class CWSASendDisconnectPtr
	: public CApiHooker
{
public:
	typedef int (WINAPI *WSASendDisconnectPtr)(SOCKET, LPWSABUF);

public:
	CWSASendDisconnectPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (WSASendDisconnectPtr) GetProcAddress(GetModuleHandleA("ws2_32.dll"), "WSASendDisconnect")); }

	int operator()(SOCKET s, LPWSABUF lpBuffers)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, lpBuffers);		
	}

private:
	WSASendDisconnectPtr m_pfn;
};


/**
 * \brief 
 */
class CIoctlSocketPtr
	: public CApiHooker
{
public:
	typedef int (WSAAPI* IoctlSocketPtr)(SOCKET s, long cmd, u_long* argp);

public:
	CIoctlSocketPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (IoctlSocketPtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "ioctlsocket")); }

	int operator()(SOCKET s, long cmd, u_long* argp)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, cmd, argp);		
	}

private:
	IoctlSocketPtr m_pfn;
};


/**
 * \brief 
 */
class CCreateIoCompletionPortTramp
	: public CApiHooker
{
public:
	typedef HANDLE (WINAPI *CreateIoCompletionPortPtr)(HANDLE, HANDLE, ULONG_PTR, DWORD);

public:
	bool Patch(CreateIoCompletionPortPtr pfnHookProc, void* pDosHeader){ return PatchFunction("kernel32.dll", "CreateIoCompletionPort", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("kernel32.dll", "CreateIoCompletionPort"); }

	HANDLE operator()(HANDLE hFile, HANDLE hPort, ULONG_PTR ulKey, DWORD dwThreads)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		CreateIoCompletionPortPtr pfn = (CreateIoCompletionPortPtr)(void*)m_abTrampBuff;
		return pfn(hFile, hPort, ulKey, dwThreads);
	}
};


/**
 * \brief 
 */
class CGetQueuedCompletionStatusTramp
	: public CApiHooker
{
public:
	typedef BOOL (WINAPI *GetQueuedCompletionStatusPtr)(HANDLE, LPDWORD, PULONG_PTR, LPOVERLAPPED*, DWORD);

public:
	bool Patch(GetQueuedCompletionStatusPtr pfnHookProc, void* pDosHeader){ return PatchFunction("kernel32.dll", "GetQueuedCompletionStatus", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return  UnPatchFunction("kernel32.dll", "GetQueuedCompletionStatus"); }

	BOOL operator()(HANDLE hPort, LPDWORD lpTransf, PULONG_PTR ulKey, LPOVERLAPPED* ovlpd, DWORD dwTime)
	{
		if (m_abTrampBuff[0] == 0)
			return FALSE;

		GetQueuedCompletionStatusPtr pfn = (GetQueuedCompletionStatusPtr)(void*)m_abTrampBuff;
		return pfn(hPort, lpTransf, ulKey, ovlpd, dwTime);
	}
};


/**
 * \brief 
 */
class CSocketRecvTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI *RecvPtr)(SOCKET, char*, int, int);

public:
	bool Patch(RecvPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "recv", PtrToUlong(pfnHookProc)); }
	bool Patch(RecvPtr pfnHookProc, CApiDllMirror& dllMirror)
	{ 
		m_pfnApi = dllMirror.GetProcAddress("recv");
		return PatchFunction("ws2_32.dll", "recv", PtrToUlong(pfnHookProc)); 
	}
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "recv"); }

	int operator()(SOCKET s, char* buf, int len, int flags)
	{
		if (m_pfnApi)
			return ((RecvPtr)m_pfnApi)(s, buf, len, flags);

		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		RecvPtr pfn = (RecvPtr)(void*)m_abTrampBuff;
		return pfn(s, buf, len, flags);
	}
};


/**
 * \brief 
 */
class CSocketSendTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI *SendPtr)(SOCKET, char*, int, int);

public:
	bool Patch(SendPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "send", PtrToUlong(pfnHookProc)); }
	bool Patch(SendPtr pfnHookProc, CApiDllMirror& dllMirror)
	{ 
		m_pfnApi = dllMirror.GetProcAddress("send");
		return PatchFunction("ws2_32.dll", "send", PtrToUlong(pfnHookProc)); 
	}
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "send"); }

	int operator()(SOCKET s, char* buf, int len, int flags)
	{
		if (m_pfnApi)
			return ((SendPtr)m_pfnApi)(s, buf, len, flags);

		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		SendPtr pfn = (SendPtr)(void*)m_abTrampBuff;
		return pfn(s, buf, len, flags);
	}
};


/**
 * \brief 
 */
class CSocketConnectTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI *ConnectPtr)(SOCKET, const struct sockaddr*, int);

public:
	bool Patch(ConnectPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "connect", PtrToUlong(pfnHookProc)); }
	bool Patch(ConnectPtr pfnHookProc, CApiDllMirror& dllMirror)
	{ 
		m_pfnApi = dllMirror.GetProcAddress("connect");
		return PatchFunction("ws2_32.dll", "connect", PtrToUlong(pfnHookProc)); 
	}

	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "connect"); }

	int operator()(SOCKET s, const struct sockaddr* addr, int len)
	{
		if (m_pfnApi)
			return ((ConnectPtr)m_pfnApi)(s, addr, len);

		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		ConnectPtr pfn = (ConnectPtr)(void*)m_abTrampBuff;
		return pfn(s, addr, len);
	}
};


/**
 * \brief 
 */
class CConnectPtr
	: public CApiHooker
{
public:
	typedef int (WINAPI *ConnectPtr)(SOCKET, const struct sockaddr*, int);

public:
	CConnectPtr() : m_pfn(0) {}
	bool Init(){ return  0 != (m_pfn = (ConnectPtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "connect")); }

	int operator()(SOCKET s, const struct sockaddr* addr, int len)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, addr, len);
	}

private:
	ConnectPtr m_pfn;
};


/**
 * \brief 
 */
class CWSAAsyncSelectTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI *WSAAsyncSelectPtr)(SOCKET, HWND, unsigned int, long);

public:
	bool Patch(WSAAsyncSelectPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "WSAAsyncSelect", PtrToUlong(pfnHookProc)); }
	bool Patch(WSAAsyncSelectPtr pfnHookProc, CApiDllMirror& dllMirror)
	{ 
		m_pfnApi = dllMirror.GetProcAddress("WSAAsyncSelect");
		return PatchFunction("ws2_32.dll", "WSAAsyncSelect", PtrToUlong(pfnHookProc));
	}

	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "WSAAsyncSelect"); }

	int operator()(SOCKET s, HWND hwnd, unsigned int umsg, long flags)
	{
		if (m_pfnApi)
			return ((WSAAsyncSelectPtr)m_pfnApi)(s, hwnd, umsg, flags);

		if (m_abTrampBuff[0] == 0)
			return SOCKET_ERROR;

		WSAAsyncSelectPtr pfn = (WSAAsyncSelectPtr)(void*)m_abTrampBuff;
		return pfn(s, hwnd, umsg, flags);
	}
};


/**
 * \brief 
 */
class CPeekMessageTramp
	: public CApiHooker
{
public:
	typedef BOOL (WINAPI *PeekMessagePtr)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

public:
	bool Patch(PeekMessagePtr pfnHookProc){ return PatchFunction("user32.dll", "PeekMessageA", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("user32.dll", "PeekMessageA"); }

	BOOL operator()(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
	{
		if (m_abTrampBuff[0] == 0)
			return FALSE;

		PeekMessagePtr pfn = (PeekMessagePtr)(void*)m_abTrampBuff;
		return pfn(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
	}
};


/**
 * \brief 
 */
class CGetMessageTramp
	: public CApiHooker
{
public:
	typedef BOOL (WINAPI *GetMessagePtr)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);

public:
	bool Patch(GetMessagePtr pfnHookProc){ return PatchFunction("user32.dll", "GetMessageA", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("user32.dll", "GetMessageA"); }

	BOOL operator()(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
	{
		if (m_abTrampBuff[0] == 0)
			return FALSE;

		GetMessagePtr pfn = (GetMessagePtr)(void*)m_abTrampBuff;
		return pfn(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
	}
};


/**
 * \brief 
 */
class CCreateWindowExATramp
	: public CApiHooker
{
public:
	typedef HWND (WINAPI *CreateWindowExAPtr)(DWORD dwExStyle,
		LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y,
		int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
		HINSTANCE hInstance, LPVOID lpParam);

public:
	bool Patch(CreateWindowExAPtr pfnHookProc){ return PatchFunction("user32.dll", "CreateWindowExA", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("user32.dll", "CreateWindowExA"); }

	HWND operator()(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y,
						int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
						HINSTANCE hInstance, LPVOID lpParam)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		CreateWindowExAPtr pfn = (CreateWindowExAPtr)(void*)m_abTrampBuff;
		return pfn(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}
};


/**
 * \brief 
 */
class CWSAGetOverlappedResultTramp
	: public CApiHooker
{
public:
	typedef BOOL (WINAPI *WSAGetOverlappedResultPtr)(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags);

public:
	bool Patch(WSAGetOverlappedResultPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "WSAGetOverlappedResult", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "WSAGetOverlappedResult"); }

	BOOL operator()(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		WSAGetOverlappedResultPtr pfn = (WSAGetOverlappedResultPtr)(void*)m_abTrampBuff;
		return pfn(s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);
	}
};



/**
 * \brief 
 */
class CWSAAcceptTramp
	: public CApiHooker
{
public:
	typedef SOCKET (WINAPI *WSAAcceptPtr)(SOCKET s, struct sockaddr* addr, LPINT addrlen, LPCONDITIONPROC lpfnCondition, DWORD dwCallbackData);

public:
	bool Patch(WSAAcceptPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "WSAAccept", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "WSAAccept"); }

	SOCKET operator()(SOCKET s, struct sockaddr* addr, LPINT addrlen, LPCONDITIONPROC lpfnCondition, DWORD dwCallbackData)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		WSAAcceptPtr pfn = (WSAAcceptPtr)(void*)m_abTrampBuff;
		return pfn(s, addr, addrlen, lpfnCondition, dwCallbackData);
	}
};


/**
 * \brief 
 */
class CAcceptPtr
	: public CApiHooker
{
public:
	typedef SOCKET (WINAPI* AcceptPtr)(SOCKET s, struct sockaddr* addr, int* addrlen);
	
public:
	CAcceptPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (AcceptPtr) GetProcAddress(GetModuleHandleA("ws2_32.dll"), "accept")); }
	bool Init(CApiDllMirror& dllMirror)
	{
		return 0 != (m_pfn = (AcceptPtr) dllMirror.GetProcAddress("socket"));
	}

	SOCKET operator()(SOCKET s, struct sockaddr* addr, int* addrlen)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, addr, addrlen);
	}

private:
	AcceptPtr m_pfn;
};


/**
 * \brief 
 */
class CSocketPtr
	: public CApiHooker
{
public:
	typedef SOCKET (WINAPI* SocketPtr)(int af, int type, int protocol);

public:
	CSocketPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (SocketPtr) GetProcAddress(GetModuleHandleA("ws2_32.dll"), "socket")); }
	bool Init(CApiDllMirror& dllMirror)
	{
		return 0 != (m_pfn = (SocketPtr) dllMirror.GetProcAddress("socket"));
	}

	SOCKET operator()(int af, int type, int protocol)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(af, type, protocol);
	}

private:
	SocketPtr m_pfn;
};


/**
 * \brief 
 */
class CBindPtr
	: public CApiHooker
{
public:
	typedef int (WINAPI* BindPtr)(SOCKET s, const struct sockaddr* name, int namelen);

public:
	CBindPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (BindPtr) GetProcAddress(GetModuleHandleA("ws2_32.dll"), "bind")); }

	int operator()(SOCKET s, const struct sockaddr* name, int namelen)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, name, namelen);
	}

private:
	BindPtr m_pfn;
};



/**
 * \brief 
 */
class CBindTramp
	: public CApiHooker
{
public:
	typedef int (WINAPI* BindPtr)(SOCKET s, const struct sockaddr* name, int namelen);

public:
	bool Patch(BindPtr pfnHookProc){ return PatchFunction("ws2_32.dll", "bind", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("ws2_32.dll", "bind"); }

	int operator()(SOCKET s, const struct sockaddr* name, int namelen)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		BindPtr pfn = (BindPtr)(void*)m_abTrampBuff;
		return pfn(s, name, namelen);
	}
};



/**
 * \brief 
 */
class CListenPtr
	: public CApiHooker
{
public:
	typedef int (WINAPI* ListenPtr)(SOCKET s, int backlog);

public:
	CListenPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (ListenPtr) GetProcAddress(GetModuleHandleA("ws2_32.dll"), "listen")); }

	int operator()(SOCKET s, int backlog)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, backlog);
	}

private:
	ListenPtr m_pfn;
};


/**
 * \brief 
 */
class CSelectPtr
	: public CApiHooker
{
public:
	typedef int (WINAPI* SelectPtr)(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timeval* timeout);

public:
	CSelectPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (SelectPtr) GetProcAddress(GetModuleHandleA("ws2_32.dll"), "select")); }

	int operator()(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timeval* timeout)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(nfds, readfds, writefds, exceptfds, timeout);
	}

private:
	SelectPtr m_pfn;
};


/**
 * \brief 
 */
class CWSAEventSelectPtr
	: public CApiHooker
{
public:
	typedef int (WSAAPI* WSAEventSelectPtr)(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents);

public:
	CWSAEventSelectPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (WSAEventSelectPtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "WSAEventSelect")); }

	int operator()(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, hEventObject, lNetworkEvents);
	}

private:
	WSAEventSelectPtr m_pfn;
};


/**
 * \brief 
 */
class CGetSockNamePtr
	: public CApiHooker
{
public:
	typedef int (WSAAPI* GetSockNamePtr)(SOCKET s, struct sockaddr* name, int* namelen);
	
public:
	CGetSockNamePtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (GetSockNamePtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "getsockname")); }

	int operator()(SOCKET s, struct sockaddr* name, int* namelen)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, name, namelen);
	}

private:
	GetSockNamePtr m_pfn;
};


/**
 * \brief 
 */
class CSendPtr
	: public CApiHooker
{
public:
	typedef int (WSAAPI* SendPtr)(SOCKET s, const char* buf, int len, int flags);

public:
	CSendPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (SendPtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "send")); }

	int operator()(SOCKET s, const char* buf, int len, int flags)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, buf, len, flags);
	}

private:
	SendPtr m_pfn;
};



/**
 * \brief 
 */
class CRecvPtr
	: public CApiHooker
{
public:
	typedef int (WSAAPI* RecvPtr)(SOCKET s, char* buf, int len, int flags);

public:
	CRecvPtr() : m_pfn(0) {}
	bool Init(){ return 0 != (m_pfn = (RecvPtr)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "recv")); }

	int operator()(SOCKET s, char* buf, int len, int flags)
	{
		if (m_pfn == 0)
			return 0;

		return m_pfn(s, buf, len, flags);
	}

private:
	RecvPtr m_pfn;
};



/**
 * \brief 
 */
class CGetAsyncKeyStateTramp
	: public CApiHooker
{
public:
	typedef SHORT (WINAPI *GetAsyncKeyStatePtr)(int vKey);

public:
	bool Patch(GetAsyncKeyStatePtr pfnHookProc){ return PatchFunction("user32.dll", "GetAsyncKeyState", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("user32.dll", "GetAsyncKeyState"); }

	SHORT operator()(int vKey)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		GetAsyncKeyStatePtr pfn = (GetAsyncKeyStatePtr)(void*)m_abTrampBuff;
		return pfn(vKey);
	}
};


/**
 * \brief 
 */
class CChangeDisplaySettingsATramp
	: public CApiHooker
{
public:
	typedef LONG (WINAPI* ChangeDisplaySettingsAPtr)(LPDEVMODEA lpDevMode, DWORD dwflags);

public:
	bool Patch(ChangeDisplaySettingsAPtr pfnHookProc){ return PatchFunction("user32.dll", "ChangeDisplaySettingsA", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("user32.dll", "ChangeDisplaySettingsA"); }

	LONG operator()(LPDEVMODEA lpDevMode, DWORD dwflags)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		ChangeDisplaySettingsAPtr pfn = (ChangeDisplaySettingsAPtr)(void*)m_abTrampBuff;
		return pfn(lpDevMode, dwflags);
	}
};


/**
 * \brief 
 */
class CGetForegroundWindowTramp
	: public CApiHooker
{
public:
	typedef HWND (WINAPI *GetForegroundWindowPtr)();

public:
	bool Patch(GetForegroundWindowPtr pfnHookProc){ return PatchFunction("user32.dll", "GetForegroundWindow", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("user32.dll", "GetForegroundWindow"); }

	HWND operator()()
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		GetForegroundWindowPtr pfn = (GetForegroundWindowPtr)(void*)m_abTrampBuff;
		return pfn();
	}
};


/**
 * \brief 
 */
class CGetActiveWindowTramp
	: public CApiHooker
{
public:
	typedef HWND (WINAPI *GetActiveWindowPtr)();

public:
	bool Patch(GetActiveWindowPtr pfnHookProc){ return PatchFunction("user32.dll", "GetActiveWindow", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("user32.dll", "GetActiveWindow"); }

	HWND operator()()
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		GetActiveWindowPtr pfn = (GetActiveWindowPtr)(void*)m_abTrampBuff;
		return pfn();
	}
};


/**
 * \brief 
 */
class CCreateMutexTramp
	: public CApiHooker
{
public:
	typedef HANDLE (WINAPI *CreateMutexPtr)(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPSTR lpName);

public:
	bool Patch(CreateMutexPtr pfnHookProc){ return PatchFunction("kernel32.dll", "CreateMutexA", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("kernel32.dll", "CreateMutexA"); }

	HANDLE operator()(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPSTR lpName)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		CreateMutexPtr pfn = (CreateMutexPtr)(void*)m_abTrampBuff;
		return pfn(lpMutexAttributes, bInitialOwner, lpName);
	}
};



/**
 * \brief 
 */
class CWriteProcessMemoryTramp
	: public CApiHooker
{
public:
	typedef BOOL (WINAPI *WriteProcessMemoryPtr)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);

public:
	bool Patch(WriteProcessMemoryPtr pfnHookProc){ return PatchFunction("kernel32.dll", "WriteProcessMemory", PtrToUlong(pfnHookProc)); }
	bool UnPatch(){ return UnPatchFunction("kernel32.dll", "WriteProcessMemory"); }

	BOOL operator()(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten)
	{
		if (m_abTrampBuff[0] == 0)
			return 0;

		WriteProcessMemoryPtr pfn = (WriteProcessMemoryPtr)(void*)m_abTrampBuff;
		return pfn(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
	}
};


#endif //__ApiHooker_H

