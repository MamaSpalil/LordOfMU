#ifndef __CommonPackets_H
#define __CommonPackets_H

#pragma once

#include "Packet.h"
#include "CommonPacketsMacros.h"

#include <map>


/**
 * \brief 
 */
class CGMMessagePacket
	: public CPacket
{
public:
	CGMMessagePacket(const char* szFormat, ...);

	BEGIN_COMMON_PACKET_DECL(CGMMessagePacket)
		PACKET_PATT4(0xC1, 0x00, 0x0D, 0x00)
		PACKET_MASK4(0xFF, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Server anouncement / GM message")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CServerMessagePacket
	: public CPacket
{
public:
	CServerMessagePacket(CPacket& in);
	CServerMessagePacket(const char* szFormat, ...);

	const char* GetMessage(){ return m_szMessage; }

	BEGIN_COMMON_PACKET_DECL_NOCCONSTR(CServerMessagePacket)
		PACKET_PATT4(0xC1, 0x00, 0x0D, 0x01)
		PACKET_MASK4(0xFF, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Server message")
	END_COMMON_PACKET_DECL()

private:
	char m_szMessage[256];
};


/**
 * \brief 
 */
class CCharacterSayPacket
	: public CPacket
{
public:
	CCharacterSayPacket(CPacket& in);
	CCharacterSayPacket(const char* szCharName, const char* szMessage);

	const char* GetMessage(){ return m_szMessage; }
	const char* GetCharName(){ return m_szCharName; }

	BEGIN_COMMON_PACKET_DECL_NOCCONSTR(CCharacterSayPacket)
		PACKET_PATT3(0xC1, 0x00, 0x00)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_XORP2(3, 3)
		PACKET_DESCR("Character speak")
	END_COMMON_PACKET_DECL()

private:
	char m_szMessage[256];
	char m_szCharName[16];
};


/**
 * \brief 
 */
class CCharacterSayPacket2
	: public CCharacterSayPacket
{
public:
	CCharacterSayPacket2(CPacket& in) : CCharacterSayPacket(in) {}

	BEGIN_COMMON_PACKET_DECL_NOCCONSTR(CCharacterSayPacket2)
		PACKET_PATT3(0xC1, 0x00, 0x45)
		PACKET_MASK3(0xFF, 0x00, 0xFC)
		PACKET_XORP2(3, 3)
		PACKET_DESCR("Character speak")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CCharacterSayPacket3
	: public CCharacterSayPacket
{
public:
	CCharacterSayPacket3(CPacket& in) : CCharacterSayPacket(in) {}

	BEGIN_COMMON_PACKET_DECL_NOCCONSTR(CCharacterSayPacket3)
		PACKET_PATT3(0xC1, 0x00, 0x65)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_XORP2(3, 3)
		PACKET_DESCR("Character speak")
	END_COMMON_PACKET_DECL()
};

/**
 * \brief 
 */
class CCharacterWhisperPacket
	: public CPacket
{
public:
	CCharacterWhisperPacket(CPacket& in);
	CCharacterWhisperPacket(const char* szCharName, const char* szMessage);

	const char* GetMessage(){ return m_szMessage; }
	const char* GetCharName(){ return m_szCharName; }

	BEGIN_COMMON_PACKET_DECL_NOCCONSTR(CCharacterWhisperPacket)
		PACKET_PATT3(0xC1, 0x00, 0x02)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_XORP2(3, 3)
		PACKET_DESCR("Character whisper")
	END_COMMON_PACKET_DECL()

private:
	char m_szMessage[256];
	char m_szCharName[16];
};


/**
 * \brief 
 */
class CClientAuthPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CClientAuthPacket)
		PACKET_PATT4(0xC3, 0x00, 0x00, 0xF1)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_XORP2(4, 2)
		PACKET_DESCR("Client login")
	END_COMMON_PACKET_DECL()

	void GetUserName(char* pszUser);
	void GetPassword(char* pszPass);
};


/**
 * \brief 
 */
class CClientAuthCSPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CClientAuthCSPacket)
		PACKET_PATT4(0xC3, 0x00, 0x00, 0xB1)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_XORP2(4, 3)
		PACKET_DESCR("Client login CS")
	END_COMMON_PACKET_DECL()

	void GetCharName(char* pszName){ BYTE* pPacket = AnyBuffer(); if (pPacket) memcpy(pszName, pPacket + 17, 10); }
};


/**
 * \brief Object moved server to client packet
 *
 * 0xC1 0x08 0xD3 X_FROM Y_FROM X_TO Y_TO FACE_DIR
 *
 */
class CObjectMovedPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CObjectMovedPacket)
		BEGIN_PACKET_PATT()
			PACKET_PATT_V3(PTYPE_KOR, 0xC1, 0x08, 0xD3)
			PACKET_PATT_V3(PTYPE_JPN, 0xC1, 0x08, 0x1D)			
			PACKET_PATT_V3(PTYPE_PHI, 0xC1, 0x08, 0xD4)
			PACKET_PATT_V3(		   0, 0xC1, 0x08, 0xD4)
		END_PACKET_PATT()
		PACKET_MASK3(0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Object moved")
	END_COMMON_PACKET_DECL()

	BYTE GetX();
	BYTE GetY();
	WORD GetId();

	BYTE GetX0();
	BYTE GetY0();
};


/**
 * \brief 
 */
class CObjectAttackedPacket
	: public CPacket
{
// Object attacks object
// C1 09 18 04 85 02 78 1C B0
//           ^ ^         ^ ^
//		   Attacker     Victim
public:
	BEGIN_COMMON_PACKET_DECL(CObjectAttackedPacket)
		PACKET_PATT3(0xC1, 0x09, 0x18)
		PACKET_MASK3(0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Object attacked")
	END_COMMON_PACKET_DECL()

	WORD GetTarget(){ BYTE* buff = AnyBuffer(); return !buff ? 0 : MAKEWORD(buff[8], (buff[7] & 0x7F)); }
	WORD GetAttacker(){ BYTE* buff = AnyBuffer(); return !buff ? 0 : MAKEWORD(buff[4], (buff[3] & 0x7F)); }
};


/**
 * \brief 
 */
class CHitInfoPacket
	: public CPacket
{
// hit display
// C1 0A 11 1C B0 00 01 00 00 00
//           ^ ^   ^  ^-------^ 
//         Victim  |   Damage
//               Hit Type (normal, reflect, ignore, critical)
public:
	BEGIN_COMMON_PACKET_DECL(CHitInfoPacket)
		BEGIN_PACKET_PATT()
			PACKET_PATT_V3(PTYPE_KOR, 0xC1, 0x0A, 0xD7)
			PACKET_PATT_V3(0        , 0xC1, 0x0A, 0x11)
		END_PACKET_PATT()
		PACKET_MASK3(0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Hit info")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CClientPingPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CClientPingPacket)
		PACKET_PATT4(0xC3, 0x11, 0x00, 0x0E)
		PACKET_MASK4(0xFF, 0xFF, 0x00, 0xFF)
		PACKET_XORP2(4, 4)
		PACKET_DESCR("Client Ping")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CPingReplyPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CPingReplyPacket)
		PACKET_PATT3(0xC1, 0x04, 0x0F)
		PACKET_MASK3(0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Ping Reply")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Meet monster packet
 *
 * meet monster (protocol version <= 104)
 * C2 00 15 13
 * 	  01 04 45 00 1A 00 00 00 00 93 70 92 71 60 00 00 00
 *    ^   ^ ^   ^-^   ^--------^  ^-^   ^-^   ^
 * Count   ID  Type   Some info  Meet At | Facing Dir.
 * of Monsters                       Stopped At
 * -------------------------------------------------------
 * meet monster (protocol version > 104)
 * C2 00 0f 13  
 *    01 0c 51 01 a5 78 bf 79 bf 40 00
 *    ^   ^ ^   ^-^   ^-^   ^-^   ^
 * Count   ID  Type  Meet At | Facing Dir.
 * of Monsters           Stopped At
 */
class CMeetMonsterPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CMeetMonsterPacket)
		PACKET_PATT4(0xC2, 0x00, 0x00, 0x13)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Meet monster(s)")
	END_COMMON_PACKET_DECL()

	int GetCount(){ BYTE* buf = AnyBuffer(); return buf ? buf[4] : 0; }
	WORD GetId(int idx);
	bool GetPos(int idx, BYTE& x, BYTE& y, BYTE* x0 = 0, BYTE* y0 = 0);
};


/**
 * \brief Meet item packet
 *
 * C2 00 1B 20 02 80 67 35 86 0D 0A A7 00 00 0D 00 80 68 35 88 20 08 60 00 00 B0 00
 *     ^ ^      ^  ^ ^   ^ ^  <---- item code --->| ^ ^   ^ ^  <----- item code -->
 *     Len      ^   ^    x,y position                ^     x,y position
 *              ^   Item ID                         Item ID
 *              number of items
 */
class CMeetItemPacket
	: public CPacket
{
public:
	int GetItemCount();
	BYTE* GetItemData(int idx);
	WORD GetItemType(int idx);
	WORD GetItemId(int idx);
	bool GetItemPos(int idx, BYTE& x, BYTE& y);

public:
	BEGIN_COMMON_PACKET_DECL(CMeetItemPacket)
		PACKET_PATT4(0xC2, 0x00, 0x00, 0x20)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Meet item(s)")
	END_COMMON_PACKET_DECL()
};


/**  
 * \brief 
 * C3 15 0C 22 0C 03 00 FF 00 00 E0 00 FF FF FF FF FF 00 FD FD FD 
 *              ^  ^----------------^   ---- Some junk here ----
 *              |      Item code
 *        Inventory Place
 */
class CPutInventoryPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CPutInventoryPacket)
		PACKET_PATT5(0xC3, 0x00, 0x00, 0x22, 0x00)
		PACKET_MASK5(0xFF, 0x00, 0x00, 0xFF, 0x80)
		PACKET_DESCR("Put inventory")
	END_COMMON_PACKET_DECL()

	WORD GetItemType()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : ((WORD)pPacket[10] << 4) | ((WORD)(pPacket[6] & 0x78) << 9) | pPacket[5];
	}

	BYTE GetInvPos()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket[4];
	}

	BYTE* GetItemData()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket + 5;
	}
};


