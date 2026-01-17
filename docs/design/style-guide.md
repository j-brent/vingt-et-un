# Visual Style Guide

This document defines the visual design system for the Blackjack application. The same design tokens are shared across all UI implementations (QML, web) via generated theme files.

## Design Philosophy

- **Dark theme** - Reduces eye strain, modern aesthetic
- **Large cards** - Primary game element should dominate the viewport
- **Classic typography** - Serif fonts on cards evoke traditional playing cards
- **Relative sizing** - Use `em` units for cross-platform consistency
- **Full screen** - Immersive gaming experience

## Design Token Architecture

```
styles/tokens.json          (source of truth)
        │
        ├──► app/qml/Theme.qml    (QML singleton)
        │
        └──► styles/theme.css     (CSS custom properties)
```

Edit `tokens.json`, then run:
```bash
node scripts/generate-theme.js
```

---

## Color Palette

| Token | Value | Usage |
|-------|-------|-------|
| `background` | `#121212` | App background (Material Design dark) |
| `cardFace` | `#fffef0` | Card front (cream/off-white) |
| `cardBack` | `#2060a0` | Card back (blue) |
| `suitRed` | `#cc0000` | Hearts, diamonds |
| `suitBlack` | `#1a1a1a` | Clubs, spades |
| `textPrimary` | `#ffffff` | Main text on dark background |
| `textSecondary` | `#b0b0b0` | Labels, secondary text |
| `buttonBg` | `#333333` | Button background |
| `buttonBgHover` | `#444444` | Button hover state |
| `buttonBgDisabled` | `#222222` | Disabled button |
| `buttonText` | `#ffffff` | Button label |
| `buttonTextDisabled` | `#666666` | Disabled button label |
| `border` | `#444444` | Card borders, dividers |

### Color Rationale

