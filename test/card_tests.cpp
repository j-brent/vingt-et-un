#include <algorithm>
#include <card.h>
#include <catch2/catch_test_macros.hpp>
#include <set>
#include <sstream>
#include <vector>

using Rank = Card::Rank;
using Suit = Card::Suit;

// ============================================================================
// Construction
// ============================================================================

SCENARIO("Card construction")
{
	GIVEN("A Card constructed with (Rank, Suit)")
	{
		const auto card = Card{Rank::Ace, Suit::Spades};

		THEN("The rank and suit are set correctly")
		{
			CHECK(card.rank() == Rank::Ace);
			CHECK(card.suit() == Suit::Spades);
		}
	}

	GIVEN("A Card constructed with (Suit, Rank) -- reversed order")
	{
		const auto card = Card{Suit::Hearts, Rank::King};

		THEN("The rank and suit are set correctly")
		{
			CHECK(card.rank() == Rank::King);
			CHECK(card.suit() == Suit::Hearts);
		}
	}

	GIVEN("Both constructor orderings with the same values")
	{
		const auto card_a = Card{Rank::Ten, Suit::Diamonds};
		const auto card_b = Card{Suit::Diamonds, Rank::Ten};

		THEN("The two cards are equal")
		{
			CHECK(card_a == card_b);
		}
	}
}

// ============================================================================
// Equality (operator==)
// ============================================================================

SCENARIO("Card equality")
{
	GIVEN("Two identical cards")
	{
		const auto a = Card{Rank::Queen, Suit::Hearts};
		const auto b = Card{Rank::Queen, Suit::Hearts};

		THEN("They are equal")
		{
			CHECK(a == b);
		}
	}

	GIVEN("Two cards with same rank but different suit")
	{
		const auto a = Card{Rank::Ace, Suit::Clubs};
		const auto b = Card{Rank::Ace, Suit::Spades};

		THEN("They are NOT equal")
		{
			CHECK_FALSE(a == b);
		}
	}

	GIVEN("Two cards with same suit but different rank")
	{
		const auto a = Card{Rank::Two, Suit::Diamonds};
		const auto b = Card{Rank::Three, Suit::Diamonds};

		THEN("They are NOT equal")
		{
			CHECK_FALSE(a == b);
		}
	}

	GIVEN("Two cards with different rank AND different suit")
	{
		const auto a = Card{Rank::Jack, Suit::Clubs};
		const auto b = Card{Rank::Five, Suit::Hearts};

		THEN("They are NOT equal")
		{
			CHECK_FALSE(a == b);
		}
	}

	GIVEN("A card compared to itself (reflexive)")
	{
		const auto card = Card{Rank::Seven, Suit::Spades};

		THEN("It is equal to itself")
		{
			CHECK(card == card);
		}
	}

	GIVEN("Equality is symmetric")
	{
		const auto a = Card{Rank::Nine, Suit::Hearts};
		const auto b = Card{Rank::Nine, Suit::Hearts};

		THEN("a == b implies b == a")
		{
			CHECK(a == b);
			CHECK(b == a);
		}
	}
}

// ============================================================================
// Ordering (operator<)
// ============================================================================

SCENARIO("Card ordering")
{
	GIVEN("A lower-rank card and a higher-rank card")
	{
		const auto two = Card{Rank::Two, Suit::Clubs};
		const auto ace = Card{Rank::Ace, Suit::Clubs};

		THEN("The lower rank is less than the higher rank")
		{
			CHECK(two < ace);
		}

		THEN("The higher rank is NOT less than the lower rank")
		{
			CHECK_FALSE(ace < two);
		}
	}

	GIVEN("Two cards with the same rank but different suits")
	{
		const auto a = Card{Rank::King, Suit::Clubs};
		const auto b = Card{Rank::King, Suit::Spades};

		THEN("Neither is less than the other (operator< ignores suit)")
		{
			CHECK_FALSE(a < b);
			CHECK_FALSE(b < a);
		}
	}

	GIVEN("A card compared to itself")
	{
		const auto card = Card{Rank::Five, Suit::Diamonds};

		THEN("It is NOT less than itself (irreflexive)")
		{
			CHECK_FALSE(card < card);
		}
	}

	GIVEN("All 13 ranks in order")
	{
		const auto ranks = std::vector<Rank>{
			Rank::Two,	Rank::Three, Rank::Four, Rank::Five,	Rank::Six,	Rank::Seven, Rank::Eight,
			Rank::Nine, Rank::Ten,	 Rank::Jack, Rank::Queen, Rank::King, Rank::Ace};

		THEN("Each rank is less than the next (transitivity chain)")
		{
			for (size_t i = 0; i + 1 < ranks.size(); ++i) {
				const auto lower = Card{ranks[i], Suit::Hearts};
				const auto higher = Card{ranks[i + 1], Suit::Hearts};
				CHECK(lower < higher);
				CHECK_FALSE(higher < lower);
			}
		}
	}
}

