#pragma once

#include "sticky_lotus/screens/CommanderDamageScreen.h"
#include "sticky_lotus/screens/GameScreen.h"
#include "sticky_lotus/screens/Screen.h"
#include "sticky_lotus/screens/ScreenContext.h"
#include "sticky_lotus/screens/SettingsScreen.h"
#include "sticky_lotus/screens/PoisonScreen.h"

namespace sticky_lotus::screens {

    /**
     * Wählt anhand des Navigationszustands den aktiven Screen aus.
     */
    class ScreenManager
    {
    public:
        explicit ScreenManager(ScreenContext context);

        void handleInput(
            const input::InputFrame& inputFrame
        );

        void draw();

    private:
        ScreenContext context_;

        GameScreen gameScreen_;
        SettingsScreen settingsScreen_;
        CommanderDamageScreen commanderDamageScreen_;
        PoisonScreen poisonScreen_;

        app::ScreenId previousScreen_ =
            app::ScreenId::Game;

        [[nodiscard]]
        Screen& currentScreen();

        void processScreenTransition();
    };

} // namespace sticky_lotus::screens