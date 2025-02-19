#include "blackjack-game.h"

#include <numeric>

namespace CardGames
{
	namespace BlackJack
	{
		int add_em_up(const std::vector<Card>& hand)
		{

			auto values = std::vector<int>(hand.size());
			std::transform(begin(hand), end(hand), std::back_inserter(values), [](const Card& card) {
				switch (card.rank) {
					case Card::Rank::Ace: return 11;
					case Card::Rank::King:
					case Card::Rank::Queen:
					case Card::Rank::Jack: return 10;
					default: return static_cast<int>(card.rank);
				}
			});

			return std::accumulate(begin(values), end(values), 0,
														 [](auto acc, auto value) { return acc + value; });
		}

		const GameState& Game::next(Game::Play play)
		{
			const auto& current_state = history.back();
			switch (current_state.node()) {
				case GameNode::Ready:
					if (play == Game::Play::Deal) {
						auto deck = current_state.deck();
						auto players_hand = deck.deal(1);								 // face down
						auto dealers_hand = deck.deal(1);								 // face down
						players_hand.emplace_back(deck.deal(1).front()); // face up
						dealers_hand.emplace_back(deck.deal(1).front()); // face up

						const auto game_node = [&]() {
							const auto player = add_em_up(players_hand);
							const auto dealer = add_em_up(dealers_hand);
							if (player == 21)
								return GameNode::GameOverPlayerWins;
							if (dealer == 21)
								return GameNode::GameOverDealerWins;
							return GameNode::PlayersRound;
						}();

						history.emplace_back(game_node, players_hand, dealers_hand, deck);
						return history.back();
					}
					break;
				case GameNode::PlayersRound:
					if (play == Game::Play::Hit) {

						auto deck = current_state.deck();
						auto players_hand = current_state.players_hand();
						players_hand.emplace_back(deck.deal(1).front());
						const auto game_node = (add_em_up(players_hand) > 21)
                                 ? GameNode::GameOverPlayerBusts
																 : GameNode::PlayersRound;

						history.emplace_back(game_node, players_hand, current_state.dealers_hand(), deck);
						return history.back();
					} 
          else if (play == Game::Play::Stay) {
						history.emplace_back(GameNode::DealersRound, current_state.players_hand(),
																 current_state.dealers_hand(), current_state.deck());
						return history.back();
					}
					break;
				case GameNode::DealersRound:
					if (play == Game::Play::Hit) {

						auto deck = current_state.deck();
						auto dealers_hand = current_state.dealers_hand();
						dealers_hand.emplace_back(deck.deal(1).front());
						const auto game_node = (add_em_up(dealers_hand) > 21) ? GameNode::GameOverDealerBusts
																																	: GameNode::DealersRound;

						history.emplace_back(game_node, current_state.players_hand(), dealers_hand, deck);
						return history.back();
					} else if (play == Game::Play::Stay) {
						const auto player = add_em_up(current_state.players_hand());
						const auto dealer = add_em_up(current_state.dealers_hand());
						const auto game_node = [&]() {
							if (player > dealer)
								return GameNode::GameOverPlayerWins;
							else if (dealer > player)
								return GameNode::GameOverDealerWins;
							else
								return GameNode::GameOverDraw;
						}();
						history.emplace_back(game_node, current_state.players_hand(),
																 current_state.dealers_hand(), current_state.deck());
						return history.back();
					}
					break;
				case GameNode::GameOverPlayerBusts:
				case GameNode::GameOverPlayerWins:
				case GameNode::GameOverDealerBusts:
				case GameNode::GameOverDealerWins:
				case GameNode::GameOverDraw: break;
			}
			return current_state;
		}
	} // namespace BlackJack
} // namespace CardGames
