#ifndef __BufferUtil_H
#define __BufferUtil_H

#pragma once

#include "atlstr.h"

/**
 *
 */
class CBufferUtil
{
public:
	/**
	 * \brief 
	 */
	static CStringA BufferToHex(BYTE* buf, int len)
	{
		return BufferToHex((char*)buf, len);
	}


	/**
	 * \brief
	 */
	static CStringA BufferToHex(char* buf, int len)
	{
		CStringA res("");

		if (buf && len != 0)
		{
			for (int i=0; i < len; ++i)
			{
				char hex_buff[4] = {0};

				hex_buff[0] = (buf[i] >> 4) & 0xF;
				hex_buff[1] = buf[i] & 0xF;

				hex_buff[0] += (hex_buff[0] < 10) ? 0x30 : 0x37;
				hex_buff[1] += (hex_buff[1] < 10) ? 0x30 : 0x37;

				hex_buff[2] = ' ';

				res += hex_buff;
			}
		}

		return res;
	}
};

#endif //__BufferUtil_H