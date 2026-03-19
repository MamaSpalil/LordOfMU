# PROMPT.md — Подробный промт для исправления ошибок в DLL-кликере LordOfMU для MuOnline

## Описание проекта

**LordOfMU** — это C++ проект DLL-кликера/инжектора для игры MU Online, состоящий из 5 подпроектов:

1. **LordOfMUdll** (`src/LordOfMUdll/`) — Основная DLL-библиотека с прокси-фильтрами пакетов, перехватом WinSock API (WSASend/WSARecv), шифрованием/дешифрованием пакетов C3/C4, автокиллом, автопикапом, мультихитом, скоростным перемещением и скриптовым движком.
2. **AutoClicker** (`src/LordOfClicker/`) — DLL автокликера с поддержкой различных классов персонажей (Dark Knight, Dark Lord, Magic Gladiator, Summoner Mage, Archer Elf, Energy Elf).
3. **LoaderDll** (`src/LoaderDll/`) — DLL-загрузчик для инжекции основной библиотеки в процесс игры.
4. **Bootstrapper** (`src/Bootstrapper/`) — DLL для установки хуков через `SetWindowsHookEx` и управления множественными экземплярами клиента.
5. **LordOfMU** (`src/LordOfMU/`) — Исполняемый файл-лаунчер (EXE).

**Общие модули** (`src/_Shared/`): ApiHooker, EncDec (шифрование), Disasm (дизассемблер x86), CommonPackets, Packet, PacketParser, MD5, base64, intercept, WinHttpClient, FileTransfer, PEUtil и др.

**Всего:** ~149 исходных файлов, ~22 000 строк кода на C++03/C++98.

---

## Целевая среда компиляции

- **IDE:** Microsoft Visual Studio 2010 (Platform Toolset v100)
- **ОС:** Windows 10 x86 / x64
- **Язык:** C++03/C++98 (без C++11 фич)
- **Архитектура:** Win32 (x86)
- **Конфигурации сборки:**
  - `Debug|Win32` — отладочная сборка с `__INCLUDE_ALL_STUFF__`
  - `Release [Clicker]|Win32` — кликер с `__CLICKER_STUFF__`
  - `Release [Hacker]|Win32` — полная версия с `__INCLUDE_ALL_STUFF__`
  - `Release [Antihack]|Win32` — античит версия с `__ANTIHACK_STUFF__`

---

## ПРОМТ ДЛЯ ИСПРАВЛЕНИЯ ОШИБОК КОМПИЛЯЦИИ

### Задача

Исправить все ошибки компиляции в проекте LordOfMU, чтобы он полностью собирался в Visual Studio 2010 (v100 toolset) на Windows 10 x86/x64 во всех четырёх конфигурациях (Debug, Release [Clicker], Release [Hacker], Release [Antihack]).

---

### 1. КРИТИЧЕСКИЕ ОШИБКИ В .vcxproj ФАЙЛАХ

#### 1.1. ToolsVersion="Current" → ToolsVersion="4.0"

**Проблема:** Все 5 файлов `.vcxproj` содержат `ToolsVersion="Current"` (формат VS2022+), который не распознаётся Visual Studio 2010.

**Файлы для исправления (строка 2 в каждом файле):**
- `src/LordOfMUdll/LordOfMUdll.vcxproj`
- `src/LordOfMU/LordOfMU.vcxproj`
- `src/LoaderDll/LoaderDll.vcxproj`
- `src/LordOfClicker/AutoClicker.vcxproj`
- `src/Bootstrapper/Bootstrapper.vcxproj`

**Было:**
```xml
<Project DefaultTargets="Build" ToolsVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
```

**Должно быть:**
```xml
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
```

#### 1.2. VCProjectVersion 17.0 → убрать или поставить 10.0

**Проблема:** Все vcxproj файлы содержат `<VCProjectVersion>17.0</VCProjectVersion>` (VS2022).

**В каждом .vcxproj файле (строка ~22):**

**Было:**
```xml
<VCProjectVersion>17.0</VCProjectVersion>
```