- **Background (#121212)**: Material Design recommended dark surface color
- **Card face (#fffef0)**: Cream tone matches real playing cards, softer than pure white
- **Suit red (#cc0000)**: Darker red for better contrast on cream background
- **Suit black (#1a1a1a)**: Not pure black to reduce harshness

---

## Typography

| Token | Value | Usage |
|-------|-------|-------|
| `cardFamily` | `Georgia, 'Times New Roman', serif` | Card rank and suit |
| `uiFamily` | `system-ui, -apple-system, sans-serif` | Buttons, labels, status |

### Font Scale (em multipliers)

| Token | Value | Computed (16px base) | Usage |
|-------|-------|----------------------|-------|
| `rankFontScale` | `4` | 64px | Card rank (A, K, 10, etc.) |
| `suitFontScale` | `2.5` | 40px | Suit symbol (♠ ♥ ♦ ♣) |
| `statusFontScale` | `1.5` | 24px | Game status message |
| `labelFontScale` | `1.25` | 20px | Section labels, buttons |

---

## Sizing

Base unit: `1em = 16px` (standard browser default)

### Cards

| Token | Value | Computed | Description |
|-------|-------|----------|-------------|
| `cardWidth` | `8em` | 128px | Card width |
| `cardHeight` | `11em` | 176px | Card height (1.375 aspect ratio) |
| `cardRadius` | `0.5em` | 8px | Corner radius |
| `cardSpacing` | `1em` | 16px | Gap between cards in hand |
| `cardBorder` | `0.125em` | 2px | Border thickness |

### Buttons

| Token | Value | Computed | Description |
|-------|-------|----------|-------------|
| `buttonWidth` | `10em` | 160px | Button width |
| `buttonHeight` | `3em` | 48px | Button height |
| `buttonRadius` | `0.375em` | 6px | Corner radius |
| `buttonSpacing` | `1em` | 16px | Gap between buttons |

### Layout

| Token | Value | Computed | Description |
|-------|-------|----------|-------------|
| `margin` | `2em` | 32px | Outer margin from screen edge |

---

## Layout Structure

```
+------------------------------------------------------------------+
|                                                         [BUTTONS] |
|   +--------+                                             [Deal]   |
|   | A      |  +--------+ +--------+                      [Hit]    |
|   |        |  | K      | | 7      |    DEALER            [Stay]   |
|   |   ♠    |  |   ♥    | |   ♦    |                      [New]    |
|   +--------+  +--------+ +--------+                               |
|                                                                   |
|                       STATUS MESSAGE                              |
|                                                                   |
|   +--------+ +--------+ +--------+                                |
|   | 10     | | 5      | | A      |    YOUR HAND (16)              |
|   |        | |        | |        |                                |
|   |   ♣    | |   ♠    | |   ♥    |                                |
|   +--------+ +--------+ +--------+                                |
+------------------------------------------------------------------+
```

### Card Layout

- Rank positioned **top-left** of card
- Suit symbol **below** rank
- Both **left-aligned** with margin
- Face-down cards show solid blue back

---

## QML Implementation

### Importing Theme

```qml
import blackjack  // Theme is auto-imported as singleton
```

### Using Colors

```qml
Rectangle {
    color: Theme.background
    border.color: Theme.border
}

Text {
    color: Theme.textPrimary
}
```

### Using Sizes

```qml
Rectangle {
    width: Theme.cardWidth      // 8em = 128px
    height: Theme.cardHeight    // 11em = 176px
    radius: Theme.cardRadius
}

// Font sizes use em * scale
Text {
    font.pixelSize: Theme.em * Theme.rankFontScale  // 64px
    font.family: Theme.cardFamily
}
```

### Button Styling

```qml
Button {
    implicitWidth: Theme.buttonWidth
    implicitHeight: Theme.buttonHeight

    background: Rectangle {
        color: parent.enabled ?
               (parent.hovered ? Theme.buttonBgHover : Theme.buttonBg) :
               Theme.buttonBgDisabled
        radius: Theme.buttonRadius
    }

    contentItem: Text {
        text: parent.text
        font.pixelSize: Theme.em * Theme.labelFontScale
        font.family: Theme.uiFamily
        color: parent.enabled ? Theme.buttonText : Theme.buttonTextDisabled
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
```

### Full Screen Window

```qml
ApplicationWindow {
    visibility: Window.FullScreen
    color: Theme.background

    // Allow Escape to exit fullscreen
    Shortcut {
        sequence: "Escape"
        onActivated: root.visibility = Window.Windowed
    }
}
```

---

## CSS Implementation

### Importing Theme

```html
<link rel="stylesheet" href="styles/theme.css">
```

### Using Colors

```css
body {
    background-color: var(--color-background);
    color: var(--color-text-primary);
}

.card {
    background-color: var(--color-card-face);
    border-color: var(--color-border);
}

.card.face-down {
    background-color: var(--color-card-back);
}

.suit-red {
    color: var(--color-suit-red);
}

.suit-black {
    color: var(--color-suit-black);
}
```

### Using Sizes

```css
.card {
    width: var(--card-width);           /* 8em */
    height: var(--card-height);         /* 11em */
    border-radius: var(--card-radius);  /* 0.5em */
    border-width: var(--card-border);   /* 0.125em */
}

.hand {
    gap: var(--card-spacing);           /* 1em */
}

.card .rank {
    font-size: calc(1em * var(--rank-font-scale));  /* 4em */
    font-family: var(--font-card-family);
}

.card .suit {
    font-size: calc(1em * var(--suit-font-scale));  /* 2.5em */
}
```

### Button Styling

```css
.button {
    width: var(--button-width);
    height: var(--button-height);
    border-radius: var(--button-radius);
    background-color: var(--color-button-bg);
    color: var(--color-button-text);
    font-size: calc(1em * var(--label-font-scale));
    font-family: var(--font-ui-family);
}

.button:hover {
    background-color: var(--color-button-bg-hover);
}

.button:disabled {
    background-color: var(--color-button-bg-disabled);
    color: var(--color-button-text-disabled);
}
```

### Full Screen Layout

```css
html, body {
    margin: 0;
    padding: 0;
    height: 100vh;
    background-color: var(--color-background);
}

.game-container {
    display: flex;
    height: 100vh;
    padding: var(--margin);
    box-sizing: border-box;
}

.game-area {
    flex: 1;
    display: flex;
    flex-direction: column;
    justify-content: space-between;
}

.button-panel {
    display: flex;
    flex-direction: column;
    gap: var(--button-spacing);
    justify-content: center;
}
```

---

## Token Reference (tokens.json)

```json
{
  "colors": {
    "background": "#121212",
    "cardFace": "#fffef0",
    "cardBack": "#2060a0",
    "suitRed": "#cc0000",
    "suitBlack": "#1a1a1a",
    "textPrimary": "#ffffff",
    "textSecondary": "#b0b0b0",
    "buttonBg": "#333333",
    "buttonBgHover": "#444444",
    "buttonBgDisabled": "#222222",
    "buttonText": "#ffffff",
    "buttonTextDisabled": "#666666",
    "border": "#444444"
  },
  "sizing": {
    "cardWidthEm": 8,
    "cardHeightEm": 11,
    "cardRadiusEm": 0.5,
    "cardSpacingEm": 1,
    "cardBorderEm": 0.125,
    "buttonWidthEm": 10,
    "buttonHeightEm": 3,
    "buttonRadiusEm": 0.375,
    "buttonSpacingEm": 1,
    "rankFontScale": 4,
    "suitFontScale": 2.5,
    "statusFontScale": 1.5,
    "labelFontScale": 1.25,
    "marginEm": 2
  },
  "fonts": {
    "cardFamily": "Georgia, 'Times New Roman', serif",
    "uiFamily": "system-ui, -apple-system, sans-serif"
  }
}
```

---

## Regenerating Theme Files

After editing `tokens.json`:

```bash
node scripts/generate-theme.js
```

This generates:
- `app/qml/Theme.qml` - QML singleton
- `styles/theme.css` - CSS custom properties

Both files have a header comment indicating they are auto-generated.
