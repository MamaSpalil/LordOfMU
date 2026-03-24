# Промт для исправления визуальных и критических багов автокликера

> **Дата анализа:** 2026-03-24 (обновлено)
> **Анализируемые файлы:** HUDButtons.cpp/h, MuWindow.cpp/h, UnifiedSettingsDlg.cpp/h, HistoryDialog.cpp/h, SettingsDlg.cpp/h, AdvSettingsDialog.cpp/h, MuTheme.cpp/h, ClickerJob.cpp/h, resource.h, AutoClicker.rc

---

## РЕЗУЛЬТАТ АНАЛИЗА РАНЕЕ ИСПРАВЛЕННЫХ ПУНКТОВ

Следующие пункты из предыдущей версии промта были **полностью реализованы** и работают корректно. Они удалены из задания:

| Бывший пункт | Описание | Статус |
|---|---|---|
| 0.1 | SubclassWindow макрос конфликт в MuWindow.cpp | ✅ Исправлено: `#undef SubclassWindow` добавлен после `#include <windowsx.h>` |
| 0.2 | CRect не объявлен в HUDButtons.cpp | ✅ Исправлено: заменён на `RECT rcPos = { 0, 0, barWidth, barHeight };` |
| 1.1 | OnLaunchMu сломан — F10 не работает | ✅ Исправлено: используется `m_cUnifiedSettingsDlg.IsWindow()` |
| 1.2 | ESC не сбрасывает m_fGuiActive | ✅ Исправлено: `m_fGuiActive = FALSE;` добавлен в ESC-обработчик (строка 382) |
| 1.3 | OnClickedOK/OnClickedCancel не уведомляют родителя | ✅ Работает через таймер 1011 + ESC-обработчик |
| 2.1 | HUD-кнопки без WM_SETCURSOR / MU-курсора | ✅ Исправлено: OnSetCursor + m_hMuCursor реализованы в CHUDButtons |
| 2.2 | HistoryDialog с неправильным курсором | ✅ Исправлено: OnSetCursor и OnShowWindow используют `m_cTheme.GetMuCursor()` |
| 2.3 | WM_SETCURSOR в UnifiedSettingsDlg non-client | ✅ Исправлено: корректно обрабатывает HTCLIENT, non-client управляется Windows |
| 3.1 | HistoryDialog Close кнопка без hover | ✅ Исправлено: bHover вычисляется через `GetCursorPos + PtInRect` |
| 3.2 | UnifiedSettingsDlg кнопки используют ODS_FOCUS для hover | ✅ Исправлено: bHover вычисляется через `GetCursorPos + PtInRect` |
| 3.3 | Кнопки не перерисовываются при hover | ✅ Исправлено: ButtonHoverSubclassProc с TME_LEAVE + InvalidateRect в обоих диалогах |
| 4.1 | Мёртвый код m_cSettingsDlg в ESC | ✅ Исправлено: удалён |
| 4.2 | Гонка в RestorePopupDialogs | ✅ Исправлено: флаги сбрасываются ДО показа окон |
| 5.2 | Checkbox subclass не инвалидирует | ✅ Работает корректно через стандартный механизм |
| 6.1 | IDC_AUTOSAY не используется | ✅ Исправлено: закомментирован с пометкой UNUSED |
| 6.2 | Off-by-one в SettingsDlg::Apply() | ✅ Исправлено: используется `>=` и `- 1` |

---

## ПРОМТ ДЛЯ ИСПРАВЛЕНИЯ:

Исправь все найденные ниже баги и ошибки в коде автокликера LordOfMU. Каждый пункт содержит описание бага, его локацию в коде и инструкции по исправлению.

---

## 0. ОШИБКИ КОМПИЛЯЦИИ (проект не собирается)

> Эти ошибки БЛОКИРУЮТ сборку проекта и должны быть исправлены В ПЕРВУЮ ОЧЕРЕДЬ перед всеми остальными пунктами.

### 0.1. UnifiedSettingsDlg.cpp — ButtonHoverSubclassProc: необъявленный идентификатор (C2065)

**Файл:** `src/LordOfClicker/UnifiedSettingsDlg.cpp`, строки 234 и 236

**Ошибки компиляции:**
```
4>UnifiedSettingsDlg.cpp(234,29): error C2065: ButtonHoverSubclassProc: необъявленный идентификатор
4>UnifiedSettingsDlg.cpp(236,33): error C2065: ButtonHoverSubclassProc: необъявленный идентификатор
```

