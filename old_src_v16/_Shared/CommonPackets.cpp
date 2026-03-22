#include "stdafx.h"
#include "EncDec.h"
#include "CommonPackets.h"
#include "AutoBuffer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**
 * \brief 
 */
CGMMessagePacket::CGMMessagePacket(const char* format, ...)
{    
	va_list args;
	va_start(args, format);

	int len = _vscprintf(format, args) + 1; // _vscprintf doesn't count terminating '\0'
	
	CCharAutoBuffer szMessage(len);
	vsprintf(szMessage, format, args);

	int iMsgLen = (int)strlen(szMessage);

	if (iMsgLen <= 0)
		return;

	int dataLen = 11 + iMsgLen + 4;
	CAutoBuffer buff_msg(dataLen);
	memset(buff_msg, 0, dataLen);

	buff_msg[0] = 0x0D;
	buff_msg[1] = 0x00;
	memcpy(buff_msg+11, szMessage, iMsgLen);

	CPacket::operator=(CPacket(0xC1, dataLen, buff_msg));
	SetInjected();
}


/**
 * \brief 
 */
CServerMessagePacket::CServerMessagePacket(const char* format, ...)
{   
	memset(m_szMessage, 0, sizeof(m_szMessage));

	va_list args;
	va_start(args, format);

	int len = _vscprintf(format, args) + 1; // _vscprintf doesn't count terminating '\0'
	
	CCharAutoBuffer szMessage(len);
	vsprintf(szMessage, format, args);

	int iMsgLen = (int)strlen(szMessage);

	if (iMsgLen <= 0 || iMsgLen > 238)
		return;

	strncpy(m_szMessage, szMessage, 255);

	int dataLen = 11 + iMsgLen + 4;
	CAutoBuffer buff_msg(dataLen);
	memset(buff_msg, 0, dataLen);

	buff_msg[0] = 0x0D;
	buff_msg[1] = 0x01;
	memcpy(buff_msg+11, szMessage, iMsgLen);

	CPacket::operator=(CPacket(0xC1, dataLen, buff_msg));
	SetInjected();
}


/**
 * \brief 
 */
CServerMessagePacket::CServerMessagePacket(CPacket& in)
{
	memset(m_szMessage, 0, sizeof(m_szMessage));

	BYTE* buf = in.GetDecryptedPacket();
	int nLen = in.GetDecryptedLen();

	if (!buf || nLen < 0)
	{
		buf = in.GetRawPacket();
		nLen = in.GetPktLen();
	}

	if (buf && nLen > 11)
	{
		memcpy(m_szMessage, (char*)buf + 13, nLen - 13);
	}

	CPacket::operator=(in);
}



/**
 * \brief 
 */
CCharacterSayPacket::CCharacterSayPacket(const char* pszChar, const char* szMessage)
{
	memset(m_szMessage, 0, sizeof(m_szMessage));
	memset(m_szCharName, 0, sizeof(m_szCharName));

	const char* szCharName = (pszChar != 0) ? pszChar : "LordOfMU";

	if (szMessage != 0 && szMessage[0] != 0)
	{
		strncpy(m_szMessage, szMessage, 255);
		strncpy(m_szCharName, szCharName, 15);

		CPacket pkt;

		int nLen = (BYTE)(3 + 10 + (int)strlen(szMessage) + 1);
		CAutoBuffer buf(nLen);
		memset(buf, 0, nLen);

		buf[0] = 0xC1;
		buf[1] = (BYTE)nLen;
		buf[2] = 0x00;

		strncpy((char*)(BYTE*)buf + 3, szCharName, 10);
		
		if (nLen - 14 > 0)
			strncpy((char*)(BYTE*)buf + 13, szMessage, nLen - 14);

		SetDecryptedPacket(buf, nLen);
		SetInjected();
	}
}


/**
 * \brief 
 */
CCharacterSayPacket::CCharacterSayPacket(CPacket& in)
{
	memset(m_szMessage, 0, sizeof(m_szMessage));
	memset(m_szCharName, 0, sizeof(m_szCharName));

	BYTE* buf = in.GetDecryptedPacket();
	int nLen = in.GetDecryptedLen();

	if (!buf || nLen < 0)
	{
		buf = in.GetRawPacket();
		nLen = in.GetPktLen();
	}

	if (buf && nLen > 13)
	{
		memcpy(m_szCharName, (char*)buf + 3, 10);
		memcpy(m_szMessage, (char*)buf + 13, nLen - 13);
	}

	CPacket::operator=(in);
}


