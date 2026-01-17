import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import blackjack

ApplicationWindow {
    id: root
    visibility: Window.FullScreen
    visible: true
    title: "Blackjack"
    color: Theme.background

    GameController {
        id: game
    }

    // Auto-deal when app starts
    Component.onCompleted: {
        game.deal()
    }

    // Game area (left side, fills available space)
    ColumnLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: buttonPanel.left
        anchors.margins: Theme.margin
        spacing: Theme.margin

        // Dealer section
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Theme.em * 0.5

            Label {
                text: "Dealer"
                font.pixelSize: Theme.em * Theme.labelFontScale
                font.family: Theme.uiFamily
                color: Theme.textSecondary
            }

            RowLayout {
                spacing: Theme.em

                // Large score display (left of cards)
                Text {
                    text: game.isGameOver ? game.dealerScore : "?"
                    font.pixelSize: Theme.cardHeight * 0.7
                    font.family: Theme.cardFamily
                    font.bold: true
                    color: Theme.cardFace
                    Layout.alignment: Qt.AlignVCenter
                }

                HandView {
                    cards: game.dealerHand
                    hideFirst: !game.isGameOver && game.gameState !== "ready"
                }
            }
        }

        // Spacer
        Item { Layout.fillHeight: true }

        // Status message
        Label {
            text: game.isGameOver ? game.resultMessage : "Your turn"
            font.pixelSize: Theme.em * Theme.statusFontScale
            font.family: Theme.uiFamily
            font.bold: true
            color: Theme.textPrimary
            Layout.alignment: Qt.AlignHCenter
        }

        // Spacer
        Item { Layout.fillHeight: true }

        // Player section
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Theme.em * 0.5

            Label {
                text: "Your Hand"
                font.pixelSize: Theme.em * Theme.labelFontScale
                font.family: Theme.uiFamily
                color: Theme.textSecondary
            }

            RowLayout {
                spacing: Theme.em

                // Large score display (left of cards)
                Text {
                    text: game.playerScore
                    font.pixelSize: Theme.cardHeight * 0.7
                    font.family: Theme.cardFamily
                    font.bold: true
                    color: Theme.cardFace
                    Layout.alignment: Qt.AlignVCenter
                }

                HandView {
                    cards: game.playerHand
                    hideFirst: false
                }
            }
        }
    }

    // Button panel (fixed to right side)
    Column {
        id: buttonPanel
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: Theme.margin
        spacing: Theme.buttonSpacing

        Button {
            text: "New Game"
            visible: game.isGameOver
            onClicked: { game.newGame(); game.deal() }
            implicitWidth: Theme.buttonWidth
            implicitHeight: Theme.buttonHeight

            background: Rectangle {
                color: parent.hovered ? Theme.buttonBgHover : Theme.buttonBg
                radius: Theme.buttonRadius
            }

            contentItem: Text {
                text: parent.text
                font.pixelSize: Theme.em * Theme.labelFontScale
                font.family: Theme.uiFamily
                color: Theme.buttonText
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Button {
            text: "Stay"
            enabled: game.canStay
            onClicked: game.stay()
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

        Button {
            text: "Hit me"
            enabled: game.canHit
            onClicked: game.hit()
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
    }

    // Escape key quits the application
    Shortcut {
        sequence: "Escape"
        onActivated: Qt.quit()
    }
}
