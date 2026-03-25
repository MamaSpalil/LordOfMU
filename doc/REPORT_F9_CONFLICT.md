# Отчёт: Конфликт F9 / Shift+F9 между Main.dll (Клиент) и AutoClicker AVANTA+ELITE

## Дата: 2026-03-25
## Версия: MuOnline Season 3 Episode 1 (1.02.19)

---

## 1. ОБЩЕЕ ОПИСАНИЕ ПРОБЛЕМЫ

После запуска `main.exe` нажатие клавиши **F9** не открывает меню настроек автокликера **AVANTA+ELITE**, а нажатие **Shift+F9** не открывает окно **History Action**. Обе функции должны работать через ImGui-оверлей, рендерящийся в D3D9 EndScene хуке.

---

## 2. АРХИТЕКТУРА СИСТЕМЫ

### 2.1. Порядок загрузки DLL

```
1. LordOfLoader.dll (LoaderDll) — WH_CBT хук, обнаружение MU-окна
2. LordOfMUdll.dll  (Client/Main.dll) — патч WndProc по адресу 0x004DBC6D
3. MUAutoClicker.dll (LordOfClicker) — ATL SubclassWindow + WH_KEYBOARD_LL хук
```

**Файл:** `src/LoaderDll/LoaderDll.cpp`, строки 93–271 (`RunDllWndProc`)

### 2.2. Цепочка обработки WndProc (от окна к оригиналу)

```
Windows Message
    ↓
[ATL CWindowImpl::WindowProc]        ← CMuWindow::SubclassWindow() (LordOfClicker)
    ↓ (ProcessWindowMessage / BEGIN_MSG_MAP)
    ↓ (если ATL не обработал → DefWindowProc chain)
    ↓
[Client WndProc]                     ← ChangeAddress(0x004DBC6D, WndProc) (Main.dll)
    ↓
[CallWindowProc(WndProcEx=0x4DA056)] ← Оригинальная WndProc игры
```

### 2.3. Цепочка обработки клавиатуры

```
Клавиша F9 нажата
    ↓
[WH_KEYBOARD_LL хук]  ← CMuWindow::KeyboardProcLL() (LordOfClicker, глобальный)
    ↓ (если window НЕ foreground → вызов OnKeyboardEvent с fCheckFgWnd=FALSE)
    ↓
[WH_KEYBOARD хук]     ← Controller::Keyboard() (Main.dll, thread-level)
    ↓ (F9 case закомментирован, но хук всё равно перехватывает событие)
    ↓
[WndProc chain]        ← WM_KEYDOWN/WM_KEYUP через ATL → ImGui overlay
    ↓
[Polling Timer 1011]   ← каждые 100мс проверяет GetAsyncKeyState (fallback)
```

---

## 3. НАЙДЕННЫЕ БАГИ И КОНФЛИКТЫ

### BUG-A: КРИТИЧЕСКИЙ — WH_KEYBOARD_LL хук ТОЛЬКО обрабатывает F9 когда игра НЕ в фокусе

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки 262–285

```cpp
LRESULT CALLBACK CMuWindow::KeyboardProcLL(int code, WPARAM wParam, LPARAM lParam)
{
    // ...
    if (CMuWindow::GetForegroundWindowTr() != pThis->m_hWnd)   // ← ПРОБЛЕМА
    {
        if (pThis->OnKeyboardEvent(kbd->vkCode, (UINT)wParam, FALSE))
            return 0;
    }
    return CallNextHookEx(pThis->m_hKbdHook, code, wParam, lParam);
}
```

**Проблема:** Блок `if (GetForegroundWindowTr() != m_hWnd)` — LL-хук вызывает `OnKeyboardEvent()` ТОЛЬКО когда игровое окно **НЕ является foreground**. Когда игрок находится в игре (окно активно/foreground), этот блок пропускается, и F9 **НЕ обрабатывается LL-хуком**.

**Ожидалось:** Что Timer-based polling (Timer 1011) компенсирует это. Но:

### BUG-B: КРИТИЧЕСКИЙ — Timer polling использует fCheckFgWnd=TRUE по умолчанию (правильно), но конфликтует с Controller::Keyboard

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки 1547–1557

