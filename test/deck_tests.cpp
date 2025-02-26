#include <catch2/catch_test_macros.hpp>
#include <deck.h>

SCENARIO("Basic properties of a deck of cards")
{
  GIVEN("A default Deck")
  {
    const auto d = Deck{};

    THEN("The deck should have 52 cards")
    {
      CHECK(d.cards().size() == 52);
    }

    THEN("It should be possible to iterate through the deck")
    {
      int count = 0;
      for (const auto card : d)
        ++count;

      CHECK(count == 52);
    }

    WHEN("Comparing two default-constructed decks")
    {
      THEN("They should be equal")
      {
        CHECK(d == Deck{});
      }
    }

    WHEN("Comparing a default-constructed deck and a shuffled deck")
    {
      THEN("They should NOT be equal")
      {
        const auto sd = shuffle(d);
        CHECK(d != sd );
      }
    }
  }
}

SCENARIO("Slicing ranks and suits")
{
  using Rank = Card::Rank;
  using Suit = Card::Suit;

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

SCENARIO("Dealing cards in order")
{
  using Rank = Card::Rank;
  using Suit = Card::Suit;

  GIVEN("Some cards in a specific order")
  {
    const std::vector<Card> cards = {
      {Rank::Ace, Suit::Clubs},
      {Rank::Ace, Suit::Hearts},
      {Rank::Ace, Suit::Diamonds},
      {Rank::Ace, Suit::Spades},
      {Rank::Two, Suit::Clubs},
    };
    auto deck = Deck{cards};

    WHEN("Dealing the cards off the top of the deck")
    {
      const auto c0 = deck.deal();
      const auto c1 = deck.deal();
      const auto c2 = deck.deal();
      const auto c3 = deck.deal();
      const auto c4 = deck.deal();

      THEN("The cards come out in the same order")
      {
        CHECK( c0 == cards.at(0) );
        CHECK( c1 == cards.at(1) );
        CHECK( c2 == cards.at(2) );
        CHECK( c3 == cards.at(3) );
        CHECK( c4 == cards.at(4) );
      }
    }
  }
}