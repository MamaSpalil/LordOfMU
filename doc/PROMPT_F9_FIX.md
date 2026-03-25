# Промт для исправления конфликта F9/Shift+F9: Main.dll vs AutoClicker AVANTA+ELITE

## Контекст проекта

Ты — опытный C/C++ разработчик с глубоким знанием:
- Win32 API (Window Messages, Hooks, WndProc, субклассирование окон)
- ATL (Active Template Library): CWindowImpl, SubclassWindow, BEGIN_MSG_MAP / END_MSG_MAP
- DirectX 9: IDirect3DDevice9, EndScene hook, vtable patching
- Dear ImGui (v1.91.8): рендеринг в D3D9 EndScene, WndProcHandler
- MuOnline Season 3 Episode 1 (1.02.19): структура клиента, WndProc по адресу 0x004DA056, window hook адрес 0x004DBC6D
- DLL injection: WH_CBT hook, LoadLibrary в процесс игры
- Клавиатурные хуки: WH_KEYBOARD_LL (global), WH_KEYBOARD (thread-level)

## Структура проекта

```
src/
├── Bootstrapper/          # Запускает main.exe и устанавливает WH_CALLWNDPROC хук
├── LoaderDll/             # WH_CBT хук → RunDllWndProc → LoadLibrary для DLL
│   └── LoaderDll.cpp      # RunDllWndProc: загружает LordOfMU.dll, затем MUAutoClicker.dll
├── Client/                # Клиентский DLL (Main.dll) — модификации MU клиента
│   ├── Controller.cpp     # WH_KEYBOARD хук (thread-level) + WH_MOUSE хук
│   ├── WndProc.cpp        # Client WndProc: mouse handling, CallWindowProc(WndProcEx=0x4DA056)
│   ├── WndProc.h          # MU_WND_PROC_HOOK=0x004DBC6D, MU_WND_PROC=0x004DA056
│   ├── AutoClickerUI.cpp  # Заглушки — вся UI перенесена в LordOfClicker ImGui overlay
│   ├── LordOfMUBridge.h   # Мост: PostMessage(hGameWnd, WM_APP+xxx) для связи с AutoClicker
│   ├── Common.cpp          # Init(): ChangeAddress(MU_WND_PROC_HOOK, WndProc), gController.Load()
│   ├── Other.h            # #define WndProcEx (WNDPROC)(0x4DA056)
│   └── Structure.h        # #define MAIN_WINDOW 0x05877F90
├── LordOfClicker/         # AutoClicker DLL (MUAutoClicker.dll) — AVANTA+ELITE
│   ├── MuWindow.cpp       # CMuWindow: ATL SubclassWindow, KeyboardProcLL, OnKeyboardEvent, Timer1011
│   ├── MuWindow.h         # WM_APP+502=WM_SHOW_SETTINGS_GUI, WM_APP+512=WM_SHOW_HISTORY
│   ├── ImGuiOverlay.cpp   # CImGuiOverlay: Render, WndProcHandler, ToggleSettings, ToggleHistory
│   ├── ImGuiOverlay.h     # CImGuiOverlay class
│   ├── D3D9Hook.cpp       # D3D9 EndScene vtable hook (vtable[42], vtable[16])
│   └── imgui/             # Dear ImGui v1.91.8 source
├── LordOfMUdll/           # Packet proxy DLL — фильтрация пакетов, history, session stats
└── _Shared/               # Общие утилиты, ApiHooker.h (trampolines)
```

## Порядок загрузки DLL

```
1. Bootstrapper → ShellExecute(main.exe)
2. LoaderDll (WH_CBT хук) обнаруживает MU-окно → RunClient()
3. RunClient → SetWindowLongPtr(GWL_WNDPROC, RunDllWndProc) → PostMessage(WM_NULL)
4. RunDllWndProc:
   a. LoadLibrary(LordOfMU.dll) → DllGetClassObject → Client Init()
   b. Client Init() → ChangeAddress(0x004DBC6D, WndProc) → gController.Load() (WH_KEYBOARD хук)
   c. LoadLibrary(MUAutoClicker.dll) → DllGetClassObject → CMuWindow::Init()
   d. CMuWindow::Init() → SubclassWindow(hwnd) → PostMessage(WM_MUWND_INITIALIZE)
   e. OnInitMuWindow → SetWindowsHookEx(WH_KEYBOARD_LL) → SetTimer(1011, 100ms) → D3D9Hook::Install()
```

## WndProc Chain (после полной инициализации)

```
Windows Message → ATL CWindowImpl (CMuWindow) → Client WndProc → Game WndProc (0x4DA056)
```

## Текущая обработка F9