```cpp
for (int i=0; m_vFnKeys[i].vk != 0; ++i)
{
    bool fOldState = m_vFnKeys[i].fPressed;
    bool fNewState = (CMuWindow::GetAsyncKeyState(m_vFnKeys[i].vk) & 0x8000) != 0;
    
    if (fOldState != fNewState)
    {
        OnKeyboardEvent(m_vFnKeys[i].vk, fNewState ? WM_KEYDOWN : WM_KEYUP);
        // ↑ fCheckFgWnd = TRUE (default)
        m_vFnKeys[i].fPressed = fNewState;
    }
}
```

Timer вызывает `OnKeyboardEvent(vk, msg)` с `fCheckFgWnd=TRUE` (значение по умолчанию). Это корректно — проверяется что окно игры в foreground. **Однако** проблема в том, что:

1. `GetAsyncKeyState(VK_F9)` может не вернуть корректное состояние, если клавиша уже была "съедена" другим хуком
2. Client's `WH_KEYBOARD` хук (`Controller::Keyboard`) получает F9 **ДО** того как Timer polling его заметит

### BUG-C: КРИТИЧЕСКИЙ — Client's WH_KEYBOARD хук перехватывает F9 ДО Timer polling

**Файл:** `src/Client/Controller.cpp`, строки 100–289

```cpp
LRESULT Controller::Keyboard(int Code, WPARAM wParam, LPARAM lParam)
{
    if(Code == HC_ACTION)
    {
        if(((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0 
           && GetForegroundWindow() == *(HWND*)(MAIN_WINDOW))
        {
            gController.wKeyBord = wParam;   // ← F9 сохраняется здесь
            
            if ( *(int *)(0x7D4C90) || *(int *)(0x7960BA5) )
                return false;                 // ← МОЖЕТ ПОДАВИТЬ F9!
            
            switch(wParam)
            {
                // F9 case закомментирован — но switch НЕ обрабатывает F9
                // и ПРОДОЛЖАЕТ через default → НЕ возвращает значение
            }
        }
    }
    return CallNextHookEx(gController.KeyboardHook, Code, wParam, lParam);
}
```

**Проблема 1:** Строка 110: `if (*(int*)(0x7D4C90) || *(int*)(0x7960BA5))` — эта проверка на состояние чата/ввода текста. Если один из этих адресов содержит ненулевое значение (что происходит когда открыт чат или текстовое поле в игре), хук возвращает `false` (= 0 = не блокировать), **НО** при этом уже записал `gController.wKeyBord = wParam`, что может повлиять на логику клиента.

**Проблема 2:** Проверка `((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0` ожидает что бит 30 (предыдущее состояние = нажата) И бит 31 (переход = отпускание) оба установлены. Это означает хук обрабатывает **ТОЛЬКО key-up** события. Но при этом хук **пропускает** F9 к следующему хуку в цепочке через `CallNextHookEx`, что означает WM_KEYUP с F9 приходит в WndProc chain.

**Проблема 3:** `WH_KEYBOARD` хук (thread-level) и `WH_KEYBOARD_LL` хук (global) — это разные уровни. `WH_KEYBOARD_LL` вызывается ДО `WH_KEYBOARD`. Но в текущем коде `KeyboardProcLL` пропускает F9 когда окно foreground (BUG-A), поэтому `WH_KEYBOARD` (Controller) получает F9 раньше Timer polling.

### BUG-D: СРЕДНИЙ — Race condition между Timer polling и GetAsyncKeyState

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки 1547–1557

Timer polling проверяет `GetAsyncKeyState(VK_F9)` каждые 100мс. Но `GetAsyncKeyState` может быть перехвачен AutoClicker'ом:

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строка 149, 21

```cpp
CGetAsyncKeyStateTramp CMuWindow::GetAsyncKeyState;  // Trampoline
// ...
GetAsyncKeyState.Patch(&MyGetAsyncKeyState);          // Патч в OnInitMuWindow
```

Если `MyGetAsyncKeyState` модифицирует поведение `GetAsyncKeyState`, Timer polling может получить неверные данные о состоянии F9.

### BUG-E: СРЕДНИЙ — Shift+F9 race condition в Timer polling

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки 379–383

