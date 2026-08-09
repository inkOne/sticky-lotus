#pragma once

#include "sticky_lotus/GameState.h"
#include "sticky_lotus/app/NavigationState.h"
#include "sticky_lotus/input/InputProvider.h"
#include "sticky_lotus/screens/ScreenManager.h"
#include "sticky_lotus/ui/AppRenderer.h"
#include "sticky_lotus/ui/Canvas.h"
#include "sticky_lotus/commander/CommanderDamageDraft.h"
#include "sticky_lotus/ui/ImageRenderer.h"

namespace sticky_lotus::app
{
    /**
     * Zentrale plattformunabhängige Anwendung.
     */
    class Application
    {
    public:
        Application(
            ui::Canvas& canvas,
            ui::ImageRenderer& imageRenderer,
            input::InputProvider& input
        );

        /**
         * Verarbeitet einen vollständigen Anwendungsdurchlauf.
         */
        void tick();

        void draw();

        void setBatteryStatus(
            int batteryPercent,
            bool charging
        );
        void showSleepScreen();
        [[nodiscard]]
        GameSnapshot createGameSnapshot() const;

        bool restoreGameSnapshot(
            const GameSnapshot& snapshot
        );
    private:
        GameState game_;
        NavigationState navigation_;
        commander::CommanderDamageDraft commanderDraft_;

        ui::Canvas& canvas_;
        input::InputProvider& input_;

        ui::AppRenderer renderer_;
        screens::ScreenManager screenManager_;
    };
} // namespace sticky_lotus::app