**Исправить на (или удалить целиком — VS2010 не использует этот тег):**
```xml
<!-- Удалить строку VCProjectVersion или заменить на: -->
```

#### 1.3. PlatformToolset v143 → v100

**Проблема:** Ряд конфигураций используют `<PlatformToolset>v143</PlatformToolset>` (Visual Studio 2022), который недоступен в VS2010.

**Нужно заменить все `v143` на `v100` в следующих файлах и конфигурациях:**

| Файл | Конфигурация | Строка |
|------|-------------|--------|
| `src/LordOfMUdll/LordOfMUdll.vcxproj` | Release [Antihack] | ~36 |
| `src/LordOfMU/LordOfMU.vcxproj` | Release [Antihack] | ~36 |
| `src/LordOfMU/LordOfMU.vcxproj` | Release [Hacker] | ~42 |
| `src/LoaderDll/LoaderDll.vcxproj` | Release [Antihack] | ~36 |
| `src/LoaderDll/LoaderDll.vcxproj` | Release [Hacker] | ~42 |
| `src/LordOfClicker/AutoClicker.vcxproj` | Release [Hacker] | ~36 |
| `src/LordOfClicker/AutoClicker.vcxproj` | Release [Antihack] | ~42 |
| `src/Bootstrapper/Bootstrapper.vcxproj` | Release [Antihack] | ~36 |
| `src/Bootstrapper/Bootstrapper.vcxproj` | Release [Hacker] | ~42 |

**Было:** `<PlatformToolset>v143</PlatformToolset>`
**Должно быть:** `<PlatformToolset>v100</PlatformToolset>`

---

### 2. КРИТИЧЕСКАЯ ОШИБКА В ФАЙЛЕ РЕШЕНИЯ (.sln)

**Файл:** `LordOfMU.sln`

**Проблема:** Формат решения указан как VS2022 (Format Version 12.00, Visual Studio Version 17).

**Было (строки 2-5):**
```
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 17
VisualStudioVersion = 17.6.33801.468
MinimumVisualStudioVersion = 10.0.40219.1
```

**Должно быть:**
```
Microsoft Visual Studio Solution File, Format Version 11.00
# Visual Studio 2010
```

> **Примечание:** В VS2010 в .sln файле НЕ используются строки `VisualStudioVersion` и `MinimumVisualStudioVersion` — их нужно удалить.

---

### 3. ОБНОВЛЕНИЕ WINVER И _WIN32_WINNT ДЛЯ WINDOWS 10

**Проблема:** Во всех файлах `stdafx.h` определены `WINVER` и `_WIN32_WINNT` как `0x0501` (Windows XP). Для корректной работы на Windows 10 рекомендуется обновить.

**Файлы для исправления:**
- `src/LordOfMUdll/stdafx.h` (строки 22-27)
- `src/LordOfMU/stdafx.h` (строки 13-18)
- `src/LoaderDll/stdafx.h` (строки 12-17)
- `src/LordOfClicker/stdafx.h` (строки 13-18)
- `src/Bootstrapper/stdafx.h` (строки 13-18)

**Было:**
```cpp
#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
```

**Рекомендуемое значение для Windows 10:**
```cpp
#ifndef WINVER
#define WINVER 0x0601          // Windows 7 (минимально поддерживаемая VS2010)
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601    // Windows 7
#endif
```

> **Важно:** VS2010 SDK не содержит заголовков для Windows 10 (0x0A00). Максимальное значение, поддерживаемое v100 toolset — `0x0601` (Windows 7). Значение `0x0501` (XP) тоже работает на Win10 благодаря обратной совместимости, но ограничивает доступные API.

---

### 4. ПРЕДУПРЕЖДЕНИЯ О НЕБЕЗОПАСНЫХ ФУНКЦИЯХ CRT

**Проблема:** В коде используются небезопасные функции `strcpy()` и `sprintf()` (~46 мест). Проект уже определяет `_CRT_SECURE_NO_DEPRECATE` для подавления предупреждений, но для безопасности рекомендуется заменить:

**Основные места использования `strcpy()`:**
- `src/LordOfMUdll/AutosayParser.cpp:80`
- `src/LordOfMUdll/AutosayFilter.cpp:259`
- `src/LordOfMUdll/CharInfoFilter.cpp:303`
- `src/LordOfMUdll/GameCommands.cpp:1049`
- `src/LordOfMUdll/ScriptParser.h:38`
- `src/LordOfMUdll/ScriptProcessorFilter.cpp:216`
- `src/LordOfMUdll/RecordRoutePacketFilter.cpp:78`
- `src/_Shared/PacketType.cpp:27,102`
- `src/_Shared/Disasm.cpp` — множественные вызовы

**Основные места использования `sprintf()`:**
- `src/LordOfMUdll/GameCommands.cpp:759,790`
- `src/LordOfMUdll/Proxifier.cpp:471` (`wsprintf`)
- `src/_Shared/FileTransfer.cpp:313,349,379,416`
- `src/_Shared/Disasm.cpp` — множественные вызовы

**Рекомендация:** Замена `strcpy` → `strcpy_s`, `sprintf` → `sprintf_s` для предотвращения переполнения буфера. Однако с определённым `_CRT_SECURE_NO_DEPRECATE` код скомпилируется и без замен.

---

### 5. ПОТЕНЦИАЛЬНЫЕ ПРОБЛЕМЫ СОВМЕСТИМОСТИ

#### 5.1. Общий сегмент данных (#pragma data_seg)

**Файлы:**
- `src/LoaderDll/LoaderDll.cpp` (строки ~12-18)
- `src/Bootstrapper/Bootstrapper.cpp` (строки ~13-18)

Используют `#pragma data_seg(".shared")` для разделяемой памяти между экземплярами DLL. Это работает в VS2010, но требует согласованности toolset между всеми DLL.

#### 5.2. API-хуки и x86 дизассемблер

**Файлы:**
- `src/_Shared/ApiHooker.h/cpp` — инлайн-хуки функций
- `src/_Shared/Disasm.h/cpp` — x86 дизассемблер
- `src/_Shared/Dsasm_Functions.cpp` — функции дизассемблера

Содержат низкоуровневый код работы с памятью и патчинга функций. При компиляции v100 для x86 должно работать корректно.

#### 5.3. ATL/MFC зависимости

Проект использует ATL (Active Template Library):
- `CAtlDllModuleT`, `CComObjectRootEx`, `CStringA`, `CStringW`
- Требуется наличие ATL в установке Visual Studio 2010

> **Важно:** Необходимо установить Visual Studio 2010 с компонентом ATL/MFC.

---

## СПИСОК ЧИТЕРСКИХ ФУНКЦИЙ В DLL

### Основные читерские модули (src/LordOfMUdll/)

| # | Модуль | Файлы | Описание | Условная компиляция |
|---|--------|-------|----------|---------------------|
| 1 | **AutoKill (Автокилл)** | `AutoKillFilter.h/cpp` | Автоматическое убийство монстров в радиусе действия с выбором скилла и настраиваемой задержкой. Очередь целей по дистанции. Отдельный поток выполнения. | `__CLICKER_STUFF__` |
| 2 | **AutoPickup (Автоподбор)** | `AutoPickupFilter.h/cpp` | Автоматический подбор предметов с земли с фильтрацией по типу (Bless, Soul, Chaos, Life Stone, Creation, Guardian). Поддержка телепортации к предмету. | `__CLICKER_STUFF__` |
| 3 | **FastMove (Быстрое перемещение)** | `FastMoveFilter.h/cpp` | Увеличение скорости перемещения путём подмены исходящих пакетов движения. Предсказание следующей позиции по вектору направления. Обход серверной проверки. | `__CLICKER_STUFF__` |
| 4 | **MultiHit (Мультихит)** | `MultihitPacketFilter.h/cpp` | Дублирование пакетов атаки для нанесения нескольких ударов за одну анимацию. Настраиваемое количество хитов и задержка между ними (по умолчанию 25мс). | `__CLICKER_STUFF__` |
| 5 | **Reflect (Отражение урона)** | `ReflectPacketFilter.h/cpp` | Автоматическая контратака монстров, наносящих урон игроку. Отслеживание ID атакующего и автоматический сброс при смерти цели. | `__CLICKER_STUFF__` |
| 6 | **Autosay (Автоответ в чат)** | `AutosayFilter.h/cpp`, `AutosayParser.h/cpp` | Автоматические ответы на входящие сообщения по шаблонам. Распознавание имён персонажей. Детекция спама и бессмысленных сообщений. | `__CLICKER_STUFF__` |
| 7 | **Script Engine (Скриптовый движок)** | `ScriptProcessorFilter.h/cpp`, `ScriptParser.h/cpp`, `ScriptCommands.h/cpp` | Выполнение предзаписанных скриптов: перемещение, атака, чат, задержки, циклы. Полный язык автоматизации ботов. | `__CLICKER_STUFF__` |
| 8 | **RecordRoute (Запись маршрута)** | `RecordRoutePacketFilter.h/cpp` | Запись пути перемещения игрока в файл для последующего воспроизведения (бот-фарминг). | `__CLICKER_STUFF__` |
| 9 | **Packet Decrypt/Encrypt** | `PacketDecryptFilter.h/cpp`, `PacketEncryptFilter.h/cpp` | Перехват и расшифровка/зашифровка сетевых пакетов C3/C4 протокола MU Online. | Все конфигурации |
| 10 | **Stealth Mode (Невидимость)** | `CharInfoFilter.h/cpp` | Режим скрытности — скрытие персонажа от других игроков. | `__CLICKER_STUFF__` |

