# PROMPT_REDESIGN.md — Полный промт для редизайна интерфейса автокликера LordOfMU

## Цель

Полная переработка визуального дизайна и структуры UI-окна настроек автокликера внутри игрового клиента MU Online. Объединение двух отдельных окон настроек (F9 и SHIFT+F9) в единое окно с табами, оформленное в стиле MU Online Season 3 Episode 1.

---

## Общее описание задачи

### Текущее состояние

Сейчас в программе существуют **два отдельных диалоговых окна**:

1. **F9 — Основные настройки** (`CSettingsDlg`, `IDD_SETTINGSDLG`)
   - Размер: 220×282 DLU
   - General Settings: auto-pickup, auto-repair, auto-heal + heal time
   - Stealth Options: auto-speak, stop move, stop zen, stop pick
   - Auto-type `/re off`, Exit at level 400
   - Class Settings: выбор класса + вложенные панели (DarkLord, EElf, AElf, BK, SM, MG)
   - Кнопки OK / Cancel

2. **SHIFT+F9 — Расширенные настройки** (`CAdvSettingsDialog`, `IDD_ADV_SETTINGS`)
   - Размер: 167×180 DLU
   - Enable advanced pick-up (master toggle)
   - 8 типов предметов (Bless, Soul, Life, Guardian, Creation, Chaos, Excellent, Zen) с чекбоксами Pick + Move To
   - Custom Items с кодом предмета
   - Кнопки OK / Cancel

### Целевое состояние

**Единое окно** на кнопке **F9**, содержащее ВСЕ настройки из обоих диалогов, организованное в виде **вкладок (tabs)** с дизайном в стиле MU Online Season 3 Episode 1.

---

## Требования к архитектуре

### 1. Объединение окон F9 и SHIFT+F9

#### 1.1. Новая структура единого окна

Создать **одно** диалоговое окно `CUnifiedSettingsDlg` (новый класс), которое заменяет оба существующих диалога. Окно содержит **Tab Control** (WC_TABCONTROL) с тремя вкладками:

| Вкладка | Название | Содержимое |
|---------|----------|------------|
| Tab 1 | ⚔ General | Все настройки из текущего IDD_SETTINGSDLG (General Settings, Stealth Options, авто-функции) |
| Tab 2 | 🎭 Class | Настройки класса персонажа — комбобокс выбора класса + 6 вложенных панелей (DL, EElf, AElf, BK, SM, MG) |
| Tab 3 | 💎 Pickup | Все настройки из текущего IDD_ADV_SETTINGS (advanced pickup, jewels, excellent items, zen, custom items) |

#### 1.2. Размер окна

- **Минимальный размер:** 300×420 DLU (ширина × высота)
- **Рекомендуемый размер:** 320×450 DLU
- Окно центрируется относительно игрового клиента (как в текущей реализации)

#### 1.3. Общие элементы (вне вкладок)

В нижней части окна, **под Tab Control**, разместить общие элементы:
- Кнопка **"Apply"** (IDOK) — сохраняет и применяет ВСЕ настройки из всех вкладок
- Кнопка **"Cancel"** (IDCANCEL) — закрывает без сохранения
- Строка версии: "LordOfMU AutoClicker v2.x" (мелкий шрифт, выровнено по центру)

---

### 2. Деактивация SHIFT+F9

#### 2.1. Файл: `src/LordOfClicker/MuWindow.cpp`

В функции `CMuWindow::OnShowSettingsGUI()` (строки 464–530) **закомментировать** проверку Shift:

```cpp
LRESULT CMuWindow::OnShowSettingsGUI(UINT, WPARAM, LPARAM, BOOL&)
{
    if (m_pClicker != NULL)
        return 0;

    // REDESIGN: F9 and SHIFT+F9 merged into unified dialog
    // Используем CWindow wrapper от нового единого диалога (как в оригинальном паттерне)
    CWindow dlg = m_cUnifiedSettingsDlg; // CWindow копирует HWND handle, не объект

    // DEPRECATED: SHIFT+F9 больше не используется
    // if ((CMuWindow::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
    // {
    //     dlg = m_cAdvSettingsDlg;
    // }

    if (m_fGuiActive || !dlg.IsWindow())
        return 0;

    // ... остальная логика остаётся без изменений ...
}
```

> **Примечание:** `CWindow dlg = m_cUnifiedSettingsDlg;` — это стандартный паттерн ATL. `CWindow` является лёгкой обёрткой вокруг `HWND` и при присваивании копирует только handle окна, а не весь объект диалога. Это аналогично существующему коду `CWindow dlg = m_cSettingsDlg;` (строка 469 в текущем MuWindow.cpp).

#### 2.2. Файл: `src/LordOfClicker/MuWindow.h`

- Добавить `#include "UnifiedSettingsDlg.h"` (новый файл)
- Добавить член класса: `CUnifiedSettingsDlg m_cUnifiedSettingsDlg;`
- **Закомментировать** (НЕ удалять): `// CAdvSettingsDialog m_cAdvSettingsDlg;`
- Оставить `CSettingsDlg m_cSettingsDlg;` как legacy (для обратной совместимости загрузки/сохранения настроек)

---

### 3. Новый класс CUnifiedSettingsDlg

#### 3.1. Файл: `src/LordOfClicker/UnifiedSettingsDlg.h`

```
Новый класс CUnifiedSettingsDlg : public CAxDialogImpl<CUnifiedSettingsDlg>
```

**Требования:**
- Наследует от `CAxDialogImpl` (как существующие диалоги)
- Содержит Tab Control (SysTabControl32)
- Содержит ВСЕ child-диалоги:
  - Панель General (новый IDD_TAB_GENERAL)
  - Панель Class (новый IDD_TAB_CLASS) — содержит 6 вложенных панелей классов
  - Панель Pickup (новый IDD_TAB_PICKUP)
- Управляет видимостью панелей при переключении вкладок
- Использует единый объект `CClickerSettings` для загрузки/сохранения всех настроек
- При Apply() вызывает Apply() всех вложенных панелей и сохраняет настройки

