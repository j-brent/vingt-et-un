# QML UI Tests

## Test Strategy

These tests verify the QML UI layer independently of the C++ game logic. The C++ `Game`, `Deck`, and `Card` classes are tested separately via Catch2.

**What we test here:**
- QML property bindings and computed properties
- Signal emission from GameController
- UI state transitions (button enable/disable, visibility)
- Visual properties (colors, dimensions, symbols)
- Data flow from controller to view components

**What we don't test here:**
- Game rules (win/lose conditions, scoring) - covered by C++ tests
- Deck shuffling/dealing correctness - covered by C++ tests
- Card value calculations - covered by C++ tests

## Test Files

| File | Component | Purpose |
|------|-----------|---------|
| `tst_gamecontroller.qml` | GameController | State machine, signals, property updates |
| `tst_cardview.qml` | CardView | Visual rendering of individual cards |
| `tst_handview.qml` | HandView | Card collection display and hiding |
| `tst_mainwindow.qml` | Main.qml | Full UI integration and game flow |

## Running Tests

```sh
# Full build + test
cmake --workflow --preset windows-qml-release

# Run only QML tests
ctest --preset windows-qml-release -R "blackjack-qml"

# Verbose output
ctest --preset windows-qml-release -R "blackjack-qml" -V
```
