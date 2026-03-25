#include "stdafx.h"
#include "Glow.h"
#include "TMemory.h"
#include "Structure.h"


cGlow gGlow;

cGlow::cGlow()
{
}

void cGlow::Init()
{
	this->iCount=NULL;
	ZeroMemory(this->m_GlowStruct,sizeof(this->m_GlowStruct));
}

void cGlow::Glow(int Type,float AxisX,float AxisY,vec3_t Light,bool Mode)
{
	pPartObjectColor(Type,AxisX,AxisY,Light,Mode);
	// ----
	for(int i=0;i<=gGlow.iCount;i++)
	{
		 if(gGlow.m_GlowStruct[i].Type == -1)
		 {
			 if(Type == ITEM2(7,gGlow.m_GlowStruct[i].Index) && Type == ITEM2(8,gGlow.m_GlowStruct[i].Index)
				 && Type == ITEM2(9,gGlow.m_GlowStruct[i].Index) && Type == ITEM2(10,gGlow.m_GlowStruct[i].Index)
				 && Type == ITEM2(11,gGlow.m_GlowStruct[i].Index))
			 {
				Vector(gGlow.m_GlowStruct[i].Red,gGlow.m_GlowStruct[i].Green,gGlow.m_GlowStruct[i].Blue,Light);
				// ----
				if(gGlow.m_GlowStruct[i].X != NONE) { abs(AxisX*gGlow.m_GlowStruct[i].X); }
				if(gGlow.m_GlowStruct[i].Y != NONE) { abs(AxisY*gGlow.m_GlowStruct[i].Y); }
			 }
		 }
		else 
		if(Type == ITEM2(gGlow.m_GlowStruct[i].Type,gGlow.m_GlowStruct[i].Index))
		{
			Vector(gGlow.m_GlowStruct[i].Red,gGlow.m_GlowStruct[i].Green,gGlow.m_GlowStruct[i].Blue,Light);
			// ----
			if(gGlow.m_GlowStruct[i].X != NONE) { abs(AxisX*gGlow.m_GlowStruct[i].X); }
			if(gGlow.m_GlowStruct[i].Y != NONE) { abs(AxisY*gGlow.m_GlowStruct[i].Y); }
		}
	}
}

void cGlow::Load()
{
	SMDFile = fopen("Data\\Customs\\Configs\\Glow.bmd", "r");

	this->iCount = 0;

	if (SMDFile == NULL)
	{
		MessageBoxA(0,"Failed to load file Glow.bmd","CRITICAL ERROR",0);
		ExitProcess(1);
		return;
	}

	int Token = 0;

	this->Init();

	while (true)
	{
		Token = GetToken();

		if (Token == END)
		{
			break;
		}
		if (Token == NUMBER)
		{
			this->m_GlowStruct[this->iCount].Type	= (char)TokenNumber;
			// ----
			Token		= GetToken();
			this->m_GlowStruct[this->iCount].Index	= (char)TokenNumber;
			// ----
			Token		= GetToken();
			this->m_GlowStruct[this->iCount].Red	= (unsigned char)TokenNumber;
			// ----
			Token		= GetToken();
			this->m_GlowStruct[this->iCount].Green	= (unsigned char)TokenNumber;
			// ----
			Token		= GetToken();
			this->m_GlowStruct[this->iCount].Blue	= (unsigned char)TokenNumber;
			//// ----
			Token		= GetToken();
			this->m_GlowStruct[this->iCount].X		= (unsigned char)TokenNumber;
			// ----
			Token		= GetToken();
			this->m_GlowStruct[this->iCount].Y		= (unsigned char)TokenNumber;
			//// ----
			this->iCount++;
		}
	}
	// ----
	if(this->iCount>NULL)
	{
		SetOp((LPVOID)0x0053B8F5, (LPVOID)this->Glow, ASM::CALL);
		//SetOp((LPVOID)0x00693716, (LPVOID)this->Glow, ASM::CALL); kris bez lvl visual bag
	}
	// ----

}