#### 3.2. Файл: `src/LordOfClicker/UnifiedSettingsDlg.cpp`

Реализация логики:
- `OnInitDialog()` — создание Tab Control, добавление вкладок, создание child-диалогов, позиционирование
- `OnTabSelChange()` — переключение видимости панелей при смене вкладки
- `OnShowWindow()` — инициализация значений из настроек, управление курсором
- `OnClickedOK()` — Apply всех панелей + Save()
- `OnClickedCancel()` — закрытие без сохранения
- `OnNCPaint()` — кастомная отрисовка рамки окна в стиле MU Online

---

## Требования к визуальному дизайну (стиль MU Online Season 3 Episode 1)

### 4. Цветовая схема

MU Online Season 3 Episode 1 использует характерную тёмную палитру с золотыми акцентами:

| Элемент | Цвет RGB | Hex | Описание |
|---------|----------|-----|----------|
| Фон окна | (15, 15, 25) | `#0F0F19` | Тёмно-синий/чёрный фон |
| Фон панели | (20, 22, 35) | `#141623` | Чуть светлее основного фона |
| Рамка окна | (85, 75, 45) | `#554B2D` | Золотисто-коричневая рамка |
| Рамка (яркая) | (180, 155, 80) | `#B49B50` | Золотой акцент для активных элементов |
| Заголовок окна | (200, 170, 80) | `#C8AA50` | Золотой текст заголовка |
| Текст основной | (210, 210, 210) | `#D2D2D2` | Светло-серый основной текст |
| Текст активный | (255, 230, 150) | `#FFE696` | Золотистый текст для выделенных элементов |
| Текст неактивный | (120, 120, 140) | `#78788C` | Серый текст для disabled элементов |
| Кнопка фон | (40, 38, 55) | `#282637` | Тёмный фон кнопок |
| Кнопка hover | (55, 50, 70) | `#373246` | Фон кнопки при наведении |
| Кнопка активная | (70, 60, 45) | `#463C2D` | Фон нажатой кнопки |
| Чекбокс отметка | (120, 200, 80) | `#78C850` | Зелёная галочка (как HP bar) |
| Tab активный | (35, 30, 50) | `#231E32` | Фон активного таба |
| Tab неактивный | (20, 18, 30) | `#14121E` | Фон неактивного таба |
| Разделитель | (60, 55, 45) | `#3C372D` | Линия-разделитель секций |
| Ошибка/Предупреждение | (200, 50, 50) | `#C83232` | Красный для предупреждений |

### 5. Шрифты

| Назначение | Шрифт | Размер | Стиль |
|------------|-------|--------|-------|
| Заголовок окна | "Tahoma" | 11pt | Bold |
| Названия вкладок | "Tahoma" | 9pt | Bold |
| Названия секций | "Tahoma" | 8pt | Bold |
| Основной текст | "Tahoma" | 8pt | Regular |
| Подписи контролов | "Tahoma" | 8pt | Regular |
| Версия / мелкий текст | "Tahoma" | 7pt | Regular |

> **Примечание:** Tahoma — штатный шрифт MU Online UI и гарантированно присутствует на всех версиях Windows. Arial используется как системный fallback только в случае недоступности Tahoma (маловероятно). `CreateFont` автоматически выберет ближайший доступный шрифт.

### 6. Текстуры и изображения для интеграции

Для создания аутентичного дизайна MU Online Season 3 Episode 1 необходимо **скачать из интернета и интегрировать** следующие графические ресурсы:

#### 6.1. Фоновая текстура окна

- **Источник:** Найти текстуру фона окна инвентаря / окна персонажа из MU Online Season 3
- **Описание:** Тёмная кожаная/металлическая текстура с лёгким градиентом, характерная для UI MU Online
- **Формат:** BMP (для LoadBitmap/LoadImage в Win32 API)
- **Размер:** 320×450 пикселей (или тайловая текстура 64×64 для заполнения)
- **Файл:** `src/LordOfClicker/res/bg_panel.bmp`
- **Интеграция:** Загрузка через `LoadImage()` в `OnInitDialog()`, отрисовка через `BitBlt()` в `WM_ERASEBKGND` / `WM_PAINT`

#### 6.2. Рамка окна (border frame)

- **Источник:** Рамка окна из UI MU Online — золотистая декоративная рамка
- **Описание:** 4 угловых элемента + 4 стороны (9-patch / nine-slice pattern)
- **Файлы:**
  - `src/LordOfClicker/res/frame_tl.bmp` — верхний левый угол
  - `src/LordOfClicker/res/frame_tr.bmp` — верхний правый угол
  - `src/LordOfClicker/res/frame_bl.bmp` — нижний левый угол
  - `src/LordOfClicker/res/frame_br.bmp` — нижний правый угол
  - `src/LordOfClicker/res/frame_top.bmp` — верхняя сторона (тайловая)
  - `src/LordOfClicker/res/frame_bottom.bmp` — нижняя сторона (тайловая)
  - `src/LordOfClicker/res/frame_left.bmp` — левая сторона (тайловая)
  - `src/LordOfClicker/res/frame_right.bmp` — правая сторона (тайловая)
- **Интеграция:** Отрисовка в `OnNCPaint()` через `StretchBlt()` / `BitBlt()`

#### 6.3. Кнопки (OK, Cancel, Apply)

- **Источник:** Кнопки из UI MU Online Season 3 (стандартные игровые кнопки)
- **Описание:** 3 состояния: normal, hover, pressed
- **Файлы:**
  - `src/LordOfClicker/res/btn_normal.bmp`
  - `src/LordOfClicker/res/btn_hover.bmp`
  - `src/LordOfClicker/res/btn_pressed.bmp`
- **Размер:** 80×25 пикселей каждый
- **Интеграция:** Owner-draw кнопки (`BS_OWNERDRAW`), обработка `WM_DRAWITEM`

#### 6.4. Вкладки (Tab headers)

