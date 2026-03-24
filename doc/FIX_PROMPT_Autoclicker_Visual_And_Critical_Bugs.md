# Промт для исправления визуальных и критических багов автокликера

> **Дата анализа:** 2026-03-24
> **Анализируемые файлы:** HUDButtons.cpp/h, MuWindow.cpp/h, UnifiedSettingsDlg.cpp/h, HistoryDialog.cpp/h, SettingsDlg.cpp/h, MuTheme.cpp/h, ClickerJob.cpp/h, resource.h, AutoClicker.rc

---

## ПРОМТ ДЛЯ ИСПРАВЛЕНИЯ:

Исправь все найденные ниже баги и ошибки в коде автокликера LordOfMU. Каждый пункт содержит описание бага, его локацию в коде и инструкции по исправлению.

---

## 0. ОШИБКИ КОМПИЛЯЦИИ (проект не собирается)

> Эти ошибки БЛОКИРУЮТ сборку проекта и должны быть исправлены В ПЕРВУЮ ОЧЕРЕДЬ перед всеми остальными пунктами.

### 0.1. MuWindow.cpp — конфликт макроса SubclassWindow из `<windowsx.h>` с методом ATL CWindowImpl::SubclassWindow

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строка 108

**Ошибки компиляции:**
```
warning C4003: недостаточно аргументов для вызова макроса, подобного функции, "SubclassWindow"
error C2059: синтаксическая ошибка: (
error C2143: синтаксическая ошибка: отсутствие ";" перед "{"
```

**Причина:** В `MuWindow.cpp` строка 13 подключается `<windowsx.h>`, который определяет макрос:
```cpp
#define SubclassWindow(hwnd, lpfn) ((WNDPROC)SetWindowLongPtr((hwnd), GWLP_WNDPROC, (LPARAM)(WNDPROC)(lpfn)))
```
Этот макрос принимает 2 аргумента `(hwnd, lpfn)`, но вызов ATL-метода `s_pInstance->SubclassWindow(hwndMuWindow)` на строке 108 передаёт только 1 аргумент. Препроцессор видит макрос с одним аргументом вместо двух и выдаёт ошибку.

**Исправление:** Добавить `#undef SubclassWindow` после `#include <windowsx.h>` в MuWindow.cpp, чтобы убрать конфликтующий макрос и позволить вызвать ATL-метод `CWindowImpl::SubclassWindow`:

```cpp
// БЫЛО (строки 13-14):
#include <windowsx.h>
#include <sys/stat.h>

// СТАЛО:
#include <windowsx.h>
#ifdef SubclassWindow
#undef SubclassWindow
#endif
#include <sys/stat.h>
```

**Альтернативное исправление:** Если `SubclassWindow` из `<windowsx.h>` используется где-то ещё в файле (не используется), можно ограничить undef только вокруг вызова. Но поскольку в MuWindow.cpp макрос SubclassWindow нигде больше не используется, глобальный undef после include — самое чистое решение.

### 0.2. HUDButtons.cpp — тип CRect не объявлен (отсутствует `<atltypes.h>`)

**Файл:** `src/LordOfClicker/HUDButtons.cpp`, строка 69

**Ошибки компиляции:**
```
error C2065: CRect: необъявленный идентификатор
error C2146: синтаксическая ошибка: отсутствие ";" перед идентификатором "rcPos"
error C3861: rcPos: идентификатор не найден
error C2065: rcPos: необъявленный идентификатор
```

**Причина:** На строке 69 используется `CRect rcPos(0, 0, barWidth, barHeight);`, но `CRect` — это ATL-класс из заголовка `<atltypes.h>`, который НЕ подключён ни в `HUDButtons.cpp`, ни в `stdafx.h`. Файл `stdafx.h` включает `<atlbase.h>`, `<atlcom.h>`, `<atlwin.h>`, `<atlstr.h>`, но НЕ `<atltypes.h>`.

**Исправление (вариант A — рекомендуемый):** Заменить `CRect` на стандартный Win32 тип `RECT`, который не требует дополнительных заголовков:

