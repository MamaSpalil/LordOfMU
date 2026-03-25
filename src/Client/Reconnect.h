#pragma once

// ----------------------------------------------------------------------------------------------

#include "Protocol.h"
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct CONNECTEX_CLOSE
{
	PBMSG_HEAD2	h;
};
#pragma pack(pop)
namespace ConnectExType
{
	enum T
	{
		OnLine = 0,
		OnReconnect = 1,
		OnForceDisconnect = 2,
	};
};
enum eReconnectStatus
{
	RECONNECT_STATUS_NONE = 0,
	RECONNECT_STATUS_RECONNECT = 1,
	RECONNECT_STATUS_DISCONNECT = 2,
};
enum eReconnectProgress
{
	RECONNECT_PROGRESS_NONE = 0,
	RECONNECT_PROGRESS_CONNECTED = 1,
	RECONNECT_PROGRESS_JOINED = 2,
	RECONNECT_PROGRESS_CHAR_LIST = 3,
	RECONNECT_PROGRESS_CHAR_INFO = 4,
	RECONNECT_PROGRESS_CHAR_SEND = 5,
};
// ----------------------------------------------------------------------------------------------
void ReconnectOnCloseSocket();
class ConnectEx
{
public:
	void	Load();
	void	Run();
	// ----
	static int CreateConnect(char * IP, WORD Port);
	// ----
	void	ClearGame();
	void	RestoreGame();
	// ----
	void	DrawProgres();
//	void	LoginReq();
	void	Reconnect();
	void	Relogin();
	void    SetAccInformation(SDHP_RECONNECT_IDPASS * RecInfo);
	void	Disconnect();
	// ----
	int		m_ConnectState;
	char	m_AccountID[11];
	char	m_Password[11];
	char    m_Name[11];
	// ----
	char	m_CSIP[16];
	WORD	m_CSPort;
	char	m_GSIP[16];
	WORD	m_GSPort;
private:

	// ----
	DWORD	m_LastSendTick;
	DWORD	m_ReconnectTick;
	bool	m_WantMUHelper;


	bool   CheckPreviewCharSet;
	// ----
}; extern ConnectEx gConnectEx;

// ----------------------------------------------------------------------------------------------
