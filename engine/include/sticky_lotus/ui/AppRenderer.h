#pragma once

#include "sticky_lotus/GameState.h"
#include "sticky_lotus/ui/Canvas.h"
#include "sticky_lotus/ui/Geometry.h"
#include <cstddef>
#include "sticky_lotus/commander/CommanderDamageDraft.h"
#include "sticky_lotus/ui/TableLayout.h"
#include "sticky_lotus/ui/ImageRenderer.h"

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
            Canvas& canvas,
            ImageRenderer& imageRenderer
        );

        /**
         * Zeichnet die normale Spielansicht.
         */
        void drawGame(const GameState& game);

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
        void flush();
    private:
        Canvas& canvas_;
        ImageRenderer& imageRenderer_;
        TableLayout tableLayout_;

        void drawPlayers(const GameState& game);

        void drawPlayer(
            const GameState& game,
            std::size_t playerIndex,
            const Rect& playerArea,
            bool upsideDown
        );

        void drawPixelSkull(
            const Rect& area,
            float rotationDegrees
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
        void drawMenuButton();

        void drawSettingsButton(
            const Rect& area,
            const char* label,
            bool selected
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
