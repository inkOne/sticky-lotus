#include "sticky_lotus/screens/PoisonScreen.h"

namespace sticky_lotus::screens {

    using input::TouchGesture;

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

        switch (inputFrame.gesture.gesture) {
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

        if (
            context_.renderer
                .getPoisonPlusRectangle(
                    playerIndex,
                    playerCount
                )
                .contains(position)
        ) {
            context_.game.changePoison(
                playerIndex,
                1
            );

            return;
        }

        if (
            context_.renderer
                .getPoisonMinusRectangle(
                    playerIndex,
                    playerCount
                )
                .contains(position)
        ) {
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