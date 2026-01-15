import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import blackjack

ApplicationWindow {
    id: root
    width: 600
    height: 500
    visible: true
    title: "Blackjack"

    GameController {
        id: game
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // Dealer section
        GroupBox {
            title: "Dealer" + (game.isGameOver ? " (" + game.dealerScore + ")" : "")
            Layout.fillWidth: true

            HandView {
                cards: game.dealerHand
                hideFirst: !game.isGameOver && game.gameState !== "ready"
            }
        }

        // Status message
        Label {
            text: game.isGameOver ? game.resultMessage :
                  (game.gameState === "ready" ? "Press Deal to start" : "Your turn")
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Player section
        GroupBox {
            title: "Your Hand (" + game.playerScore + ")"
            Layout.fillWidth: true

            HandView {
                cards: game.playerHand
                hideFirst: false
            }
        }

        // Action buttons
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 10

            Button {
                text: "Deal"
                enabled: game.canDeal
                onClicked: game.deal()
            }
            Button {
                text: "Hit"
                enabled: game.canHit
                onClicked: game.hit()
            }
            Button {
                text: "Stay"
                enabled: game.canStay
                onClicked: game.stay()
            }
            Button {
                text: "New Game"
                enabled: game.isGameOver
                onClicked: game.newGame()
            }
        }
    }
}
