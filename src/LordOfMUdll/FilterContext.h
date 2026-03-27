#ifndef __FilterContext_H
#define __FilterContext_H

#pragma once 

/**
 * \brief Context data passed through the packet filter chain.
 *        Carries per-packet metadata that filters can read or modify.
 */
class CFilterContext
{
public:
	CFilterContext() : dwMainOffset(0) {}

	/**
	 * \brief For CLIENT->SERVER packets: the offset within main.exe
	 *        from which the send() call originated.
	 *        0 means unknown (e.g. injected packets from the DLL).
	 */
	DWORD dwMainOffset;
};

#endif //__FilterContext_H