## Auto-Clicker Debug Log (AVANTA+ELITE Mode)

### Session Log (Before Fix)

The following log captured the broken state where MUEliteClicker.dll could not be found
by the Clicker DLL (MUAutoClicker.dll). Root causes identified and fixed below.

[2026-03-23 02:53:06] [CLICKER] DLL successfully hooked into main.exe, clicker initialized
[2026-03-23 02:53:23] [CLICKER] Clicker started
[2026-03-23 02:53:23] [CLICKER] === Auto-clicker starting ===
[2026-03-23 02:53:23] [DEBUG] Debug mode ENABLED - verbose logging active, console window opened
[2026-03-23 02:53:23] [CLICKER] Clicker Debug Mode auto-enabled on autoclicker start
[2026-03-23 02:53:23] [CLICKER] Debug mode active - propagating to DLL
[2026-03-23 02:53:23] [CLICKER] SayToServer: All lookup methods failed - m_hWnd=0x06100B94, PID=17940
[2026-03-23 02:53:23] [CLICKER] SayToServer diag: env __LordOfMU_HMODULE__='(not set)' (len=0)
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//debugmode on'
[2026-03-23 02:53:23] [CLICKER] Settings: fAutoPick=0 fAutoRepair=1 fAutoLife=1 fAutoReOff=0
[2026-03-23 02:53:23] [CLICKER] Settings: dwPickTime=1000 dwRepairTime=300000 dwHealTime=10000 dwClass=5
[2026-03-23 02:53:23] [CLICKER] Settings: fAdvAutoPick=1 fExitAtLvl400=0 fAntiAFKProtect=0 fPickRunMode=1
[2026-03-23 02:53:23] [CLICKER] Settings: fAdvPickBless=1 fAdvPickSoul=1 fAdvPickChaos=1 fAdvPickLife=1
[2026-03-23 02:53:23] [CLICKER] Settings: fAdvPickCreation=1 fAdvPickGuardian=1 fAdvPickExl=1 fAdvPickZen=1
[2026-03-23 02:53:23] [CLICKER] Settings: fAdvPickCustom=1 fAdvPickCustomMove=1 wPickCustomCode=0x0E33
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//autosay off'
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_stealth_opt susp_zen_pick off'
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_stealth_opt susp_move_pick off'
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_stealth_opt susp_pick off'
[2026-03-23 02:53:23] [CLICKER] Advanced auto-pickup enabled - setting options
[2026-03-23 02:53:23] [CLICKER] SetAdvancedAutopickOptions: sending all pickup options to DLL
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt bless on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt bless on on'
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt soul on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt soul on on'
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt chaos on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt chaos on on'
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt jol on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt jol on on'
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt joc on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt joc on on'
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt jog on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt jog on on'
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt exl on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt exl on on'
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt zen on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt zen on on'
[2026-03-23 02:53:23] [CLICKER] Sending to DLL: //set_pick_opt custom on on
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//set_pick_opt custom on on'
[2026-03-23 02:53:23] [CLICKER] Sending //pick clear to DLL
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//pick clear'
[2026-03-23 02:53:23] [CLICKER] Sending custom pick command: //pick 14 51
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//pick 14 51'
[2026-03-23 02:53:23] [CLICKER] SetAdvancedAutopickOptions: all pickup options sent
[2026-03-23 02:53:23] [CLICKER] Sending //autopick on to DLL
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//autopick on'
[2026-03-23 02:53:23] [CLICKER] Sending //pickclass 5 to DLL
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//pickclass 5'
[2026-03-23 02:53:23] [CLICKER] Auto run mode enabled (boots level detection)
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//pickautorunmode on'
[2026-03-23 02:53:23] [CLICKER] Pick-up run mode enabled
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//pickrunmode on'
[2026-03-23 02:53:23] [CLICKER] Pick-up debug logging enabled
[2026-03-23 02:53:23] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//pickdebug on'
[2026-03-23 02:53:23] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:24] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:24] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:24] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:25] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:25] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:25] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:26] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:27] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:27] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:27] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:28] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:28] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:28] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:29] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:29] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:30] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:30] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:30] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:31] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:31] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:31] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:32] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:32] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:32] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:33] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:33] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:33] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:33] [CLICKER] Auto-heal triggered (interval=10000 ms)
[2026-03-23 02:53:33] [CLICKER] Auto-heal triggered
[2026-03-23 02:53:34] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:34] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:34] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:35] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:35] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:35] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:36] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:36] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:36] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:37] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:37] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:37] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:38] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:38] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:38] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:39] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:39] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:39] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:40] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:40] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:40] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:41] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:41] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:41] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:42] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 02:53:42] [CLICKER] Auto-pickup SPACE triggered (interval=1000 ms, fAutoPick=0, fAdvAutoPick=1)
[2026-03-23 02:53:42] [CLICKER] Auto-pickup triggered
[2026-03-23 02:53:42] [CLICKER] Clicker stop requested
[2026-03-23 02:53:42] [CLICKER] === Auto-clicker stopping ===
[2026-03-23 02:53:42] [CLICKER] Advanced auto-pickup disabled
[2026-03-23 02:53:42] [CLICKER] Sending //pickdebug off and //autopick off to DLL
[2026-03-23 02:53:42] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//pickdebug off'
[2026-03-23 02:53:42] [CLICKER] SayToServer FAILED: DLL module not found for cmd='//autopick off'
[2026-03-23 02:53:42] [CLICKER] Debug mode remains active in DLL after clicker stop
[2026-03-23 02:53:42] [CLICKER] === Auto-clicker stopped ===
[2026-03-23 02:53:42] [CLICKER] Clicker stopped

