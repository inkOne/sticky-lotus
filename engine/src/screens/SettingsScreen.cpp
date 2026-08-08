#include "sticky_lotus/screens/SettingsScreen.h"

namespace sticky_lotus::screens {

using input::TouchGesture;

SettingsScreen::SettingsScreen(
    const ScreenContext context
)
    : context_(context)
{
}

void SettingsScreen::handleInput(
    const input::InputFrame& inputFrame
)
{
    if (inputFrame.cancelPressed) {
        context_.navigation.showGame();
        return;
    }

    if (inputFrame.buttons.centerPressed) {
        context_.navigation.showGame();
        return;
    }

    if (
        inputFrame.gesture.gesture ==
        TouchGesture::Tap
    ) {
        processTap(
            inputFrame.gesture.endPosition
        );
    }
}

void SettingsScreen::processTap(
    const ui::Point position
)
{
    const ui::AppRenderer& renderer =
        context_.renderer;

    auto redraw = [&]()
    {
        context_.renderer.drawSettings(
            context_.game
        );

        context_.renderer.flush();
    };

    if (
        renderer
            .getCloseButtonRectangle()
            .contains(position)
    ) {
        context_.navigation.showGame();
        return;
    }

    if (
        renderer
            .getTwoPlayersButtonRectangle()
            .contains(position)
    ) {
        context_.game.setPlayerMode(
            PlayerMode::TwoPlayers
        );

        redraw();
        return;
    }

    if (
        renderer
            .getFourPlayersButtonRectangle()
            .contains(position)
    ) {
        context_.game.setPlayerMode(
            PlayerMode::FourPlayers
        );

        redraw();
        return;
    }

    if (
        renderer
            .getMultiplayer40Rectangle()
            .contains(position)
    ) {
        context_.game.setMultiplayerStartingLife(
            40
        );

        redraw();
        return;
    }

    if (
        renderer
            .getMultiplayer30Rectangle()
            .contains(position)
    ) {
        context_.game.setMultiplayerStartingLife(
            30
        );

        redraw();
        return;
    }

    if (
        renderer
            .getMultiplayer20Rectangle()
            .contains(position)
    ) {
        context_.game.setMultiplayerStartingLife(
            20
        );

        redraw();
        return;
    }

    if (
        renderer
            .getMultiplayerEditRectangle()
            .contains(position)
    ) {
        return;
    }

    if (
        renderer
            .getTwoPlayer20Rectangle()
            .contains(position)
    ) {
        context_.game.setTwoPlayerStartingLife(
            20
        );

        redraw();
        return;
    }

    if (
        renderer
            .getTwoPlayer30Rectangle()
            .contains(position)
    ) {
        context_.game.setTwoPlayerStartingLife(
            30
        );

        redraw();
        return;
    }

    if (
        renderer
            .getTwoPlayer40Rectangle()
            .contains(position)
    ) {
        context_.game.setTwoPlayerStartingLife(
            40
        );

        redraw();
        return;
    }

    if (
        renderer
            .getTwoPlayerEditRectangle()
            .contains(position)
    ) {
        return;
    }

    if (
        renderer
            .getResetButtonRectangle()
            .contains(position)
    ) {
        context_.game.reset();

        context_.navigation.showGame();
        return;
    }

    if (
        renderer
            .getDoneButtonRectangle()
            .contains(position)
    ) {
        context_.navigation.showGame();
        return;
    }
}

void SettingsScreen::draw()
{
    context_.renderer.drawSettings(
        context_.game
    );
}

} // namespace sticky_lotus::screens