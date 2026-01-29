#pragma once

#include "deck.h"

#include <algorithm>
#include <concepts>
#include <optional>
#include <span>

namespace CardGames
{
	namespace BlackJack
	{
		/// Result of calculating a hand's blackjack value
		struct HandValue {
			int total;					///< Final calculated total (soft aces adjusted)
			bool is_soft;				///< True if an Ace is currently counted as 11
			int soft_ace_count; ///< Number of Aces counted as 11

			bool operator==(const HandValue&) const = default;
		};

		/// Calculate hand value with soft ace logic
		/// Aces count as 11 unless that would cause a bust, then they count as 1
		HandValue calculate_hand_value(std::span<const Card> hand);

		/// Dealer's hand - simple container, no split logic
		class DealersHand
		{
		public:
			DealersHand() = default;
			explicit DealersHand(const std::vector<Card>& cards)
				: m_cards{cards}
			{
			}

			const std::vector<Card>& cards() const { return m_cards; }
			void add(const Card& card) { m_cards.push_back(card); }

			HandValue value() const { return calculate_hand_value(m_cards); }
			int total() const { return value().total; }
			bool is_soft() const { return value().is_soft; }

			bool operator==(const DealersHand&) const = default;

		private:
			std::vector<Card> m_cards;
		};

		/// Player's hand container managing multiple hands (for splits)
		class PlayersHand
		{
		public:
			struct SingleHand {
				std::vector<Card> cards;
				bool is_from_split = false;
				bool is_from_split_aces = false;
				bool is_complete = false;
				int split_count = 0;

				bool operator==(const SingleHand&) const = default;
			};

			PlayersHand() = default;
			explicit PlayersHand(const std::vector<Card>& cards) { m_hands[0].cards = cards; }

			// Active hand operations
			const std::vector<Card>& active_cards() const { return m_hands[m_active_index].cards; }
			void add_to_active(const Card& card) { m_hands[m_active_index].cards.push_back(card); }
			int active_total() const { return calculate_hand_value(active_cards()).total; }
			bool active_is_busted() const { return active_total() > 21; }

			// Split operations
			bool can_split(bool allow_resplit_aces = false) const
			{
				const auto& hand = m_hands[m_active_index];
				return hand.cards.size() == 2 && hand.cards[0].rank() == hand.cards[1].rank() &&
							 (allow_resplit_aces || !hand.is_from_split_aces) && hand.split_count < 3;
			}

			void split(const Card& first_new_card, const Card& second_new_card)
			{
				auto& current = m_hands[m_active_index];
				const bool is_aces = current.cards[0].rank() == Card::Rank::Ace;
				const int new_split_count = current.split_count + 1;

				// Create second hand from split
				SingleHand second_hand;
				second_hand.cards = {current.cards[1], second_new_card};
				second_hand.is_from_split = true;
				second_hand.is_from_split_aces = is_aces;
				second_hand.is_complete = is_aces;
				second_hand.split_count = new_split_count;

				// Update current hand
				current.cards = {current.cards[0], first_new_card};
				current.is_from_split = true;
				current.is_from_split_aces = is_aces;
				current.is_complete = is_aces;
				current.split_count = new_split_count;

				// Insert second hand after current
				m_hands.insert(m_hands.begin() + static_cast<long>(m_active_index) + 1, second_hand);
			}

			// Completion tracking
			void mark_active_complete() { m_hands[m_active_index].is_complete = true; }

			bool advance_to_next_incomplete()
			{
				for (size_t i = m_active_index + 1; i < m_hands.size(); ++i) {
					if (!m_hands[i].is_complete) {
						m_active_index = i;
						return true;
					}
				}
				return false;
			}

			bool all_complete() const
			{
				return std::ranges::all_of(m_hands, [](const SingleHand& h) { return h.is_complete; });
			}

			bool all_busted() const
			{
				return std::ranges::all_of(m_hands, [](const SingleHand& h) {
					return h.is_complete && calculate_hand_value(h.cards).total > 21;
				});
			}

			// Multi-hand queries
			size_t hand_count() const { return m_hands.size(); }
			size_t active_index() const { return m_active_index; }
			const std::vector<SingleHand>& all_hands() const { return m_hands; }

			bool operator==(const PlayersHand&) const = default;

		private:
			std::vector<SingleHand> m_hands = {SingleHand{}};
			size_t m_active_index = 0;
		};

		enum class GameNode {
			Ready,
			PlayersRound,
			PlayersSplitRound, ///< Playing split hands sequentially
			DealersRound,
			GameOverPlayerBusts,
			GameOverPlayerWins,
			GameOverDealerBusts,
			GameOverDealerWins,
			GameOverDraw,
		};

		struct GameState {
		public:
			GameState() = default;

			GameState(GameNode node, PlayersHand players_hand, DealersHand dealers_hand, Deck deck)
				: m_node{node}
				, m_players_hand{std::move(players_hand)}
				, m_dealers_hand{std::move(dealers_hand)}
				, m_deck{std::move(deck)}
			{
			}

			GameNode node() const { return m_node; }
			const PlayersHand& players_hand() const { return m_players_hand; }
			const DealersHand& dealer_hand() const { return m_dealers_hand; }
			const Deck& deck() const { return m_deck; }

			bool can_split(bool allow_resplit_aces = false) const
			{
				return m_players_hand.can_split(allow_resplit_aces);
			}

			bool operator==(const GameState&) const = default;

		private:
			GameNode m_node = GameNode::Ready;
			PlayersHand m_players_hand;
			DealersHand m_dealers_hand;
			Deck m_deck = {shuffle(Deck{})};
		};

		static_assert(std::regular<GameState>);

		/// Configuration for blackjack game rules
		struct BlackjackConfig {
			bool hit_soft_17 = true;				 ///< If true, dealer hits on soft 17 (standard casino rule)
			bool allow_resplit_aces = false; ///< If true, player can resplit aces (RSA); default is NRSA
			std::optional<Deck> initial_deck = std::nullopt; ///< For deterministic testing
		};

		class Game
		{
		public:
			enum class Play { Deal, Hit, Stay, Split };

			explicit Game(BlackjackConfig config = {})
				: m_config{config}
				, history{config.initial_deck
										? std::vector<GameState>{GameState{GameNode::Ready, PlayersHand{},
																											 DealersHand{}, *config.initial_deck}}
										: std::vector<GameState>{GameState{}}}
			{
			}

			const GameState& next(Play play);
			const GameState& state() const { return history.back(); }

		private:
			/// Plays the dealer's turn automatically according to game rules
			/// Called after player stays; appends states to history until game over
			void play_dealer_turn();

			BlackjackConfig m_config;
			std::vector<GameState> history;
		};

		int add_em_up(std::span<const Card> hand);

	} // namespace BlackJack
} // namespace CardGames
