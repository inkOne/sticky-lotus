#include "sticky_lotus/screens/CommanderDamageScreen.h"

#include <cstddef>

namespace sticky_lotus::screens {

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
    if (!context_.commanderDraft.isActive()) {
        return;
    }

    if (commitOnExit_) {
        context_.commanderDraft.commit(
            context_.game
        );
    } else {
        context_.commanderDraft.cancel();
    }
}

void CommanderDamageScreen::handleInput(
    const input::InputFrame& inputFrame
)
{
    // Escape verwirft die Änderungen.
    if (inputFrame.cancelPressed) {
        leaveWithoutCommit();
        return;
    }

    // Die mittlere Hardwaretaste speichert und schließt.
    if (inputFrame.buttons.centerPressed) {
        leaveAndCommit();
        return;
    }

    switch (inputFrame.gesture.gesture) {
        case TouchGesture::Tap:
            processTap(
                inputFrame.gesture.endPosition
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
    const std::size_t receivingPlayer =
        context_.commanderDraft.receivingPlayer();

    const std::size_t playerCount =
        context_.commanderDraft.playerCount();

    if (
        !context_.commanderDraft.isActive() ||
        receivingPlayer >= playerCount
    ) {
        return;
    }

    for (
        std::size_t attackingPlayer = 0;
        attackingPlayer < playerCount;
        ++attackingPlayer
    ) {
        if (attackingPlayer == receivingPlayer) {
            continue;
        }

        if (
            context_.renderer
                .getCommanderDamageMinusRectangle(
                    attackingPlayer,
                    playerCount
                )
                .contains(position)
        ) {
            context_.commanderDraft.changeDamage(
                attackingPlayer,
                -1
            );

            return;
        }

        if (
            context_.renderer
                .getCommanderDamagePlusRectangle(
                    attackingPlayer,
                    playerCount
                )
                .contains(position)
        ) {
            context_.commanderDraft.changeDamage(
                attackingPlayer,
                1
            );

            return;
        }
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