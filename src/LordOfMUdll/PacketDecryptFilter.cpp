#include "stdafx.h"
#include "PacketDecryptFilter.h"
#include "CommonPackets.h"
#include "EncDec.h"
#include "BufferUtil.h"
#include "DebugOut.h"
#include "MuWindowUtil.h"


#define WM_SET_GAME_VERSION	WM_APP + 509

/**
 * \brief 
 */
#define DETECT_PACKET_TYPE(type) \
	__if_not_exists (__f_type_declared) \
	{ \
		bool __f_type_declared = true; \
		if (false) \
		{ \
		} \
	} \
	else if (pkt == type) \
	{ \
		pkt.SetType(type); \
	}


/**
 * \brief 
 */
CPacketDecryptFilter::CPacketDecryptFilter(CProxy* pProxy) 
	: CPacketFilter(pProxy) 
{ 
	CRegKey key;
	key.Create(HKEY_CURRENT_USER, _T("Software"));
	key.Create(key.m_hKey, _T("LordJerec"));
	key.Create(key.m_hKey, _T("MUAutoClicker"));
	key.Create(key.m_hKey, _T("Config"));

	DWORD dwType = PTYPE_KOR;
	DWORD dwGType = 1;

#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__) || defined(DEBUG)
	key.QueryDWORDValue(_T("ProtocolType"), dwType);
	key.QueryDWORDValue(_T("ClientType"), dwGType); // 0 - N/S, 1 - S3, 2 - S4
#endif

	m_ulVersion = dwType | ((dwGType == 1) ? GTYPE_S3 : GTYPE_S4);
}


/**
 * \brief 
 */
int CPacketDecryptFilter::FilterRecvPacket(CPacket& pkt, CFilterContext& context)
{
	if (pkt.IsDecrypted())
		return 0;

	if (pkt.GetPktClass() == 0xC3 || pkt.GetPktClass() == 0xC4)
	{
#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__)
		CEncDec::DecryptC3asClient(pkt);
#else
		pkt.SetDecryptedPacket(pkt.GetRawPacket(), pkt.GetPktLen());
#endif
	}
	else
	{
		pkt.SetDecryptedPacket(pkt.GetRawPacket(), pkt.GetPktLen());
	}

	DetectRecvPacketType(pkt);

	int len = pkt.GetDecryptedLen();
	BYTE* buf = pkt.GetDecryptedPacket();
	const BYTE* xorP = pkt.GetType().GetXorParams();

	if (buf && xorP && (xorP[0] != 0 || xorP[1] != 0) && xorP[0] < len)
		CEncDec::DecXor32(buf + xorP[0], xorP[1], len - xorP[0]);

	PostDetectRecvPacketType(pkt);


	// C3 0F 07 30 09 F8 B9 84 0E 90 F8 DE 13 D8 AC
//	BYTE* pTemp = pkt.GetDecryptedPacket();
//	if (pTemp[0] == 0xC3 && pTemp[1] == 0x0F && pTemp[3] == 0x30 && pTemp[4] == 0x09)
//		return -1;

	return 0;
}


/**
 * \brief 
 */
int CPacketDecryptFilter::FilterSendPacket(CPacket& pkt, CFilterContext& context)
{
	if (pkt.IsDecrypted())
		return 0;

	if (pkt.GetPktClass() == 0xC3 || pkt.GetPktClass() == 0xC4)
	{
#if defined(__CLICKER_STUFF__) || defined(__HACK_STUFF__)
		CEncDec::DecryptC3asServer(pkt);
#else
		pkt.SetDecryptedPacket(pkt.GetRawPacket(), pkt.GetPktLen());
#endif
	}
	else
	{
		pkt.SetDecryptedPacket(pkt.GetRawPacket(), pkt.GetPktLen());
	}

	DetectSendPacketType(pkt);

	int len = pkt.GetDecryptedLen();
	BYTE* buf = pkt.GetDecryptedPacket();
	const BYTE* xorP = pkt.GetType().GetXorParams();

	if (buf && xorP && (xorP[0] != 0 || xorP[1] != 0) && xorP[0] < len)
		CEncDec::DecXor32(buf + xorP[0], xorP[1], len - xorP[0]);

	PostDetectSendPacketType(pkt);
	DetectProtocolVersion(pkt);

	return 0;
}


/**
 * \brief 
 */