```cpp
// БЫЛО (строка 69):
CRect rcPos(0, 0, barWidth, barHeight);

// СТАЛО:
RECT rcPos = { 0, 0, barWidth, barHeight };
```

**Исправление (вариант B — альтернативный):** Добавить `#include <atltypes.h>` в `stdafx.h` или в начало `HUDButtons.cpp`:

```cpp
// В stdafx.h после #include <atlstr.h> (строка 41):
#include <atltypes.h>   // CRect, CPoint, CSize
```

> **Рекомендация:** Вариант A предпочтительнее, так как `CRect` используется только в одном месте, а `RECT` — стандартный Win32 тип, доступный везде. Это также уменьшает зависимость от ATL-расширений.

---

## 1. КРИТИЧЕСКИЕ БАГИ (функциональность сломана)

### 1.1. OnLaunchMu ПОЛНОСТЬЮ СЛОМАН — диалог запуска нового экземпляра MU никогда не показывается

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строка ~790

**Проблема:** В `OnLaunchMu` есть проверка `!m_cSettingsDlg.IsWindow()`, но `m_cSettingsDlg.Create()` закомментирован (строка ~179), поэтому `IsWindow()` ВСЕГДА возвращает `FALSE`. Условие `if (m_fGuiActive || !m_cSettingsDlg.IsWindow())` ВСЕГДА TRUE — функция всегда возвращает 0, диалог IDD_LAUNCH_MESSAGE (кнопка F10) никогда не показывается.

**Исправление:** Заменить проверку `!m_cSettingsDlg.IsWindow()` на `!m_cUnifiedSettingsDlg.IsWindow()`, так как CSettingsDlg больше не создаёт окно и используется только как хранилище настроек:

```cpp
// БЫЛО:
if (m_fGuiActive || !m_cSettingsDlg.IsWindow())
    return 0;

// СТАЛО:
if (m_fGuiActive || !m_cUnifiedSettingsDlg.IsWindow())
    return 0;
```

### 1.2. ESC-обработчик НЕ сбрасывает m_fGuiActive при закрытии диалогов

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки ~373-382

**Проблема:** При нажатии ESC диалоги скрываются (`ShowWindow(SW_HIDE)`), но `m_fGuiActive` не устанавливается в `FALSE`. Сброс происходит только через таймер 1011 (100 мс задержка), что создаёт временной промежуток, в течение которого F5-F9 остаются заблокированы, а новые диалоги не могут быть открыты.

**Исправление:** Добавить `m_fGuiActive = FALSE;` в ESC-обработчик после скрытия всех диалогов:

```cpp
if (vkCode == VK_ESCAPE && uMsg == WM_KEYUP && m_fGuiActive)
{
    if (m_cSettingsDlg.IsWindow())
        m_cSettingsDlg.ShowWindow(SW_HIDE);
    m_cUnifiedSettingsDlg.ShowWindow(SW_HIDE);
    m_cHistoryDlg.ShowWindow(SW_HIDE);
    m_bSettingsWasVisible = FALSE;
    m_bHistoryWasVisible = FALSE;
    m_fGuiActive = FALSE;  // <-- ДОБАВИТЬ ЭТУ СТРОКУ
    return TRUE;
}
```

### 1.3. OnClickedOK/OnClickedCancel в UnifiedSettingsDlg и HistoryDialog НЕ уведомляют родительское окно

**Файлы:**
- `src/LordOfClicker/UnifiedSettingsDlg.cpp`, строки ~634-649 (OnClickedOK, OnClickedCancel)
- `src/LordOfClicker/HistoryDialog.cpp`, строки ~214-225 (OnClickedOK, OnClickedCancel)

**Проблема:** При нажатии Apply/Cancel/Close в диалогах, они только скрываются через `ShowWindow(SW_HIDE)`, но НЕ уведомляют CMuWindow о закрытии. Сброс `m_fGuiActive` зависит ТОЛЬКО от таймера 1011 (100 мс поллинг), что ненадёжно и создаёт задержку.

**Исправление:** В каждом обработчике OK/Cancel добавить уведомление родительского окна ПОСЛЕ `ShowWindow(SW_HIDE)`:

