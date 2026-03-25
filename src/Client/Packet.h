#pragma once 

#include <windows.h> 
#include <stdio.h> 
#define	MAX_XOR_KEYS		32
class CStreamPacketEngine_Client 
{ 
public: 
    CStreamPacketEngine_Client(void); 
    ~CStreamPacketEngine_Client(void); 
    void Clear(void) ; 
    BOOL AddData(void* pSrc, WORD wSize ); 
    BOOL ExtractPacket(void* pTar); 
	BYTE XOR[MAX_XOR_KEYS];
protected: 
    void XorData(int iStart, int iEnd, int iDir); 
private: 
    WORD m_wSize;     
    BYTE m_byBuffer[2048];     

};