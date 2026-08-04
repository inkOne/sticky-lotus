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
    // Escape beziehungsweise Abbrechen kehrt zum Spiel zurück.
    if (inputFrame.cancelPressed) {
        context_.navigation.showGame();
        return;
    }

    // Die mittlere Sticky-Taste schließt ebenfalls die Settings.
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
        return;
    }

    if (
        renderer
            .getMultiplayer40Rectangle()
            .contains(position)
    ) {
        context_.game.setMultiplayerStartingLife(40);
        return;
    }

    if (
        renderer
            .getMultiplayer30Rectangle()
            .contains(position)
    ) {
        context_.game.setMultiplayerStartingLife(30);
        return;
    }

    if (
        renderer
            .getMultiplayer20Rectangle()
            .contains(position)
    ) {
        context_.game.setMultiplayerStartingLife(20);
        return;
    }

    // Eigener Multiplayer-Wert folgt später.
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
        context_.game.setTwoPlayerStartingLife(20);
        return;
    }

    if (
        renderer
            .getTwoPlayer30Rectangle()
            .contains(position)
    ) {
        context_.game.setTwoPlayerStartingLife(30);
        return;
    }

    if (
        renderer
            .getTwoPlayer40Rectangle()
            .contains(position)
    ) {
        context_.game.setTwoPlayerStartingLife(40);
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