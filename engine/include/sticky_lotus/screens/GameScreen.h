#pragma once

#include "sticky_lotus/screens/Screen.h"
#include "sticky_lotus/screens/ScreenContext.h"

#include <cstddef>

namespace sticky_lotus::screens {

    /**
     * Normale Spielansicht mit Lebenspunkten und Menüknopf.
     */
    class GameScreen final : public Screen
    {
    public:
        explicit GameScreen(ScreenContext context);

        void handleInput(
            const input::InputFrame& inputFrame
        ) override;

        void draw() override;

    private:
        ScreenContext context_;

        void processTap(ui::Point position);
        void processSwipe(
            const input::GestureEvent& gesture
        );

        [[nodiscard]]
        std::size_t getPlayerIndex(
            ui::Point position
        ) const;

        [[nodiscard]]
        static int getLifeChange(
            ui::Point position
        );
        void processLongPress(
            ui::Point position
        );
    };

} // namespace sticky_lotus::screens