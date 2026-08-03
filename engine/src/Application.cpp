#include "sticky_lotus/app/Application.h"

namespace sticky_lotus::app {

    Application::Application(
        ui::Canvas& canvas,
        input::InputProvider& input
    )
        : canvas_(canvas),
          input_(input),
          renderer_(canvas),
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
        const input::InputFrame inputFrame =
            input_.poll();

        screenManager_.handleInput(
            inputFrame
        );

        canvas_.beginFrame();

        screenManager_.draw();

        canvas_.endFrame();
        canvas_.flush();
    }

} // namespace sticky_lotus::app