#include "sticky_lotus/ui/TableLayout.h"

namespace sticky_lotus::ui {

Rect TableLayout::playerArea(
    const std::size_t playerIndex,
    const std::size_t playerCount
) const
{
    if (playerCount == 2) {
        constexpr float playerWidth =
            screenWidth / 2.0F;

        return {
            static_cast<float>(playerIndex) *
                playerWidth,
            0.0F,
            playerWidth,
            screenHeight
        };
    }

    constexpr float playerWidth =
        screenWidth / 2.0F;

    constexpr float playerHeight =
        screenHeight / 2.0F;

    const std::size_t column =
        playerIndex % 2;

    const std::size_t row =
        playerIndex / 2;

    return {
        static_cast<float>(column) *
            playerWidth,
        static_cast<float>(row) *
            playerHeight,
        playerWidth,
        playerHeight
    };
}

bool TableLayout::isUpsideDown(
    const std::size_t playerIndex,
    const std::size_t playerCount
) const
{
    if (playerCount == 2) {
        return playerIndex == 0;
    }

    return playerIndex == 0 ||
        playerIndex == 1;
}

Rect TableLayout::receiverArea(
    const std::size_t receivingPlayer,
    const std::size_t playerCount
) const
{
    return playerArea(
        receivingPlayer,
        playerCount
    );
}

Rect TableLayout::minusArea(
    const std::size_t playerIndex,
    const std::size_t playerCount
) const
{
    const Rect area =
        playerArea(playerIndex, playerCount);

    constexpr float buttonWidth = 74.0F;
    constexpr float margin = 14.0F;

    return {
        area.x + margin,
        area.y,
        buttonWidth,
        area.height
    };
}

Rect TableLayout::plusArea(
    const std::size_t playerIndex,
    const std::size_t playerCount
) const
{
    const Rect area =
        playerArea(playerIndex, playerCount);

    constexpr float buttonWidth = 74.0F;
    constexpr float margin = 14.0F;

    return {
        area.x +
            area.width -
            buttonWidth -
            margin,
        area.y,
        buttonWidth,
        area.height
    };
}

Rect TableLayout::counterArea(
    const std::size_t playerIndex,
    const std::size_t playerCount
) const
{
    const Rect area =
        playerArea(playerIndex, playerCount);

    constexpr float sideWidth = 88.0F;

    return {
        area.x + sideWidth,
        area.y,
        area.width - sideWidth * 2.0F,
        area.height
    };
}



} // namespace sticky_lotus::ui