- **Источник:** Табы/вкладки из интерфейса MU Online
- **Описание:** 2 состояния: active, inactive
- **Файлы:**
  - `src/LordOfClicker/res/tab_active.bmp`
  - `src/LordOfClicker/res/tab_inactive.bmp`
- **Размер:** 100×24 пикселя каждый
- **Интеграция:** Owner-draw Tab Control (`TCS_OWNERDRAWFIXED`), обработка `WM_DRAWITEM`

#### 6.5. Чекбоксы

- **Источник:** Стилизованные чекбоксы в стиле MU Online
- **Описание:** 4 состояния: unchecked, checked, unchecked-disabled, checked-disabled
- **Файлы:**
  - `src/LordOfClicker/res/chk_unchecked.bmp`
  - `src/LordOfClicker/res/chk_checked.bmp`
  - `src/LordOfClicker/res/chk_unchecked_dis.bmp`
  - `src/LordOfClicker/res/chk_checked_dis.bmp`
- **Размер:** 13×13 пикселей каждый
- **Интеграция:** Owner-draw чекбоксы или подклассирование с `SetWindowSubclass()`

#### 6.6. Заголовок окна (title bar)

- **Источник:** Полоска заголовка из UI MU Online с декоративным орнаментом
- **Файл:** `src/LordOfClicker/res/titlebar.bmp`
- **Размер:** 320×30 пикселей
- **Интеграция:** Отрисовка в `OnNCPaint()` вместо стандартного заголовка

#### 6.7. Иконки секций (опционально)

- **Описание:** Маленькие иконки для визуального разделения секций настроек
- **Файлы:**
  - `src/LordOfClicker/res/ico_sword.bmp` — для секции General (меч)
  - `src/LordOfClicker/res/ico_helm.bmp` — для секции Class (шлем)
  - `src/LordOfClicker/res/ico_jewel.bmp` — для секции Pickup (драгоценный камень)
- **Размер:** 16×16 пикселей каждый
- **Интеграция:** `ImageList_Create()` + `TabCtrl_SetImageList()`

### 7. Интеграция ресурсов в проект

#### 7.1. Структура директории ресурсов

Создать новую директорию для графических ресурсов:

```
src/LordOfClicker/
├── res/                    ← НОВАЯ ДИРЕКТОРИЯ
│   ├── bg_panel.bmp
│   ├── frame_tl.bmp
│   ├── frame_tr.bmp
│   ├── frame_bl.bmp
│   ├── frame_br.bmp
│   ├── frame_top.bmp
│   ├── frame_bottom.bmp
│   ├── frame_left.bmp
│   ├── frame_right.bmp
│   ├── btn_normal.bmp
│   ├── btn_hover.bmp
│   ├── btn_pressed.bmp
│   ├── tab_active.bmp
│   ├── tab_inactive.bmp
│   ├── chk_unchecked.bmp
│   ├── chk_checked.bmp
│   ├── chk_unchecked_dis.bmp
│   ├── chk_checked_dis.bmp
│   ├── titlebar.bmp
│   ├── ico_sword.bmp
│   ├── ico_helm.bmp
│   └── ico_jewel.bmp
├── AutoClicker.rc          ← обновить (добавить BITMAP ресурсы)
├── resource.h              ← обновить (добавить IDB_* идентификаторы)
└── ...
```

#### 7.2. Обновление resource.h

Добавить идентификаторы растровых ресурсов:

```cpp
// Bitmap resources for MU Online S3E1 themed UI
#define IDB_BG_PANEL                    500
#define IDB_FRAME_TL                    501
#define IDB_FRAME_TR                    502
#define IDB_FRAME_BL                    503
#define IDB_FRAME_BR                    504
#define IDB_FRAME_TOP                   505
#define IDB_FRAME_BOTTOM                506
#define IDB_FRAME_LEFT                  507
#define IDB_FRAME_RIGHT                 508
#define IDB_BTN_NORMAL                  509
#define IDB_BTN_HOVER                   510
#define IDB_BTN_PRESSED                 511
#define IDB_TAB_ACTIVE                  512
#define IDB_TAB_INACTIVE                513
#define IDB_CHK_UNCHECKED               514
#define IDB_CHK_CHECKED                 515
#define IDB_CHK_UNCHECKED_DIS           516
#define IDB_CHK_CHECKED_DIS             517
#define IDB_TITLEBAR                    518
#define IDB_ICO_SWORD                   519
#define IDB_ICO_HELM                    520
#define IDB_ICO_JEWEL                   521

// New dialog IDs for unified settings
#define IDD_UNIFIED_SETTINGS            400
#define IDD_TAB_GENERAL                 401
#define IDD_TAB_CLASS                   402
#define IDD_TAB_PICKUP                  403
#define IDC_SETTINGS_TAB                404
#define IDC_VERSION_LABEL               405
```

#### 7.3. Обновление AutoClicker.rc

Добавить секцию BITMAP ресурсов:

```rc
/////////////////////////////////////////////////////////////////////////////
//
// Bitmap
//

IDB_BG_PANEL            BITMAP  "res\\bg_panel.bmp"
IDB_FRAME_TL            BITMAP  "res\\frame_tl.bmp"
IDB_FRAME_TR            BITMAP  "res\\frame_tr.bmp"
IDB_FRAME_BL            BITMAP  "res\\frame_bl.bmp"
IDB_FRAME_BR            BITMAP  "res\\frame_br.bmp"
IDB_FRAME_TOP           BITMAP  "res\\frame_top.bmp"
IDB_FRAME_BOTTOM        BITMAP  "res\\frame_bottom.bmp"
IDB_FRAME_LEFT          BITMAP  "res\\frame_left.bmp"
IDB_FRAME_RIGHT         BITMAP  "res\\frame_right.bmp"
IDB_BTN_NORMAL          BITMAP  "res\\btn_normal.bmp"
IDB_BTN_HOVER           BITMAP  "res\\btn_hover.bmp"
IDB_BTN_PRESSED         BITMAP  "res\\btn_pressed.bmp"
IDB_TAB_ACTIVE          BITMAP  "res\\tab_active.bmp"
IDB_TAB_INACTIVE        BITMAP  "res\\tab_inactive.bmp"
IDB_CHK_UNCHECKED       BITMAP  "res\\chk_unchecked.bmp"
IDB_CHK_CHECKED         BITMAP  "res\\chk_checked.bmp"
IDB_CHK_UNCHECKED_DIS   BITMAP  "res\\chk_unchecked_dis.bmp"
IDB_CHK_CHECKED_DIS     BITMAP  "res\\chk_checked_dis.bmp"
IDB_TITLEBAR            BITMAP  "res\\titlebar.bmp"
IDB_ICO_SWORD           BITMAP  "res\\ico_sword.bmp"
IDB_ICO_HELM            BITMAP  "res\\ico_helm.bmp"
IDB_ICO_JEWEL           BITMAP  "res\\ico_jewel.bmp"
```

