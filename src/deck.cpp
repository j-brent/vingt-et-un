#include "deck.h"

const std::vector<Card>& Deck::cards() const
{
	return m_cards;
}

Card Deck::deal()
{
	return deal(1).front();
}

std::vector<Card> Deck::deal(size_t num_cards)
{
	auto hand = std::vector<Card>{};
	if (num_cards <= m_cards.size()) {
		const auto b = std::begin(m_cards);
		const auto m = std::next(b, num_cards);
		const auto e = std::end(m_cards);
		hand = {b, m};
		m_cards = {m, e};
	}
	return hand;
}

Deck shuffle(const Deck& deck)
{
	std::random_device rd;
	std::mt19937 rng(rd());

	auto cards = deck.cards();
	std::shuffle(begin(cards), end(cards), rng);
	return {cards};
}
