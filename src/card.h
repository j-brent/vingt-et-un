#pragma once

#include <compare>
#include <concepts>
#include <cstdint>
#include <vector>

/// Playing card value type.
/// Equality (==) compares both rank and suit. Ordering (<, <=, >, >=) compares
/// by rank only — suit is not considered when determining which card is higher,
/// matching standard card game conventions.
struct Card {

	enum class Suit : std::uint8_t { Clubs, Diamonds, Hearts, Spades };

	enum class Rank : std::uint8_t {
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
		: m_suit{suit}
		, m_rank{rank}
	{
	}
	Card(Suit suit, Rank rank)
		: m_suit{suit}
		, m_rank{rank}
	{
	}

	[[nodiscard]] Suit suit() const { return m_suit; }
	[[nodiscard]] Rank rank() const { return m_rank; }

	static std::vector<Suit> suits()
	{
		return {Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades};
	}

	static std::vector<Rank> ranks()
	{
		return {Rank::Two,	Rank::Three, Rank::Four, Rank::Five,	Rank::Six,	Rank::Seven, Rank::Eight,
						Rank::Nine, Rank::Ten,	 Rank::Jack, Rank::Queen, Rank::King, Rank::Ace};
	}

	bool operator==(const Card&) const = default;

	/// Orders by rank only — suit is ignored for ordering purposes.
	std::strong_ordering operator<=>(const Card& other) const
	{
		return static_cast<std::uint8_t>(m_rank) <=> static_cast<std::uint8_t>(other.m_rank);
	}

private:
	Suit m_suit{};
	Rank m_rank{};
};

static_assert(std::regular<Card>);
