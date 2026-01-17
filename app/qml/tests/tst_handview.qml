import QtQuick
import QtTest
import blackjack

TestCase {
    id: testCase
    name: "HandViewTests"
    when: windowShown

    Component {
        id: handComponent
        HandView {}
    }

    property var hand: null

    function init() {
        hand = createTemporaryObject(handComponent, testCase)
        verify(hand !== null, "HandView should be created")
    }

    // Default state tests
    function test_defaultProperties() {
        compare(hand.cards.length, 0, "Default cards should be empty array")
        compare(hand.hideFirst, false, "Default hideFirst should be false")
    }

    function test_spacingMatchesTheme() {
        compare(hand.spacing, Theme.cardSpacing, "Spacing should match Theme.cardSpacing")
    }

    // Card population tests
    function test_emptyHandHasNoCards() {
        hand.cards = []
        // Repeater should produce no children
        wait(50) // Allow binding to update
        var cardCount = countCardViews(hand)
        compare(cardCount, 0, "Empty hand should have no CardViews")
    }

    function test_singleCardHand() {
        hand.cards = [{ rank: "A", suit: "spades" }]
        wait(50)
        var cardCount = countCardViews(hand)
        compare(cardCount, 1, "Single card hand should have 1 CardView")
    }

    function test_twoCardHand() {
        hand.cards = [
            { rank: "A", suit: "spades" },
            { rank: "K", suit: "hearts" }
        ]
        wait(50)
        var cardCount = countCardViews(hand)
        compare(cardCount, 2, "Two card hand should have 2 CardViews")
    }

    function test_fiveCardHand() {
        hand.cards = [
            { rank: "2", suit: "clubs" },
            { rank: "3", suit: "diamonds" },
            { rank: "4", suit: "hearts" },
            { rank: "5", suit: "spades" },
            { rank: "6", suit: "clubs" }
        ]
        wait(50)
        var cardCount = countCardViews(hand)
        compare(cardCount, 5, "Five card hand should have 5 CardViews")
    }

    // hideFirst tests
    function test_hideFirstFalseAllFaceUp() {
        hand.cards = [
            { rank: "A", suit: "spades" },
            { rank: "K", suit: "hearts" }
        ]
        hand.hideFirst = false
        wait(50)

        var cards = getCardViews(hand)
        if (cards.length >= 2) {
            compare(cards[0].faceDown, false, "First card should be face up")
            compare(cards[1].faceDown, false, "Second card should be face up")
        }
    }

    function test_hideFirstTrueFirstCardFaceDown() {
        hand.cards = [
            { rank: "A", suit: "spades" },
            { rank: "K", suit: "hearts" }
        ]
        hand.hideFirst = true
        wait(50)

        var cards = getCardViews(hand)
        if (cards.length >= 2) {
            compare(cards[0].faceDown, true, "First card should be face down")
            compare(cards[1].faceDown, false, "Second card should be face up")
        }
    }

    // Card data binding tests
    function test_cardPropertiesAreBound() {
        hand.cards = [{ rank: "Q", suit: "diamonds" }]
        wait(50)

        var cards = getCardViews(hand)
        if (cards.length >= 1) {
            compare(cards[0].rank, "Q", "Card rank should be bound")
            compare(cards[0].suit, "diamonds", "Card suit should be bound")
        }
    }

    // Helper functions
    function countCardViews(parent) {
        var count = 0
        for (var i = 0; i < parent.children.length; i++) {
            var child = parent.children[i]
            // Repeater creates items, check for CardView-like objects
            if (child.hasOwnProperty("rank") && child.hasOwnProperty("suit")) {
                count++
            }
        }
        return count
    }

    function getCardViews(parent) {
        var cards = []
        for (var i = 0; i < parent.children.length; i++) {
            var child = parent.children[i]
            if (child.hasOwnProperty("rank") && child.hasOwnProperty("suit")) {
                cards.push(child)
            }
        }
        return cards
    }
}
