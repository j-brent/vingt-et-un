#pragma once

#include "CompileTimeChecks.h"
#include "card.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

class Deck
{
  using Rank = Card::Rank;
  using Suit = Card::Suit;
public:
	Deck()
		: m_cards{{{Rank::Two, Suit::Clubs},			{Rank::Three, Suit::Clubs},
							 {Rank::Four, Suit::Clubs},			{Rank::Five, Suit::Clubs},
							 {Rank::Six, Suit::Clubs},			{Rank::Seven, Suit::Clubs},
							 {Rank::Eight, Suit::Clubs},		{Rank::Nine, Suit::Clubs},
							 {Rank::Ten, Suit::Clubs},			{Rank::Jack, Suit::Clubs},
							 {Rank::Queen, Suit::Clubs},		{Rank::King, Suit::Clubs},
							 {Rank::Ace, Suit::Clubs},

							 {Rank::Two, Suit::Diamonds},		{Rank::Three, Suit::Diamonds},
							 {Rank::Four, Suit::Diamonds},	{Rank::Five, Suit::Diamonds},
							 {Rank::Six, Suit::Diamonds},		{Rank::Seven, Suit::Diamonds},
							 {Rank::Eight, Suit::Diamonds}, {Rank::Nine, Suit::Diamonds},
							 {Rank::Ten, Suit::Diamonds},		{Rank::Jack, Suit::Diamonds},
							 {Rank::Queen, Suit::Diamonds}, {Rank::King, Suit::Diamonds},
							 {Rank::Ace, Suit::Diamonds},

							 {Rank::Two, Suit::Hearts},			{Rank::Three, Suit::Hearts},
							 {Rank::Four, Suit::Hearts},		{Rank::Five, Suit::Hearts},
							 {Rank::Six, Suit::Hearts},			{Rank::Seven, Suit::Hearts},
							 {Rank::Eight, Suit::Hearts},		{Rank::Nine, Suit::Hearts},
							 {Rank::Ten, Suit::Hearts},			{Rank::Jack, Suit::Hearts},
							 {Rank::Queen, Suit::Hearts},		{Rank::King, Suit::Hearts},
							 {Rank::Ace, Suit::Hearts},

							 {Rank::Two, Suit::Spades},			{Rank::Three, Suit::Spades},
							 {Rank::Four, Suit::Spades},		{Rank::Five, Suit::Spades},
							 {Rank::Six, Suit::Spades},			{Rank::Seven, Suit::Spades},
							 {Rank::Eight, Suit::Spades},		{Rank::Nine, Suit::Spades},
							 {Rank::Ten, Suit::Spades},			{Rank::Jack, Suit::Spades},
							 {Rank::Queen, Suit::Spades},		{Rank::King, Suit::Spades},
							 {Rank::Ace, Suit::Spades}}}
	{
	}

	Deck(const std::vector<Card>& cards)
		: m_cards{cards}
	{
	}

	Deck(const Deck&) = default;
	Deck(Deck&&) = default;

	Deck& operator=(const Deck&) = default;
	Deck& operator=(Deck&&) = default;

	const std::vector<Card>& cards() const;

  Card deal();

	std::vector<Card> deal(size_t num_cards);

	using const_iterator = std::vector<Card>::const_iterator;
	const_iterator begin() const { return m_cards.cbegin(); }
	const_iterator end() const { return m_cards.cend(); }

	using iterator = std::vector<Card>::iterator;
	iterator begin() { return m_cards.begin(); }
	iterator end() { return m_cards.end(); }

private:
	std::vector<Card> m_cards;
};

inline bool operator==(const Deck& lhs, const Deck& rhs)
{
	const auto& lhs_cards = lhs.cards();
	const auto& rhs_cards = rhs.cards();
	return lhs_cards.size() == rhs_cards.size() &&
				 std::equal(begin(lhs_cards), end(lhs_cards), begin(rhs_cards));
}

inline bool operator!=(const Deck& lhs, const Deck& rhs)
{
  return !(lhs == rhs);
}

static_assert(is_regular<Deck>::value, "User-defined type Deck is not a regular type.");

Deck shuffle(const Deck& deck);

inline std::vector<Card::Suit> slice_suits(const std::vector<Card>& cards)
{
	auto suits = std::vector<Card::Suit>{};
	std::transform(std::begin(cards), std::end(cards), std::back_inserter(suits),
								 [](const auto& card) { return card.suit; });
	return suits;
}

inline std::vector<Card::Rank> slice_face_values(const std::vector<Card>& cards)
{
	auto values = std::vector<Card::Rank>{};
	std::transform(std::begin(cards), std::end(cards), std::back_inserter(values),
								 [](const auto& card) { return card.rank; });
	return values;
}