#### 7.4. Обновление AutoClicker.vcxproj

Добавить все BMP файлы из `res/` в секцию `<ItemGroup>` проекта как `<Image>` или `<None>`:

```xml
<ItemGroup>
  <Image Include="res\bg_panel.bmp" />
  <Image Include="res\frame_tl.bmp" />
  <!-- ... все остальные BMP файлы ... -->
</ItemGroup>
```

---

## Детальная спецификация каждой вкладки

### 8. Вкладка "General" (⚔ General)

Содержит все настройки из текущего `IDD_SETTINGSDLG` кроме класса.

#### Секция "Auto Functions"
```
┌─ Auto Functions ──────────────────────────────────────┐
│  ☐ Enable auto-pick-up (press 'SPACE')                │
│  ☐ Enable auto-repair                                 │
│  ☐ Enable auto-heal (press 'Q')    Time: [▼ 5] sec.  │
│  ☐ Auto-type '/re off' command                        │
│  ☐ Exit game at level 400                             │
└───────────────────────────────────────────────────────┘
```

#### Секция "Stealth Options"
```
┌─ Stealth Options (if s.o. approaches) ────────────────┐
│  ☐ Auto-speak           ☐ Stop zen pick-up            │
│  ☐ Stop move-to-pick    ☐ Stop all pick-up            │
└───────────────────────────────────────────────────────┘
```

**Логика:**
- Если `__CLICKER_AVANTA__` определён, секция Stealth Options заблокирована (как в текущем коде)
- "Stop all pick-up" блокирует "Stop zen pick-up" и "Stop move-to-pick" (как в текущей логике `OnCheckStopPick`)
- Auto-heal time combo включается/выключается по чекбоксу auto-heal

### 9. Вкладка "Class" (🎭 Class)

Содержит настройки выбора и конфигурации класса персонажа из текущего `IDD_SETTINGSDLG`.

```
┌─ Character Class ─────────────────────────────────────┐
│  Character Class: [▼ Dark Lord         ]              │
│  ┌────────────────────────────────────────────────┐   │
│  │  (динамическая панель настроек класса)          │   │
│  │  Attack skill slot           [▼ 1]             │   │
│  │  'Critical Damage' skill     [▼ 2] ☐           │   │
│  │  Dark horse skill slot       [▼ 3] ☐           │   │
│  │  ─────────────────────────                     │   │
│  │  No-click margin: [  70 ↕] px                  │   │
│  └────────────────────────────────────────────────┘   │
└───────────────────────────────────────────────────────┘
```

**Логика:**
- Комбобокс с 6 классами: Dark Lord, Energy Elf, Agility Elf, Blade Knight, Dark Wizard, Magic Gladiator
- При смене класса — показать соответствующую вложенную панель (как в текущем `OnSelectCharClass`)
- Все 6 вложенных диалогов (IDD_DARKLORD, IDD_EELF, IDD_AELF, IDD_BLADEKNIGHT, IDD_DARKWIZARD, IDD_MG) **переиспользуются** из текущего кода без изменений

### 10. Вкладка "Pickup" (💎 Pickup)

Содержит ВСЕ настройки из текущего `IDD_ADV_SETTINGS`.

```
┌─ Advanced Pick-up Settings ───────────────────────────┐
│  ☐ Enable advanced pick-up                            │
│                                                       │
│        Item                    Pick    Move To        │
│  ──────────────────────────────────────────           │
│  ☐ Jewel of Bless              ☐                     │
│  ☐ Jewel of Soul               ☐                     │
│  ☐ Jewel of Life               ☐                     │
│  ☐ Jewel of Guardian           ☐                     │
│  ☐ Jewel of Creation           ☐                     │
│  ☐ Jewel of Chaos              ☐                     │
│  ──────────────────────────────────────────           │
│  ☐ Excellent & Ancient Items   ☐                     │
│  ☐ Zen                         ☐                     │
│  ──────────────────────────────────────────           │
│  ☐ Custom Items (//pick xx yy) ☐                     │
│    Item code: [ 14 51 ]                               │
└───────────────────────────────────────────────────────┘
```

**Логика:**
- Master toggle "Enable advanced pick-up" управляет доступностью всех остальных контролов
- Каждый тип предмета имеет чекбокс Pick и чекбокс Move To
- Move To включается только если соответствующий Pick включён
- Custom Items имеет текстовое поле для кода предмета
- Вся логика из текущего `CAdvSettingsDialog::ApplyState()` переносится сюда

---

## Технические детали реализации

### 11. Owner-Draw отрисовка

#### 11.1. Фон окна (WM_ERASEBKGND)

