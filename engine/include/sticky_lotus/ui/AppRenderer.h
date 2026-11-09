#pragma once

#include "sticky_lotus/GameState.h"
#include "sticky_lotus/ui/Canvas.h"
#include "sticky_lotus/ui/Geometry.h"
#include <cstddef>
#include "sticky_lotus/commander/CommanderDamageDraft.h"
#include "sticky_lotus/ui/TableLayout.h"

class GameState;

namespace sticky_lotus::ui
{
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

        AppRenderer(
            Canvas& canvas
        );

        /**
         * Zeichnet die normale Spielansicht.
         */
        void drawGame(const GameState& game);
        void drawGameImmediately(
            const GameState& game
        );
        /**
         * Zeichnet die Settings-Ansicht.
         */
        void drawSettings(const GameState& game);

        void drawCommanderDamage(
            const GameState& game,
            const commander::CommanderDamageDraft& draft
        );

        [[nodiscard]]
        Rect getMenuButtonRectangle() const;

        [[nodiscard]]
        Rect getCloseButtonRectangle() const;

        [[nodiscard]]
        Rect getTwoPlayersButtonRectangle() const;

        [[nodiscard]]
        Rect getFourPlayersButtonRectangle() const;

        [[nodiscard]]
        Rect getSoundOnButtonRectangle() const;

        [[nodiscard]]
        Rect getSoundOffButtonRectangle() const;

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
        Rect getTwoPlayer25Rectangle() const;

        [[nodiscard]]
        Rect getTwoPlayer30Rectangle() const;

        [[nodiscard]]
        Rect getTwoPlayer40Rectangle() const;

        [[nodiscard]]
        Rect getTwoPlayerEditRectangle() const;

        [[nodiscard]]
        Rect getResetButtonRectangle() const;

        [[nodiscard]]
        Rect getRollStartButtonRectangle() const;

        [[nodiscard]]
        Rect getDoneButtonRectangle() const;

        [[nodiscard]]
        Rect getCommanderDamageMinusRectangle(
            std::size_t attackingPlayer,
            std::size_t playerCount
        ) const;

        [[nodiscard]]
        Rect getCommanderDamagePlusRectangle(
            std::size_t attackingPlayer,
            std::size_t playerCount
        ) const;
        void drawPlayerModeSettingsButtons(
            const GameState& game
        );

        void drawSoundSettingsButtons(
            const GameState& game
        );

        void drawMultiplayerLifeSettingsButtons(
            const GameState& game
        );

        void drawTwoPlayerLifeSettingsButtons(
            const GameState& game
        );
        void drawUiButton(
            const Rect& area,
            const char* label,
            bool selected,
            float rotationDegrees = 0.0F
        );
        void drawPoison(
            const GameState& game,
            std::size_t playerIndex
        );

