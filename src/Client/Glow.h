#include "stdafx.h"

#ifndef __GLOW_H__
#define __GLOW_H__
#define pPartObjectColor				((void(__cdecl*)(int Type,float AxisX,float AxisY,vec3_t Light,bool Mode))0x0068BBBD)
typedef struct
{
	DWORD	Type;
	DWORD	Index;
	FLOAT	Red;
	FLOAT	Green;
	FLOAT	Blue;
	FLOAT	X;
	FLOAT	Y;
}GlowStruct,*lpGlowStruct;

class cGlow
{
	public:
	// ----
	cGlow();
	// ----
	void Init();
	void Load();
	// ----
	static void Glow(int Type,float AxisX,float AxisY,vec3_t Light,bool Mode);
	// ----
	private:
	// ----
	int iCount;
	GlowStruct m_GlowStruct[512];
};

extern cGlow gGlow;

#endif	// __GLOW_H__