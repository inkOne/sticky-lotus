#include "GameState.h"

#include <raylib.h>

#include <cstddef>

namespace {

// -----------------------------------------------------------------------------
// Display-Konfiguration
// -----------------------------------------------------------------------------
//
// Der reTerminal Sticky besitzt ein Display mit 800 × 480 Pixeln.
// Deshalb verwendet auch der Simulator exakt diese Auflösung.
//
// Das echte Gerät ist ein monochromes E-Ink-Display mit vier Graustufen.
// Im Simulator verwenden wir deshalb ausschließlich Schwarz, Weiß und Grau.
//
constexpr int screenWidth = 800;
constexpr int screenHeight = 480;

constexpr int columns = 2;
constexpr int rows = 2;

constexpr int cellWidth = screenWidth / columns;
constexpr int cellHeight = screenHeight / rows;

// -----------------------------------------------------------------------------
// Monochrome Farbpalette
// -----------------------------------------------------------------------------
//
// Diese vier Werte entsprechen sinngemäß den vier Graustufen des Sticky.
// Die konkreten Pixelwerte werden beim späteren Sticky-Renderer angepasst.
//
constexpr Color inkBlack = {
    0,
    0,
    0,
    255
};

constexpr Color inkDarkGray = {
    85,
    85,
    85,
    255
};

constexpr Color inkLightGray = {
    190,
    190,
    190,
    255
};

constexpr Color inkWhite = {
    255,
    255,
    255,
    255
};

// -----------------------------------------------------------------------------
// Zentrierten Text zeichnen
// -----------------------------------------------------------------------------
//
// Diese Funktion zeichnet einen Text mittig innerhalb eines Rechtecks.
//
// upsideDown:
// true  -> Text wird um 180 Grad gedreht
// false -> Text wird normal dargestellt
//
// Die Drehung wird für die oberen beiden Spieler verwendet.
//
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

// -----------------------------------------------------------------------------
// Rechteck des zentralen Menüknopfes
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Zentralen Menüknopf zeichnen
// -----------------------------------------------------------------------------
//
// Der Knopf verwendet keine Farben, sondern nur:
// - weißen Hintergrund
// - schwarze Umrandung
// - drei schwarze Linien
//
// Dadurch ist er für E-Ink geeignet.
//
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

// -----------------------------------------------------------------------------
// Spieler anhand einer Touch- oder Mausposition bestimmen
// -----------------------------------------------------------------------------
std::size_t getPlayerIndex(
    const Vector2 position,
    const std::size_t playerCount
)
{
    // Bei zwei Spielern wird der Bildschirm vertikal geteilt.
    if (playerCount == 2) {
        return position.x < screenWidth / 2.0F
            ? 0
            : 1;
    }

    // Bei vier Spielern wird ein 2 × 2-Raster verwendet.
    const int column =
        position.x < cellWidth ? 0 : 1;

    const int row =
        position.y < cellHeight ? 0 : 1;

    return static_cast<std::size_t>(
        row * columns + column
    );
}

// -----------------------------------------------------------------------------
// Ermitteln, ob Leben addiert oder abgezogen werden soll
// -----------------------------------------------------------------------------
//
// Linke Hälfte eines Spielerfeldes  -> -1
// Rechte Hälfte eines Spielerfeldes -> +1
//
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

// -----------------------------------------------------------------------------
// Einzelnes Spielerfeld zeichnen
// -----------------------------------------------------------------------------
void drawPlayer(
    const Player& player,
    const Rectangle& playerArea,
    const bool upsideDown
)
{
    // Weißer Hintergrund.
    DrawRectangleRec(
        playerArea,
        inkWhite
    );

    // Schwarze Feldumrandung.
    DrawRectangleLinesEx(
        playerArea,
        2.0F,
        inkBlack
    );

    // Spielernamen werden absichtlich nicht angezeigt.
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

    // Linker Bedienbereich.
    const Rectangle minusArea = {
        playerArea.x + sideMargin,
        playerArea.y,
        sideButtonWidth,
        playerArea.height
    };

    // Rechter Bedienbereich.
    const Rectangle plusArea = {
        playerArea.x +
            playerArea.width -
            sideButtonWidth -
            sideMargin,
        playerArea.y,
        sideButtonWidth,
        playerArea.height
    };

    // Mittlerer Bereich für die Lebenspunkte.
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

// -----------------------------------------------------------------------------
// Alle Spielerfelder zeichnen
// -----------------------------------------------------------------------------
void drawPlayers(const GameState& game)
{
    const std::size_t playerCount =
        game.getPlayerCount();

    // Zwei-Spieler-Modus.
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

            // Spieler 1 steht auf der gegenüberliegenden Seite
            // und wird deshalb um 180 Grad gedreht.
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

    // Vier-Spieler-Modus.
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

        // Die oberen beiden Spielerfelder stehen auf dem Kopf.
        const bool upsideDown =
            index == 0 || index == 1;

        drawPlayer(
            game.getPlayer(index),
            playerArea,
            upsideDown
        );
    }
}

// -----------------------------------------------------------------------------
// Einstellungsbutton zeichnen
// -----------------------------------------------------------------------------
//
// Der Button verwendet keine Farben.
//
// Ausgewählt:
// - schwarzer Hintergrund
// - weiße Schrift
//
// Nicht ausgewählt:
// - weißer Hintergrund
// - schwarze Schrift
//
bool drawSettingsButton(
    const Rectangle& area,
    const char* label,
    const bool selected
)
{
    const Vector2 pointer =
        GetMousePosition();

    const bool hovered =
        CheckCollisionPointRec(
            pointer,
            area
        );

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
        hovered ? 3.0F : 2.0F,
        inkBlack
    );

