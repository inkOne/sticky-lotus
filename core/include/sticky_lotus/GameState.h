#pragma once

#include "sticky_lotus/EliminationReason.h"
#include "sticky_lotus/GameSettings.h"
#include "sticky_lotus/PlayerProfiles.h"

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>

namespace sticky_lotus {

/*
 * Maximale Spielerzahl der Anwendung.
 *
 * Wird sowohl von GameState als auch von GameSnapshot verwendet.
 */
inline constexpr std::size_t maximumPlayerCount = 4;

struct Player
{
    std::string name;
    int life = 40;
    int poison = 0;
    std::string deathMessage;

    EliminationReason lastEliminationReason =
        EliminationReason::None;
};

/*
 * Spielstand für Deep-Sleep.
 */
struct GameSnapshot
{
    static constexpr std::uint32_t validMagic =
        0x534C4F54; // "SLOT"

    std::uint32_t magic = 0;

    GameSettings settings{};

    std::array<
        int,
        maximumPlayerCount
    > life{};

    std::array<
        int,
        maximumPlayerCount
    > poison{};

    std::array<
        EliminationReason,
        maximumPlayerCount
    > eliminationReasons{};

    std::array<
        std::array<
            int,
            maximumPlayerCount
        >,
        maximumPlayerCount
    > commanderDamage{};
};

class GameState
{
public:
    static constexpr std::size_t maximumPlayerCount =
        sticky_lotus::maximumPlayerCount;

    GameState();

    /*
     * Gespeicherte Spielerprofile (Namen, Siege) und ihre Sitzplätze.
     * Teil der Persistenz, aber nicht von GameSnapshot.
     */
    PlayerProfiles profiles;

    [[nodiscard]]
    const Player& getPlayer(
        std::size_t index
    ) const;

    [[nodiscard]]
    std::size_t getPlayerCount() const;

    [[nodiscard]]
    const GameSettings& getSettings() const;

    [[nodiscard]]
    EliminationReason getEliminationReason(
        std::size_t playerIndex
    ) const;

    [[nodiscard]]
    bool isPlayerEliminated(
        std::size_t playerIndex
    ) const;

    void changeLife(
        std::size_t playerIndex,
        int amount
    );

    void setPlayerMode(
        PlayerMode mode
    );

    void setMultiplayerStartingLife(
        int life
    );

    void setTwoPlayerStartingLife(
        int life
    );

    void setSoundEnabled(
        bool enabled
    );

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

    void changePoison(
        std::size_t playerIndex,
        int amount
    );

    [[nodiscard]]
    int getPoison(
        std::size_t playerIndex
    ) const;

    /*
     * Würfelt zufällig einen Startspieler unter den aktuell
     * aktiven Spielern aus und merkt ihn für die Hervorhebung
     * auf dem Game-Screen vor.
     *
     * Bewusst nicht Teil von GameSnapshot - reine, kurzlebige
     * UI-Rückmeldung, kein dauerhafter Spielzustand.
     */
    void rollStartingPlayer();

    /*
     * Würfelt mit dem prozessweiten Zufallsgenerator (kein lokaler
     * Seed auf dem kleinen Main-Task-Stack). Erlaubt sind 2 (Münze),
     * 4, 6, 8, 10, 12 und 20; sonst 0.
     */
    [[nodiscard]]
    static bool isValidDie(int sides);

    [[nodiscard]]
    static int rollDie(int sides);

    [[nodiscard]]
    bool hasStartingPlayerHighlight() const;

    [[nodiscard]]
    std::size_t getStartingPlayerHighlight() const;

    void clearStartingPlayerHighlight();

    /*
     * Zeigt an, um wie viel sich die Lebenspunkte eines Spielers
     * innerhalb der letzten 15 Sekunden insgesamt verändert haben
     * (z.B. "-5" nach mehreren schnellen Taps), damit die Änderung
     * kurz nachvollziehbar bleibt.
     *
     * Bewusst nicht Teil von GameSnapshot - reine, kurzlebige
     * UI-Rückmeldung, kein dauerhafter Spielzustand.
     */
    [[nodiscard]]
    bool hasLifeDeltaHighlight(
        std::size_t playerIndex
    ) const;

    [[nodiscard]]
    int getLifeDelta(
        std::size_t playerIndex
    ) const;

    [[nodiscard]]
    std::chrono::steady_clock::time_point getLifeDeltaTimestamp(
        std::size_t playerIndex
    ) const;

    void clearLifeDeltaHighlight(
        std::size_t playerIndex
    );

    /*
     * Analog zu hasLifeDeltaHighlight() / getLifeDelta(), aber für
     * die Giftzähler-Änderung auf dem Poison-Screen.
     */
    [[nodiscard]]
    bool hasPoisonDeltaHighlight(
        std::size_t playerIndex
    ) const;

    [[nodiscard]]
    int getPoisonDelta(
        std::size_t playerIndex
    ) const;

    [[nodiscard]]
    std::chrono::steady_clock::time_point getPoisonDeltaTimestamp(
        std::size_t playerIndex
    ) const;

    void clearPoisonDeltaHighlight(
        std::size_t playerIndex
    );

    /*
     * Deep-Sleep-Unterstützung
     */
    [[nodiscard]]
    GameSnapshot createSnapshot() const;

    bool restoreSnapshot(
        const GameSnapshot& snapshot
    );

private:
    GameSettings settings_;

    std::array<
        Player,
        maximumPlayerCount
    > players_;

    bool hasStartingPlayerHighlight_ = false;
    std::size_t startingPlayerHighlight_ = 0;

    std::array<
        int,
        maximumPlayerCount
    > lifeDelta_{};

    std::array<
        bool,
        maximumPlayerCount
    > hasLifeDeltaHighlight_{};

    std::array<
        std::chrono::steady_clock::time_point,
        maximumPlayerCount
    > lifeDeltaTimestamp_{};

    std::array<
        int,
        maximumPlayerCount
    > poisonDelta_{};

    std::array<
        bool,
        maximumPlayerCount
    > hasPoisonDeltaHighlight_{};

    std::array<
        std::chrono::steady_clock::time_point,
        maximumPlayerCount
    > poisonDeltaTimestamp_{};

    /*
     * Erster Index:
     * angreifender Commander
     *
     * Zweiter Index:
     * verteidigender Spieler
     */
    std::array<
        std::array<
            int,
            maximumPlayerCount
        >,
        maximumPlayerCount
    > commanderDamage_{};

    void resetCommanderDamage();

    void updateEliminationState(
        std::size_t playerIndex
    );
};

} // namespace sticky_lotus