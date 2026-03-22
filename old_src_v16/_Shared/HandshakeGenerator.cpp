#include "stdafx.h"
#include "HandshakeGenerator.h"
#include "base64.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**
 * \brief 
 */
CHandshakeGenerator::CHandshakeGenerator()
{
	memset(m_vOldHS, 0, sizeof(m_vOldHS));

	m_aiSeed = new int*[4];
	
	for (int i=0; i < 4; i++)
	{
		m_aiSeed[i] = new int[PARAMETER_VALUES_COUNT];
		SeedParameter(i);
	}
}


/**
 * \brief 
 */
CHandshakeGenerator::~CHandshakeGenerator()
{
	if (m_aiSeed)
	{
		for (int i=0; i < 4; i++)
		{
			if (m_aiSeed[i])
				delete[] m_aiSeed[i];
		}

		delete[] m_aiSeed;
		m_aiSeed = 0;
	}
}


/**
 * \brief 
 */
std::string CHandshakeGenerator::GenerateHandshake()
{
	srand(GetTickCount());

	BYTE ai = rand() % PARAMETER_VALUES_COUNT;
	BYTE bi = rand() % PARAMETER_VALUES_COUNT;
	BYTE ci = rand() % PARAMETER_VALUES_COUNT;
	BYTE xi = rand() % PARAMETER_VALUES_COUNT;
	int a = m_aiSeed[0][ai];
	int b = m_aiSeed[1][bi];
	int c = m_aiSeed[2][ci];
	int x = m_aiSeed[3][xi];
	int y = a*x*x + b*x + c;

	BYTE aiIndices[4] = {ai, bi, ci, xi};
	int aiHandshake[5] = {a, b, c, x, y};
	
	BYTE data[sizeof(aiIndices) + sizeof(aiHandshake) + 1] = {0}; // size of handshake = 4 + 5*4 + 1 = 25 bytes
	data[0] = 'P';
	memcpy(data + 1, aiIndices, sizeof(aiIndices));
	memcpy(data + 1 + sizeof(aiIndices), (BYTE*)aiHandshake, sizeof(aiHandshake));

	return base64_encode(data, sizeof(data));
}


/**
 * \brief 
 */
bool CHandshakeGenerator::CheckHandshake(std::string data)
{
	if (data.length() != 25 || data[0] != 'P')
		return false;

	BYTE buf[25] = {0};
	memcpy(buf, data.c_str(), 25);

	if (memcmp(m_vOldHS, buf, 25) == 0)
		return false;

	memcpy(m_vOldHS, buf, 25);

	BYTE* abIndices = buf + 1;
	int* aiHandshake = (int*)(buf + 5);

	BYTE ai = abIndices[0];
	BYTE bi = abIndices[1];
	BYTE ci = abIndices[2];
	BYTE xi = abIndices[3];

	int a1 = aiHandshake[0];
	int b1 = aiHandshake[1];
	int c1 = aiHandshake[2];
	int x1 = aiHandshake[3];

	int a2 = m_aiSeed[0][ai];
	int b2 = m_aiSeed[1][bi];
	int c2 = m_aiSeed[2][ci];
	int x2 = m_aiSeed[3][xi];

	int y1 = aiHandshake[4];
	int y2 = a2*x2*x2 + b2*x2 + c2;

	return (a1 == a2 && b1 == b2 && c1 == c2 && x1 == x2 && y1 == y2);
}


/**
 * \brief 
 */
void CHandshakeGenerator::SeedParameter(int idx)
{
	for (int i=0; i < PARAMETER_VALUES_COUNT; i++)
	{
		if (i % 2 == 0)
		{
			m_aiSeed[idx][i] = (idx*idx*121213 + i*i*i + 112);
		}
		else
		{
			m_aiSeed[idx][i] = -(idx*idx*112213 + i*i*i + 112);
		}
	}
}
