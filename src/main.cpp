#include "blackjack-game.h"
#include "streaming.h"
#include "test-decks.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>

namespace blackjack_v0
{
	char naive_strategy(const std::vector<Card>& hand)
	{
		auto total = CardGames::BlackJack::add_em_up(hand);
		return total < 18 ? 'h' : 's';
	}

	char dealers_strategy(const std::vector<Card>& hand)
	{
		auto total = CardGames::BlackJack::add_em_up(hand);
		return total < 17 ? 'h' : 's';
	}
} // namespace blackjack_v0

namespace blackjack_io
{

	void writeline(const std::string& line)
	{
		std::cout << line << std::endl;
	}

	CardGames::BlackJack::Game::Play get_move(bool can_split)
	{
		auto user_input = 0;
		auto user_input_is_valid = true;
		do {
			if (!user_input_is_valid) {
				writeline(std::format("Invalid move: {}", user_input));
			}
			if (can_split) {
				writeline("Press 'h' to hit, 's' to stay, or 'p' to split.");
			} else {
				writeline("Press 'h' to hit or 's' to stay.");
			}
			user_input = std::cin.get();
			std::cin.get(); // flush out 'Enter'
			user_input_is_valid = user_input == 'h' || user_input == 's' ||
			                      (can_split && user_input == 'p');
		} while (!user_input_is_valid);

		switch (user_input) {
			case 'h': return CardGames::BlackJack::Game::Play::Hit;
			case 's': return CardGames::BlackJack::Game::Play::Stay;
			case 'p': return CardGames::BlackJack::Game::Play::Split;
			default: {
				std::cout << "Invalid move: " << user_input << "\nQuitting game.";
				exit(1);
			}
		}
	}

	void print_all_cards_face_up(const CardGames::BlackJack::GameState& state)
	{
		std::cout << std::endl;
		std::cout << "Dealer: ";
		print_hand(state.dealer_hand().cards());
		std::cout << "Player: ";
		print_hand(state.players_hand().active_cards());
	}

	void print_game_state(const CardGames::BlackJack::GameState& state)
	{
		using namespace CardGames::BlackJack;
		using enum GameNode;

		if (state.node() == Ready) {
			writeline("Let's play some blackjack!");
		} else if (state.node() == PlayersRound || state.node() == DealersRound) {
			const auto& dealers_hand = state.dealer_hand().cards();
			const auto& players_hand = state.players_hand().active_cards();
			std::cout << "Dealer: ";
			print_hand_hide_some(dealers_hand, 1);
			std::cout << "Player: ";
			print_hand(players_hand);
			std::cout << "(" << state.players_hand().active_total() << ")" << std::endl;
		} else if (state.node() == PlayersSplitRound) {
			const auto& dealers_hand = state.dealer_hand().cards();
			std::cout << "Dealer: ";
			print_hand_hide_some(dealers_hand, 1);
			std::cout << "\nSplit hands:\n";
			const auto& all_hands = state.players_hand().all_hands();
			for (size_t i = 0; i < all_hands.size(); ++i) {
				const auto& hand = all_hands[i];
				std::cout << "  Hand " << (i + 1);
				if (i == state.players_hand().active_index()) {
					std::cout << " (active)";
				}
				if (hand.is_complete) {
					std::cout << " [done]";
				}
				std::cout << ": ";
				print_hand(hand.cards);
				std::cout << "    (" << calculate_hand_value(hand.cards).total << ")" << std::endl;
			}
		} else if (state.node() == GameOverPlayerWins) {
			print_all_cards_face_up(state);
			const auto player = state.players_hand().active_total();
			const auto dealer = state.dealer_hand().total();
			writeline(std::format("Congratulations! You win, {} to {}", player, dealer));
		} else if (state.node() == GameOverPlayerBusts) {
			print_all_cards_face_up(state);
			const auto player = state.players_hand().active_total();
			writeline(std::format("Sorry, you bust({}). Dealer wins.", player));
		} else if (state.node() == GameOverDealerWins) {
			print_all_cards_face_up(state);
			const auto player = state.players_hand().active_total();
			const auto dealer = state.dealer_hand().total();
			writeline(std::format("Dealer wins, {} to {}", dealer, player));
		} else if (state.node() == GameOverDealerBusts) {
			print_all_cards_face_up(state);
			const auto dealer = state.dealer_hand().total();
			writeline(std::format("Dealer busts ({}). You win!", dealer));
		} else if (state.node() == GameOverDraw) {
			print_all_cards_face_up(state);
			const auto player = state.players_hand().active_total();
			const auto dealer = state.dealer_hand().total();
			assert(player == dealer);
			writeline(std::format("Draw, {} up.", dealer));
		} else {
			std::cout << "Unknown game node" << std::endl;
		}
	}
} // namespace blackjack_io

void play_blackjack(CardGames::BlackJack::BlackjackConfig config = {})
{
	using namespace CardGames;
	using enum BlackJack::GameNode;

	auto game = BlackJack::Game{config};
	blackjack_io::print_game_state(game.state());

	auto state = game.next(BlackJack::Game::Play::Deal);
	blackjack_io::print_game_state(state);

	// Player's turn - handles both normal and split rounds
	while (state.node() == PlayersRound ||
	       state.node() == PlayersSplitRound) {
		const auto can_split = state.can_split();
		const auto players_move = blackjack_io::get_move(can_split);
		state = game.next(players_move);
		blackjack_io::print_game_state(state);
	}
	// Note: Dealer auto-plays when player stays, so no dealer loop needed
}

