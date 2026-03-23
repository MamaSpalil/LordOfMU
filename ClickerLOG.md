## Ошибки найденные в логе / Issues Found in Log

### 1. Пустые hex-данные пакетов (строки 38, 44, 48-50)
**Причина:** В `PacketLogger.cpp` использовался `const char* szHex` для хранения результата `CBufferUtil::BufferToHex()`, 
который возвращает `CStringA` (временный объект). После разрушения временного объекта указатель становился висячим (dangling pointer),
и при чтении из него возвращались пустые данные или мусор.

**Исправление:** Заменено на `CStringA szHex` — объект владеет данными и живёт до конца блока.

### 2. Гонка состояний: подбор предмета после остановки кликера (строки 232-256)
**Причина:** `GoPickNextItem()` не проверял `m_fEnabled` между перемещением к предмету и отправкой `CPickItemPacket`.
Если пользователь останавливал кликер (`F5`) во время движения, пакеты подбора и возврата всё равно отправлялись.
- Строка 246: `CPickItemPacket` отправлен ПОСЛЕ `autopick off`
- Строка 250: `RunTo` обратно выполнен ПОСЛЕ остановки

**Исправление:** Добавлена проверка `m_fEnabled` перед `PickItem()` — если autopick выключен во время движения, подбор отменяется.

### 3. Отсутствие радиуса подбора
**Причина:** `m_iDist = -1` (без ограничений) — предметы подбирались на любом расстоянии.
Кастомные предметы с `Move To` вообще не проверяли дистанцию.

**Исправление:** Добавлен радиус подбора (1-8 тайлов) для ВСЕХ режимов (RunTo, WalkTo, NoMove).

---