/**  
 * \brief 
 * C3 16 1A 24 00 0C 22 13 4A 00 00 A0 00 FF FF FF FF FF 00 00 00 00 
 *                 ^  ^----------------^   ---- Some junk here ----
 *                 |      Item code
 *           Inventory Place
 */
class CMoveToInventoryPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CMoveToInventoryPacket)
		PACKET_PATT5(0xC3, 0x00, 0x00, 0x24, 0x00)
		PACKET_MASK5(0xFF, 0x00, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Move to inventory")
	END_COMMON_PACKET_DECL()

	WORD GetItemType()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : ((WORD)pPacket[11] << 4) | ((WORD)(pPacket[7] & 0x78) << 9) | pPacket[6];
	}

	BYTE GetInvPos()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket[5];
	}

	BYTE* GetItemData()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket + 6;
	}
};


/**  
 * \brief 
 * C3 16 1A 24 02 0C 22 13 4A 00 00 A0 00 FF FF FF FF FF 00 00 00 00 
 *                 ^  ^----------------^   ---- Some junk here ----
 *                 |      Item code
 *           Inventory Place
 */
class CMoveToVaultPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CMoveToVaultPacket)
		PACKET_PATT5(0xC3, 0x00, 0x00, 0x24, 0x02)
		PACKET_MASK5(0xFF, 0x00, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Move to vault")
	END_COMMON_PACKET_DECL()

	WORD GetItemType()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : ((WORD)pPacket[11] << 4) | ((WORD)(pPacket[7] & 0x78) << 9) | pPacket[6];
	}

	BYTE GetInvPos()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket[5];
	}
};


/**  
 * \brief 
 * C3 18 06 24 00 14 1C 20 78 00 00 E0 00 FF FF FF FF FF 02 17 7D 9A 7D 2B
 *              ^  ^  ^----------------^   ^-- junk --^   ^  ^  ^-- dont know --
 *              |  |      Item code                       | Inventory place
 *              | Inventory Place                       Destination: 0 - inventory, 2 - vault, 4 - store
 *             Source: 0 - inventory, 2 - vault 
 */
class CMoveItemReqPacket
	: public CPacket
{
public:
	CMoveItemReqPacket(WORD src, WORD dst, BYTE* pData)
	{
		BYTE buff[] = {0xC3, 0x18, 0x00, 0x24, HIBYTE(src), LOBYTE(src), pData[0], pData[1], pData[2], pData[3], pData[4], pData[5], pData[6], 
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, HIBYTE(dst), LOBYTE(dst), 0x7D, 0x9A, 0x7D, 0x2B};

		SetDecryptedPacket(buff, sizeof(buff));
		SetInjected();
	}

	static const BYTE PLACE_INVENTORY = 0;
	static const BYTE PLACE_VAULT = 2;
	static const BYTE PLACE_STORE = 4;

	BEGIN_COMMON_PACKET_DECL(CMoveItemReqPacket)
		PACKET_PATT4(0xC3, 0x00, 0x00, 0x24)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_XORP2(4, 3)
		PACKET_DESCR("Move item request")
	END_COMMON_PACKET_DECL()

	WORD GetItemType()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : ((WORD)pPacket[11] << 4) | ((WORD)(pPacket[7] & 0x78) << 9) | pPacket[6];
	}

	BYTE GetInvPos()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket[5];
	}

	BYTE GetSource()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket[4];
	}

	BYTE GetDest()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket[18];
	}

	BYTE GetDestPos()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket[19];
	}
};


/**  
 * \brief 
 *
 *   C3 08 10 23 39 0C B2 68
 *                
 */
class CDropItemPacket
	: public CPacket
{
public:
	CDropItemPacket(BYTE x, BYTE y, BYTE pos);

	BEGIN_COMMON_PACKET_DECL(CDropItemPacket)
		PACKET_PATT4(0xC3, 0x00, 0x00, 0x23)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Drop item")
		PACKET_XORP2(4, 3)
	END_COMMON_PACKET_DECL()

};