Для **UnifiedSettingsDlg** — OnClickedOK и OnClickedCancel:
```cpp
ShowWindow(SW_HIDE);
// Уведомить родителя что диалог закрыт — m_fGuiActive будет сброшен
// (таймер 1011 также проверяет видимость, но это ускоряет сброс)
```

> **Примечание:** Таймер 1011 уже обрабатывает сброс m_fGuiActive при проверке видимости диалогов. Однако для мгновенного отклика можно добавить прямой `PostMessage(GetParent(), WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), 0)` или определить отдельное сообщение `WM_DIALOG_CLOSED` — но самый безопасный подход: **сброс m_fGuiActive уже делается таймером, поэтому достаточно исправить ESC-обработчик (п. 1.2)**. Это наименее инвазивное решение.

---

## 2. БАГИ КУРСОРА (визуальные проблемы)

### 2.1. HUD-кнопки (CHUDButtons) НЕ имеют обработчика WM_SETCURSOR — при наведении показывается стандартная стрелка вместо MU-курсора

**Файл:** `src/LordOfClicker/HUDButtons.h`, строки 42-52 (BEGIN_MSG_MAP)
**Файл:** `src/LordOfClicker/HUDButtons.cpp`

**Проблема:** В CHUDButtons::BEGIN_MSG_MAP отсутствует `MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)`. При наведении мыши на кнопки HUD (Settings, Start/Stop, History) показывается стандартная стрелка Windows вместо тематического MU-курсора. Поскольку CHUDButtons — это popup-окно с WS_EX_NOACTIVATE, оно обрабатывает курсор самостоятельно, а не через родительское окно.

**Исправление:**

**a)** Добавить в `HUDButtons.h`:
- Добавить в message map: `MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)`
- Добавить объявление: `LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);`
- Добавить поле: `HCURSOR m_hMuCursor;` для хранения загруженного курсора

**b)** Добавить в `HUDButtons.cpp`:
- В `Create()` — загрузить курсор: `m_hMuCursor = (HCURSOR)LoadImage(hInstance, MAKEINTRESOURCE(IDC_MU_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);`
- В `Destroy()` — освободить курсор: `if (m_hMuCursor) { DestroyCursor(m_hMuCursor); m_hMuCursor = NULL; }`
- Реализовать обработчик:
```cpp
LRESULT CHUDButtons::OnSetCursor(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
    if (LOWORD(lParam) == HTCLIENT && m_hMuCursor != NULL)
    {
        SetCursor(m_hMuCursor);
        bHandled = TRUE;
        return TRUE;
    }
    bHandled = FALSE;
    return FALSE;
}
```

**c)** Инициализировать `m_hMuCursor = NULL;` в конструкторе.

### 2.2. HistoryDialog использует стандартную стрелку вместо MU-курсора — несоответствие с UnifiedSettingsDlg

**Файл:** `src/LordOfClicker/HistoryDialog.cpp`, строки 120-124

**Проблема:** `HistoryDialog::OnSetCursor` устанавливает `LoadCursor(NULL, IDC_ARROW)` — стандартную стрелку, в то время как UnifiedSettingsDlg использует тематический MU-курсор из `m_cTheme.GetMuCursor()`. Визуальное несоответствие: при переключении между Settings и History диалогами курсор меняется.

**Исправление:** Обновить `HistoryDialog::OnSetCursor` для использования MU-курсора:

```cpp
LRESULT CHistoryDialog::OnSetCursor(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
    if (LOWORD(lParam) == HTCLIENT)
    {
        HCURSOR hCursor = m_cTheme.GetMuCursor();
        if (hCursor == NULL)
            hCursor = LoadCursor(NULL, IDC_ARROW);
        SetCursor(hCursor);
        bHandled = TRUE;
        return TRUE;
    }
    bHandled = FALSE;
    return FALSE;
}
```

А также обновить `HistoryDialog::OnShowWindow` (строки 91-117) — при показе диалога устанавливать MU-курсор вместо IDC_ARROW:

