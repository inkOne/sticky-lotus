#include "sticky_lotus/GameState.h"
#include "sticky_lotus/app/NavigationState.h"
#include "sticky_lotus/input/InputProvider.h"
#include "sticky_lotus/ui/AppRenderer.h"
#include "sticky_lotus/ui/Geometry.h"
#include "sticky_lotus_raylib/RaylibCanvas.h"
#include "sticky_lotus_raylib/RaylibInputProvider.h"

#include <cstddef>

namespace {

using sticky_lotus::app::NavigationState;
using sticky_lotus::app::ScreenId;

using sticky_lotus::input::GestureEvent;
using sticky_lotus::input::HardwareButtonState;
using sticky_lotus::input::InputFrame;
using sticky_lotus::input::TouchGesture;

using sticky_lotus::ui::AppRenderer;
using sticky_lotus::ui::Point;

using sticky_lotus_raylib::RaylibCanvas;
using sticky_lotus_raylib::RaylibInputProvider;

/**
 * Ermittelt das Spielerfeld an einer Bildschirmposition.
 */
std::size_t getPlayerIndex(
    const Point position,
    const std::size_t playerCount
)
{
    if (playerCount == 2) {
        return position.x <
            AppRenderer::screenWidth / 2.0F
            ? 0
            : 1;
    }

    constexpr float cellWidth =
        AppRenderer::screenWidth / 2.0F;

    constexpr float cellHeight =
        AppRenderer::screenHeight / 2.0F;

    const int column =
        position.x < cellWidth ? 0 : 1;

    const int row =
        position.y < cellHeight ? 0 : 1;

    return static_cast<std::size_t>(
        row * 2 + column
    );
}

/**
 * Linke Hälfte eines Spielerfeldes  -> -1
 * Rechte Hälfte eines Spielerfeldes -> +1
 */
int getLifeChange(const Point position)
{
    constexpr float cellWidth =
        AppRenderer::screenWidth / 2.0F;

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

/**
 * Verarbeitet einen Tap in den Settings.
 */
void processSettingsTap(
    GameState& game,
    const AppRenderer& renderer,
    const Point position,
    NavigationState& navigation
)
{
    if (
        renderer
            .getCloseButtonRectangle()
            .contains(position)
    ) {
        navigation.showGame();
        return;
    }

    if (
        renderer
            .getTwoPlayersButtonRectangle()
            .contains(position)
    ) {
        game.setPlayerMode(
            PlayerMode::TwoPlayers
        );
        return;
    }

    if (
        renderer
            .getFourPlayersButtonRectangle()
            .contains(position)
    ) {
        game.setPlayerMode(
            PlayerMode::FourPlayers
        );
        return;
    }

    if (
        renderer
            .getMultiplayer40Rectangle()
            .contains(position)
    ) {
        game.setMultiplayerStartingLife(40);
        return;
    }

    if (
        renderer
            .getMultiplayer30Rectangle()
            .contains(position)
    ) {
        game.setMultiplayerStartingLife(30);
        return;
    }

    if (
        renderer
            .getMultiplayer20Rectangle()
            .contains(position)
    ) {
        game.setMultiplayerStartingLife(20);
        return;
    }

    if (
        renderer
            .getTwoPlayer20Rectangle()
            .contains(position)
    ) {
        game.setTwoPlayerStartingLife(20);
        return;
    }

    if (
        renderer
            .getTwoPlayer30Rectangle()
            .contains(position)
    ) {
        game.setTwoPlayerStartingLife(30);
        return;
    }

    if (
        renderer
            .getTwoPlayer40Rectangle()
            .contains(position)
    ) {
        game.setTwoPlayerStartingLife(40);
        return;
    }

    if (
        renderer
            .getResetButtonRectangle()
            .contains(position)
    ) {
        game.reset();
        navigation.showGame();
        return;
    }

    if (
        renderer
            .getDoneButtonRectangle()
            .contains(position)
    ) {
        navigation.showGame();
    }
}

/**
 * Verarbeitet einen Tap in der normalen Spielansicht.
 */
void processGameTap(
    GameState& game,
    const AppRenderer& renderer,
    const Point position,
    NavigationState& navigation
)
{
    if (
        renderer
            .getMenuButtonRectangle()
            .contains(position)
    ) {
        navigation.showSettings();
        return;
    }

    const std::size_t playerIndex =
        getPlayerIndex(
            position,
            game.getPlayerCount()
        );

    if (playerIndex >= game.getPlayerCount()) {
        return;
    }

    game.changeLife(
        playerIndex,
        getLifeChange(position)
    );
}

/**
 * Öffnet von der Spielansicht aus den
 * Commander-Damage-Screen.
 */
void processGameSwipe(
    const GameState& game,
    const GestureEvent& gestureEvent,
    NavigationState& navigation
)
{
    const std::size_t playerIndex =
        getPlayerIndex(
            gestureEvent.startPosition,
            game.getPlayerCount()
        );

    if (playerIndex >= game.getPlayerCount()) {
        return;
    }

    navigation.showCommanderDamage(
        playerIndex
    );
}

/**
 * Verarbeitet die simulierten Sticky-Tasten.
 */
void processHardwareButtons(
    GameState& game,
    const HardwareButtonState& buttons,
    NavigationState& navigation
)
{
    if (buttons.centerPressed) {
        if (
            navigation.currentScreen() ==
            ScreenId::Game
        ) {
            navigation.showSettings();
        } else {
            navigation.showGame();
        }
    }

    if (
        navigation.currentScreen() !=
        ScreenId::Game
    ) {
        return;
    }

    if (buttons.leftPressed) {
        game.changeLife(0, -1);
    }

    if (buttons.rightPressed) {
        game.changeLife(0, 1);
    }
}
/**
 * Verarbeitet Plus und Minus im Commander-Damage-Screen.
 */
void processCommanderDamageTap(
        GameState& game,
        const AppRenderer& renderer,
        const Point position,
        const NavigationState& navigation
    )
{
    const std::size_t sourcePlayer =
        navigation.selectedPlayer();

    if (sourcePlayer >= game.getPlayerCount()) {
        return;
    }

    for (
        std::size_t targetPlayer = 0;
        targetPlayer < game.getPlayerCount();
        ++targetPlayer
    ) {
        if (targetPlayer == sourcePlayer) {
            continue;
        }

        if (
            renderer
                .getCommanderDamageMinusRectangle(
                    sourcePlayer,
                    targetPlayer,
                    game.getPlayerCount()
                )
                .contains(position)
        ) {
            game.changeCommanderDamage(
                sourcePlayer,
                targetPlayer,
                -1
            );

            return;
        }

        if (
            renderer
                .getCommanderDamagePlusRectangle(
                    sourcePlayer,
                    targetPlayer,
                    game.getPlayerCount()
                )
                .contains(position)
        ) {
            game.changeCommanderDamage(
                sourcePlayer,
                targetPlayer,
                1
            );

            return;
        }
    }
}
/**
 * Verarbeitet eine abgeschlossene Touchgeste.
 */
void processGesture(
    GameState& game,
    const AppRenderer& renderer,
    const GestureEvent& gestureEvent,
    NavigationState& navigation
)
{
    switch (gestureEvent.gesture) {
        case TouchGesture::Tap:
            switch (navigation.currentScreen()) {
                case ScreenId::Game:
                    processGameTap(
                        game,
                        renderer,
                        gestureEvent.endPosition,
                        navigation
                    );
                    break;

                case ScreenId::Settings:
                    processSettingsTap(
                        game,
                        renderer,
                        gestureEvent.endPosition,
                        navigation
                    );
                    break;

            case ScreenId::CommanderDamage:
                processCommanderDamageTap(
                    game,
                    renderer,
                    gestureEvent.endPosition,
                    navigation
                );
                break;
            }
            break;

        case TouchGesture::SwipeLeft:
        case TouchGesture::SwipeRight:
            if (
                navigation.currentScreen() ==
                ScreenId::Game
            ) {
                processGameSwipe(
                    game,
                    gestureEvent,
                    navigation
                );
            } else if (
                navigation.currentScreen() ==
                ScreenId::CommanderDamage
            ) {
                navigation.showGame();
            }
            break;

        case TouchGesture::SwipeUp:
        case TouchGesture::SwipeDown:
        case TouchGesture::None:
            break;
    }
}

/**
 * Zeichnet die aktuell ausgewählte Ansicht.
 */
void drawCurrentScreen(
    AppRenderer& renderer,
    const GameState& game,
    const NavigationState& navigation
)
{
    switch (navigation.currentScreen()) {
        case ScreenId::Game:
            renderer.drawGame(game);
            break;

        case ScreenId::Settings:
            renderer.drawSettings(game);
            break;

        case ScreenId::CommanderDamage:
            renderer.drawCommanderDamage(
                game,
                navigation.selectedPlayer()
            );
            break;
    }
}

} // namespace

int main()
{
    RaylibCanvas canvas(
        AppRenderer::screenWidth,
        AppRenderer::screenHeight,
        "Sticky Lotus Simulator"
    );

    RaylibInputProvider input;

    GameState game;
    AppRenderer renderer(canvas);
    NavigationState navigation;

    while (!canvas.shouldClose()) {
        const InputFrame inputFrame =
            input.poll();

        processHardwareButtons(
            game,
            inputFrame.buttons,
            navigation
        );

        processGesture(
            game,
            renderer,
            inputFrame.gesture,
            navigation
        );

        if (inputFrame.cancelPressed) {
            navigation.showGame();
        }

        canvas.beginFrame();

        drawCurrentScreen(
            renderer,
            game,
            navigation
        );

        canvas.endFrame();
        canvas.flush();
    }

    return 0;
}