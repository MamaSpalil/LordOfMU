#ifndef __COMMON_MESSAGES_H
#define __COMMON_MESSAGES_H

/**
 * \brief Shared WM_APP-based message definitions used across DLL modules.
 *        Centralizes message IDs to prevent value mismatches between
 *        LordOfMUdll and LordOfClicker (AutoClicker).
 */

#define WM_CHAR_SELECTED    (WM_APP + 510)
#define WM_SET_GAME_VERSION (WM_APP + 509)
#define WM_CHAR_DESELECTED  (WM_APP + 511)

#endif // __COMMON_MESSAGES_H
