#include "test-decks.h"

#include <unordered_map>

namespace CardGames::BlackJack
{

	namespace
	{
		using Rank = Card::Rank;
		using Suit = Card::Suit;

		// Named deck configurations for deterministic testing
		// Card order: player1, dealer1, player2, dealer2, then remaining cards for hits
		const std::unordered_map<std::string, std::vector<Card>> s_test_decks = {
			// Player gets pair of 8s (16), dealer gets 10+6 (16)
			{"split_pair",
			 {
				 {Rank::Eight, Suit::Clubs},	// player 1
				 {Rank::Ten, Suit::Hearts},		// dealer 1
				 {Rank::Eight, Suit::Hearts}, // player 2
				 {Rank::Six, Suit::Diamonds}, // dealer 2
				 {Rank::Five, Suit::Spades},	// hit cards follow...
				 {Rank::Three, Suit::Clubs},
				 {Rank::Two, Suit::Hearts},
				 {Rank::Four, Suit::Diamonds},
				 {Rank::Seven, Suit::Spades},
				 {Rank::Six, Suit::Clubs}, // extra cards for dealer/more hits
				 {Rank::Nine, Suit::Hearts},
				 {Rank::Two, Suit::Diamonds},
			 }},
			// Player gets 10+8 (18), dealer gets 10+6 then draws 10 (bust)
			{"dealer_bust",
			 {
				 {Rank::Ten, Suit::Clubs},
				 {Rank::Ten, Suit::Hearts},
				 {Rank::Eight, Suit::Diamonds},
				 {Rank::Six, Suit::Spades},
				 {Rank::Ten, Suit::Diamonds}, // dealer hits and busts
			 }},
			// Player gets A+K (blackjack 21)
			{"player_blackjack",
			 {
				 {Rank::Ace, Suit::Spades},
				 {Rank::Ten, Suit::Hearts},
				 {Rank::King, Suit::Diamonds},
				 {Rank::Seven, Suit::Clubs},
			 }},
			// Player gets 10+6 (16), hit card is 6 (bust)
			{"player_bust",
			 {
				 {Rank::Ten, Suit::Clubs},
				 {Rank::Ten, Suit::Hearts},
				 {Rank::Six, Suit::Diamonds},
				 {Rank::Seven, Suit::Spades},
				 {Rank::Six, Suit::Clubs}, // player hits and busts
			 }},
			// Player gets pair of Aces (12), dealer gets 10+6 (16)
			{"split_aces",
			 {
				 {Rank::Ace, Suit::Clubs},		// player 1
				 {Rank::Ten, Suit::Hearts},		// dealer 1
				 {Rank::Ace, Suit::Hearts},		// player 2
				 {Rank::Six, Suit::Diamonds}, // dealer 2
				 {Rank::Ten, Suit::Spades},		// split hand 1 gets 10 (21)
				 {Rank::Nine, Suit::Clubs},		// split hand 2 gets 9 (20)
				 {Rank::Ten, Suit::Diamonds}, // dealer hits and busts
			 }},
		};
	} // namespace

	std::optional<Deck> get_test_deck(const std::string& name)
	{
		auto it = s_test_decks.find(name);
		if (it != s_test_decks.end()) {
			return Deck{it->second};
		}
		return std::nullopt;
	}

	std::vector<std::string> get_test_deck_names()
	{
		std::vector<std::string> names;
		names.reserve(s_test_decks.size());
		for (const auto& [name, _] : s_test_decks) {
			names.push_back(name);
		}
		return names;
	}

} // namespace CardGames::BlackJack
