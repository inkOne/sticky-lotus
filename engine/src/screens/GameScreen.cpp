#include "sticky_lotus/screens/GameScreen.h"

namespace sticky_lotus::screens {

using input::TouchGesture;

GameScreen::GameScreen(
    const ScreenContext context
)
    : context_(context)
{
}

void GameScreen::handleInput(
    const input::InputFrame& inputFrame
)
{
    // Mittlere Hardwaretaste öffnet die Settings.
    if (inputFrame.buttons.centerPressed) {
        context_.navigation.showSettings();
        return;
    }

    // Vorläufig verändern die äußeren Hardwaretasten Spieler 1.
    if (inputFrame.buttons.leftPressed) {
        context_.game.changeLife(0, -1);
    }

    if (inputFrame.buttons.rightPressed) {
        context_.game.changeLife(0, 1);
    }

    switch (inputFrame.gesture.gesture) {
        case TouchGesture::Tap:
            processTap(
                inputFrame.gesture.endPosition
            );
            break;

        case TouchGesture::SwipeLeft:
        case TouchGesture::SwipeRight:
            processSwipe(
                inputFrame.gesture
            );
            break;

        case TouchGesture::SwipeUp:
        case TouchGesture::SwipeDown:
        case TouchGesture::None:
            break;
    }
}

void GameScreen::processTap(
    const ui::Point position
)
{
    if (
        context_.renderer
            .getMenuButtonRectangle()
            .contains(position)
    ) {
        context_.navigation.showSettings();
        return;
    }

    const std::size_t playerIndex =
    getPlayerIndex(position);

    if (
        playerIndex >=
        context_.game.getPlayerCount()
    ) {
        return;
    }

    if (
        context_.game.isPlayerEliminated(
            playerIndex
        )
    ) {
        return;
    }

    context_.game.changeLife(
        playerIndex,
        getLifeChange(position)
    );
}

void GameScreen::processSwipe(
    const input::GestureEvent& gesture
)
{
    const std::size_t playerIndex =
        getPlayerIndex(
            gesture.startPosition
        );

    if (
        playerIndex >=
        context_.game.getPlayerCount()
    ) {
        return;
    }

    /*
     * Der gewischte Spieler wird als ausgewählter Spieler gespeichert.
     *
     * Im nächsten Schritt erhält dieser Wert die klare Bedeutung:
     * Empfänger des Commander Damage.
     */
    context_.navigation.showCommanderDamage(
        playerIndex
    );
}

std::size_t GameScreen::getPlayerIndex(
    const ui::Point position
) const
{
    if (context_.game.getPlayerCount() == 2) {
        return position.x <
            ui::AppRenderer::screenWidth / 2.0F
            ? 0
            : 1;
    }

    constexpr float cellWidth =
        ui::AppRenderer::screenWidth / 2.0F;

    constexpr float cellHeight =
        ui::AppRenderer::screenHeight / 2.0F;

    const int column =
        position.x < cellWidth ? 0 : 1;

    const int row =
        position.y < cellHeight ? 0 : 1;

    return static_cast<std::size_t>(
        row * 2 + column
    );
}

int GameScreen::getLifeChange(
    const ui::Point position
)
{
    constexpr float cellWidth =
        ui::AppRenderer::screenWidth / 2.0F;

    const int column =
        static_cast<int>(
            position.x / cellWidth
        );

    const float localX =
        position.x -
        static_cast<float>(column) *
            cellWidth;

    return localX < cellWidth / 2.0F
        ? -1
        : 1;
}

void GameScreen::draw()
{
    context_.renderer.drawGame(
        context_.game
    );
}

} // namespace sticky_lotus::screens