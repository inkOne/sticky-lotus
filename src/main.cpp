#include "GameState.h"

#include <raylib.h>

#include <cstddef>

namespace {

constexpr int screenWidth = 800;
constexpr int screenHeight = 480;

constexpr int columns = 2;
constexpr int rows = 2;

constexpr int cellWidth = screenWidth / columns;
constexpr int cellHeight = screenHeight / rows;

constexpr Color overlayBackground = {
    20,
    20,
    24,
    245
};

constexpr Color panelBackground = {
    45,
    45,
    52,
    255
};

constexpr Color selectedButtonColor = {
    80,
    130,
    220,
    255
};

void drawCenteredText(
    const char* text,
    const Rectangle& area,
    const int fontSize,
    const Color color,
    const bool upsideDown = false
)
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

Rectangle getMenuButtonRectangle()
{
    constexpr float buttonSize = 58.0F;

    return {
        screenWidth / 2.0F - buttonSize / 2.0F,
        screenHeight / 2.0F - buttonSize / 2.0F,
        buttonSize,
        buttonSize
    };
}

void drawMenuButton()
{
    const Rectangle button = getMenuButtonRectangle();

    const Vector2 center = {
        button.x + button.width / 2.0F,
        button.y + button.height / 2.0F
    };

    DrawCircleV(
        center,
        button.width / 2.0F,
        DARKGRAY
    );

    DrawCircleLines(
        static_cast<int>(center.x),
        static_cast<int>(center.y),
        button.width / 2.0F,
        LIGHTGRAY
    );

    constexpr float lineWidth = 25.0F;
    constexpr float lineHeight = 3.0F;
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

        DrawRectangleRounded(
            lineArea,
            1.0F,
            4,
            WHITE
        );
    }
}

std::size_t getPlayerIndex(
    const Vector2 position,
    const std::size_t playerCount
)
{
    if (playerCount == 2) {
        return position.x < screenWidth / 2.0F
            ? 0
            : 1;
    }

    const int column =
        position.x < cellWidth ? 0 : 1;

    const int row =
        position.y < cellHeight ? 0 : 1;

    return static_cast<std::size_t>(
        row * columns + column
    );
}

int getLifeChange(
    const Vector2 position,
    const std::size_t playerCount
)
{
    const float activeCellWidth =
        playerCount == 2
            ? screenWidth / 2.0F
            : static_cast<float>(cellWidth);

    const int column =
        static_cast<int>(
            position.x / activeCellWidth
        );

    const float localX =
        position.x -
        static_cast<float>(column) * activeCellWidth;

    return localX < activeCellWidth / 2.0F
        ? -1
        : 1;
}

void drawPlayer(
    const Player& player,
    const std::size_t index,
    const Rectangle& playerArea,
    const bool upsideDown
)
{
    DrawRectangleRec(
        playerArea,
        RAYWHITE
    );

    DrawRectangleLinesEx(
        playerArea,
        2.0F,
        BLACK
    );

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
        playerArea.width - sideButtonWidth * 2.0F,
        playerArea.height
    };

    drawCenteredText(
        TextFormat("%d", player.life),
        lifeArea,
        90,
        BLACK,
        upsideDown
    );

    drawCenteredText(
        "-",
        minusArea,
        42,
        DARKGRAY,
        upsideDown
    );

    drawCenteredText(
        "+",
        plusArea,
        42,
        DARKGRAY,
        upsideDown
    );

    (void)index;
}

void drawPlayers(const GameState& game)
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
                index,
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
            index,
            playerArea,
            upsideDown
        );
    }
}

bool drawSettingsButton(
    const Rectangle& area,
    const char* label,
    const bool selected
)
{
    const Vector2 mousePosition =
        GetMousePosition();

    const bool hovered =
        CheckCollisionPointRec(
            mousePosition,
            area
        );

    Color background = selected
        ? selectedButtonColor
        : GRAY;

    if (hovered && !selected) {
        background = LIGHTGRAY;
    }

    DrawRectangleRounded(
        area,
        0.25F,
        8,
        background
    );

    DrawRectangleLinesEx(
        area,
        1.5F,
        selected ? WHITE : DARKGRAY
    );

    drawCenteredText(
        label,
        area,
        20,
        selected ? WHITE : BLACK
    );

    return hovered &&
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        );
}

