# Design Decisions

This document captures design decisions evident in the codebase.

## Regular Types Enforcement

**Location:** `src/CompileTimeChecks.h:24-33`, used in `src/card.h:57`, `src/deck.h:94`, `src/blackjack-game.h:60-61`

**Decision:** All core value types (`Card`, `Deck`, `GameState`) use `static_assert(is_regular<T>::value)` to enforce regular type semantics at compile time.

**What is a Regular Type:** A type that is:
- Default constructible
- Copy constructible
- Move constructible
- Copy assignable
- Move assignable
- Equality comparable

**Rationale:** Based on Eric Niebler's "C++11 Library Design" talk (referenced in `CompileTimeChecks.h:5-7`). Regular types behave predictably and can be used in standard containers and algorithms without surprises.

**Source:** https://youtu.be/zgOF4NrQllo?t=37m15s

## Immutable State with History

**Location:** `src/blackjack-game.h:71`, `src/blackjack-game.cpp:27-106`

**Decision:** `GameState` is immutable. The `Game` class maintains a `vector<GameState>` history, appending new states rather than mutating.

**Evidence:**
- `GameState` has no mutator methods
- `Game::next()` creates new `GameState` objects via `history.emplace_back()`
- Current state accessed via `history.back()`

**Rationale:** Implementation detail - rationale not documented in code. Likely benefits include:
- Undo/replay capability (history preserved)
- Thread safety (immutable states)
- Debugging (full game history available)

## Card Rank Values

**Location:** `src/card.h:11-25`

**Decision:** Rank enum values are assigned explicit integers (Two=2 through Ace=14).

**Rationale:** Enables:
- Direct comparison via `operator<` (`src/card.h:47-50`)
- Conversion to blackjack values in `add_em_up()` via `static_cast<int>(card.rank)` for 2-10

## Nested Enums in Card

**Location:** `src/card.h:7-45`

**Decision:** `Suit` and `Rank` enums are nested inside the `Card` struct.

**Rationale:** Implementation detail - rationale not documented. Scoping enums to their owning type improves namespace organization.

## Free Function shuffle()

**Location:** `src/deck.h:96`, `src/deck.cpp:26-34`

**Decision:** `shuffle` is a free function that returns a new `Deck` rather than a member function that mutates in place.

**Evidence:** `Deck shuffle(const Deck& deck)` takes const reference and returns new Deck.

**Rationale:** Consistent with immutability pattern. Original deck is preserved.

## Deck Deals from Front

**Location:** `src/deck.cpp:13-24`

**Decision:** Cards are dealt from the front of the internal vector, not the back.

**Evidence:** `deal()` uses iterators from `begin()` and erases dealt cards from front.

**Rationale:** Implementation detail - rationale not documented. Front-dealing matches physical card dealing (top of deck).

## Simplified Ace Handling

**Location:** `src/blackjack-game.cpp:15`

**Decision:** Aces are always valued at 11.

**Evidence:** `case Card::Rank::Ace: return 11;` with no conditional logic.

**Limitation:** Standard blackjack allows aces to count as 1 when 11 would cause a bust ("soft" hands). This is not implemented.

## Platform-Specific Unicode Output

**Location:** `src/streaming.cpp:131-151`, `src/streaming.cpp:153-188`

**Decision:** Windows-specific code for Unicode card display using `_setmode()` and `_O_U16TEXT`.

**Evidence:** `#ifdef _WIN32` guards around Unicode output code.

**Rationale:** Windows console requires mode switching for Unicode output. Non-Windows platforms use standard wide character output.