/**
 * \brief 
 */
class CGameServerHelloPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CGameServerHelloPacket)
		PACKET_PATT4(0xC1, 0x00, 0xF1, 0x00)
		PACKET_MASK4(0xFF, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Game server hello")
	END_COMMON_PACKET_DECL()

	WORD GetPlayerId()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket && (pPacket[1] > 6)) ? (((WORD)pPacket[5] << 8) | pPacket[6]) : 0;
	}
};


/**
 * \brief 
 */
class CCharCommandsPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharCommandsPacket)
		PACKET_PATT3(0xC1, 0x00, 0xF3)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_XORP2(3, 3)
		PACKET_DESCR("Character command")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CCharSelectedPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharSelectedPacket)
		PACKET_PATT4(0xC1, 0x00, 0xF3, 0x03)
		PACKET_MASK4(0xFF, 0x00, 0xFF, 0xFF)
		PACKET_XORP2(3, 3)
		PACKET_DESCR("Character selected")
	END_COMMON_PACKET_DECL()

	CStringA GetCharName();
};


/**
 * \brief 
 */
class CCharListReqPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharListReqPacket)
		PACKET_PATT4(0xC1, 0x04, 0xF3, 0x00)
		PACKET_MASK4(0xFF, 0xFF, 0xFF, 0xFF)
		PACKET_XORP2(3, 3)
		PACKET_DESCR("Character list request")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CCharSetupPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharSetupPacket)
		PACKET_PATT4(0xC1, 0x00, 0xF3, 0x30)
		PACKET_MASK4(0xFF, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Character setup")
	END_COMMON_PACKET_DECL()
};

/**
 * \brief 
 */
class CCharListReplyPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharListReplyPacket)
		PACKET_PATT4(0xC1, 0x00, 0xF3, 0x00)
		PACKET_MASK4(0xFF, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Character list reply")
	END_COMMON_PACKET_DECL()

	int GetCharCount();
	CStringA GetCharName(int idx);
	WORD GetCharLevel(int idx);
	int FindChar(const char* pszName);
};



/**
 * \brief 
 */
class CPickItemPacket
	: public CPacket
{
public:
	CPickItemPacket(WORD wId);

	BEGIN_COMMON_PACKET_DECL(CPickItemPacket)
		PACKET_PATT4(0xC3, 0x00, 0x00, 0x22)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_XORP2(4, 3)
		PACKET_DESCR("Pick item")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CWarpReplyPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CWarpReplyPacket)
		PACKET_PATT4(0xC3, 0x00, 0x00, 0x1C)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Warp reply")
	END_COMMON_PACKET_DECL()

	BYTE GetX(){ BYTE* pPacket = AnyBuffer(); return (pPacket && pPacket[1] > 6) ? pPacket[6] : 0; }
	BYTE GetY(){ BYTE* pPacket = AnyBuffer(); return (pPacket && pPacket[1] > 7) ? pPacket[7] : 0; }
	BYTE GetMap(){ BYTE* pPacket = AnyBuffer(); return (pPacket && pPacket[1] > 5) ? pPacket[5] : 0; }
};


/**
 * \brief 
 */
class CCharRespawnPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharRespawnPacket)
		PACKET_PATT5(0xC3, 0x00, 0x00, 0xF3, 0x04)
		PACKET_MASK5(0xFF, 0x00, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Character respawn")
	END_COMMON_PACKET_DECL()

	BYTE GetX(){ BYTE* pPacket = AnyBuffer(); return (pPacket && pPacket[1] > 5) ? pPacket[5] : 0; }
	BYTE GetY(){ BYTE* pPacket = AnyBuffer(); return (pPacket && pPacket[1] > 6) ? pPacket[6] : 0; }
};



/**
 * \brief 
 */
class CCharStatsPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharStatsPacket)
		PACKET_PATT5(0xC3, 0x00, 0x00, 0xF3, 0x03)
		PACKET_MASK5(0xFF, 0x00, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Character stats")
	END_COMMON_PACKET_DECL()

	BYTE GetX(){ BYTE* pPacket = AnyBuffer(); return (pPacket && pPacket[1] > 5) ? pPacket[5] : 0; }
	BYTE GetY(){ BYTE* pPacket = AnyBuffer(); return (pPacket && pPacket[1] > 6) ? pPacket[6] : 0; }
};



/**
 * \brief Update position server-to-client packet
 * Update Position STC ->>>> versions prior to 104
 * C1 07 15 1C 32 28 E4 
 *            ^ ^  ^  ^
 *            ^ ^  ^  Y position
 *             ^   X position
 *         Character ID
 *
 * Update Position STC ->>>> versions 104 and above
 * C1 07 DF 19 4B 30 84
 *           ^ ^  ^  ^
 *           ^ ^  ^  Y position
 *            ^   X position
 *        Character ID
 *
 */