**Причина:** В `OnInitDialog` (строки 230-236) вызывается `SetWindowSubclass` с указателем на функцию `ButtonHoverSubclassProc`:
```cpp
// Строки 230-236:
HWND hBtnOK = GetDlgItem(IDOK);
HWND hBtnCancel = GetDlgItem(IDCANCEL);
if (hBtnOK)
    SetWindowSubclass(hBtnOK, ButtonHoverSubclassProc, 100, 0);   // строка 234 — ОШИБКА
if (hBtnCancel)
    SetWindowSubclass(hBtnCancel, ButtonHoverSubclassProc, 101, 0); // строка 236 — ОШИБКА
```

Однако `ButtonHoverSubclassProc` определена как **file-scope static** функция только на **строке 1330** — далеко ниже по файлу:
```cpp
// Строка 1330:
static LRESULT CALLBACK ButtonHoverSubclassProc(
    HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
    UINT_PTR uIdSubclass, DWORD_PTR /*dwRefData*/)
{ ... }
```

C++ компилятор обрабатывает файл сверху вниз. На строке 234 функция ещё не объявлена, поэтому компилятор выдаёт C2065 "необъявленный идентификатор".

**Сравнение с HistoryDialog.cpp:** В `HistoryDialog.cpp` аналогичная функция `ButtonHoverSubclassProc` определена на **строках 10-34** (в самом начале файла), ПЕРЕД её использованием в `OnInitDialog` (строка 118). Поэтому в HistoryDialog ошибки компиляции нет.

**Сравнение с другими subclass proc в UnifiedSettingsDlg:** Функции `TabSubclassProc`, `CheckboxSubclassProc` и `SeparatorSubclassProc` являются **static-членами класса** `CUnifiedSettingsDlg` — они объявлены в заголовочном файле `UnifiedSettingsDlg.h` (строки 99-106) и определены с квалификатором `CUnifiedSettingsDlg::`. Поэтому они видны в любом месте внутри методов класса. `ButtonHoverSubclassProc` же — свободная `static` функция, НЕ объявленная в заголовке, поэтому и не видна до определения.

**Исправление (рекомендуемое):** Добавить forward declaration для `ButtonHoverSubclassProc` в начало файла `UnifiedSettingsDlg.cpp`, после включений и перед первой функцией:

```cpp
// ДОБАВИТЬ после строки 12 (после `static const int CHECKBOX_BOX_SIZE = 13;`):
static LRESULT CALLBACK ButtonHoverSubclassProc(
    HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
    UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
```

**Альтернативное исправление:** Переместить полное определение `ButtonHoverSubclassProc` (строки 1326-1354) выше по файлу — перед `OnInitDialog`, например после строки 12. Это устранит необходимость forward declaration.

---

## 1. ВИЗУАЛЬНЫЕ БАГИ (LOW-MEDIUM ПРИОРИТЕТ)

### 1.1. Tab control border — отсутствие двойной буферизации в TabSubclassProc

**Файл:** `src/LordOfClicker/UnifiedSettingsDlg.cpp`, строки 1099-1173 (TabSubclassProc)

**Проблема:** В `WM_PAINT` обработчике `TabSubclassProc` сначала вызывается `DefSubclassProc` (рисует стандартные табы с белым/серым бордером), а затем поверх рисуется dark-gold бордер через `Rectangle()` с прямым рисованием через `GetDC()`. Между этими двумя операциями может быть визуально заметно мерцание — кратковременное появление стандартного бордера перед перерисовкой тёмно-золотым.

**Сравнение:** `CheckboxSubclassProc` в том же файле (строки 1237-1248) использует **двойную буферизацию** (`CreateCompatibleDC` + `CreateCompatibleBitmap` + `BitBlt`), что устраняет мерцание. `OnPaint` в HUDButtons.cpp также использует двойную буферизацию. `TabSubclassProc` — единственный subclass без неё.

**Исправление:** Обернуть отрисовку бордера в TabSubclassProc в двойную буферизацию:

```cpp
case WM_PAINT:
{
    // Сначала позволить стандартному Tab control отрисовать себя
    LRESULT lr = DefSubclassProc(hWnd, uMsg, wParam, lParam);

    // Затем поверх нарисовать dark-gold бордер с двойной буферизацией
    HDC hDC = GetDC(hWnd);
    RECT rc;
    GetClientRect(hWnd, &rc);

    HDC hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hBmp = CreateCompatibleBitmap(hDC, rc.right, rc.bottom);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

    // Скопировать текущее содержимое окна в буфер
    BitBlt(hMemDC, 0, 0, rc.right, rc.bottom, hDC, 0, 0, SRCCOPY);

    // Нарисовать бордер в буфере
    HPEN hPen = CreatePen(PS_SOLID, 1, CMuTheme::ClrBorder());
    HPEN hOldPen = (HPEN)SelectObject(hMemDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hMemDC, GetStockObject(NULL_BRUSH));
    Rectangle(hMemDC, rc.left, rc.top, rc.right, rc.bottom);
    SelectObject(hMemDC, hOldBrush);
    SelectObject(hMemDC, hOldPen);
    DeleteObject(hPen);

    // BitBlt буфер обратно
    BitBlt(hDC, 0, 0, rc.right, rc.bottom, hMemDC, 0, 0, SRCCOPY);

    SelectObject(hMemDC, hOldBmp);
    DeleteObject(hBmp);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd, hDC);

    return lr;
}
```