### Путь 1: WH_KEYBOARD_LL хук (KeyboardProcLL)
```
Файл: src/LordOfClicker/MuWindow.cpp:262-285
- ТОЛЬКО когда GetForegroundWindowTr() != m_hWnd (окно НЕ в фокусе)
- Вызывает OnKeyboardEvent(vk, msg, fCheckFgWnd=FALSE)
```

### Путь 2: Timer polling (каждые 100мс)
```
Файл: src/LordOfClicker/MuWindow.cpp:1547-1557
- GetAsyncKeyState(VK_F9) каждые 100мс
- Вызывает OnKeyboardEvent(vk, msg, fCheckFgWnd=TRUE)
```

### Путь 3: ATL WndProc → OnKeyboardMessage → ImGui
```
Файл: src/LordOfClicker/MuWindow.cpp:483-500
- MESSAGE_RANGE_HANDLER(WM_KEYFIRST..WM_KEYLAST → OnKeyboardMessage)
- Передаёт m_cOverlay.WndProcHandler() → ImGui обновляет io.KeysDown
- НЕ генерирует WM_SHOW_SETTINGS_GUI для F9
```

### OnKeyboardEvent (основная логика)
```
Файл: src/LordOfClicker/MuWindow.cpp:291-431
- F9 KEYDOWN: m_fShiftWasDown = GetAsyncKeyState(VK_SHIFT) & 0x8000
- F9 KEYUP: PostMessage(WM_SHOW_HISTORY) if shift, else PostMessage(WM_SHOW_SETTINGS_GUI)
```

### OnShowSettingsGUI
```
Файл: src/LordOfClicker/MuWindow.cpp:606-625
- Если m_pClicker != NULL → stop clicker, beep, return (НЕ открывает меню!)
- Иначе → m_cOverlay.ToggleSettings(), m_fGuiActive sync
```

### OnShowHistory
```
Файл: src/LordOfClicker/MuWindow.cpp:1590-1601
- QueryPickupHistory(), QuerySessionStats()
- m_cOverlay.ToggleHistory(), m_fGuiActive sync
```

## Конфликт: Client WH_KEYBOARD хук
```
Файл: src/Client/Controller.cpp:100-289
- WH_KEYBOARD (thread-level) — установлен через gController.Load()
- Обрабатывает ТОЛЬКО key-up: ((lParam & (1<<30)) && (lParam & (1<<31)))
- F9 case закомментирован, но:
  - gController.wKeyBord = VK_F9 записывается (строка 108)
  - Проверка 0x7D4C90/0x7960BA5 может вернуть false и пропустить всё
  - CallNextHookEx передаёт дальше
```

---

## НАЙДЕННЫЕ БАГИ (в порядке критичности)

### BUG-A: KeyboardProcLL не обрабатывает F9 когда окно foreground
- **Файл:** `src/LordOfClicker/MuWindow.cpp:274`
- **Строка:** `if (CMuWindow::GetForegroundWindowTr() != pThis->m_hWnd)`
- **Проблема:** Блок OnKeyboardEvent вызывается ТОЛЬКО когда окно NOT foreground
- **Эффект:** Когда игрок в игре (foreground), LL-хук пропускает F9

### BUG-B: Timer polling 100мс слишком медленный для клавиш
- **Файл:** `src/LordOfClicker/MuWindow.cpp:1547-1557`
- **Проблема:** Быстрое нажатие F9 (<100мс) может не зафиксироваться
- **Зависимость:** GetAsyncKeyState может быть "очищен" другими хуками к моменту polling

### BUG-C: Client WH_KEYBOARD хук мешает F9 обработке
- **Файл:** `src/Client/Controller.cpp:48,100-115`
- **Проблема:** Хук записывает wKeyBord = VK_F9; check на адреса 0x7D4C90/0x7960BA5 может подавить
- **Эффект:** F9 может не дойти до WndProc chain или дойти с задержкой

### BUG-D: GetAsyncKeyState trampolined — может скрывать F9
- **Файл:** `src/LordOfClicker/MuWindow.cpp:149`

### BUG-E: Shift+F9 race condition (100мс gap между KEYDOWN и KEYUP)
- **Файл:** `src/LordOfClicker/MuWindow.cpp:379-383`

### BUG-I: F12 конфликт: Controller::Window заменяет WndProc через SetWindowLong
- **Файл:** `src/Client/Controller.cpp:210`
- **Проблема:** `SetWindowLong(GWL_WNDPROC, Controller::Window)` разрывает ATL SubclassWindow chain
- **Эффект:** После F12 tray mode, WM_SHOW_SETTINGS_GUI/WM_SHOW_HISTORY не доходят до CMuWindow

---