#if 0

void play_blackjack_v0() {
  auto deck = shuffle(Deck{});
  auto dealers_hand = deck.deal(2);
  auto players_hand = deck.deal(2);
  std::cout << "Dealer: ";
  print_hand_hide_some(dealers_hand, 1);
  std::cout << "Player: ";
  print_hand(players_hand);
  std::cout << "(" << CardGames::BlackJack::add_em_up(players_hand) << ")" << std::endl;

  //***************************************//
  //										                   //
  //		Player's Round					           //
  //										                   //
  //***************************************//
  auto player_busts = false;
  {
    // auto move = std::cin.get();
    std::cin.get();
    auto move = blackjack_v0::naive_strategy(players_hand);
    while (move) {
      if (move == 'h') {
        std::cout << "Player hits" << std::endl;
        players_hand.emplace_back(deck.deal(1).front());
        std::cout << std::endl;
        std::cout << "Dealer: ";
        print_hand_hide_some(dealers_hand, 1);
        std::cout << "Player: ";
        print_hand(players_hand);
        std::cout << "(" << CardGames::BlackJack::add_em_up(players_hand) << ")"
                  << std::endl;
        if (CardGames::BlackJack::add_em_up(players_hand) > 21) {
          std::cout << "Sorry, you bust. Dealer wins." << std::endl;
          player_busts = true;
          break;
        }
      } else if (move == 's') {
        std::cout << "Player stays" << std::endl;
        break;
      } else {
        std::cout << std::endl;
        std::cout << "Invalid move." << std::endl;
        std::cout << "Press 'h' to hit or 's' to stay." << std::endl;
      }
      // move = std::cin.get();
      std::cin.get();
      move = blackjack_v0::naive_strategy(players_hand);
    }
  }

  //***************************************//
  //										                   //
  //		Dealer's Round					           //
  //										                   //
  //***************************************//

  auto dealer_busts = false;
  if (!player_busts) {
    auto move = blackjack_v0::dealers_strategy(dealers_hand);
    while (move) {
      if (move == 'h') {
        std::cout << "Dealer hits" << std::endl;
        dealers_hand.emplace_back(deck.deal(1).front());
        std::cout << std::endl;
        std::cout << "Dealer: ";
        print_hand(dealers_hand);
        std::cout << "Player: ";
        print_hand(players_hand);
        std::cout << "(" << CardGames::BlackJack::add_em_up(players_hand) << ")"
                  << std::endl;
        if (CardGames::BlackJack::add_em_up(dealers_hand) > 21) {
          std::cout << "Dealer busts. You win!" << std::endl;
          dealer_busts = true;
          break;
        }
      } else if (move == 's') {
        std::cout << "Dealer stays" << std::endl;
        break;
      } else {
        std::cout << std::endl;
        std::cout << "Invalid move." << std::endl;
        std::cout << "Press 'h' to hit or 's' to stay." << std::endl;
      }
      // move = std::cin.get();
      std::cin.get();
      move = blackjack_v0::dealers_strategy(players_hand);
    }
  }

  //***************************************//
  //										                   //
  //		Synopsis						               //
  //										                   //
  //***************************************//

  if (!(player_busts || dealer_busts)) {
    std::cout << std::endl;
    std::cout << "Dealer: ";
    print_hand(dealers_hand);
    std::cout << "Player: ";
    print_hand(players_hand);

    const auto player = CardGames::BlackJack::add_em_up(players_hand);
    const auto dealer = CardGames::BlackJack::add_em_up(dealers_hand);
    const auto player_busts = player > 21;
    const auto dealer_busts = dealer > 21;
    if (player_busts || (dealer > player && !dealer_busts)) {
      std::cout << "Dealer wins, " << dealer << " to " << player << std::endl;
    } else if (dealer_busts || (player > dealer && !player_busts)) {
      std::cout << "You win, " << player << " to " << dealer << std::endl;
    } else if (dealer_busts && player_busts) {
      std::cout << "Not quite sure what happens here. Maybe the player wins if "
                   "both she and the dealer bust. But what happens then with "
                   "multiple players?"
                << std::endl;
      std::cout << "Dealer: " << dealer << std::endl;
      std::cout << "Player: " << player << std::endl;
    } else {
      std::cout << "Draw, " << dealer << " up" << std::endl;

      std::cout << "Dealer: " << dealer << std::endl;
      std::cout << "Player: " << player << std::endl;
      assert(dealer == player);
    }
  }

  std::cin.get();
}
#endif

int main(int argc, char* argv[])
{
	std::optional<std::string> deck_name;
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "--deck" && i + 1 < argc) {
			deck_name = argv[++i];
		}
	}

	if (deck_name) {
		auto deck = CardGames::BlackJack::get_test_deck(*deck_name);
		if (!deck) {
			std::cerr << "Unknown deck: " << *deck_name << "\nAvailable: ";
			for (const auto& n : CardGames::BlackJack::get_test_deck_names()) {
				std::cerr << n << " ";
			}
			std::cerr << "\n";
			return 1;
		}
		play_blackjack({.initial_deck = deck});
	} else {
		play_blackjack();
	}

	std::cin.get();
	return 0;
}