> **Приоритет:** LOW. Визуально заметно только при медленной отрисовке или слабом оборудовании.

### 1.2. Legacy SettingsDlg/AdvSettingsDialog используют старую синюю тему

**Файлы:**
- `src/LordOfClicker/SettingsDlg.cpp`, строки 237-271 (OnNCPaint)
- `src/LordOfClicker/AdvSettingsDialog.cpp`, строки 169-203 (OnNCPaint)

**Проблема:** Оба legacy-диалога используют жёстко закодированный синий фон `RGB(10,35,150)` в OnNCPaint вместо тёмно-золотой темы MU Online (`CMuTheme::DrawMuFrame()` + `CMuTheme::ClrTitleText()`), применяемой в `UnifiedSettingsDlg` и `HistoryDialog`.

**Текущий статус:** Оба диалога **не создаются** в текущем коде (Create() закомментирован в MuWindow.cpp), поэтому визуально не проявляются.

**Исправление (при восстановлении legacy диалогов):** Заменить жёсткий синий фон на вызов `CMuTheme::DrawMuFrame()` и использовать цвета из MuTheme.

> **Приоритет:** LOW. Не проявляется в текущей сборке.

### 1.3. AdvSettingsDialog — отсутствует WM_SETCURSOR, используется стандартная стрелка

**Файл:** `src/LordOfClicker/AdvSettingsDialog.h`, строки 20-37 (BEGIN_MSG_MAP)
**Файл:** `src/LordOfClicker/AdvSettingsDialog.cpp`, строка 135

**Проблема:**
1. В AdvSettingsDialog::BEGIN_MSG_MAP **отсутствует** `MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)`. При наведении мыши на диалог курсор возвращается к стандартной стрелке Windows.
2. В `OnShowWindow` (строка 135) используется `LoadCursor(0, IDC_ARROW)` — стандартная стрелка, а не MU-курсор из `m_cTheme.GetMuCursor()`.

**Текущий статус:** AdvSettingsDialog **не создаётся** в текущем коде (`m_cAdvSettingsDlg.Create(m_hWnd)` закомментирован в MuWindow.cpp строка 185).

**Исправление (при восстановлении AdvSettingsDialog):**
- Добавить `MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)` в message map
- Реализовать OnSetCursor аналогично HistoryDialog с использованием `m_cTheme.GetMuCursor()`
- Заменить `LoadCursor(0, IDC_ARROW)` на `m_cTheme.GetMuCursor()` в OnShowWindow

> **Приоритет:** LOW. Не проявляется в текущей сборке.

---

## 2. ПРИОРИТЕТЫ ИСПРАВЛЕНИЯ

| Приоритет | Пункт | Описание |
|-----------|-------|----------|
| **BLOCKER** | 0.1 | Ошибка компиляции: ButtonHoverSubclassProc необъявленный идентификатор (C2065) |
| **LOW** | 1.1 | Tab control border без двойной буферизации — возможно мерцание |
| **LOW** | 1.2 | Legacy SettingsDlg/AdvSettingsDialog — старая синяя тема (не активны) |
| **LOW** | 1.3 | AdvSettingsDialog — нет WM_SETCURSOR / MU-курсора (не активен) |

---

## 3. ПОРЯДОК ПРИМЕНЕНИЯ ИСПРАВЛЕНИЙ

1. **ПЕРВЫМ ДЕЛОМ:** Исправить п. 0.1 (forward declaration для ButtonHoverSubclassProc) — без этого проект НЕ компилируется
2. **По желанию:** Исправить п. 1.1 (двойная буферизация TabSubclassProc) — визуальное улучшение
3. **При необходимости:** Исправить п. 1.2, 1.3 — только при восстановлении legacy диалогов

---

## 4. ФАЙЛЫ, КОТОРЫЕ НУЖНО ИЗМЕНИТЬ

