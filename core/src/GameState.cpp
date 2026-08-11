#include "sticky_lotus/GameState.h"
#include "sticky_lotus/DeathMessageGenerator.h"

#include <algorithm>

namespace sticky_lotus
{
    namespace
    {
        Player createPlayer(
            const char* name
        )
        {
            return Player{
                .name = name,
                .life = 40,
                .poison = 0,
                .deathMessage = "",
                .lastEliminationReason =
                EliminationReason::None
            };
        }
    } // namespace

    GameState::GameState()
        : players_{
            createPlayer("Player 1"),
            createPlayer("Player 2"),
            createPlayer("Player 3"),
            createPlayer("Player 4")
        }
    {
    }

    const Player& GameState::getPlayer(
        const std::size_t index
    ) const
    {
        /*
         * Im Embedded-Build sind C++-Exceptions deaktiviert.
         * Als defensive Rückfalllösung geben wir Spieler 1 zurück.
         */
        if (index >= getPlayerCount())
        {
            return players_[0];
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
        if (playerIndex >= getPlayerCount())
        {
            return;
        }

        const EliminationReason eliminationReason =
            getEliminationReason(
                playerIndex
            );

        /*
         * Ein Spieler, der durch Poison oder Commander Damage
         * ausgeschieden ist, kann nicht durch Life Gain
         * zurück ins Spiel gebracht werden.
         *
         * Der entsprechende Eliminierungsgrund muss zuerst
         * selbst beseitigt werden.
         */
        if (
            eliminationReason ==
            EliminationReason::Poison ||
            eliminationReason ==
            EliminationReason::CommanderDamage
        )
        {
            return;
        }

        /*
         * Bei 0 Leben ist weiteres Life Loss bedeutungslos.
         *
         * Positive Änderungen sind dagegen erlaubt,
         * damit ein durch Life ausgeschiedener Spieler
         * wieder ins Spiel zurückkehren kann.
         */
        if (
            players_[playerIndex].life == 0 &&
            amount <= 0
        )
        {
            return;
        }

        players_[playerIndex].life =
            std::max(
                0,
                players_[playerIndex].life + amount
            );

        updateEliminationState(
            playerIndex
        );
    }
    void GameState::setPlayerMode(
        const PlayerMode mode
    )
    {
        settings_.playerMode =
            mode;

        reset();
    }
    void GameState::setMultiplayerStartingLife(
        const int life
    )
    {
        if (life <= 0)
        {
            return;
        }

        settings_.multiplayerStartingLife =
            life;

        if (
            settings_.playerMode ==
            PlayerMode::FourPlayers
        )
        {
            reset();
        }
    }

    void GameState::setTwoPlayerStartingLife(
        const int life
    )
    {
        if (life <= 0)
        {
            return;
        }

        settings_.twoPlayerStartingLife =
            life;

        if (
            settings_.playerMode ==
            PlayerMode::TwoPlayers
        )
        {
            reset();
        }
    }

    void GameState::reset()
    {
        const int startingLife =
            settings_.startingLife();

        for (Player& player : players_)
        {
            player.life =
                startingLife;

            player.poison =
                0;

            player.deathMessage.clear();

            player.lastEliminationReason =
                EliminationReason::None;
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
        )
        {
            return;
        }

        int& damage =
            commanderDamage_[
                attackerIndex
            ][
                defenderIndex
            ];

        const int previousDamage =
            damage;

        damage =
            std::max(
                0,
                damage + amount
            );

        const int actualChange =
            damage - previousDamage;

        players_[defenderIndex].life =
            std::max(
                0,
                players_[defenderIndex].life -
                actualChange
            );

        updateEliminationState(
            defenderIndex
        );
    }

    int GameState::getCommanderDamage(
        const std::size_t attackerIndex,
        const std::size_t defenderIndex
    ) const
    {
        if (
            attackerIndex >= getPlayerCount() ||
            defenderIndex >= getPlayerCount()
        )
        {
            return 0;
        }

        return commanderDamage_[
            attackerIndex
        ][
            defenderIndex
        ];
    }

    void GameState::resetCommanderDamage()
    {
        for (
            auto& attackerValues :
            commanderDamage_
        )
        {
            attackerValues.fill(0);
        }
    }

    EliminationReason GameState::getEliminationReason(
        const std::size_t playerIndex
    ) const
    {
        if (playerIndex >= getPlayerCount())
        {
            return EliminationReason::None;
        }

        for (
            std::size_t attackingPlayer = 0;
            attackingPlayer < getPlayerCount();
            ++attackingPlayer
        )
        {
            if (attackingPlayer == playerIndex)
            {
                continue;
            }

            if (
                commanderDamage_[
                    attackingPlayer
                ][
                    playerIndex
                ] >= 21
            )
            {
                return EliminationReason::CommanderDamage;
            }
        }

        if (
            players_[playerIndex].poison >= 10
        )
        {
            return EliminationReason::Poison;
        }

        if (
            players_[playerIndex].life == 0
        )
        {
            return EliminationReason::Life;
        }

        return EliminationReason::None;
    }

    bool GameState::isPlayerEliminated(
        const std::size_t playerIndex
    ) const
    {
        return
            getEliminationReason(
                playerIndex
            ) !=
            EliminationReason::None;
    }

    void GameState::changePoison(
        const std::size_t playerIndex,
        const int amount
    )
    {
        if (playerIndex >= getPlayerCount())
        {
            return;
        }

        players_[playerIndex].poison =
            std::max(
                0,
                players_[playerIndex].poison +
                amount
            );

        updateEliminationState(
            playerIndex
        );
    }

    int GameState::getPoison(
        const std::size_t playerIndex
    ) const
    {
        if (playerIndex >= getPlayerCount())
        {
            return 0;
        }

        return players_[playerIndex].poison;
    }

    GameSnapshot GameState::createSnapshot() const
    {
        GameSnapshot snapshot{};

        snapshot.magic =
            GameSnapshot::validMagic;

        snapshot.settings =
            settings_;

        for (
            std::size_t playerIndex = 0;
            playerIndex < maximumPlayerCount;
            ++playerIndex
        )
        {
            snapshot.life[playerIndex] =
                players_[playerIndex].life;

            snapshot.poison[playerIndex] =
                players_[playerIndex].poison;

            snapshot.eliminationReasons[playerIndex] =
                players_[playerIndex]
                .lastEliminationReason;
        }

        snapshot.commanderDamage =
            commanderDamage_;

        return snapshot;
    }

    bool GameState::restoreSnapshot(
        const GameSnapshot& snapshot
    )
    {
        if (
            snapshot.magic !=
            GameSnapshot::validMagic
        )
        {
            return false;
        }

        settings_ =
            snapshot.settings;

        for (
            std::size_t playerIndex = 0;
            playerIndex < maximumPlayerCount;
            ++playerIndex
        )
        {
            players_[playerIndex].life =
                snapshot.life[playerIndex];

            players_[playerIndex].poison =
                snapshot.poison[playerIndex];

            players_[playerIndex]
                .lastEliminationReason =
                snapshot.eliminationReasons[
                    playerIndex
                ];

            /*
             * Todestexte werden nicht persistent gespeichert.
             * Nach Restore bei ausgeschiedenen Spielern kann
             * wieder einer erzeugt werden.
             */
            players_[playerIndex]
                .deathMessage
                .clear();

            updateEliminationState(
                playerIndex
            );
        }

        commanderDamage_ =
            snapshot.commanderDamage;

        return true;
    }


    void GameState::updateEliminationState(
        const std::size_t playerIndex
    )
    {
        if (playerIndex >= getPlayerCount())
        {
            return;
        }

        Player& player =
            players_[playerIndex];

        const EliminationReason currentReason =
            getEliminationReason(
                playerIndex
            );

        /*
         * Spieler ist wieder aktiv.
         */
        if (
            currentReason ==
            EliminationReason::None
        )
        {
            player.lastEliminationReason =
                EliminationReason::None;

            player.deathMessage.clear();

            return;
        }

        /*
         * Nur beim Übergang von aktiv zu ausgeschieden
         * wird eine neue Nachricht ausgewählt.
         */
        if (
            player.lastEliminationReason ==
            EliminationReason::None
        )
        {
            player.deathMessage =
                DeathMessageGenerator::randomMessage(
                    currentReason
                );
        }

        player.lastEliminationReason =
            currentReason;
    }
} // namespace sticky_lotus