```cpp
// БЫЛО:
m_hOldCursor = SetCursor(LoadCursor(NULL, IDC_ARROW));

// СТАЛО:
HCURSOR hCursor = m_cTheme.GetMuCursor();
if (hCursor == NULL)
    hCursor = LoadCursor(NULL, IDC_ARROW);
m_hOldCursor = SetCursor(hCursor);
```

### 2.3. WM_SETCURSOR в UnifiedSettingsDlg не обрабатывает non-client область корректно

**Файл:** `src/LordOfClicker/UnifiedSettingsDlg.cpp`, строки 366-377

**Проблема:** OnSetCursor проверяет `LOWORD(lParam) == HTCLIENT`, но при наведении на title bar, рамку окна или кнопки закрытия — курсор не устанавливается (bHandled = FALSE, возвращается FALSE). Это может привести к мерцанию курсора между MU-курсором и стрелкой при перемещении мыши между client и non-client областями.

**Исправление:** Для non-client областей следует также устанавливать стандартную стрелку явно, либо оставить текущее поведение (приемлемо для non-client — Windows сам управляет курсором для resize/close/title):

```cpp
LRESULT CUnifiedSettingsDlg::OnSetCursor(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    WORD hitTest = LOWORD(lParam);
    if (hitTest == HTCLIENT)
    {
        HCURSOR hCursor = m_cTheme.GetMuCursor();
        if (hCursor != NULL)
        {
            SetCursor(hCursor);
            bHandled = TRUE;
            return TRUE;
        }
    }
    // Для non-client области — позволить Windows управлять курсором
    bHandled = FALSE;
    return FALSE;
}
```

> **Статус:** Текущая реализация работает корректно для большинства случаев. Это LOW-priority улучшение.

---

## 3. ВИЗУАЛЬНЫЕ БАГИ КНОПОК И ОТРИСОВКИ

### 3.1. Owner-draw кнопки в HistoryDialog НЕ поддерживают hover-состояние

**Файл:** `src/LordOfClicker/HistoryDialog.cpp`, строки 190-211 (OnDrawItem)

**Проблема:** В `OnDrawItem` для кнопки "Close" в HistoryDialog, `bHover` всегда `FALSE`:
```cpp
BOOL bHover = FALSE;  // Никогда не становится TRUE
CMuTheme::DrawMuButton(lpDIS->hDC, lpDIS->rcItem, szText, bPressed, bHover);
```
Кнопка не подсвечивается при наведении мыши, что выглядит "мёртвой".

**Исправление:** Для реализации hover-эффекта на owner-draw кнопках нужно:

**a)** Добавить отслеживание hover-состояния. Простейший подход — проверить позицию курсора в момент отрисовки:

```cpp
// Определить hover-состояние
POINT pt;
GetCursorPos(&pt);
RECT rcScreen;
::GetWindowRect(lpDIS->hwndItem, &rcScreen);
BOOL bHover = PtInRect(&rcScreen, pt);
```

**b)** Либо реализовать полноценный WM_MOUSEMOVE + TME_LEAVE tracking (более сложный подход с subclass кнопки).

### 3.2. Owner-draw кнопки в UnifiedSettingsDlg используют ODS_FOCUS как proxy для hover — некорректно

**Файл:** `src/LordOfClicker/UnifiedSettingsDlg.cpp`, строки 847-884 (OnDrawItem)

**Проблема:** В обработчике OnDrawItem для кнопок "Apply" и "Cancel":
```cpp
BOOL bFocused = (lpDIS->itemState & ODS_FOCUS) != 0;
CMuTheme::DrawMuButton(lpDIS->hDC, lpDIS->rcItem, szText, bPressed, bFocused);
```
`ODS_FOCUS` указывает на фокус ввода (Tab-навигация), а НЕ на hover мыши. Кнопка подсвечена, когда имеет фокус клавиатуры, но не при наведении мыши.

**Исправление:** Аналогично п. 3.1 — проверять реальное положение курсора:

```cpp
POINT pt;
GetCursorPos(&pt);
RECT rcScreen;
::GetWindowRect(lpDIS->hwndItem, &rcScreen);
BOOL bHover = PtInRect(&rcScreen, pt);

CMuTheme::DrawMuButton(lpDIS->hDC, lpDIS->rcItem, szText, bPressed, bHover);
```

