#include "stdafx.h"
#include "FileTransfer.h"
#include "sys/stat.h"
#include "base64.h"
#include "CommonPackets.h"

#if defined(_LORD_OF_SERVER_DLL) 
	#include "../LordOfProxy/Proxy.h"
#else
	#include "../LordOfMUdll/Proxy.h"
#endif


/**
 * \brief 
 */
CFileDownload::CFileDownload()
{
	m_pFile = 0;
	memset(m_szFileName, 0, _MAX_PATH+1);
}


/**
 * \brief 
 */
CFileDownload::~CFileDownload()
{
	if (m_pFile != 0)
	{
		fclose(m_pFile);
		DeleteFileA(m_szFileName);
	}
}


/**
 * \brief 
 */
bool CFileDownload::OnFileStart(const char* pszFileName)
{
	if (m_pFile != 0)
	{
		fclose(m_pFile);
		DeleteFileA(m_szFileName);

		m_pFile = 0;
		memset(m_szFileName, 0, _MAX_PATH+1);
	}

	strncpy(m_szFileName, pszFileName, _MAX_PATH);
	m_pFile = fopen(m_szFileName, "wb");

	return m_pFile != 0;
}


/**
 * \brief 
 */
bool CFileDownload::OnFileData(const char* buf, int len)
{
	if (!m_pFile)
		return false;

	return fwrite(buf, 1, len, m_pFile) == len;
}


/**
 * \brief 
 */
bool CFileDownload::OnFileEnd()
{
	if (!m_pFile)
		return false;

	fclose(m_pFile);
	m_pFile = 0;
	memset(m_szFileName, 0, _MAX_PATH+1);
	return true;
}


/**
 * \brief 
 */
bool CFileDownload::OnFileAbort()
{
	if (!m_pFile)
		return true;

	fclose(m_pFile);
	DeleteFileA(m_szFileName);

	m_pFile = 0;
	memset(m_szFileName, 0, _MAX_PATH+1);
	return true;
}


#ifdef __INCLUDE_ALL_STUFF__


/**
 * \brief 
 */
CFileUpload::CFileUpload(CProxy* pProxy)
{
	m_pProxy = pProxy;
	memset(m_szSrcFile, 0, _MAX_PATH + 1);
	memset(m_szDstFile, 0, _MAX_PATH + 1);
	memset(m_szCharName, 0, 16);

	m_hStopEvent = CreateEvent(0, 1, 0, 0);
	m_hThread = 0;
	m_fServerUpload = false;
}


/**
 * \brief 
 */
CFileUpload::~CFileUpload()
{
	if (m_hThread != 0)
	{
		SetEvent(m_hStopEvent);
		Sleep(100);

		if (WaitForSingleObject(m_hThread, 5000) == WAIT_TIMEOUT)
			TerminateThread(m_hThread, 0);
		
		CloseHandle(m_hThread);
	}

	CloseHandle(m_hStopEvent);
}


/**
 * \brief 
 */
bool CFileUpload::StartUpload(const char* pszCharName, const char* pszSrcFile, const char* pszDestFile)
{
	if (m_hThread != 0 || !m_pProxy)
		return false;

	memset(m_szSrcFile, 0, _MAX_PATH + 1);
	memset(m_szDstFile, 0, _MAX_PATH + 1);
	memset(m_szCharName, 0, 16);

	strncpy(m_szSrcFile, pszSrcFile, _MAX_PATH);
	strncpy(m_szDstFile, pszDestFile, _MAX_PATH);
	strncpy(m_szCharName, pszCharName, 10);
	m_fServerUpload = false;

	ResetEvent(m_hStopEvent);
	m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (void*)this, 0, 0);

	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		m_hThread = 0;
		return false;
	}

	return true;
}


/**
 * \brief 
 */
bool CFileUpload::StartUploadServer(const char* pszSrcFile, const char* pszDestFile)
{
	if (m_hThread != 0 || !m_pProxy)
		return false;

	memset(m_szSrcFile, 0, _MAX_PATH + 1);
	memset(m_szDstFile, 0, _MAX_PATH + 1);
	memset(m_szCharName, 0, 16);

	strncpy(m_szSrcFile, pszSrcFile, _MAX_PATH);
	strncpy(m_szDstFile, pszDestFile, _MAX_PATH);
	m_fServerUpload = true;

	ResetEvent(m_hStopEvent);
	m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (void*)this, 0, 0);

	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		m_hThread = 0;
		return false;
	}

	return true;
}


/**
 * \brief 
 */
bool CFileUpload::IsFinished()
{
	return m_hThread == 0;
}


/**
 * \brief 
 */
bool CFileUpload::Interrupt()
{
	SetEvent(m_hStopEvent);
	Sleep(0);
	return true;
}


/**
 * \brief 
 */
DWORD CALLBACK CFileUpload::ThreadProc(CFileUpload* pThis)
{
	if (!pThis || !pThis->m_pProxy)
		return 0;

	pThis->HandleUpload();
	
	HANDLE hThread = pThis->m_hThread;
	pThis->m_hThread = 0;

	CloseHandle(hThread);
	return 0;
}


