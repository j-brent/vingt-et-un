#pragma once

#include <QObject>
#include <QVariantList>
#include <QString>
#include <QtQml/qqmlregistration.h>

#include "blackjack-game.h"

class GameController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString gameState READ gameState NOTIFY gameStateChanged)
    Q_PROPERTY(QVariantList playerHand READ playerHand NOTIFY handsChanged)
    Q_PROPERTY(QVariantList dealerHand READ dealerHand NOTIFY handsChanged)
    Q_PROPERTY(int playerScore READ playerScore NOTIFY handsChanged)
    Q_PROPERTY(int dealerScore READ dealerScore NOTIFY handsChanged)
    Q_PROPERTY(bool canDeal READ canDeal NOTIFY gameStateChanged)
    Q_PROPERTY(bool canHit READ canHit NOTIFY gameStateChanged)
    Q_PROPERTY(bool canStay READ canStay NOTIFY gameStateChanged)
    Q_PROPERTY(bool isGameOver READ isGameOver NOTIFY gameStateChanged)
    Q_PROPERTY(QString resultMessage READ resultMessage NOTIFY gameStateChanged)

public:
    explicit GameController(QObject* parent = nullptr);

    QString gameState() const;
    QVariantList playerHand() const;
    QVariantList dealerHand() const;
    int playerScore() const;
    int dealerScore() const;
    bool canDeal() const;
    bool canHit() const;
    bool canStay() const;
    bool isGameOver() const;
    QString resultMessage() const;

public slots:
    void deal();
    void hit();
    void stay();
    void newGame();

signals:
    void gameStateChanged();
    void handsChanged();

private:
    CardGames::BlackJack::Game m_game;

    QVariantMap cardToVariant(const Card& card) const;
    void runDealerTurn();
};