void drawSettingsOverlay(
    GameState& game,
    bool& settingsOpen
)
{
    DrawRectangle(
        0,
        0,
        screenWidth,
        screenHeight,
        overlayBackground
    );

    const Rectangle panel = {
        120.0F,
        25.0F,
        560.0F,
        430.0F
    };

    DrawRectangleRounded(
        panel,
        0.04F,
        10,
        panelBackground
    );

    DrawRectangleLinesEx(
        panel,
        2.0F,
        LIGHTGRAY
    );

    DrawText(
        "Settings",
        150,
        48,
        32,
        WHITE
    );

    const Rectangle closeButton = {
        620.0F,
        42.0F,
        38.0F,
        38.0F
    };

    if (drawSettingsButton(
        closeButton,
        "X",
        false
    )) {
        settingsOpen = false;
        return;
    }

    DrawText(
        "Players",
        150,
        100,
        22,
        LIGHTGRAY
    );

    const Rectangle twoPlayersButton = {
        150.0F,
        130.0F,
        120.0F,
        44.0F
    };

    const Rectangle fourPlayersButton = {
        285.0F,
        130.0F,
        120.0F,
        44.0F
    };

    const bool twoPlayersSelected =
        game.getPlayerCount() == 2;

    const bool fourPlayersSelected =
        game.getPlayerCount() == 4;

    if (drawSettingsButton(
        twoPlayersButton,
        "2 Players",
        twoPlayersSelected
    )) {
        game.setPlayerMode(
            PlayerMode::TwoPlayers
        );
    }

    if (drawSettingsButton(
        fourPlayersButton,
        "4 Players",
        fourPlayersSelected
    )) {
        game.setPlayerMode(
            PlayerMode::FourPlayers
        );
    }

    DrawText(
        "Multiplayer Starting Life",
        150,
        195,
        22,
        LIGHTGRAY
    );

    const int multiplayerLife =
        game.getSettings()
            .multiplayerStartingLife;

    const Rectangle multiplayer40 = {
        150.0F,
        225.0F,
        75.0F,
        42.0F
    };

    const Rectangle multiplayer30 = {
        235.0F,
        225.0F,
        75.0F,
        42.0F
    };

    const Rectangle multiplayer20 = {
        320.0F,
        225.0F,
        75.0F,
        42.0F
    };

    const Rectangle multiplayerEdit = {
        405.0F,
        225.0F,
        95.0F,
        42.0F
    };

    if (drawSettingsButton(
        multiplayer40,
        "40",
        multiplayerLife == 40
    )) {
        game.setMultiplayerStartingLife(40);
    }

    if (drawSettingsButton(
        multiplayer30,
        "30",
        multiplayerLife == 30
    )) {
        game.setMultiplayerStartingLife(30);
    }

    if (drawSettingsButton(
        multiplayer20,
        "20",
        multiplayerLife == 20
    )) {
        game.setMultiplayerStartingLife(20);
    }

    if (drawSettingsButton(
        multiplayerEdit,
        "Edit",
        multiplayerLife != 40 &&
            multiplayerLife != 30 &&
            multiplayerLife != 20
    )) {
        TraceLog(
            LOG_INFO,
            "Custom multiplayer life editor"
        );
    }

    DrawText(
        "Two Player Starting Life",
        150,
        290,
        22,
        LIGHTGRAY
    );

    const int twoPlayerLife =
        game.getSettings()
            .twoPlayerStartingLife;

    const Rectangle twoPlayer20 = {
        150.0F,
        320.0F,
        75.0F,
        42.0F
    };

    const Rectangle twoPlayer30 = {
        235.0F,
        320.0F,
        75.0F,
        42.0F
    };

    const Rectangle twoPlayer40 = {
        320.0F,
        320.0F,
        75.0F,
        42.0F
    };

    const Rectangle twoPlayerEdit = {
        405.0F,
        320.0F,
        95.0F,
        42.0F
    };

    if (drawSettingsButton(
        twoPlayer20,
        "20",
        twoPlayerLife == 20
    )) {
        game.setTwoPlayerStartingLife(20);
    }

    if (drawSettingsButton(
        twoPlayer30,
        "30",
        twoPlayerLife == 30
    )) {
        game.setTwoPlayerStartingLife(30);
    }

    if (drawSettingsButton(
        twoPlayer40,
        "40",
        twoPlayerLife == 40
    )) {
        game.setTwoPlayerStartingLife(40);
    }

    if (drawSettingsButton(
        twoPlayerEdit,
        "Edit",
        twoPlayerLife != 20 &&
            twoPlayerLife != 30 &&
            twoPlayerLife != 40
    )) {
        TraceLog(
            LOG_INFO,
            "Custom two player life editor"
        );
    }

    const Rectangle resetButton = {
        150.0F,
        390.0F,
        170.0F,
        42.0F
    };

    if (drawSettingsButton(
        resetButton,
        "Reset Game",
        false
    )) {
        game.reset();
        settingsOpen = false;
    }

    const Rectangle doneButton = {
        470.0F,
        390.0F,
        170.0F,
        42.0F
    };

    if (drawSettingsButton(
        doneButton,
        "Done",
        false
    )) {
        settingsOpen = false;
    }
}

}

int main()
{
    InitWindow(
        screenWidth,
        screenHeight,
        "Sticky Lotus Simulator"
    );

    SetTargetFPS(60);

    GameState game;
    bool settingsOpen = false;

    while (!WindowShouldClose()) {
        if (
            !settingsOpen &&
            IsMouseButtonPressed(
                MOUSE_BUTTON_LEFT
            )
        ) {
            const Vector2 pointer =
                GetMousePosition();

            const Rectangle menuButton =
                getMenuButtonRectangle();

            if (CheckCollisionPointRec(
                pointer,
                menuButton
            )) {
                settingsOpen = true;
            } else {
                const std::size_t playerIndex =
                    getPlayerIndex(
                        pointer,
                        game.getPlayerCount()
                    );

                if (
                    playerIndex <
                    game.getPlayerCount()
                ) {
                    game.changeLife(
                        playerIndex,
                        getLifeChange(
                            pointer,
                            game.getPlayerCount()
                        )
                    );
                }
            }
        }

        if (
            !settingsOpen &&
            IsKeyPressed(KEY_R)
        ) {
            game.reset();
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            settingsOpen = false;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        drawPlayers(game);
        drawMenuButton();

        if (settingsOpen) {
            drawSettingsOverlay(
                game,
                settingsOpen
            );
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}