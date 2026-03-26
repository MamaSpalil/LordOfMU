#ifndef __HistoryCategory_H
#define __HistoryCategory_H

#pragma once

/**
 * \brief Categories for history log entries.
 *
 *        Used by both the DLL (producer) and the UI overlay (consumer)
 *        to tag and filter history entries displayed in the Debug Console.
 */
enum HistoryCategory
{
	HISTORY_CAT_ITEMS_PICK   = 0,  // Item picked up (CPutInventoryPacket, non-Zen)
	HISTORY_CAT_ITEMS_DROP   = 1,  // Item appeared on ground (CMeetItemPacket, non-Zen)
	HISTORY_CAT_ZEN_PICK     = 2,  // Zen picked up (CZenUpdatePacket delta)
	HISTORY_CAT_ZEN_DROP     = 3,  // Zen appeared on ground (CMeetItemPacket, Zen type)
	HISTORY_CAT_KILL_MONSTER = 4,  // Monster killed (CKillExpPacket)
	HISTORY_CAT_SERVER       = 5,  // Server -> Client packet log
	HISTORY_CAT_CLIENT       = 6,  // Client -> Server packet log
	HISTORY_CAT_COUNT        = 7
};

// Bitmask: all categories enabled
#define HISTORY_FILTER_ALL  ((1u << HISTORY_CAT_COUNT) - 1u)

#endif // __HistoryCategory_H