/**
 * \brief 
 */
CCharacterWhisperPacket::CCharacterWhisperPacket(const char* szCharName, const char* szMessage)
{
	memset(m_szMessage, 0, sizeof(m_szMessage));
	memset(m_szCharName, 0, sizeof(m_szCharName));

	if (szCharName != 0 && szMessage != 0 && szMessage[0] != 0)
	{
		strncpy(m_szMessage, szMessage, 255);
		strncpy(m_szCharName, szCharName, 15);

		CPacket pkt;

		int nLen = (BYTE)(3 + 10 + (int)strlen(szMessage) + 1);
		CAutoBuffer buf(nLen);
		memset(buf, 0, nLen);

		buf[0] = 0xC1;
		buf[1] = (BYTE)nLen;
		buf[2] = 0x02;

		strncpy((char*)(BYTE*)buf + 3, szCharName, 10);
		strncpy((char*)(BYTE*)buf + 13, szMessage, nLen - 14);

		SetDecryptedPacket(buf, nLen);
		SetInjected();
	}
}


/**
 * \brief 
 */
CCharacterWhisperPacket::CCharacterWhisperPacket(CPacket& in)
{
	memset(m_szMessage, 0, sizeof(m_szMessage));
	memset(m_szCharName, 0, sizeof(m_szCharName));

	BYTE* buf = in.GetDecryptedPacket();
	int nLen = in.GetDecryptedLen();

	if (!buf || nLen < 0)
	{
		buf = in.GetRawPacket();
		nLen = in.GetPktLen();
	}

	if (buf && nLen > 13)
	{
		memcpy(m_szCharName, (char*)buf + 3, 10);
		memcpy(m_szMessage, (char*)buf + 13, nLen - 13);
	}

	CPacket::operator=(in);
}


/**
 * \brief 
 */
int CMeetItemPacket::GetItemCount()
{
	BYTE* buf = GetDecryptedPacket();
	
	if (!buf)
		buf = GetRawPacket();

	if (!GetDecryptedPacket())
		return 0;

	return (int)(buf[4]);
}


/**
 * \brief 
 */
BYTE* CMeetItemPacket::GetItemData(int idx)
{
	BYTE* buf = AnyBuffer();

	if (!buf)
		return 0;

	int iCount = (int)buf[4];
	int iLen = (int)((WORD)buf[1] << 8 | buf[2]);

	if (idx >= iCount || iCount <= 0)
		return 0;

	int itemSize = (iLen - 5)/iCount;

	return buf + 5 + itemSize*idx;
}


/**
 * \brief 
 */
WORD CMeetItemPacket::GetItemType(int idx)
{
	BYTE* buf = GetItemData(idx);

	if (!buf)
		return 0;

	BYTE* pItemCode = buf + 4;

	return ((WORD)pItemCode[5] << 4) | ((WORD)(pItemCode[1] & 0x78) << 9) | pItemCode[0];
}

/**
 * \brief 
 */
WORD CMeetItemPacket::GetItemId(int idx)
{
	BYTE* buf = GetItemData(idx);

	if (!buf)
		return 0;

	return (WORD)(buf[0] & 0x7F) << 8 | buf[1];
}


/**
 * \brief 
 */
bool CMeetItemPacket::GetItemPos(int idx, BYTE& x, BYTE& y)
{
	BYTE* buf = GetItemData(idx);

	if (!buf)
		return false;

	x = buf[2];
	y = buf[3];
	return true;
}


/**
 * \brief 
 */