```cpp
LRESULT CUnifiedSettingsDlg::OnEraseBkgnd(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
    HDC hDC = (HDC)wParam;
    if (hDC == NULL || m_hBgBitmap == NULL)
    {
        bHandled = FALSE;
        return 0;
    }

    RECT rc;
    GetClientRect(&rc);

    // Рисуем фоновую текстуру MU Online
    HDC hMemDC = CreateCompatibleDC(hDC);
    if (hMemDC == NULL)
    {
        bHandled = FALSE;
        return 0;
    }

    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, m_hBgBitmap);
    
    // Тайлинг текстуры по всей области
    BITMAP bm;
    GetObject(m_hBgBitmap, sizeof(bm), &bm);
    for (int y = rc.top; y < rc.bottom; y += bm.bmHeight)
        for (int x = rc.left; x < rc.right; x += bm.bmWidth)
            BitBlt(hDC, x, y, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
    
    SelectObject(hMemDC, hOldBmp);
    DeleteDC(hMemDC);
    
    bHandled = TRUE;
    return 1;
}
```

#### 11.2. Рамка окна (WM_NCPAINT)

Заменить текущую простую заливку `RGB(10,35,150)` на отрисовку текстурной рамки:

```cpp
LRESULT CUnifiedSettingsDlg::OnNCPaint(UINT, WPARAM, LPARAM, BOOL&)
{
    HDC hDC = GetWindowDC();
    if (hDC == NULL)
        return 0;
    
    // Отрисовка 9-patch рамки из текстур
    DrawFrameCorner(hDC, m_hFrameTL, 0, 0);  // Top-left
    DrawFrameCorner(hDC, m_hFrameTR, right-cornerW, 0);  // Top-right
    DrawFrameCorner(hDC, m_hFrameBL, 0, bottom-cornerH);  // Bottom-left
    DrawFrameCorner(hDC, m_hFrameBR, right-cornerW, bottom-cornerH);  // Bottom-right
    
    // Тайловые стороны
    DrawFrameSide(hDC, m_hFrameTop, cornerW, 0, width-2*cornerW, borderH);
    DrawFrameSide(hDC, m_hFrameBottom, cornerW, bottom-borderH, width-2*cornerW, borderH);
    DrawFrameSide(hDC, m_hFrameLeft, 0, cornerH, borderW, height-2*cornerH);
    DrawFrameSide(hDC, m_hFrameRight, right-borderW, cornerH, borderW, height-2*cornerH);
    
    // Заголовок окна
    DrawTitleBar(hDC, m_hTitleBar);
    DrawTitleText(hDC, _T("MU AutoClicker Settings"));
    
    ReleaseDC(hDC);
    return 0;
}
```

#### 11.3. Tab Control (Owner-Draw)

```cpp
// Стиль Tab Control: TCS_OWNERDRAWFIXED
// Обработка WM_DRAWITEM для табов

void CUnifiedSettingsDlg::DrawTab(LPDRAWITEMSTRUCT lpDIS)
{
    BOOL bSelected = (lpDIS->itemState & ODS_SELECTED);
    HDC hDC = lpDIS->hDC;
    RECT rc = lpDIS->rcItem;
    
    // Рисуем фон таба из текстуры
    HDC hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hBmp = bSelected ? m_hTabActive : m_hTabInactive;
    SelectObject(hMemDC, hBmp);
    StretchBlt(hDC, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top,
               hMemDC, 0, 0, tabBmpW, tabBmpH, SRCCOPY);
    DeleteDC(hMemDC);
    
    // Рисуем текст
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, bSelected ? RGB(255,230,150) : RGB(180,180,190));
    
    TCHAR szText[64];
    TCITEM tci = { TCIF_TEXT, 0, 0, szText, 63 };
    TabCtrl_GetItem(lpDIS->hwndItem, lpDIS->itemID, &tci);
    DrawText(hDC, szText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
```

#### 11.4. Кнопки (Owner-Draw)

```cpp
// Стиль кнопок: BS_OWNERDRAW
// Обработка WM_DRAWITEM для кнопок

void CUnifiedSettingsDlg::DrawButton(LPDRAWITEMSTRUCT lpDIS)
{
    BOOL bPressed = (lpDIS->itemState & ODS_SELECTED);
    BOOL bFocused = (lpDIS->itemState & ODS_FOCUS);
    
    HBITMAP hBmp = bPressed ? m_hBtnPressed : 
                   bFocused ? m_hBtnHover : m_hBtnNormal;
    
    HDC hMemDC = CreateCompatibleDC(lpDIS->hDC);
    SelectObject(hMemDC, hBmp);
    StretchBlt(lpDIS->hDC, lpDIS->rcItem.left, lpDIS->rcItem.top,
               lpDIS->rcItem.right - lpDIS->rcItem.left,
               lpDIS->rcItem.bottom - lpDIS->rcItem.top,
               hMemDC, 0, 0, btnBmpW, btnBmpH, SRCCOPY);
    DeleteDC(hMemDC);
    
    // Текст кнопки
    SetBkMode(lpDIS->hDC, TRANSPARENT);
    SetTextColor(lpDIS->hDC, RGB(255, 230, 150));
    
    TCHAR szText[64];
    GetWindowText(lpDIS->hwndItem, szText, 63);
    DrawText(lpDIS->hDC, szText, -1, &lpDIS->rcItem, 
             DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
```

#### 11.5. Секционные заголовки (Group Labels)

Вместо стандартных `GROUPBOX`, рисовать секции вручную:

```cpp
void DrawSectionHeader(HDC hDC, int x, int y, int width, LPCTSTR text)
{
    // Линия-разделитель слева
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(85, 75, 45));
    SelectObject(hDC, hPen);
    MoveToEx(hDC, x, y + 6, NULL);
    LineTo(hDC, x + 5, y + 6);
    
    // Текст заголовка секции
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(200, 170, 80)); // Золотой
    HFONT hFont = CreateFont(-11, 0, 0, 0, FW_BOLD, ...);
    SelectObject(hDC, hFont);
    
    SIZE sz;
    GetTextExtentPoint32(hDC, text, _tcslen(text), &sz);
    TextOut(hDC, x + 8, y, text, _tcslen(text));
    
    // Линия-разделитель справа
    MoveToEx(hDC, x + 12 + sz.cx, y + 6, NULL);
    LineTo(hDC, x + width, y + 6);
    
    DeleteObject(hPen);
    DeleteObject(hFont);
}
```