        [[nodiscard]]
        Rect getPoisonPlusRectangle(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;

        [[nodiscard]]
        Rect getPoisonMinusRectangle(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;

        Rect getPlayerRectangle(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;

        [[nodiscard]]
        bool isPlayerUpsideDown(
            std::size_t playerIndex,
            std::size_t playerCount
        ) const;

        void setBatteryPercent(
            int batteryPercent
        );
        void setBatteryStatus(
            int batteryPercent,
            bool charging
        );

        void drawSleepScreen();

        void drawPlayerRegion(
            const GameState& game,
            std::size_t playerIndex
        );
        void drawPoisonRegion(
            const GameState& game,
            std::size_t selectedPlayer
        );

        void drawCommanderDamageRegion(
            const GameState& game,
            const commander::CommanderDamageDraft& draft,
            std::size_t playerIndex
        );
        void drawLifeCounterRegion(
            const GameState& game,
            std::size_t playerIndex
        );
        void flush();

    private:
        Canvas& canvas_;
        TableLayout tableLayout_;

        void drawPlayers(const GameState& game);

        void drawPlayer(
            const GameState& game,
            std::size_t playerIndex,
            const Rect& playerArea,
            bool upsideDown
        );


        void drawPixelPoison(
            const Rect& area,
            float rotationDegrees
        );

        void drawPixelIcon(
            const int* pixels,
            int gridSize,
            const Rect& area,
            float rotationDegrees,
            Ink ink = Ink::Black
        );

        void drawTriangleUp(
            const Rect& area,
            Ink ink,
            bool upsideDown
        );

        void drawTriangleDown(
            const Rect& area,
            Ink ink,
            bool upsideDown
        );

        void drawTriangleLeft(
            const Rect& area,
            Ink ink
        );

        void drawTriangleRight(
            const Rect& area,
            Ink ink
        );

        void drawGestureHints(
            const Rect& playerArea,
            float rotation
        );

        /**
         * Hintergrundfarbe eines Spielerfeldes auf dem Game-Screen.
         */
        [[nodiscard]]
        Ink playerBackgroundInk(
            const GameState& game,
            std::size_t playerIndex
        ) const;

        /**
         * Zeichnet die Anzeige der zuletzt veränderten Lebenspunkte
         * eines Spielers (z.B. "-5") in dessen oberer linker Ecke
         * (Minus) oder oberer rechter Ecke (Plus).
         *
         * Löscht zunächst beide Ecken, damit ein Vorzeichenwechsel
         * oder das Verschwinden nach Ablauf des Timeouts sauber
         * ohne Bildreste dargestellt wird.
         */
        void drawLifeDeltaBadge(
            const GameState& game,
            std::size_t playerIndex,
            const Rect& playerArea,
            bool upsideDown
        );

        /**
         * Analog zu drawLifeDeltaBadge(), aber für die Giftzähler-
         * Änderung auf dem Poison-Screen. Selbe Eckenposition.
         */
        void drawPoisonDeltaBadge(
            const GameState& game,
            std::size_t playerIndex,
            const Rect& playerArea,
            bool upsideDown
        );

        /**
         * Analog zu drawLifeDeltaBadge(), aber für die Schadens-
         * änderung eines einzelnen Angreifers auf dem Commander-
         * Damage-Screen.
         *
         * Nutzt bewusst die untere statt die obere Ecke (siehe
         * getCommanderDamageBadgeRectangle()), da oben bereits die
         * Spielernummer und - im Empfängerfeld - die Überschrift
         * bzw. der Swipe-Hinweis liegen.
         */
        void drawCommanderDamageDeltaBadge(
            const commander::CommanderDamageDraft& draft,
            std::size_t attackingPlayer,
            const Rect& playerArea,
            bool upsideDown
        );

        /**
         * Liefert die Ecke eines Spielerfeldes für eine Zähler-
         * Änderungsanzeige (Leben, Gift).
         *
         * "leftSide" bezieht sich auf die Sicht des Spielers selbst
         * (links = Minus, rechts = Plus). Bei umgedrehten Spielern
         * liegt deren "oben links" physisch in der unteren rechten
         * Bildschirmecke des Feldes - analog zur Vertauschung von
         * Minus- und Plus-Bereich in drawPlayer().
         */
        [[nodiscard]]
        Rect getCornerBadgeRectangle(
            const Rect& playerArea,
            bool leftSide,
            bool upsideDown
        ) const;

        /**
         * Wie getCornerBadgeRectangle(), aber für die untere statt
         * die obere Ecke - siehe drawCommanderDamageDeltaBadge().
         */
        [[nodiscard]]
        Rect getCommanderDamageBadgeRectangle(
            const Rect& playerArea,
            bool leftSide,
            bool upsideDown
        ) const;

        /**
         * Y-Position "distanceFromOuterEdge" Pixel von der
         * Außenkante eines Spielerfeldes entfernt.
         *
         * Bei normalen Spielern ist die Außenkante der untere
         * Bildschirmrand, bei umgedrehten Spielern (obere
         * Feldreihe) der obere Bildschirmrand.
         *
         * Dadurch liegen Statustexte wie "Swipe ... to save"
         * immer Richtung Bildschirmrand statt Richtung Mitte/
         * Menü-Button.
         */
        float outwardOffsetFromEdge(
            const Rect& playerArea,
            bool upsideDown,
            float distanceFromOuterEdge
        ) const;

        /**
         * Beschriftet die drei Gehäusetasten (Roll, Reset, Power/IO)
         * oben links, um 180 Grad gedreht. Auf Game- und Settings-
         * Screen; "large" (Settings) verdoppelt die Schriftgröße.
         */
        void drawButtonHints(
            bool large
        );

        /**
         * Zeichnet die Tasten-Hinweise neu, falls dirtyArea sie
         * überdeckt hat (z.B. beim Redraw eines unteren Spielerfeldes).
         */
        void redrawButtonHintsIfCovered(
            const Rect& dirtyArea
        );

        [[nodiscard]]
        Rect getButtonHintsRectangle() const;

        void drawMenuButton();

        void drawSettingsButton(
            const Rect& area,
            const char* label,
            bool selected
        );
        void drawSettingsPanel(
            const Rect& area
        );

        void drawBatteryPercent();

        void drawChargingBolt(
            Point origin
        );

        int batteryPercent_ = -1;
        bool batteryCharging_ = false;
        /**
     * Berechnet die sichtbare Zeile eines Zielspielers.
     *
     * Der Quellspieler selbst wird nicht dargestellt.
     */
        [[nodiscard]]
        static std::size_t getCommanderDamageRowIndex(
            std::size_t sourcePlayer,
            std::size_t targetPlayer,
            std::size_t playerCount
        );

        [[nodiscard]]
        Rect getBatteryRectangle() const;

        void redrawStatusOverlay(
            const Rect& dirtyArea
        );
    };
} // namespace sticky_lotus::ui
