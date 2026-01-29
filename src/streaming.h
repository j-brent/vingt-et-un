#pragma once

#include "card.h"
#include "deck.h"

#include <span>

std::ostream& operator<<(std::ostream& os, const Card::Rank& value);
std::ostream& operator<<(std::ostream& os, const Card::Suit& suit);
std::ostream& operator<<(std::ostream& os, const Card& card);

void print(const Deck& deck);

#ifdef _WIN32
void print_terse(const Deck& deck);
#endif

void print_hand(std::span<const Card> hand);

void print_hidden_hand(size_t num_cards);

void print_hand_hide_some(std::span<const Card> hand, size_t num_to_hide);
