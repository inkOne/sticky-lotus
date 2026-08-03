#pragma once

#include "sticky_lotus/GameState.h"

#include <algorithm>
#include <array>
#include <cstddef>

namespace sticky_lotus::commander {

/**
 * Temporärer Bearbeitungszustand des Commander-Damage-Screens.
 *
 * Während der Screen geöffnet ist, werden nur editedDamage_
 * verändert. Erst beim Commit werden die Änderungen in den
 * eigentlichen GameState übertragen.
 */
class CommanderDamageDraft
{
public:
    static constexpr std::size_t maximumPlayerCount = 4;

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

        originalDamage_.fill(0);
        editedDamage_.fill(0);

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

            const int damage =
                game.getCommanderDamage(
                    attacker,
                    receivingPlayer_
                );

            originalDamage_[attacker] = damage;
            editedDamage_[attacker] = damage;
        }
    }

    /**
     * Verändert den temporären Schaden einer Quelle.
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

        editedDamage_[attackingPlayer] =
            std::max(
                0,
                editedDamage_[attackingPlayer] + amount
            );
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

            const int difference =
                editedDamage_[attacker] -
                originalDamage_[attacker];

            if (difference == 0) {
                continue;
            }

            game.changeCommanderDamage(
                attacker,
                receivingPlayer_,
                difference
            );
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

    [[nodiscard]]
    int damageFrom(
        const std::size_t attackingPlayer
    ) const
    {
        if (attackingPlayer >= playerCount_) {
            return 0;
        }

        return editedDamage_[attackingPlayer];
    }

private:
    bool active_ = false;

    std::size_t receivingPlayer_ = 0;
    std::size_t playerCount_ = 0;

    std::array<int, maximumPlayerCount>
        originalDamage_{};

    std::array<int, maximumPlayerCount>
        editedDamage_{};
};

} // namespace sticky_lotus::commander