void CPacketDecryptFilter::DetectRecvPacketType(CPacket& pkt)
{
	DETECT_PACKET_TYPE(CServerMessagePacket::Type())
	DETECT_PACKET_TYPE(CGMMessagePacket::Type())
	DETECT_PACKET_TYPE(CCharacterWhisperPacket::Type())
		
	DETECT_PACKET_TYPE(CPingReplyPacket::Type())
	DETECT_PACKET_TYPE(CGameServerHelloPacket::Type())

	DETECT_PACKET_TYPE(CLevelUpPacket::Type())
	DETECT_PACKET_TYPE(CMeetMonsterPacket::Type())
	DETECT_PACKET_TYPE(CMeetItemPacket::Type())
	DETECT_PACKET_TYPE(CWarpReplyPacket::Type())
	DETECT_PACKET_TYPE(CObjectMovedPacket::Type())
	DETECT_PACKET_TYPE(CHitInfoPacket::Type())
	DETECT_PACKET_TYPE(CCharRespawnPacket::Type())
	DETECT_PACKET_TYPE(CCharStatsPacket::Type())
	DETECT_PACKET_TYPE(CUpdatePosSTCPacket::Type())
	DETECT_PACKET_TYPE(CMeetPlayerPacket::Type())
	DETECT_PACKET_TYPE(CForgetObjectPacket::Type())
	DETECT_PACKET_TYPE(CObjectDeathPacket::Type())
	DETECT_PACKET_TYPE(CForgetItemPacket::Type())
	DETECT_PACKET_TYPE(CPutInventoryPacket::Type())
	DETECT_PACKET_TYPE(CPickItemResultFailPacket::Type())
	DETECT_PACKET_TYPE(CMoveToInventoryPacket::Type())
	DETECT_PACKET_TYPE(CInitInventoryPacket::Type())
	DETECT_PACKET_TYPE(CMoveToVaultPacket::Type())
	DETECT_PACKET_TYPE(CRemoveFromInventoryPacket::Type())
	DETECT_PACKET_TYPE(CVaultContentsPacket::Type())

	DETECT_PACKET_TYPE(CObjectAttackedPacket::Type())
	DETECT_PACKET_TYPE(CSkillUsedPacket::Type())
	DETECT_PACKET_TYPE(CMassiveSkillEffectPacket::Type())

	// Additional known packet types to avoid "Unknown packet" flood
	DETECT_PACKET_TYPE(CViewportSkillEffectPacket::Type())
	DETECT_PACKET_TYPE(CExtendedProtocolPacket::Type())
	DETECT_PACKET_TYPE(CShieldGaugePacket::Type())
	DETECT_PACKET_TYPE(CZenUpdatePacket::Type())
	DETECT_PACKET_TYPE(CMasterLevelPacket::Type())
	DETECT_PACKET_TYPE(CExtCharInfoPacket::Type())
	DETECT_PACKET_TYPE(CBuffEffectPacket::Type())
	DETECT_PACKET_TYPE(CCharAnimationPacket::Type())
	DETECT_PACKET_TYPE(CViewportInfoPacket::Type())
	DETECT_PACKET_TYPE(CGuildInfoPacket::Type())
	DETECT_PACKET_TYPE(CLiveClientCountPacket::Type())
	DETECT_PACKET_TYPE(CPartyHPPacket::Type())
	DETECT_PACKET_TYPE(CViewportObjectPacket::Type())
	DETECT_PACKET_TYPE(CMassiveSkillAreaPacket::Type())
	DETECT_PACKET_TYPE(CHitInfoJPNPacket::Type())
	DETECT_PACKET_TYPE(CAuthSubProtocolPacket::Type())
}


/**
 * \brief 
 */
void CPacketDecryptFilter::DetectSendPacketType(CPacket& pkt)
{
	DETECT_PACKET_TYPE(CCharacterSayPacket::Type())
	DETECT_PACKET_TYPE(CCharacterSayPacket2::Type())
	DETECT_PACKET_TYPE(CCharacterSayPacket3::Type())
	DETECT_PACKET_TYPE(CCharacterWhisperPacket::Type())
	DETECT_PACKET_TYPE(CClientPingPacket::Type())

	DETECT_PACKET_TYPE(CPickItemPacket::Type())
	DETECT_PACKET_TYPE(CCharCommandsPacket::Type())
	DETECT_PACKET_TYPE(CNormalAttackPacket::Type())
	DETECT_PACKET_TYPE(CSingleSkillPacket::Type())
	DETECT_PACKET_TYPE(CMassiveSkillPacket::Type())
	DETECT_PACKET_TYPE(CCharMoveCTSPacket::Type())
	DETECT_PACKET_TYPE(CUpdatePosCTSPacket::Type())
	DETECT_PACKET_TYPE(CClientAuthPacket::Type())
	DETECT_PACKET_TYPE(CDropItemPacket::Type())
	DETECT_PACKET_TYPE(CMoveItemReqPacket::Type())
	DETECT_PACKET_TYPE(CUseItemPacket::Type())

	// Additional known send packet types
	DETECT_PACKET_TYPE(CExtendedProtocolPacket::Type())
}

/**
 * \brief 
 */
void CPacketDecryptFilter::PostDetectRecvPacketType(CPacket& pkt)
{
	DETECT_PACKET_TYPE(CCharListReplyPacket::Type())
	DETECT_PACKET_TYPE(CCharSetupPacket::Type())
}


/**
 * \brief 
 */
void CPacketDecryptFilter::PostDetectSendPacketType(CPacket& pkt)
{
	DETECT_PACKET_TYPE(CCharSelectedPacket::Type())
	DETECT_PACKET_TYPE(CCharListReqPacket::Type())
}

/**
 * \brief 
 */
bool CPacketDecryptFilter::GetParam(const char* pszParam, void* pData)
{ 
	if (!pszParam || !pData)
		return false;

	if (_stricmp(pszParam, "version") == 0)
	{
		*((ULONG*)pData) = CPacketType::GetVersion();
		return true;
	}

	return false; 
}


/**
 * \brief 
 */
void CPacketDecryptFilter::DetectProtocolVersion(CPacket& pkt)
{
	if (pkt == CClientAuthPacket::Type())
	{
		ULONG ulGameVersion = ((m_ulVersion & GTYPE_S3) != GTYPE_S3) ? 105 : 104;
		CMuWindowUtil::PostMessageToMuWindow(WM_SET_GAME_VERSION, 0, (LPARAM)ulGameVersion);

		CPacketType::SetVersion(m_ulVersion);
		CPacketType::SetProtocol(m_ulVersion);
	}
}