    drawCenteredText(
        label,
        area,
        20,
        foreground
    );

    return hovered &&
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        );
}

// -----------------------------------------------------------------------------
// Settings-Overlay zeichnen
// -----------------------------------------------------------------------------
void drawSettingsOverlay(
    GameState& game,
    bool& settingsOpen
)
{
    // Das Overlay ersetzt den bisherigen Bildinhalt vollständig.
    //
    // Für das echte E-Ink-Gerät ist das besser als transparente
    // Überlagerungen, da Alpha-Blending dort nicht sinnvoll ist.
    DrawRectangle(
        0,
        0,
        screenWidth,
        screenHeight,
        inkWhite
    );

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

    // Schließen-Button.
    const Rectangle closeButton = {
        635.0F,
        35.0F,
        45.0F,
        45.0F
    };

    if (drawSettingsButton(
        closeButton,
        "X",
        false
    )) {
        settingsOpen = false;
        return;
    }

    // -------------------------------------------------------------------------
    // Spielerzahl
    // -------------------------------------------------------------------------
    DrawText(
        "Players",
        130,
        95,
        22,
        inkBlack
    );

    const Rectangle twoPlayersButton = {
        130.0F,
        125.0F,
        125.0F,
        44.0F
    };

    const Rectangle fourPlayersButton = {
        270.0F,
        125.0F,
        125.0F,
        44.0F
    };

    if (drawSettingsButton(
        twoPlayersButton,
        "2 Players",
        game.getPlayerCount() == 2
    )) {
        game.setPlayerMode(
            PlayerMode::TwoPlayers
        );
    }

    if (drawSettingsButton(
        fourPlayersButton,
        "4 Players",
        game.getPlayerCount() == 4
    )) {
        game.setPlayerMode(
            PlayerMode::FourPlayers
        );
    }

    // -------------------------------------------------------------------------
    // Multiplayer Starting Life
    // -------------------------------------------------------------------------
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

    const Rectangle multiplayer40 = {
        130.0F,
        220.0F,
        75.0F,
        42.0F
    };

    const Rectangle multiplayer30 = {
        215.0F,
        220.0F,
        75.0F,
        42.0F
    };

    const Rectangle multiplayer20 = {
        300.0F,
        220.0F,
        75.0F,
        42.0F
    };

    const Rectangle multiplayerEdit = {
        385.0F,
        220.0F,
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

    // -------------------------------------------------------------------------
    // Two Player Starting Life
    // -------------------------------------------------------------------------
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

    const Rectangle twoPlayer40 = {
        130.0F,
        315.0F,
        75.0F,
        42.0F
    };

    const Rectangle twoPlayer30 = {
        215.0F,
        315.0F,
        75.0F,
        42.0F
    };

    const Rectangle twoPlayer20 = {
        300.0F,
        315.0F,
        75.0F,
        42.0F
    };

    const Rectangle twoPlayerEdit = {
        385.0F,
        315.0F,
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

    // -------------------------------------------------------------------------
    // Reset und Done
    // -------------------------------------------------------------------------
    const Rectangle resetButton = {
        130.0F,
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
        500.0F,
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

} // namespace

int main()
{
    // -------------------------------------------------------------------------
    // Desktop-Simulator initialisieren
    // -------------------------------------------------------------------------
    //
    // InitWindow gehört ausschließlich zum Raylib-Simulator.
    // Auf dem Sticky wird später stattdessen der Displaytreiber initialisiert.
    //
    InitWindow(
        screenWidth,
        screenHeight,
        "Sticky Lotus Simulator"
    );

    SetTargetFPS(30);

    GameState game;
    bool settingsOpen = false;

    while (!WindowShouldClose()) {
        // ---------------------------------------------------------------------
        // Eingabe verarbeiten
        // ---------------------------------------------------------------------
        //
        // Im Simulator verwenden wir die Maus.
        // Auf dem Sticky wird diese Stelle später durch Touch-Ereignisse ersetzt.
        //
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

        // Reset-Taste nur für den Simulator.
        if (
            !settingsOpen &&
            IsKeyPressed(KEY_R)
        ) {
            game.reset();
        }

        // Escape schließt im Simulator das Overlay.
        if (IsKeyPressed(KEY_ESCAPE)) {
            settingsOpen = false;
        }

        // ---------------------------------------------------------------------
        // Bildschirm zeichnen
        // ---------------------------------------------------------------------
        //
        // Raylib zeichnet hier weiterhin jedes Frame neu.
        //
        // Auf dem Sticky darf später nur dann aktualisiert werden, wenn sich
        // tatsächlich etwas geändert hat, zum Beispiel:
        //
        // - Lebenspunkte geändert
        // - Menü geöffnet
        // - Einstellung geändert
        // - Commander Damage geändert
        //
        BeginDrawing();

        ClearBackground(inkWhite);

        if (settingsOpen) {
            drawSettingsOverlay(
                game,
                settingsOpen
            );
        } else {
            drawPlayers(game);
            drawMenuButton();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}