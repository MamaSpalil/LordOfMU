# Отчёт: Анализ вызова диалоговых окон AutoClicker AVANTA+ELITE

**Дата:** 2026-03-25  
**Компонент:** LordOfClicker — ImGui overlay (Menu / History Action)  
**Горячие клавиши:** F9 (Menu/Settings), Shift+F9 (History Action)

---

## Архитектура

Диалоговые окна рендерятся через Dear ImGui v1.91.8 непосредственно в D3D9 EndScene хуке.
Цепочка вызова:

1. **Обнаружение клавиши** (LL-хук или таймер 100мс) → `OnKeyboardEvent()`
2. **PostMessage** → `WM_SHOW_SETTINGS_GUI` / `WM_SHOW_HISTORY`
3. **ATL message handler** → `OnShowSettingsGUI()` / `OnShowHistory()`
4. **Toggle ImGui флагов** → `m_bShowSettings` / `m_bShowHistory`
5. **Рендеринг** → `CImGuiOverlay::Render()` в `OnEndSceneCallback()`

---

## Найденные ошибки

### BUG-1 (Критическая): LL keyboard hook — мёртвый путь для F9

**Файл:** `src/LordOfClicker/MuWindow.cpp`

`KeyboardProcLL` (строка 273) вызывает `OnKeyboardEvent()` **только когда игра НЕ foreground**:

```cpp
if (CMuWindow::GetForegroundWindowTr() != pThis->m_hWnd)
{
    if (pThis->OnKeyboardEvent(kbd->vkCode, (UINT)wParam))  // fCheckFgWnd = TRUE (default)
        return 0;
}
```

Но внутри `OnKeyboardEvent` (строка 310) стоит **противоположная проверка**:

```cpp
if (fCheckFgWnd && CMuWindow::GetForegroundWindowTr() != m_hWnd)
    return FALSE;   // отклоняет, т.к. игра НЕ foreground
```

**Результат:** LL-хук НИКОГДА не обрабатывает F9, т.к. оба условия взаимоисключающие.  
F12 работает только потому, что обрабатывается ДО этой проверки.

**Исправление:** Передавать `fCheckFgWnd = FALSE` из `KeyboardProcLL`, т.к. хук уже проверил foreground.

---

### BUG-2 (Критическая): Бесконечный re-post loop в OnShowSettingsGUI

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки 562-578

```cpp
if (m_pClicker != NULL)
{
    PostMessage(WM_STOP_CLICKER, 0, 0);
    PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);   // re-post самого себя
    ...
    return 0;
}
```

При каждом повторном вызове `m_pClicker` всё ещё не NULL (остановка асинхронная — 
`OnClickerJobFinished` ещё не вызван). Накапливаются сообщения `WM_SHOW_SETTINGS_GUI`,
каждое вызывает `ToggleSettings()`. Если их чётное число — диалог мерцает и закрывается.

**Исправление:** Не re-постить `WM_SHOW_SETTINGS_GUI`, а лишь остановить кликер и оповестить звуком.

---

### BUG-3 (Средняя): Клавиатурные WM не пересылаются в ImGui

**Файл:** `src/LordOfClicker/MuWindow.h`, строка 50  
**Файл:** `src/LordOfClicker/ImGuiOverlay.cpp`, строки 174-178

ATL message map обрабатывает только `WM_MOUSEFIRST..WM_MOUSELAST` через `OnMouseMessage`.
Клавиатурные сообщения (`WM_KEYDOWN`, `WM_KEYUP`, `WM_CHAR`) **никогда не пересылаются**
в `ImGui_ImplWin32_WndProcHandler`. Код в `WndProcHandler` для `WM_KEYDOWN`/`WM_KEYUP` — мёртвый.

**Результат:** Навигация клавиатурой (Tab/Enter/стрелки) и текстовый ввод в ImGui окнах не работают.

**Исправление:** Добавить `MESSAGE_RANGE_HANDLER(WM_KEYFIRST, WM_KEYLAST, OnKeyboardMessage)` 
и реализовать `OnKeyboardMessage`, который пересылает сообщения в `m_cOverlay.WndProcHandler()`.

---

### BUG-4 (Средняя): Гонка состояний при Shift+F9

**Файл:** `src/LordOfClicker/MuWindow.cpp`, строки 371-382

```cpp
if (vkCode == VK_F9 && uMsg == WM_KEYUP)
{
    if ((CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
        PostMessage(WM_SHOW_HISTORY, 0, 0);
    else
        PostMessage(WM_SHOW_SETTINGS_GUI, 0, 0);
}
```

Таймер опрашивает клавиши каждые 100мс. К моменту WM_KEYUP для F9 пользователь
мог уже отпустить Shift. `GetAsyncKeyState(VK_SHIFT)` может вернуть 0.

**Исправление:** Запоминать состояние Shift при WM_KEYDOWN F9, использовать при WM_KEYUP.

---

## Статус исправлений

| # | Серьёзность | Описание | Статус |
|---|------------|----------|--------|
| 1 | 🔴 Критическая | LL keyboard hook мёртвый путь для F9 | ✅ Исправлено |
| 2 | 🔴 Критическая | Бесконечный re-post loop в OnShowSettingsGUI | ✅ Исправлено |
| 3 | 🟡 Средняя | Клавиатурные WM не пересылаются в ImGui | ✅ Исправлено |
| 4 | 🟡 Средняя | Гонка состояний при Shift+F9 | ✅ Исправлено |