[2026-03-23 05:46:56] [CLICKER] EnsureLordOfMUDllLoaded: Loading MUEliteClicker.dll from 'D:\1WOM\MUEliteClicker.dll'
[2026-03-23 05:46:56] [CLICKER] EnsureLordOfMUDllLoaded: Loaded at 0x77510000 - initializing proxy hooks
[2026-03-23 05:46:56] [DEBUG] LordOfMU DLL initializing (InternalInit)
[2026-03-23 05:46:56] [DEBUG] Debug mode handler installed
[2026-03-23 05:46:56] [HOOK] Crash handler installed for main.exe process
[2026-03-23 05:46:56] [HOOK] DLL hook presence check passed - no duplicate DLL detected
[2026-03-23 05:46:56] [HOOK] Hook integrity WARNING: ws2_32!connect NOT hooked (byte=0x8B at 0x77035710)
[2026-03-23 05:46:56] [HOOK] Applying DLL hooks for the first time (InterlockedCompareExchange 0->1)
[2026-03-23 05:46:56] [HOOK] Proxifier initialization started
[2026-03-23 05:46:56] [HOOK] All Winsock API hooks installed successfully
[2026-03-23 05:46:56] [HOOK] Proxifier initialization completed successfully
[2026-03-23 05:46:56] [HOOK] DLL hooks applied successfully - proxy system active
[2026-03-23 05:46:56] [HOOK] Hook integrity WARNING: ws2_32!connect NOT hooked (byte=0x90 at 0x77035710)
[2026-03-23 05:46:56] [DEBUG] LordOfMU DLL initialization complete
[2026-03-23 05:46:56] [CLICKER] EnsureLordOfMUDllLoaded: DllGetClassObject called, root='D:\1WOM\'
[2026-03-23 05:46:56] [CLICKER] EnsureLordOfMUDllLoaded: env var __LordOfMU_HMODULE__ set to '0x77510000'
[2026-03-23 05:47:11] [CLICKER] DLL successfully hooked into main.exe, clicker initialized
[2026-03-23 05:47:13] [SOCKET] IP override from Connect.ini: 192.168.0.105
[2026-03-23 05:47:13] [SOCKET] connect() socket=2216 IP=192.168.0.105 port=44405
[2026-03-23 05:47:15] [SOCKET] IP override from Connect.ini: 192.168.0.105
[2026-03-23 05:47:15] [SOCKET] connect() socket=2216 IP=192.168.0.105 port=55901
[2026-03-23 05:47:15] [PACKET] Packet logger filter initialized - AVANTA+ELITE relevant packets only
[2026-03-23 05:47:26] [PACKET] SERVER -> CLIENT | Server message | Len=56 | C1 38 0D 01 00 00 00 00 00 00 00 00 00 5B 50 68 6F 65 6E 69 78 5D 20 57 65 6C 6C 63 6F 6D 65 20 74 6F 20 50 68 6F 65 6E 69 78 20 56 69 70 20 41 44 4D 49 4E 00 00 00 00 
[2026-03-23 05:47:26] [PACKET] SERVER -> CLIENT | Server message | Len=49 | C1 31 0D 01 00 00 00 00 00 00 00 00 00 5B 50 68 6F 65 6E 69 78 5D 20 50 6C 61 79 65 72 73 20 4F 6E 6C 69 6E 65 3A 20 33 39 2F 33 30 30 00 20 41 44 
[2026-03-23 05:47:26] [PACKET] SERVER -> CLIENT | Server message | Len=49 | C1 31 0D 01 00 00 00 00 00 5B 50 68 6F 5B 50 68 6F 65 6E 69 78 5D 20 48 6F 6D 65 20 50 61 67 65 3A 20 50 68 6F 65 6E 69 78 2E 63 6F 6D 00 49 4E 00 
[2026-03-23 05:47:26] [PACKET] SERVER -> CLIENT | Character stats | Len=81 | C3 51 00 F3 03 8D 86 00 00 F0 59 D1 E3 90 05 FF E7 BE 02 FF 7F FF 7F FF 7F FF 7F D8 16 D8 16 FF 16 A2 16 2C 8C 2C 8C 99 39 32 73 19 00 26 DE 10 18 03 20 69 F7 59 00 00 00 00 00 59 00 05 00 00 00 0A 04 00 00 00 00 00 00 00 00 00 00 AC 6B 58 00 
[2026-03-23 05:47:26] [PACKET] SERVER -> CLIENT | Server message | Len=69 | C1 45 0D 01 00 00 00 00 00 00 00 00 00 5B 4C 75 63 6B 79 20 57 68 65 65 6C 5D 3A 20 44 6F 6E 27 74 20 66 6F 72 67 65 74 20 79 6F 75 20 6E 65 65 64 20 73 70 61 63 65 20 66 6F 72 20 69 74 65 6D 73 00 00 00 00 
[2026-03-23 05:47:26] [PACKET] SERVER -> CLIENT | Character setup | Len=19 | C1 13 F3 30 00 38 00 00 00 00 00 00 00 00 09 00 04 08 04 
[2026-03-23 05:47:42] [PACKET] SERVER -> CLIENT | Warp reply | Len=10 | C3 0A 02 1C 01 06 03 90 00 E0 
[2026-03-23 05:47:42] [PICKUP] RECV CWarpReplyPacket: clearing pickup queues | Len=10 | C3 0A 02 1C 01 06 03 90 00 E0 
[2026-03-23 05:48:05] [PACKET] SERVER -> CLIENT | Server anouncement / GM message | Len=41 | C1 29 0D 00 4A 00 00 00 1C 11 1E 00 00 54 68 69 73 20 4D 65 73 73 61 67 65 20 45 76 65 72 79 20 31 20 6D 69 6E 00 00 00 BE 
[2026-03-23 05:48:07] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 05 C5 42 44 
[2026-03-23 05:48:07] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 06 C6 42 54 
[2026-03-23 05:48:07] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 06 C7 42 34 
[2026-03-23 05:48:08] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 07 C7 42 44 
[2026-03-23 05:48:08] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 08 C8 42 34 
[2026-03-23 05:48:08] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 09 C8 41 40 
[2026-03-23 05:48:08] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 |  
[2026-03-23 05:48:09] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 0B C9 21 20 
[2026-03-23 05:48:09] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 0C C8 31 30 
[2026-03-23 05:48:10] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 0D C8 32 33 
[2026-03-23 05:48:10] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 0E C8 31 30 
[2026-03-23 05:48:10] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 0F C8 51 50 
[2026-03-23 05:48:10] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 |  
[2026-03-23 05:48:10] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 10 C9 51 50 
[2026-03-23 05:48:11] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 10 CA 31 30 
[2026-03-23 05:48:11] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 11 CA 51 50 
[2026-03-23 05:48:12] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 |  
[2026-03-23 05:48:12] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 |  
[2026-03-23 05:48:12] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 |  
[2026-03-23 05:48:12] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 13 C9 41 20 
[2026-03-23 05:48:12] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 14 C8 32 33 
[2026-03-23 05:48:13] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 15 C8 32 33 
[2026-03-23 05:48:13] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 16 C8 32 23 
[2026-03-23 05:48:14] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 17 C7 42 34 
[2026-03-23 05:48:14] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 18 C7 41 40 
[2026-03-23 05:48:14] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 19 C8 31 30 
[2026-03-23 05:48:18] [PACKET] CLIENT -> SERVER | Char Move CTS | Len=7 | C1 07 1D 1D D0 31 30 
[2026-03-23 05:48:21] [CLICKER] Clicker started
[2026-03-23 05:48:21] [CLICKER] === Auto-clicker starting ===
[2026-03-23 05:48:21] [DEBUG] Debug mode ENABLED - verbose logging active, console window opened
[2026-03-23 05:48:21] [CLICKER] Clicker Debug Mode auto-enabled on autoclicker start
[2026-03-23 05:48:21] [CLICKER] Debug mode active - propagating to DLL
[2026-03-23 05:48:21] [CLICKER] SayToServer: DLL found at 0x77510000 via env var (after 1 attempts)
[2026-03-23 05:48:21] [CLICKER] Settings: fAutoPick=0 fAutoRepair=1 fAutoLife=1 fAutoReOff=0
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=28 | C1 1C 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 64 65 62 75 67 6D 6F 64 65 20 6F 6E 00 
[2026-03-23 05:48:21] [CLICKER] Settings: dwPickTime=1000 dwRepairTime=300000 dwHealTime=10000 dwClass=5
[2026-03-23 05:48:21] [CLICKER] Settings: fAdvAutoPick=1 fExitAtLvl400=0 fAntiAFKProtect=0 fPickRunMode=1
[2026-03-23 05:48:21] [DEBUG] Debug mode ENABLED - verbose logging active, console window opened
[2026-03-23 05:48:21] [DEBUG] Debug mode ENABLED in DLL context via //debugmode command
[2026-03-23 05:48:21] [CLICKER] Settings: fAdvPickBless=1 fAdvPickSoul=1 fAdvPickChaos=1 fAdvPickLife=1
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=39 | C1 27 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 44 65 62 75 67 20 6D 6F 64 65 20 6F 6E 20 5B 4F...
[2026-03-23 05:48:21] [CLICKER] Settings: fAdvPickCreation=1 fAdvPickGuardian=1 fAdvPickExl=1 fAdvPickZen=1
[2026-03-23 05:48:21] [CLICKER] Settings: fAdvPickCustom=1 fAdvPickCustomMove=1 wPickCustomCode=0x0E33
[2026-03-23 05:48:21] [CLICKER] Advanced auto-pickup enabled - setting options
[2026-03-23 05:48:21] [CLICKER] SetAdvancedAutopickOptions: sending all pickup options to DLL
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt bless on on
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt soul on on
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=27 | C1 1B 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 61 75 74 6F 73 61 79 20 6F 66 66 00 
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt chaos on on
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=37 | C1 25 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 61 75 74 6F 73 61 79 20 6F 66 66 20 5B 4F 4B 5D...
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt jol on on
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=49 | C1 31 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 73 74 65 61 6C 74 68 5F 6F 70 74 20 73 75 73 70 5F 7A 65 6E 5F 70 69 63 6B 20 6F 66 66 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: susp_zen_pick=0
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt joc on on
[2026-03-23 05:48:21] [DEBUG] Command //set_stealth_opt: opt=susp_zen_pick value=off
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt jog on on
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=50 | C1 32 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 73 74 65 61 6C 74 68 5F 6F 70 74 20 73 75 73 70 5F 6D 6F 76 65 5F 70 69 63 6B 20 6F 66 66 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: susp_move_pick=0
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt exl on on
[2026-03-23 05:48:21] [DEBUG] Command //set_stealth_opt: opt=susp_move_pick value=off
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt zen on on
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=45 | C1 2D 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 73 74 65 61 6C 74 68 5F 6F 70 74 20 73 75 73 70 5F 70 69 63 6B 20 6F 66 66 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: susp_pick=0
[2026-03-23 05:48:21] [CLICKER] Sending to DLL: //set_pick_opt custom on on
[2026-03-23 05:48:21] [DEBUG] Command //set_stealth_opt: opt=susp_pick value=off
[2026-03-23 05:48:21] [CLICKER] Sending //pick clear to DLL
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=40 | C1 28 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 62 6C 65 73 73 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: bless flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [CLICKER] Sending custom pick command: //pick 14 51
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=bless pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [CLICKER] SetAdvancedAutopickOptions: all pickup options sent
[2026-03-23 05:48:21] [CLICKER] Sending //autopick on to DLL
[2026-03-23 05:48:21] [CLICKER] Sending //pickclass 5 to DLL
[2026-03-23 05:48:21] [CLICKER] Auto run mode enabled (boots level detection)
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=39 | C1 27 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 73 6F 75 6C 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [CLICKER] Pick-up run mode enabled
[2026-03-23 05:48:21] [PICKUP] SetParam: soul flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=soul pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [CLICKER] Pick-up debug logging enabled
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=40 | C1 28 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 63 68 61 6F 73 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [CLICKER] === AVANTA+ELITE mode active features ===
[2026-03-23 05:48:21] [PICKUP] SetParam: chaos flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [CLICKER]   AVANTA: AutoPick(SPACE)=off AutoHeal=ON AutoRepair=ON AutoReqOff=off
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=chaos pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [CLICKER]   ELITE:  AdvPick(DLL)=ON Exit400=off AntiAFK=0x0 PickRun=ON Class=5
[2026-03-23 05:48:21] [CLICKER] === Auto-clicker initialization complete ===
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=38 | C1 26 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 6A 6F 6C 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 05:48:21] [PICKUP] SetParam: jol flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=jol pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=38 | C1 26 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 6A 6F 63 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: joc flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=joc pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=38 | C1 26 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 6A 6F 67 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: jog flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=jog pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=38 | C1 26 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 65 78 6C 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: exl flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=exl pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=38 | C1 26 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 7A 65 6E 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: zen flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=zen pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=41 | C1 29 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 73 65 74 5F 70 69 63 6B 5F 6F 70 74 20 63 75 73 74 6F 6D 20 6F 6E 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: custom flags 0x00 -> 0x03 (pick=1, moveTo=1)
[2026-03-23 05:48:21] [PICKUP] Command //set_pick_opt: item=custom pick=on moveTo=on (flags=0x03)
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=26 | C1 1A 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 70 69 63 6B 20 63 6C 65 61 72 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: pick list cleared (was 0 items)
[2026-03-23 05:48:21] [PICKUP] Command //pick clear: pick list cleared
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=36 | C1 24 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 70 69 63 6B 20 63 6C 65 61 72 20 5B 4F 4B 5D 2E...
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=26 | C1 1A 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 70 69 63 6B 20 31 34 20 35 31 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: pick item code=0x0E33 mask=0xFFFF (total pick items=1)
[2026-03-23 05:48:21] [PICKUP] Command //pick: code=0x0E33 mask=0xFFFF (args: 14 51 )
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=36 | C1 24 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 70 69 63 6B 20 31 34 20 35 31 20 5B 4F 4B 5D 2E...
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=27 | C1 1B 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 61 75 74 6F 70 69 63 6B 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: autopick ENABLED (m_fEnabled=1)
[2026-03-23 05:48:21] [PICKUP] Command //autopick: on (fCmd=1)
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=37 | C1 25 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 61 75 74 6F 70 69 63 6B 20 6F 6E 20 5B 4F 4B 5D...
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=27 | C1 1B 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 70 69 63 6B 63 6C 61 73 73 20 35 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: charclass=5
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=53 | C1 35 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 50 69 63 6B 2D 75 70 20 63 68 61 72 20 63 6C 61...
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Update position CTS | Len=6 | C1 06 1D 1E D0 10 
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Massive skill | Len=17 | C3 11 07 1E 38 1E D0 04 87 83 FF FF 00 2E 7A 39 5D 
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=34 | C1 22 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 70 69 63 6B 61 75 74 6F 72 75 6E 6D 6F 64 65 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: autorunmode=1
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=50 | C1 32 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 50 69 63 6B 2D 75 70 20 61 75 74 6F 20 72 75 6E...
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=30 | C1 1E 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 70 69 63 6B 72 75 6E 6D 6F 64 65 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: pickrunmode=1
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=45 | C1 2D 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 50 69 63 6B 2D 75 70 20 72 75 6E 20 6D 6F 64 65...
[2026-03-23 05:48:21] [PACKET] CLIENT -> SERVER | Character speak | Len=28 | C1 1C 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 70 69 63 6B 64 65 62 75 67 20 6F 6E 00 
[2026-03-23 05:48:21] [PICKUP] SetParam: pickdebug=1
[2026-03-23 05:48:21] [PACKET] DIRECT>CLIENT | Server message | Len=47 | C1 2F 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 50 69 63 6B 2D 75 70 20 64 65 62 75 67 20 6D 6F...
[2026-03-23 05:48:21] [PACKET] SERVER -> CLIENT | Object death | Len=9 | C1 09 17 00 DE 00 38 39 81 
[2026-03-23 05:48:21] [PACKET] SERVER -> CLIENT | Server message | Len=30 | C1 1E 0D 01 BC BE 40 37 B9 A4 E0 00 00 4D 61 78 69 6D 75 6D 20 4C 65 76 65 6C 00 37 C0 C6 
[2026-03-23 05:48:21] [PACKET] SERVER -> CLIENT | Server message | Len=30 | C1 1E 0D 01 BC BE 40 37 B9 A4 E0 00 00 4D 61 78 69 6D 75 6D 20 4C 65 76 65 6C 00 37 C0 C6 
[2026-03-23 05:48:21] [PACKET] SERVER -> CLIENT | Server message | Len=30 | C1 1E 0D 01 BC BE 40 37 B9 A4 E0 00 00 4D 61 78 69 6D 75 6D 20 4C 65 76 65 6C 00 37 C0 C6 
[2026-03-23 05:48:21] [PACKET] SERVER -> CLIENT | Server message | Len=30 | C1 1E 0D 01 BC BE 40 37 B9 A4 E0 00 00 4D 61 78 69 6D 75 6D 20 4C 65 76 65 6C 00 37 C0 C6 
[2026-03-23 05:48:21] [PACKET] SERVER -> CLIENT | Server message | Len=30 | C1 1E 0D 01 10 C1 58 38 10 00 00 00 33 4D 61 78 69 6D 75 6D 20 4C 65 76 65 6C 00 00 00 00 
[2026-03-23 05:48:21] [PACKET] SERVER -> CLIENT | Server message | Len=30 | C1 1E 0D 01 10 C1 58 38 10 00 00 00 33 4D 61 78 69 6D 75 6D 20 4C 65 76 65 6C 00 00 00 00 
[2026-03-23 05:48:21] [PACKET] SERVER -> CLIENT | Server message | Len=30 | C1 1E 0D 01 10 C1 58 38 10 00 00 00 33 4D 61 78 69 6D 75 6D 20 4C 65 76 65 6C 00 00 00 00 
[2026-03-23 05:48:22] [PACKET] CLIENT -> SERVER | Update position CTS | Len=6 |   (throttled: 6 similar packets skipped)
[2026-03-23 05:48:22] [PACKET] CLIENT -> SERVER | Massive skill | Len=17 | C3 11 0E 1E 38 1E D0 5F 88 DE FF FF 00 7D 20 39 5D  (throttled: 6 similar packets skipped)
[2026-03-23 05:48:22] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 05:48:22] [PACKET] SERVER -> CLIENT | Server message | Len=42 | C1 2A 0D 01 00 00 00 00 00 00 00 00 00 52 61 67 65 20 45 78 70 3A 20 34 33 30 30 30 20 4F 62 74 61 69 6E 64 65 64 00 00 00 00 
[2026-03-23 05:48:22] [PACKET] SERVER -> CLIENT | Server message | Len=42 | C1 2A 0D 01 B0 8D 58 00 00 00 00 00 00 52 61 67 65 20 45 78 70 3A 20 34 33 30 30 30 20 4F 62 74 61 69 6E 64 65 64 00 00 00 00 
[2026-03-23 05:48:22] [PACKET] SERVER -> CLIENT | Server message | Len=42 | C1 2A 0D 01 B0 8D 58 00 00 00 00 00 00 52 61 67 65 20 45 78 70 3A 20 34 33 30 30 30 20 4F 62 74 61 69 6E 64 65 64 00 00 00 00 
[2026-03-23 05:48:22] [PACKET] SERVER -> CLIENT | Server message | Len=42 | C1 2A 0D 01 30 5D 5B 30 5D 5B 30 5D 20 52 61 67 65 20 45 78 70 3A 20 34 33 30 30 30 20 4F 62 74 61 69 6E 64 65 64 00 00 78 00 
[2026-03-23 05:48:22] [PACKET] SERVER -> CLIENT | Server message | Len=42 | C1 2A 0D 01 30 5D 5B 30 5D 20 45 78 3A 52 61 67 65 20 45 78 70 3A 20 34 33 30 30 30 20 4F 62 74 61 69 6E 64 65 64 00 00 78 00 
[2026-03-23 05:48:22] [PACKET] SERVER -> CLIENT | Server message | Len=42 | C1 2A 0D 01 08 C2 A1 55 90 EE 19 00 00 52 61 67 65 20 45 78 70 3A 20 34 33 30 30 30 20 4F 62 74 61 69 6E 64 65 64 00 00 78 00 
[2026-03-23 05:48:22] [PACKET] SERVER -> CLIENT | Server message | Len=42 | C1 2A 0D 01 00 00 00 00 00 00 00 00 00 52 61 67 65 20 45 78 70 3A 20 34 33 30 30 30 20 4F 62 74 61 69 6E 64 65 64 00 00 00 00 
[2026-03-23 05:48:23] [PACKET] CLIENT -> SERVER | Update position CTS | Len=6 | C1 06 1D 1E D0 40  (throttled: 10 similar packets skipped)
[2026-03-23 05:48:23] [PACKET] CLIENT -> SERVER | Massive skill | Len=17 | C3 11 19 1E 38 1E D0 5F 88 DE FF FF 00 2C 71 39 5D  (throttled: 10 similar packets skipped)
[2026-03-23 05:48:23] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 05:48:23] [PACKET] SERVER -> CLIENT | Meet item(s) | Len=71 | C2 00 47 20 06 80 00 20 D5 0F 03 E4 00 E0 E0 00 80 01 1F CC 0F 04 11 00 06 E0 00 80 02 22 CB 10 38 80 00 00 D0 00 80 03 20 CD 1D 28 01 00 00 E0 00 80 04 1E CF 0B 00 00 00 00 F0 00 80 05 1F D0 1D 28 01 00 00 E0 00 
[2026-03-23 05:48:23] [PICKUP] RECV CMeetItemPacket: 6 item(s) dropped on ground | Len=71 | C2 00 47 20 06 80 00 20 D5 0F 03 E4 00 E0 E0 00 80 01 1F CC 0F 04 11 00 06 E0 00 80 02 22 CB 10 38 80 00 00 D0 00 80 03 20 CD 1D 28 01 00 00 E0 00 80 04 1E CF 0B 00 00 00 00 F0 00 80 05 1F D0 1D 28 01 00 00 E0 00 
[2026-03-23 05:48:23] [PICKUP] Zen item 0x0E0F (id=0x0001) at (31,204) dist=(1,4) queued for Move To pickup
[2026-03-23 05:48:23] [PICKUP] Zen item 0x0E0F (id=0x0000) at (32,213) dist=(2,5) queued for Move To pickup
[2026-03-23 05:48:23] [PICKUP] GoPickNextItem: dequeued item 0x0001 at (31,204), suspended=0, suspMove=0
[2026-03-23 05:48:24] [PICKUP] Auto run mode: class=5, boots=7, wings=0, pet=0 -> RUN
[2026-03-23 05:48:24] [PICKUP] FilterSendPacket: BLOCKED Update position CTS during auto-pickup walk (throttled: 9 similar packets skipped)
[2026-03-23 05:48:24] [PACKET] CLIENT -> SERVER | Massive skill | Len=17 | C3 11 23 1E 38 1E D0 5F 88 DE FF FF 00 9B C6 39 5D  (throttled: 12 similar packets skipped)
[2026-03-23 05:48:24] [PICKUP] GoPickNextItem: RUNNING to item 0x0001 at (31,204) from (30,208)
[2026-03-23 05:48:24] [PICKUP] RunTo: from (30,208) to (31,204), playerId=0x3981
[2026-03-23 05:48:24] [PICKUP] RunTo step 1: RECV STC | C1 07 D6 39 81 1F CF  | SEND CTS | C1 06 1D 1F CF 00  | pos=(31,207)
[2026-03-23 05:48:24] [PACKET] SERVER -> CLIENT | Update position STC | Len=7 | C1 07 D6 39 81 1F CF 
[2026-03-23 05:48:24] [PICKUP] RunTo step 2: pos=(31,206)
[2026-03-23 05:48:24] [PICKUP] RunTo step 3: pos=(31,205)
[2026-03-23 05:48:24] [PICKUP] RunTo step 4: RECV STC | C1 07 D6 39 81 1F CC  | SEND CTS | C1 06 1D 1F CC 00  | pos=(31,204)
[2026-03-23 05:48:24] [PICKUP] RunTo: completed in 4 steps (200 ms)
[2026-03-23 05:48:24] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 05:48:24] [PICKUP] SEND CPickItemPacket: picking up item ID=0x0001 (walking=1) | Len=7 | C3 07 00 22 00 01 71 
[2026-03-23 05:48:24] [PACKET] CLIENT -> SERVER | Pick item | Len=7 | C3 07 00 22 00 01 71 
[2026-03-23 05:48:25] [PICKUP] FilterSendPacket: BLOCKED Update position CTS during auto-pickup walk (throttled: 15 similar packets skipped)
[2026-03-23 05:48:25] [PACKET] CLIENT -> SERVER | Massive skill | Len=17 | C3 11 2F 1E 38 1F CC 3E 98 BD FF FF 00 5B 09 39 5D  (throttled: 23 similar packets skipped)
[2026-03-23 05:48:25] [PICKUP] GoPickNextItem: RUNNING back to original position (30,208)
[2026-03-23 05:48:25] [PICKUP] RunTo: from (31,204) to (30,208), playerId=0x3981
[2026-03-23 05:48:25] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 05:48:25] [PICKUP] RunTo step 1: RECV STC | C1 07 D6 39 81 1E CD  | SEND CTS | C1 06 1D 1E CD 00  | pos=(30,205)
[2026-03-23 05:48:25] [PACKET] SERVER -> CLIENT | Update position STC | Len=7 |   (throttled: 3 similar packets skipped)
[2026-03-23 05:48:25] [PICKUP] RunTo step 2: pos=(30,206)
[2026-03-23 05:48:25] [PICKUP] RunTo step 3: pos=(30,207)
[2026-03-23 05:48:25] [PICKUP] RunTo step 4: RECV STC | C1 07 D6 39 81 1E D0  | SEND CTS | C1 06 1D 1E D0 00  | pos=(30,208)
[2026-03-23 05:48:25] [PACKET] SERVER -> CLIENT | Forget item(s) | Len=7 | C2 00 07 21 01 00 01 
[2026-03-23 05:48:25] [PICKUP] RECV CForgetItemPacket: 1 item(s) disappeared from ground | Len=7 | C2 00 07 21 01 00 01 
[2026-03-23 05:48:25] [PICKUP] RunTo: completed in 4 steps (200 ms)
[2026-03-23 05:48:26] [PICKUP] FilterSendPacket: BLOCKED Update position CTS during auto-pickup walk (throttled: 15 similar packets skipped)
[2026-03-23 05:48:26] [PACKET] CLIENT -> SERVER | Massive skill | Len=17 | C3 11 3B 1E 38 1E CF 3E 99 BD FF FF 00 DB 8A 39 5D  (throttled: 23 similar packets skipped)
[2026-03-23 05:48:26] [PICKUP] GoPickNextItem: dequeued item 0x0000 at (32,213), suspended=0, suspMove=0
[2026-03-23 05:48:26] [CLICKER] Attack: right-click at (400,256)
[2026-03-23 05:48:26] [PICKUP] GoPickNextItem: RUNNING to item 0x0000 at (32,213) from (30,207)
[2026-03-23 05:48:26] [PICKUP] RunTo: from (30,207) to (32,213), playerId=0x3981
[2026-03-23 05:48:26] [PICKUP] RunTo step 1: RECV STC | C1 07 D6 39 81 1F D0  | SEND CTS | C1 06 1D 1F D0 00  | pos=(31,208)
[2026-03-23 05:48:26] [PACKET] SERVER -> CLIENT | Update position STC | Len=7 |   (throttled: 3 similar packets skipped)
[2026-03-23 05:48:26] [PICKUP] RunTo step 2: pos=(32,209)
[2026-03-23 05:48:26] [PICKUP] RunTo step 3: pos=(32,210)
[2026-03-23 05:48:26] [PICKUP] RunTo step 4: pos=(32,211)
[2026-03-23 05:48:26] [PICKUP] RunTo step 5: pos=(32,212)
[2026-03-23 05:48:26] [PICKUP] RunTo step 6: RECV STC | C1 07 D6 39 81 20 D5  | SEND CTS | C1 06 1D 20 D5 00  | pos=(32,213)
[2026-03-23 05:48:27] [PICKUP] RunTo: completed in 6 steps (300 ms)
[2026-03-23 05:48:27] [PICKUP] FilterSendPacket: BLOCKED Update position CTS during auto-pickup walk (throttled: 18 similar packets skipped)
[2026-03-23 05:48:27] [PACKET] CLIENT -> SERVER | Massive skill | Len=17 | C3 11 48 1E 38 20 D5 3E 86 BD FF FF 00 B1 DB 39 5D  (throttled: 25 similar packets skipped)
[2026-03-23 05:48:27] [CLICKER] Clicker stop requested
[2026-03-23 05:48:27] [CLICKER] === Auto-clicker stopping ===
[2026-03-23 05:48:27] [CLICKER] Advanced auto-pickup disabled
[2026-03-23 05:48:27] [CLICKER] Sending //pickdebug off and //autopick off to DLL
[2026-03-23 05:48:27] [CLICKER] Debug mode remains active in DLL after clicker stop
[2026-03-23 05:48:27] [PACKET] CLIENT -> SERVER | Character speak | Len=29 | C1 1D 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 70 69 63 6B 64 65 62 75 67 20 6F 66 66 00 
[2026-03-23 05:48:27] [PICKUP] SetParam: pickdebug=0
[2026-03-23 05:48:27] [PACKET] DIRECT>CLIENT | Server message | Len=48 | C1 30 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 50 69 63 6B 2D 75 70 20 64 65 62 75 67 20 6D 6F...
[2026-03-23 05:48:27] [PACKET] CLIENT -> SERVER | Character speak | Len=28 | C1 1C 00 41 44 4D 49 4E 00 00 00 00 00 2F 2F 61 75 74 6F 70 69 63 6B 20 6F 66 66 00 
[2026-03-23 05:48:27] [PICKUP] SetParam: autopick DISABLED (m_fEnabled=0)
[2026-03-23 05:48:27] [PICKUP] Command //autopick: off (fCmd=0)
[2026-03-23 05:48:27] [PACKET] DIRECT>CLIENT | Server message | Len=38 | C1 26 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 61 75 74 6F 70 69 63 6B 20 6F 66 66 20 5B 4F 4B...
[2026-03-23 05:48:27] [CLICKER] === Auto-clicker stopped ===
[2026-03-23 05:48:27] [CLICKER] Clicker stopped
[2026-03-23 05:48:27] [PICKUP] SEND CPickItemPacket: picking up item ID=0x0000 (walking=1) | Len=7 | C3 07 00 22 00 00 A2 
[2026-03-23 05:48:27] [PACKET] CLIENT -> SERVER | Pick item | Len=7 | C3 07 00 22 00 00 A2 
[2026-03-23 05:48:27] [PACKET] SERVER -> CLIENT | Forget item(s) | Len=9 | C2 00 09 21 02 00 00 00 02 
[2026-03-23 05:48:27] [PICKUP] RECV CForgetItemPacket: 2 item(s) disappeared from ground | Len=9 | C2 00 09 21 02 00 00 00 02 
[2026-03-23 05:48:28] [PICKUP] GoPickNextItem: RUNNING back to original position (30,207)
[2026-03-23 05:48:28] [PICKUP] RunTo: from (32,213) to (30,207), playerId=0x3981
[2026-03-23 05:48:28] [PICKUP] RunTo step 1: RECV STC | C1 07 D6 39 81 1F D4  | SEND CTS | C1 06 1D 1F D4 00  | pos=(31,212)
[2026-03-23 05:48:28] [PACKET] CLIENT -> SERVER | Update position CTS | Len=6 | C1 06 1D 1F D4 00 
[2026-03-23 05:48:28] [PACKET] SERVER -> CLIENT | Update position STC | Len=7 | C1 07 D6 39 81 1F D4  (throttled: 5 similar packets skipped)
[2026-03-23 05:48:28] [PICKUP] RunTo step 6: RECV STC | C1 07 D6 39 81 1E CF  | SEND CTS | C1 06 1D 1E CF 00  | pos=(30,207)
[2026-03-23 05:48:28] [PICKUP] RunTo: completed in 6 steps (300 ms)
[2026-03-23 05:48:28] [PACKET] SERVER -> CLIENT | Meet item(s) | Len=16 | C2 00 10 20 01 00 02 22 CB 10 38 80 00 00 D0 00 
[2026-03-23 05:48:28] [PICKUP] RECV CMeetItemPacket: 1 item(s) dropped but autopick DISABLED (m_fEnabled=0) - items ignored
[2026-03-23 05:48:37] [SOCKET] IP override from Connect.ini: 192.168.0.105
[2026-03-23 05:48:37] [SOCKET] connect() socket=2216 IP=192.168.0.105 port=55901
[2026-03-23 05:48:37] [PACKET] Packet logger filter destroyed - monitoring stopped
[2026-03-23 05:48:37] [PACKET] Packet logger filter initialized - AVANTA+ELITE relevant packets only
[2026-03-23 05:48:37] [PACKET] SERVER -> CLIENT | Server message | Len=56 | C1 38 0D 01 00 00 00 00 00 00 00 00 00 5B 50 68 6F 65 6E 69 78 5D 20 57 65 6C 6C 63 6F 6D 65 20 74 6F 20 50 68 6F 65 6E 69 78 20 56 69 70 20 41 44 4D 49 4E 00 00 00 00 
[2026-03-23 05:48:37] [PACKET] SERVER -> CLIENT | Server message | Len=49 | C1 31 0D 01 00 00 00 00 00 00 00 00 00 5B 50 68 6F 65 6E 69 78 5D 20 50 6C 61 79 65 72 73 20 4F 6E 6C 69 6E 65 3A 20 33 39 2F 33 30 30 00 20 41 44 
[2026-03-23 05:48:37] [PACKET] SERVER -> CLIENT | Server message | Len=49 | C1 31 0D 01 00 00 00 00 00 5B 50 68 6F 5B 50 68 6F 65 6E 69 78 5D 20 48 6F 6D 65 20 50 61 67 65 3A 20 50 68 6F 65 6E 69 78 2E 63 6F 6D 00 49 4E 00 
[2026-03-23 05:48:37] [PACKET] SERVER -> CLIENT | Character stats | Len=81 | C3 51 00 F3 03 1E D0 06 00 F0 59 D1 E3 90 05 FF E7 BE 02 FF 7F FF 7F FF 7F FF 7F 1A 0D D8 16 A2 16 A2 16 2C 8C 2C 8C 99 39 32 73 00 00 6C AE F2 17 03 20 69 F7 59 00 00 00 00 00 59 00 05 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
[2026-03-23 05:48:37] [PACKET] DIRECT>CLIENT | Server message | Len=37 | C1 25 0D 01 00 00 00 00 00 00 00 00 00 3E 3E 20 6D 65 65 74 20 70 6C 61 79 65 72 20 41 44 4D 49...
[2026-03-23 05:48:37] [PICKUP] SetParam: suspended=1
[2026-03-23 05:48:37] [PACKET] SERVER -> CLIENT | Server message | Len=69 | C1 45 0D 01 00 00 00 00 00 00 00 00 00 5B 4C 75 63 6B 79 20 57 68 65 65 6C 5D 3A 20 44 6F 6E 27 74 20 66 6F 72 67 65 74 20 79 6F 75 20 6E 65 65 64 20 73 70 61 63 65 20 66 6F 72 20 69 74 65 6D 73 00 00 00 00 
[2026-03-23 05:48:37] [PACKET] SERVER -> CLIENT | Character setup | Len=19 | C1 13 F3 30 00 38 00 00 00 00 00 00 00 00 09 00 04 08 04 
[2026-03-23 05:48:38] [PACKET] SERVER -> CLIENT | Meet item(s) | Len=49 | C2 00 31 20 04 00 02 22 CB 10 38 80 00 00 D0 00 00 03 20 CD 1D 28 01 00 00 E0 00 00 04 1E CF 0B 00 00 00 00 F0 00 00 05 1F D0 1D 28 01 00 00 E0 00 
[2026-03-23 05:48:38] [PICKUP] RECV CMeetItemPacket: 4 item(s) dropped but autopick DISABLED (m_fEnabled=0) - items ignored
[2026-03-23 05:48:39] [HOOK] DLL_PROCESS_DETACH - main.exe unloading LordOfMU DLL, removing hooks
[2026-03-23 05:48:39] [DEBUG] LordOfMU DLL shutting down (OnExit)
[2026-03-23 05:48:39] [HOOK] DLL shutdown initiated - starting hook removal from main.exe
[2026-03-23 05:48:39] [DEBUG] Debug mode handler uninstalled
[2026-03-23 05:48:39] [HOOK] Crash handler uninstalled
[2026-03-23 05:48:39] [HOOK] Starting DLL hook cleanup from main.exe process
[2026-03-23 05:48:39] [HOOK] Proxifier cleanup started - removing hooks
[2026-03-23 05:48:39] [HOOK] Proxifier cleanup complete - all hooks removed
[2026-03-23 05:48:39] [HOOK] DLL hook cleanup complete - all hooks removed, g_lHookApplied reset to 0
[2026-03-23 05:48:39] [DEBUG] LordOfMU DLL cleanup complete - all resources released