```cpp
if (vkCode == VK_F9 && uMsg == WM_KEYDOWN)
{
    m_fShiftWasDown = (CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    return TRUE;
}
```

Когда F9 обнаруживается через Timer polling (каждые 100мс), генерируются синтетические WM_KEYDOWN и WM_KEYUP. Между генерацией WM_KEYDOWN (когда `m_fShiftWasDown` записывается) и WM_KEYUP (когда читается) проходит минимум один цикл таймера (100мс). За это время пользователь может отпустить Shift.

Комментарий в коде (BUG-4 fix, строка 376) подтверждает эту проблему — она была частично исправлена перемещением `GetAsyncKeyState(VK_SHIFT)` на KEYDOWN, но фундаментальная проблема 100мс задержки Timer polling остаётся.

### BUG-F: НИЗКИЙ — ATL END_MSG_MAP не возвращает bHandled=TRUE для WM_APP+xxx сообщений

**Файл:** `src/LordOfClicker/MuWindow.h`, строки 49–80

ATL `END_MSG_MAP()` макрос генерирует `bHandled = FALSE; return 0;` для необработанных сообщений. Это означает что если `PostMessage(WM_SHOW_SETTINGS_GUI)` как-то не совпадёт с обработчиком в message map, сообщение пройдёт к предыдущему WndProc (Client WndProc), который его не обработает, и оно уйдёт в оригинальный WndProc игры. Это сообщение WM_APP+502 может быть интерпретировано игрой как что-то другое.

### BUG-G: НИЗКИЙ — Controller::Keyboard записывает wKeyBord для F9

**Файл:** `src/Client/Controller.cpp`, строка 108

```cpp
gController.wKeyBord = wParam;  // Записывает VK_F9
```

Даже если F9 не обрабатывается в switch-case, значение `wKeyBord` обновляется до VK_F9 (0x78). Другие части кода клиента могут читать это значение и выполнять нежелательные действия.

### BUG-H: СРЕДНИЙ — Двойная инициализация хуков в Client DLL

**Файлы:**
- `src/Client/Common.cpp`, строка 530: `gController.Load()` — устанавливает WH_KEYBOARD хук
- `src/Client/dllmain.cpp`, строка 290: `ChangeAddress(MU_WND_PROC_HOOK, FPTR(WndProc))` — патчит WndProc

Файлы `Common.cpp` и `dllmain.cpp` оба содержат функцию `Init()` с `ChangeAddress(MU_WND_PROC_HOOK, FPTR(WndProc))`. В зависимости от конфигурации сборки, оба могут вызываться, что создаёт двойной патч WndProc.

---

## 4. КОРНЕВАЯ ПРИЧИНА НЕРАБОЧИХ F9/Shift+F9

### Сценарий: Игрок нажимает F9 внутри игры (окно активно/foreground)

```
1. WH_KEYBOARD_LL (KeyboardProcLL):
   → GetForegroundWindowTr() == m_hWnd (TRUE, окно активно)
   → Блок if (!=) НЕ выполняется
   → F9 НЕ перехвачен
   → CallNextHookEx → передано дальше
   
2. WH_KEYBOARD (Controller::Keyboard):
   → GetForegroundWindow() == MAIN_WINDOW (TRUE)
   → gController.wKeyBord = VK_F9
   → Проверка адресов 0x7D4C90 / 0x7960BA5:
     - Если чат открыт → return false → F9 проходит к игре
     - Если чат закрыт → switch(VK_F9) → case VK_F9 закомментирован
     → default → никакого return → break → CallNextHookEx
   
3. WndProc chain:
   → WM_KEYDOWN(VK_F9) приходит в ATL CMuWindow::ProcessWindowMessage
   → MESSAGE_RANGE_HANDLER(WM_KEYFIRST..WM_KEYLAST → OnKeyboardMessage)
   → OnKeyboardMessage → m_cOverlay.WndProcHandler(WM_KEYDOWN, VK_F9)
   → ImGui_ImplWin32_WndProcHandler записывает F9 в io.KeysDown
   → io.WantCaptureKeyboard = FALSE (нет активного текстового поля)
   → bHandled = FALSE → WM_KEYDOWN уходит к Client WndProc
   → Client WndProc: нет обработки WM_KEYDOWN → CallWindowProc(WndProcEx)
   → Оригинальная WndProc игры обрабатывает WM_KEYDOWN(VK_F9)
   → Игра может выполнить свою логику для F9

4. Timer Polling (через ≤100мс):
   → GetAsyncKeyState(VK_F9) & 0x8000 → TRUE (если клавиша всё ещё нажата)
   → Но GetAsyncKeyState мог вернуть 0 если клавиша уже обработана хуком
   → fOldState != fNewState → OnKeyboardEvent(VK_F9, WM_KEYDOWN)
   → fCheckFgWnd=TRUE → foreground check → OK
   → m_fShiftWasDown записывается
   → PostMessage(WM_SHOW_SETTINGS_GUI) или WM_SHOW_HISTORY
   
   НО: Если шаг 2 или 3 уже "съели" состояние F9, Timer может НЕ заметить нажатие
```

