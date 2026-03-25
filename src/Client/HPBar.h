#include "stdafx.h"

#pragma once

#pragma pack(push, 1)
struct Angle
{
	float X;
	float Y;
	float Z;
};

struct ViewportInfo
{
	bool Live;				// 00
	BYTE Unknown[0x0F];		// 01
	Angle Angle1;			// 10
	BYTE Unknown2[0xA0];	// 1C
	BYTE Type;				// BC
	BYTE Unknown3[0xA7];	// BD
	Angle Angle2;			// 164
	BYTE Unknown4[0xA1];	// 170
	char Name[33];			// 211
	short GuildIndex;		// 232
	short Index;			// 234
	BYTE Unknown5[0x24E];	// 236
};

#pragma pack(pop)

class HPBar
{
public:
	HPBar();
	~HPBar();

public:
	void Load();
	void Clear();
	void Insert(short Index, BYTE Percent);
	BYTE GetPercent(short Index) const;

public:
	static void Draw();

private:
	std::unordered_map<short, BYTE> HP;
};

extern HPBar gHPBar;
extern bool MonsterHPBarMode;