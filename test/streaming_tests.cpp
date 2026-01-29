#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <streaming.h>
#include <string>
#include <vector>

using Rank = Card::Rank;
using Suit = Card::Suit;

// ============================================================================
// operator<< for Card::Rank (ostream)
// ============================================================================

SCENARIO("Streaming Rank to ostream")
{
	GIVEN("Each rank value")
	{
		THEN("Two streams as '2'")
		{
			std::ostringstream os;
			os << Rank::Two;
			CHECK(os.str() == "2");
		}

		THEN("Three streams as '3'")
		{
			std::ostringstream os;
			os << Rank::Three;
			CHECK(os.str() == "3");
		}

		THEN("Four streams as '4'")
		{
			std::ostringstream os;
			os << Rank::Four;
			CHECK(os.str() == "4");
		}

		THEN("Five streams as '5'")
		{
			std::ostringstream os;
			os << Rank::Five;
			CHECK(os.str() == "5");
		}

		THEN("Six streams as '6'")
		{
			std::ostringstream os;
			os << Rank::Six;
			CHECK(os.str() == "6");
		}

		THEN("Seven streams as '7'")
		{
			std::ostringstream os;
			os << Rank::Seven;
			CHECK(os.str() == "7");
		}

		THEN("Eight streams as '8'")
		{
			std::ostringstream os;
			os << Rank::Eight;
			CHECK(os.str() == "8");
		}

		THEN("Nine streams as '9'")
		{
			std::ostringstream os;
			os << Rank::Nine;
			CHECK(os.str() == "9");
		}

		THEN("Ten streams as '10'")
		{
			std::ostringstream os;
			os << Rank::Ten;
			CHECK(os.str() == "10");
		}

		THEN("Jack streams as 'Jack'")
		{
			std::ostringstream os;
			os << Rank::Jack;
			CHECK(os.str() == "Jack");
		}

		THEN("Queen streams as 'Queen'")
		{
			std::ostringstream os;
			os << Rank::Queen;
			CHECK(os.str() == "Queen");
		}

		THEN("King streams as 'King'")
		{
			std::ostringstream os;
			os << Rank::King;
			CHECK(os.str() == "King");
		}

		THEN("Ace streams as 'Ace'")
		{
			std::ostringstream os;
			os << Rank::Ace;
			CHECK(os.str() == "Ace");
		}
	}
}

// ============================================================================
// operator<< for Card::Suit (ostream)
// ============================================================================

SCENARIO("Streaming Suit to ostream")
{
	GIVEN("Each suit value")
	{
		THEN("Clubs streams as 'Clubs'")
		{
			std::ostringstream os;
			os << Suit::Clubs;
			CHECK(os.str() == "Clubs");
		}

		THEN("Diamonds streams as 'Diamonds'")
		{
			std::ostringstream os;
			os << Suit::Diamonds;
			CHECK(os.str() == "Diamonds");
		}

		THEN("Hearts streams as 'Hearts'")
		{
			std::ostringstream os;
			os << Suit::Hearts;
			CHECK(os.str() == "Hearts");
		}

		THEN("Spades streams as 'Spades'")
		{
			std::ostringstream os;
			os << Suit::Spades;
			CHECK(os.str() == "Spades");
		}
	}
}

// ============================================================================
// operator<< for Card (ostream)
// ============================================================================

SCENARIO("Streaming a Card to ostream")
{
	GIVEN("A Card with a number rank")
	{
		const auto card = Card{Rank::Five, Suit::Hearts};

		THEN("It streams as 'Rank of Suit'")
		{
			std::ostringstream os;
			os << card;
			CHECK(os.str() == "5 of Hearts");
		}
	}

	GIVEN("A Card with a face rank")
	{
		const auto card = Card{Rank::King, Suit::Spades};

		THEN("It streams as 'King of Spades'")
		{
			std::ostringstream os;
			os << card;
			CHECK(os.str() == "King of Spades");
		}
	}

	GIVEN("A Card with Ace")
	{
		const auto card = Card{Rank::Ace, Suit::Clubs};

		THEN("It streams as 'Ace of Clubs'")
		{
			std::ostringstream os;
			os << card;
			CHECK(os.str() == "Ace of Clubs");
		}
	}

	GIVEN("A Card with Ten")
	{
		const auto card = Card{Rank::Ten, Suit::Diamonds};

		THEN("It streams as '10 of Diamonds'")
		{
			std::ostringstream os;
			os << card;
			CHECK(os.str() == "10 of Diamonds");
		}
	}

	GIVEN("The lowest card: Two of Clubs")
	{
		const auto card = Card{Rank::Two, Suit::Clubs};

		THEN("It streams as '2 of Clubs'")
		{
			std::ostringstream os;
			os << card;
			CHECK(os.str() == "2 of Clubs");
		}
	}

	GIVEN("The highest card: Ace of Spades")
	{
		const auto card = Card{Rank::Ace, Suit::Spades};

		THEN("It streams as 'Ace of Spades'")
		{
			std::ostringstream os;
			os << card;
			CHECK(os.str() == "Ace of Spades");
		}
	}
}

// ============================================================================
// Chaining multiple cards to the same stream
// ============================================================================

SCENARIO("Streaming multiple cards to the same ostream")
{
	GIVEN("Two cards streamed in sequence")
	{
		const auto c1 = Card{Rank::Three, Suit::Diamonds};
		const auto c2 = Card{Rank::Jack, Suit::Clubs};

		THEN("Both appear in order in the stream")
		{
			std::ostringstream os;
			os << c1 << " | " << c2;
			CHECK(os.str() == "3 of Diamonds | Jack of Clubs");
		}
	}
}

// ============================================================================
// Streaming returns the ostream reference (chainability)
// ============================================================================

SCENARIO("Streaming operators return ostream reference for chaining")
{
	GIVEN("An ostringstream")
	{
		std::ostringstream os;

		THEN("Rank streaming returns the same stream")
		{
			auto& result = (os << Rank::Ace);
			CHECK(&result == &os);
		}

		THEN("Suit streaming returns the same stream")
		{
			auto& result = (os << Suit::Hearts);
			CHECK(&result == &os);
		}

		THEN("Card streaming returns the same stream")
		{
			const auto card = Card{Rank::Two, Suit::Clubs};
			auto& result = (os << card);
			CHECK(&result == &os);
		}
	}
}
