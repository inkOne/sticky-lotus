#include "sticky_lotus/GameState.h"

#include <algorithm>
#include <stdexcept>

GameState::GameState()
{
    players_ = {{
        {"Player 1", 40},
        {"Player 2", 40},
        {"Player 3", 40},
        {"Player 4", 40}
    }};

    reset();
}

const Player& GameState::getPlayer(const std::size_t index) const
{
    if (index >= getPlayerCount()) {
        throw std::out_of_range("Invalid player index");
    }

    return players_[index];
}

std::size_t GameState::getPlayerCount() const
{
    return settings_.playerCount();
}

const GameSettings& GameState::getSettings() const
{
    return settings_;
}

void GameState::changeLife(
    const std::size_t playerIndex,
    const int amount
)
{
    if (playerIndex >= getPlayerCount()) {
        return;
    }

    players_[playerIndex].life += amount;
}

void GameState::setPlayerMode(const PlayerMode mode)
{
    settings_.playerMode = mode;
    reset();
}

void GameState::setMultiplayerStartingLife(const int life)
{
    if (life <= 0) {
        return;
    }

    settings_.multiplayerStartingLife = life;

    if (settings_.playerMode == PlayerMode::FourPlayers) {
        reset();
    }
}

void GameState::setTwoPlayerStartingLife(const int life)
{
    if (life <= 0) {
        return;
    }

    settings_.twoPlayerStartingLife = life;

    if (settings_.playerMode == PlayerMode::TwoPlayers) {
        reset();
    }
}

void GameState::reset()
{
    const int startingLife = settings_.startingLife();

    for (Player& player : players_) {
        player.life = startingLife;
    }

    resetCommanderDamage();
}

void GameState::changeCommanderDamage(
    const std::size_t attackerIndex,
    const std::size_t defenderIndex,
    const int amount
)
{
    if (
        attackerIndex >= getPlayerCount() ||
        defenderIndex >= getPlayerCount() ||
        attackerIndex == defenderIndex
    ) {
        return;
    }

    int& damage = commanderDamage_[attackerIndex][defenderIndex];

    const int previousDamage = damage;
    damage = std::max(0, damage + amount);

    const int actualChange = damage - previousDamage;

    // Commander Damage verändert gleichzeitig die Lebenspunkte.
    players_[defenderIndex].life -= actualChange;
}

int GameState::getCommanderDamage(
    const std::size_t attackerIndex,
    const std::size_t defenderIndex
) const
{
    if (
        attackerIndex >= getPlayerCount() ||
        defenderIndex >= getPlayerCount()
    ) {
        return 0;
    }

    return commanderDamage_[attackerIndex][defenderIndex];
}

void GameState::resetCommanderDamage()
{
    for (auto& attackerValues : commanderDamage_) {
        attackerValues.fill(0);
    }
}