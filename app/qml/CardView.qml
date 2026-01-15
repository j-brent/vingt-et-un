import QtQuick

Rectangle {
    id: card
    width: 50
    height: 70
    radius: 5
    border.color: "black"
    border.width: 1

    property string rank: ""
    property string suit: ""
    property bool faceDown: false

    color: faceDown ? "#2060A0" : "white"

    readonly property color suitColor:
        (suit === "hearts" || suit === "diamonds") ? "red" : "black"

    readonly property string suitSymbol: {
        switch(suit) {
            case "hearts": return "\u2665"
            case "diamonds": return "\u2666"
            case "clubs": return "\u2663"
            case "spades": return "\u2660"
            default: return ""
        }
    }

    Column {
        anchors.centerIn: parent
        visible: !faceDown

        Text {
            text: rank
            font.pixelSize: 16
            font.bold: true
            color: suitColor
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            text: suitSymbol
            font.pixelSize: 20
            color: suitColor
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
