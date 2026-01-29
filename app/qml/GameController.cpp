#include "GameController.h"

#include <QVariantMap>

GameController::GameController(QObject* parent)
	: QObject(parent)
{
}

QVariantMap GameController::cardToVariant(const Card& card) const
{
	static const QMap<Card::Suit, QString> suitNames = {{Card::Suit::Clubs, "clubs"},
																											{Card::Suit::Diamonds, "diamonds"},
																											{Card::Suit::Hearts, "hearts"},
																											{Card::Suit::Spades, "spades"}};
	static const QMap<Card::Rank, QString> rankNames = {
		{Card::Rank::Two, "2"},		{Card::Rank::Three, "3"}, {Card::Rank::Four, "4"},
		{Card::Rank::Five, "5"},	{Card::Rank::Six, "6"},		{Card::Rank::Seven, "7"},
		{Card::Rank::Eight, "8"}, {Card::Rank::Nine, "9"},	{Card::Rank::Ten, "10"},
		{Card::Rank::Jack, "J"},	{Card::Rank::Queen, "Q"}, {Card::Rank::King, "K"},
		{Card::Rank::Ace, "A"}};

	return {{"suit", suitNames[card.suit]}, {"rank", rankNames[card.rank]}};
}

QString GameController::gameState() const
{
	using enum CardGames::BlackJack::GameNode;
	switch (m_game.state().node()) {
		case Ready: return "ready";
		case PlayersRound: return "playersTurn";
		case PlayersSplitRound: return "playersSplitTurn";
		case DealersRound: return "dealersTurn";
		case GameOverPlayerWins: return "playerWins";
		case GameOverPlayerBusts: return "playerBusts";
		case GameOverDealerWins: return "dealerWins";
		case GameOverDealerBusts: return "dealerBusts";
		case GameOverDraw: return "draw";
	}
	return "unknown";
}

QVariantList GameController::playerHand() const
{
	QVariantList result;
	for (const auto& card : m_game.state().players_hand().active_cards()) {
		result.append(cardToVariant(card));
	}
	return result;
}

QVariantList GameController::dealerHand() const
{
	QVariantList result;
	for (const auto& card : m_game.state().dealer_hand().cards()) {
		result.append(cardToVariant(card));
	}
	return result;
}

int GameController::playerScore() const
{
	return m_game.state().players_hand().active_total();
}

int GameController::dealerScore() const
{
	return m_game.state().dealer_hand().total();
}

bool GameController::canDeal() const
{
	return m_game.state().node() == CardGames::BlackJack::GameNode::Ready;
}

bool GameController::canHit() const
{
	using enum CardGames::BlackJack::GameNode;
	auto node = m_game.state().node();
	return node == PlayersRound || node == PlayersSplitRound;
}

bool GameController::canStay() const
{
	using enum CardGames::BlackJack::GameNode;
	auto node = m_game.state().node();
	return node == PlayersRound || node == PlayersSplitRound;
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
	return static_cast<int>(m_game.state().players_hand().hand_count());
}

int GameController::activeHandIndex() const
{
	return static_cast<int>(m_game.state().players_hand().active_index());
}

QVariantList GameController::playerHands() const
{
	QVariantList result;
	const auto& allHands = m_game.state().players_hand().all_hands();
	const auto activeIdx = m_game.state().players_hand().active_index();

	for (size_t i = 0; i < allHands.size(); ++i) {
		const auto& hand = allHands[i];
		QVariantList cards;
		for (const auto& card : hand.cards) {
			cards.append(cardToVariant(card));
		}

		const auto handValue = CardGames::BlackJack::calculate_hand_value(hand.cards);
		QVariantMap handData;
		handData["cards"] = cards;
		handData["score"] = handValue.total;
		handData["isComplete"] = hand.is_complete;
		handData["isActive"] = (i == activeIdx);
		handData["isBusted"] = handValue.total > 21;

		result.append(handData);
	}
	return result;
}

bool GameController::isGameOver() const
{
	using enum CardGames::BlackJack::GameNode;
	auto node = m_game.state().node();
	return node == GameOverPlayerWins || node == GameOverPlayerBusts || node == GameOverDealerWins ||
				 node == GameOverDealerBusts || node == GameOverDraw;
}

QString GameController::resultMessage() const
{
	using enum CardGames::BlackJack::GameNode;
	switch (m_game.state().node()) {
		case GameOverPlayerWins: return "You win!";
		case GameOverPlayerBusts: return "Bust! You lose.";
		case GameOverDealerWins: return "Dealer wins.";
		case GameOverDealerBusts: return "Dealer busts! You win!";
		case GameOverDraw: return "Push (Draw).";
		default: return "";
	}
}

void GameController::deal()
{
	if (!canDeal())
		return;
	m_game.next(CardGames::BlackJack::Game::Play::Deal);
	emit handsChanged();
	emit gameStateChanged();
}

void GameController::hit()
{
	if (!canHit())
		return;
	m_game.next(CardGames::BlackJack::Game::Play::Hit);
	emit handsChanged();
	emit gameStateChanged();
}

void GameController::stay()
{
	if (!canStay())
		return;
	// Dealer auto-plays when player stays (or all split hands complete)
	m_game.next(CardGames::BlackJack::Game::Play::Stay);
	emit handsChanged();
	emit gameStateChanged();
}

void GameController::split()
{
	if (!canSplit())
		return;
	m_game.next(CardGames::BlackJack::Game::Play::Split);
	emit handsChanged();
	emit gameStateChanged();
}

void GameController::newGame(const QString& deckName)
{
	if (!deckName.isEmpty()) {
		auto deck = CardGames::BlackJack::get_test_deck(deckName.toStdString());
		if (deck) {
			m_game = CardGames::BlackJack::Game{{.initial_deck = deck}};
			emit handsChanged();
			emit gameStateChanged();
			return;
		}
	}
	m_game = CardGames::BlackJack::Game{};
	emit handsChanged();
	emit gameStateChanged();
}
