#pragma once

#include "CompileTimeChecks.h"
#include "deck.h"

namespace CardGames
{
	namespace BlackJack
	{
		/// Result of calculating a hand's blackjack value
		struct HandValue {
			int total;           ///< Final calculated total (soft aces adjusted)
			bool is_soft;        ///< True if an Ace is currently counted as 11
			int soft_ace_count;  ///< Number of Aces counted as 11
		};

		inline bool operator==(const HandValue& lhs, const HandValue& rhs) {
			return lhs.total == rhs.total &&
			       lhs.is_soft == rhs.is_soft &&
			       lhs.soft_ace_count == rhs.soft_ace_count;
		}

		/// Calculate hand value with soft ace logic
		/// Aces count as 11 unless that would cause a bust, then they count as 1
		HandValue calculate_hand_value(const std::vector<Card>& hand);

		/// A blackjack hand with metadata for split tracking
		class Hand {
		public:
			Hand() = default;
			explicit Hand(const std::vector<Card>& cards)
				: m_cards{cards} {}
			Hand(const std::vector<Card>& cards, bool is_split, bool is_complete)
				: m_cards{cards}, m_is_split_hand{is_split}, m_is_complete{is_complete} {}

			const std::vector<Card>& cards() const { return m_cards; }
			void add(const Card& card) { m_cards.push_back(card); }

			bool is_split_hand() const { return m_is_split_hand; }
			bool is_complete() const { return m_is_complete; }
			void set_complete(bool complete) { m_is_complete = complete; }

			/// Returns true if hand can be split (2 cards of same rank, not already split)
			bool can_split() const {
				return m_cards.size() == 2 &&
				       m_cards[0].rank == m_cards[1].rank &&
				       !m_is_split_hand;
			}

			/// Returns the hand value with soft ace logic
			HandValue value() const { return calculate_hand_value(m_cards); }

			/// Returns true if the hand is soft (has an ace counted as 11)
			bool is_soft() const { return value().is_soft; }

			/// Returns the total value
			int total() const { return value().total; }

			bool operator==(const Hand& other) const {
				return m_cards.size() == other.m_cards.size() &&
				       std::equal(m_cards.begin(), m_cards.end(), other.m_cards.begin()) &&
				       m_is_split_hand == other.m_is_split_hand &&
				       m_is_complete == other.m_is_complete;
			}

		private:
			std::vector<Card> m_cards;
			bool m_is_split_hand = false;
			bool m_is_complete = false;
		};

		enum class GameNode {
			Ready,
			PlayersRound,
			PlayersSplitRound,  ///< Playing split hands sequentially
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

			/// Legacy constructor for backward compatibility
			GameState(const GameNode& node, const std::vector<Card>& players_hand,
								const std::vector<Card>& dealers_hand, const Deck& deck)
				: m_node{node}
				, m_player_hands{Hand{players_hand}}
				, m_dealers_hand{Hand{dealers_hand}}
				, m_deck{deck}
			{
			}

			/// New constructor for split support
			GameState(const GameNode& node, const std::vector<Hand>& player_hands,
								size_t active_index, const Hand& dealers_hand, const Deck& deck)
				: m_node{node}
				, m_player_hands{player_hands}
				, m_active_hand_index{active_index}
				, m_dealers_hand{dealers_hand}
				, m_deck{deck}
			{
			}

			GameNode node() const { return m_node; }

			/// Returns the active player hand's cards (for backward compatibility)
			const std::vector<Card>& players_hand() const {
				static const std::vector<Card> empty_cards;
				return m_player_hands.empty() ? empty_cards : m_player_hands[m_active_hand_index].cards();
			}

			/// Returns the dealer's hand cards (for backward compatibility)
			const std::vector<Card>& dealers_hand() const { return m_dealers_hand.cards(); }

			const Deck& deck() const { return m_deck; }

			/// New accessors for split support
			const std::vector<Hand>& player_hands() const { return m_player_hands; }
			const Hand& active_hand() const { return m_player_hands[m_active_hand_index]; }
			size_t active_hand_index() const { return m_active_hand_index; }
			const Hand& dealer_hand() const { return m_dealers_hand; }

			/// Returns true if the active hand can be split
			bool can_split() const {
				return !m_player_hands.empty() && m_player_hands[m_active_hand_index].can_split();
			}

		private:
			GameNode m_node = GameNode::Ready;
			std::vector<Hand> m_player_hands = {};
			size_t m_active_hand_index = 0;
			Hand m_dealers_hand;
			Deck m_deck = {shuffle(Deck{})};
		};

		inline bool operator==(const GameState& lhs, const GameState& rhs)
		{
			if (lhs.node() != rhs.node()) return false;
			if (lhs.active_hand_index() != rhs.active_hand_index()) return false;
			if (lhs.player_hands().size() != rhs.player_hands().size()) return false;
			if (!(lhs.dealer_hand() == rhs.dealer_hand())) return false;
			if (!(lhs.deck() == rhs.deck())) return false;

			for (size_t i = 0; i < lhs.player_hands().size(); ++i) {
				if (!(lhs.player_hands()[i] == rhs.player_hands()[i])) return false;
			}
			return true;
		}

		static_assert(is_regular<GameState>::value,
									"User-defined type GameState is not a regular type.");

		/// Configuration for dealer behavior
		struct DealerRules {
			bool hit_soft_17 = true;  ///< If true, dealer hits on soft 17 (standard casino rule)
		};

    class Game
    {
    public:
      enum class Play { Deal, Hit, Stay, Split };
			const GameState& next(Play play);
			const GameState& state() const { return history.back(); }

			/// Access dealer rules configuration
			DealerRules& dealer_rules() { return m_dealer_rules; }
			const DealerRules& dealer_rules() const { return m_dealer_rules; }

		private:
			/// Plays the dealer's turn automatically according to dealer rules
			/// Called after player stays; appends states to history until game over
			void play_dealer_turn();

      std::vector<GameState> history = {GameState{}};
			DealerRules m_dealer_rules;
		};

		int add_em_up(const std::vector<Card>& hand);

	} // namespace BlackJack
} // namespace CardGames