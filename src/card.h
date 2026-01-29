#pragma once

#include <compare>
#include <concepts>
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

	bool operator==(const Card&) const = default;

	std::strong_ordering operator<=>(const Card& other) const
	{
		return static_cast<int>(rank) <=> static_cast<int>(other.rank);
	}
};

static_assert(std::regular<Card>);