// ============================================================================
// Boundary ranks
// ============================================================================

SCENARIO("Boundary rank values")
{
	GIVEN("The lowest rank (Two)")
	{
		const auto two = Card{Rank::Two, Suit::Clubs};

		THEN("Its underlying value is 2")
		{
			CHECK(static_cast<int>(two.rank()) == 2);
		}
	}

	GIVEN("The highest rank (Ace)")
	{
		const auto ace = Card{Rank::Ace, Suit::Clubs};

		THEN("Its underlying value is 14")
		{
			CHECK(static_cast<int>(ace.rank()) == 14);
		}
	}

	GIVEN("The boundary between number cards and face cards")
	{
		const auto ten = Card{Rank::Ten, Suit::Clubs};
		const auto jack = Card{Rank::Jack, Suit::Clubs};

		THEN("Ten is less than Jack")
		{
			CHECK(ten < jack);
		}

		THEN("Their underlying values are consecutive")
		{
			CHECK(static_cast<int>(Rank::Ten) == 10);
			CHECK(static_cast<int>(Rank::Jack) == 11);
		}
	}

	GIVEN("Face card progression")
	{
		THEN("Jack < Queen < King < Ace")
		{
			const auto j = Card{Rank::Jack, Suit::Spades};
			const auto q = Card{Rank::Queen, Suit::Spades};
			const auto k = Card{Rank::King, Suit::Spades};
			const auto a = Card{Rank::Ace, Suit::Spades};

			CHECK(j < q);
			CHECK(q < k);
			CHECK(k < a);
		}
	}
}

// ============================================================================
// Suit enumeration
// ============================================================================

SCENARIO("Card::suits() static method")
{
	GIVEN("The result of Card::suits()")
	{
		const auto suits = Card::suits();

		THEN("It returns exactly 4 suits")
		{
			CHECK(suits.size() == 4);
		}

		THEN("It contains all four suits in order: Clubs, Diamonds, Hearts, Spades")
		{
			REQUIRE(suits.size() == 4);
			CHECK(suits[0] == Suit::Clubs);
			CHECK(suits[1] == Suit::Diamonds);
			CHECK(suits[2] == Suit::Hearts);
			CHECK(suits[3] == Suit::Spades);
		}

		THEN("All suits are distinct")
		{
			for (size_t i = 0; i < suits.size(); ++i) {
				for (size_t j = i + 1; j < suits.size(); ++j) {
					CHECK_FALSE(suits[i] == suits[j]);
				}
			}
		}
	}
}

// ============================================================================
// Rank enumeration
// ============================================================================

SCENARIO("Card::ranks() static method")
{
	GIVEN("The result of Card::ranks()")
	{
		const auto ranks = Card::ranks();

		THEN("It returns exactly 13 ranks")
		{
			CHECK(ranks.size() == 13);
		}

		THEN("It contains all thirteen ranks in order: Two through Ace")
		{
			REQUIRE(ranks.size() == 13);
			CHECK(ranks[0] == Rank::Two);
			CHECK(ranks[1] == Rank::Three);
			CHECK(ranks[2] == Rank::Four);
			CHECK(ranks[3] == Rank::Five);
			CHECK(ranks[4] == Rank::Six);
			CHECK(ranks[5] == Rank::Seven);
			CHECK(ranks[6] == Rank::Eight);
			CHECK(ranks[7] == Rank::Nine);
			CHECK(ranks[8] == Rank::Ten);
			CHECK(ranks[9] == Rank::Jack);
			CHECK(ranks[10] == Rank::Queen);
			CHECK(ranks[11] == Rank::King);
			CHECK(ranks[12] == Rank::Ace);
		}

		THEN("All ranks are distinct")
		{
			for (size_t i = 0; i < ranks.size(); ++i) {
				for (size_t j = i + 1; j < ranks.size(); ++j) {
					CHECK_FALSE(ranks[i] == ranks[j]);
				}
			}
		}
	}
}