### 3.3. Кнопки Apply/Cancel/Close не перерисовываются при входе/выходе мыши

**Файлы:**
- `src/LordOfClicker/UnifiedSettingsDlg.cpp` (Apply, Cancel)
- `src/LordOfClicker/HistoryDialog.cpp` (Close)

**Проблема:** Owner-draw кнопки перерисовываются только при изменении состояния (нажатие, фокус). Без WM_MOUSEMOVE отслеживания на самих кнопках, hover-подсветка не будет работать даже с исправлением из п. 3.1/3.2, потому что InvalidateRect не вызывается при перемещении мыши.

**Исправление:** Добавить subclass для owner-draw кнопок с WM_MOUSEMOVE + TME_LEAVE обработкой:

```cpp
// В OnInitDialog — subclass кнопок:
SetWindowSubclass(GetDlgItem(IDOK), ButtonHoverSubclassProc, 100, 0);
SetWindowSubclass(GetDlgItem(IDCANCEL), ButtonHoverSubclassProc, 101, 0);

// Subclass proc:
LRESULT CALLBACK ButtonHoverSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
    UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    static BOOL s_bHover = FALSE;

    switch (uMsg)
    {
    case WM_MOUSEMOVE:
        if (!s_bHover)
        {
            s_bHover = TRUE;
            TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hWnd, 0 };
            TrackMouseEvent(&tme);
            ::InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

    case WM_MOUSELEAVE:
        s_bHover = FALSE;
        ::InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_NCDESTROY:
        RemoveWindowSubclass(hWnd, ButtonHoverSubclassProc, uIdSubclass);
        break;
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
```

> **Примечание:** Если кнопок несколько, hover-состояние нужно хранить per-window (через `dwRefData` или window property), а не в статической переменной.

---

## 4. БАГИ ЖИЗНЕННОГО ЦИКЛА ДИАЛОГОВ

### 4.1. Legacy код: m_cSettingsDlg.IsWindow() в ESC-обработчике — мёртвая проверка

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки ~375-376

**Проблема:** ESC-обработчик проверяет `m_cSettingsDlg.IsWindow()`, но Create() для CSettingsDlg закомментирован. Проверка всегда FALSE, ShowWindow(SW_HIDE) никогда не вызывается. Это мёртвый код.

**Исправление:** Удалить мёртвый код:

```cpp
// УДАЛИТЬ ЭТИ СТРОКИ:
if (m_cSettingsDlg.IsWindow())
    m_cSettingsDlg.ShowWindow(SW_HIDE);
```

### 4.2. HidePopupDialogs/RestorePopupDialogs — гонка с пользовательским вводом

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки ~1723-1755

**Проблема потенциальная:** При быстром ALT+TAB → возврат → ALT+TAB, RestorePopupDialogs() сбрасывает `m_bSettingsWasVisible = FALSE` и `m_bHistoryWasVisible = FALSE` ПОСЛЕ показа диалогов. Если HidePopupDialogs() вызывается до завершения ShowWindow(), флаги уже сброшены, и при следующем ALT+TAB диалоги не будут восстановлены.

**Исправление:** Сбрасывать флаги ДО показа окон или использовать атомарную операцию:

```cpp
void CMuWindow::RestorePopupDialogs()
{
    BOOL bRestoreSettings = m_bSettingsWasVisible;
    BOOL bRestoreHistory = m_bHistoryWasVisible;

    // Сбросить флаги ПЕРЕД показом — защита от повторного вызова
    m_bSettingsWasVisible = FALSE;
    m_bHistoryWasVisible = FALSE;

    if (bRestoreSettings && m_cUnifiedSettingsDlg.IsWindow())
    {
        m_cUnifiedSettingsDlg.ShowWindow(SW_SHOWNOACTIVATE);
        ::SetWindowPos(m_cUnifiedSettingsDlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }
    if (bRestoreHistory && m_cHistoryDlg.IsWindow())
    {
        m_cHistoryDlg.ShowWindow(SW_SHOWNOACTIVATE);
        ::SetWindowPos(m_cHistoryDlg.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }
}
```