### Root Cause Analysis

**Problem 1: SayToServer fails — "DLL module not found" for ALL commands**

ROOT CAUSE: In `LoaderDll.cpp`, `RunDllWndProc()` had no `return` statement after 
`CKillUtil::KillGame()` calls. If `KillGame()` does not terminate the process immediately 
(e.g., during WndProc execution, or if `TerminateProcess` is delayed), execution falls through:
- The `else` block that sets `SetProp()` and `SetEnvironmentVariableA()` is NEVER entered
- The Clicker DLL is loaded by the second block without the LordOfMU DLL
- Result: ALL SayToServer lookup methods fail (env var not set, window property not set, 
  stealth name module not loaded, Toolhelp32 finds nothing)

FIX: Added `return res;` after each `CKillUtil::KillGame()` call to prevent fall-through.
Also restructured the error handling to not use else-block for the success path, ensuring
`SetProp`/`SetEnvironmentVariableA` always execute after successful `LoadLibrary`.

**Problem 2: SPACE key fires when fAdvAutoPick=1 but fAutoPick=0**

ROOT CAUSE: `DoClicker()` in `ClickerJob.cpp` used `fAutoPick || fAdvAutoPick` for the 
SPACE key pickup condition. When Advanced Pick-up (fAdvAutoPick) is enabled, the DLL handles 
pickup via packet injection. Pressing SPACE interferes by picking up unfiltered items and 
conflicting with DLL movement commands.

FIX: Changed condition to only `fAutoPick` for SPACE key. DLL-based pickup handles 
fAdvAutoPick independently.

**Problem 3: No packet logging for dropped/picked items**

ROOT CAUSE: The DLL AutoPickupFilter logs all packets (CMeetItemPacket, CPickItemPacket)
unconditionally — but `m_fEnabled` was never set to TRUE because the `//autopick on` command 
never reached the DLL (due to Problem 1). With `m_fEnabled=0`, incoming item packets are 
logged as "autopick DISABLED - items ignored" and pickup packets are never generated.

FIX: Fixing Problem 1 ensures commands reach the DLL, enabling `m_fEnabled` and packet logging.

**Problem 4: Clicker DLL loaded without LordOfMU DLL**

ROOT CAUSE: `RunDllWndProc()` always loaded the Clicker DLL in the second block, even if 
the LordOfMU DLL failed to load in the first block.

FIX: Added `g_hInjected` check before loading the Clicker DLL — only load it if the 
LordOfMU DLL was successfully loaded.

### Fixes Applied

- `src/LoaderDll/LoaderDll.cpp`: Added `return res` after KillGame, improved error logging,
  source file validation in CopyDlls, guard Clicker DLL loading on LordOfMU DLL success
- `src/LordOfClicker/ClickerJob.cpp`: SPACE key only triggers for fAutoPick, not fAdvAutoPick
- `src/LordOfClicker/MuWindow.cpp`: SayToServer retry with delay, better diagnostics,
  GetProp validation with SendCommand export check
- AVANTA+ELITE mode: F9 opens unified settings dialog (already implemented)
- Both __CLICKER_AVANTA__ and __CLICKER_ELITE__ defined in version.h for unified mode