---

### 12. Загрузка и кэширование ресурсов

#### 12.1. Класс-хелпер для управления GDI ресурсами

Создать вспомогательный класс `CMuTheme` для управления загрузкой и освобождением всех графических ресурсов:

```cpp
// src/LordOfClicker/MuTheme.h

class CMuTheme
{
public:
    CMuTheme();
    ~CMuTheme();
    
    BOOL Initialize(HINSTANCE hInstance);
    void Cleanup();
    
    // Фоны
    HBITMAP GetBgPanel()       const { return m_hBgPanel; }
    HBITMAP GetTitleBar()      const { return m_hTitleBar; }
    
    // Рамки (9-patch)
    HBITMAP GetFrameTL()       const { return m_hFrameTL; }
    HBITMAP GetFrameTR()       const { return m_hFrameTR; }
    HBITMAP GetFrameBL()       const { return m_hFrameBL; }
    HBITMAP GetFrameBR()       const { return m_hFrameBR; }
    HBITMAP GetFrameTop()      const { return m_hFrameTop; }
    HBITMAP GetFrameBottom()   const { return m_hFrameBottom; }
    HBITMAP GetFrameLeft()     const { return m_hFrameLeft; }
    HBITMAP GetFrameRight()    const { return m_hFrameRight; }
    
    // Кнопки
    HBITMAP GetBtnNormal()     const { return m_hBtnNormal; }
    HBITMAP GetBtnHover()      const { return m_hBtnHover; }
    HBITMAP GetBtnPressed()    const { return m_hBtnPressed; }
    
    // Табы
    HBITMAP GetTabActive()     const { return m_hTabActive; }
    HBITMAP GetTabInactive()   const { return m_hTabInactive; }
    
    // Чекбоксы
    HBITMAP GetChkUnchecked()     const { return m_hChkUnchecked; }
    HBITMAP GetChkChecked()       const { return m_hChkChecked; }
    HBITMAP GetChkUncheckedDis()  const { return m_hChkUncheckedDis; }
    HBITMAP GetChkCheckedDis()    const { return m_hChkCheckedDis; }
    
    // Шрифты
    HFONT GetTitleFont()       const { return m_hTitleFont; }
    HFONT GetTabFont()         const { return m_hTabFont; }
    HFONT GetSectionFont()     const { return m_hSectionFont; }
    HFONT GetBodyFont()        const { return m_hBodyFont; }
    HFONT GetSmallFont()       const { return m_hSmallFont; }
    
    // Кисти
    HBRUSH GetBgBrush()        const { return m_hBgBrush; }
    HBRUSH GetPanelBrush()     const { return m_hPanelBrush; }
    
    // Цвета
    static COLORREF ClrWindowBg()     { return RGB(15, 15, 25); }
    static COLORREF ClrPanelBg()      { return RGB(20, 22, 35); }
    static COLORREF ClrFrameGold()    { return RGB(85, 75, 45); }
    static COLORREF ClrFrameBright()  { return RGB(180, 155, 80); }
    static COLORREF ClrTitleText()    { return RGB(200, 170, 80); }
    static COLORREF ClrBodyText()     { return RGB(210, 210, 210); }
    static COLORREF ClrActiveText()   { return RGB(255, 230, 150); }
    static COLORREF ClrDisabledText() { return RGB(120, 120, 140); }
    static COLORREF ClrCheckMark()    { return RGB(120, 200, 80); }
    static COLORREF ClrSeparator()    { return RGB(60, 55, 45); }
    
private:
    HBITMAP m_hBgPanel, m_hTitleBar;
    HBITMAP m_hFrameTL, m_hFrameTR, m_hFrameBL, m_hFrameBR;
    HBITMAP m_hFrameTop, m_hFrameBottom, m_hFrameLeft, m_hFrameRight;
    HBITMAP m_hBtnNormal, m_hBtnHover, m_hBtnPressed;
    HBITMAP m_hTabActive, m_hTabInactive;
    HBITMAP m_hChkUnchecked, m_hChkChecked, m_hChkUncheckedDis, m_hChkCheckedDis;
    
    HFONT m_hTitleFont, m_hTabFont, m_hSectionFont, m_hBodyFont, m_hSmallFont;
    HBRUSH m_hBgBrush, m_hPanelBrush;
};
```

#### 12.2. Инициализация

```cpp
BOOL CMuTheme::Initialize(HINSTANCE hInst)
{
    // Загрузка всех текстур
    m_hBgPanel = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BG_PANEL), IMAGE_BITMAP, 0, 0, 0);
    m_hTitleBar = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_TITLEBAR), IMAGE_BITMAP, 0, 0, 0);
    m_hFrameTL = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_FRAME_TL), IMAGE_BITMAP, 0, 0, 0);
    // ... загрузка всех остальных ...
    
    // Создание шрифтов
    m_hTitleFont = CreateFont(-15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
    
    m_hTabFont = CreateFont(-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
    
    m_hSectionFont = CreateFont(-11, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
    
    m_hBodyFont = CreateFont(-11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
    
    m_hSmallFont = CreateFont(-9, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
    
    // Создание кистей
    m_hBgBrush = CreateSolidBrush(ClrWindowBg());
    m_hPanelBrush = CreateSolidBrush(ClrPanelBg());
    
    return TRUE;
}
```

---

### 13. Альтернативный подход (без текстур / fallback)

Если графические ресурсы не удастся подготовить или найти, реализовать **программную отрисовку** всех элементов через GDI:

#### 13.1. Фон — градиент (с использованием GradientFill API)

