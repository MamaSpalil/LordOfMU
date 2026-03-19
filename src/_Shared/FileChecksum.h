#ifndef __FileChecksum_H
#define __FileChecksum_H

#pragma once

#include "MD5.h"
#include "SYS/stat.h"

/**
 * \brief 
 */
class CFileChecksum
{
public:
	CFileChecksum()
	{
		memset(m_szFile, 0, sizeof(m_szFile));
		memset(m_szMD5, 0, sizeof(m_szMD5));
	}

	CFileChecksum(const char* pszFilePath, const char* pszMD5 = 0)
	{
		memset(m_szFile, 0, sizeof(m_szFile));
		memset(m_szMD5, 0, sizeof(m_szMD5));

		if (pszFilePath)
			strncpy(m_szFile, pszFilePath, _MAX_PATH);

		if (pszMD5)
			strncpy(m_szMD5, pszMD5, 127);
	}

	CFileChecksum(const CFileChecksum& in){ *this = in; }
	
	virtual ~CFileChecksum(){}

	CFileChecksum& operator=(const CFileChecksum& in)
	{
		memset(m_szFile, 0, sizeof(m_szFile));
		memset(m_szMD5, 0, sizeof(m_szMD5));

		strcpy(m_szFile, in.m_szFile);
		strcpy(m_szMD5, in.m_szMD5);
		return *this;
	}

	bool Calculate()
	{
		struct _stat st = {0};
		
		if (_tstat(CA2CT(m_szFile), &st) != 0)
			return false;


		FILE* f = fopen(m_szFile, "rb");

		if (!f)
			return false;


		MD5 md5;

		BYTE buff[4096] = {0};
		size_t ulRead = 0;

		while ((ulRead = fread(buff, 1, 4096, f)) != 0)
			md5.update(buff, (MD5::size_type)ulRead);

		md5.finalize();

		memset(m_szMD5, 0, sizeof(m_szMD5));
		strncpy(m_szMD5, md5.hexdigest().c_str(), 127);

		fclose(f);
		return true;
	}

	const char* GetFilename() const { return m_szFile; }
	const char* GetMD5() const { return m_szMD5; }

private:
	char m_szFile[_MAX_PATH+1];
	char m_szMD5[128];
};


#endif //__FileChecksum_H