CPickItemPacket::CPickItemPacket(WORD wID)
{
	unsigned char pickBuf[10] = {0xC3, 0x07, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	DWORD dwTick = GetTickCount();
	pickBuf[6] = (BYTE)(dwTick & 0xFF);
	pickBuf[7] = (BYTE)((dwTick >> 8) & 0xFF);
	pickBuf[8] = (BYTE)((dwTick >> 16) & 0xFF);
	pickBuf[9] = (BYTE)((dwTick >> 24) & 0xFF);

	pickBuf[4] = (BYTE)((wID >> 8) & 0x007F);
	pickBuf[5] = (BYTE)(wID & 0x00FF);

	SetDecryptedPacket(pickBuf, 7);	
	SetInjected();
}


/**
 * \brief 
 */
int CCharListReplyPacket::GetCharCount()
{
	BYTE* buf = AnyBuffer();

	if (!buf)
		return 0;

	return (int)(BYTE)buf[6];
}


/**
 * \brief 
 */
CStringA CCharListReplyPacket::GetCharName(int idx)
{
	BYTE* buf = AnyBuffer();

	if (!buf || GetCharCount() <= idx)
		return CStringA("");

	int offs = 7 + idx*34;

	char szName[16] = {0};
	memcpy(szName, buf + offs + 1, 10);
	
	return CStringA(szName);
}


/**
 * \brief 
 */
WORD CCharListReplyPacket::GetCharLevel(int idx)
{
	BYTE* buf = AnyBuffer();

	if (!buf || GetCharCount() <= idx)
		return 0;

	int offs = 7 + idx*34;

	WORD wLvl = 0;
	memcpy(&wLvl, buf + offs + 12, 2);

	return wLvl;
}


/**
 * \brief 
 */
int CCharListReplyPacket::FindChar(const char* pszName)
{
	BYTE* buf = AnyBuffer();

	if (!buf || !pszName)
		return -1;

	for (int i = GetCharCount() - 1; i >= 0; --i)
	{
		int offs = 7 + i*34;

		char szName[16] = {0};
		memcpy(szName, buf + offs + 1, 10);

		if (_stricmp(szName, pszName) == 0)
			return i;
	}

	return -1;
}


/**
 * \brief 
 */
CStringA CCharSelectedPacket::GetCharName()
{
	BYTE* buf = AnyBuffer();

	if (!buf)
		return CStringA("");

	char szCharName[16] = {0};
	memcpy(szCharName, buf + 4, 10);

	return CStringA(szCharName);
}


/**
 * \brief 
 */
BYTE CObjectMovedPacket::GetX()
{
	BYTE* buf = AnyBuffer();
	return (buf && (buf[1] > 5)) ?  buf[5] : 0;
}


/**
 * \brief 
 */
BYTE CObjectMovedPacket::GetY()
{
	BYTE* buf = AnyBuffer();
	return (buf && (buf[1] > 6)) ?  buf[6] : 0;
}


/**
 * \brief 
 */
BYTE CObjectMovedPacket::GetX0()
{
	BYTE* buf = AnyBuffer();
	return buf ?  buf[3] : 0;
}


/**
 * \brief 
 */
BYTE CObjectMovedPacket::GetY0()
{
	BYTE* buf = AnyBuffer();
	return buf ?  buf[4] : 0;
}


/**
 * \brief 
 */
WORD CObjectMovedPacket::GetId()
{
	BYTE* pPacket = AnyBuffer();
	return (pPacket && pPacket[1] > 4) ? ((WORD)(unsigned char)pPacket[3] << 8 | (unsigned char)pPacket[4]) : 0;
}


/**
 * \brief 
 */
CUpdatePosSTCPacket::CUpdatePosSTCPacket(WORD wId, BYTE x, BYTE y)
{
	SetType(Type());

	BYTE buf[7] = {0xC1, 0x07, 0x15, HIBYTE(wId), LOBYTE(wId), x, y};
	
	const BYTE* pat = Type().GetPattern();

	if (pat && pat[0] >= 2)
		buf[2] = pat[3];

	SetDecryptedPacket(buf, 7);
	SetInjected();
}


/**
 * \brief 
 */
CUpdatePosCTSPacket::CUpdatePosCTSPacket(BYTE x, BYTE y)
{
	SetType(Type());

	BYTE buf[7] = {0xC1, 0x07, 0xD4, x, y, 0, 0};
	int len = 7;

	if ((CPacketType::GetVersion() & 0xFF) == PTYPE_KOR)
	{
		len = 6;
		buf[1] = 0x06;
		buf[2] = 0xD3;
	}
	else if ((CPacketType::GetVersion() & 0xFF) == PTYPE_JPN)
	{
		len = 6;
		buf[1] = 0x06;
		buf[2] = 0x1D;
	}

	SetDecryptedPacket(buf, len);
	SetInjected();
}


/**  
 * \brief 
 */
CDropItemPacket::CDropItemPacket(BYTE x, BYTE y, BYTE pos)
{
	unsigned char pPacket[] = {0xC3, 0x08, 0x00, 0x23, x, y, pos, 0x5E};
									// xor, hdr=3,     ^  ^   ^    ^------ some junk
									//                 X  Y  Inventory Pos

	SetDecryptedPacket(pPacket, sizeof(pPacket));
	SetInjected();
}


/**
 * \brief 
 */
int CMeetPlayerPacket::CalcExtraBytes(BYTE* pPacket, int idx, int iSize, int* pExtBytes, bool fAdvance)
{
	int extraBytes = 0;

	if (!pExtBytes)
	{
		for (int i=0; i < idx; ++i)
			extraBytes += GetPlayerExtraBytes(pPacket, i, iSize, extraBytes);
	}
	else if (idx == 0)
	{
		*pExtBytes = GetPlayerExtraBytes(pPacket, 0, iSize, 0);
	}
	else
	{
		extraBytes = *pExtBytes;

		if (fAdvance)
			*pExtBytes += GetPlayerExtraBytes(pPacket, idx, iSize, extraBytes);
	}

	return extraBytes;
}


/**
 * \brief 
 */
int CMeetPlayerPacket::GetPlayerExtraBytes(BYTE* pPacket, int idx, int iSize, int prevExtraBytes)
{
	return (int)pPacket[5 + (idx+1)*iSize - 1 + prevExtraBytes];
}


/**
 * \brief 
 */
CStringA CMeetPlayerPacket::GetPlayerName(int idx, int* pExtBytes, bool fAdvance)
{
	BYTE* pPacket = AnyBuffer();
	int iCount = GetPlayerCount();
	int iLen = (int)(((WORD)pPacket[1] << 8) | pPacket[2]);

	if (!pPacket || idx < 0 || idx >= iCount || iLen < 5)
		return CStringA("");

	char szName[16] = {0};
	int iItemSize = (iLen - 5)/iCount;

	if (iItemSize < 44)
	{
		int extraBytes = CalcExtraBytes(pPacket, idx, 36, pExtBytes, fAdvance);
		memcpy(szName, pPacket + 5 + idx*36 + 22 + extraBytes, 10);
	}
	else
	{
		int extraBytes = CalcExtraBytes(pPacket, idx, 44, pExtBytes, fAdvance);
		memcpy(szName, pPacket + 5 + idx*44 + 28 + extraBytes, 10);
	}

	return CStringA(szName);
}


/**
 * \brief 
 */
bool CMeetPlayerPacket::SkipPlayer(int idx)
{
	BYTE* pPacket = AnyBuffer();
	int iCount = GetPlayerCount();
	int iLen = (int)((WORD)pPacket[1] << 8 | pPacket[2]);

	if (!pPacket || idx < 0 || idx >= iCount || iCount <= 0 || iLen < 5)
		return false;

	int iItemSize = ((iLen - 5)/iCount < 44) ? 36 : 44;

	int extraBytes = CalcExtraBytes(pPacket, idx, iItemSize, 0);
	int extraBytesP = GetPlayerExtraBytes(pPacket, idx, iItemSize, extraBytes);

	if (idx != iCount-1)
	{
		int iDstOffs = 5 + idx*iItemSize + extraBytes;
		int iSrcOffs = 5 + (idx+1)*iItemSize + extraBytes + extraBytesP;

		CAutoBuffer buffTemp(iLen);
		memcpy(buffTemp, pPacket + iSrcOffs, iLen - iSrcOffs);
		memcpy(pPacket + iDstOffs, buffTemp, iLen - iSrcOffs);
	}

	pPacket[4] = (BYTE)(iCount-1);

	iLen = iLen - (iItemSize + extraBytesP);

	pPacket[1] = HIBYTE(iLen);
	pPacket[2] = LOBYTE(iLen);
	SetDecryptedLen(iLen);

	return true;
}


/**
 * \brief 
 */
WORD CMeetPlayerPacket::GetPlayerId(int idx, int* pExtBytes, bool fAdvance)
{
	BYTE* pPacket = AnyBuffer();
	int iCount = GetPlayerCount();
	int iLen = (int)((WORD)pPacket[1] << 8 | pPacket[2]);

	if (!pPacket || idx < 0 || idx >= iCount || iLen < 5)
		return 0;

	int iItemSize = ((iLen - 5)/iCount < 44) ? 36 : 44;
	int offs = 5 + idx*iItemSize + CalcExtraBytes(pPacket, idx, iItemSize, pExtBytes, fAdvance);

	return ((WORD)((BYTE)pPacket[offs] & 0x7F) << 8) | (BYTE)pPacket[offs+1];
}


/**
 * \brief 
 */
WORD CForgetObjectPacket::GetObjectId(int idx)
{
	BYTE* pPacket = AnyBuffer();

	if (!pPacket || idx < 0 || idx >= GetObjectCount())
		return 0;

	int offs = 4 + idx*2;
	return ((WORD)pPacket[offs] << 8) | (BYTE)pPacket[offs+1];
}


/**
 * \brief 
 */
WORD CForgetItemPacket::GetItemId(int idx)
{
	BYTE* pPacket = AnyBuffer();

	if (!pPacket || idx < 0 || idx >= GetItemCount())
		return 0;

	int offs = 5 + idx*2;
	return ((WORD)pPacket[offs] << 8) | (BYTE)pPacket[offs+1];
}


/**
 * \brief 
 */
CNormalAttackPacket::CNormalAttackPacket(WORD wId)
{
	if ((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S4)
	{
		BYTE buf[] = {0xC1, 0x0A, Type().GetPattern()[3], (BYTE)((wId >> 8) & 0x00FF), (BYTE)(wId & 0x00FF), 0x00, 0x33, 0x00, 0x00, 0x00};
		SetDecryptedPacket(buf, (int)buf[1]);
	}
	else
	{
		BYTE buf[] = {0xC1, 0x07, Type().GetPattern()[3], (BYTE)((wId >> 8) & 0x00FF), (BYTE)(wId & 0x00FF), 0x78, 0x06};
		SetDecryptedPacket(buf, (int)buf[1]);
	}
}


/**
 * \brief 
 */
WORD CMeetMonsterPacket::GetId(int idx)
{
	BYTE* pPacket = AnyBuffer();

	if (!pPacket || idx < 0 || idx >= GetCount())
		return 0;

	int iSize = ((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S3) ? 16 : 10;
	int offs = 5 + idx*iSize;

	return ((WORD)(pPacket[offs] & 0x7F) << 8) | (BYTE)pPacket[offs+1];
}


/**
 * \brief 
 */
bool CMeetMonsterPacket::GetPos(int idx, BYTE& x, BYTE& y, BYTE* x0, BYTE* y0)
{
	BYTE* pPacket = AnyBuffer();

	if (!pPacket || idx < 0 || idx >= GetCount())
		return false;

	int iSize = ((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S3) ? 16 : 10;
	int offs = 5 + idx*iSize 
				+ (((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S3) ? 8 : 4) ;

	x = pPacket[offs+2];
	y = pPacket[offs+3];

	if (x0)
		*x0 = pPacket[offs];

	if (y0)
		*y0 = pPacket[offs+1];

	return true;
}


/**
 * \brief 
 */
CSingleSkillPacket::CSingleSkillPacket(BYTE bSkill, WORD wTarget, int fInjected)
{
	if ((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S3)
	{
		unsigned char buff[] = {0xC3, 0x0A, 0x00, 0x19, bSkill, 
			(BYTE)((wTarget >> 8) & 0x00FF), (BYTE)(wTarget & 0x00FF), 0x00, 0x00, 0x23};

		SetDecryptedPacket(buff, sizeof(buff));	
	}
	else
	{
		DWORD dwTick = GetTickCount();

		unsigned char buff[] = {0xC3, 0x09, 0x00, 0x19, 0x00, bSkill, 
			(BYTE)((wTarget >> 8) & 0x00FF), (BYTE)(wTarget & 0x00FF), LOBYTE(dwTick)};

		SetDecryptedPacket(buff, sizeof(buff));	
	}

	SetInjected(fInjected);
}


/**  
 * \brief 
 */
WORD CSkillUsedPacket::GetTarget()
{ 
	WORD wRes = 0;
	BYTE* pPacket = AnyBuffer();

	if (!pPacket)
		return 0;

	if ((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S3)
	{
		wRes = MAKEWORD(pPacket[8], (pPacket[7] & 0x7F));
	}
	else
	{
		wRes = MAKEWORD(pPacket[9], (pPacket[8] & 0x7F));
	}

	return wRes;
}


/**  
 * \brief 
 */
WORD CSkillUsedPacket::GetAttacker()
{ 
	WORD wRes = 0;
	BYTE* pPacket = AnyBuffer();

	if (!pPacket)
		return 0;


	if ((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S3)
	{
		wRes = MAKEWORD(pPacket[6], (pPacket[5] & 0x7F));
	}
	else
	{
		wRes = MAKEWORD(pPacket[7], (pPacket[6] & 0x7F));
	}

	return wRes;
}



/**
 * \brief 
 */
CMassiveSkillPacket::CMassiveSkillPacket(BYTE bSkill, BYTE x, BYTE y, BYTE rot, WORD wTarget, int fInjected)
{
	DWORD dwTickCount = GetTickCount();

	if ((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S3)
	{
		// C3 12 07 1E 
		//  3E - skill
		//	D1 5F - x, y of character
		//	2C - rotation
		//	98 89 - don't know
		//	FF FF - mob ID
		//	00 - reserved
		//	05 - ??
		//  22 39 5D 8A - some incrementing number, maybe tick count
		unsigned char buff[] = {0xC3, 0x12, 0x00, 0x1E, bSkill, 
			(BYTE)x, (BYTE)y, 
			(BYTE)rot, 
			0x00, 0x00, 
			HIBYTE(wTarget), LOBYTE(wTarget), 
			0x00, 0x05,
			LOBYTE(LOWORD(dwTickCount)), HIBYTE(LOWORD(dwTickCount)), LOBYTE(HIWORD(dwTickCount)), HIBYTE(HIWORD(dwTickCount))};

		SetDecryptedPacket(buff, sizeof(buff));	
	}
	else
	{
		// C3 12 B6 1E 00 
		//  D6 - skill
		//  6A D7 - x,y of character
		//  DD - rotation
		//  00 00 - reserved
		//  07 7A - mob ID
		//  00  - reserved 
		//  40 10 5D 8A - some incrementing number, maybe tick count
		unsigned char buff[] = {0xC3, 0x12, 0x00, 0x1E, 0x00, 0xD6, 
			(BYTE)x, (BYTE)y, 
			(BYTE)rot, 
			0x00, 0x00, 
			HIBYTE(wTarget), LOBYTE(wTarget), 
			0x00, 
			LOBYTE(LOWORD(dwTickCount)), HIBYTE(LOWORD(dwTickCount)), LOBYTE(HIWORD(dwTickCount)), HIBYTE(HIWORD(dwTickCount))};

		SetDecryptedPacket(buff, sizeof(buff));	
	}

	SetInjected(fInjected);
}


/**  
 * \brief 
 */
WORD CMassiveSkillPacket::GetTarget()
{ 
	WORD wRes = 0;
	BYTE* pPacket = AnyBuffer();

	if (!pPacket)
		return 0;

	if ((CPacketType::GetVersion() & 0xFF000000) == GTYPE_S3)
	{
		wRes = MAKEWORD(pPacket[11], pPacket[10]);
	}
	else
	{
		wRes = MAKEWORD(pPacket[12], pPacket[11]);
	}

	return wRes;
}


/**
 * \brief 
 */
CObjectDeathPacket::CObjectDeathPacket(WORD wId)
{
	BYTE buf[] = {0xC1, 0x08, 0x17, HIBYTE(wId), LOBYTE(wId), 0x00, 0x00, 0x00};

	SetDecryptedPacket(buf, sizeof(buf));
	SetInjected();
}


/**
 * \brief 
 */
void CClientAuthPacket::GetUserName(char* pszUser)
{
	if (!pszUser)
		return;

	pszUser[0] = 0;

	BYTE* buf = AnyBuffer();
	if (!buf)
		return;

	memcpy(pszUser, buf + 5, 10);
	CEncDec::EncDecLogin((BYTE*)pszUser, 10); //Decrypt Login
}


/**
 * \brief 
 */
void CClientAuthPacket::GetPassword(char* pszPass)
{
	if (!pszPass)
		return;

	pszPass[0] = 0;

	BYTE* buf = AnyBuffer();
	if (!buf)
		return;

	memcpy(pszPass, buf + 5, 10);
	CEncDec::EncDecLogin((BYTE*)pszPass, 10); //Decrypt Login
}


/**  
 * \brief 
 */
BYTE CInitInventoryPacket::GetPosition(int idx)
{
	if (idx < 0 || idx >= GetCount())
		return 0;

	BYTE* pPacket = AnyBuffer();

	if (!pPacket)
		return 0;

	int offs = 7 + idx*13;
	return pPacket[offs];
}


/**  
 * \brief 
 */
BYTE* CInitInventoryPacket::GetItemData(int idx)
{
	if (idx < 0 || idx >= GetCount())
		return 0;

	BYTE* pPacket = AnyBuffer();

	if (!pPacket)
		return 0;

	int offs = 7 + idx*13 + 1;
	return pPacket + offs;
}


/**  
 * \brief 
 */
int CRemoveFromInventoryPacket::GetPosition(int idx)
{
	if (idx < 0 || idx >= GetCount())
		return 0;

	BYTE* pPacket = AnyBuffer();

	if (!pPacket)
		return 0;

	int offs = 4 + idx;
	return pPacket[offs];
}