### Главные причины:

1. **LL-хук не обрабатывает F9 когда окно в фокусе** (BUG-A) — по дизайну полагается на Timer polling
2. **Timer polling ненадёжен из-за 100мс интервала** — пользователь может нажать и отпустить F9 быстрее
3. **Client's WH_KEYBOARD хук записывает F9** и может повлиять на `GetAsyncKeyState` (BUG-C)
4. **Нет прямого перехвата F9 из WndProc message map** — ATL message map не содержит специального обработчика для WM_KEYDOWN/WM_KEYUP с VK_F9

---

## 5. КОНФЛИКТЫ В ЛОГИКЕ

### 5.1. Конфликт: Два keyboard хука одного уровня

| Хук | Уровень | Установка | F9 обработка |
|-----|---------|-----------|-------------|
| `KeyboardProcLL` (LordOfClicker) | WH_KEYBOARD_LL (глобальный) | `MuWindow.cpp:155` | Только когда NOT foreground |
| `Controller::Keyboard` (Client) | WH_KEYBOARD (thread) | `Controller.cpp:48` | Записывает wKeyBord, не блокирует |

Оба хука установлены для одного потока. WH_KEYBOARD_LL вызывается ДО WH_KEYBOARD, но не перехватывает F9 когда окно в фокусе.

### 5.2. Конфликт: WndProc chain и message forwarding

**Client WndProc** (src/Client/WndProc.cpp) не обрабатывает WM_KEYDOWN/WM_KEYUP, но пропускает их к оригинальной WndProc игры. Оригинальная WndProc может "съесть" F9 message до того, как Timer polling заметит нажатие.

### 5.3. Конфликт: GetAsyncKeyState hook

AutoClicker патчит `GetAsyncKeyState` (строка 149 MuWindow.cpp), что может повлиять на Timer polling. Если `MyGetAsyncKeyState` изменяет возвращаемое значение для F9, polling не заметит нажатие.

### 5.4. Конфликт: F12 в Client vs AutoClicker

Оба DLL обрабатывают F12:
- **Controller.cpp:206–216:** `SetWindowLong(GWL_WNDPROC, Controller::Window)` — заменяет WndProc на Controller::Window для tray mode
- **MuWindow.cpp:293–312:** F12 обрабатывается в `OnKeyboardEvent` — show/hide all MU windows

Если Client обрабатывает F12 первым через `WH_KEYBOARD` и заменяет WndProc через `SetWindowLong`, это разрывает ATL subclass chain AutoClicker'а, что может привести к тому, что WM_SHOW_SETTINGS_GUI и WM_SHOW_HISTORY никогда не дойдут до CMuWindow.

---

## 6. ДОПОЛНИТЕЛЬНЫЕ ПРОБЛЕМЫ

### 6.1. ImGui overlay не инициализируется до первого EndScene

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки 1641–1646

ImGui overlay инициализируется лениво (lazy init) при первом вызове EndScene. Если F9 нажимается до первого рендер-кадра, `m_cOverlay.IsInitialized()` вернёт `false`, и `ToggleSettings()` установит `m_bShowSettings=true`, но overlay ещё не готов к рендерингу.

### 6.2. OnShowSettingsGUI блокирует если кликер запущен

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки 613–617

