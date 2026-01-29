#include <catch2/catch_test_macros.hpp>
#include <deck.h>

SCENARIO("Basic properties of a deck of cards")
{
	GIVEN("A default Deck")
	{
		const auto d = Deck{};

		THEN("The deck should have 52 cards")
		{
			CHECK(d.cards().size() == 52);
		}

		THEN("It should be possible to iterate through the deck")
		{
			int count = 0;
			for (const auto card : d)
				++count;

			CHECK(count == 52);
		}

		WHEN("Comparing two default-constructed decks")
		{
			THEN("They should be equal")
			{
				CHECK(d == Deck{});
			}
		}

		WHEN("Comparing a default-constructed deck and a shuffled deck")
		{
			THEN("They should NOT be equal")
			{
				const auto sd = shuffle(d);
				CHECK(d != sd);
			}
		}
	}
}

SCENARIO("Slicing ranks and suits")
{
	using Rank = Card::Rank;
	using Suit = Card::Suit;

	GIVEN("Some cards")
	{
		const auto cards = std::vector<Card>{
			{Rank::Ace, Suit::Clubs}, {Rank::King, Suit::Hearts}, {Rank::Queen, Suit::Diamonds}};

		WHEN("Slicing the suits")
		{
			THEN("We get the suits")
			{
				CHECK(slice_suits(cards) == std::vector<Suit>{Suit::Clubs, Suit::Hearts, Suit::Diamonds});
			}
		}

		WHEN("Slicing the face value")
		{
			THEN("We get the ranks")
			{
				CHECK(slice_face_values(cards) == std::vector<Rank>{Rank::Ace, Rank::King, Rank::Queen});
			}
		}
	}
}

SCENARIO("Dealing cards in order")
{
	using Rank = Card::Rank;
	using Suit = Card::Suit;

	GIVEN("Some cards in a specific order")
	{
		const std::vector<Card> cards = {
			{Rank::Ace, Suit::Clubs},	 {Rank::Ace, Suit::Hearts}, {Rank::Ace, Suit::Diamonds},
			{Rank::Ace, Suit::Spades}, {Rank::Two, Suit::Clubs},
		};
		auto deck = Deck{cards};

		WHEN("Dealing the cards off the top of the deck")
		{
			const auto c0 = deck.deal();
			const auto c1 = deck.deal();
			const auto c2 = deck.deal();
			const auto c3 = deck.deal();
			const auto c4 = deck.deal();

			THEN("The cards come out in the same order")
			{
				CHECK(c0 == cards.at(0));
				CHECK(c1 == cards.at(1));
				CHECK(c2 == cards.at(2));
				CHECK(c3 == cards.at(3));
				CHECK(c4 == cards.at(4));
			}
		}
	}
}

// ============================================================================
// Dealing multiple cards at once
// ============================================================================

SCENARIO("Dealing multiple cards at once")
{
	using Rank = Card::Rank;
	using Suit = Card::Suit;

	GIVEN("A deck with known cards")
	{
		const std::vector<Card> cards = {
			{Rank::Two, Suit::Clubs},		{Rank::Three, Suit::Hearts}, {Rank::Four, Suit::Diamonds},
			{Rank::Five, Suit::Spades}, {Rank::Six, Suit::Clubs},
		};
		auto deck = Deck{cards};

		WHEN("Dealing 3 cards at once")
		{
			const auto dealt = deck.deal(3);

			THEN("3 cards are returned in order from the top")
			{
				REQUIRE(dealt.size() == 3);
				CHECK(dealt[0] == cards[0]);
				CHECK(dealt[1] == cards[1]);
				CHECK(dealt[2] == cards[2]);
			}

			THEN("The deck has 2 cards remaining")
			{
				CHECK(deck.cards().size() == 2);
			}
		}

		WHEN("Dealing all 5 cards at once")
		{
			const auto dealt = deck.deal(5);

			THEN("All 5 cards are returned")
			{
				REQUIRE(dealt.size() == 5);
				for (size_t i = 0; i < 5; ++i) {
					CHECK(dealt[i] == cards[i]);
				}
			}

			THEN("The deck is empty")
			{
				CHECK(deck.cards().empty());
			}
		}

		WHEN("Dealing 0 cards")
		{
			const auto dealt = deck.deal(static_cast<size_t>(0));

			THEN("An empty vector is returned")
			{
				CHECK(dealt.empty());
			}

			THEN("The deck is unchanged")
			{
				CHECK(deck.cards().size() == 5);
			}
		}

		WHEN("Requesting more cards than the deck contains")
		{
			const auto dealt = deck.deal(10);

			THEN("An empty vector is returned (not enough cards)")
			{
				CHECK(dealt.empty());
			}

			THEN("The deck is unchanged")
			{
				CHECK(deck.cards().size() == 5);
			}
		}
	}
}

// ============================================================================
// Dealing reduces deck size
// ============================================================================

SCENARIO("Dealing reduces deck size correctly")
{
	GIVEN("A default 52-card deck")
	{
		auto deck = Deck{};
		CHECK(deck.cards().size() == 52);

		WHEN("Dealing 1 card")
		{
			deck.deal();

			THEN("The deck has 51 cards remaining")
			{
				CHECK(deck.cards().size() == 51);
			}
		}

		WHEN("Dealing 5 cards via deal(5)")
		{
			deck.deal(5);

			THEN("The deck has 47 cards remaining")
			{
				CHECK(deck.cards().size() == 47);
			}
		}
	}
}

