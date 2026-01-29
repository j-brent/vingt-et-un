#include "blackjack-game.h"

#include <numeric>
#include <span>

namespace CardGames::BlackJack
{
		HandValue calculate_hand_value(std::span<const Card> hand)
		{
			int total = 0;
			int ace_count = 0;

			for (const auto& card : hand) {
				switch (card.rank()) {
					case Card::Rank::Ace:
						total += 11;
						++ace_count;
						break;
					case Card::Rank::King:
					case Card::Rank::Queen:
					case Card::Rank::Jack: total += 10; break;
					default: total += static_cast<int>(card.rank()); break;
				}
			}

			// Reduce aces from 11 to 1 as needed to avoid bust
			int soft_ace_count = ace_count;
			while (total > 21 && soft_ace_count > 0) {
				total -= 10;
				--soft_ace_count;
			}

			return {.total = total, .is_soft = soft_ace_count > 0, .soft_ace_count = soft_ace_count};
		}

		int add_em_up(std::span<const Card> hand)
		{
			return calculate_hand_value(hand).total;
		}

		const GameState& Game::next(Game::Play play)
		{
			using enum GameNode;
			using enum Play;

			const auto& current_state = history.back();
			switch (current_state.node()) {
				case Ready:
					if (play == Deal) {
						auto deck = current_state.deck();
						auto player_cards = deck.deal(1);								 // face down
						auto dealer_cards = deck.deal(1);								 // face down
						player_cards.emplace_back(deck.deal(1).front()); // face up
						dealer_cards.emplace_back(deck.deal(1).front()); // face up

						const auto game_node = [&]() {
							const auto player = add_em_up(player_cards);
							const auto dealer = add_em_up(dealer_cards);
							if (player == 21) {
								return GameOverPlayerWins;
							}
							if (dealer == 21) {
								return GameOverDealerWins;
							}
							return PlayersRound;
						}();

						history.emplace_back(game_node, PlayersHand{player_cards}, DealersHand{dealer_cards},
																 deck);
						return history.back();
					}
					break;

				case PlayersRound:
					if (play == Hit) {
						auto deck = current_state.deck();
						auto players_hand = current_state.players_hand();
						players_hand.add_to_active(deck.deal(1).front());
						const auto game_node =
							players_hand.active_is_busted() ? GameOverPlayerBusts : PlayersRound;

						history.emplace_back(game_node, players_hand, current_state.dealer_hand(), deck);
						return history.back();
					} else if (play == Stay) {
						history.emplace_back(DealersRound, current_state.players_hand(),
																 current_state.dealer_hand(), current_state.deck());
						play_dealer_turn();
						return history.back();
					} else if (play == Play::Split) {
						if (current_state.can_split(m_config.allow_resplit_aces)) {
							auto deck = current_state.deck();
							auto players_hand = current_state.players_hand();
							const bool is_aces = players_hand.active_cards()[0].rank() == Card::Rank::Ace;

							players_hand.split(deck.deal(1).front(), deck.deal(1).front());

							if (is_aces) {
								// Both hands complete immediately - go to dealer
								history.emplace_back(DealersRound, players_hand, current_state.dealer_hand(), deck);
								play_dealer_turn();
							} else {
								// Start playing hand 0
								history.emplace_back(PlayersSplitRound, players_hand, current_state.dealer_hand(),
																		 deck);
							}
							return history.back();
						}
					}
					break;

				case PlayersSplitRound:
					if (play == Hit) {
						auto deck = current_state.deck();
						auto players_hand = current_state.players_hand();

						players_hand.add_to_active(deck.deal(1).front());

						if (players_hand.active_is_busted()) {
							players_hand.mark_active_complete();
							players_hand.advance_to_next_incomplete();
						}

						if (players_hand.all_busted()) {
							history.emplace_back(GameOverPlayerBusts, players_hand, current_state.dealer_hand(),
																	 deck);
						} else if (players_hand.all_complete()) {
							history.emplace_back(DealersRound, players_hand, current_state.dealer_hand(), deck);
							play_dealer_turn();
						} else {
							history.emplace_back(PlayersSplitRound, players_hand, current_state.dealer_hand(),
																	 deck);
						}
						return history.back();
					} else if (play == Stay) {
						auto players_hand = current_state.players_hand();

						players_hand.mark_active_complete();
						players_hand.advance_to_next_incomplete();

						if (players_hand.all_busted()) {
							history.emplace_back(GameOverPlayerBusts, players_hand, current_state.dealer_hand(),
																	 current_state.deck());
						} else if (players_hand.all_complete()) {
							history.emplace_back(DealersRound, players_hand, current_state.dealer_hand(),
																	 current_state.deck());
							play_dealer_turn();
						} else {
							history.emplace_back(PlayersSplitRound, players_hand, current_state.dealer_hand(),
																	 current_state.deck());
						}
						return history.back();
					} else if (play == Play::Split) {
						if (current_state.can_split(m_config.allow_resplit_aces)) {
							auto deck = current_state.deck();
							auto players_hand = current_state.players_hand();
							const bool is_aces = players_hand.active_cards()[0].rank() == Card::Rank::Ace;

							players_hand.split(deck.deal(1).front(), deck.deal(1).front());

							if (is_aces) {
								if (players_hand.all_busted()) {
									history.emplace_back(GameOverPlayerBusts, players_hand,
																			 current_state.dealer_hand(), deck);
								} else if (players_hand.all_complete()) {
									history.emplace_back(DealersRound, players_hand, current_state.dealer_hand(),
																			 deck);
									play_dealer_turn();
								} else {
									players_hand.advance_to_next_incomplete();
									history.emplace_back(PlayersSplitRound, players_hand, current_state.dealer_hand(),
																			 deck);
								}
							} else {
								history.emplace_back(PlayersSplitRound, players_hand, current_state.dealer_hand(),
																		 deck);
							}
							return history.back();
						}
					}
					break;

				case DealersRound: // handled automatically by play_dealer_turn()
				case GameOverPlayerBusts:
				case GameOverPlayerWins:
				case GameOverDealerBusts:
				case GameOverDealerWins:
				case GameOverDraw: break;
			}
			return current_state;
		}

		void Game::play_dealer_turn()
		{
			using enum GameNode;

			while (history.back().node() == DealersRound) {
				const auto& current_state = history.back();
				const auto& dealer = current_state.dealer_hand();
				const auto hand_value = dealer.value();

				const bool must_hit = hand_value.total < 17 || (hand_value.total == 17 &&
																												hand_value.is_soft && m_config.hit_soft_17);

				if (must_hit) {
					auto deck = current_state.deck();
					auto dealers_hand = dealer;
					dealers_hand.add(deck.deal(1).front());

					const auto new_value = dealers_hand.value();
					const auto game_node = (new_value.total > 21) ? GameOverDealerBusts : DealersRound;

					history.emplace_back(game_node, current_state.players_hand(), dealers_hand, deck);
				} else {
					const auto player_total = current_state.players_hand().active_total();
					const auto dealer_total = hand_value.total;
					const auto game_node = [&]() {
						if (player_total > dealer_total) {
							return GameOverPlayerWins;
						}
						if (dealer_total > player_total) {
							return GameOverDealerWins;
						}
						return GameOverDraw;
					}();
					history.emplace_back(game_node, current_state.players_hand(), current_state.dealer_hand(),
															 current_state.deck());
				}
			}
		}
} // namespace CardGames::BlackJack
