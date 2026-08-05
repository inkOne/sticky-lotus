#pragma once

namespace sticky_lotus {

    /**
     * Grund, aus dem ein Spieler aktuell ausgeschieden ist.
     *
     * Der Zustand wird aus Lebenspunkten, Commander Damage
     * und Poison Countern berechnet.
     */
    enum class EliminationReason
    {
        None,
        Life,
        CommanderDamage,
        Poison
    };

} // namespace sticky_lotus