> **Статус:** Потенциальная проблема при быстром ALT+TAB. LOW-priority.

---

## 5. БАГИ ОТОБРАЖЕНИЯ ЭЛЕМЕНТОВ УПРАВЛЕНИЯ

### 5.1. ComboBox dropdown-список в Class tab — курсор не MU-стиль

**Файлы:** Все class child dialogs (DarkLordSettings, EElfSettings, AElfSettings, BKSettings, SMSettings, MGSettings)

**Проблема:** Когда пользователь открывает ComboBox dropdown-список (например, выбор skill slot), popup ListBox — это отдельное окно. WM_SETCURSOR для popup ListBox НЕ перенаправляется в CUnifiedSettingsDlg, поэтому показывается стандартная стрелка.

**Исправление:** Это ожидаемое поведение для ComboBox dropdown — стандартная стрелка является нормой для dropdown list. **НЕ ТРЕБУЕТ ИСПРАВЛЕНИЯ** если не требуется 100% кастомизация.

Если всё же нужен MU-курсор в dropdown:
- Нужно subclass'ить dropdown ListBox каждого ComboBox
- Обрабатывать WM_SETCURSOR в subclass proc

### 5.2. Checkbox-subclass не инвалидирует при клике — возможна задержка перерисовки

**Файл:** `src/LordOfClicker/UnifiedSettingsDlg.cpp`, строки 1166-1310 (CheckboxSubclassProc)

**Проблема потенциальная:** CheckboxSubclassProc обрабатывает WM_PAINT и WM_ENABLE, но НЕ обрабатывает BM_SETCHECK напрямую. При клике на checkbox, Windows вызывает BM_SETCHECK (который переключает состояние), затем отправляет WM_PAINT. Subclass proc рисует правильное состояние в WM_PAINT на основе BM_GETCHECK. Это должно работать корректно.

**Статус:** Работает корректно через стандартный механизм. **НЕ ТРЕБУЕТ ИСПРАВЛЕНИЯ.**

### 5.3. Tab control border артефакты при переключении вкладок

**Файл:** `src/LordOfClicker/UnifiedSettingsDlg.cpp`, строки 1086-1160 (TabSubclassProc)

**Проблема потенциальная:** В WM_PAINT обработчике TabSubclassProc: сначала вызывается DefSubclassProc (рисует стандартные табы), затем поверх рисуется dark-gold border. Между этими операциями может быть видна мерцание (flash of white/standard border).

**Исправление (если мерцание видимо):** Использовать двойную буферизацию в TabSubclassProc::WM_PAINT или рисовать border до DefSubclassProc.

> **Статус:** LOW-priority. Визуально заметно только при медленной отрисовке.

---

## 6. НЕИСПОЛЬЗУЕМЫЙ КОД И МЕЛКИЕ ПРОБЛЕМЫ

### 6.1. IDC_AUTOSAY (285) определён, но нигде не используется

**Файл:** `src/LordOfClicker/resource.h`, строка 47

**Проблема:** `#define IDC_AUTOSAY 285` определён, но не используется ни в одном .rc-шаблоне диалога и ни в одном .cpp/.h файле.

**Исправление:** Удалить или закомментировать, если контрол не планируется использовать:
```cpp
// #define IDC_AUTOSAY                     285  // UNUSED
```

### 6.2. Legacy SettingsDlg::Apply() — off-by-one ошибка в проверке индекса

**Файл:** `src/LordOfClicker/SettingsDlg.cpp`, строка ~192

**Проблема:** Проверка `if (nIdx > sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0]))` использует `>` вместо `>=`. Если `nIdx` равен размеру массива (7), проверка не срабатывает, и `s_arrHealTimes[7]` будет доступ за пределы массива.

**Примечание:** CSettingsDlg не создаёт окно (Create() закомментирован), поэтому Apply() никогда не вызывается в текущем коде. Но если код будет восстановлен, баг проявится.

