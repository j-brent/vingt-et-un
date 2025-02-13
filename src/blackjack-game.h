#pragma once

#include "CompileTimeChecks.h"
#include "deck.h"

namespace CardGames
{
	namespace BlackJack
	{
		enum class GameNode {
			Ready,
			PlayersRound,
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
			GameState(const GameNode& node, const std::vector<Card>& players_hand,
								const std::vector<Card>& dealers_hand, const Deck& deck)
				: m_node{node}
				, m_players_hand{players_hand}
				, m_dealers_hand{dealers_hand}
				, m_deck{deck}
			{
			}
			GameNode node() const { return m_node; };
			const std::vector<Card>& players_hand() const { return m_players_hand; };
			const std::vector<Card>& dealers_hand() const { return m_dealers_hand; };
			const Deck& deck() const { return m_deck; };

		private:
			GameNode m_node = GameNode::Ready;
			std::vector<Card> m_players_hand = {};
			std::vector<Card> m_dealers_hand = {};
			Deck m_deck = {shuffle(Deck{})};
		};

		inline bool operator==(const GameState& lhs, const GameState& rhs)
		{
			const auto& lhs_players_hand = lhs.players_hand();
			const auto& rhs_players_hand = rhs.players_hand();
			const auto& lhs_dealers_hand = lhs.dealers_hand();
			const auto& rhs_dealers_hand = rhs.dealers_hand();

			return lhs.node() == rhs.node() && lhs_players_hand.size() == rhs_players_hand.size() &&
						 std::equal(cbegin(lhs_players_hand), cend(lhs_players_hand),
												cbegin(rhs_players_hand)) &&
						 lhs_dealers_hand.size() == rhs_dealers_hand.size() &&
						 std::equal(cbegin(lhs_dealers_hand), cend(lhs_dealers_hand),
												cbegin(rhs_dealers_hand)) &&
						 lhs.deck() == rhs.deck();
		}

		static_assert(is_regular<GameState>::value,
									"User-defined type GameState is not a regular type.");

    class Game
    {
    public:
      enum class Play { Deal, Hit, Stay };
			const GameState& next(Play play);
			const GameState& state() const { return history.back(); }

		private:
      std::vector<GameState> history = {GameState{}};
		};

	} // namespace BlackJack
} // namespace CardGames