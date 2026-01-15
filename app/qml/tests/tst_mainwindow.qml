import QtQuick
import QtQuick.Controls
import QtTest
import blackjack

TestCase {
    id: testCase
    name: "MainWindowIntegrationTests"
    when: windowShown
    width: 600
    height: 500

    // Recreate the Main.qml structure for testing
    GameController {
        id: game
    }

    // Buttons under test
    Button {
        id: dealButton
        text: "Deal"
        enabled: game.canDeal
        onClicked: game.deal()
    }

    Button {
        id: hitButton
        text: "Hit"
        enabled: game.canHit
        onClicked: game.hit()
    }

    Button {
        id: stayButton
        text: "Stay"
        enabled: game.canStay
        onClicked: game.stay()
    }

    Button {
        id: newGameButton
        text: "New Game"
        enabled: game.isGameOver
        onClicked: game.newGame()
    }

    // Status label under test
    Label {
        id: statusLabel
        text: game.isGameOver ? game.resultMessage :
              (game.gameState === "ready" ? "Press Deal to start" : "Your turn")
    }

    // Dealer title logic
    property string dealerTitle: "Dealer" + (game.isGameOver ? " (" + game.dealerScore + ")" : "")

    // hideFirst logic for dealer hand
    property bool dealerHideFirst: !game.isGameOver && game.gameState !== "ready"

    function cleanup() {
        game.newGame()
    }

    // Initial UI state tests
    function test_initialButtonStates() {
        compare(dealButton.enabled, true, "Deal button should be enabled initially")
        compare(hitButton.enabled, false, "Hit button should be disabled initially")
        compare(stayButton.enabled, false, "Stay button should be disabled initially")
        compare(newGameButton.enabled, false, "New Game button should be disabled initially")
    }

    function test_initialStatusMessage() {
        compare(statusLabel.text, "Press Deal to start", "Initial status should prompt to deal")
    }

    function test_initialDealerTitle() {
        compare(dealerTitle, "Dealer", "Dealer title should not show score initially")
    }

    function test_initialDealerHideFirst() {
        compare(dealerHideFirst, false, "Dealer cards should not be hidden before deal")
    }

    // Button state transitions after Deal
    function test_buttonStatesAfterDeal() {
        dealButton.clicked()
        wait(50)

        compare(dealButton.enabled, false, "Deal should be disabled after deal")

        if (game.gameState === "playersTurn") {
            compare(hitButton.enabled, true, "Hit should be enabled during player turn")
            compare(stayButton.enabled, true, "Stay should be enabled during player turn")
            compare(newGameButton.enabled, false, "New Game should be disabled during play")
        } else {
            // Blackjack scenario - game ended immediately
            compare(newGameButton.enabled, true, "New Game should be enabled after blackjack")
        }
    }

    function test_statusMessageAfterDeal() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            compare(statusLabel.text, "Your turn", "Status should show 'Your turn' during play")
        } else {
            // Game over - should show result message
            verify(statusLabel.text.length > 0, "Status should show result message")
            verify(statusLabel.text !== "Your turn" && statusLabel.text !== "Press Deal to start",
                   "Status should be a result message")
        }
    }

    function test_dealerHideFirstDuringPlay() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            compare(dealerHideFirst, true, "Dealer's first card should be hidden during play")
        }
    }

    // Button state transitions after Stay
    function test_buttonStatesAfterStay() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            wait(50)

            compare(dealButton.enabled, false, "Deal should be disabled after game over")
            compare(hitButton.enabled, false, "Hit should be disabled after game over")
            compare(stayButton.enabled, false, "Stay should be disabled after game over")
            compare(newGameButton.enabled, true, "New Game should be enabled after game over")
        }
    }

    function test_statusMessageAfterStay() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            wait(50)

            verify(statusLabel.text !== "Your turn", "Status should not be 'Your turn' after stay")
            verify(statusLabel.text !== "Press Deal to start", "Status should show game result")
        }
    }

    function test_dealerTitleShowsScoreWhenGameOver() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            wait(50)
        }

        verify(dealerTitle.indexOf("(") !== -1, "Dealer title should include score in parentheses")
    }

    function test_dealerCardsRevealedWhenGameOver() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            wait(50)
        }

        compare(dealerHideFirst, false, "Dealer's cards should be revealed when game is over")
    }

    // Button state transitions after New Game
    function test_buttonStatesAfterNewGame() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            wait(50)
        }

        newGameButton.clicked()
        wait(50)

        compare(dealButton.enabled, true, "Deal should be enabled after new game")
        compare(hitButton.enabled, false, "Hit should be disabled after new game")
        compare(stayButton.enabled, false, "Stay should be disabled after new game")
        compare(newGameButton.enabled, false, "New Game should be disabled after new game")
    }

    function test_statusMessageAfterNewGame() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            wait(50)
        }

        newGameButton.clicked()
        wait(50)

        compare(statusLabel.text, "Press Deal to start", "Status should reset after new game")
    }

    // Hit action flow
    function test_hitKeepsPlayerTurnButtons() {
        dealButton.clicked()
        wait(50)

        if (game.gameState === "playersTurn") {
            var initialScore = game.playerScore
            hitButton.clicked()
            wait(50)

            if (game.gameState === "playersTurn") {
                // Still playing - buttons should remain
                compare(hitButton.enabled, true, "Hit should still be enabled")
                compare(stayButton.enabled, true, "Stay should still be enabled")
            } else {
                // Player busted
                compare(newGameButton.enabled, true, "New Game should be enabled after bust")
            }
        }
    }

    // Full game flow test
    function test_fullGameFlowDealStayNewGame() {
        // Initial
        compare(game.gameState, "ready")

        // Deal
        dealButton.clicked()
        wait(50)
        verify(game.gameState !== "ready", "State should change after deal")

        // If player turn, stay
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            wait(50)
        }

        // Should be game over
        compare(game.isGameOver, true, "Game should be over")

        // New game
        newGameButton.clicked()
        wait(50)

        // Back to ready
        compare(game.gameState, "ready", "Should be back to ready state")
    }
}
