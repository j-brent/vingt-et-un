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

        // Player section - supports up to 4 split hands
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Theme.em * 0.5

            Label {
                text: game.handCount > 1 ? "Your Hands" : "Your Hand"
                font.pixelSize: Theme.em * Theme.labelFontScale
                font.family: Theme.uiFamily
                color: Theme.textSecondary
            }

            // Single hand display (normal play)
            RowLayout {
                visible: game.handCount <= 1
                spacing: Theme.em

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

            // Multiple hands display (split play)
            Column {
                visible: game.handCount > 1
                spacing: Theme.em * 0.5

                Repeater {
                    model: game.playerHands

                    // Each hand in a Row (not RowLayout - avoids overlap bug)
                    Row {
                        spacing: Theme.em * 0.5
                        height: Theme.cardHeight * 0.7 + Theme.em

                        // Hand indicator (number, active marker)
                        Rectangle {
                            width: Theme.em * 2.5
                            height: Theme.em * 2.5
                            radius: Theme.em * 0.3
                            color: modelData.isActive ? Theme.activeHandBg : "transparent"
                            border.color: modelData.isActive ? Theme.activeHandBorder : Theme.textSecondary
                            border.width: modelData.isActive ? 2 : 1
                            anchors.verticalCenter: parent.verticalCenter

                            Text {
                                anchors.centerIn: parent
                                text: (index + 1)
                                font.pixelSize: Theme.em
                                font.family: Theme.uiFamily
                                font.bold: modelData.isActive
                                color: modelData.isActive ? Theme.activeHandText : Theme.textSecondary
                            }
                        }

                        // Score display (large font with proper container width)
                        Text {
                            text: modelData.score
                            width: Theme.em * 6
                            font.pixelSize: Theme.cardHeight * 0.5
                            font.family: Theme.cardFamily
                            font.bold: true
                            color: modelData.isBusted ? Theme.bustedText : Theme.cardFace
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignRight
                        }

                        // Status indicator
                        Text {
                            visible: modelData.isComplete
                            text: modelData.isBusted ? "BUST" : "DONE"
                            width: visible ? Theme.em * 3.5 : 0
                            font.pixelSize: Theme.em * 0.8
                            font.family: Theme.uiFamily
                            font.bold: true
                            color: modelData.isBusted ? Theme.bustedText : Theme.completeText
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        // Spacer when no status
                        Item {
                            visible: !modelData.isComplete
                            width: Theme.em * 0.5
                            height: 1
                        }

                        HandView {
                            cards: modelData.cards
                            hideFirst: false
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
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

        Button {
            text: "Split"
            visible: game.canSplit
            onClicked: game.split()
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
    }

    // Escape key quits the application
    Shortcut {
        sequence: "Escape"
        onActivated: Qt.quit()
    }
}
