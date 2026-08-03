#pragma once

#include "sticky_lotus/GameState.h"

#include <raylib.h>

namespace sticky_lotus_sim {

/**
 * Zeichnet die Sticky-Lotus-Oberfläche im Raylib-Simulator.
 *
 * Diese Klasse enthält ausschließlich Darstellungslogik.
 * Spiellogik, Navigation und Eingabeverarbeitung gehören nicht hier hinein.
 *
 * Der Renderer verwendet bewusst nur Schwarz, Weiß und zwei Graustufen,
 * damit die Darstellung ungefähr den Möglichkeiten des E-Ink-Displays
 * des Sticky entspricht.
 */
class RaylibRenderer
{
public:
    static constexpr int screenWidth = 800;
    static constexpr int screenHeight = 480;

    /**
     * Zeichnet die normale Spielansicht.
     */
    void drawGame(const GameState& game) const;

    /**
     * Zeichnet das Settings-Overlay.
     *
     * Die Auswertung der Buttons erfolgt weiterhin in main.cpp.
     */
    void drawSettings(const GameState& game) const;

    /**
     * Zeichnet den zentralen Menüknopf.
     */
    void drawMenuButton() const;

    /**
     * Liefert den anklickbaren Bereich des Menüknopfes.
     */
    [[nodiscard]]
    Rectangle getMenuButtonRectangle() const;

    /**
     * Liefert die Bereiche der Settings-Buttons.
     *
     * Diese Funktionen werden vorerst auch von main.cpp genutzt,
     * damit Eingabe und Darstellung dieselben Koordinaten verwenden.
     */
    [[nodiscard]] Rectangle getCloseButtonRectangle() const;
    [[nodiscard]] Rectangle getTwoPlayersButtonRectangle() const;
    [[nodiscard]] Rectangle getFourPlayersButtonRectangle() const;

    [[nodiscard]] Rectangle getMultiplayer40Rectangle() const;
    [[nodiscard]] Rectangle getMultiplayer30Rectangle() const;
    [[nodiscard]] Rectangle getMultiplayer20Rectangle() const;
    [[nodiscard]] Rectangle getMultiplayerEditRectangle() const;

    [[nodiscard]] Rectangle getTwoPlayer20Rectangle() const;
    [[nodiscard]] Rectangle getTwoPlayer30Rectangle() const;
    [[nodiscard]] Rectangle getTwoPlayer40Rectangle() const;
    [[nodiscard]] Rectangle getTwoPlayerEditRectangle() const;

    [[nodiscard]] Rectangle getResetButtonRectangle() const;
    [[nodiscard]] Rectangle getDoneButtonRectangle() const;

private:
    static constexpr int columns = 2;
    static constexpr int rows = 2;

    static constexpr int cellWidth =
        screenWidth / columns;

    static constexpr int cellHeight =
        screenHeight / rows;

    static constexpr Color inkBlack = {
        0,
        0,
        0,
        255
    };

    static constexpr Color inkDarkGray = {
        85,
        85,
        85,
        255
    };

    static constexpr Color inkLightGray = {
        190,
        190,
        190,
        255
    };

    static constexpr Color inkWhite = {
        255,
        255,
        255,
        255
    };

    void drawCenteredText(
        const char* text,
        const Rectangle& area,
        int fontSize,
        Color color,
        bool upsideDown = false
    ) const;

    void drawPlayer(
        const Player& player,
        const Rectangle& playerArea,
        bool upsideDown
    ) const;

    void drawPlayers(const GameState& game) const;

    void drawSettingsButton(
        const Rectangle& area,
        const char* label,
        bool selected
    ) const;
};

}