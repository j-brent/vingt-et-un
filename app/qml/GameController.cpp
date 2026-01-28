#include "GameController.h"
#include <QVariantMap>

GameController::GameController(QObject* parent)
    : QObject(parent)
{
}

QVariantMap GameController::cardToVariant(const Card& card) const
{
    static const QMap<Card::Suit, QString> suitNames = {
        {Card::Suit::Clubs, "clubs"},
        {Card::Suit::Diamonds, "diamonds"},
        {Card::Suit::Hearts, "hearts"},
        {Card::Suit::Spades, "spades"}
    };
    static const QMap<Card::Rank, QString> rankNames = {
        {Card::Rank::Two, "2"}, {Card::Rank::Three, "3"},
        {Card::Rank::Four, "4"}, {Card::Rank::Five, "5"},
        {Card::Rank::Six, "6"}, {Card::Rank::Seven, "7"},
        {Card::Rank::Eight, "8"}, {Card::Rank::Nine, "9"},
        {Card::Rank::Ten, "10"}, {Card::Rank::Jack, "J"},
        {Card::Rank::Queen, "Q"}, {Card::Rank::King, "K"},
        {Card::Rank::Ace, "A"}
    };

    return {
        {"suit", suitNames[card.suit]},
        {"rank", rankNames[card.rank]}
    };
}

QString GameController::gameState() const
{
    using Node = CardGames::BlackJack::GameNode;
    switch (m_game.state().node()) {
        case Node::Ready: return "ready";
        case Node::PlayersRound: return "playersTurn";
        case Node::PlayersSplitRound: return "playersSplitTurn";
        case Node::DealersRound: return "dealersTurn";
        case Node::GameOverPlayerWins: return "playerWins";
        case Node::GameOverPlayerBusts: return "playerBusts";
        case Node::GameOverDealerWins: return "dealerWins";
        case Node::GameOverDealerBusts: return "dealerBusts";
        case Node::GameOverDraw: return "draw";
    }
    return "unknown";
}

QVariantList GameController::playerHand() const
{
    QVariantList result;
    for (const auto& card : m_game.state().players_hand()) {
        result.append(cardToVariant(card));
    }
    return result;
}

QVariantList GameController::dealerHand() const
{
    QVariantList result;
    for (const auto& card : m_game.state().dealers_hand()) {
        result.append(cardToVariant(card));
    }
    return result;
}

int GameController::playerScore() const
{
    return CardGames::BlackJack::add_em_up(m_game.state().players_hand());
}

int GameController::dealerScore() const
{
    return CardGames::BlackJack::add_em_up(m_game.state().dealers_hand());
}

bool GameController::canDeal() const
{
    return m_game.state().node() == CardGames::BlackJack::GameNode::Ready;
}

bool GameController::canHit() const
{
    using Node = CardGames::BlackJack::GameNode;
    auto node = m_game.state().node();
    return node == Node::PlayersRound || node == Node::PlayersSplitRound;
}

bool GameController::canStay() const
{
    using Node = CardGames::BlackJack::GameNode;
    auto node = m_game.state().node();
    return node == Node::PlayersRound || node == Node::PlayersSplitRound;
}

bool GameController::canSplit() const
{
    return m_game.state().node() == CardGames::BlackJack::GameNode::PlayersRound &&
           m_game.state().can_split();
}

bool GameController::isSplitRound() const
{
    return m_game.state().node() == CardGames::BlackJack::GameNode::PlayersSplitRound;
}

int GameController::handCount() const
{
    return static_cast<int>(m_game.state().player_hands().size());
}

int GameController::activeHandIndex() const
{
    return static_cast<int>(m_game.state().active_hand_index());
}

bool GameController::isGameOver() const
{
    using Node = CardGames::BlackJack::GameNode;
    auto node = m_game.state().node();
    return node == Node::GameOverPlayerWins ||
           node == Node::GameOverPlayerBusts ||
           node == Node::GameOverDealerWins ||
           node == Node::GameOverDealerBusts ||
           node == Node::GameOverDraw;
}

QString GameController::resultMessage() const
{
    using Node = CardGames::BlackJack::GameNode;
    switch (m_game.state().node()) {
        case Node::GameOverPlayerWins: return "You win!";
        case Node::GameOverPlayerBusts: return "Bust! You lose.";
        case Node::GameOverDealerWins: return "Dealer wins.";
        case Node::GameOverDealerBusts: return "Dealer busts! You win!";
        case Node::GameOverDraw: return "Push (Draw).";
        default: return "";
    }
}

void GameController::deal()
{
    if (!canDeal()) return;
    m_game.next(CardGames::BlackJack::Game::Play::Deal);
    emit handsChanged();
    emit gameStateChanged();
}

void GameController::hit()
{
    if (!canHit()) return;
    m_game.next(CardGames::BlackJack::Game::Play::Hit);
    emit handsChanged();
    emit gameStateChanged();
}

void GameController::stay()
{
    if (!canStay()) return;
    // Dealer auto-plays when player stays (or all split hands complete)
    m_game.next(CardGames::BlackJack::Game::Play::Stay);
    emit handsChanged();
    emit gameStateChanged();
}

void GameController::split()
{
    if (!canSplit()) return;
    m_game.next(CardGames::BlackJack::Game::Play::Split);
    emit handsChanged();
    emit gameStateChanged();
}

void GameController::newGame()
{
    m_game = CardGames::BlackJack::Game{};
    emit handsChanged();
    emit gameStateChanged();
}
