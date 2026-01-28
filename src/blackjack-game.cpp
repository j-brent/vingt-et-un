#include "blackjack-game.h"

#include <numeric>

namespace CardGames
{
	namespace BlackJack
	{
		HandValue calculate_hand_value(const std::vector<Card>& hand)
		{
			int total = 0;
			int ace_count = 0;

			for (const auto& card : hand) {
				switch (card.rank) {
					case Card::Rank::Ace:
						total += 11;
						++ace_count;
						break;
					case Card::Rank::King:
					case Card::Rank::Queen:
					case Card::Rank::Jack:
						total += 10;
						break;
					default:
						total += static_cast<int>(card.rank);
						break;
				}
			}

			// Reduce aces from 11 to 1 as needed to avoid bust
			int soft_ace_count = ace_count;
			while (total > 21 && soft_ace_count > 0) {
				total -= 10;
				--soft_ace_count;
			}

			return {total, soft_ace_count > 0, soft_ace_count};
		}

		int add_em_up(const std::vector<Card>& hand)
		{
			return calculate_hand_value(hand).total;
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
						play_dealer_turn();
						return history.back();
					}
					else if (play == Game::Play::Split) {
						if (current_state.can_split()) {
							auto deck = current_state.deck();
							const auto& original_cards = current_state.players_hand();
							const bool is_aces = original_cards[0].rank == Card::Rank::Ace;

							// Create two new hands from the split
							std::vector<Hand> new_hands;
							auto card1 = std::vector<Card>{original_cards[0], deck.deal(1).front()};
							auto card2 = std::vector<Card>{original_cards[1], deck.deal(1).front()};

							// Mark as split hands; if aces, they're complete (can't hit)
							new_hands.emplace_back(Hand{card1, true, is_aces});
							new_hands.emplace_back(Hand{card2, true, is_aces});

							if (is_aces) {
								// Both hands complete immediately - go to dealer
								history.emplace_back(GameNode::DealersRound, new_hands, 0,
								                     current_state.dealer_hand(), deck);
								play_dealer_turn();
							} else {
								// Start playing hand 0
								history.emplace_back(GameNode::PlayersSplitRound, new_hands, 0,
								                     current_state.dealer_hand(), deck);
							}
							return history.back();
						}
					}
					break;

				case GameNode::PlayersSplitRound:
					if (play == Game::Play::Hit) {
						auto deck = current_state.deck();
						auto hands = current_state.player_hands();
						auto active_idx = current_state.active_hand_index();

						// Add card to active hand
						hands[active_idx].add(deck.deal(1).front());

						if (hands[active_idx].total() > 21) {
							// Busted - mark complete and advance to next incomplete hand
							hands[active_idx].set_complete(true);
							for (size_t i = active_idx + 1; i < hands.size(); ++i) {
								if (!hands[i].is_complete()) {
									active_idx = i;
									break;
								}
							}
						}

						// Check if all hands are complete
						const bool all_complete = std::all_of(hands.begin(), hands.end(),
						                                      [](const Hand& h) { return h.is_complete(); });
						const bool all_busted = std::all_of(hands.begin(), hands.end(),
						                                    [](const Hand& h) { return h.is_complete() && h.total() > 21; });

						if (all_busted) {
							history.emplace_back(GameNode::GameOverPlayerBusts, hands, 0,
							                     current_state.dealer_hand(), deck);
						} else if (all_complete) {
							history.emplace_back(GameNode::DealersRound, hands, 0,
							                     current_state.dealer_hand(), deck);
							play_dealer_turn();
						} else {
							history.emplace_back(GameNode::PlayersSplitRound, hands, active_idx,
							                     current_state.dealer_hand(), deck);
						}
						return history.back();
					}
					else if (play == Game::Play::Stay) {
						auto hands = current_state.player_hands();
						auto active_idx = current_state.active_hand_index();

						// Mark current hand complete and advance to next incomplete hand
						hands[active_idx].set_complete(true);
						for (size_t i = active_idx + 1; i < hands.size(); ++i) {
							if (!hands[i].is_complete()) {
								active_idx = i;
								break;
							}
						}

						// Check if all hands are complete
						const bool all_complete = std::all_of(hands.begin(), hands.end(),
						                                      [](const Hand& h) { return h.is_complete(); });
						const bool all_busted = std::all_of(hands.begin(), hands.end(),
						                                    [](const Hand& h) { return h.is_complete() && h.total() > 21; });

						if (all_busted) {
							history.emplace_back(GameNode::GameOverPlayerBusts, hands, 0,
							                     current_state.dealer_hand(), current_state.deck());
						} else if (all_complete) {
							history.emplace_back(GameNode::DealersRound, hands, 0,
							                     current_state.dealer_hand(), current_state.deck());
							play_dealer_turn();
						} else {
							history.emplace_back(GameNode::PlayersSplitRound, hands, active_idx,
							                     current_state.dealer_hand(), current_state.deck());
						}
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
				case GameNode::GameOverDraw:
					break;
			}
			return current_state;
		}

		void Game::play_dealer_turn()
		{
			while (history.back().node() == GameNode::DealersRound) {
				const auto& current_state = history.back();
				const auto hand_value = calculate_hand_value(current_state.dealers_hand());

				// Dealer must hit on 16 or less
				// Dealer must hit on soft 17 if hit_soft_17 is true
				// Dealer must stand on hard 17 or higher
				const bool must_hit = hand_value.total < 17 ||
				                      (hand_value.total == 17 && hand_value.is_soft && m_dealer_rules.hit_soft_17);

				if (must_hit) {
					// Dealer hits
					auto deck = current_state.deck();
					auto dealers_hand = current_state.dealers_hand();
					dealers_hand.emplace_back(deck.deal(1).front());

					const auto new_value = calculate_hand_value(dealers_hand);
					const auto game_node = (new_value.total > 21) ? GameNode::GameOverDealerBusts
					                                              : GameNode::DealersRound;

					history.emplace_back(game_node, current_state.players_hand(), dealers_hand, deck);
				} else {
					// Dealer stands - final comparison
					const auto player = add_em_up(current_state.players_hand());
					const auto dealer = hand_value.total;
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
				}
			}
		}
	} // namespace BlackJack
} // namespace CardGames
