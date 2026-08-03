#pragma once

#include "sticky_lotus/GameState.h"
#include "sticky_lotus/app/NavigationState.h"
#include "sticky_lotus/commander/CommanderDamageDraft.h"
#include "sticky_lotus/ui/AppRenderer.h"

namespace sticky_lotus::screens {

    /**
     * Gemeinsame Abhängigkeiten aller Screens.
     */
    struct ScreenContext
    {
        GameState& game;
        app::NavigationState& navigation;
        commander::CommanderDamageDraft& commanderDraft;
        ui::AppRenderer& renderer;
    };

} // namespace sticky_lotus::screens