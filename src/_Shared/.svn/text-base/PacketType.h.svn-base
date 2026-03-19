#ifndef __PacketType_H
#define __PacketType_H

#pragma once

//old
//Phi   0xDD  0xD6    0xDF              0x10 ? 0x11 <-->0x1D


//      walk  attack  movefix obj_moved magicskill
//KOR   0xD3  0xD7    0xDF    0xD3      0x1D ? 0x10
//jpn   0x1D  0xDC    0xD6    0x1D      0xD7
//Phi   0xD4  0x11    0x15              0x1D
//chs	0xD7  0xD9	  0xD0	            0x1D
//vtm	0xD9  0x15    0xDC              0x1D


// V1 = 104
// V2 = 105
#define PTYPE_KOR 0x00000001
#define PTYPE_CHS 0x00000002
#define PTYPE_VTM 0x00000003
#define PTYPE_JPN 0x00000004
#define PTYPE_PHI 0x00000005

#define GTYPE_S3  0x01000000
#define GTYPE_S4  0x02000000

/**
 * \brief 
 */
class CPacketType
{
public:
	CPacketType();
	CPacketType(const CPacketType& in);
	virtual ~CPacketType();

	CPacketType& operator=(const CPacketType& in);

public:
	// Packet Type Interface
	// Note: The first elements mask and pattern arrays contain the number of following elements
	virtual const BYTE* GetPattern() const { return m_pPat; }
	virtual const BYTE* GetMask() const { return m_pMask; }
	virtual const BYTE* GetXorParams() const { return m_abXorP; }
	virtual const char* GetDescription() const { return m_szDescr; }

	static ULONG GetVersion(){ return m_ulVersion; }
	static void SetVersion(ULONG ulVersion){ m_ulVersion = (m_ulVersion & 0x00FFFFFF) | (ulVersion & 0xFF000000); }
	static void SetProtocol(ULONG ulProto){ m_ulVersion = (m_ulVersion & 0xFFFFFF00) | (ulProto & 0x000000FF); }

protected:
	BYTE* m_pPat;
	BYTE* m_pMask;
	BYTE  m_abXorP[2]; // 0 - offset for packet start, 1 - size of header
	char* m_szDescr;
	static ULONG m_ulVersion;
};

#endif //__PacketType_H