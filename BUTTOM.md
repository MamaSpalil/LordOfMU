# BUTTOM.md — Подробный план исправления всех багов HUD-кнопок  
# (Settings, Start/Stop Clicker, History Action)

> **Цель:** Кнопки Settings, Start/Stop Clicker и History Action должны появляться в игровом клиенте  
> сразу после выбора персонажа и входа в игровой мир. Пользователь управляет ими исключительно  
> через ЛКМ (левая кнопка мыши). Кнопки, автокликер, настройки и история должны работать  
> как единый организм — без наложений, конфликтов ввода и визуальных багов.

---

## СОДЕРЖАНИЕ

1. [Задача 1 — Исправить определения WM-сообщений (отсутствие скобок в макросах)](#задача-1)
2. [Задача 2 — Устранить конфликт перехвата мыши при работающем автокликере](#задача-2)
3. [Задача 3 — Исправить блокировку открытия Settings при работающем автокликере](#задача-3)
4. [Задача 4 — Устранить наложение HUD-кнопок на игровое окно (z-order и прозрачность)](#задача-4)
5. [Задача 5 — Исправить конфликт нажатий клавиатуры и мыши с HUD-кнопками](#задача-5)
6. [Задача 6 — Проверить корректность жизненного цикла HUD-кнопок (показ/скрытие)](#задача-6)
7. [Задача 7 — Проверить компиляцию и интеграцию в процесс сборки](#задача-7)
8. [Задача 8 — Проверить корректность ресурсов (bitmap-иконки)](#задача-8)
9. [Задача 9 — Проверить связь между DLL-модулями (CharInfoFilter → MuWindow)](#задача-9)
10. [Задача 10 — Интеграционное тестирование и валидация](#задача-10)

---

<a id="задача-1"></a>
## Задача 1 — Исправить определения WM-сообщений (отсутствие скобок в макросах)

### Описание проблемы

В файлах `HUDButtons.h`, `MuWindow.h` и `CharInfoFilter.cpp` все пользовательские WM-сообщения  
определены **без скобок**:

```cpp
#define WM_HUD_SETTINGS     WM_APP + 601   // ⚠️ Без скобок!
#define WM_HUD_STARTSTOP    WM_APP + 602
#define WM_HUD_HISTORY      WM_APP + 603
```

В контексте ATL-макроса `MESSAGE_HANDLER` это раскрывается как:
```cpp
MESSAGE_HANDLER(WM_APP + 601, OnHUDSettings)
```

Хотя в данном конкретном контексте макрос ATL использует `==` сравнение и оператор `+`  
имеет более высокий приоритет чем `==`, **при использовании этих макросов в любом другом  
арифметическом или битовом выражении** (например, `switch/case`, побитовые операции,  
сравнения через `<`, `>`, вложенные макросы) — отсутствие скобок приведёт к  
некорректному вычислению значения сообщения.

### Подпункты выполнения

**1.1.** Открыть файл `src/LordOfClicker/HUDButtons.h` (строки 10–12).  
Заменить:
```cpp
#define WM_HUD_SETTINGS     WM_APP + 601
#define WM_HUD_STARTSTOP    WM_APP + 602
#define WM_HUD_HISTORY      WM_APP + 603
```
На:
```cpp
#define WM_HUD_SETTINGS     (WM_APP + 601)
#define WM_HUD_STARTSTOP    (WM_APP + 602)
#define WM_HUD_HISTORY      (WM_APP + 603)
```

**1.2.** Открыть файл `src/LordOfClicker/MuWindow.h` (строки 15–26).  
Добавить скобки ко **всем** определениям WM-сообщений:
```cpp
#define WM_IS_CLICKER_INSTALLED (WM_APP + 402)
#define WM_MUWND_INITIALIZE     (WM_APP + 501)
#define WM_SHOW_SETTINGS_GUI    (WM_APP + 502)
#define WM_START_CLICKER        (WM_APP + 503)
#define WM_STOP_CLICKER         (WM_APP + 504)
#define WM_LAUNCH_MU            (WM_APP + 505)
#define WM_SWITCH_INSTANCE      (WM_APP + 506)
#define WM_SHOW_MU              (WM_APP + 507)
#define WM_HIDE_MU              (WM_APP + 508)
#define WM_SET_GAME_VERSION     (WM_APP + 509)
#define WM_CHAR_SELECTED        (WM_APP + 510)
```

**1.3.** Открыть файл `src/LordOfMUdll/CharInfoFilter.cpp` (строка 7).  
Заменить:
```cpp
#define WM_CHAR_SELECTED    WM_APP + 510
```
На:
```cpp
#define WM_CHAR_SELECTED    (WM_APP + 510)
```

**1.4.** Выполнить глобальный поиск по всему проекту на наличие `WM_APP +` без скобок  
и исправить все найденные случаи. Команда:
```
grep -rn "WM_APP +" src/ --include="*.h" --include="*.cpp" | grep -v "(WM_APP"
```

**1.5.** Скомпилировать проект и убедиться, что все определения корректны.

---

<a id="задача-2"></a>
## Задача 2 — Устранить конфликт перехвата мыши при работающем автокликере

### Описание проблемы

В `MuWindow.h` (строка 49) установлен `MESSAGE_RANGE_HANDLER` для **всех** мышиных сообщений:
```cpp
MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
```

Обработчик `OnMouseMessage` в `MuWindow.cpp` (строки 375–392):
```cpp
if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDBLCLK)
{
    bHandled = FALSE;  // Пропускаем левый клик
}
else
{
    bHandled = m_fBlockInput;  // Блокируем остальные мышиные сообщения
}
```

**Проблема:** Когда автокликер работает (`m_fBlockInput = TRUE`), все WM_MOUSEMOVE  
сообщения для **основного окна игры** блокируются. Однако HUD-кнопки — это **отдельное  
popup-окно** (класс `MU_HUDButtons`), и его сообщения обрабатываются в **собственном**  
оконном процессе. Мышиные сообщения игрового окна не влияют на HUD-окно напрямую.

Тем не менее, возникает **косвенный конфликт**: когда автокликер программно двигает мышь  
(`SendMessage WM_MOUSEMOVE`) и кликает (`WM_LBUTTONDOWN/UP`), а пользователь одновременно  
пытается кликнуть по HUD-кнопке — происходит состояние гонки (race condition).

Также обработчик `WM_CAPTURECHANGED` (строка 1101–1107) блокирует сообщение когда кликер  
запущен (`m_pClicker != 0`), что может привести к потере capture у HUD-кнопки если она  
вызвала `SetCapture()` во время `OnLButtonDown`.

### Подпункты выполнения

**2.1.** В файле `src/LordOfClicker/HUDButtons.cpp`, в методе `OnLButtonDown` (строка 391):  
**Добавить проверку** — при нажатии на кнопку HUD, отправить сигнал CMuWindow о том,  
что пользователь взаимодействует с HUD, чтобы автокликер не перехватывал управление мышью:
```cpp
LRESULT CHUDButtons::OnLButtonDown(UINT, WPARAM, LPARAM lParam, BOOL&)
{
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    int btn = HitTest(x, y);

    if (btn >= 0)
    {
        m_iPressedBtn = btn;
        SetCapture();
        InvalidateRect(NULL, FALSE);
    }
    else
    {
        // Клик вне кнопок — переслать в игровое окно
        // чтобы не "проглатывать" клики
    }

    return 0;
}
```

**2.2.** В файле `src/LordOfClicker/MuWindow.cpp`, в обработчике `OnCaptureChanged`  
(строки 1101–1107), добавить проверку: если capture переходит к HUD-окну, не блокировать:
```cpp
LRESULT CMuWindow::OnCaptureChanged(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
    HWND hwndNewCapture = (HWND)lParam;
    
    // Не блокировать если capture перешёл к HUD-кнопкам
    if (m_cHUDButtons.IsWindow() && hwndNewCapture == m_cHUDButtons.m_hWnd)
    {
        bHandled = FALSE;
        return 0;
    }
    
    if (m_pClicker == 0)
        bHandled = FALSE;

    return 0;
}
```

**2.3.** Проверить, что `SetCapture()` в HUD-кнопках корректно работает при запущенном  
автокликере. HUD — отдельное окно, и `SetCapture()` для него должен работать независимо  
от автокликера.

**2.4.** Добавить в `OnHUDStartStop` логирование для отладки:
```cpp
WriteClickerLogFmt("HUD", "StartStop clicked, m_pClicker=%p", m_pClicker);
```

**2.5.** Скомпилировать и протестировать:
- Запустить автокликер (F5 или HUD-кнопка Start)
- Попытаться нажать Stop через HUD
- Убедиться что кнопка срабатывает без задержек

---

<a id="задача-3"></a>
## Задача 3 — Исправить блокировку открытия Settings при работающем автокликере

### Описание проблемы

В `MuWindow.cpp` (строки 498–501):
```cpp
LRESULT CMuWindow::OnShowSettingsGUI(UINT, WPARAM, LPARAM, BOOL&)
{
    if (m_pClicker != NULL)
        return 0;              // ← БЛОКИРОВКА: Settings не открывается!
```

Когда автокликер запущен, нажатие на HUD-кнопку Settings **ничего не делает** —  
обработчик возвращает 0 и диалог не показывается. Пользователь ожидает хотя бы  
уведомление или возможность остановить кликер и открыть настройки.

### Подпункты выполнения

**3.1.** Определить желаемое поведение:
- **Вариант A:** При нажатии Settings во время работы автокликера — сначала остановить  
  автокликер, затем показать настройки.
- **Вариант B:** Показать уведомление "Stop the clicker first" (через `MessageBeep`  
  или серверное сообщение).
- **Вариант C:** Разрешить просмотр настроек в read-only режиме.

**3.2.** Реализовать выбранный вариант в `OnShowSettingsGUI`:

Для **Варианта A** (рекомендуется):
```cpp
LRESULT CMuWindow::OnShowSettingsGUI(UINT, WPARAM, LPARAM, BOOL&)
{
    // Если автокликер запущен — останавливаем его перед показом настроек
    if (m_pClicker != NULL)
    {
        PostMessage(WM_STOP_CLICKER, 0, 0);
        // Настройки откроются после остановки через повторный вызов
        // (пользователь нажмёт кнопку ещё раз)
        MessageBeep(MB_ICONINFORMATION);
        return 0;
    }

    CWindow dlg = m_cUnifiedSettingsDlg;
    if (m_fGuiActive || !dlg.IsWindow())
        return 0;

    m_fGuiActive = TRUE;
    dlg.ShowWindow(SW_SHOWNOACTIVATE);
    ::SetWindowPos(dlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

    return 0;
}
```

**3.3.** Добавить визуальную обратную связь на HUD-кнопку Settings:  
Если кликер запущен — показать кнопку Settings в "заблокированном" (серый/тусклый)  
состоянии. Добавить новое состояние `m_bSettingsDisabled` в CHUDButtons и отрисовывать  
серую кнопку.

**3.4.** Скомпилировать и протестировать:
- Запустить автокликер
- Нажать Settings через HUD
- Убедиться что кликер останавливается / показывается уведомление
- Нажать Settings повторно — настройки должны открыться

---

<a id="задача-4"></a>
## Задача 4 — Устранить наложение HUD-кнопок на игровое окно (z-order и прозрачность)

### Описание проблемы

HUD-кнопки создаются как `WS_POPUP | WS_EX_TOPMOST | WS_EX_LAYERED` окно.  
Прозрачность реализована через `LWA_COLORKEY` с ключом `RGB(255, 0, 255)` (магента).

**Потенциальные проблемы наложения:**

1. **DirectDraw/Direct3D рендеринг игры может перекрывать окно HUD.** MU Online S3E1  
   использует DirectDraw/Direct3D для рендеринга, и при определённых условиях  
   (fullscreen exclusive mode) Direct3D surface может рендериться поверх всех окон.

2. **WS_EX_TOPMOST может быть понижен** системой или другим приложением через  
   `SetWindowPos(HWND_NOTOPMOST)`. После этого HUD "провалится" за игровое окно.

3. **Размер HUD-кнопок (56×18 пикселей)** слишком мал — кнопки 16×16 пикселей с  
   2px отступами. При масштабировании DPI (125%, 150%) кнопки становятся ещё мельче  
   визуально.

4. **Позиция (70, 3) от клиентской области** может перекрываться с элементами UI  
   игры (FPS-счётчик, мини-карта, чат).

### Подпункты выполнения

**4.1.** В `CHUDButtons::Reposition()` (строка 161) **всегда** восстанавливать TOPMOST:
```cpp
// Текущий код уже делает это:
SetWindowPos(HWND_TOPMOST, x, y, 0, 0,
    SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
```
Убедиться что HWND_TOPMOST **никогда** не заменяется на HWND_NOTOPMOST.

**4.2.** Проверить что в `Create()` (строка 79):
```cpp
SetWindowPos(NULL, x, y, barWidth, barHeight, SWP_NOZORDER | SWP_NOACTIVATE);
```
`SWP_NOZORDER` здесь корректен, так как z-order устанавливается при первом `Reposition()`.

**4.3.** **Увеличить размер кнопок** для лучшей видимости и кликабельности.  
Рекомендуется:
```cpp
enum { BTN_SIZE = 24 };      // Было 16, стало 24 — увеличение на 50%
enum { BTN_SPACING = 3 };    // Было 2, стало 3
enum { BAR_PADDING = 2 };    // Было 1, стало 2
```
Новый общий размер: (3×24) + (2×3) + (2×2) = 82×28 пикселей — достаточно для  
комфортного клика мышью.

**4.4.** **Добавить DPI-awareness.** В методе `Create()` перед расчётом размеров:
```cpp
// Получить DPI масштаб для правильного размера кнопок
HDC hDC = ::GetDC(hwndOwner);
int dpiX = GetDeviceCaps(hDC, LOGPIXELSX);
::ReleaseDC(hwndOwner, hDC);
float scale = (float)dpiX / 96.0f;

int scaledBtnSize = (int)(BTN_SIZE * scale);
int scaledSpacing = (int)(BTN_SPACING * scale);
int scaledPadding = (int)(BAR_PADDING * scale);
```

**4.5.** **Добавить полупрозрачный фон** вместо полностью прозрачного для лучшей  
видимости кнопок на ярком фоне игры. Изменить LWA_COLORKEY на LWA_ALPHA:
```cpp
// Вместо:
::SetLayeredWindowAttributes(m_hWnd, HUD_TRANSPARENT_KEY, 0, LWA_COLORKEY);

// Использовать комбинацию:
::SetLayeredWindowAttributes(m_hWnd, HUD_TRANSPARENT_KEY, 220, LWA_COLORKEY | LWA_ALPHA);
```
Это сделает кнопки слегка полупрозрачными (220/255 = ~86% непрозрачности).

**4.6.** **Проверить позицию кнопок** — не перекрывают ли они стандартные UI-элементы  
MU Online (FPS-счётчик в верхнем левом углу, мини-карта в верхнем правом).  
При необходимости изменить смещение:
```cpp
int x = ptClient.x + 90;   // Было 70, отодвинуть правее от FPS
int y = ptClient.y + 48;   // Было 3, опустить ниже заголовка
```

**4.7.** Скомпилировать и протестировать:
- Убедиться что кнопки видны поверх игрового мира
- Убедиться что кнопки не перекрывают важные элементы UI игры
- Убедиться что при перемещении окна кнопки следуют за ним
- Проверить при разных DPI (100%, 125%, 150%)

---

<a id="задача-5"></a>
## Задача 5 — Исправить конфликт нажатий клавиатуры и мыши с HUD-кнопками

### Описание проблемы

**Конфликт клавиатуры:**  
В `MuWindow.cpp` установлен глобальный низкоуровневый хук клавиатуры  
`WH_KEYBOARD_LL` (строка 146):
```cpp
m_hKbdHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProcLL, ...);
```

Обработчик `KeyboardProcLL` (строки 232–251) перехватывает клавиши F5–F12  
и ESC **глобально**. Когда открыт диалог Settings или History, нажатие ESC  
закрывает диалог (строки 360–366):
```cpp
if (vkCode == VK_ESCAPE && uMsg == WM_KEYUP && m_fGuiActive)
{
    m_cSettingsDlg.ShowWindow(SW_HIDE);
    m_cUnifiedSettingsDlg.ShowWindow(SW_HIDE);
    m_cHistoryDlg.ShowWindow(SW_HIDE);
    return TRUE;
}
```

**Проблема:** Хук `return TRUE` "проглатывает" ESC и не передаёт его игре.  
Но если пользователь нажимает F5 (Start/Stop) во время открытого диалога —  
автокликер запускается/останавливается **одновременно** с открытым диалогом.

**Конфликт мыши:**  
Когда пользователь кликает по HUD-кнопке, `WS_EX_NOACTIVATE` предотвращает  
перехват фокуса. Но `MA_NOACTIVATE` в `OnMouseActivate` **не передаёт клик  
обратно в игру** — клик полностью "поглощается" HUD-окном. Если пользователь  
промахнулся мимо кнопки, клик теряется (не доходит до игры).

### Подпункты выполнения

**5.1.** В `OnKeyboardEvent` (строки 257–369), добавить проверку: если `m_fGuiActive == TRUE`,  
**блокировать** F5/F6/F7/F8 (управление кликером) пока диалог открыт:
```cpp
// Добавить в начало OnKeyboardEvent, после проверки F12:
if (m_fGuiActive)
{
    // Пока открыт диалог — запретить управление автокликером
    if (vkCode >= VK_F5 && vkCode <= VK_F8)
        return TRUE;  // Проглотить нажатие
}
```

**5.2.** В `CHUDButtons::OnLButtonDown`, если клик **не попал** ни на одну кнопку  
(HitTest возвращает -1), переслать клик в игровое окно:
```cpp
if (btn < 0)
{
    // Клик мимо кнопок — переслать в игровое окно
    POINT pt = { LOWORD(lParam), HIWORD(lParam) };
    ClientToScreen(m_hWnd, &pt);
    ScreenToClient(m_hwndOwner, &pt);
    ::PostMessage(m_hwndOwner, WM_LBUTTONDOWN, 0, MAKELPARAM(pt.x, pt.y));
    return 0;
}
```
И аналогично для `OnLButtonUp`.

**5.3.** В `CHUDButtons::OnMouseActivate` (строка 478), убедиться что возвращается  
именно `MA_NOACTIVATE` (а не `MA_NOACTIVATEANDEAT` который проглатывает клик):
```cpp
return MA_NOACTIVATE;  // ✓ Корректно — клик обрабатывается, фокус не меняется
```

**5.4.** Добавить обработку `WM_NCHITTEST` в HUD-кнопках для правильного поведения  
при наведении — возвращать `HTCLIENT` для области кнопок и `HTTRANSPARENT` для  
пустого пространства:
```cpp
// Добавить в BEGIN_MSG_MAP:
MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)

// Реализация:
LRESULT CHUDButtons::OnNCHitTest(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    ScreenToClient(&pt);
    
    if (HitTest(pt.x, pt.y) >= 0)
        return HTCLIENT;      // Кнопка — обрабатываем
    
    return HTTRANSPARENT;      // Пустое место — клик пройдёт сквозь
}
```

**5.5.** Скомпилировать и протестировать:
- Нажать F5 при открытом диалоге Settings — кликер НЕ должен запускаться
- Кликнуть мимо HUD-кнопки — клик должен пройти в игру
- Кликнуть по HUD-кнопке — клик обрабатывается, фокус остаётся в игре

---

<a id="задача-6"></a>
## Задача 6 — Проверить корректность жизненного цикла HUD-кнопок (показ/скрытие)

### Описание проблемы

Кнопки создаются в `OnInitMuWindow` (строка 182), но показываются только после  
получения `WM_CHAR_SELECTED` от `CharInfoFilter`. Между созданием и показом  
могут пройти минуты (экран выбора персонажа).

**Потенциальные проблемы:**

1. Если игрок **выходит из игры** (возвращается на экран выбора персонажа) и затем  
   **входит снова** — `WM_CHAR_SELECTED` отправляется повторно, но `Show()` уже  
   вызывался и `m_bEnabled = TRUE`.

2. Флаг `m_bGameActive` инициализируется как `TRUE` в конструкторе (строка 16),  
   но фактически игра может быть не в фокусе в момент создания HUD.

3. Таймер `REPOSITION_TIMER_ID` (200ms) может накапливать вызовы если `Show()`  
   вызывается многократно — `SetTimer()` с тем же ID перезапускает таймер,  
   так что это безопасно.

### Подпункты выполнения

**6.1.** В конструкторе `CHUDButtons` (строка 16), изменить начальное значение:
```cpp
m_bGameActive = FALSE;  // Было TRUE — неверно до первого WM_ACTIVATEAPP
```
Это безопасно: `SetGameActive(TRUE)` будет вызван из `OnActivateApp` при получении  
фокуса.

**6.2.** Добавить обработку повторного входа в игру. В `CHUDButtons::Show()`:
```cpp
void CHUDButtons::Show()
{
    if (!IsWindow())
        return;

    m_bEnabled = TRUE;

    SetTimer(REPOSITION_TIMER_ID, 200, NULL);

    if (!m_bGameActive || IsWindowVisible())
        return;

    Reposition();
}
```
Текущий код корректен — повторный вызов `Show()` безопасен.

**6.3.** Добавить метод `CHUDButtons::Reset()` для сценария выхода из игры  
(возврат на экран выбора персонажа):
```cpp
void CHUDButtons::Reset()
{
    m_bEnabled = FALSE;
    m_bClickerRunning = FALSE;
    if (IsWindow())
    {
        KillTimer(REPOSITION_TIMER_ID);
        if (IsWindowVisible())
            ShowWindow(SW_HIDE);
        InvalidateRect(NULL, FALSE);
    }
}
```

**6.4.** Вызвать `Reset()` при получении пакета о выходе из игрового мира  
(если такой обработчик существует) или при отключении от сервера.

**6.5.** Скомпилировать и протестировать:
- Выбрать персонажа → кнопки появляются
- Выйти из игры на экран выбора → кнопки скрываются
- Выбрать другого персонажа → кнопки снова появляются
- Alt+Tab из игры → кнопки скрываются
- Alt+Tab обратно → кнопки появляются

---

<a id="задача-7"></a>
## Задача 7 — Проверить компиляцию и интеграцию в процесс сборки

### Описание проблемы

Проект использует **Visual Studio MSBuild** (файл `AutoClicker.vcxproj`).  
HUD-кнопки должны быть корректно интегрированы в процесс компиляции.

### Подпункты выполнения

**7.1.** Открыть `src/LordOfClicker/AutoClicker.vcxproj` и убедиться что:

- [ ] `HUDButtons.cpp` присутствует в секции `<ClCompile>`:
  ```xml
  <ClCompile Include="HUDButtons.cpp" />
  ```

- [ ] `HUDButtons.h` присутствует в секции `<ClInclude>`:
  ```xml
  <ClInclude Include="HUDButtons.h" />
  ```

- [ ] `HistoryDialog.cpp` присутствует в секции `<ClCompile>`:
  ```xml
  <ClCompile Include="HistoryDialog.cpp" />
  ```

- [ ] `HistoryDialog.h` присутствует в секции `<ClInclude>`:
  ```xml
  <ClInclude Include="HistoryDialog.h" />
  ```

- [ ] Ресурсные файлы:
  ```xml
  <ResourceCompile Include="AutoClicker.rc" />
  ```

- [ ] Bitmap-файлы:
  ```xml
  <Image Include="res\ico_settings.bmp" />
  <Image Include="res\ico_play.bmp" />
  <Image Include="res\ico_stop.bmp" />
  <Image Include="res\ico_history.bmp" />
  ```

**7.2.** Открыть `src/LordOfClicker/AutoClicker.rc` и убедиться что bitmap-ресурсы  
определены с правильными ID:
```rc
IDB_ICO_SETTINGS        BITMAP  "res\\ico_settings.bmp"
IDB_ICO_PLAY            BITMAP  "res\\ico_play.bmp"
IDB_ICO_STOP            BITMAP  "res\\ico_stop.bmp"
IDB_ICO_HISTORY         BITMAP  "res\\ico_history.bmp"
```

**7.3.** Убедиться что `resource.h` содержит корректные ID:
```cpp
#define IDB_ICO_SETTINGS    523
#define IDB_ICO_PLAY        524
#define IDB_ICO_STOP        525
#define IDB_ICO_HISTORY     526
#define IDD_HISTORY_DLG     530
#define IDC_HISTORY_LIST    531
```

**7.4.** Проверить что `#include "resource.h"` присутствует в `HUDButtons.cpp`  
(через `stdafx.h` или напрямую). Текущий `HUDButtons.cpp` включает `stdafx.h`  
(строка 1), который должен включать `resource.h`.

**7.5.** Открыть `src/LordOfClicker/stdafx.h` и убедиться что `resource.h` включён:
```cpp
#include "resource.h"
```

**7.6.** Убедиться что `HUDButtons.cpp` компилируется с теми же настройками  
(Configuration Type, Runtime Library, Character Set) что и остальные файлы проекта.

**7.7.** Выполнить **полную пересборку** (Rebuild All) проекта `AutoClicker`:
```
msbuild AutoClicker.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=Win32
```

**7.8.** Проверить что в выходном файле (DLL или EXE) присутствуют все ресурсы.  
Использовать `Resource Hacker` или `dumpbin /resources` для проверки.

---

<a id="задача-8"></a>
## Задача 8 — Проверить корректность ресурсов (bitmap-иконки)

### Описание проблемы

HUD-кнопки загружают bitmap-иконки через `LoadImage`:
```cpp
m_hIcoSettings = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_ICO_SETTINGS), 
                                    IMAGE_BITMAP, 0, 0, 0);
```

Если `hInstance` указывает на **неправильный модуль** (не тот DLL в котором  
скомпилированы ресурсы) — `LoadImage` вернёт NULL и иконки не отобразятся.

### Подпункты выполнения

**8.1.** В `CHUDButtons::Create()` после загрузки иконок, добавить проверку:
```cpp
if (!m_hIcoSettings || !m_hIcoPlay || !m_hIcoStop || !m_hIcoHistory)
{
    // Логировать ошибку — ресурсы не найдены в модуле hInstance
    OutputDebugStringA("HUD: Failed to load icon bitmaps!\n");
    // Попробовать загрузить из NULL (основной EXE)
    if (!m_hIcoSettings)
        m_hIcoSettings = (HBITMAP)LoadImage(NULL, MAKEINTRESOURCE(IDB_ICO_SETTINGS), 
                                            IMAGE_BITMAP, 0, 0, 0);
    // ... аналогично для остальных
}
```

**8.2.** Проверить что `_AtlBaseModule.GetModuleInstance()` в `OnInitMuWindow`  
возвращает HINSTANCE модуля `AutoClicker.dll` (в котором скомпилированы ресурсы),  
а не `main.exe` (игровой клиент).

**8.3.** Если ресурсы находятся в другом модуле, передать правильный HINSTANCE:
```cpp
// В MuWindow.cpp, OnInitMuWindow:
HINSTANCE hResMod = _AtlBaseModule.GetModuleInstance();
// или если ресурсы в другом DLL:
// HINSTANCE hResMod = GetModuleHandle(_T("AutoClicker.dll"));
m_cHUDButtons.Create(m_hWnd, hResMod);
```

**8.4.** Убедиться что bitmap-файлы (`res/ico_*.bmp`) имеют корректный формат:
- Формат: Windows BMP
- Глубина цвета: 24-bit (без альфа-канала)
- Фоновый цвет: `RGB(25, 22, 16)` (используется в `TransparentBlt`)
- Размер: любой (масштабируется через `TransparentBlt`)

**8.5.** Скомпилировать и проверить через отладчик:
- Установить точку останова на `LoadImage` в `CHUDButtons::Create()`
- Убедиться что все 4 вызова возвращают ненулевой HBITMAP
- Проверить значение `hInstance`

---

<a id="задача-9"></a>
## Задача 9 — Проверить связь между DLL-модулями (CharInfoFilter → MuWindow)

### Описание проблемы

`WM_CHAR_SELECTED` отправляется из `LordOfMUdll` (CharInfoFilter) в `AutoClicker.dll`  
(MuWindow) через `PostMessageToMuWindow`. Эта функция ищет окно по классу:
```cpp
static HWND FindMuWindow()
{
    HWND hwnd = 0;
    EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);
    return hwnd;
}
```

Функция `IsMuWindow` проверяет имена классов: `"MU"`, `"DIEmWin"`, `"MuOnline"`, и т.д.

**Потенциальная проблема:** CMuWindow использует `SubclassWindow` — он не создаёт  
новое окно, а подменяет оконную процедуру существующего. `FindMuWindow` должна  
найти **оригинальное** окно игры, и сообщения будут направлены в подменённую процедуру  
CMuWindow. Это **должно работать корректно**, если:
1. SubclassWindow было вызвано до первого WM_CHAR_SELECTED
2. Оконный класс игры совпадает с одним из проверяемых имён

### Подпункты выполнения

**9.1.** Добавить логирование в `CharInfoFilter.cpp` при отправке WM_CHAR_SELECTED:
```cpp
CMuWindowUtil::PostMessageToMuWindow(WM_CHAR_SELECTED, 0, 0);
// Добавить:
WriteHookLog("CharInfo: WM_CHAR_SELECTED posted to MuWindow (hwnd=0x%X)", hwndMU);
```

**9.2.** Добавить логирование в `CMuWindow::OnCharSelected`:
```cpp
LRESULT CMuWindow::OnCharSelected(UINT, WPARAM, LPARAM, BOOL&)
{
    WriteClickerLog("OnCharSelected: Showing HUD buttons");
    m_cHUDButtons.Show();
    return 0;
}
```

**9.3.** Проверить что `PostMessageToMuWindow` находит правильное окно.  
Добавить в `MuWindowUtil.h` логирование для `FindMuWindow`.

**9.4.** Проверить что определение `WM_CHAR_SELECTED` **идентично** в обоих файлах:
- `src/LordOfClicker/MuWindow.h:26` → `(WM_APP + 510)` (после исправления из Задачи 1)
- `src/LordOfMUdll/CharInfoFilter.cpp:7` → `(WM_APP + 510)` (после исправления)

Если значения различаются, сообщение не будет обработано!

**9.5.** Рекомендация: вынести определение `WM_CHAR_SELECTED` в общий заголовочный  
файл `src/_Shared/CommonMessages.h` (создать если не существует) чтобы избежать  
рассогласования:
```cpp
#ifndef __COMMON_MESSAGES_H
#define __COMMON_MESSAGES_H

#define WM_CHAR_SELECTED    (WM_APP + 510)

#endif
```

**9.6.** Скомпилировать оба проекта и протестировать:
- Зайти в игру
- Выбрать персонажа
- Проверить лог — WM_CHAR_SELECTED должен быть отправлен и получен
- Кнопки HUD должны появиться

---

<a id="задача-10"></a>
## Задача 10 — Интеграционное тестирование и валидация

### Описание

После выполнения всех предыдущих задач, необходимо провести полное тестирование.

### Подпункты выполнения

**10.1. Тест: Появление кнопок при входе в игру**
- [ ] Запустить игру
- [ ] Ввести логин/пароль, подключиться к серверу
- [ ] Выбрать персонажа и войти в игровой мир
- [ ] Убедиться что 3 HUD-кнопки (Settings, Start/Stop, History) появились
- [ ] Убедиться что кнопки расположены в правильном месте (не перекрывают UI)

**10.2. Тест: Функциональность кнопки Settings**
- [ ] Нажать ЛКМ по кнопке Settings (шестерёнка)
- [ ] Убедиться что диалог настроек открывается
- [ ] Убедиться что игра продолжает рендериться за диалогом
- [ ] Закрыть диалог (кнопка Apply/Cancel или ESC)
- [ ] Убедиться что диалог закрылся и m_fGuiActive сбросился

**10.3. Тест: Функциональность кнопки Start/Stop**
- [ ] Нажать ЛКМ по кнопке Start (треугольник Play)
- [ ] Убедиться что автокликер запустился
- [ ] Убедиться что иконка сменилась на Stop (квадрат)
- [ ] Нажать ЛКМ по кнопке Stop
- [ ] Убедиться что автокликер остановился
- [ ] Убедиться что иконка сменилась обратно на Play

**10.4. Тест: Функциональность кнопки History**
- [ ] Нажать ЛКМ по кнопке History
- [ ] Убедиться что диалог истории открывается
- [ ] Убедиться что история пикапа отображается (если были подобранные предметы)
- [ ] Закрыть диалог

**10.5. Тест: Взаимодействие кнопок с автокликером**
- [ ] Запустить автокликер через HUD-кнопку Start
- [ ] Попытаться нажать Settings — проверить поведение (см. Задачу 3)
- [ ] Попытаться нажать History — проверить что диалог открывается
- [ ] Остановить автокликер через HUD-кнопку Stop
- [ ] Убедиться что кнопки реагируют на ЛКМ во время работы автокликера

**10.6. Тест: Alt+Tab и потеря фокуса**
- [ ] При видимых кнопках — Alt+Tab в другое приложение
- [ ] Убедиться что кнопки скрылись
- [ ] Alt+Tab обратно в игру
- [ ] Убедиться что кнопки снова появились

**10.7. Тест: Перемещение игрового окна (windowed mode)**
- [ ] Перетащить окно игры
- [ ] Убедиться что кнопки следуют за окном
- [ ] Изменить размер окна
- [ ] Убедиться что кнопки остались на правильной позиции

**10.8. Тест: Множественные нажатия и edge-cases**
- [ ] Быстро кликнуть по кнопке несколько раз подряд
- [ ] Нажать на кнопку и отвести мышь (cancel)
- [ ] Нажать одновременно HUD-кнопку и клавишу F5
- [ ] Открыть Settings и нажать F5 — кликер НЕ должен запускаться

**10.9. Тест: Повторный вход в игру**
- [ ] Выйти из игры на экран выбора персонажа
- [ ] Убедиться что кнопки скрылись (если реализован Reset)
- [ ] Войти снова — кнопки должны появиться

**10.10. Тест: Производительность**
- [ ] Проверить FPS игры с видимыми HUD-кнопками
- [ ] Убедиться что таймер Reposition (200ms) не создаёт значительной нагрузки
- [ ] Проверить потребление памяти (bitmap-ресурсы, GDI-объекты)

---

## ПРИОРИТЕТЫ ИСПРАВЛЕНИЙ

| Приоритет | Задача | Критичность | Описание |
|-----------|--------|-------------|----------|
| 🔴 1 | Задача 1 | Высокая | Скобки в макросах — потенциальный источник багов |
| 🔴 2 | Задача 5.4 | Высокая | WM_NCHITTEST + HTTRANSPARENT — клики проваливаются сквозь пустое пространство |
| 🟡 3 | Задача 2 | Средняя | Конфликт capture при работающем автокликере |
| 🟡 4 | Задача 3 | Средняя | Блокировка Settings при работающем кликере |
| 🟡 5 | Задача 6 | Средняя | Жизненный цикл — начальное значение m_bGameActive |
| 🟢 6 | Задача 4 | Низкая | Увеличение размера кнопок и DPI |
| 🟢 7 | Задача 5.1 | Низкая | Блокировка F5-F8 при открытом диалоге |
| 🟢 8 | Задача 8 | Низкая | Проверка загрузки bitmap-ресурсов |
| 🟢 9 | Задача 9 | Низкая | Логирование для отладки |
| ⚪ 10 | Задача 7 | Проверка | Верификация сборки |

---

## РЕЗЮМЕ

Основные причины, по которым кнопки Settings, Start/Stop Clicker и History Action  
могут не работать в игровом клиенте:

1. **Макросы WM-сообщений без скобок** — в редких контекстах может привести к  
   неправильному вычислению номера сообщения.

2. **Конфликт `WM_CAPTURECHANGED`** — обработчик CMuWindow блокирует capture-переход  
   при работающем автокликере, что может повлиять на `SetCapture()` в HUD-кнопках.

3. **Блокировка Settings** (`if (m_pClicker != NULL) return 0;`) — при работающем  
   автокликере кнопка Settings полностью неактивна без обратной связи.

4. **Отсутствие `WM_NCHITTEST` обработчика** — клики по пустому пространству HUD-окна  
   (между кнопками или на фоне) поглощаются и не передаются в игру, создавая впечатление  
   "мёртвой зоны" поверх игрового окна.

5. **Начальное значение `m_bGameActive = TRUE`** — если окно не в фокусе при создании HUD,  
   кнопки могут показаться некорректно.

6. **Конфликт клавиатурных хоткеев** — F5/F8 работают одновременно с HUD-кнопками  
   при открытых диалогах, что может привести к неожиданному поведению.

7. **Ресурсы могут не загружаться** — если `hInstance` указывает на неправильный модуль,  
   `LoadImage` вернёт NULL и иконки не отобразятся (кнопки будут пустыми).
