# LordOfMU

DLL-кликер и инжектор для MU Online.

## Структура проекта

- **src/LordOfMUdll/** — Основная DLL с прокси-фильтрами пакетов
- **src/LordOfClicker/** — DLL автокликера для различных классов персонажей
- **src/LoaderDll/** — DLL-загрузчик для инжекции
- **src/Bootstrapper/** — DLL для установки хуков и управления экземплярами
- **src/LordOfMU/** — Исполняемый файл-лаунчер
- **src/_Shared/** — Общие модули (ApiHooker, EncDec, Disasm, Packets и др.)
- **doc/** — Документация

## Сборка

Проект предназначен для компиляции в **Visual Studio 2019** (Platform Toolset v142) на **Windows 10 x86/x64**.

Конфигурации сборки:
- `Debug|Win32`
- `Release [Clicker]|Win32`
- `Release [Hacker]|Win32`
- `Release [Antihack]|Win32`

## Документация

Подробный промт для исправления ошибок компиляции и список читерских функций — см. [PROMPT.md](PROMPT.md).
