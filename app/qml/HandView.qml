import QtQuick
import QtQuick.Layouts

RowLayout {
    property var cards: []
    property bool hideFirst: false

    spacing: 5

    Repeater {
        model: cards

        CardView {
            rank: modelData.rank
            suit: modelData.suit
            faceDown: hideFirst && index === 0
        }
    }
}
