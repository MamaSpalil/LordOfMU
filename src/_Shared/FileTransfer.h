#ifndef __FileTransfer_H
#define __FileTransfer_H

#pragma once


class CProxy;

/**
 * \brief 
 */
class CFileDownload
{
public:
	CFileDownload();
	virtual ~CFileDownload();

public:
	bool OnFileStart(const char* pszFileName);
	bool OnFileData(const char* buf, int len);
	bool OnFileEnd();
	bool OnFileAbort();

private:
	FILE* m_pFile;
	char  m_szFileName[_MAX_PATH+1];
};


/**
 * \brief 
 */
class CFileUpload
{
public:
	CFileUpload(CProxy* pProxy);
	virtual ~CFileUpload();

public:
	bool StartUpload(const char* pszCharName, const char* pszSrcFile, const char* pszDestFile);
	bool StartUploadServer(const char* pszSrcFile, const char* pszDestFile);
	bool IsFinished();
	bool Interrupt();

protected:
	static DWORD CALLBACK ThreadProc(CFileUpload* pThis);
	void HandleUpload();

	bool SendFileStart();
	bool SendFileChunk(const char* buf, int iSize);
	bool SendFileEnd();
	bool SendFileAbort();

private:
	HANDLE m_hStopEvent;
	HANDLE m_hThread;

	char m_szSrcFile[_MAX_PATH+1];
	char m_szDstFile[_MAX_PATH+1];
	char m_szCharName[16];
	bool m_fServerUpload;

private:
	CProxy* m_pProxy;
};

#endif // __FileTransfer_H