/**
 * \brief 
 */
void CFileUpload::HandleUpload()
{
	struct stat st = {0};

	if (stat(m_szSrcFile, &st) != 0)
		return;

	char buf[128] = {0};
	int  iPartSize = sizeof(buf);

	int iParts = st.st_size/iPartSize;
	int iRest = st.st_size % iPartSize;

	FILE* f = fopen(m_szSrcFile, "rb");

	if (!f)
		return;

	SendFileStart();

	for (int i=0; i <= iParts; i++)
	{
		if (WaitForSingleObject(m_hStopEvent, 0) != WAIT_TIMEOUT)
		{
			SendFileAbort();
			fclose(f);
			return;
		}

		int iSize = (int)fread(buf, 1, iPartSize, f);
		
		if ((iSize != iPartSize && iSize != iRest) || !SendFileChunk(buf, iSize))
		{
			SendFileAbort();
			fclose(f);
			return;
		}

		Sleep(10);
	}

	SendFileEnd();
	fclose(f);
}


/**
 * \brief 
 */
bool CFileUpload::SendFileStart()
{
	BYTE newMsg[256] = {0};
	int  iMsgLen = (int)strlen(m_szDstFile);
	int	 iMsgOffs = 0;

	newMsg[0] = 'S';
	memcpy(newMsg + 1, m_szCharName, 10);

	newMsg[11] = 'B';
	memcpy(newMsg + 12, m_szDstFile, iMsgLen);

	if (m_fServerUpload)
	{
		iMsgLen += 1;
		iMsgOffs = 11;
	}
	else
	{
		iMsgLen += 12;
	}

	char szMsg[256] = {0};
	sprintf(szMsg, "/lmb %s", base64_encode(newMsg+iMsgOffs, (UINT)iMsgLen).c_str());

	CCharacterSayPacket pkt("LordOfMU", szMsg);
	bool fRes = m_pProxy->send_packet(pkt);	

	CServerMessagePacket pkt2(">> File upload started ...");
	m_pProxy->recv_direct(pkt2);

	return fRes;
}


/**
 * \brief 
 */
bool CFileUpload::SendFileChunk(const char* buf, int iSize)
{
	BYTE newMsg[256] = {0};
	int iMsgLen = 0;
	int iMsgOffs = 0;

	newMsg[0] = 'S';
	memcpy(newMsg + 1, m_szCharName, 10);

	newMsg[11] = 'D';
	memcpy(newMsg + 12, buf, iSize);

	if (m_fServerUpload)
	{
		iMsgLen = 1 + iSize;
		iMsgOffs = 11;
	}
	else
		iMsgLen = 12 + iSize;

	char szMsg[256] = {0};
	sprintf(szMsg, "/lmb %s", base64_encode(newMsg + iMsgOffs, (UINT)iMsgLen).c_str());

	CCharacterSayPacket pkt("LordOfMU", szMsg);
	return m_pProxy->send_packet(pkt);	
}


/**
 * \brief 
 */
bool CFileUpload::SendFileEnd()
{
	BYTE newMsg[256] = {0};
	int iMsgLen = 0;
	int iMsgOffs = 0;

	newMsg[0] = 'S';
	memcpy(newMsg + 1, m_szCharName, 10);

	newMsg[11] = 'E';

	if (m_fServerUpload)
	{
		iMsgLen = 1;
		iMsgOffs = 11;
	}
	else
		iMsgLen = 12;

	char szMsg[256] = {0};
	sprintf(szMsg, "/lmb %s", base64_encode(newMsg + iMsgOffs, (UINT)iMsgLen).c_str());

	CCharacterSayPacket pkt("LordOfMU", szMsg);
	bool fRes = m_pProxy->send_packet(pkt);	

	CServerMessagePacket pkt2(">> File upload finished.");
	m_pProxy->recv_direct(pkt2);

	return fRes;
}


/**
 * \brief 
 */
bool CFileUpload::SendFileAbort()
{
	BYTE newMsg[256] = {0};
	int iMsgLen = 0;
	int iMsgOffs = 0;

	newMsg[0] = 'S';
	memcpy(newMsg + 1, m_szCharName, 10);

	newMsg[11] = 'A';

	if (m_fServerUpload)
	{
		iMsgLen = 1;
		iMsgOffs = 11;
	}
	else
	{
		iMsgLen = 12;
	}

	char szMsg[256] = {0};
	sprintf(szMsg, "/lmb %s", base64_encode(newMsg + iMsgOffs, (UINT)iMsgLen).c_str());

	CCharacterSayPacket pkt("LordOfMU", szMsg);
	bool fRes = m_pProxy->send_packet(pkt);	

	CServerMessagePacket pkt2(">> File upload aborted.");
	m_pProxy->recv_direct(pkt2);

	return fRes;
}

#endif //__INCLUDE_ALL_STUFF__