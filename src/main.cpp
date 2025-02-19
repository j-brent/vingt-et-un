#include "blackjack-game.h"
#include "streaming.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <string>

namespace blackjack_v0
{
	int add_em_up(const std::vector<Card>& hand)
	{
    using Rank = Card::Rank;
    using Suit = Card::Suit;

		auto values = std::vector<int>(hand.size());
		std::transform(begin(hand), end(hand), std::back_inserter(values), [](const Card& card) {
			switch (card.rank) {
				case Rank::Ace: return 11;
				case Rank::King:
				case Rank::Queen:
				case Rank::Jack: return 10;
				default: return static_cast<int>(card.rank);
			}
		});

		return std::accumulate(begin(values), end(values), 0,
													 [](auto acc, auto value) { return acc + value; });
	}

	char naive_strategy(const std::vector<Card>& hand)
	{
		auto total = add_em_up(hand);
		return total < 18 ? 'h' : 's';
	}

	char dealers_strategy(const std::vector<Card>& hand)
	{
		auto total = add_em_up(hand);
		return total < 17 ? 'h' : 's';
	}
} // namespace blackjack_v0

namespace blackjack_io
{

	void writeline(const std::string& line)
	{
		std::cout << line << std::endl;
	}

	CardGames::BlackJack::Game::Play get_move()
	{

		auto user_input = 0;
		auto user_input_is_valid = true;
		do {
			if (!user_input_is_valid) {
				writeline("Invalid move: " + std::to_string(user_input));
			}
			writeline("Press 'h' to hit or 's' to stay.");
			user_input = std::cin.get();
      std::cin.get(); // flush out 'Enter'
			user_input_is_valid = user_input == 'h' || user_input == 's';
		} while (!user_input_is_valid);

		switch (user_input) {
			case 'h': return CardGames::BlackJack::Game::Play::Hit;
			case 's': return CardGames::BlackJack::Game::Play::Stay;
			default: {
				std::cout << "Invalid move: " << user_input << "\nQuitting game.";
				exit(1);
			}
		}
	}

	CardGames::BlackJack::Game::Play dealers_strategy(const std::vector<Card>& dealers_hand)
	{
		auto total = blackjack_v0::add_em_up(dealers_hand);
		return (total < 17) ? CardGames::BlackJack::Game::Play::Hit
												: CardGames::BlackJack::Game::Play::Stay;
	}

	void print_all_cards_face_up(const CardGames::BlackJack::GameState& state)
	{
		std::cout << std::endl;
		std::cout << "Dealer: ";
		print_hand(state.dealers_hand());
		std::cout << "Player: ";
		print_hand(state.players_hand());
	}

	void print_game_state(const CardGames::BlackJack::GameState& state)
	{
		using namespace CardGames::BlackJack;
		if (state.node() == GameNode::Ready) {
			writeline("Let's play some blackjack!");
		} else if (state.node() == GameNode::PlayersRound || state.node() == GameNode::DealersRound) {
			auto dealers_hand = state.dealers_hand();
			auto players_hand = state.players_hand();
			std::cout << "Dealer: ";
			print_hand_hide_some(dealers_hand, 1);
			std::cout << "Player: ";
			print_hand(players_hand);
			std::cout << "(" << blackjack_v0::add_em_up(players_hand) << ")" << std::endl;
		} else if (state.node() == GameNode::GameOverPlayerWins) {
			print_all_cards_face_up(state);
			const auto player = blackjack_v0::add_em_up(state.players_hand());
			const auto dealer = blackjack_v0::add_em_up(state.dealers_hand());
			writeline("Congratulations! You win, " + std::to_string(player) + " to " +
								std::to_string(dealer));
		} else if (state.node() == GameNode::GameOverPlayerBusts) {
			print_all_cards_face_up(state);
			const auto player = blackjack_v0::add_em_up(state.players_hand());
			writeline("Sorry, you bust(" + std::to_string(player) + "). Dealer wins.");
		} else if (state.node() == GameNode::GameOverDealerWins) {
			print_all_cards_face_up(state);
			const auto player = blackjack_v0::add_em_up(state.players_hand());
			const auto dealer = blackjack_v0::add_em_up(state.dealers_hand());
			writeline("Dealer wins, " + std::to_string(dealer) + " to " + std::to_string(player));
		} else if (state.node() == GameNode::GameOverDealerBusts) {
			print_all_cards_face_up(state);
			const auto dealer = blackjack_v0::add_em_up(state.dealers_hand());
			writeline("Dealer busts (" + std::to_string(dealer) + "). You win!");
		} else if (state.node() == GameNode::GameOverDraw) {
			print_all_cards_face_up(state);
			const auto player = blackjack_v0::add_em_up(state.players_hand());
			const auto dealer = blackjack_v0::add_em_up(state.dealers_hand());
			assert(player == dealer);
			writeline("Draw, " + std::to_string(dealer) + " up.");
		} else {
			std::cout << "Unknown game node" << std::endl;
		}
	}
} // namespace blackjack_io

void play_blackjack()
{
	using namespace CardGames;

	auto game = BlackJack::Game{};
	blackjack_io::print_game_state(game.state());

	auto state = game.next(BlackJack::Game::Play::Deal);
	blackjack_io::print_game_state(state);
  
	while (state.node() == BlackJack::GameNode::PlayersRound) {
		const auto players_move = blackjack_io::get_move();
		state = game.next(players_move);
		blackjack_io::print_game_state(state);
	}

  while (state.node() == BlackJack::GameNode::DealersRound) {
    const auto dealers_move = blackjack_io::dealers_strategy(state.dealers_hand());
    state = game.next(dealers_move);
    blackjack_io::print_game_state(state);
  }
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
  std::cout << "(" << blackjack_v0::add_em_up(players_hand) << ")" << std::endl;

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
        std::cout << "(" << blackjack_v0::add_em_up(players_hand) << ")"
                  << std::endl;
        if (blackjack_v0::add_em_up(players_hand) > 21) {
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
        std::cout << "(" << blackjack_v0::add_em_up(players_hand) << ")"
                  << std::endl;
        if (blackjack_v0::add_em_up(dealers_hand) > 21) {
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

    const auto player = blackjack_v0::add_em_up(players_hand);
    const auto dealer = blackjack_v0::add_em_up(dealers_hand);
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

int main()
{
	play_blackjack();
	std::cin.get();
	return 0;
}
