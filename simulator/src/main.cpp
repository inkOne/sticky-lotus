#include "sticky_lotus/GameState.h"
#include "sticky_lotus_sim/RaylibInput.h"
#include "sticky_lotus_sim/RaylibRenderer.h"

#include <raylib.h>

#include <cstddef>

namespace {


using sticky_lotus_sim::GestureEvent;
using sticky_lotus_sim::TouchGesture;
using sticky_lotus_sim::HardwareButtonState;
using sticky_lotus_sim::PointerState;
using sticky_lotus_sim::RaylibInput;
using sticky_lotus_sim::RaylibRenderer;

/**
 * Ermittelt anhand einer Bildschirmposition,
 * welches Spielerfeld berührt wurde.
 */
std::size_t getPlayerIndex(
    const Vector2 position,
    const std::size_t playerCount
)
{
    if (playerCount == 2) {
        return position.x <
            RaylibRenderer::screenWidth / 2.0F
            ? 0
            : 1;
    }

    constexpr float cellWidth =
        RaylibRenderer::screenWidth / 2.0F;

    constexpr float cellHeight =
        RaylibRenderer::screenHeight / 2.0F;

    const int column =
        position.x < cellWidth ? 0 : 1;

    const int row =
        position.y < cellHeight ? 0 : 1;

    return static_cast<std::size_t>(
        row * 2 + column
    );
}

/**
 * Ermittelt bei einem Tap, ob Leben addiert
 * oder abgezogen werden soll.
 *
 * Linke Hälfte  -> -1
 * Rechte Hälfte -> +1
 */
int getLifeChange(
    const Vector2 position
)
{
    constexpr float cellWidth =
        RaylibRenderer::screenWidth / 2.0F;

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
 * Prüft, ob ein Punkt in einem Rechteck liegt.
 */
bool containsPoint(
    const Rectangle& area,
    const Vector2 point
)
{
    return CheckCollisionPointRec(
        point,
        area
    );
}

/**
 * Verarbeitet einen Tap innerhalb der Settings.
 */
void processSettingsTap(
    GameState& game,
    const RaylibRenderer& renderer,
    const Vector2 position,
    bool& settingsOpen
)
{
    if (containsPoint(
        renderer.getCloseButtonRectangle(),
        position
    )) {
        settingsOpen = false;
        return;
    }

    if (containsPoint(
        renderer.getTwoPlayersButtonRectangle(),
        position
    )) {
        game.setPlayerMode(
            PlayerMode::TwoPlayers
        );
        return;
    }

    if (containsPoint(
        renderer.getFourPlayersButtonRectangle(),
        position
    )) {
        game.setPlayerMode(
            PlayerMode::FourPlayers
        );
        return;
    }

    if (containsPoint(
        renderer.getMultiplayer40Rectangle(),
        position
    )) {
        game.setMultiplayerStartingLife(40);
        return;
    }

    if (containsPoint(
        renderer.getMultiplayer30Rectangle(),
        position
    )) {
        game.setMultiplayerStartingLife(30);
        return;
    }

    if (containsPoint(
        renderer.getMultiplayer20Rectangle(),
        position
    )) {
        game.setMultiplayerStartingLife(20);
        return;
    }

    if (containsPoint(
        renderer.getMultiplayerEditRectangle(),
        position
    )) {
        TraceLog(
            LOG_INFO,
            "Custom multiplayer editor pressed"
        );

        return;
    }

    if (containsPoint(
        renderer.getTwoPlayer20Rectangle(),
        position
    )) {
        game.setTwoPlayerStartingLife(20);
        return;
    }

    if (containsPoint(
        renderer.getTwoPlayer30Rectangle(),
        position
    )) {
        game.setTwoPlayerStartingLife(30);
        return;
    }

    if (containsPoint(
        renderer.getTwoPlayer40Rectangle(),
        position
    )) {
        game.setTwoPlayerStartingLife(40);
        return;
    }

    if (containsPoint(
        renderer.getTwoPlayerEditRectangle(),
        position
    )) {
        TraceLog(
            LOG_INFO,
            "Custom two-player editor pressed"
        );

        return;
    }

    if (containsPoint(
        renderer.getResetButtonRectangle(),
        position
    )) {
        game.reset();
        settingsOpen = false;
        return;
    }

    if (containsPoint(
        renderer.getDoneButtonRectangle(),
        position
    )) {
        settingsOpen = false;
    }
}

/**
 * Verarbeitet einen Tap in der Spielansicht.
 */
void processGameTap(
    GameState& game,
    const RaylibRenderer& renderer,
    const Vector2 position,
    bool& settingsOpen
)
{
    if (containsPoint(
        renderer.getMenuButtonRectangle(),
        position
    )) {
        settingsOpen = true;
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
 * Verarbeitet eine horizontale Wischgeste.
 *
 * Im Moment wird nur protokolliert, welcher Spieler
 * gewischt hat. Im nächsten Schritt öffnen wir damit
 * die Commander-Damage-Ansicht.
 */
void processSwipe(
    const GameState& game,
    const GestureEvent& gestureEvent
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

    if (
        gestureEvent.gesture ==
        TouchGesture::SwipeLeft
    ) {
        TraceLog(
            LOG_INFO,
            "Player %d swiped left",
            static_cast<int>(playerIndex + 1)
        );
    }

    if (
        gestureEvent.gesture ==
        TouchGesture::SwipeRight
    ) {
        TraceLog(
            LOG_INFO,
            "Player %d swiped right",
            static_cast<int>(playerIndex + 1)
        );
    }
}

/**
 * Verarbeitet die drei simulierten Sticky-Tasten.
 */
void processHardwareButtons(
    GameState& game,
    const HardwareButtonState& buttons,
    bool& settingsOpen
)
{
    // Mittlere Taste öffnet oder schließt das Menü.
    if (buttons.centerPressed) {
        settingsOpen = !settingsOpen;
    }

    // Links und rechts werden vorerst nur außerhalb
    // der Settings verwendet.
    if (settingsOpen) {
        return;
    }

    // Vorläufig verändern die beiden äußeren Tasten
    // die Lebenspunkte von Spieler 1.
    //
    // Später können sie beispielsweise für Navigation,
    // Auswahl oder Undo verwendet werden.
    if (buttons.leftPressed) {
        game.changeLife(0, -1);
    }

    if (buttons.rightPressed) {
        game.changeLife(0, 1);
    }
}

} // namespace

int main()
{
    InitWindow(
        RaylibRenderer::screenWidth,
        RaylibRenderer::screenHeight,
        "Sticky Lotus Simulator"
    );

    SetTargetFPS(30);

    GameState game;
    RaylibRenderer renderer;
    RaylibInput input;

    bool settingsOpen = false;

    while (!WindowShouldClose()) {
        // Aktuellen Maus- beziehungsweise Touchzustand lesen.
        const PointerState pointer =
            input.readPointer();

        // Aus Mausbewegung und Tastenzuständen eine
        // abgeschlossene Geste erzeugen.
        const GestureEvent gestureEvent =
            input.updateGesture(pointer);

        // Die drei Sticky-Tasten werden im Simulator
        // durch Tastaturtasten nachgebildet.
        const HardwareButtonState buttons =
            input.readHardwareButtons();

        processHardwareButtons(
            game,
            buttons,
            settingsOpen
        );

        // Tap oder Swipe werden erst beim Loslassen ausgewertet.
        switch (gestureEvent.gesture) {
            case TouchGesture::Tap:
                if (settingsOpen) {
                    processSettingsTap(
                        game,
                        renderer,
                        gestureEvent.endPosition,
                        settingsOpen
                    );
                } else {
                    processGameTap(
                        game,
                        renderer,
                        gestureEvent.endPosition,
                        settingsOpen
                    );
                }

                break;

            case TouchGesture::SwipeLeft:
            case TouchGesture::SwipeRight:
                if (!settingsOpen) {
                    processSwipe(
                        game,
                        gestureEvent
                    );
                }

                break;

            case TouchGesture::SwipeUp:
            case TouchGesture::SwipeDown:
            case TouchGesture::None:
                break;
        }

        // Escape schließt nur das Settings-Overlay.
        if (
            settingsOpen &&
            IsKeyPressed(KEY_ESCAPE)
        ) {
            settingsOpen = false;
        }

        BeginDrawing();

        if (settingsOpen) {
            renderer.drawSettings(game);
        } else {
            renderer.drawGame(game);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}