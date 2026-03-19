#ifndef __HandshakeGenerator_H
#define __HandshakeGenerator_H

#pragma once

#include <string>


/**
 * \brief 
 */
class CHandshakeGenerator
{
public:
	CHandshakeGenerator();
	virtual ~CHandshakeGenerator();

	std::string GenerateHandshake();
	bool CheckHandshake(std::string);

protected:
	void SeedParameter(int idx);

protected:
	static const UINT PARAMETER_VALUES_COUNT = 128;

private:
	int** m_aiSeed;

	BYTE m_vOldHS[25];
};

#endif //__HandshakeGenerator_H