#pragma once

#include "sticky_lotus/screens/Screen.h"
#include "sticky_lotus/screens/ScreenContext.h"

#include <array>
#include <chrono>
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

        /**
         * Verwirft die Möglichkeit, den letzten Ping rückgängig zu machen.
         */
        void cancelPingUndo() { pingUndoActive_ = false; }

        void onExit() override { cancelPingUndo(); }

    private:
        ScreenContext context_;

        /*
         * Letzter Ping: Wer gepingt hat, wie viel Leben jeder Spieler
         * dadurch tatsächlich verloren hat und wann. Eine Wischgeste nach
         * oben im selben Feld macht ihn vier Sekunden lang rückgängig.
         */
        bool pingUndoActive_ = false;
        std::size_t pingSource_ = 0;
        std::size_t pingPlayerCount_ = 0;
        std::array<int, maximumPlayerCount> pingLoss_{};
        std::chrono::steady_clock::time_point pingTime_{};

        void processPing(ui::Point position);
        bool undoPing(ui::Point position);

        void processTap(ui::Point position);
        void processSwipe(
            const input::GestureEvent& gesture
        );

        [[nodiscard]]
        std::size_t getPlayerIndex(
            ui::Point position
        ) const;

        [[nodiscard]]
        int getLifeChange(
            ui::Point position
        ) const;
        void processLongPress(
            ui::Point position
        );
    };

} // namespace sticky_lotus::screens