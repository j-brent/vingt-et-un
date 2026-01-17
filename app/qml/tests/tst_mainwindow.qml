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

    // Buttons under test (no Deal button - auto-deals on start)
    Button {
        id: hitButton
        text: "Hit me"
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
        visible: game.isGameOver
        onClicked: { game.newGame(); game.deal() }
    }

    // Status label under test (no "Press Deal to start" message)
    Label {
        id: statusLabel
        text: game.isGameOver ? game.resultMessage : "Your turn"
    }

    // Dealer title logic
    property string dealerTitle: "Dealer" + (game.isGameOver ? " (" + game.dealerScore + ")" : "")

    // hideFirst logic for dealer hand
    property bool dealerHideFirst: !game.isGameOver && game.gameState !== "ready"

    function init() {
        // Auto-deal at start of each test (simulating Component.onCompleted)
        game.deal()
        wait(50)
    }

    function cleanup() {
        game.newGame()
    }

    // Initial state after auto-deal
    function test_initialButtonStates() {
        if (game.gameState === "playersTurn") {
            compare(hitButton.enabled, true, "Hit should be enabled during player turn")
            compare(stayButton.enabled, true, "Stay should be enabled during player turn")
            compare(game.isGameOver, false, "Game should not be over during player turn")
        } else {
            // Blackjack scenario - game ended immediately
            compare(game.isGameOver, true, "Game should be over after blackjack")
        }
    }

    function test_initialStatusMessage() {
        if (game.gameState === "playersTurn") {
            compare(statusLabel.text, "Your turn", "Status should show 'Your turn' during play")
        } else {
            // Game over - should show result message
            verify(statusLabel.text.length > 0, "Status should show result message")
            verify(statusLabel.text !== "Your turn", "Status should be a result message")
        }
    }

    function test_initialDealerTitle() {
        if (!game.isGameOver) {
            compare(dealerTitle, "Dealer", "Dealer title should not show score during play")
        }
    }

    function test_dealerHideFirstDuringPlay() {
        if (game.gameState === "playersTurn") {
            compare(dealerHideFirst, true, "Dealer's first card should be hidden during play")
        }
    }

    // Button state transitions after Stay
    function test_buttonStatesAfterStay() {
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            tryCompare(game, "isGameOver", true, 1000)

            compare(hitButton.enabled, false, "Hit should be disabled after game over")
            compare(stayButton.enabled, false, "Stay should be disabled after game over")
        }
    }

    function test_statusMessageAfterStay() {
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            tryCompare(game, "isGameOver", true, 1000)

            verify(statusLabel.text !== "Your turn", "Status should not be 'Your turn' after stay")
        }
    }

    function test_dealerTitleShowsScoreWhenGameOver() {
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            tryCompare(game, "isGameOver", true, 1000)
        }

        verify(dealerTitle.indexOf("(") !== -1, "Dealer title should include score in parentheses")
    }

    function test_dealerCardsRevealedWhenGameOver() {
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            tryCompare(game, "isGameOver", true, 1000)
        }

        compare(dealerHideFirst, false, "Dealer's cards should be revealed when game is over")
    }

    // Button state transitions after New Game
    function test_buttonStatesAfterNewGame() {
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            tryCompare(game, "isGameOver", true, 1000)
        }

        newGameButton.clicked()
        wait(100)

        if (game.gameState === "playersTurn") {
            compare(hitButton.enabled, true, "Hit should be enabled after new game")
            compare(stayButton.enabled, true, "Stay should be enabled after new game")
            compare(game.isGameOver, false, "Game should not be over after new game")
        }
    }

    function test_statusMessageAfterNewGame() {
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            tryCompare(game, "isGameOver", true, 1000)
        }

        newGameButton.clicked()
        wait(100)

        if (game.gameState === "playersTurn") {
            compare(statusLabel.text, "Your turn", "Status should show 'Your turn' after new game")
        }
    }

    // Hit action flow
    function test_hitKeepsPlayerTurnButtons() {
        if (game.gameState === "playersTurn") {
            hitButton.clicked()
            wait(100)

            if (game.gameState === "playersTurn") {
                // Still playing - buttons should remain
                compare(hitButton.enabled, true, "Hit should still be enabled")
                compare(stayButton.enabled, true, "Stay should still be enabled")
            } else {
                // Player busted
                compare(game.isGameOver, true, "Game should be over after bust")
            }
        }
    }

    // Full game flow test
    function test_fullGameFlow() {
        // Should already be dealt (from init)
        verify(game.gameState !== "ready", "Game should have auto-dealt")

        // If player turn, stay
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            tryCompare(game, "isGameOver", true, 1000)
        }

        // Should be game over
        compare(game.isGameOver, true, "Game should be over")

        // New game (calls deal)
        newGameButton.clicked()
        wait(100)

        // Should be in a new game
        verify(game.gameState !== "ready", "New game should auto-deal")
    }

    // Regression test: New Game button must actually start a new game
    // This test catches the bug where deal() silently fails if not in Ready state
    function test_newGameButtonActuallyStartsNewGame() {
        // Step 1: Ensure we're in a game over state
        if (game.gameState === "playersTurn") {
            stayButton.clicked()
            tryCompare(game, "isGameOver", true, 1000)
        }
        compare(game.isGameOver, true, "Precondition: game must be over")

        // Step 2: Record hand sizes before clicking New Game
        // (We can't compare card contents directly, but we can verify new cards were dealt)
        var oldPlayerHandSize = game.playerHand.length
        var oldDealerHandSize = game.dealerHand.length

        // Step 3: Click New Game button
        newGameButton.clicked()
        wait(100)

        // Step 4: CRITICAL - Verify new cards were dealt
        // This catches the regression where deal() silently failed
        // Note: After new game, hands should have exactly 2 cards (fresh deal)
        // If blackjack occurs, game ends immediately but cards are still dealt
        compare(game.playerHand.length, 2, "Player should have exactly 2 cards after New Game")
        compare(game.dealerHand.length, 2, "Dealer should have exactly 2 cards after New Game")

        // Step 5: Verify game state is valid (either playing or ended with blackjack)
        if (!game.isGameOver) {
            // Normal case: player's turn
            compare(game.gameState, "playersTurn", "Should be player's turn after New Game")
            compare(game.canHit, true, "Player should be able to Hit after New Game")
            compare(game.canStay, true, "Player should be able to Stay after New Game")
        } else {
            // Blackjack case: game ended immediately (valid outcome)
            verify(game.resultMessage.length > 0, "Should have result message if game ended with blackjack")
        }
    }
}
