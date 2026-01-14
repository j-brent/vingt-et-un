# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```sh
# Windows
cmake --preset windows
cmake --build --preset windows-release   # or windows-debug

# Linux
cmake --preset linux
cmake --build --preset linux-release     # or linux-debug
```

## Testing

Tests use Catch2 (fetched automatically via CMake).

```sh
# Run all tests
ctest --preset windows-release   # or linux-release

# Full workflow (configure + build + test)
cmake --workflow --preset windows-release
```

## Architecture

C++ blackjack game with a `cardgames` static library and `blackjack` executable.

**Core types** (`src/`):
- `Card` - Suit/Rank value type with comparison operators
- `Deck` - Card container with shuffle/deal operations
- `CardGames::BlackJack::Game` - State machine managing game flow via `GameNode` enum (Ready → PlayersRound → DealersRound → GameOver*)
- `GameState` - Immutable snapshot holding hands, deck, and current node

**Design patterns**:
- All core types use `static_assert(is_regular<T>)` to enforce regular type semantics (copyable, comparable)
- Game uses history-based state tracking (vector of GameState)
- Player actions: `Deal`, `Hit`, `Stay`
