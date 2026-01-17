import QtQuick
import blackjack

Rectangle {
    id: card
    width: Theme.cardWidth
    height: Theme.cardHeight
    radius: Theme.cardRadius
    border.color: Theme.border
    border.width: Theme.cardBorder

    property string rank: ""
    property string suit: ""
    property bool faceDown: false

    color: faceDown ? Theme.cardBack : Theme.cardFace

    readonly property color suitColor:
        (suit === "hearts" || suit === "diamonds") ? Theme.suitRed : Theme.suitBlack

    readonly property string suitSymbol: {
        switch(suit) {
            case "hearts": return "\u2665"
            case "diamonds": return "\u2666"
            case "clubs": return "\u2663"
            case "spades": return "\u2660"
            default: return ""
        }
    }

    // Card content - rank and suit on left side
    Column {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: Theme.em * 0.5
        visible: !faceDown
        spacing: Theme.em * 0.25

        Text {
            text: rank
            font.pixelSize: Theme.em * Theme.rankFontScale
            font.family: Theme.cardFamily
            font.bold: true
            color: suitColor
        }
        Text {
            text: suitSymbol
            font.pixelSize: Theme.em * Theme.suitFontScale
            color: suitColor
        }
    }
}