## ЗАДАНИЕ НА ИСПРАВЛЕНИЕ

### Исправление 1: KeyboardProcLL — обработка F9 ВСЕГДА (BUG-A fix)

**Файл:** `src/LordOfClicker/MuWindow.cpp`
**Строки:** 262–285

Текущий код:
```cpp
LRESULT CALLBACK CMuWindow::KeyboardProcLL(int code, WPARAM wParam, LPARAM lParam)
{
    CMuWindow* pThis = CMuWindow::GetInstance();
    if (!pThis) return 0;
    if (code < 0) return CallNextHookEx(pThis->m_hKbdHook, code, wParam, lParam);
    
    KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
    
    if (CMuWindow::GetForegroundWindowTr() != pThis->m_hWnd)
    {
        if (pThis->OnKeyboardEvent(kbd->vkCode, (UINT)wParam, FALSE))
            return 0;
    }
    
    return CallNextHookEx(pThis->m_hKbdHook, code, wParam, lParam);
}
```

**Требуемое изменение:**

LL-хук должен обрабатывать F9 **ВСЕГДА**, а для остальных клавиш — только когда окно не foreground. Нужно добавить ветку: если `vkCode == VK_F9`, вызывать `OnKeyboardEvent` независимо от foreground status. Для VK_F9 передавать `fCheckFgWnd=FALSE`, так как LL-хук должен перехватывать F9 глобально.

**Логика:**
```
if (foreground window != game window) {
    // Существующая логика: обработка всех F-клавиш когда окно не в фокусе
    OnKeyboardEvent(vk, msg, fCheckFgWnd=FALSE);
} else if (vk == VK_F9) {
    // НОВОЕ: Обработка F9 когда окно В фокусе
    OnKeyboardEvent(VK_F9, msg, fCheckFgWnd=FALSE);
}
```

**Важно:** Для остальных клавиш (F5-F8, F10-F12) когда окно foreground, по-прежнему полагаемся на Timer polling, так как LL-хук + foreground = двойная обработка для других клавиш.

### Исправление 2: OnKeyboardMessage — дополнительный path для F9 (BUG-B backup fix)

**Файл:** `src/LordOfClicker/MuWindow.cpp`
**Функция:** `OnKeyboardMessage` (строки 483–500)

Добавить прямую обработку VK_F9 в OnKeyboardMessage как резервный путь, если LL-хук или Timer не сработали:

```
OnKeyboardMessage(WM_KEYDOWN, VK_F9, ...) {
    // Если это F9 KEYDOWN → запомнить Shift state
    // Если это F9 KEYUP → PostMessage(WM_SHOW_SETTINGS_GUI или WM_SHOW_HISTORY)
    // bHandled = TRUE; return 0;  // НЕ передавать F9 дальше в игру
}
```

**Важно:** Нужно предотвратить двойное срабатывание. Использовать флаг `m_fF9HandledByHook` или проверять, что WM_SHOW_SETTINGS_GUI ещё не был отправлен для этого нажатия.

**Логика защиты от дублирования:**
```cpp
// В OnKeyboardMessage для WM_KEYDOWN(VK_F9):
if (wParam == VK_F9 && uMsg == WM_KEYDOWN) {
    if (!m_fF9Pending) {
        m_fShiftWasDown = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
        m_fF9Pending = true;
    }
    bHandled = TRUE;
    return 0;
}
if (wParam == VK_F9 && uMsg == WM_KEYUP) {
    if (m_fF9Pending) {
        if (m_fShiftWasDown)
            PostMessage(WM_SHOW_HISTORY, 0, 0);
        else
            PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);
        m_fF9Pending = false;
        m_fShiftWasDown = false;
    }
    bHandled = TRUE;
    return 0;
}
```

**Новый member variable:**
```cpp
bool m_fF9Pending;  // TRUE между F9 KEYDOWN и F9 KEYUP обработанных через WndProc path
```

### Исправление 3: Controller::Keyboard — не мешать AutoClicker клавишам (BUG-C fix)

**Файл:** `src/Client/Controller.cpp`
**Функция:** `Controller::Keyboard` (строка 100)

Добавить раннее возвращение для клавиш, управляемых AutoClicker'ом (F5-F12), чтобы Client keyboard хук **не перехватывал** их:

```cpp
LRESULT Controller::Keyboard(int Code, WPARAM wParam, LPARAM lParam)
{
    if (Code == HC_ACTION)
    {
        // Клавиши F5-F12 управляются AutoClicker (LordOfMU).
        // Не записываем wKeyBord и не обрабатываем через Client.
        if (wParam >= VK_F5 && wParam <= VK_F12)
            return CallNextHookEx(gController.KeyboardHook, Code, wParam, lParam);
        
        if (((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0
            && GetForegroundWindow() == *(HWND*)(MAIN_WINDOW))
        {
            gController.wKeyBord = wParam;
            // ... остальной код без изменений
        }
    }
    return CallNextHookEx(gController.KeyboardHook, Code, wParam, lParam);
}
```