class CUpdatePosSTCPacket
	: public CPacket
{
public:
	CUpdatePosSTCPacket(WORD wId, BYTE x, BYTE y);

	BEGIN_COMMON_PACKET_DECL(CUpdatePosSTCPacket)
		BEGIN_PACKET_PATT()
			PACKET_PATT_V3(PTYPE_KOR, 0xC1, 0x07, 0xDF)
			PACKET_PATT_V3(PTYPE_JPN, 0xC1, 0x07, 0xD6)
			PACKET_PATT_V3(PTYPE_PHI, 0xC1, 0x07, 0x15)
			PACKET_PATT_V3(		   0, 0xC1, 0x07, 0x15)
		END_PACKET_PATT()
		PACKET_MASK3(0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Update position STC")
	END_COMMON_PACKET_DECL()

	BYTE GetX(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[5] : 0; }
	BYTE GetY(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[6] : 0; }
	WORD GetId() { BYTE* pPacket = AnyBuffer(); return pPacket ? (WORD)((pPacket[3] << 8) | pPacket[4]) : 0; }
};


/**
 * \brief Update position client-to-server packet
 *
 * Object Moved CTS ->>>> versions prior to 104
 * C1 07 D4 30 84 00 00
 *           ^  ^  ^  ^
 *           ^  ^  ^  Y Direction offs
 *           ^  ^  X Direction offs
 *           ^  Y Position
 *           X position
 *
 * Update Position CTS ->>>> versions 104 and above
 * C1 06 D3 30 84 00
 *           ^  ^
 *           ^  Y Position
 *           X position
 */
class CUpdatePosCTSPacket
	: public CPacket
{
public:
	CUpdatePosCTSPacket(BYTE x, BYTE y);

	BEGIN_COMMON_PACKET_DECL(CUpdatePosCTSPacket)
		BEGIN_PACKET_PATT()
			PACKET_PATT_V3(PTYPE_KOR, 0xC1, 0x06, 0xD3)
			PACKET_PATT_V3(PTYPE_JPN, 0xC1, 0x06, 0x1D)
			PACKET_PATT_V3(PTYPE_PHI, 0xC1, 0x07, 0xD4)
			PACKET_PATT_V3(		   0, 0xC1, 0x07, 0xD4)
		END_PACKET_PATT()
		PACKET_MASK3(0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Update position CTS")
		PACKET_XORP2(3, 3)
	END_COMMON_PACKET_DECL()

	BYTE GetX(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[3] : 0; }
	BYTE GetY(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[4] : 0; }
};



/**
 * \brief 
 */
class CLevelUpPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CLevelUpPacket)
		PACKET_PATT4(0xC1, 0x00, 0xF3, 0x05)
		PACKET_MASK4(0xFF, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Character level-up")
	END_COMMON_PACKET_DECL()

	BYTE GetLevel(){ BYTE* pPacket = AnyBuffer(); return (WORD)(BYTE)pPacket[4] | ((WORD)pPacket[5] << 8); }
};



/**
 * \brief 
 */
class CSingleSkillPacket
	: public CPacket
{
public:
	CSingleSkillPacket(BYTE bSkill, WORD wTarget, int fInjected = 1);

	BEGIN_COMMON_PACKET_DECL(CSingleSkillPacket)
		BEGIN_PACKET_PATT()
			PACKET_PATT_V4(PTYPE_KOR, 0xC3, 0x00, 0x00, 0x19)
			PACKET_PATT_V4(PTYPE_JPN, 0xC3, 0x00, 0x00, 0x19)
			PACKET_PATT_V4(		   0, 0xC3, 0x00, 0x00, 0x19)
		END_PACKET_PATT()
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_XORP2(4, 3)
		PACKET_DESCR("Single target skill")
	END_COMMON_PACKET_DECL()
};



/**
 * \brief 
 *
 * C3 0B 12 19 00 3D 2F 11 AF 14 00
 *                 ^  ^-^   ^_^
 *               Skill |   Victim 
 *                   Attacker
 */
class CSkillUsedPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CSkillUsedPacket)
		BEGIN_PACKET_PATT()
			PACKET_PATT_V4(PTYPE_KOR, 0xC3, 0x00, 0x00, 0x19)
			PACKET_PATT_V4(PTYPE_JPN, 0xC3, 0x00, 0x00, 0x19)
			PACKET_PATT_V4(		   0, 0xC3, 0x00, 0x00, 0x19)
		END_PACKET_PATT()
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Skill attacked")
	END_COMMON_PACKET_DECL()

	WORD GetTarget();
	WORD GetAttacker();
};


