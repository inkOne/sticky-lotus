#include "sticky_lotus_sim/RaylibRenderer.h"

#include <cstddef>

namespace sticky_lotus_sim {

void RaylibRenderer::drawCenteredText(
    const char* text,
    const Rectangle& area,
    const int fontSize,
    const Color color,
    const bool upsideDown
) const
{
    const Font font = GetFontDefault();

    const Vector2 textSize = MeasureTextEx(
        font,
        text,
        static_cast<float>(fontSize),
        1.0F
    );

    const Vector2 center = {
        area.x + area.width / 2.0F,
        area.y + area.height / 2.0F
    };

    const Vector2 origin = {
        textSize.x / 2.0F,
        textSize.y / 2.0F
    };

    DrawTextPro(
        font,
        text,
        center,
        origin,
        upsideDown ? 180.0F : 0.0F,
        static_cast<float>(fontSize),
        1.0F,
        color
    );
}

Rectangle RaylibRenderer::getMenuButtonRectangle() const
{
    constexpr float buttonSize = 58.0F;

    return {
        screenWidth / 2.0F - buttonSize / 2.0F,
        screenHeight / 2.0F - buttonSize / 2.0F,
        buttonSize,
        buttonSize
    };
}

void RaylibRenderer::drawMenuButton() const
{
    const Rectangle button =
        getMenuButtonRectangle();

    const Vector2 center = {
        button.x + button.width / 2.0F,
        button.y + button.height / 2.0F
    };

    DrawCircleV(
        center,
        button.width / 2.0F,
        inkWhite
    );

    DrawCircleLines(
        static_cast<int>(center.x),
        static_cast<int>(center.y),
        button.width / 2.0F,
        inkBlack
    );

    constexpr float lineWidth = 25.0F;
    constexpr float lineHeight = 4.0F;
    constexpr float lineSpacing = 8.0F;

    for (int line = -1; line <= 1; ++line) {
        const Rectangle lineArea = {
            center.x - lineWidth / 2.0F,
            center.y +
                static_cast<float>(line) * lineSpacing -
                lineHeight / 2.0F,
            lineWidth,
            lineHeight
        };

        DrawRectangleRec(
            lineArea,
            inkBlack
        );
    }
}

void RaylibRenderer::drawPlayer(
    const Player& player,
    const Rectangle& playerArea,
    const bool upsideDown
) const
{
    DrawRectangleRec(
        playerArea,
        inkWhite
    );

    DrawRectangleLinesEx(
        playerArea,
        2.0F,
        inkBlack
    );

    // Spielernamen bleiben bewusst ausgeblendet.
    //
    // DrawText(
    //     player.name.c_str(),
    //     static_cast<int>(playerArea.x + 20.0F),
    //     static_cast<int>(playerArea.y + 20.0F),
    //     24,
    //     inkBlack
    // );

    constexpr float sideButtonWidth = 75.0F;
    constexpr float sideMargin = 15.0F;

    const Rectangle minusArea = {
        playerArea.x + sideMargin,
        playerArea.y,
        sideButtonWidth,
        playerArea.height
    };

    const Rectangle plusArea = {
        playerArea.x +
            playerArea.width -
            sideButtonWidth -
            sideMargin,
        playerArea.y,
        sideButtonWidth,
        playerArea.height
    };

    const Rectangle lifeArea = {
        playerArea.x + sideButtonWidth,
        playerArea.y,
        playerArea.width -
            sideButtonWidth * 2.0F,
        playerArea.height
    };

    drawCenteredText(
        TextFormat("%d", player.life),
        lifeArea,
        90,
        inkBlack,
        upsideDown
    );

    drawCenteredText(
        "-",
        minusArea,
        42,
        inkDarkGray,
        upsideDown
    );

    drawCenteredText(
        "+",
        plusArea,
        42,
        inkDarkGray,
        upsideDown
    );
}

void RaylibRenderer::drawPlayers(
    const GameState& game
) const
{
    const std::size_t playerCount =
        game.getPlayerCount();

    if (playerCount == 2) {
        for (
            std::size_t index = 0;
            index < playerCount;
            ++index
        ) {
            const Rectangle playerArea = {
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
            static_cast<int>(
                index % columns
            );

        const int row =
            static_cast<int>(
                index / columns
            );

        const Rectangle playerArea = {
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

void RaylibRenderer::drawGame(
    const GameState& game
) const
{
    ClearBackground(inkWhite);

    drawPlayers(game);
    drawMenuButton();
}

void RaylibRenderer::drawSettingsButton(
    const Rectangle& area,
    const char* label,
    const bool selected
) const
{
    const Color background =
        selected
            ? inkBlack
            : inkWhite;

    const Color foreground =
        selected
            ? inkWhite
            : inkBlack;

    DrawRectangleRec(
        area,
        background
    );

    DrawRectangleLinesEx(
        area,
        2.0F,
        inkBlack
    );

    drawCenteredText(
        label,
        area,
        20,
        foreground
    );
}

Rectangle RaylibRenderer::getCloseButtonRectangle() const
{
    return {
        635.0F,
        35.0F,
        45.0F,
        45.0F
    };
}

Rectangle RaylibRenderer::getTwoPlayersButtonRectangle() const
{
    return {
        130.0F,
        125.0F,
        125.0F,
        44.0F
    };
}

Rectangle RaylibRenderer::getFourPlayersButtonRectangle() const
{
    return {
        270.0F,
        125.0F,
        125.0F,
        44.0F
    };
}

Rectangle RaylibRenderer::getMultiplayer40Rectangle() const
{
    return {
        130.0F,
        220.0F,
        75.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getMultiplayer30Rectangle() const
{
    return {
        215.0F,
        220.0F,
        75.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getMultiplayer20Rectangle() const
{
    return {
        300.0F,
        220.0F,
        75.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getMultiplayerEditRectangle() const
{
    return {
        385.0F,
        220.0F,
        95.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getTwoPlayer20Rectangle() const
{
    return {
        130.0F,
        315.0F,
        75.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getTwoPlayer30Rectangle() const
{
    return {
        215.0F,
        315.0F,
        75.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getTwoPlayer40Rectangle() const
{
    return {
        300.0F,
        315.0F,
        75.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getTwoPlayerEditRectangle() const
{
    return {
        385.0F,
        315.0F,
        95.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getResetButtonRectangle() const
{
    return {
        130.0F,
        390.0F,
        170.0F,
        42.0F
    };
}

Rectangle RaylibRenderer::getDoneButtonRectangle() const
{
    return {
        500.0F,
        390.0F,
        170.0F,
        42.0F
    };
}

void RaylibRenderer::drawSettings(
    const GameState& game
) const
{
    ClearBackground(inkWhite);

    const Rectangle panel = {
        100.0F,
        20.0F,
        600.0F,
        440.0F
    };

    DrawRectangleRec(
        panel,
        inkWhite
    );

    DrawRectangleLinesEx(
        panel,
        3.0F,
        inkBlack
    );

    DrawText(
        "Settings",
        130,
        42,
        32,
        inkBlack
    );

    drawSettingsButton(
        getCloseButtonRectangle(),
        "X",
        false
    );

    DrawText(
        "Players",
        130,
        95,
        22,
        inkBlack
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

    DrawText(
        "Multiplayer Starting Life",
        130,
        190,
        22,
        inkBlack
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

    DrawText(
        "Two Player Starting Life",
        130,
        285,
        22,
        inkBlack
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
}

}