```cpp
void DrawMuGradientBg(HDC hDC, const RECT& rc)
{
    // Используем GradientFill API для производительной отрисовки
    TRIVERTEX vertices[2];
    GRADIENT_RECT gRect;
    
    // Верхний цвет: RGB(15, 15, 25)
    vertices[0].x = rc.left;
    vertices[0].y = rc.top;
    vertices[0].Red   = 15 << 8;
    vertices[0].Green = 15 << 8;
    vertices[0].Blue  = 25 << 8;
    vertices[0].Alpha = 0;
    
    // Нижний цвет: RGB(20, 22, 35)
    vertices[1].x = rc.right;
    vertices[1].y = rc.bottom;
    vertices[1].Red   = 20 << 8;
    vertices[1].Green = 22 << 8;
    vertices[1].Blue  = 35 << 8;
    vertices[1].Alpha = 0;
    
    gRect.UpperLeft = 0;
    gRect.LowerRight = 1;
    
    GradientFill(hDC, vertices, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
}
```

> **Примечание:** `GradientFill` из msimg32.lib — эффективный аппаратно-ускоренный способ отрисовки градиента. Доступен начиная с Windows 2000. Для линковки добавить `msimg32.lib` в `AdditionalDependencies` файла vcxproj. Альтернативно: если msimg32 нежелателен, использовать `CreateDIBSection` с ручным заполнением пиксельного буфера.

#### 13.2. Рамка — двойная линия с золотым эффектом

```cpp
void DrawMuFrame(HDC hDC, const RECT& rc)
{
    // Внешняя тёмная рамка
    HPEN hDarkPen = CreatePen(PS_SOLID, 1, RGB(40, 35, 25));
    SelectObject(hDC, hDarkPen);
    SelectObject(hDC, GetStockObject(NULL_BRUSH));
    Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
    
    // Средняя золотая рамка
    HPEN hGoldPen = CreatePen(PS_SOLID, 2, RGB(85, 75, 45));
    SelectObject(hDC, hGoldPen);
    Rectangle(hDC, rc.left+1, rc.top+1, rc.right-1, rc.bottom-1);
    
    // Внутренняя яркая линия (highlight)
    HPEN hBrightPen = CreatePen(PS_SOLID, 1, RGB(120, 105, 60));
    SelectObject(hDC, hBrightPen);
    Rectangle(hDC, rc.left+3, rc.top+3, rc.right-3, rc.bottom-3);
    
    DeleteObject(hDarkPen);
    DeleteObject(hGoldPen);
    DeleteObject(hBrightPen);
}
```

#### 13.3. Кнопки — программная отрисовка

```cpp
void DrawMuButton(HDC hDC, const RECT& rc, LPCTSTR text, BOOL pressed, BOOL hover)
{
    COLORREF clrBg = pressed ? RGB(70, 60, 45) : 
                     hover ? RGB(55, 50, 70) : RGB(40, 38, 55);
    
    // Фон кнопки
    HBRUSH hBr = CreateSolidBrush(clrBg);
    FillRect(hDC, &rc, hBr);
    DeleteObject(hBr);
    
    // Рамка кнопки
    HPEN hPen = CreatePen(PS_SOLID, 1, pressed ? RGB(120, 105, 60) : RGB(85, 75, 45));
    SelectObject(hDC, hPen);
    SelectObject(hDC, GetStockObject(NULL_BRUSH));
    Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
    DeleteObject(hPen);
    
    // Текст
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(255, 230, 150));
    DrawText(hDC, text, -1, (LPRECT)&rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
```

---

## Список файлов для изменения

### 14. Новые файлы (создать)

| Файл | Описание |
|------|----------|
| `src/LordOfClicker/UnifiedSettingsDlg.h` | Заголовочный файл нового единого диалога |
| `src/LordOfClicker/UnifiedSettingsDlg.cpp` | Реализация нового единого диалога |
| `src/LordOfClicker/MuTheme.h` | Класс управления графическими ресурсами и темой |
| `src/LordOfClicker/MuTheme.cpp` | Реализация загрузки ресурсов |
| `src/LordOfClicker/res/` | Директория для графических ресурсов (22 BMP файла) |

### 15. Существующие файлы (изменить)

| Файл | Изменения |
|------|-----------|
| `src/LordOfClicker/MuWindow.h` | Добавить `#include "UnifiedSettingsDlg.h"`, добавить `CUnifiedSettingsDlg m_cUnifiedSettingsDlg`, закомментировать `m_cAdvSettingsDlg` |
| `src/LordOfClicker/MuWindow.cpp` | В `OnShowSettingsGUI()` — использовать `m_cUnifiedSettingsDlg` вместо условного выбора между двумя диалогами. Закомментировать логику SHIFT+F9 |
| `src/LordOfClicker/resource.h` | Добавить новые `IDD_*`, `IDB_*`, `IDC_*` идентификаторы |
| `src/LordOfClicker/AutoClicker.rc` | Добавить новые диалоги (IDD_UNIFIED_SETTINGS, IDD_TAB_GENERAL, IDD_TAB_CLASS, IDD_TAB_PICKUP), добавить BITMAP ресурсы |
| `src/LordOfClicker/AutoClicker.vcxproj` | Добавить новые .h/.cpp файлы и BMP ресурсы в проект |

### 16. Существующие файлы (НЕ изменять)

Следующие файлы должны остаться **без изменений** для обратной совместимости:

| Файл | Причина |
|------|---------|
| `src/LordOfClicker/SettingsDlg.h/cpp` | Legacy — используется для загрузки/сохранения настроек через `CClickerSettings` |
| `src/LordOfClicker/AdvSettingsDialog.h/cpp` | Legacy — код закомментирован, но сохранён для reference |
| `src/LordOfClicker/Settings.h` | Структура данных настроек — полностью переиспользуется |
| `src/LordOfClicker/DarkLordSettings.h` | Панель класса — переиспользуется как child dialog |
| `src/LordOfClicker/EElfSettings.h` | Панель класса — переиспользуется |
| `src/LordOfClicker/AElfSettings.h` | Панель класса — переиспользуется |
| `src/LordOfClicker/BKSettings.h` | Панель класса — переиспользуется |
| `src/LordOfClicker/SMSettings.h` | Панель класса — переиспользуется |
| `src/LordOfClicker/MGSettings.h` | Панель класса — переиспользуется |
| `src/LordOfClicker/ClickerJob.cpp` | Инициализация кликера — без изменений |