/**
 * \brief 
 */
class CMassiveSkillPacket
	: public CPacket
{
public:
	CMassiveSkillPacket(BYTE bSkill, BYTE x, BYTE y, BYTE rot = 0, WORD wTarget = 0, int fInjected = 1);

	BEGIN_COMMON_PACKET_DECL(CMassiveSkillPacket)
		PACKET_PATT4(0xC3, 0x00, 0x00, 0x1E)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_XORP2(4, 3)
		PACKET_DESCR("Massive skill")
	END_COMMON_PACKET_DECL()

	WORD GetTarget();
};



/**
 * \brief 
 *
 * C1 10 BF 0A 00 D7 | 1E 2F | 03 00 | 20 2F | 20 2F | EF 07
 *                ^     ^_^     ^_^     ^_^     ^_^     ^_^  
 *                |   Attacker   |     Target1 Target2 Target2 
 *              Skill  (LSBF)  Targets Count (LSBF)
 *
 *  Note: Attacker ID, Targets Count and Target IDs are received with least significant byte first
 */
class CMassiveSkillEffectPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CMassiveSkillEffectPacket)
		PACKET_PATT3(0xC1, 0x00, 0xBF)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Massive skill effect")
	END_COMMON_PACKET_DECL()

//	WORD GetAttacker(int idx);
//	WORD GetTargetsCount(int idx);
//	WORD GetTarget(int idx);
};



/**
 * \brief 
 *
 * --> S4
 *
 */
class CNormalAttackPacket
	: public CPacket
{
public:
	CNormalAttackPacket(WORD wId);

	BEGIN_COMMON_PACKET_DECL(CNormalAttackPacket)
		BEGIN_PACKET_PATT()
			PACKET_PATT_V3(PTYPE_KOR, 0xC1, 0x00, 0xD7)
			PACKET_PATT_V3(PTYPE_JPN, 0xC1, 0x00, 0xDC)
			PACKET_PATT_V3(		   0, 0xC1, 0x00, 0x11)
		END_PACKET_PATT()
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Normal attack")
		PACKET_XORP2(3, 3)
	END_COMMON_PACKET_DECL()
};



/**
 * \brief Character move client-to-server packet
 *
 * Char Move CTS
 * C1 07 D4 30 84 00 00
 *           ^  ^  ^  ^
 *           ^  ^  ^  Y Direction offs
 *           ^  ^  X Direction offs
 *           ^  Y Position
 *           X position
 */
class CCharMoveCTSPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharMoveCTSPacket)
		BEGIN_PACKET_PATT()
			PACKET_PATT_V3(PTYPE_KOR, 0xC1, 0x07, 0xD3)
			PACKET_PATT_V3(PTYPE_JPN, 0xC1, 0x07, 0x1D)
			PACKET_PATT_V3(		   0, 0xC1, 0x07, 0xD4)
		END_PACKET_PATT()
		PACKET_MASK3(0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Char Move CTS")
		PACKET_XORP2(3, 3)
	END_COMMON_PACKET_DECL()

	BYTE GetX(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[3] : 0; }
	BYTE GetY(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[4] : 0; }
	BYTE GetDir(){ BYTE* pPacket = AnyBuffer(); return pPacket ? ((pPacket[5] >> 4) % 7) : 0; }
};


/**
 * \brief 
 */
class CMeetPlayerPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CMeetPlayerPacket)
		PACKET_PATT4(0xC2, 0x00, 0x00, 0x12)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Meet player")
	END_COMMON_PACKET_DECL()

	int GetPlayerCount(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[4] : 0; }
	CStringA GetPlayerName(int idx, int* pExtBytes = 0, bool fAdvance = true);
	WORD GetPlayerId(int idx, int* pExtBytes = 0, bool fAdvance = true);
	bool SkipPlayer(int idx);

	int CalcExtraBytes(BYTE* pPacket, int idx, int iSize, int* pExtBytes, bool fAdvance = true);
	int GetPlayerExtraBytes(BYTE* pPacket, int idx, int iSize, int prevExtraBytes);
};



/**
 * \brief 
 */
class CForgetObjectPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CForgetObjectPacket)
		PACKET_PATT3(0xC1, 0x00, 0x14)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Forget object")
	END_COMMON_PACKET_DECL()

	int GetObjectCount(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[3] : 0; }
	WORD GetObjectId(int idx);
};


/**
 * \brief 
 */
class CForgetItemPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CForgetItemPacket)
		PACKET_PATT4(0xC2, 0x00, 0x00, 0x21)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Forget item(s)")
	END_COMMON_PACKET_DECL()

	int GetItemCount(){ BYTE* pPacket = AnyBuffer(); return pPacket ? pPacket[4] : 0; }
	WORD GetItemId(int idx);
};


