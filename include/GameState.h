#pragma once

#include "GameSettings.h"

#include <array>
#include <cstddef>
#include <string>

struct Player {
    std::string name;
    int life = 40;
};

class GameState {
public:
    static constexpr std::size_t maximumPlayerCount = 4;

    GameState();

    [[nodiscard]]
    const Player& getPlayer(std::size_t index) const;

    [[nodiscard]]
    std::size_t getPlayerCount() const;

    [[nodiscard]]
    const GameSettings& getSettings() const;

    void changeLife(std::size_t playerIndex, int amount);

    void setPlayerMode(PlayerMode mode);
    void setMultiplayerStartingLife(int life);
    void setTwoPlayerStartingLife(int life);

    void reset();

    void changeCommanderDamage(
        std::size_t attackerIndex,
        std::size_t defenderIndex,
        int amount
    );

    [[nodiscard]]
    int getCommanderDamage(
        std::size_t attackerIndex,
        std::size_t defenderIndex
    ) const;

private:
    GameSettings settings_;

    std::array<Player, maximumPlayerCount> players_;

    // Erster Index: angreifender Commander
    // Zweiter Index: Spieler, der Schaden erhalten hat
    std::array<
        std::array<int, maximumPlayerCount>,
        maximumPlayerCount
    > commanderDamage_{};

    void resetCommanderDamage();
};