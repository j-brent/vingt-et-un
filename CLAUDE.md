# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Console Application

```sh
# Windows
cmake --preset windows
cmake --build --preset windows-release   # or windows-debug

# Linux
cmake --preset linux
cmake --build --preset linux-release     # or linux-debug
```

### QML GUI Application

Requires Qt 6.8+ installed via vcpkg or system package manager.

```sh
# Windows (with vcpkg)
cmake --preset windows-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset windows-qml-release

# Linux GCC (with vcpkg)
cmake --preset linux-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset linux-qml-release

# Linux Clang (with vcpkg)
cmake --preset linux-clang-qml -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build --preset linux-clang-qml-release
```

**Installing Qt via vcpkg:**
```sh
# Windows
vcpkg install qtbase:x64-windows qtdeclarative:x64-windows

# Linux
vcpkg install qtbase:x64-linux qtdeclarative:x64-linux
```

**Windows deployment** (copy Qt DLLs to build directory):
```sh
windeployqt --qmldir app/qml build-win-qml/Release/blackjack-qml.exe
```

## Testing

Tests use Catch2 (fetched automatically via CMake).

```sh
# Run all tests
ctest --preset windows-release   # or linux-release

# Run single test by name
ctest --preset windows-release -R "Game states"

# Full workflow (configure + build + test)
cmake --workflow --preset windows-release
```

## Architecture

C++ blackjack game with a `cardgames` static library, `blackjack` console executable, and optional `blackjack-qml` GUI.

**Core types** (`src/`):
- `Card` - Suit/Rank value type with comparison operators
- `Deck` - Card container with shuffle/deal operations
- `CardGames::BlackJack::Game` - State machine managing game flow via `GameNode` enum (Ready → PlayersRound → DealersRound → GameOver*)
- `GameState` - Immutable snapshot holding hands, deck, and current node
- `add_em_up(hand)` - Calculates blackjack hand value (Ace=11, face cards=10)

**QML UI** (`app/qml/`):
- `GameController` - Q_OBJECT wrapper exposing Game to QML
- `Main.qml` - Main window with dealer/player sections and action buttons
- `CardView.qml` - Single card display (rank, suit, face-down state)
- `HandView.qml` - Row of cards using Repeater

**Design patterns**:
- All core types use `static_assert(is_regular<T>)` to enforce regular type semantics (copyable, comparable)
- Game uses history-based state tracking (vector of GameState)
- Player actions: `Deal`, `Hit`, `Stay`
- Blackjack (21 on initial deal) immediately wins/loses - no PlayersRound
