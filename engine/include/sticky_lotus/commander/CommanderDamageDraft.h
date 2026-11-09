#pragma once

#include "sticky_lotus/GameState.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>

namespace sticky_lotus::commander {

/**
 * Temporärer Bearbeitungszustand des Commander-Damage-Screens.
 *
 * Während der Screen geöffnet ist, werden nur editedDamage_
 * verändert. Erst beim Commit werden die Änderungen in den
 * eigentlichen GameState übertragen.
 *
 * Jeder Angreifer hat zwei Zähler: Commander A und Partner B. Welcher
 * davon gerade angezeigt und verändert wird, wählt selectedCommander().
 */
class CommanderDamageDraft
{
public:
    static constexpr std::size_t maximumPlayerCount = 4;
    static constexpr int commanderCount = 2;

    /**
     * Lädt den aktuellen Commander Damage eines Empfängers.
     */
    void begin(
        const GameState& game,
        const std::size_t receivingPlayer
    )
    {
        receivingPlayer_ = receivingPlayer;
        playerCount_ = game.getPlayerCount();
        active_ = receivingPlayer < playerCount_;

        for (std::size_t attacker = 0; attacker < maximumPlayerCount; ++attacker) {
            originalDamage_[attacker].fill(0);
            editedDamage_[attacker].fill(0);
            damageDelta_[attacker].fill(0);
            hasDamageDeltaHighlight_[attacker].fill(false);
        }

        selectedCommander_.fill(0);

        if (!active_) {
            return;
        }

        for (
            std::size_t attacker = 0;
            attacker < playerCount_;
            ++attacker
        ) {
            if (attacker == receivingPlayer_) {
                continue;
            }

            for (int commander = 0; commander < commanderCount; ++commander) {
                const int damage =
                    game.getCommanderDamage(
                        attacker,
                        receivingPlayer_,
                        commander
                    );

                originalDamage_[attacker][commander] = damage;
                editedDamage_[attacker][commander] = damage;
            }
        }
    }

    /**
     * Verändert den temporären Schaden des gerade gewählten
     * Commanders einer Quelle.
     */
    void changeDamage(
        const std::size_t attackingPlayer,
        const int amount
    )
    {
        if (
            !active_ ||
            attackingPlayer >= playerCount_ ||
            attackingPlayer == receivingPlayer_
        ) {
            return;
        }

        const int commander =
            selectedCommander_[attackingPlayer];

        int& damage =
            editedDamage_[attackingPlayer][commander];

        const int before = damage;

        damage = std::max(0, damage + amount);

        const int appliedChange = damage - before;

        if (appliedChange != 0) {
            damageDelta_[attackingPlayer][commander] +=
                appliedChange;

            hasDamageDeltaHighlight_[attackingPlayer][commander] =
                damageDelta_[attackingPlayer][commander] != 0;

            damageDeltaTimestamp_[attackingPlayer][commander] =
                std::chrono::steady_clock::now();
        }
    }

    /**
     * Wechselt bei einem Angreifer zwischen Commander A und Partner B.
     */
    void toggleCommander(
        const std::size_t attackingPlayer
    )
    {
        if (
            !active_ ||
            attackingPlayer >= playerCount_ ||
            attackingPlayer == receivingPlayer_
        ) {
            return;
        }

        selectedCommander_[attackingPlayer] =
            1 - selectedCommander_[attackingPlayer];
    }

    /**
     * 0 = Commander A, 1 = Partner B.
     */
    [[nodiscard]]
    int selectedCommander(
        const std::size_t attackingPlayer
    ) const
    {
        if (attackingPlayer >= maximumPlayerCount) {
            return 0;
        }

        return selectedCommander_[attackingPlayer];
    }

    /*
     * Änderungsanzeige pro Angreifer und Commander, analog zu
     * GameState::hasLifeDeltaHighlight() / getLifeDelta() - kurzlebige
     * UI-Rückmeldung, die nicht über begin()/commit() hinaus erhalten
     * bleiben muss.
     */
    [[nodiscard]]
    bool hasDamageDeltaHighlight(
        const std::size_t attackingPlayer,
        const int commander
    ) const
    {
        if (
            attackingPlayer >= maximumPlayerCount ||
            !validCommander(commander)
        ) {
            return false;
        }

        return hasDamageDeltaHighlight_[attackingPlayer][commander];
    }

