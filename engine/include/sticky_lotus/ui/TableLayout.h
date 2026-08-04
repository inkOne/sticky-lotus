#pragma once

#include "sticky_lotus/ui/Geometry.h"

#include <cstddef>

namespace sticky_lotus::ui {

    /**
     * Berechnet die räumliche Anordnung der Spieler auf dem Spieltisch.
     *
     * Vier Spieler:
     *
     *   0 | 1
     *  ---+---
     *   2 | 3
     *
     * Die oberen Felder 0 und 1 werden um 180 Grad dargestellt.
     *
     * Zwei Spieler:
     *
     *   0 | 1
     *
     * Spieler 0 wird um 180 Grad dargestellt.
     */
    class TableLayout
    {
    public:
        static constexpr float screenWidth = 800.0F;
        static constexpr float screenHeight = 480.0F;

        /**
         * Liefert das vollständige Feld eines Spielers.
         */
        [[nodiscard]]
        Rect playerArea(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;

        /**
         * Gibt an, ob die Darstellung für diesen Spieler
         * um 180 Grad gedreht werden soll.
         */
        [[nodiscard]]
        bool isUpsideDown(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;

        /**
         * Liefert das Feld, in dem beim Commander-Damage-Screen
         * die Hinweise für den Empfänger angezeigt werden.
         *
         * Es ist identisch mit dessen normalem Spielerfeld.
         */
        [[nodiscard]]
        Rect receiverArea(
            std::size_t receivingPlayer,
            std::size_t playerCount
        ) const;

        /**
         * Liefert den Minus-Bereich innerhalb eines Spielerfeldes.
         *
         * Die geometrische Position bleibt gleich. Die Darstellung
         * wird später abhängig von der Orientierung gedreht.
         */
        [[nodiscard]]
        Rect minusArea(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;

        /**
         * Liefert den Plus-Bereich innerhalb eines Spielerfeldes.
         */
        [[nodiscard]]
        Rect plusArea(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;

        /**
         * Liefert den mittleren Bereich für Leben oder Schaden.
         */
        [[nodiscard]]
        Rect counterArea(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;


    };



} // namespace sticky_lotus::ui