// ============================================================================
// Copy semantics (regular type)
// ============================================================================

SCENARIO("Card copy semantics")
{
	GIVEN("A Card")
	{
		const auto original = Card{Rank::Eight, Suit::Diamonds};

		WHEN("Copy constructing a new card")
		{
			const auto copy = original;

			THEN("The copy equals the original")
			{
				CHECK(copy == original);
				CHECK(copy.rank() == Rank::Eight);
				CHECK(copy.suit() == Suit::Diamonds);
			}
		}

		WHEN("Copy assigning to another card")
		{
			auto target = Card{Rank::Two, Suit::Clubs};
			target = original;

			THEN("The target equals the original")
			{
				CHECK(target == original);
			}
		}
	}
}

// ============================================================================
// Use in standard containers
// ============================================================================

SCENARIO("Card usability with standard algorithms")
{
	GIVEN("A vector of cards in descending rank order")
	{
		auto cards = std::vector<Card>{{Rank::Ace, Suit::Spades},
																	 {Rank::King, Suit::Hearts},
																	 {Rank::Queen, Suit::Diamonds},
																	 {Rank::Jack, Suit::Clubs},
																	 {Rank::Two, Suit::Hearts}};

		WHEN("Sorting with std::sort using operator<")
		{
			std::ranges::sort(cards);

			THEN("Cards are in ascending rank order")
			{
				CHECK(cards[0].rank() == Rank::Two);
				CHECK(cards[1].rank() == Rank::Jack);
				CHECK(cards[2].rank() == Rank::Queen);
				CHECK(cards[3].rank() == Rank::King);
				CHECK(cards[4].rank() == Rank::Ace);
			}
		}
	}

	GIVEN("A vector of cards")
	{
		const auto cards = std::vector<Card>{
			{Rank::Five, Suit::Clubs}, {Rank::Ten, Suit::Hearts}, {Rank::Five, Suit::Diamonds}};

		WHEN("Using std::find to locate a specific card")
		{
			const auto target = Card{Rank::Ten, Suit::Hearts};
			auto it = std::ranges::find(cards, target);

			THEN("The card is found")
			{
				REQUIRE(it != cards.end());
				CHECK(*it == target);
			}
		}

		WHEN("Using std::find for a card not in the vector")
		{
			const auto missing = Card{Rank::Ace, Suit::Spades};
			auto it = std::ranges::find(cards, missing);

			THEN("The card is not found")
			{
				CHECK(it == cards.end());
			}
		}

		WHEN("Using std::count to count cards of a specific rank and suit")
		{
			const auto target = Card{Rank::Five, Suit::Clubs};
			auto count = std::ranges::count(cards, target);

			THEN("Exactly one match is found (equality uses rank AND suit)")
			{
				CHECK(count == 1);
			}
		}
	}
}

// ============================================================================
// operator< vs operator== consistency check
// ============================================================================

SCENARIO("Operator< and operator== consistency")
{
	// operator< compares only rank; operator== compares rank AND suit.
	// This means two cards can be "equivalent" under < but not == under ==.
	// This test documents this deliberate design choice.

	GIVEN("Two cards with same rank, different suit")
	{
		const auto a = Card{Rank::Seven, Suit::Clubs};
		const auto b = Card{Rank::Seven, Suit::Hearts};

		THEN("They are equivalent under operator< (neither is less)")
		{
			CHECK_FALSE(a < b);
			CHECK_FALSE(b < a);
		}

		THEN("But they are NOT equal under operator==")
		{
			CHECK_FALSE(a == b);
		}
	}
}

// ============================================================================
// Rank enum underlying values
// ============================================================================

