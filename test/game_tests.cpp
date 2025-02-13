#include <catch2/catch_test_macros.hpp>
#include <blackjack-game.h>

using namespace CardGames::BlackJack;

SCENARIO("Game states")
{
  GIVEN("A default GameState")
  {
    const auto gs = GameState{};

    THEN("The defaults should be as expected")
    {
      CHECK(gs.node() == GameNode::Ready);
      CHECK(gs.players_hand().empty());
      CHECK(gs.dealers_hand().empty());
      CHECK(gs.deck().cards().size() == 52);
    }
    
    GIVEN("A default Game")
    {
      auto g = Game{};

      THEN("The GameState defaults should be as expected")
      {
        const auto gs = g.state();
        CHECK(gs.node() == GameNode::Ready);
        CHECK(gs.players_hand().empty());
        CHECK(gs.dealers_hand().empty());
        CHECK(gs.deck().cards().size() == 52);
      }

      WHEN("Dealing the first round")
      {
        const auto gs = g.next(Game::Play::Deal);

        THEN("...as expected")
        {
          CHECK(gs.node() == GameNode::PlayersRound);
          CHECK(gs.players_hand().size() == 2);
          CHECK(gs.dealers_hand().size() == 2);
          CHECK(gs.deck().cards().size() == 48);
        }
      }
    }
  }
}
#if 0
SCENARIO("Slicing ranks and suits")
{
  GIVEN("Some cards")
  {
    const auto cards = std::vector<Card>{
      {Rank::Ace, Suit::Clubs}, 
      {Rank::King, Suit::Hearts}, 
      {Rank::Queen, Suit::Diamonds}
    };

    WHEN("Slicing the suits")
    {
      THEN("We get the suits")
      {
        CHECK(slice_suits(cards) == std::vector<Suit>{Suit::Clubs, Suit::Hearts, Suit::Diamonds});
      }
    }
    
    WHEN("Slicing the face value")
    {
      THEN("We get the ranks")
      {
        CHECK(slice_face_values(cards) == std::vector<Rank>{Rank::Ace, Rank::King, Rank::Queen});
      }
    }
  }
}
#endif