```cpp
if (m_pClicker != NULL)
{
    PostMessage(WM_STOP_CLICKER, 0, 0);
    MessageBeep(MB_ICONINFORMATION);
    return 0;  // ← Настройки НЕ открываются!
}
```

Если автокликер запущен, F9 остановит его, но **НЕ откроет** меню настроек. Пользователь должен нажать F9 повторно.

### 6.3. m_fGuiActive десинхронизация

`m_fGuiActive` обновляется в Timer через `m_cOverlay.IsAnyWindowVisible()` каждые 100мс. Но между обновлениями флаг может быть неактуальным, что вызывает:
- Ложная блокировка F5-F8 (если overlay закрылся, но флаг ещё TRUE)
- Пропуск блокировки (если overlay открылся, но флаг ещё FALSE)

---

## 7. СВОДНАЯ ТАБЛИЦА БАГОВ

| # | Критичность | Описание | Файл:строка |
|---|-------------|----------|-------------|
| BUG-A | 🔴 КРИТИЧЕСКИЙ | LL-хук не обрабатывает F9 когда окно в фокусе | MuWindow.cpp:274 |
| BUG-B | 🔴 КРИТИЧЕСКИЙ | Timer polling ненадёжен (100мс интервал) | MuWindow.cpp:1547-1557 |
| BUG-C | 🔴 КРИТИЧЕСКИЙ | Client WH_KEYBOARD хук перехватывает F9 до Timer | Controller.cpp:48,100-115 |
| BUG-D | 🟡 СРЕДНИЙ | GetAsyncKeyState hook может скрывать F9 от polling | MuWindow.cpp:149,21 |
| BUG-E | 🟡 СРЕДНИЙ | Shift+F9 race condition (100мс gap) | MuWindow.cpp:379-383 |
| BUG-F | 🟢 НИЗКИЙ | ATL END_MSG_MAP пропускает WM_APP+xxx сообщения | MuWindow.h:80 |
| BUG-G | 🟢 НИЗКИЙ | Controller записывает wKeyBord для F9 | Controller.cpp:108 |
| BUG-H | 🟡 СРЕДНИЙ | Двойная Init() в Common.cpp и dllmain.cpp | Common.cpp:529, dllmain.cpp:290 |
| BUG-I | 🟡 СРЕДНИЙ | F12 Client конфликт: SetWindowLong разрывает ATL chain | Controller.cpp:210 |
| BUG-J | 🟢 НИЗКИЙ | Lazy ImGui init — F9 до первого кадра | MuWindow.cpp:1641-1646 |
| BUG-K | 🟢 НИЗКИЙ | OnShowSettingsGUI не открывает меню при работающем кликере | MuWindow.cpp:613-617 |

---

## 8. РЕКОМЕНДУЕМОЕ РЕШЕНИЕ

### Подход 1: Исправить LL-хук (KeyboardProcLL) — обрабатывать F9 ВСЕГДА

Убрать условие `if (GetForegroundWindowTr() != m_hWnd)` или добавить обработку F9/Shift+F9 вне этого условия. LL-хук должен перехватывать F9 независимо от того, в фокусе окно или нет.

### Подход 2: Добавить обработку F9 в OnKeyboardMessage (WndProc path)

В `OnKeyboardMessage` добавить прямую проверку VK_F9 и генерацию WM_SHOW_SETTINGS_GUI / WM_SHOW_HISTORY, как дополнительный путь обработки.

### Подход 3: Отключить Controller::Keyboard хук для F-клавиш автокликера

В `Controller::Keyboard` добавить раннее возвращение для VK_F5–VK_F12 чтобы клиент не перехватывал эти клавиши.

### Рекомендация: Использовать все три подхода одновременно для максимальной надёжности.

---

## 9. ЗАТРОНУТЫЕ ФАЙЛЫ

| Файл | Что менять |
|------|-----------|
| `src/LordOfClicker/MuWindow.cpp` | KeyboardProcLL: обработка F9 при foreground; OnKeyboardMessage: direct F9 handling |
| `src/Client/Controller.cpp` | Keyboard: skip F5-F12 для LordOfClicker |
| `src/LordOfClicker/MuWindow.h` | Возможно добавление новых MESSAGE_HANDLER |
