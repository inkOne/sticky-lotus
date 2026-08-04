#include "sticky_lotus/screens/ScreenManager.h"

namespace sticky_lotus::screens {

    ScreenManager::ScreenManager(
        const ScreenContext context
    )
        : context_(context),
          gameScreen_(context),
          settingsScreen_(context),
          commanderDamageScreen_(context),
          poisonScreen_(context)
    {
        currentScreen().onEnter();
    }

    Screen& ScreenManager::currentScreen()
    {
        switch (context_.navigation.currentScreen()) {
        case app::ScreenId::Game:
            return gameScreen_;

        case app::ScreenId::Settings:
            return settingsScreen_;

        case app::ScreenId::CommanderDamage:
            return commanderDamageScreen_;

        case app::ScreenId::Poison:
            return poisonScreen_;
        }

        return gameScreen_;
    }

    void ScreenManager::processScreenTransition()
    {
        const app::ScreenId currentScreenId =
            context_.navigation.currentScreen();

        if (currentScreenId == previousScreen_) {
            return;
        }

        switch (previousScreen_) {
        case app::ScreenId::Game:
            gameScreen_.onExit();
            break;

        case app::ScreenId::Settings:
            settingsScreen_.onExit();
            break;

        case app::ScreenId::CommanderDamage:
            commanderDamageScreen_.onExit();
            break;

        case app::ScreenId::Poison:
            poisonScreen_.onExit();
            break;
        }

        previousScreen_ = currentScreenId;

        currentScreen().onEnter();
    }

    void ScreenManager::handleInput(
        const input::InputFrame& inputFrame
    )
    {
        processScreenTransition();

        currentScreen().handleInput(
            inputFrame
        );

        processScreenTransition();
    }

    void ScreenManager::draw()
    {
        processScreenTransition();
        currentScreen().draw();
    }

} // namespace sticky_lotus::screens