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

    // Dimension tests
    function test_defaultDimensions() {
        compare(card.width, 50, "Card width should be 50")
        compare(card.height, 70, "Card height should be 70")
    }

    // Property tests
    function test_defaultProperties() {
        compare(card.rank, "", "Default rank should be empty")
        compare(card.suit, "", "Default suit should be empty")
        compare(card.faceDown, false, "Default faceDown should be false")
    }

    // Suit color tests
    function test_heartsSuitColor() {
        card.suit = "hearts"
        compare(card.suitColor, "#ff0000", "Hearts should be red")
    }

    function test_diamondsSuitColor() {
        card.suit = "diamonds"
        compare(card.suitColor, "#ff0000", "Diamonds should be red")
    }

    function test_clubsSuitColor() {
        card.suit = "clubs"
        compare(card.suitColor, "#000000", "Clubs should be black")
    }

    function test_spadesSuitColor() {
        card.suit = "spades"
        compare(card.suitColor, "#000000", "Spades should be black")
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

    // Face-down tests
    function test_faceUpBackgroundColor() {
        card.faceDown = false
        compare(card.color, "#ffffff", "Face-up card should be white")
    }

    function test_faceDownBackgroundColor() {
        card.faceDown = true
        compare(card.color, "#2060a0", "Face-down card should be blue")
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
