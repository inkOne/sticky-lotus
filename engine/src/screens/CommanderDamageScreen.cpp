#include "sticky_lotus/screens/CommanderDamageScreen.h"

#include <cstddef>

namespace sticky_lotus::screens
{
    using input::TouchGesture;

    CommanderDamageScreen::CommanderDamageScreen(
        const ScreenContext context
    )
        : context_(context)
    {
    }

    void CommanderDamageScreen::onEnter()
    {
        commitOnExit_ = true;

        context_.commanderDraft.begin(
            context_.game,
            context_.navigation.selectedPlayer()
        );
    }

    void CommanderDamageScreen::onExit()
    {
        if (!context_.commanderDraft.isActive())
        {
            return;
        }

        if (commitOnExit_)
        {
            context_.commanderDraft.commit(
                context_.game
            );
        }
        else
        {
            context_.commanderDraft.cancel();
        }
    }

    void CommanderDamageScreen::handleInput(
        const input::InputFrame& inputFrame
    )
    {
        // Escape verwirft die Änderungen.
        if (inputFrame.cancelPressed)
        {
            leaveWithoutCommit();
            return;
        }

        // Die mittlere Hardwaretaste speichert und schließt.
        if (inputFrame.buttons.centerPressed)
        {
            leaveAndCommit();
            return;
        }

        switch (inputFrame.gesture.gesture)
        {
        case TouchGesture::Tap:
            processTap(
                inputFrame.gesture.endPosition,
                1
            );
            break;

        case TouchGesture::LongPress:
            processTap(
                inputFrame.gesture.endPosition,
                10
            );
            break;
        case TouchGesture::SwipeLeft:
        case TouchGesture::SwipeRight:
            leaveAndCommit();
            break;

        case TouchGesture::SwipeUp:
        case TouchGesture::SwipeDown:
        case TouchGesture::None:
            break;
        }
    }

void CommanderDamageScreen::processTap(
    const ui::Point position,
    const int step
)
{
    const std::size_t receivingPlayer =
        context_.commanderDraft.receivingPlayer();

    const std::size_t playerCount =
        context_.commanderDraft.playerCount();

    if (
        !context_.commanderDraft.isActive() ||
        receivingPlayer >= playerCount
    )
    {
        return;
    }

    for (
        std::size_t attackingPlayer = 0;
        attackingPlayer < playerCount;
        ++attackingPlayer
    )
    {
        if (attackingPlayer == receivingPlayer)
        {
            continue;
        }

        /*
         * Das komplette Spielerfeld des Angreifers
         * ist die Touchfläche.
         */
        const ui::Rect playerArea =
            context_.renderer.getPlayerRectangle(
                attackingPlayer,
                playerCount
            );

        /*
         * Tap gehört nicht zu diesem Spielerfeld.
         */
        if (!playerArea.contains(position))
        {
            continue;
        }

        const bool upsideDown =
            context_.renderer.isPlayerUpsideDown(
                attackingPlayer,
                playerCount
            );

        /*
         * Physikalisch wird das Feld einfach
         * in linke und rechte Hälfte geteilt.
         */
        const float centerX =
            playerArea.x +
            playerArea.width / 2.0F;

        const bool touchedLeft =
            position.x < centerX;

        /*
         * Normale Spieler:
         *
         * links  = Minus
         * rechts = Plus
         *
         * Gedrehte Spieler:
         *
         * links  = Plus
         * rechts = Minus
         *
         * Dadurch entspricht die Bedienung weiterhin
         * der um 180 Grad gedrehten Darstellung.
         */
        const bool minusPressed =
            upsideDown
                ? !touchedLeft
                : touchedLeft;

        /*
         * MINUS
         */
        if (minusPressed)
        {
            context_.commanderDraft.changeDamage(
                attackingPlayer,
                -step
            );

            /*
             * Bestehendes Verhalten unverändert:
             * bei Minus kompletter Commander-Screen neu zeichnen.
             */
            context_.renderer.drawCommanderDamage(
                context_.game,
                context_.commanderDraft
            );

            context_.renderer.flush();

            return;
        }

        /*
         * PLUS
         */
        context_.commanderDraft.changeDamage(
            attackingPlayer,
            step
        );

        /*
         * Bestehendes Partial-Redraw unverändert.
         */
        context_.renderer.drawCommanderDamageRegion(
            context_.game,
            context_.commanderDraft,
            attackingPlayer
        );

        context_.renderer.drawCommanderDamageRegion(
            context_.game,
            context_.commanderDraft,
            receivingPlayer
        );

        return;
    }
}
    void CommanderDamageScreen::leaveAndCommit()
    {
        commitOnExit_ = true;
        context_.navigation.showGame();
    }

    void CommanderDamageScreen::leaveWithoutCommit()
    {
        commitOnExit_ = false;
        context_.navigation.showGame();
    }

    void CommanderDamageScreen::draw()
    {
        context_.renderer.drawCommanderDamage(
            context_.game,
            context_.commanderDraft
        );
    }
} // namespace sticky_lotus::screens