### Инфраструктура читов

| # | Модуль | Файлы | Описание |
|---|--------|-------|----------|
| 11 | **API Hooking** | `_Shared/ApiHooker.h/cpp` | Перехват WinSock функций (WSASend, WSARecv, connect, closesocket) через inline-патчинг с трамплинами. |
| 12 | **IAT Hooking** | `_Shared/intercept.h/cpp` | Перехват через таблицу импорта (Import Address Table). |
| 13 | **x86 Disassembler** | `_Shared/Disasm.h/cpp`, `_Shared/Dsasm_Functions.cpp` | Дизассемблер для определения точек патчинга в памяти. |
| 14 | **Packet Proxy** | `Proxifier.h/cpp`, `GameProxy.h/cpp`, `ProxyConnection.h/cpp`, `ProxyEndpoint.h/cpp` | Полный прокси сетевого трафика с цепочкой фильтров. |
| 15 | **DLL Injection** | `LoaderDll.cpp`, `Bootstrapper.cpp` | Инжекция DLL в процесс игры через SetWindowsHookEx и CreateRemoteThread. |
| 16 | **EncDec (Шифрование)** | `_Shared/EncDec.h/cpp` | Реализация алгоритмов шифрования C3/C4 пакетов MU Online (XOR32, 8-to-11 byte encoding). |
| 17 | **File Transfer** | `_Shared/FileTransfer.h/cpp` | Передача файлов (только в `__HACK_STUFF__` сборке). |
| 18 | **Inventory Manager** | `InventoryManagerFilter.h/cpp` | Управление инвентарём: поиск, дроп, использование предметов программно. |
| 19 | **CharInfo Tracking** | `CharInfoFilter.h/cpp` | Отслеживание позиции (X, Y), уровня, ID игрока, списка партнёров/гильдии. |

### Команды управления читами (GameCommands.h/cpp — 43+ команд)

**Боевые команды:**
- `//autokill [on|off]` — включить/выключить автокилл
- `//autopick [on|off]` — включить/выключить автоподбор
- `//multihit [on|off]` — включить/выключить мультихит
- `//reflect [on|off]` — включить/выключить отражение урона
- `//fastmove [on|off]` — включить/выключить быстрое перемещение
- `//stealth [on|off]` — включить/выключить невидимость
- `//autosay [on|off]` — включить/выключить автоответы

**Настройки:**
- `//skill <id>` — выбрать скилл для автокилла
- `//hitdelay <ms>` — задержка между хитами мультихита
- `//hitdelayak <ms>` — задержка автокилла
- `//hitdist <dist>` — дистанция хитов
- `//pickdist <dist>` — дистанция подбора
- `//pickopt <options>` — настройки фильтров подбора (bless, soul, chaos, life, creation, guardian, zen)
- `//itemcode <code>` — фильтр по коду предмета

