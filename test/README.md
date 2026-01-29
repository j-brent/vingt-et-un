# Test Strategy

All tests use [Catch2](https://github.com/catchorg/Catch2) with BDD-style macros (`SCENARIO`/`GIVEN`/`WHEN`/`THEN`).

## Card

**File:** [card_tests.cpp](card_tests.cpp)

Tests verify that `Card` behaves as a regular value type with correct construction, comparison, and container interoperability.

| Area | Technique | Location |
|---|---|---|
| Dual constructor ordering `(Rank, Suit)` and `(Suit, Rank)` | Equivalence partitioning | [L15](card_tests.cpp#L15) |
| `operator==` — identical, same rank/different suit, same suit/different rank, fully different, reflexive, symmetric | Equivalence partitioning | [L55](card_tests.cpp#L55) |
| `operator<=>` — lower vs higher rank, same rank different suit, irreflexive, full 13-rank transitivity chain | Boundary analysis | [L128](card_tests.cpp#L128) |
| Boundary ranks (Two=2, Ace=14, Ten/Jack boundary, face card progression) | Boundary analysis | [L190](card_tests.cpp#L190) |
| `Card::suits()` — count, order, distinctness | Coverage | [L249](card_tests.cpp#L249) |
| Copy construction and copy assignment | Regular type semantics | [L284](card_tests.cpp#L284) |
| `std::ranges::sort`, `std::ranges::find`, `std::ranges::count` with Cards | Container interop | [L319](card_tests.cpp#L319) |
| `operator<=>` vs `operator==` consistency (same rank, different suit) | Error guessing | [L389](card_tests.cpp#L389) |
| All 13 rank enum underlying values | Exhaustive | [L417](card_tests.cpp#L417) |
| 13 ranks in one suit — distinctness and sort order | Exhaustive | [L441](card_tests.cpp#L441) |
| 4 suits with same rank — distinct under `==`, equivalent under `<` | Exhaustive | [L477](card_tests.cpp#L477) |
| Default construction, move construction, move assignment | Regular type semantics | [L516](card_tests.cpp#L516) |

## Deck

**File:** [deck_tests.cpp](deck_tests.cpp)

Tests verify deck construction, dealing mechanics, iteration, slicing utilities, and value semantics.

| Area | Technique | Location |
|---|---|---|
| Default deck — 52 cards, iterable, equality | Equivalence partitioning | [L4](deck_tests.cpp#L4) |
| Shuffle produces a different ordering but preserves content | Equivalence partitioning | [L4](deck_tests.cpp#L4), [L372](deck_tests.cpp#L372) |
| `slice_suits` / `slice_face_values` — normal, empty, single-card inputs | Boundary analysis | [L43](deck_tests.cpp#L43), [L401](deck_tests.cpp#L401), [L423](deck_tests.cpp#L423) |
| `deal()` single card — order preserved across 5 deals | Boundary analysis | [L71](deck_tests.cpp#L71) |
| `deal(n)` — deal 3, deal all, deal 0, deal more than available | Boundary analysis | [L108](deck_tests.cpp#L108) |
| Deck size decreases after `deal()` and `deal(n)` | Coverage | [L193](deck_tests.cpp#L193) |
| Vector constructor — empty and single-card decks | Boundary analysis | [L226](deck_tests.cpp#L226) |
| Copy construction and copy assignment | Regular type semantics | [L273](deck_tests.cpp#L273) |
| `operator!=` — identical vs different decks | Coverage | [L306](deck_tests.cpp#L306) |
| All 52 cards unique, 13 per suit | Exhaustive | [L338](deck_tests.cpp#L338) |
| Non-const `begin()`/`end()` iteration | Coverage | [L452](deck_tests.cpp#L452) |

## Game / GameState / GameNode

**File:** [game_tests.cpp](game_tests.cpp)

Tests verify the blackjack state machine transitions, hand scoring, and game outcome determination.

### `add_em_up` (hand scoring)

| Area | Technique | Location |
|---|---|---|
| Empty hand = 0 | Boundary analysis | [L91](game_tests.cpp#L91) |
| Single number cards (Two, Ten), multi-card sums (2-9) | Equivalence partitioning | [L91](game_tests.cpp#L91) |
| Face cards — Jack, Queen, King each = 10, combined = 30 | Equivalence partitioning | [L154](game_tests.cpp#L154) |
| Ace = 11, Ace+Ten = 21, Ace+King = 21, two Aces = 22 | Boundary analysis | [L215](game_tests.cpp#L215) |
| Boundary totals: exactly 21, 22 (bust), 20 (under) | Boundary analysis | [L437](game_tests.cpp#L437) |
| Every rank individually | Exhaustive | [L986](game_tests.cpp#L986) |
| Large hand (six low cards) | Error guessing | [L1013](game_tests.cpp#L1013) |

### `GameState`

| Area | Technique | Location |
|---|---|---|
| Default state — Ready, empty hands, full deck | Coverage | [L6](game_tests.cpp#L6) |
| Parameterized construction — fields set correctly | Coverage | [L479](game_tests.cpp#L479) |
| Equality — identical, differing node, differing hand | Equivalence partitioning | [L516](game_tests.cpp#L516) |

### `Game` (state machine)

| Area | Technique | Location |
|---|---|---|
| Deal from Ready — hands populated, deck reduced, correct node | Coverage | [L571](game_tests.cpp#L571) |
| Invalid plays ignored — Hit/Stay in Ready, Deal in PlayersRound, Deal in DealersRound | Error guessing | [L637](game_tests.cpp#L637), [L913](game_tests.cpp#L913) |
| Player Stay — transitions to DealersRound, hands/deck unchanged | Coverage | [L693](game_tests.cpp#L693) |
| Player Hit — hand grows, deck shrinks, bust detection | Coverage | [L732](game_tests.cpp#L732) |
| Dealer Hit — hand grows, deck shrinks, bust detection | Coverage | [L775](game_tests.cpp#L775) |
| Dealer Stay — final comparison (player wins, dealer wins, draw) | Equivalence partitioning | [L824](game_tests.cpp#L824) |
| GameOver states ignore all plays (Deal, Hit, Stay) | Error guessing | [L861](game_tests.cpp#L861) |
| `state()` consistency with `next()` return value | Coverage | [L938](game_tests.cpp#L938) |
| All GameNode enum values are distinct | Exhaustive | [L968](game_tests.cpp#L968) |
| Multiple consecutive hits in PlayersRound and DealersRound | Coverage | [L1035](game_tests.cpp#L1035), [L1069](game_tests.cpp#L1069) |
| Second Deal after first is ignored | Error guessing | [L1104](game_tests.cpp#L1104) |

## Streaming

**File:** [streaming_tests.cpp](streaming_tests.cpp)

Tests verify `operator<<` overloads for `Rank`, `Suit`, and `Card` produce correct string output.

| Area | Technique | Location |
|---|---|---|
| All 13 ranks — expected string for each | Exhaustive | [L14](streaming_tests.cpp#L14) |
| All 4 suits — expected string for each | Exhaustive | [L115](streaming_tests.cpp#L115) |
| Card formatting — number rank, face rank, Ace, Ten, boundary cards | Equivalence partitioning | [L153](streaming_tests.cpp#L153) |
| Stream chaining — two cards in sequence | Coverage | [L232](streaming_tests.cpp#L232) |
| Return type — `operator<<` returns `ostream&` for all three overloads | Coverage | [L252](streaming_tests.cpp#L252) |

## Compile-Time Type Checks

No runtime tests. Core type headers contain `static_assert(std::regular<T>)` declarations (using the C++20 `<concepts>` library) that verify regular type semantics for `Card`, `Deck`, and `GameState` at compile time. These assertions are validated every time the project builds successfully.
