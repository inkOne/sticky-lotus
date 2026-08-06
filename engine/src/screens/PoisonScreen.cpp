#include "sticky_lotus/screens/PoisonScreen.h"

namespace sticky_lotus::screens {

using input::TouchGesture;

namespace {

TouchGesture rotateGesture180(
    const TouchGesture gesture
)
{
    switch (gesture) {
    case TouchGesture::None:
        return TouchGesture::None;

    case TouchGesture::Tap:
        return TouchGesture::Tap;

    case TouchGesture::SwipeUp:
        return TouchGesture::SwipeDown;

    case TouchGesture::SwipeDown:
        return TouchGesture::SwipeUp;

    case TouchGesture::SwipeLeft:
        return TouchGesture::SwipeRight;

    case TouchGesture::SwipeRight:
        return TouchGesture::SwipeLeft;
    }

    return TouchGesture::None;
}

} // namespace

PoisonScreen::PoisonScreen(
    const ScreenContext context
)
    : context_(context)
{
}

void PoisonScreen::handleInput(
    const input::InputFrame& inputFrame
)
{
    if (
        inputFrame.cancelPressed ||
        inputFrame.buttons.centerPressed
    ) {
        context_.navigation.showGame();
        return;
    }

    const std::size_t playerIndex =
        context_.navigation.selectedPlayer();

    const std::size_t playerCount =
        context_.game.getPlayerCount();

    TouchGesture effectiveGesture =
        inputFrame.gesture.gesture;

    /*
     * Die oberen Spielerfelder 3 und 4 werden um
     * 180 Grad dargestellt.
     *
     * Deshalb muss dort auch die Bedeutung einer
     * Swipe-Richtung um 180 Grad gedreht werden.
     */
    if (
        playerIndex < playerCount &&
        context_.renderer.isPlayerUpsideDown(
            playerIndex,
            playerCount
        )
    ) {
        effectiveGesture =
            rotateGesture180(
                effectiveGesture
            );
    }

    switch (effectiveGesture) {
    case TouchGesture::Tap:
        processTap(
            inputFrame.gesture.endPosition
        );
        break;

    case TouchGesture::SwipeDown:
        context_.navigation.showGame();
        break;

    case TouchGesture::SwipeLeft:
    case TouchGesture::SwipeRight:
    case TouchGesture::SwipeUp:
    case TouchGesture::None:
        break;
    }
}

void PoisonScreen::processTap(
    const ui::Point position
)
{
    const std::size_t playerIndex =
        context_.navigation.selectedPlayer();

    const std::size_t playerCount =
        context_.game.getPlayerCount();

    if (playerIndex >= playerCount) {
        return;
    }

    const bool upsideDown =
        context_.renderer.isPlayerUpsideDown(
            playerIndex,
            playerCount
        );

    /*
     * Die Rechtecke selbst liegen weiterhin an ihren
     * globalen Displaypositionen.
     *
     * Bei einem um 180 Grad dargestellten Spielerfeld
     * ist das sichtbare Plus jedoch an der globalen
     * Minus-Position und umgekehrt.
     */
    const ui::Rect plusRectangle =
        upsideDown
            ? context_.renderer
                  .getPoisonMinusRectangle(
                      playerIndex,
                      playerCount
                  )
            : context_.renderer
                  .getPoisonPlusRectangle(
                      playerIndex,
                      playerCount
                  );

    const ui::Rect minusRectangle =
        upsideDown
            ? context_.renderer
                  .getPoisonPlusRectangle(
                      playerIndex,
                      playerCount
                  )
            : context_.renderer
                  .getPoisonMinusRectangle(
                      playerIndex,
                      playerCount
                  );

    if (plusRectangle.contains(position)) {
        context_.game.changePoison(
            playerIndex,
            1
        );

        return;
    }

    if (minusRectangle.contains(position)) {
        context_.game.changePoison(
            playerIndex,
            -1
        );
    }
}

void PoisonScreen::draw()
{
    context_.renderer.drawPoison(
        context_.game,
        context_.navigation.selectedPlayer()
    );
}

} // namespace sticky_lotus::screens