/**
 * \brief 
 */
class CObjectDeathPacket
	: public CPacket
{
public:
	CObjectDeathPacket(WORD wId);

	BEGIN_COMMON_PACKET_DECL(CObjectDeathPacket)
		PACKET_PATT3(0xC1, 0x00, 0x17)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Object death")
	END_COMMON_PACKET_DECL()

	WORD GetObjectId(){ BYTE* pPacket = AnyBuffer(); return pPacket ? (((WORD)pPacket[3] << 8) | (WORD)pPacket[4]) : 0; }
};


/**
 * \brief 
 */
class CWingMix3lvlPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CWingMix3lvlPacket)
		PACKET_PATT4(0xC1, 0x04, 0x86, 0x28)
		PACKET_MASK4(0xFF, 0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Mix 3lvl wings")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CFeatherMixPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CFeatherMixPacket)
		PACKET_PATT4(0xC1, 0x04, 0x86, 0x29)
		PACKET_MASK4(0xFF, 0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Mix feather of condor")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CChaosMachineClosePacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CChaosMachineClosePacket)
		PACKET_PATT3(0xC1, 0x03, 0x87)
		PACKET_MASK3(0xFF, 0xFF, 0xFF)
		PACKET_DESCR("Close chaos machine")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 */
class CChaosMixSuccessPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CChaosMixSuccessPacket)
		PACKET_PATT4(0xC1, 0x00, 0x86, 0x01)
		PACKET_MASK4(0xFF, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Chaos machine mix succeeded")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief 
 *
 * C4 01 84 01 F3 10 | 1D | 00 12 4C FF 00 00 50 00 FF FF FF FF FF | 01 15 98 3F 00 00 50 00 FF FF FF FF FF  .....
 *                      ^    ^  ^----------------^                    ^  ^----------------^
 *                      |    |     Item Code                          |      Item Code
 *                      |  Inventory position                    Inventory Position
 *                    Item count
 */
class CInitInventoryPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CInitInventoryPacket)
		PACKET_PATT6(0xC4, 0x00, 0x00, 0x00, 0xF3, 0x10)
		PACKET_MASK6(0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF)
		PACKET_DESCR("Initialize inventory")
	END_COMMON_PACKET_DECL()

	int GetCount(){ BYTE* pPacket = AnyBuffer(); return !pPacket ? 0 : (int)pPacket[6]; }
	BYTE GetPosition(int idx);
	BYTE* GetItemData(int idx);

	WORD GetItemType(int idx)
	{
		BYTE* pPacket = GetItemData(idx);
		return (pPacket == 0) ? 0 : ((WORD)pPacket[5] << 4) | ((WORD)(pPacket[1] & 0x78) << 9) | pPacket[0];
	}

};


/**
 * \brief 
 * 
 * 0xC1, 0x05, 0x23, 0x01, 0x0C
 *                     ^     ^--..---- item positions
 *                  Number of items
 */
class CRemoveFromInventoryPacket
	: public CPacket
{
public:
	CRemoveFromInventoryPacket(BYTE pos)
	{
		BYTE buff[] = {0xC1, 0x05, 0x23, 0x01, pos};
		SetDecryptedPacket(buff, sizeof(buff));
		SetInjected();
	}

	BEGIN_COMMON_PACKET_DECL(CRemoveFromInventoryPacket)
		PACKET_PATT3(0xC1, 0x00, 0x23)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Remove from inventory")
	END_COMMON_PACKET_DECL()

	int GetCount(){ BYTE* pPacket = AnyBuffer(); return !pPacket ? 0 : (int)pPacket[3]; }
	int GetPosition(int idx);
};




/**  
 * \brief 
 *
 * C2 | 01 B3 | 31 | 00 21 | 00 00 00 C8 00 00 C0 00 | FF FF FF FF FF || 03 | 01 00 C8 00 00 C0 00 | FF FF FF FF FF || ...
 *                    ^_^     ^  ^----------------^      Some Junk        ^    ^----------------^      Some Junk
 *                 Item Count |      Item Code                            |        Item Code           
 *                           Item Position                            Item Position
 */
class CVaultContentsPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CVaultContentsPacket)
		PACKET_PATT4(0xC2, 0x00, 0x00, 0x31)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Vault contents")
	END_COMMON_PACKET_DECL()
};

/**  
 * \brief 
 *
 * 0xC3, 0x08, 0x1E, 0x26, 0x0C, 0x00, 0x00, 0xFF
 *                          ^       ^-- junk --^
 *                        Inventory Position
 */
