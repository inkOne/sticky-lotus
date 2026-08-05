#include "sticky_lotus/DeathMessageGenerator.h"

#include <array>

#include <cstddef>

#include <random>

#include <string>
namespace sticky_lotus {

    namespace {

        std::mt19937 rng{std::random_device{}()};

        template<std::size_t N>
        std::string pick(
            const std::array<const char*, N>& messages
        )
        {
            std::uniform_int_distribution<std::size_t> dist(
                0,
                N - 1
            );

            return messages[dist(rng)];
        }

    }

    std::string DeathMessageGenerator::randomMessage(
        const EliminationReason reason
    )
    {
        static constexpr std::array life = {
            "You Died",
            "Fatal Damage",
            "Combat Casualty",
            "Life Zero",
            "Game Over",
            "Nice Try",
            "Better Luck",
            "Oops... Dead",
            "Dead Anyway",
            "Skill Issue",
            "Wrong Block",
            "Misplayed Hard",
            "GG Fallen",
            "RIP Legend",
            "Mulligan Please"
        };

        static constexpr std::array commander = {
            "Commander Down",
            "Wrong Block",
            "Misplayed Hard",
            "GG Fallen",
            "Legend Slain",
            "Fatal Commander",
            "Commander Claims Another"
        };

        static constexpr std::array poison = {
            "Poisoned Out",
            "Corrupted",
            "Toxic Victory",
            "Infect Complete",
            "Phyrexia Wins"
        };

        switch (reason) {

        case EliminationReason::Life:
            return pick(life);

        case EliminationReason::CommanderDamage:
            return pick(commander);

        case EliminationReason::Poison:
            return pick(poison);

        default:
            return "Game Over";
        }
    }

}