// Auto-generated from styles/tokens.json - do not edit directly
// Run: node scripts/generate-theme.js
pragma Singleton
import QtQuick

QtObject {
    id: theme

    // Base unit (1em equivalent) - 16px is standard browser default
    readonly property real em: 16

    // Colors
    readonly property color background: "#121212"
    readonly property color cardFace: "#fffef0"
    readonly property color cardBack: "#2060a0"
    readonly property color suitRed: "#cc0000"
    readonly property color suitBlack: "#1a1a1a"
    readonly property color textPrimary: "#ffffff"
    readonly property color textSecondary: "#b0b0b0"
    readonly property color buttonBg: "#333333"
    readonly property color buttonBgHover: "#444444"
    readonly property color buttonBgDisabled: "#222222"
    readonly property color buttonText: "#ffffff"
    readonly property color buttonTextDisabled: "#666666"
    readonly property color border: "#444444"

    // Sizes (em-based)
    readonly property real cardWidth: em * 8
    readonly property real cardHeight: em * 11
    readonly property real cardRadius: em * 0.5
    readonly property real cardSpacing: em * 1
    readonly property real cardBorder: em * 0.125
    readonly property real buttonWidth: em * 10
    readonly property real buttonHeight: em * 3
    readonly property real buttonRadius: em * 0.375
    readonly property real buttonSpacing: em * 1
    readonly property real rankFontScale: 4
    readonly property real suitFontScale: 2.5
    readonly property real statusFontScale: 1.5
    readonly property real labelFontScale: 1.25
    readonly property real margin: em * 2

    // Fonts
    readonly property string cardFamily: "Georgia, 'Times New Roman', serif"
    readonly property string uiFamily: "system-ui, -apple-system, sans-serif"
}
