#pragma once

#include "CompileTimeChecks.h"

#include <vector>

struct Card {
  
enum class Suit { Clubs, Diamonds, Hearts, Spades };

enum class Rank {
	Two = 2,
	Three = 3,
	Four = 4,
	Five = 5,
	Six = 6,
	Seven = 7,
	Eight = 8,
	Nine = 9,
	Ten = 10,
	Jack = 11,
	Queen = 12,
	King = 13,
	Ace = 14
};

	Card() = default;
	Card(Rank rank, Suit suit)
		: suit{suit}
		, rank{rank}
	{
	}
	Card(Suit suit, Rank rank)
		: suit{suit}
		, rank{rank}
	{
	}
	Suit suit;
	Rank rank;

	static std::vector<Suit> suits()
	{
		return {Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades};
	}
};

inline bool operator<(const Card& a, const Card& b)
{
	return a.rank < b.rank;
}

inline bool operator==(const Card& lhs, const Card& rhs)
{
	return lhs.rank == rhs.rank && lhs.suit == rhs.suit;
}

static_assert(is_regular<Card>::value, "User-defined type Card is not a regular type.");
