#include "sticky_lotus/GameState.h"
#include "sticky_lotus/DeathMessageGenerator.h"

#include <algorithm>
#include <random>

namespace sticky_lotus
{
    namespace
    {
        std::mt19937 rng{std::random_device{}()};

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

        const int lifeBefore =
            players_[playerIndex].life;

        players_[playerIndex].life =
            std::max(
                0,
                players_[playerIndex].life + amount
            );

        updateEliminationState(
            playerIndex
        );

        const int appliedChange =
            players_[playerIndex].life - lifeBefore;

        if (appliedChange != 0)
        {
            lifeDelta_[playerIndex] +=
                appliedChange;

            hasLifeDeltaHighlight_[playerIndex] =
                lifeDelta_[playerIndex] != 0;

            lifeDeltaTimestamp_[playerIndex] =
                std::chrono::steady_clock::now();
        }

        /*
         * Ein soeben ausgeschiedener Spieler zeigt keine
         * Lebenspunkte mehr an - die Anzeige wäre also ohnehin
         * unsichtbar und könnte bei Rückkehr ins Spiel veraltet
         * wieder auftauchen.
         */
        if (isPlayerEliminated(playerIndex))
        {
            lifeDelta_[playerIndex] = 0;
            hasLifeDeltaHighlight_[playerIndex] = false;
        }
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
            settings_.playerMode !=
            PlayerMode::TwoPlayers
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

    void GameState::setSoundEnabled(
        const bool enabled
    )
    {
        settings_.soundEnabled =
            enabled;
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

        clearStartingPlayerHighlight();

        lifeDelta_.fill(0);
        hasLifeDeltaHighlight_.fill(false);

        poisonDelta_.fill(0);
        hasPoisonDeltaHighlight_.fill(false);
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

        const int poisonBefore =
            players_[playerIndex].poison;

        players_[playerIndex].poison =
            std::max(
                0,
                players_[playerIndex].poison +
                amount
            );

        updateEliminationState(
            playerIndex
        );

        const int appliedChange =
            players_[playerIndex].poison - poisonBefore;

        if (appliedChange != 0)
        {
            poisonDelta_[playerIndex] +=
                appliedChange;

            hasPoisonDeltaHighlight_[playerIndex] =
                poisonDelta_[playerIndex] != 0;

            poisonDeltaTimestamp_[playerIndex] =
                std::chrono::steady_clock::now();
        }

        if (isPlayerEliminated(playerIndex))
        {
            poisonDelta_[playerIndex] = 0;
            hasPoisonDeltaHighlight_[playerIndex] = false;
        }
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

    bool GameState::isValidDie(const int sides)
    {
        return sides == 2 || sides == 4 || sides == 6 ||
               sides == 8 || sides == 10 || sides == 12 ||
               sides == 20;
    }

    int GameState::rollDie(const int sides)
    {
        if (!isValidDie(sides))
        {
            return 0;
        }

        return std::uniform_int_distribution<int>(
            1,
            sides
        )(rng);
    }

    void GameState::rollStartingPlayer()
    {
        std::uniform_int_distribution<std::size_t> dist(
            0,
            getPlayerCount() - 1
        );

        startingPlayerHighlight_ =
            dist(rng);

        hasStartingPlayerHighlight_ =
            true;
    }

    bool GameState::hasStartingPlayerHighlight() const
    {
        return hasStartingPlayerHighlight_;
    }

    std::size_t GameState::getStartingPlayerHighlight() const
    {
        return startingPlayerHighlight_;
    }

    void GameState::clearStartingPlayerHighlight()
    {
        hasStartingPlayerHighlight_ =
            false;
    }

    bool GameState::hasLifeDeltaHighlight(
        const std::size_t playerIndex
    ) const
    {
        if (playerIndex >= maximumPlayerCount)
        {
            return false;
        }

        return hasLifeDeltaHighlight_[playerIndex];
    }

    int GameState::getLifeDelta(
        const std::size_t playerIndex
    ) const
    {
        if (playerIndex >= maximumPlayerCount)
        {
            return 0;
        }

        return lifeDelta_[playerIndex];
    }

    std::chrono::steady_clock::time_point
    GameState::getLifeDeltaTimestamp(
        const std::size_t playerIndex
    ) const
    {
        if (playerIndex >= maximumPlayerCount)
        {
            return {};
        }

        return lifeDeltaTimestamp_[playerIndex];
    }

    void GameState::clearLifeDeltaHighlight(
        const std::size_t playerIndex
    )
    {
        if (playerIndex >= maximumPlayerCount)
        {
            return;
        }

        lifeDelta_[playerIndex] = 0;
        hasLifeDeltaHighlight_[playerIndex] = false;
    }

    bool GameState::hasPoisonDeltaHighlight(
        const std::size_t playerIndex
    ) const
    {
        if (playerIndex >= maximumPlayerCount)
        {
            return false;
        }

        return hasPoisonDeltaHighlight_[playerIndex];
    }

    int GameState::getPoisonDelta(
        const std::size_t playerIndex
    ) const
    {
        if (playerIndex >= maximumPlayerCount)
        {
            return 0;
        }

        return poisonDelta_[playerIndex];
    }

    std::chrono::steady_clock::time_point
    GameState::getPoisonDeltaTimestamp(
        const std::size_t playerIndex
    ) const
    {
        if (playerIndex >= maximumPlayerCount)
        {
            return {};
        }

        return poisonDeltaTimestamp_[playerIndex];
    }

    void GameState::clearPoisonDeltaHighlight(
        const std::size_t playerIndex
    )
    {
        if (playerIndex >= maximumPlayerCount)
        {
            return;
        }

        poisonDelta_[playerIndex] = 0;
        hasPoisonDeltaHighlight_[playerIndex] = false;
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
