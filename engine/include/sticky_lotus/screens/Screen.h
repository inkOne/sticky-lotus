#pragma once

#include "sticky_lotus/input/InputProvider.h"

namespace sticky_lotus::screens {

    /**
     * Gemeinsame Schnittstelle aller Anwendungsbildschirme.
     */
    class Screen
    {
    public:
        virtual ~Screen() = default;

        /**
         * Wird aufgerufen, wenn dieser Screen aktiv wird.
         */
        virtual void onEnter()
        {
        }

        /**
         * Wird aufgerufen, bevor dieser Screen verlassen wird.
         */
        virtual void onExit()
        {
        }

        /**
         * Verarbeitet alle Eingaben eines Durchlaufs.
         */
        virtual void handleInput(
            const input::InputFrame& inputFrame
        ) = 0;

        /**
         * Zeichnet den Screen.
         */
        virtual void draw() = 0;
    };

} // namespace sticky_lotus::screens