    [[nodiscard]]
    int getDamageDelta(
        const std::size_t attackingPlayer,
        const int commander
    ) const
    {
        if (
            attackingPlayer >= maximumPlayerCount ||
            !validCommander(commander)
        ) {
            return 0;
        }

        return damageDelta_[attackingPlayer][commander];
    }

    [[nodiscard]]
    std::chrono::steady_clock::time_point getDamageDeltaTimestamp(
        const std::size_t attackingPlayer,
        const int commander
    ) const
    {
        if (
            attackingPlayer >= maximumPlayerCount ||
            !validCommander(commander)
        ) {
            return {};
        }

        return damageDeltaTimestamp_[attackingPlayer][commander];
    }

    void clearDamageDeltaHighlight(
        const std::size_t attackingPlayer,
        const int commander
    )
    {
        if (
            attackingPlayer >= maximumPlayerCount ||
            !validCommander(commander)
        ) {
            return;
        }

        damageDelta_[attackingPlayer][commander] = 0;
        hasDamageDeltaHighlight_[attackingPlayer][commander] = false;
    }

    /**
     * Überträgt die Differenzen in den GameState.
     *
     * GameState verändert dabei gleichzeitig die Lebenspunkte
     * des Empfängers.
     */
    void commit(GameState& game)
    {
        if (!active_) {
            return;
        }

        for (
            std::size_t attacker = 0;
            attacker < playerCount_;
            ++attacker
        ) {
            if (attacker == receivingPlayer_) {
                continue;
            }

            for (int commander = 0; commander < commanderCount; ++commander) {
                const int difference =
                    editedDamage_[attacker][commander] -
                    originalDamage_[attacker][commander];

                if (difference == 0) {
                    continue;
                }

                game.changeCommanderDamage(
                    attacker,
                    receivingPlayer_,
                    difference,
                    commander
                );
            }
        }

        active_ = false;
    }

    /**
     * Verwirft alle Änderungen.
     */
    void cancel()
    {
        active_ = false;
    }

    [[nodiscard]]
    bool isActive() const
    {
        return active_;
    }

    [[nodiscard]]
    std::size_t receivingPlayer() const
    {
        return receivingPlayer_;
    }

    [[nodiscard]]
    std::size_t playerCount() const
    {
        return playerCount_;
    }

    /**
     * Schaden des gerade gewählten Commanders eines Angreifers.
     */
    [[nodiscard]]
    int damageFrom(
        const std::size_t attackingPlayer
    ) const
    {
        if (attackingPlayer >= playerCount_) {
            return 0;
        }

        return editedDamage_[attackingPlayer][
            selectedCommander_[attackingPlayer]
        ];
    }

    /**
     * Schaden eines bestimmten Commanders (0 = A, 1 = B).
     */
    [[nodiscard]]
    int damageFrom(
        const std::size_t attackingPlayer,
        const int commander
    ) const
    {
        if (
            attackingPlayer >= playerCount_ ||
            !validCommander(commander)
        ) {
            return 0;
        }

        return editedDamage_[attackingPlayer][commander];
    }

    [[nodiscard]]
    bool isLethal() const
    {
        if (!active_) {
            return false;
        }

        for (
            std::size_t attacker = 0;
            attacker < playerCount_;
            ++attacker
        ) {
            if (attacker == receivingPlayer_) {
                continue;
            }

            /*
             * Commander Damage wird pro gegnerischem Commander
             * getrennt geprüft - auch Commander und Partner
             * desselben Gegners werden nicht zusammengerechnet.
             */
            for (int commander = 0; commander < commanderCount; ++commander) {
                if (editedDamage_[attacker][commander] >= 21) {
                    return true;
                }
            }
        }

        return false;
    }

private:
    static constexpr bool validCommander(const int commander)
    {
        return commander >= 0 && commander < commanderCount;
    }

    template <typename T>
    using PerCommander =
        std::array<std::array<T, commanderCount>, maximumPlayerCount>;

    bool active_ = false;

    std::size_t receivingPlayer_ = 0;
    std::size_t playerCount_ = 0;

    PerCommander<int> originalDamage_{};
    PerCommander<int> editedDamage_{};

    PerCommander<int> damageDelta_{};
    PerCommander<bool> hasDamageDeltaHighlight_{};
    PerCommander<std::chrono::steady_clock::time_point>
        damageDeltaTimestamp_{};

    std::array<int, maximumPlayerCount> selectedCommander_{};
};

} // namespace sticky_lotus::commander
