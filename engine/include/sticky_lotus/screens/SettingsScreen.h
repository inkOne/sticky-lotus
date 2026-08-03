#pragma once

#include "sticky_lotus/screens/Screen.h"
#include "sticky_lotus/screens/ScreenContext.h"

namespace sticky_lotus::screens {

    /**
     * Einstellungen für Spielerzahl und Startleben.
     */
    class SettingsScreen final : public Screen
    {
    public:
        explicit SettingsScreen(ScreenContext context);

        void handleInput(
            const input::InputFrame& inputFrame
        ) override;

        void draw() override;

    private:
        ScreenContext context_;

        void processTap(ui::Point position);
    };

} // namespace sticky_lotus::screens