| Файл | Пункты |
|------|--------|
| `src/LordOfClicker/UnifiedSettingsDlg.cpp` | 0.1 (forward declaration ButtonHoverSubclassProc), 1.1 (двойная буферизация TabSubclassProc) |
| `src/LordOfClicker/SettingsDlg.cpp` | 1.2 (опционально — legacy тема) |
| `src/LordOfClicker/AdvSettingsDialog.cpp` | 1.3 (опционально — legacy курсор и тема) |
| `src/LordOfClicker/AdvSettingsDialog.h` | 1.3 (опционально — WM_SETCURSOR в message map) |

---

## 5. ВЕРИФИЦИРОВАННЫЕ КОМПОНЕНТЫ (работают корректно)

Следующие компоненты были проверены в ходе анализа и **не содержат ошибок**:

| Компонент | Статус | Примечание |
|---|---|---|
| **CHUDButtons** — OnPaint с двойной буферизацией | ✅ OK | Используется CreateCompatibleDC + BitBlt |
| **CHUDButtons** — OnNCHitTest click-through | ✅ OK | HTTRANSPARENT для пустого пространства, HTCLIENT для кнопок |
| **CHUDButtons** — OnSetCursor MU-курсор | ✅ OK | m_hMuCursor загружается из ресурса IDC_MU_CURSOR |
| **CHUDButtons** — OnMouseMove/OnMouseLeave hover | ✅ OK | TME_LEAVE + InvalidateRect |
| **CHUDButtons** — OnLButtonDown/OnLButtonUp | ✅ OK | SetCapture/ReleaseCapture, PostMessage к родителю |
| **CHUDButtons** — Reposition/SetGameActive | ✅ OK | ClientToScreen, HWND_TOPMOST, проверка IsIconic |
| **CHUDButtons** — WS_POPUP + WS_EX_LAYERED + WS_EX_NOACTIVATE | ✅ OK | Корректные стили для overlay |
| **CMuWindow** — OnShowSettingsGUI | ✅ OK | Закрывает History, проверяет m_fGuiActive |
| **CMuWindow** — OnHUDSettings/OnHUDStartStop/OnHUDHistory | ✅ OK | PostMessage, взаимное исключение диалогов |
| **CMuWindow** — ESC-обработчик | ✅ OK | Скрывает диалоги, сбрасывает m_fGuiActive = FALSE |
| **CMuWindow** — OnActivateApp + HidePopupDialogs/RestorePopupDialogs | ✅ OK | Флаги сбрасываются ДО показа окон |
| **CMuWindow** — Timer 1011 m_fGuiActive мониторинг | ✅ OK | Проверяет видимость + m_bWasVisible флаги |
| **CUnifiedSettingsDlg** — OnSetCursor MU-курсор | ✅ OK | HTCLIENT → MU-курсор, non-client → Windows default |
| **CUnifiedSettingsDlg** — OnDrawItem hover кнопок | ✅ OK | GetCursorPos + PtInRect |
| **CUnifiedSettingsDlg** — TabPageDlgProc WM_SETCURSOR forwarding | ✅ OK | Перенаправляет к родителю |
| **CUnifiedSettingsDlg** — OnCtlColor* handlers | ✅ OK | Корректные цвета темы |
| **CUnifiedSettingsDlg** — CheckboxSubclassProc | ✅ OK | Двойная буферизация, per-window |
| **CUnifiedSettingsDlg** — SeparatorSubclassProc | ✅ OK | Корректная отрисовка |
| **CUnifiedSettingsDlg** — ButtonHoverSubclassProc impl | ✅ OK | Per-window tracking (без static s_bHover), TME_LEAVE |
| **CHistoryDialog** — OnSetCursor MU-курсор | ✅ OK | m_cTheme.GetMuCursor() с fallback к IDC_ARROW |
| **CHistoryDialog** — OnShowWindow курсор save/restore | ✅ OK | Snapshot + restore ShowCursor count |
| **CHistoryDialog** — OnDrawItem hover | ✅ OK | GetCursorPos + PtInRect |
| **CHistoryDialog** — ButtonHoverSubclassProc | ✅ OK | Определена ПЕРЕД использованием (строки 10-34) |
| **CHistoryDialog** — OnNCPaint MU-тема | ✅ OK | DrawMuFrame + ClrTitleText |
| **CMuTheme** — GetMuCursor() | ✅ OK | LoadImage + DestroyCursor lifecycle |
| **Class settings** (DL/EE/AE/BK/SM/MG) — WM_SETCURSOR | ✅ OK | OnForwardToParent → SendMessage к родителю |
| **CClickerJob** — безопасное завершение | ✅ OK | 10s timeout без TerminateThread |
| **resource.h** — нет конфликтов ID | ✅ OK | IDC_AUTOSAY закомментирован, IDC_STOPMOVE=249 уникален |
