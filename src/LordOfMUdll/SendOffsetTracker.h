#ifndef __SendOffsetTracker_H
#define __SendOffsetTracker_H

#pragma once

/**
 * \brief Thread-safe single-producer single-consumer ring buffer
 *        for passing main.exe return address offsets from the ws2_32!send
 *        hook (main.exe thread) to the packet filter chain (proxy thread).
 *
 *        Each send() call from main.exe pushes one entry.
 *        Each CLIENT->SERVER packet processed pops one entry.
 */

static const int SEND_OFFSET_RING_SIZE = 256;

struct SendOffsetRing
{
	volatile DWORD entries[SEND_OFFSET_RING_SIZE];
	volatile LONG  writeIdx;
	volatile LONG  readIdx;
};

/**
 * \brief Global ring buffer instance (shared between send hook and proxy thread).
 *        Uses __declspec(selectany) so it can be defined in a header included
 *        by multiple translation units without linker errors.
 */
__declspec(selectany) SendOffsetRing g_sendOffsetRing = { {0}, 0, 0 };


/**
 * \brief Push a main.exe offset into the ring buffer.
 *        Called from the send() hook on main.exe's thread (single producer).
 */
inline void PushMainSendOffset(DWORD dwOffset)
{
	LONG idx = InterlockedIncrement(&g_sendOffsetRing.writeIdx) - 1;
	g_sendOffsetRing.entries[idx & (SEND_OFFSET_RING_SIZE - 1)] = dwOffset;
}


/**
 * \brief Pop a main.exe offset from the ring buffer.
 *        Called from the proxy thread when processing a CLIENT->SERVER packet.
 *        Returns 0 if no entries are available.
 *
 *        Thread safety: SPSC (single-producer, single-consumer) on x86.
 *        InterlockedIncrement on the producer side provides a full memory
 *        barrier ensuring the entry is visible before writeIdx advances.
 *        We read writeIdx with InterlockedCompareExchange(0) for explicit
 *        atomic load semantics.
 */
inline DWORD PopMainSendOffset()
{
	LONG readIdx = g_sendOffsetRing.readIdx;
	LONG writeIdx = InterlockedCompareExchange(&g_sendOffsetRing.writeIdx, 0, 0);

	if (readIdx >= writeIdx)
		return 0;

	DWORD dwOffset = g_sendOffsetRing.entries[readIdx & (SEND_OFFSET_RING_SIZE - 1)];
	InterlockedIncrement(&g_sendOffsetRing.readIdx);
	return dwOffset;
}

#endif //__SendOffsetTracker_H
