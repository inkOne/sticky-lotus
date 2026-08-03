#pragma once

#include "sticky_lotus/screens/Screen.h"
#include "sticky_lotus/screens/ScreenContext.h"

namespace sticky_lotus::screens {

    /**
     * Bearbeitet den Commander Damage, den der gewählte Spieler
     * von den anderen Spielern erhalten hat.
     */
    class CommanderDamageScreen final : public Screen
    {
    public:
        explicit CommanderDamageScreen(
            ScreenContext context
        );

        void onEnter() override;
        void onExit() override;

        void handleInput(
            const input::InputFrame& inputFrame
        ) override;

        void draw() override;

    private:
        ScreenContext context_;

        bool commitOnExit_ = true;

        void processTap(ui::Point position);
        void leaveAndCommit();
        void leaveWithoutCommit();
    };

} // namespace sticky_lotus::screens