# Test Strategy

All tests use [Catch2](https://github.com/catchorg/Catch2) with BDD-style macros (`SCENARIO`/`GIVEN`/`WHEN`/`THEN`).

## Card

**File:** [card_tests.cpp](card_tests.cpp)

Tests verify that `Card` behaves as a regular value type with correct construction, comparison, and container interoperability.

| Area | Technique | Location |
|---|---|---|
| Dual constructor ordering `(Rank, Suit)` and `(Suit, Rank)` | Equivalence partitioning | [L16](card_tests.cpp#L16) |
| `operator==` — identical, same rank/different suit, same suit/different rank, fully different, reflexive, symmetric | Equivalence partitioning | [L56](card_tests.cpp#L56) |
| `operator<` — lower vs higher rank, same rank different suit, irreflexive, full 13-rank transitivity chain | Boundary analysis | [L129](card_tests.cpp#L129) |
| Boundary ranks (Two=2, Ace=14, Ten/Jack boundary, face card progression) | Boundary analysis | [L193](card_tests.cpp#L193) |
| `Card::suits()` — count, order, distinctness | Coverage | [L252](card_tests.cpp#L252) |
| Copy construction and copy assignment | Regular type semantics | [L287](card_tests.cpp#L287) |
| `std::sort`, `std::find`, `std::count` with Cards | Container interop | [L322](card_tests.cpp#L322) |
| `operator<` vs `operator==` consistency (same rank, different suit) | Error guessing | [L397](card_tests.cpp#L397) |
| All 13 rank enum underlying values | Exhaustive | [L425](card_tests.cpp#L425) |
| 13 ranks in one suit — distinctness and sort order | Exhaustive | [L449](card_tests.cpp#L449) |
| 4 suits with same rank — distinct under `==`, equivalent under `<` | Exhaustive | [L494](card_tests.cpp#L494) |
| Default construction, move construction, move assignment | Regular type semantics | [L535](card_tests.cpp#L535) |

## Deck

**File:** [deck_tests.cpp](deck_tests.cpp)

Tests verify deck construction, dealing mechanics, iteration, slicing utilities, and value semantics.

| Area | Technique | Location |
|---|---|---|
| Default deck — 52 cards, iterable, equality | Equivalence partitioning | [L4](deck_tests.cpp#L4) |
| Shuffle produces a different ordering but preserves content | Equivalence partitioning | [L4](deck_tests.cpp#L4), [L380](deck_tests.cpp#L380) |
| `slice_suits` / `slice_face_values` — normal, empty, single-card inputs | Boundary analysis | [L43](deck_tests.cpp#L43), [L409](deck_tests.cpp#L409), [L431](deck_tests.cpp#L431) |
| `deal()` single card — order preserved across 5 deals | Boundary analysis | [L74](deck_tests.cpp#L74) |
| `deal(n)` — deal 3, deal all, deal 0, deal more than available | Boundary analysis | [L114](deck_tests.cpp#L114) |
| Deck size decreases after `deal()` and `deal(n)` | Coverage | [L202](deck_tests.cpp#L202) |
| Vector constructor — empty and single-card decks | Boundary analysis | [L235](deck_tests.cpp#L235) |
| Copy construction and copy assignment | Regular type semantics | [L282](deck_tests.cpp#L282) |
| `operator!=` — identical vs different decks | Coverage | [L315](deck_tests.cpp#L315) |
| All 52 cards unique, 13 per suit | Exhaustive | [L347](deck_tests.cpp#L347) |
| Non-const `begin()`/`end()` iteration | Coverage | [L460](deck_tests.cpp#L460) |

## Game / GameState / GameNode

**File:** [game_tests.cpp](game_tests.cpp)

Tests verify the blackjack state machine transitions, hand scoring, and game outcome determination.

### `add_em_up` (hand scoring)

| Area | Technique | Location |
|---|---|---|
| Empty hand = 0 | Boundary analysis | [L91](game_tests.cpp#L91) |
| Single number cards (Two, Ten), multi-card sums (2-9) | Equivalence partitioning | [L91](game_tests.cpp#L91) |
| Face cards — Jack, Queen, King each = 10, combined = 30 | Equivalence partitioning | [L163](game_tests.cpp#L163) |
| Ace = 11, Ace+Ten = 21, Ace+King = 21, two Aces = 22 | Boundary analysis | [L230](game_tests.cpp#L230) |
| Boundary totals: exactly 21, 22 (bust), 20 (under) | Boundary analysis | [L303](game_tests.cpp#L303) |
| Every rank individually | Exhaustive | [L872](game_tests.cpp#L872) |
| Large hand (six low cards) | Error guessing | [L899](game_tests.cpp#L899) |

### `GameState`

| Area | Technique | Location |
|---|---|---|
| Default state — Ready, empty hands, full deck | Coverage | [L6](game_tests.cpp#L6) |
| Parameterized construction — fields set correctly | Coverage | [L354](game_tests.cpp#L354) |
| Equality — identical, differing node, differing hand | Equivalence partitioning | [L394](game_tests.cpp#L394) |

### `Game` (state machine)

| Area | Technique | Location |
|---|---|---|
| Deal from Ready — hands populated, deck reduced, correct node | Coverage | [L447](game_tests.cpp#L447) |
| Invalid plays ignored — Hit/Stay in Ready, Deal in PlayersRound, Deal in DealersRound | Error guessing | [L514](game_tests.cpp#L514), [L796](game_tests.cpp#L796) |
| Player Stay — transitions to DealersRound, hands/deck unchanged | Coverage | [L570](game_tests.cpp#L570) |
| Player Hit — hand grows, deck shrinks, bust detection | Coverage | [L610](game_tests.cpp#L610) |
| Dealer Hit — hand grows, deck shrinks, bust detection | Coverage | [L653](game_tests.cpp#L653) |
| Dealer Stay — final comparison (player wins, dealer wins, draw) | Equivalence partitioning | [L698](game_tests.cpp#L698) |
| GameOver states ignore all plays (Deal, Hit, Stay) | Error guessing | [L741](game_tests.cpp#L741) |
| `state()` consistency with `next()` return value | Coverage | [L825](game_tests.cpp#L825) |
| All GameNode enum values are distinct | Exhaustive | [L854](game_tests.cpp#L854) |
| Multiple consecutive hits in PlayersRound and DealersRound | Coverage | [L926](game_tests.cpp#L926), [L960](game_tests.cpp#L960) |
| Second Deal after first is ignored | Error guessing | [L993](game_tests.cpp#L993) |

## Streaming

**File:** [streaming_tests.cpp](streaming_tests.cpp)

Tests verify `operator<<` overloads for `Rank`, `Suit`, and `Card` produce correct string output.

| Area | Technique | Location |
|---|---|---|
| All 13 ranks — expected string for each | Exhaustive | [L15](streaming_tests.cpp#L15) |
| All 4 suits — expected string for each | Exhaustive | [L116](streaming_tests.cpp#L116) |
| Card formatting — number rank, face rank, Ace, Ten, boundary cards | Equivalence partitioning | [L154](streaming_tests.cpp#L154) |
| Stream chaining — two cards in sequence | Coverage | [L233](streaming_tests.cpp#L233) |
| Return type — `operator<<` returns `ostream&` for all three overloads | Coverage | [L253](streaming_tests.cpp#L253) |

## CompileTimeChecks

**File:** `src/CompileTimeChecks.h`

No runtime tests. This header contains `static_assert` declarations that verify regular type properties (`is_regular`) for core types at compile time. These assertions are validated every time the project builds successfully.
