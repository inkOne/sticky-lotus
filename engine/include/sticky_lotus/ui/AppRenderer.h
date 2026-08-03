#pragma once

#include "sticky_lotus/GameState.h"
#include "sticky_lotus/ui/Canvas.h"
#include "sticky_lotus/ui/Geometry.h"

namespace sticky_lotus::ui {

/**
 * Plattformunabhängiger Renderer der Sticky-Lotus-Anwendung.
 *
 * Diese Klasse kennt weder Raylib noch ESP-IDF.
 * Sämtliche Ausgabe erfolgt ausschließlich über Canvas.
 */
class AppRenderer
{
public:
    static constexpr int screenWidth = 800;
    static constexpr int screenHeight = 480;

    explicit AppRenderer(Canvas& canvas);

    /**
     * Zeichnet die normale Spielansicht.
     */
    void drawGame(const GameState& game);

    /**
     * Zeichnet die Settings-Ansicht.
     */
    void drawSettings(const GameState& game);

    // Bereiche, die auch für die Eingabeverarbeitung benötigt werden.

    [[nodiscard]]
    Rect getMenuButtonRectangle() const;

    [[nodiscard]]
    Rect getCloseButtonRectangle() const;

    [[nodiscard]]
    Rect getTwoPlayersButtonRectangle() const;

    [[nodiscard]]
    Rect getFourPlayersButtonRectangle() const;

    [[nodiscard]]
    Rect getMultiplayer40Rectangle() const;

    [[nodiscard]]
    Rect getMultiplayer30Rectangle() const;

    [[nodiscard]]
    Rect getMultiplayer20Rectangle() const;

    [[nodiscard]]
    Rect getMultiplayerEditRectangle() const;

    [[nodiscard]]
    Rect getTwoPlayer20Rectangle() const;

    [[nodiscard]]
    Rect getTwoPlayer30Rectangle() const;

    [[nodiscard]]
    Rect getTwoPlayer40Rectangle() const;

    [[nodiscard]]
    Rect getTwoPlayerEditRectangle() const;

    [[nodiscard]]
    Rect getResetButtonRectangle() const;

    [[nodiscard]]
    Rect getDoneButtonRectangle() const;

private:
    static constexpr int columns = 2;
    static constexpr int rows = 2;

    static constexpr int cellWidth =
        screenWidth / columns;

    static constexpr int cellHeight =
        screenHeight / rows;

    Canvas& canvas_;

    void drawPlayers(const GameState& game);

    void drawPlayer(
        const Player& player,
        const Rect& playerArea,
        bool upsideDown
    );

    void drawMenuButton();

    void drawSettingsButton(
        const Rect& area,
        const char* label,
        bool selected
    );
};

} // namespace sticky_lotus::ui