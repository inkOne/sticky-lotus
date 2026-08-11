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
    const ui::Point position
)
{
    const std::size_t defenderIndex =
        context_.navigation.selectedPlayer();

    const std::size_t playerCount =
        context_.game.getPlayerCount();

    if (defenderIndex >= playerCount)
    {
        return;
    }

    const std::size_t attackerIndex =
        draft_.selectedAttacker();

    if (
        attackerIndex >= playerCount ||
        attackerIndex == defenderIndex
    )
    {
        return;
    }

    const ui::Rect playerArea =
        context_.renderer.getPlayerRectangle(
            defenderIndex,
            playerCount
        );

    /*
     * Nur Taps innerhalb des ausgewählten
     * Spielerfeldes berücksichtigen.
     */
    if (!playerArea.contains(position))
    {
        return;
    }

    const bool upsideDown =
        context_.renderer.isPlayerUpsideDown(
            defenderIndex,
            playerCount
        );

    /*
     * Ganze Spielerfläche teilen:
     *
     * obere Hälfte  = +1 Commander Damage
     * untere Hälfte = -1 Commander Damage
     */
    const float centerY =
        playerArea.y +
        playerArea.height / 2.0F;

    int damageChange =
        position.y < centerY
            ? 1
            : -1;

    /*
     * Für Spieler 3/4 Bedienrichtung umkehren.
     */
    if (upsideDown)
    {
        damageChange =
            -damageChange;
    }

    const int damageBefore =
        context_.game.getCommanderDamage(
            attackerIndex,
            defenderIndex
        );

    const bool wasEliminated =
        context_.game.isPlayerEliminated(
            defenderIndex
        );

    context_.game.changeCommanderDamage(
        attackerIndex,
        defenderIndex,
        damageChange
    );

    const int damageAfter =
        context_.game.getCommanderDamage(
            attackerIndex,
            defenderIndex
        );

    /*
     * Keine tatsächliche Änderung, z. B.
     * bei 0 Schaden und erneut Minus.
     */
    if (damageBefore == damageAfter)
    {
        return;
    }

    const bool isEliminated =
        context_.game.isPlayerEliminated(
            defenderIndex
        );

    /*
     * Commander-Tod oder Wiederbelebung:
     * kompletten Game-Screen neu zeichnen.
     */
    if (wasEliminated != isEliminated)
    {
        context_.renderer.drawGameImmediately(
            context_.game
        );

        context_.navigation.showGame();

        return;
    }

    /*
     * Sonst nur Commander-Bereich neu zeichnen.
     */
    context_.renderer.drawCommanderDamageRegion(
        context_.game,
        draft_
    );
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