---

## Порядок реализации (пошагово)

### Шаг 1: Подготовка графических ресурсов
1. Найти в интернете текстуры UI из MU Online Season 3 Episode 1 (скриншоты, извлечённые ресурсы, фан-пакеты)
2. Создать/адаптировать все необходимые BMP файлы (22 штуки)
3. Поместить в `src/LordOfClicker/res/`

### Шаг 2: Обновление ресурсов проекта
1. Обновить `resource.h` — добавить все новые идентификаторы
2. Обновить `AutoClicker.rc` — добавить BITMAP ресурсы и новые шаблоны диалогов
3. Обновить `AutoClicker.vcxproj` — добавить файлы в проект

### Шаг 3: Создание MuTheme
1. Создать `MuTheme.h` и `MuTheme.cpp`
2. Реализовать загрузку всех графических ресурсов
3. Реализовать helper-функции отрисовки

### Шаг 4: Создание UnifiedSettingsDlg
1. Создать `UnifiedSettingsDlg.h` — объявление класса
2. Создать `UnifiedSettingsDlg.cpp` — реализация:
   - `OnInitDialog()` — инициализация Tab Control, child dialogs
   - `OnTabSelChange()` — переключение вкладок
   - `OnShowWindow()` — загрузка настроек
   - `OnClickedOK()` / `OnClickedCancel()` — сохранение/отмена
   - `OnEraseBkgnd()` — отрисовка фона
   - `OnNCPaint()` — отрисовка рамки и заголовка
   - `OnDrawItem()` — отрисовка кнопок и табов

### Шаг 5: Интеграция в MuWindow
1. Обновить `MuWindow.h` — добавить новый включаемый файл и член класса
2. Обновить `MuWindow.cpp` — заменить логику в `OnShowSettingsGUI()`
3. Закомментировать код SHIFT+F9

### Шаг 6: Тестирование
1. Скомпилировать проект во всех 4 конфигурациях
2. Проверить открытие единого окна по F9
3. Проверить, что SHIFT+F9 больше НЕ открывает отдельное окно
4. Проверить загрузку/сохранение всех настроек
5. Проверить корректность визуального отображения

---

## Важные ограничения и замечания

1. **C++03/C++98** — проект не использует C++11. Не применять auto, range-for, lambda, nullptr, move-семантику. Использовать `NULL` вместо `nullptr`, `0` для целочисленных значений (как в существующем коде).
2. **ATL Framework** — все диалоги используют ATL (`CAxDialogImpl`). Новый диалог тоже должен использовать ATL.
3. **Win32 API** — для GDI операций использовать стандартный Win32 API (CreateFont, LoadImage, BitBlt, StretchBlt и т.д.). Если требуется `GradientFill` — добавить линковку с `msimg32.lib`.
4. **Без WTL** — проект не использует WTL. Tab Control управляется через Win32 API (`TabCtrl_*` макросы).
5. **Обратная совместимость настроек** — формат файла настроек (`ClickerSettings` struct) НЕ ДОЛЖЕН меняться. Структура в `Settings.h` остаётся идентичной.
6. **Условная компиляция** — учитывать `__CLICKER_AVANTA__` макрос (блокировка Stealth Options).
7. **DLL контекст** — код работает внутри DLL, инжектированной в процесс игры. `HINSTANCE` доступен через `_AtlBaseModule.GetModuleInstance()`.
8. **x86 only** — проект собирается только для Win32 (x86). Не использовать x64-специфичные типы.
9. **Шрифт Tahoma** — гарантированно присутствует на всех версиях Windows, подходит для MU Online стиля.
10. **Нет зависимостей от GDI+** — использовать только стандартный GDI (без GDI+, без Direct2D, без WPF). BMP формат для всех текстур.

---

## Визуальный макет финального окна

```
╔═══════════════════════════════════════════════════════╗
║  ⚔  MU AutoClicker Settings  ⚔                      ║ ← Золотой заголовок
╠═══════════════════════════════════════════════════════╣
║  ┌──────────┐ ┌──────────┐ ┌──────────┐             ║
║  │⚔ General │ │🎭 Class  │ │💎 Pickup │             ║ ← Owner-draw табы
║  └──────────┘ └──────────┘ └──────────┘             ║
║  ┌───────────────────────────────────────────────┐   ║
║  │                                               │   ║
║  │   (Содержимое активной вкладки)               │   ║
║  │                                               │   ║
║  │   • General: авто-функции, стелс              │   ║
║  │   • Class: выбор класса + настройки           │   ║
║  │   • Pickup: расширенный подбор предметов      │   ║
║  │                                               │   ║
║  │                                               │   ║
║  │                                               │   ║
║  └───────────────────────────────────────────────┘   ║
║  ─────────────────────────────────────────────────── ║
║           [ Apply ]        [ Cancel ]                ║ ← Owner-draw кнопки
║              LordOfMU AutoClicker v2.x               ║ ← Мелкий текст версии
╚═══════════════════════════════════════════════════════╝
```

---

## Источники изображений для дизайна

Для поиска и скачивания графических ресурсов использовать следующие источники:

1. **MU Online Wiki / Fan sites** — скриншоты интерфейса Season 3 Episode 1
2. **DeviantArt** — MU Online UI элементы, созданные фанатами
3. **GitHub** — открытые MU Online эмуляторы с извлечёнными UI ресурсами
4. **MU Online Private Server resources** — пакеты клиентских ресурсов
5. **Wayback Machine** — архивные скриншоты оригинального интерфейса

Ключевые слова для поиска:
- "MU Online Season 3 UI textures"
- "MU Online interface elements PSD"
- "MU Online game window frame"
- "MU Online button textures"
- "MU Online inventory background texture"
- "MU Online Season 3 Episode 1 GUI resources"

Если оригинальные текстуры не доступны — использовать **программный fallback** (раздел 13) с GDI-отрисовкой цветов и градиентов из цветовой палитры MU Online.