class CUseItemPacket
	: public CPacket
{
public:
	CUseItemPacket(BYTE bPos)
	{
		BYTE buf[] = {0xC3, 0x08, 0x00, 0x26, bPos, 0x00, 0x00, 0xFF};

		SetDecryptedPacket(buf, sizeof(buf));
		SetInjected();
	}

	BEGIN_COMMON_PACKET_DECL(CUseItemPacket)
		PACKET_PATT4(0xC3, 0x00, 0x00, 0x26)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Use item")
		PACKET_XORP2(4, 3)
	END_COMMON_PACKET_DECL()

	BYTE GetInvPos()
	{
		BYTE* pPacket = AnyBuffer();
		return (pPacket == 0) ? 0 : pPacket[4];
	}
};


/**
 * \brief Viewport magic/skill effect animation (server tells client to show spell visual)
 *
 * C1 10 27 ...
 */
class CViewportSkillEffectPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CViewportSkillEffectPacket)
		PACKET_PATT3(0xC1, 0x00, 0x27)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Viewport skill effect")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Extended protocol sub-system (Season 4+), carries various sub-commands
 *
 * C1 xx FB sub_opcode ...
 */
class CExtendedProtocolPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CExtendedProtocolPacket)
		PACKET_PATT3(0xC1, 0x00, 0xFB)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Extended protocol")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Shield/SD gauge update from server
 *
 * C1 05 B2 ...
 */
class CShieldGaugePacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CShieldGaugePacket)
		PACKET_PATT3(0xC1, 0x00, 0xB2)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Shield gauge update")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Zen/money amount update from server
 *
 * C1 05 B8 ...
 */
class CZenUpdatePacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CZenUpdatePacket)
		PACKET_PATT3(0xC1, 0x00, 0xB8)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Zen/money update")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Master level system data from server
 *
 * C1 24 F9 ...
 */
class CMasterLevelPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CMasterLevelPacket)
		PACKET_PATT3(0xC1, 0x00, 0xF9)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Master level data")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Extended character info / buff duration / skill buff status
 *
 * C1 xx FD ...
 */
class CExtCharInfoPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CExtCharInfoPacket)
		PACKET_PATT3(0xC1, 0x00, 0xFD)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Extended character info")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Buff/skill effect applied on character
 *
 * C1 08 69 ...
 */
class CBuffEffectPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CBuffEffectPacket)
		PACKET_PATT3(0xC1, 0x00, 0x69)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Buff effect applied")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Object/character animation in viewport
 *
 * C1 06 A0 ...
 */
class CCharAnimationPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CCharAnimationPacket)
		PACKET_PATT3(0xC1, 0x00, 0xA0)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Character animation")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Viewport info / refresh notification
 *
 * C1 06 2A ...
 */
class CViewportInfoPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CViewportInfoPacket)
		PACKET_PATT3(0xC1, 0x00, 0x2A)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Viewport info")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Guild information from server
 *
 * C1 08 F5 ...
 */
class CGuildInfoPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CGuildInfoPacket)
		PACKET_PATT3(0xC1, 0x00, 0xF5)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Guild info")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Live client count / server population info
 *
 * C1 06 03 ...
 */
class CLiveClientCountPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CLiveClientCountPacket)
		PACKET_PATT3(0xC1, 0x00, 0x03)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Live client count")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Party HP bar update list
 *
 * C2 xx xx 38 ...
 */
class CPartyHPPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CPartyHPPacket)
		PACKET_PATT4(0xC2, 0x00, 0x00, 0x38)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Party HP bar update")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Viewport object spawn/update
 *
 * C2 xx xx 06 ...
 */
class CViewportObjectPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CViewportObjectPacket)
		PACKET_PATT4(0xC2, 0x00, 0x00, 0x06)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Viewport object spawn")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Massive skill area damage display from server
 *
 * C2 xx xx 1E ...
 */
class CMassiveSkillAreaPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CMassiveSkillAreaPacket)
		PACKET_PATT4(0xC2, 0x00, 0x00, 0x1E)
		PACKET_MASK4(0xFF, 0x00, 0x00, 0xFF)
		PACKET_DESCR("Massive skill area damage")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Hit info / damage display (JPN version with opcode 0xDC)
 *
 * C1 14 DC ...
 */
class CHitInfoJPNPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CHitInfoJPNPacket)
		PACKET_PATT3(0xC1, 0x00, 0xDC)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Hit info (JPN)")
	END_COMMON_PACKET_DECL()
};


/**
 * \brief Authentication sub-protocol (login result, server list, etc.)
 *
 * C1 xx F1 ...
 */
class CAuthSubProtocolPacket
	: public CPacket
{
public:
	BEGIN_COMMON_PACKET_DECL(CAuthSubProtocolPacket)
		PACKET_PATT3(0xC1, 0x00, 0xF1)
		PACKET_MASK3(0xFF, 0x00, 0xFF)
		PACKET_DESCR("Auth sub-protocol")
	END_COMMON_PACKET_DECL()
};


#endif //__CommonPackets_H