**Навигация:**
- `//goto <x> <y>` — телепорт на координаты
- `//route [record|stop|play]` — запись/воспроизведение маршрута
- `//run <route_file>` — запуск маршрута из файла
- `//script <file>` — выполнить скрипт

**Утилиты:**
- `//say <char> <msg>` — отправить сообщение
- `//whisp <char> <msg>` — шёпот
- `//autoexit` — авто-выход при условии
- `//suicide` — убить персонажа
- `//die` — вызвать смерть
- `//throw` — использование скилла
- `//drop` — дроп предмета
- `//moveitem` — перемещение предмета в инвентаре
- `//lahap` — специальный вызов
- `//loglevel <level>` — уровень логирования
- `//rcon <cmd>` — удалённая консоль (клиент)
- `//rconsv <cmd>` — удалённая консоль (сервер)
- `//fcopy <file>` — копирование файла (только Hacker build)
- `//fcopysv <file>` — серверное копирование файла (только Hacker build)
- `//test` — тестовая/отладочная команда

---

## АРХИТЕКТУРА ПРОЕКТА

```
Игровой клиент MU Online
    ↓ (DLL Injection через Bootstrapper/LoaderDll)
LordOfMUdll.dll
    ↓ (API Hooking: WSASend/WSARecv)
Proxifier → ProxyBuilder → GameProxy
    ↓
Цепочка фильтров (Filter Chain):
  1. CommandInterface      [CRITICAL] — обработка команд //
  2. PacketDecryptFilter   [CRITICAL] — расшифровка входящих пакетов
  3. PacketEncryptFilter   [CRITICAL] — зашифровка исходящих пакетов
  4. CharInfoFilter        [CRITICAL] — отслеживание состояния персонажа
  5. AutoKillFilter        [NORMAL]   — автоматическое убийство монстров
  6. AutoPickupFilter      [NORMAL]   — автоматический подбор предметов
  7. MultihitPacketFilter  [NORMAL]   — мультихит
  8. FastMoveFilter        [NORMAL]   — быстрое перемещение
  9. ReflectPacketFilter   [NORMAL]   — отражение урона
  10. AutosayFilter        [NORMAL]   — автоответы в чат
  11. ScriptProcessorFilter [NORMAL]  — скриптовый движок
  12. RecordRouteFilter    [NORMAL]   — запись маршрута
  13. InventoryManagerFilter [NORMAL] — управление инвентарём
  14. PacketLogger         [CRITICAL] — логирование пакетов
    ↓
Сервер MU Online
```

---

## ВЫХОДНЫЕ ФАЙЛЫ СБОРКИ

| Конфигурация | LordOfMUdll | AutoClicker | LoaderDll | Bootstrapper | LordOfMU |
|---|---|---|---|---|---|
| Debug | LordOfMU.dll | LordOfClicker.dll | LordOfLoader.dll | Bootstrapper.dll | LordOfMU.exe |
| Release [Clicker] | MUEliteClicker.dll | MUAutoClicker.dll | MUAutoLoader.dll | Bootstrapper.dll | LordOfMU.exe |
| Release [Hacker] | LordOfMU.dll | LordOfClicker.dll | LordOfLoader.dll | Bootstrapper.dll | LordOfMU.exe |
| Release [Antihack] | LordOfMU.dll | LordOfClicker.dll | LordOfLoader.dll + anti.dll | Bootstrapper.dll | LordOfMU.exe |

---

## ЗАВИСИМОСТИ

- **ATL (Active Template Library)** — CString, CAtlDllModuleT, CComObjectRootEx
- **WinSock2** (ws2_32.lib) — сетевое взаимодействие
- **version.lib** — информация о версии файлов (LordOfMUdll)
- **psapi.lib** — Process Status API (LordOfMUdll)
- **Comdlg32.lib** — диалоги Windows (Bootstrapper, LordOfMU)
- **winhttp.lib / wininet.lib** — HTTP-клиент (WinHttpClient)