SCENARIO("Rank enum underlying values are correct")
{
	THEN("Each rank has the expected integer value")
	{
		CHECK(static_cast<int>(Rank::Two) == 2);
		CHECK(static_cast<int>(Rank::Three) == 3);
		CHECK(static_cast<int>(Rank::Four) == 4);
		CHECK(static_cast<int>(Rank::Five) == 5);
		CHECK(static_cast<int>(Rank::Six) == 6);
		CHECK(static_cast<int>(Rank::Seven) == 7);
		CHECK(static_cast<int>(Rank::Eight) == 8);
		CHECK(static_cast<int>(Rank::Nine) == 9);
		CHECK(static_cast<int>(Rank::Ten) == 10);
		CHECK(static_cast<int>(Rank::Jack) == 11);
		CHECK(static_cast<int>(Rank::Queen) == 12);
		CHECK(static_cast<int>(Rank::King) == 13);
		CHECK(static_cast<int>(Rank::Ace) == 14);
	}
}

// ============================================================================
// Every suit-rank combination (exhaustive for a single suit)
// ============================================================================

SCENARIO("All 13 ranks can be paired with any suit")
{
	GIVEN("All 13 ranks paired with Clubs")
	{
		const auto cards = std::vector<Card>{
			{Rank::Two, Suit::Clubs},		{Rank::Three, Suit::Clubs}, {Rank::Four, Suit::Clubs},
			{Rank::Five, Suit::Clubs},	{Rank::Six, Suit::Clubs},		{Rank::Seven, Suit::Clubs},
			{Rank::Eight, Suit::Clubs}, {Rank::Nine, Suit::Clubs},	{Rank::Ten, Suit::Clubs},
			{Rank::Jack, Suit::Clubs},	{Rank::Queen, Suit::Clubs}, {Rank::King, Suit::Clubs},
			{Rank::Ace, Suit::Clubs}};

		THEN("All 13 cards are distinct")
		{
			for (size_t i = 0; i < cards.size(); ++i) {
				for (size_t j = i + 1; j < cards.size(); ++j) {
					CHECK_FALSE(cards[i] == cards[j]);
				}
			}
		}

		THEN("They sort into ascending order by rank")
		{
			auto sorted = cards;
			std::ranges::sort(sorted);

			for (size_t i = 0; i + 1 < sorted.size(); ++i) {
				CHECK(sorted[i] < sorted[i + 1]);
			}
		}
	}
}

// ============================================================================
// Four suits, same rank -- all distinct under ==
// ============================================================================

SCENARIO("Four cards of the same rank in different suits")
{
	GIVEN("Four Aces, one per suit")
	{
		const auto cards = std::vector<Card>{{Rank::Ace, Suit::Clubs},
																				 {Rank::Ace, Suit::Diamonds},
																				 {Rank::Ace, Suit::Hearts},
																				 {Rank::Ace, Suit::Spades}};

		THEN("All four are distinct under operator==")
		{
			for (size_t i = 0; i < cards.size(); ++i) {
				for (size_t j = i + 1; j < cards.size(); ++j) {
					CHECK_FALSE(cards[i] == cards[j]);
				}
			}
		}

		THEN("None is less than any other under operator< (same rank)")
		{
			for (size_t i = 0; i < cards.size(); ++i) {
				for (size_t j = 0; j < cards.size(); ++j) {
					if (i != j) {
						CHECK_FALSE(cards[i] < cards[j]);
					}
				}
			}
		}
	}
}

// ============================================================================
// static_assert coverage -- compile-time check
// ============================================================================

// This test exists to document that Card satisfies regular type constraints.
// The static_assert in card.h covers this at compile time; this test verifies
// the runtime behavior that supports it.

SCENARIO("Card satisfies regular type properties at runtime")
{
	GIVEN("Default-constructed Cards")
	{
		const auto a = Card{};
		const auto b = Card{};

		THEN("Two default-constructed Cards are equal")
		{
			CHECK(a == b);
		}
	}

	GIVEN("A Card value")
	{
		auto original = Card{Rank::Six, Suit::Hearts};

		WHEN("Move constructing")
		{
			auto moved = std::move(original);

			THEN("The moved-to card has the expected values")
			{
				CHECK(moved.rank() == Rank::Six);
				CHECK(moved.suit() == Suit::Hearts);
			}
		}

		WHEN("Move assigning")
		{
			auto target = Card{};
			target = std::move(original);

			THEN("The target has the expected values")
			{
				CHECK(target.rank() == Rank::Six);
				CHECK(target.suit() == Suit::Hearts);
			}
		}
	}
}
