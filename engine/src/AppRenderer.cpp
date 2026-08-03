#include "sticky_lotus/ui/AppRenderer.h"

#include <cstddef>
#include <string>

namespace sticky_lotus::ui {

AppRenderer::AppRenderer(Canvas& canvas)
    : canvas_(canvas)
{
}

void AppRenderer::drawGame(const GameState& game)
{
    canvas_.clear(Ink::White);

    drawPlayers(game);
    drawMenuButton();

    canvas_.invalidate({
        0.0F,
        0.0F,
        static_cast<float>(screenWidth),
        static_cast<float>(screenHeight)
    });
}

void AppRenderer::drawPlayers(const GameState& game)
{
    const std::size_t playerCount =
        game.getPlayerCount();

    if (playerCount == 2) {
        for (
            std::size_t index = 0;
            index < playerCount;
            ++index
        ) {
            const Rect playerArea = {
                static_cast<float>(
                    index * screenWidth / 2
                ),
                0.0F,
                screenWidth / 2.0F,
                static_cast<float>(screenHeight)
            };

            const bool upsideDown =
                index == 0;

            drawPlayer(
                game.getPlayer(index),
                playerArea,
                upsideDown
            );
        }

        return;
    }

    for (
        std::size_t index = 0;
        index < playerCount;
        ++index
    ) {
        const int column =
            static_cast<int>(index % columns);

        const int row =
            static_cast<int>(index / columns);

        const Rect playerArea = {
            static_cast<float>(
                column * cellWidth
            ),
            static_cast<float>(
                row * cellHeight
            ),
            static_cast<float>(cellWidth),
            static_cast<float>(cellHeight)
        };

        const bool upsideDown =
            index == 0 || index == 1;

        drawPlayer(
            game.getPlayer(index),
            playerArea,
            upsideDown
        );
    }
}

void AppRenderer::drawPlayer(
    const Player& player,
    const Rect& playerArea,
    const bool upsideDown
)
{
    canvas_.fillRect(
        playerArea,
        Ink::White
    );

    canvas_.drawRect(
        playerArea,
        2.0F,
        Ink::Black
    );

    // Spielernamen bleiben bewusst ausgeblendet.
    //
    // canvas_.drawText(
    //     player.name,
    //     {...},
    //     24,
    //     Ink::Black
    // );

    constexpr float sideButtonWidth = 75.0F;
    constexpr float sideMargin = 15.0F;

    const Rect minusArea = {
        playerArea.x + sideMargin,
        playerArea.y,
        sideButtonWidth,
        playerArea.height
    };

    const Rect plusArea = {
        playerArea.x +
            playerArea.width -
            sideButtonWidth -
            sideMargin,
        playerArea.y,
        sideButtonWidth,
        playerArea.height
    };

    const Rect lifeArea = {
        playerArea.x + sideButtonWidth,
        playerArea.y,
        playerArea.width -
            sideButtonWidth * 2.0F,
        playerArea.height
    };

    const float rotation =
        upsideDown ? 180.0F : 0.0F;

    canvas_.drawText(
        std::to_string(player.life),
        lifeArea,
        90,
        Ink::Black,
        TextAlignment::Center,
        rotation
    );

    canvas_.drawText(
        "-",
        minusArea,
        42,
        Ink::DarkGray,
        TextAlignment::Center,
        rotation
    );

    canvas_.drawText(
        "+",
        plusArea,
        42,
        Ink::DarkGray,
        TextAlignment::Center,
        rotation
    );
}

Rect AppRenderer::getMenuButtonRectangle() const
{
    constexpr float buttonSize = 58.0F;

    return {
        screenWidth / 2.0F - buttonSize / 2.0F,
        screenHeight / 2.0F - buttonSize / 2.0F,
        buttonSize,
        buttonSize
    };
}

void AppRenderer::drawMenuButton()
{
    const Rect button =
        getMenuButtonRectangle();

    const Point center = {
        button.x + button.width / 2.0F,
        button.y + button.height / 2.0F
    };

    canvas_.fillCircle(
        center,
        button.width / 2.0F,
        Ink::White
    );

    canvas_.drawCircle(
        center,
        button.width / 2.0F,
        2.0F,
        Ink::Black
    );

    constexpr float lineWidth = 25.0F;
    constexpr float lineThickness = 4.0F;
    constexpr float lineSpacing = 8.0F;

    for (int line = -1; line <= 1; ++line) {
        const float y =
            center.y +
            static_cast<float>(line) *
                lineSpacing;

        canvas_.drawLine(
            {
                center.x - lineWidth / 2.0F,
                y
            },
            {
                center.x + lineWidth / 2.0F,
                y
            },
            lineThickness,
            Ink::Black
        );
    }
}

void AppRenderer::drawSettingsButton(
    const Rect& area,
    const char* label,
    const bool selected
)
{
    canvas_.fillRect(
        area,
        selected
            ? Ink::Black
            : Ink::White
    );

    canvas_.drawRect(
        area,
        2.0F,
        Ink::Black
    );

    canvas_.drawText(
        label,
        area,
        20,
        selected
            ? Ink::White
            : Ink::Black
    );
}

void AppRenderer::drawSettings(
    const GameState& game
)
{
    canvas_.clear(Ink::White);

    const Rect panel = {
        100.0F,
        20.0F,
        600.0F,
        440.0F
    };

    canvas_.fillRect(
        panel,
        Ink::White
    );

    canvas_.drawRect(
        panel,
        3.0F,
        Ink::Black
    );

    canvas_.drawText(
        "Settings",
        {
            130.0F,
            35.0F,
            220.0F,
            50.0F
        },
        32,
        Ink::Black,
        TextAlignment::Left
    );

    drawSettingsButton(
        getCloseButtonRectangle(),
        "X",
        false
    );

    canvas_.drawText(
        "Players",
        {
            130.0F,
            90.0F,
            300.0F,
            30.0F
        },
        22,
        Ink::Black,
        TextAlignment::Left
    );

    drawSettingsButton(
        getTwoPlayersButtonRectangle(),
        "2 Players",
        game.getPlayerCount() == 2
    );

    drawSettingsButton(
        getFourPlayersButtonRectangle(),
        "4 Players",
        game.getPlayerCount() == 4
    );

    canvas_.drawText(
        "Multiplayer Starting Life",
        {
            130.0F,
            185.0F,
            400.0F,
            30.0F
        },
        22,
        Ink::Black,
        TextAlignment::Left
    );

    const int multiplayerLife =
        game.getSettings()
            .multiplayerStartingLife;

    drawSettingsButton(
        getMultiplayer40Rectangle(),
        "40",
        multiplayerLife == 40
    );

    drawSettingsButton(
        getMultiplayer30Rectangle(),
        "30",
        multiplayerLife == 30
    );

    drawSettingsButton(
        getMultiplayer20Rectangle(),
        "20",
        multiplayerLife == 20
    );

    drawSettingsButton(
        getMultiplayerEditRectangle(),
        "Edit",
        multiplayerLife != 40 &&
            multiplayerLife != 30 &&
            multiplayerLife != 20
    );

    canvas_.drawText(
        "Two Player Starting Life",
        {
            130.0F,
            280.0F,
            400.0F,
            30.0F
        },
        22,
        Ink::Black,
        TextAlignment::Left
    );

    const int twoPlayerLife =
        game.getSettings()
            .twoPlayerStartingLife;

    drawSettingsButton(
        getTwoPlayer20Rectangle(),
        "20",
        twoPlayerLife == 20
    );

    drawSettingsButton(
        getTwoPlayer30Rectangle(),
        "30",
        twoPlayerLife == 30
    );

    drawSettingsButton(
        getTwoPlayer40Rectangle(),
        "40",
        twoPlayerLife == 40
    );

    drawSettingsButton(
        getTwoPlayerEditRectangle(),
        "Edit",
        twoPlayerLife != 20 &&
            twoPlayerLife != 30 &&
            twoPlayerLife != 40
    );

    drawSettingsButton(
        getResetButtonRectangle(),
        "Reset Game",
        false
    );

    drawSettingsButton(
        getDoneButtonRectangle(),
        "Done",
        false
    );

    canvas_.invalidate(panel);
}

Rect AppRenderer::getCloseButtonRectangle() const
{
    return {
        635.0F,
        35.0F,
        45.0F,
        45.0F
    };
}

Rect AppRenderer::getTwoPlayersButtonRectangle() const
{
    return {
        130.0F,
        125.0F,
        125.0F,
        44.0F
    };
}

Rect AppRenderer::getFourPlayersButtonRectangle() const
{
    return {
        270.0F,
        125.0F,
        125.0F,
        44.0F
    };
}

Rect AppRenderer::getMultiplayer40Rectangle() const
{
    return {
        130.0F,
        220.0F,
        75.0F,
        42.0F
    };
}

Rect AppRenderer::getMultiplayer30Rectangle() const
{
    return {
        215.0F,
        220.0F,
        75.0F,
        42.0F
    };
}

Rect AppRenderer::getMultiplayer20Rectangle() const
{
    return {
        300.0F,
        220.0F,
        75.0F,
        42.0F
    };
}

Rect AppRenderer::getMultiplayerEditRectangle() const
{
    return {
        385.0F,
        220.0F,
        95.0F,
        42.0F
    };
}

Rect AppRenderer::getTwoPlayer20Rectangle() const
{
    return {
        130.0F,
        315.0F,
        75.0F,
        42.0F
    };
}

Rect AppRenderer::getTwoPlayer30Rectangle() const
{
    return {
        215.0F,
        315.0F,
        75.0F,
        42.0F
    };
}

Rect AppRenderer::getTwoPlayer40Rectangle() const
{
    return {
        300.0F,
        315.0F,
        75.0F,
        42.0F
    };
}

Rect AppRenderer::getTwoPlayerEditRectangle() const
{
    return {
        385.0F,
        315.0F,
        95.0F,
        42.0F
    };
}

Rect AppRenderer::getResetButtonRectangle() const
{
    return {
        130.0F,
        390.0F,
        170.0F,
        42.0F
    };
}

Rect AppRenderer::getDoneButtonRectangle() const
{
    return {
        500.0F,
        390.0F,
        170.0F,
        42.0F
    };
}

} // namespace sticky_lotus::ui