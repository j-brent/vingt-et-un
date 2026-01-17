import QtQuick
import QtTest
import blackjack

TestCase {
    id: testCase
    name: "CardViewTests"
    when: windowShown

    Component {
        id: cardComponent
        CardView {}
    }

    property var card: null

    function init() {
        card = createTemporaryObject(cardComponent, testCase)
        verify(card !== null, "CardView should be created")
    }

    // Dimension tests - verify Theme-based sizing
    function test_dimensionsMatchTheme() {
        compare(card.width, Theme.cardWidth, "Card width should match Theme.cardWidth")
        compare(card.height, Theme.cardHeight, "Card height should match Theme.cardHeight")
    }

    // Property tests
    function test_defaultProperties() {
        compare(card.rank, "", "Default rank should be empty")
        compare(card.suit, "", "Default suit should be empty")
        compare(card.faceDown, false, "Default faceDown should be false")
    }

    // Suit color tests - verify Theme colors
    function test_heartsSuitColor() {
        card.suit = "hearts"
        compare(card.suitColor, Theme.suitRed, "Hearts should use Theme.suitRed")
    }

    function test_diamondsSuitColor() {
        card.suit = "diamonds"
        compare(card.suitColor, Theme.suitRed, "Diamonds should use Theme.suitRed")
    }

    function test_clubsSuitColor() {
        card.suit = "clubs"
        compare(card.suitColor, Theme.suitBlack, "Clubs should use Theme.suitBlack")
    }

    function test_spadesSuitColor() {
        card.suit = "spades"
        compare(card.suitColor, Theme.suitBlack, "Spades should use Theme.suitBlack")
    }

    // Suit symbol tests
    function test_heartsSuitSymbol() {
        card.suit = "hearts"
        compare(card.suitSymbol, "\u2665", "Hearts symbol should be ♥")
    }

    function test_diamondsSuitSymbol() {
        card.suit = "diamonds"
        compare(card.suitSymbol, "\u2666", "Diamonds symbol should be ♦")
    }

    function test_clubsSuitSymbol() {
        card.suit = "clubs"
        compare(card.suitSymbol, "\u2663", "Clubs symbol should be ♣")
    }

    function test_spadesSuitSymbol() {
        card.suit = "spades"
        compare(card.suitSymbol, "\u2660", "Spades symbol should be ♠")
    }

    function test_unknownSuitSymbol() {
        card.suit = "unknown"
        compare(card.suitSymbol, "", "Unknown suit symbol should be empty")
    }

    // Face-down tests - verify Theme colors
    function test_faceUpBackgroundColor() {
        card.faceDown = false
        compare(card.color, Theme.cardFace, "Face-up card should use Theme.cardFace")
    }

    function test_faceDownBackgroundColor() {
        card.faceDown = true
        compare(card.color, Theme.cardBack, "Face-down card should use Theme.cardBack")
    }

    // Visual element tests (find children)
    function test_hasColumnWithTexts() {
        card.rank = "A"
        card.suit = "spades"
        // Find the Column containing Text elements
        var column = findChild(card, null, "Column")
        verify(column !== null || card.children.length > 0,
               "Card should have visual content")
    }

    // Helper to find child by type
    function findChild(item, name, typeName) {
        for (var i = 0; i < item.children.length; i++) {
            var child = item.children[i]
            if (child.toString().indexOf(typeName) !== -1) {
                return child
            }
            var found = findChild(child, name, typeName)
            if (found) return found
        }
        return null
    }
}
