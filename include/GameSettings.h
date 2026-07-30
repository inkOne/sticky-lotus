#pragma once

#include <cstddef>

enum class PlayerMode {
    TwoPlayers = 2,
    FourPlayers = 4
};

struct GameSettings {
    PlayerMode playerMode = PlayerMode::FourPlayers;

    int multiplayerStartingLife = 40;
    int twoPlayerStartingLife = 20;

    [[nodiscard]]
    std::size_t playerCount() const
    {
        return static_cast<std::size_t>(playerMode);
    }

    [[nodiscard]]
    int startingLife() const
    {
        return playerMode == PlayerMode::TwoPlayers
            ? twoPlayerStartingLife
            : multiplayerStartingLife;
    }
};