// ============================================================================
// Deck constructed from a vector
// ============================================================================

SCENARIO("Deck constructed from a vector of cards")
{
	using Rank = Card::Rank;
	using Suit = Card::Suit;

	GIVEN("An empty vector")
	{
		auto deck = Deck{std::vector<Card>{}};

		THEN("The deck has 0 cards")
		{
			CHECK(deck.cards().empty());
		}
	}

	GIVEN("A single-card vector")
	{
		const auto cards = std::vector<Card>{{Rank::Ace, Suit::Spades}};
		auto deck = Deck{cards};

		THEN("The deck has 1 card")
		{
			CHECK(deck.cards().size() == 1);
			CHECK(deck.cards().front() == cards.front());
		}

		WHEN("Dealing the single card")
		{
			const auto dealt = deck.deal();

			THEN("The dealt card matches")
			{
				CHECK(dealt == cards.front());
			}

			THEN("The deck is now empty")
			{
				CHECK(deck.cards().empty());
			}
		}
	}
}

// ============================================================================
// Deck copy semantics
// ============================================================================

SCENARIO("Deck copy semantics")
{
	GIVEN("A default deck")
	{
		const auto original = Deck{};

		WHEN("Copy constructing")
		{
			const auto copy = original;

			THEN("The copy equals the original")
			{
				CHECK(copy == original);
			}
		}

		WHEN("Copy assigning")
		{
			auto target = Deck{std::vector<Card>{}};
			target = original;

			THEN("The target equals the original")
			{
				CHECK(target == original);
			}
		}
	}
}

// ============================================================================
// Deck inequality operator
// ============================================================================

SCENARIO("Deck inequality operator")
{
	using Rank = Card::Rank;
	using Suit = Card::Suit;

	GIVEN("Two identical decks")
	{
		const auto a = Deck{};
		const auto b = Deck{};

		THEN("They are NOT not-equal")
		{
			CHECK_FALSE(a != b);
		}
	}

	GIVEN("Two different decks")
	{
		const auto a = Deck{};
		const auto b = Deck{std::vector<Card>{{Rank::Ace, Suit::Spades}}};

		THEN("They are not-equal")
		{
			CHECK(a != b);
		}
	}
}

// ============================================================================
// Deck contains all 52 unique cards
// ============================================================================

SCENARIO("Default deck contains all 52 unique cards")
{
	GIVEN("A default deck")
	{
		const auto deck = Deck{};
		const auto& cards = deck.cards();

		THEN("All 52 cards are distinct")
		{
			for (size_t i = 0; i < cards.size(); ++i) {
				for (size_t j = i + 1; j < cards.size(); ++j) {
					CHECK_FALSE(cards[i] == cards[j]);
				}
			}
		}

		THEN("There are 13 cards per suit")
		{
			for (const auto& suit : Card::suits()) {
				int count = 0;
				for (const auto& card : cards) {
					if (card.suit == suit)
						++count;
				}
				CHECK(count == 13);
			}
		}
	}
}

// ============================================================================
// Shuffle preserves card count and content
// ============================================================================

SCENARIO("Shuffle preserves deck content")
{
	GIVEN("A default deck")
	{
		const auto original = Deck{};

		WHEN("Shuffling the deck")
		{
			const auto shuffled = shuffle(original);

			THEN("The shuffled deck still has 52 cards")
			{
				CHECK(shuffled.cards().size() == 52);
			}

			THEN("The original deck is unchanged (shuffle is non-mutating)")
			{
				CHECK(original.cards().size() == 52);
				// original should still be in sorted order
				CHECK(original == Deck{});
			}
		}
	}
}

// ============================================================================
// Slicing empty collections
// ============================================================================

SCENARIO("Slicing empty card collections")
{
	GIVEN("An empty vector of cards")
	{
		const auto cards = std::vector<Card>{};

		THEN("slice_suits returns empty")
		{
			CHECK(slice_suits(cards).empty());
		}

		THEN("slice_face_values returns empty")
		{
			CHECK(slice_face_values(cards).empty());
		}
	}
}

// ============================================================================
// Slicing single card
// ============================================================================

SCENARIO("Slicing a single card")
{
	using Rank = Card::Rank;
	using Suit = Card::Suit;

	GIVEN("A single card")
	{
		const auto cards = std::vector<Card>{{Rank::Seven, Suit::Diamonds}};

		THEN("slice_suits returns one suit")
		{
			const auto suits = slice_suits(cards);
			REQUIRE(suits.size() == 1);
			CHECK(suits[0] == Suit::Diamonds);
		}

		THEN("slice_face_values returns one rank")
		{
			const auto ranks = slice_face_values(cards);
			REQUIRE(ranks.size() == 1);
			CHECK(ranks[0] == Rank::Seven);
		}
	}
}

// ============================================================================
// Mutable iteration
// ============================================================================

SCENARIO("Mutable deck iteration")
{
	using Rank = Card::Rank;
	using Suit = Card::Suit;

	GIVEN("A small deck")
	{
		auto deck = Deck{std::vector<Card>{{Rank::Two, Suit::Clubs}, {Rank::Three, Suit::Clubs}}};

		WHEN("Iterating with non-const begin/end")
		{
			int count = 0;
			for (auto it = deck.begin(); it != deck.end(); ++it) {
				++count;
			}

			THEN("We can iterate over all cards")
			{
				CHECK(count == 2);
			}
		}
	}
}