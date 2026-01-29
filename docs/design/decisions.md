# Design Decisions

This document captures design decisions evident in the codebase.

## Regular Types Enforcement

**Location:** `src/card.h`, `src/deck.h`, `src/blackjack-game.h`

**Decision:** All core value types (`Card`, `Deck`, `GameState`) use `static_assert(std::regular<T>)` (C++20 `<concepts>`) to enforce regular type semantics at compile time.

**What is a Regular Type:** A type that is:
- Default constructible
- Copy constructible
- Move constructible
- Copy assignable
- Move assignable
- Equality comparable

**Rationale:** Based on Eric Niebler's "C++11 Library Design" talk. Regular types behave predictably and can be used in standard containers and algorithms without surprises.

**Source:** https://youtu.be/zgOF4NrQllo?t=37m15s

## Immutable State with History

**Location:** `src/blackjack-game.h:166-193`, `src/blackjack-game.h:234`

**Decision:** `GameState` is immutable. The `Game` class maintains a `vector<GameState>` history, appending new states rather than mutating.

**Evidence:**
- `GameState` has no mutator methods (only const getters)
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
- Direct comparison via `operator<=>` (rank-only ordering)
- Conversion to blackjack values in `add_em_up()` via `static_cast<int>(card.rank)` for 2-10

## Card Ordering: Rank Only

**Location:** `src/card.h` — `operator<=>` and `operator==`

**Decision:** Card ordering (`<`, `<=`, `>`, `>=`) compares by rank only. Suit is ignored for ordering purposes. Card equality (`==`, `!=`) compares both rank and suit.

**Evidence:**
- `operator<=>(const Card& other)` compares `static_cast<int>(rank) <=> static_cast<int>(other.rank)`
- `operator==(const Card&) const = default` compares all members (suit and rank)

**Rationale:** In standard card games, when comparing which card is higher, the suit is not taken into consideration. Only rank determines relative value. This means two cards of the same rank but different suits are equivalent under ordering but not equal — consistent with how card games treat them.

**Note:** This means `Card` satisfies `std::regular` but not `std::totally_ordered`, since `(a <=> b) == 0` does not imply `a == b`.

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

## Soft Ace Handling

**Location:** `src/blackjack-game.h:13-17`, `src/blackjack-game.cpp:9-39`

**Decision:** Aces are initially counted as 11 but automatically convert to 1 when the hand would bust.

**Evidence:**
- `HandValue` struct tracks `total`, `is_soft`, and `soft_ace_count`
- `calculate_hand_value()` first counts all aces as 11, then reduces them to 1 while total > 21
- `DealersHand::is_soft()` and `HandValue::is_soft` expose soft status

**Algorithm:**
1. Sum all cards, counting Aces as 11
2. While total > 21 and soft_ace_count > 0:
   - Subtract 10 (convert one ace from 11 to 1)
   - Decrement soft_ace_count
3. Return total with `is_soft = (soft_ace_count > 0)`

**Rationale:** Standard blackjack rules. Soft hands affect dealer play (hit on soft 17) and player strategy.

## Platform-Specific Unicode Output

**Location:** `src/streaming.cpp:131-151`, `src/streaming.cpp:153-188`

**Decision:** Windows-specific code for Unicode card display using `_setmode()` and `_O_U16TEXT`.

**Evidence:** `#ifdef _WIN32` guards around Unicode output code.

**Rationale:** Windows console requires mode switching for Unicode output. Non-Windows platforms use standard wide character output.

## Split Tracking in PlayersHand

**Location:** `src/blackjack-game.h:51-152`

**Decision:** Split state is managed entirely within `PlayersHand` class rather than in `GameState` or `Game`.

**Evidence:**
- `PlayersHand` contains `vector<SingleHand> m_hands` and `m_active_index`
- `SingleHand` struct tracks per-hand state: `is_from_split`, `is_from_split_aces`, `is_complete`, `split_count`
- Split operations (`can_split()`, `split()`) are PlayersHand methods

**Rationale:** Encapsulation - all split logic is contained in the hand container. GameState and Game remain simple, delegating split complexity to PlayersHand.

## Split Aces Auto-Complete

**Location:** `src/blackjack-game.h:99-100`, `src/blackjack-game.h:106-107`

**Decision:** When aces are split, both resulting hands are immediately marked complete after receiving one card each.

**Evidence:** In `PlayersHand::split()`:
- `is_from_split_aces = is_aces;`
- `is_complete = is_aces;` (for both hands)

**Rationale:** Standard casino rule - split aces receive only one card and cannot be hit.

## Resplit Limit

**Location:** `src/blackjack-game.h:59`, `src/blackjack-game.h:87`

**Decision:** Maximum of 3 splits allowed (4 total hands).

**Evidence:** `can_split()` checks `hand.split_count < 3`

**Rationale:** Standard casino limit prevents unlimited splitting.

## Configurable Game Rules

**Location:** `src/blackjack-game.h:206-211`

**Decision:** Game rules are configurable via `BlackjackConfig` struct passed to `Game` constructor.

**Options:**
- `hit_soft_17` (default: true) - Dealer hits on soft 17
- `allow_resplit_aces` (default: false) - Can split aces multiple times
- `initial_deck` (default: nullopt) - Deterministic deck for testing

**Rationale:** Supports different casino rule variations and enables deterministic testing.

## Dealer Auto-Play

**Location:** `src/blackjack-game.cpp:204-239`

**Decision:** Dealer's turn plays automatically without user input.

**Evidence:** `play_dealer_turn()` is a private method called internally when player stays. It loops until dealer stands or busts.

**Rationale:** Dealer has no decisions to make - must follow fixed rules (hit on <17, hit on soft 17 if configured). Auto-play simplifies the state machine.

## QML/C++ Separation

**Location:** `app/qml/GameController.h`, `app/qml/GameController.cpp`

**Decision:** Game logic remains in pure C++ (`cardgames` library). QML accesses it through a thin `GameController` Q_OBJECT wrapper.

**Evidence:**
- `GameController` contains `CardGames::BlackJack::Game m_game`
- Properties expose game state as QML-friendly types (QString, QVariantList, int, bool)
- Slots delegate to `m_game.next()`

**Rationale:** Clean separation between UI and logic. Core game logic can be tested without Qt. Multiple UIs (console, QML) share the same library.
