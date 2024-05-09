#pragma once

#include "card.h"
#include "deck.h"

std::ostream& operator<<(std::ostream& os, const Rank& value);
std::ostream& operator<<(std::ostream& os, const Suit& suit);
std::ostream& operator<<(std::ostream& os, const Card& card);

void print(const Deck& deck);

#ifdef _WIN32
void print_terse(const Deck& deck);
#endif

void print_hand(const std::vector<Card>& hand);

void print_hidden_hand(size_t num_cards);

void print_hand_hide_some(const std::vector<Card>& hand, size_t num_to_hide);