**Исправление:**
```cpp
// БЫЛО:
if (nIdx > sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0]))
    nIdx = sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0]);

// СТАЛО:
if (nIdx >= (int)(sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0])))
    nIdx = (int)(sizeof(s_arrHealTimes)/sizeof(s_arrHealTimes[0])) - 1;
```

### 6.3. SettingsDlg::OnNCPaint использует старую синюю тему вместо dark-gold MU

**Файл:** `src/LordOfClicker/SettingsDlg.cpp`, строки 237-271

**Проблема:** OnNCPaint рисует заголовок с `RGB(10,35,150)` (синий фон), что не соответствует тёмно-золотой теме MU Online, используемой в UnifiedSettingsDlg и HistoryDialog.

**Статус:** CSettingsDlg не создаёт окно в текущем коде, поэтому визуально не проявляется. **LOW-priority**, исправлять только при восстановлении legacy диалога.

---

## 7. ПРИОРИТЕТЫ ИСПРАВЛЕНИЯ

| Приоритет | Пункт | Описание |
|-----------|-------|----------|
| **BLOCKER** | 0.1 | Ошибка компиляции: SubclassWindow макрос конфликт |
| **BLOCKER** | 0.2 | Ошибка компиляции: CRect не объявлен |
| **CRITICAL** | 1.1 | OnLaunchMu сломан — F10 не работает |
| **CRITICAL** | 1.2 | ESC не сбрасывает m_fGuiActive |
| **HIGH** | 2.1 | HUD-кнопки без MU-курсора |
| **HIGH** | 2.2 | HistoryDialog с неправильным курсором |
| **MEDIUM** | 3.1, 3.2, 3.3 | Owner-draw кнопки без hover-эффекта |
| **MEDIUM** | 4.1 | Мёртвый код m_cSettingsDlg в ESC |
| **LOW** | 4.2 | Потенциальная гонка в RestorePopupDialogs |
| **LOW** | 5.3 | Артефакты border при переключении табов |
| **LOW** | 6.1 | Неиспользуемый IDC_AUTOSAY |
| **LOW** | 6.2 | Off-by-one в legacy SettingsDlg |
| **LOW** | 6.3 | Синяя тема в legacy SettingsDlg |

---

## 8. ПОРЯДОК ПРИМЕНЕНИЯ ИСПРАВЛЕНИЙ

1. **ПЕРВЫМ ДЕЛОМ:** Исправить п. 0.1 (SubclassWindow макрос) — без этого проект НЕ компилируется
2. **ПЕРВЫМ ДЕЛОМ:** Исправить п. 0.2 (CRect → RECT) — без этого проект НЕ компилируется
3. **Затем:** Исправить п. 1.1 (OnLaunchMu) — восстанавливает сломанную функциональность
4. **Затем:** Исправить п. 1.2 (ESC + m_fGuiActive) — устраняет блокировку клавиш
5. **Затем:** Исправить п. 4.1 (удалить мёртвый код m_cSettingsDlg в ESC)
6. **Затем:** Исправить п. 2.1 (HUD cursor) — улучшает визуальную целостность
7. **Затем:** Исправить п. 2.2 (History cursor) — согласует стиль курсора
8. **Затем:** Исправить п. 3.1-3.3 (hover кнопок) — улучшает UX
9. **По желанию:** Исправить п. 4.2, 5.3, 6.1-6.3

---

## 9. ФАЙЛЫ, КОТОРЫЕ НУЖНО ИЗМЕНИТЬ

| Файл | Пункты |
|------|--------|
| `src/LordOfClicker/MuWindow.cpp` | 0.1, 1.1, 1.2, 4.1 |
| `src/LordOfClicker/HUDButtons.h` | 2.1 |
| `src/LordOfClicker/HUDButtons.cpp` | 0.2, 2.1 |
| `src/LordOfClicker/HistoryDialog.h` | (возможно для hover subclass) |
| `src/LordOfClicker/HistoryDialog.cpp` | 2.2, 3.1 |
| `src/LordOfClicker/UnifiedSettingsDlg.cpp` | 3.2, 3.3 |
| `src/LordOfClicker/resource.h` | 6.1 (опционально) |
| `src/LordOfClicker/SettingsDlg.cpp` | 6.2 (опционально) |
