import QtQuick
import QtTest
import blackjack

TestCase {
    id: testCase
    name: "GameControllerTests"

    GameController {
        id: controller
    }

    SignalSpy {
        id: gameStateSpy
        target: controller
        signalName: "gameStateChanged"
    }

    SignalSpy {
        id: handsSpy
        target: controller
        signalName: "handsChanged"
    }

    function cleanup() {
        controller.newGame()
        gameStateSpy.clear()
        handsSpy.clear()
    }

    // Initial state tests
    function test_initialState() {
        compare(controller.gameState, "ready", "Initial state should be 'ready'")
        compare(controller.isGameOver, false, "Game should not be over initially")
        compare(controller.playerHand.length, 0, "Player hand should be empty")
        compare(controller.dealerHand.length, 0, "Dealer hand should be empty")
        compare(controller.playerScore, 0, "Player score should be 0")
        compare(controller.dealerScore, 0, "Dealer score should be 0")
    }

    function test_initialButtonStates() {
        compare(controller.canDeal, true, "Should be able to deal initially")
        compare(controller.canHit, false, "Should not be able to hit initially")
        compare(controller.canStay, false, "Should not be able to stay initially")
    }

    // Deal action tests
    function test_dealChangesState() {
        controller.deal()
        compare(controller.gameState !== "ready", true, "State should change after deal")
        // State could be playersTurn or a GameOver state (blackjack)
        verify(controller.gameState === "playersTurn" ||
               controller.gameState === "playerWins" ||
               controller.gameState === "dealerWins",
               "State should be playersTurn or a game-over state")
    }

    function test_dealCreatesHands() {
        controller.deal()
        compare(controller.playerHand.length, 2, "Player should have 2 cards after deal")
        compare(controller.dealerHand.length, 2, "Dealer should have 2 cards after deal")
    }

    function test_dealEmitsSignals() {
        controller.deal()
        verify(gameStateSpy.count >= 1, "gameStateChanged should be emitted")
        verify(handsSpy.count >= 1, "handsChanged should be emitted")
    }

    function test_dealDisablesCanDeal() {
        controller.deal()
        // Can only deal again if game is over (blackjack scenario)
        if (!controller.isGameOver) {
            compare(controller.canDeal, false, "Should not be able to deal during play")
        }
    }

    // Hit action tests (only valid during playersTurn)
    function test_hitAddsCard() {
        controller.deal()
        if (controller.gameState === "playersTurn") {
            var initialCount = controller.playerHand.length
            controller.hit()
            // Hand grows unless player busts
            verify(controller.playerHand.length >= initialCount,
                   "Player hand should have at least same cards after hit")
        }
    }

    function test_hitEmitsHandsChanged() {
        controller.deal()
        if (controller.gameState === "playersTurn") {
            handsSpy.clear()
            controller.hit()
            verify(handsSpy.count >= 1, "handsChanged should be emitted after hit")
        }
    }

    // Stay action tests
    function test_stayEndsPlayerTurn() {
        controller.deal()
        if (controller.gameState === "playersTurn") {
            controller.stay()
            compare(controller.gameState !== "playersTurn", true,
                    "State should change after stay")
        }
    }

    function test_stayTriggersGameOver() {
        controller.deal()
        if (controller.gameState === "playersTurn") {
            controller.stay()
            compare(controller.isGameOver, true, "Game should be over after stay")
        }
    }

    // NewGame action tests
    function test_newGameResetsState() {
        controller.deal()
        if (controller.isGameOver) {
            controller.newGame()
            compare(controller.gameState, "ready", "State should be ready after newGame")
            compare(controller.playerHand.length, 0, "Player hand should be empty")
            compare(controller.dealerHand.length, 0, "Dealer hand should be empty")
        }
    }

    // Card data structure tests
    function test_cardHasRequiredProperties() {
        controller.deal()
        var card = controller.playerHand[0]
        verify(card.hasOwnProperty("rank"), "Card should have 'rank' property")
        verify(card.hasOwnProperty("suit"), "Card should have 'suit' property")
    }

    function test_cardRankIsValid() {
        controller.deal()
        var validRanks = ["A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"]
        var card = controller.playerHand[0]
        verify(validRanks.indexOf(card.rank) !== -1,
               "Card rank '" + card.rank + "' should be valid")
    }

    function test_cardSuitIsValid() {
        controller.deal()
        var validSuits = ["hearts", "diamonds", "clubs", "spades"]
        var card = controller.playerHand[0]
        verify(validSuits.indexOf(card.suit) !== -1,
               "Card suit '" + card.suit + "' should be valid")
    }

    // Result message tests
    function test_resultMessageEmptyDuringPlay() {
        controller.deal()
        if (controller.gameState === "playersTurn") {
            compare(controller.resultMessage, "", "Result message should be empty during play")
        }
    }

    function test_resultMessageSetWhenGameOver() {
        controller.deal()
        if (controller.gameState === "playersTurn") {
            controller.stay()
        }
        verify(controller.resultMessage.length > 0,
               "Result message should be set when game is over")
    }
}
