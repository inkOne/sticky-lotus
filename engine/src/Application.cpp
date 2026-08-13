#include "sticky_lotus/app/Application.h"

namespace sticky_lotus::app {

    Application::Application(
        ui::Canvas& canvas,
        input::InputProvider& input
    )
        : input_(input),
          renderer_(
              canvas
          ),
          screenManager_({
              game_,
              navigation_,
              commanderDraft_,
              renderer_
          })
    {
    }

    void Application::tick()
    {
        const input::InputFrame input =
            input_.poll();

        if (
            input.gesture.gesture !=
                input::TouchGesture::None ||
            input.buttons.leftPressed ||
            input.buttons.centerPressed ||
            input.buttons.rightPressed ||
            input.cancelPressed
        ) {
            screenManager_.handleInput(
                input
            );
        }
    }
    void Application::draw()
    {
        screenManager_.draw();
    }

    void Application::setBatteryStatus(
        const int batteryPercent,
        const bool charging
    )
    {
        renderer_.setBatteryStatus(
            batteryPercent,
            charging
        );
    }
    void Application::showSleepScreen()
    {
        renderer_.drawSleepScreen();
    }
    GameSnapshot Application::createGameSnapshot() const
    {
        return game_.createSnapshot();
    }

    bool Application::restoreGameSnapshot(
        const GameSnapshot& snapshot
    )
    {
        return game_.restoreSnapshot(
            snapshot
        );
    }
} // namespace sticky_lotus::app