import QtQuick
import QtQuick.Layouts
import blackjack

RowLayout {
    property var cards: []
    property bool hideFirst: false

    spacing: Theme.cardSpacing

    Repeater {
        model: cards

        CardView {
            rank: modelData.rank
            suit: modelData.suit
            faceDown: hideFirst && index === 0
        }
    }
}
