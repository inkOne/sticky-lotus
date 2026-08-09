#include "sticky_lotus/screens/GameScreen.h"
#include "sticky_lotus/ui/AppRenderer.h"

namespace
{
    sticky_lotus::input::TouchGesture rotateGesture180(
        const sticky_lotus::input::TouchGesture gesture
    )
    {
        using sticky_lotus::input::TouchGesture;

        switch (gesture)
        {
        case TouchGesture::None:
            return TouchGesture::None;

        case TouchGesture::Tap:
            return TouchGesture::Tap;

        case TouchGesture::LongPress:
            return TouchGesture::LongPress;

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

    bool isUpperPlayerArea(
        const sticky_lotus::ui::Point& position
    )
    {
        return position.y <
            sticky_lotus::ui::AppRenderer::screenHeight / 2.0F;
    }
} // namespace

namespace sticky_lotus::screens
{
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
        if (inputFrame.buttons.centerPressed)
        {
            context_.navigation.showSettings();
            return;
        }

        if (inputFrame.buttons.leftPressed)
        {
            context_.game.changeLife(0, -1);
        }

        if (inputFrame.buttons.rightPressed)
        {
            context_.game.changeLife(0, 1);
        }

        TouchGesture effectiveGesture =
            inputFrame.gesture.gesture;

        /*
         * Nur Swipes in der oberen Displayhälfte drehen.
         *
         * Die Position selbst bleibt unverändert.
         * Dadurch bleibt oben links auch oben links und
         * unten links auch unten links.
         */
        const bool isSwipe =
            effectiveGesture == TouchGesture::SwipeLeft ||
            effectiveGesture == TouchGesture::SwipeRight ||
            effectiveGesture == TouchGesture::SwipeUp ||
            effectiveGesture == TouchGesture::SwipeDown;

        if (
            isSwipe &&
            isUpperPlayerArea(
                inputFrame.gesture.startPosition
            )
        )
        {
            effectiveGesture =
                rotateGesture180(
                    effectiveGesture
                );
        }

        switch (effectiveGesture)
        {
        case TouchGesture::Tap:
            processTap(
                inputFrame.gesture.endPosition
            );
            break;
        case TouchGesture::LongPress:
            processLongPress(
                inputFrame.gesture.endPosition
            );
            break;
        case TouchGesture::SwipeLeft:
        case TouchGesture::SwipeRight:
            {
                input::GestureEvent effectiveEvent =
                    inputFrame.gesture;

                effectiveEvent.gesture =
                    effectiveGesture;

                processSwipe(
                    effectiveEvent
                );
                break;
            }

        case TouchGesture::SwipeUp:
            {
                const std::size_t playerIndex =
                    getPlayerIndex(
                        inputFrame.gesture.startPosition
                    );

                if (
                    playerIndex <
                    context_.game.getPlayerCount()
                )
                {
                    context_.navigation.showPoison(
                        playerIndex
                    );
                }

                break;
            }

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
        )
        {
            context_.navigation.showSettings();
            return;
        }

        const std::size_t playerIndex =
            getPlayerIndex(
                position
            );

        if (
            playerIndex >=
            context_.game.getPlayerCount()
        )
        {
            return;
        }

        int lifeChange =
            getLifeChange(
                position
            );

        const bool upsideDown =
            context_.renderer.isPlayerUpsideDown(
                playerIndex,
                context_.game.getPlayerCount()
            );

        if (upsideDown)
        {
            lifeChange =
                -lifeChange;
        }

        context_.game.changeLife(
            playerIndex,
            lifeChange
        );

        /*
         * Nur das tatsächlich geänderte Spielerfeld
         * im Framebuffer neu zeichnen.
         */
        /*context_.renderer.drawPlayerRegion(
            context_.game,
            playerIndex
        );*/
        context_.renderer.drawLifeCounterRegion(
            context_.game,
            playerIndex
        );
    }

    void GameScreen::processLongPress(
        const ui::Point position
    )
    {
        const std::size_t playerIndex =
            getPlayerIndex(
                position
            );

        if (
            playerIndex >=
            context_.game.getPlayerCount()
        )
        {
            return;
        }

        int lifeChange =
            getLifeChange(
                position
            );

        const bool upsideDown =
            context_.renderer.isPlayerUpsideDown(
                playerIndex,
                context_.game.getPlayerCount()
            );

        if (upsideDown)
        {
            lifeChange =
                -lifeChange;
        }

        /*
         * Aus +/-1 wird +/-10.
         */
        lifeChange *= 10;

        context_.game.changeLife(
            playerIndex,
            lifeChange
        );

        context_.renderer.drawLifeCounterRegion(
            context_.game,
            playerIndex
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
        )
        {
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
        if (context_.game.getPlayerCount() == 2)
        {
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