### Исправление 4: OnShowSettingsGUI — открывать меню после остановки кликера (BUG-K fix)

**Файл:** `src/LordOfClicker/MuWindow.cpp`
**Функция:** `OnShowSettingsGUI` (строки 606–625)

Вместо возврата без открытия меню, можно добавить задержку и повторную попытку. Или убрать блокировку если overlay уже рендерится:

**Вариант A (рекомендуемый):** Остановить кликер И открыть меню:
```cpp
LRESULT CMuWindow::OnShowSettingsGUI(UINT, WPARAM, LPARAM, BOOL&)
{
    if (m_pClicker != NULL)
    {
        PostMessage(WM_STOP_CLICKER, 0, 0);
        // Не return — продолжить к toggle
    }
    m_cOverlay.ToggleSettings();
    m_fGuiActive = m_cOverlay.IsAnyWindowVisible();
    return 0;
}
```

**Риск:** m_pClicker может ещё не стать NULL к моменту ToggleSettings(). Но это не влияет на ImGui overlay — он просто отобразит окно настроек, а кликер остановится асинхронно.

### Исправление 5 (опционально): OnKeyboardEvent — убрать зависимость от Timer для F9

Сделать `OnKeyboardEvent` для VK_F9 вызываемым из нескольких мест (LL-хук, WndProc, Timer) с защитой от дублирования:

```cpp
// Добавить в OnKeyboardEvent для F9:
if (vkCode == VK_F9 && uMsg == WM_KEYDOWN)
{
    if (m_fF9Pending) return TRUE;  // Уже обрабатывается
    m_fShiftWasDown = (CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    m_fF9Pending = true;
    return TRUE;
}

if (vkCode == VK_F9 && uMsg == WM_KEYUP)
{
    if (!m_fF9Pending) return TRUE;  // Не было KEYDOWN
    if (m_fShiftWasDown)
        PostMessage(WM_SHOW_HISTORY, 0, 0);
    else
        PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);
    m_fShiftWasDown = false;
    m_fF9Pending = false;
    return TRUE;
}
```

---

## ПОРЯДОК ПРИМЕНЕНИЯ ИСПРАВЛЕНИЙ

1. **Исправление 3** (Controller.cpp) — убрать помехи Client хука для F5-F12
2. **Исправление 1** (KeyboardProcLL) — обработка F9 при foreground окне
3. **Исправление 2** (OnKeyboardMessage) — backup path через WndProc
4. **Исправление 5** (OnKeyboardEvent) — защита от дублирования
5. **Исправление 4** (OnShowSettingsGUI) — открытие меню при работающем кликере

## ТЕСТИРОВАНИЕ ПОСЛЕ ИСПРАВЛЕНИЙ

1. Запустить main.exe → войти в игру → нажать F9 → должно открыться меню настроек
2. Нажать F9 снова → меню должно закрыться
3. Нажать Shift+F9 → должно открыться окно History Action
4. Нажать Shift+F9 снова → окно должно закрыться
5. Запустить автокликер (F5) → нажать F9 → кликер должен остановиться и меню открыться
6. Alt+Tab из игры → нажать F9 → меню должно открыться (LL-хук)
7. Нажать F12 (tray mode) → восстановить → нажать F9 → меню должно работать
8. Открыть чат в игре → нажать F9 → меню должно открыться (не подавлено чатом)
9. Быстро нажать и отпустить F9 (<50мс) → меню должно открыться
10. Нажать Shift+F9 когда History уже открыт → должно закрыться (toggle)

## ФАЙЛЫ ДЛЯ ИЗМЕНЕНИЯ

| Файл | Изменения | Исправление |
|------|-----------|-------------|
| `src/LordOfClicker/MuWindow.cpp` | KeyboardProcLL: F9 при foreground | #1 |
| `src/LordOfClicker/MuWindow.cpp` | OnKeyboardMessage: direct F9 handling | #2 |
| `src/LordOfClicker/MuWindow.cpp` | OnKeyboardEvent: dedup guard | #5 |
| `src/LordOfClicker/MuWindow.cpp` | OnShowSettingsGUI: allow with running clicker | #4 |
| `src/LordOfClicker/MuWindow.h` | Добавить m_fF9Pending member | #2, #5 |
| `src/Client/Controller.cpp` | Skip F5-F12 в Keyboard hook | #3 |
