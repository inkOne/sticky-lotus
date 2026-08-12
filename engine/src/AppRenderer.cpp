#include "sticky_lotus/ui/AppRenderer.h"

#include <algorithm>
#include <cstddef>
#include <string>

#include "icons/lotus_menu_icon.h"
#include "icons/poison.h"
#include "icons/skull_icon.h"
#include "icons/cooppunks_social.h"

namespace sticky_lotus::ui
{
    AppRenderer::AppRenderer(
        Canvas& canvas
    )
        : canvas_(canvas)
    {
    }

    void AppRenderer::drawGame(
        const GameState& game
    )
    {
        canvas_.clear(
            Ink::White
        );

        drawPlayers(
            game
        );

        drawMenuButton();
        drawBatteryPercent();

        /*
         * Der komplette Bildschirm wurde neu gezeichnet.
         */
        canvas_.invalidate({
            0.0F,
            0.0F,
            static_cast<float>(screenWidth),
            static_cast<float>(screenHeight)
        });

        /*
         * Refresh vormerken.
         * StickyCanvas::serviceRefresh() führt ihn
         * nach der konfigurierten Verzögerung aus.
         */
        canvas_.flush();
    }

    void AppRenderer::drawPlayerRegion(
        const GameState& game,
        const std::size_t playerIndex
    )
    {
        const std::size_t playerCount =
            game.getPlayerCount();

        if (playerIndex >= playerCount)
        {
            return;
        }

        const Rect playerArea =
            tableLayout_.playerArea(
                playerIndex,
                playerCount
            );

        const bool upsideDown =
            tableLayout_.isUpsideDown(
                playerIndex,
                playerCount
            );

        canvas_.fillRect(
            playerArea,
            Ink::White
        );

        drawPlayer(
            game,
            playerIndex,
            playerArea,
            upsideDown
        );

        /*
         * Der Menübutton liegt über den Spielerfeldern.
         * Ein regionaler Redraw kann Teile davon löschen.
         *
         * Deshalb das zentrale Overlay immer danach neu zeichnen.
         */
        drawMenuButton();

        /*
         * Dasselbe gilt für die Batterieanzeige unten rechts.
         */
        drawBatteryPercent();

        canvas_.invalidate(
            playerArea
        );

        canvas_.invalidate(
            getMenuButtonRectangle()
        );

        canvas_.invalidate(
            getBatteryRectangle()
        );

        canvas_.flush();
    }

    void AppRenderer::drawGameImmediately(
        const GameState& game
    )
    {
        /*
         * Kompletten Framebuffer neu aufbauen.
         */
        drawGame(
            game
        );

        /*
         * Den von drawGame() vorgemerkten Partial Refresh
         * durch einen echten Full Refresh ersetzen.
         *
         * Wichtig bei grundlegenden Zustandswechseln wie
         * lebend <-> ausgeschieden.
         */
        canvas_.flushImmediately();
    }


    void AppRenderer::drawPoisonRegion(
        const GameState& game,
        const std::size_t selectedPlayer
    )
    {
        const std::size_t playerCount =
            game.getPlayerCount();

        if (selectedPlayer >= playerCount)
        {
            return;
        }

        const Rect playerArea =
            tableLayout_.playerArea(
                selectedPlayer,
                playerCount
            );

        const bool upsideDown =
            tableLayout_.isUpsideDown(
                selectedPlayer,
                playerCount
            );

        const float rotation =
            upsideDown
                ? 180.0F
                : 0.0F;

        /*
         * Nur das ausgewählte Spielerfeld im RAM löschen.
         */
        canvas_.fillRect(
            playerArea,
            Ink::White
        );

        canvas_.drawRect(
            playerArea,
            2.0F,
            Ink::Black
        );

        const Rect physicalTopArea =
            getPoisonPlusRectangle(
                selectedPlayer,
                playerCount
            );

        const Rect physicalBottomArea =
            getPoisonMinusRectangle(
                selectedPlayer,
                playerCount
            );

        /*
         * Für gedrehte Spieler:
         *
         * normal:
         * +
         * Logo
         * Counter
         * -
         *
         * upsideDown:
         * -
         * Counter
         * Logo
         * +
         */
        const Rect plusArea =
            upsideDown
                ? physicalBottomArea
                : physicalTopArea;

        const Rect minusArea =
            upsideDown
                ? physicalTopArea
                : physicalBottomArea;

        canvas_.drawText(
            "+",
            plusArea,
            42,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        constexpr float poisonIconWidth =
            static_cast<float>(
                POISON_WIDTH
            );

        constexpr float poisonIconHeight =
            static_cast<float>(
                POISON_HEIGHT
            );

        /*
         * Abstand zwischen Logo und Counter.
         */
        constexpr float poisonContentGap =
            22.0F;

        /*
         * Breite für den Zahlenbereich.
         */
        constexpr float poisonNumberWidth =
            70.0F;

        constexpr float poisonNumberHeight =
            60.0F;
        constexpr float poisonLethalGap =
            14.0F;

        const bool poisonLethal =
            game.getPoison(
                selectedPlayer
            ) >= 10;
        const float skullWidth =
    poisonLethal
        ? static_cast<float>(
            SKULL_ICON_WIDTH
        )
        : 0.0F;

        const float skullGap =
            poisonLethal
                ? poisonLethalGap
                : 0.0F;

        /*
         * Gesamte Breite:
         *
         * normal:
         * [ Poison ][ Gap ][ Zahl ]
         *
         * lethal:
         * [ Skull ][ Gap ][ Poison ][ Gap ][ Zahl ]
         */
        const float poisonContentWidth =
            skullWidth +
            skullGap +
            poisonIconWidth +
            poisonContentGap +
            poisonNumberWidth;
        /*
         * Gemeinsamer linker Startpunkt, sodass die
         * komplette Gruppe horizontal im Spielerfeld
         * zentriert ist.
         */
        const float poisonContentStartX =
            playerArea.x +
            (
                playerArea.width -
                poisonContentWidth
            ) / 2.0F;

        /*
         * Gemeinsame vertikale Mitte.
         */
        const float poisonContentCenterY =
            playerArea.y +
            playerArea.height / 2.0F;

        /*
         * Für Spieler 1/2:
         *
         * [ Logo ][ Abstand ][ Counter ]
         *
         * Für Spieler 3/4:
         *
         * [ Counter ][ Abstand ][ Logo ]
         *
         * Das Logo selbst wird nicht gedreht.
         */
        const float normalPoisonIconX =
    poisonContentStartX +
    skullWidth +
    skullGap;

        const float normalPoisonNumberX =
            normalPoisonIconX +
            poisonIconWidth +
            poisonContentGap;

        const float poisonIconX =
            upsideDown
                ? poisonContentStartX +
                  poisonNumberWidth +
                  poisonContentGap
                : normalPoisonIconX;

        const float poisonNumberX =
            upsideDown
                ? poisonContentStartX
                : normalPoisonNumberX;

        const Rect poisonIconArea = {
            poisonIconX,

            poisonContentCenterY -
            poisonIconHeight / 2.0F,

            poisonIconWidth,
            poisonIconHeight
        };

        const Rect poisonNumberArea = {
            poisonNumberX,

            poisonContentCenterY -
            poisonNumberHeight / 2.0F,

            poisonNumberWidth,
            poisonNumberHeight
        };

        if (poisonLethal)
        {
            const Rect skullIconArea = {
                upsideDown
                    ? poisonContentStartX +
                      poisonNumberWidth +
                      poisonContentGap +
                      poisonIconWidth +
                      skullGap
                    : poisonContentStartX,

                poisonContentCenterY -
                static_cast<float>(
                    SKULL_ICON_HEIGHT
                ) / 2.0F,

                static_cast<float>(
                    SKULL_ICON_WIDTH
                ),

                static_cast<float>(
                    SKULL_ICON_HEIGHT
                )
            };

            canvas_.drawMonochromeBitmap(
                skull_icon,
                SKULL_ICON_WIDTH,
                SKULL_ICON_HEIGHT,
                SKULL_ICON_BYTES_PER_ROW,
                skullIconArea,
                rotation
            );
        }

        /*
         * Poison-Icon immer zeichnen.
         * Das Icon selbst bleibt ungedreht.
         */
        canvas_.drawMonochromeBitmap(
            poison,
            POISON_WIDTH,
            POISON_HEIGHT,
            POISON_BYTES_PER_ROW,
            poisonIconArea,
            0.0F
        );

        canvas_.drawText(
            std::to_string(
                game.getPoison(
                    selectedPlayer
                )
            ),
            poisonNumberArea,
            42,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        canvas_.drawText(
            std::to_string(
                game.getPoison(
                    selectedPlayer
                )
            ),
            poisonNumberArea,
            42,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        canvas_.drawText(
            "-",
            minusArea,
            42,
            game.getPoison(selectedPlayer) > 0
                ? Ink::Black
                : Ink::LightGray,
            TextAlignment::Center,
            rotation
        );
        canvas_.drawText(
            "Swipe up or down to save",
            {
                playerArea.x + 20.0F,
                playerArea.y +
                playerArea.height -
                50.0F,
                playerArea.width - 40.0F,
                35.0F
            },
            20,
            Ink::DarkGray,
            TextAlignment::Center,
            rotation
        );
        /*
         * Batterie und Menübutton nach dem partiellen
         * Redraw wieder über das Spielerfeld legen.
         */
        redrawStatusOverlay(
            playerArea
        );
    }

    void AppRenderer::drawPlayers(
        const GameState& game
    )
    {
        const std::size_t playerCount =
            game.getPlayerCount();

        for (
            std::size_t playerIndex = 0;
            playerIndex < playerCount;
            ++playerIndex
        )
        {
            drawPlayer(
                game,
                playerIndex,
                tableLayout_.playerArea(
                    playerIndex,
                    playerCount
                ),
                tableLayout_.isUpsideDown(
                    playerIndex,
                    playerCount
                )
            );
        }
    }

    Rect AppRenderer::getBatteryRectangle() const
    {
        return {
            static_cast<float>(screenWidth) - 80.0F,
            static_cast<float>(screenHeight) - 32.0F,
            75.0F,
            28.0F
        };
    }

    void AppRenderer::redrawStatusOverlay(
        const Rect& dirtyArea
    )
    {
        /*
         * Overlays immer zuletzt zeichnen,
         * damit sie nicht von einem Spielerfeld
         * wieder übermalt werden.
         */
        drawBatteryPercent();
        drawMenuButton();

        /*
         * Den eigentlichen geänderten Bereich markieren.
         */
        canvas_.invalidate(
            dirtyArea
        );

        /*
         * Zusätzlich die Overlay-Bereiche markieren.
         */
        canvas_.invalidate(
            getBatteryRectangle()
        );

        canvas_.invalidate(
            getMenuButtonRectangle()
        );

        /*
         * Refresh nur vormerken.
         * serviceRefresh() führt ihn später aus.
         */
        canvas_.flush();
    }

    void AppRenderer::drawPixelIcon(
        const int* pixels,
        const int gridSize,
        const Rect& area,
        const float rotationDegrees,
        const Ink ink
    )
    {
        if (
            pixels == nullptr ||
            gridSize <= 0 ||
            area.width <= 0.0F ||
            area.height <= 0.0F
        )
        {
            return;
        }

        const float gridSizeAsFloat =
            static_cast<float>(
                gridSize
            );

        const float pixelSize =
            std::min(
                area.width / gridSizeAsFloat,
                area.height / gridSizeAsFloat
            );

        const float drawingSize =
            pixelSize * gridSizeAsFloat;

        const float startX =
            area.x +
            (
                area.width -
                drawingSize
            ) / 2.0F;

        const float startY =
            area.y +
            (
                area.height -
                drawingSize
            ) / 2.0F;

        const bool upsideDown =
            rotationDegrees == 180.0F;

        for (
            int row = 0;
            row < gridSize;
            ++row
        )
        {
            for (
                int column = 0;
                column < gridSize;
                ++column
            )
            {
                const int pixel =
                    pixels[
                        row * gridSize +
                        column
                    ];

                if (pixel == 0)
                {
                    continue;
                }

                const int displayRow =
                    upsideDown
                        ? gridSize - 1 - row
                        : row;

                const int displayColumn =
                    upsideDown
                        ? gridSize - 1 - column
                        : column;

                canvas_.fillRect(
                    {
                        startX +
                        static_cast<float>(
                            displayColumn
                        ) * pixelSize,

                        startY +
                        static_cast<float>(
                            displayRow
                        ) * pixelSize,

                        pixelSize,
                        pixelSize
                    },
                    ink
                );
            }
        }
    }

    void AppRenderer::drawPlayer(
        const GameState& game,
        const std::size_t playerIndex,
        const Rect& playerArea,
        const bool upsideDown
    )
    {
        const Player& player =
            game.getPlayer(
                playerIndex
            );

        const std::size_t playerCount =
            game.getPlayerCount();

        const Rect physicalMinusArea =
            tableLayout_.minusArea(
                playerIndex,
                playerCount
            );

        const Rect physicalPlusArea =
            tableLayout_.plusArea(
                playerIndex,
                playerCount
            );

        /*
         * Bei gedrehten Spielern werden Plus und Minus
         * auch positionsmäßig vertauscht.
         */
        const Rect minusArea =
            upsideDown
                ? physicalPlusArea
                : physicalMinusArea;

        const Rect plusArea =
            upsideDown
                ? physicalMinusArea
                : physicalPlusArea;

        const Rect lifeArea =
            tableLayout_.counterArea(
                playerIndex,
                playerCount
            );

        const float rotation =
            upsideDown
                ? 180.0F
                : 0.0F;

        const EliminationReason eliminationReason =
            game.getEliminationReason(
                playerIndex
            );

        const bool eliminated =
            eliminationReason !=
            EliminationReason::None;

        canvas_.fillRect(
            playerArea,
            eliminated
                ? Ink::LightGray
                : Ink::White
        );

        canvas_.drawRect(
            playerArea,
            2.0F,
            Ink::Black
        );

        if (!eliminated)
        {
            canvas_.drawText(
                std::to_string(
                    player.life
                ),
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

            return;
        }

        canvas_.drawText(
            player.deathMessage.empty()
                ? "Game Over"
                : player.deathMessage.c_str(),
            {
                playerArea.x + 20.0F,
                playerArea.y +
                playerArea.height / 2.0F -
                20.0F,
                playerArea.width - 40.0F,
                50.0F
            },
            30,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        switch (eliminationReason)
        {
        case EliminationReason::CommanderDamage:
            canvas_.drawText(
                "COMMANDER DAMAGE",
                {
                    playerArea.x + 20.0F,
                    playerArea.y +
                    playerArea.height / 2.0F +
                    12.0F,
                    playerArea.width - 40.0F,
                    30.0F
                },
                20,
                Ink::Black,
                TextAlignment::Center,
                rotation
            );

            canvas_.drawText(
                "",
                {
                    playerArea.x + 20.0F,
                    playerArea.y +
                    playerArea.height / 2.0F +
                    43.0F,
                    playerArea.width - 40.0F,
                    30.0F
                },
                20,
                Ink::Black,
                TextAlignment::Center,
                rotation
            );
            break;

        case EliminationReason::Life:
            canvas_.drawText(
                "0",
                {
                    playerArea.x + 20.0F,
                    playerArea.y +
                    playerArea.height / 2.0F +
                    20.0F,
                    playerArea.width - 40.0F,
                    55.0F
                },
                40,
                Ink::Black,
                TextAlignment::Center,
                rotation
            );
            break;

        case EliminationReason::Poison:
            canvas_.drawText(
                "POISONED",
                {
                    playerArea.x + 20.0F,
                    playerArea.y +
                    playerArea.height / 2.0F +
                    25.0F,
                    playerArea.width - 40.0F,
                    40.0F
                },
                20,
                Ink::Black,
                TextAlignment::Center,
                rotation
            );
            break;

        case EliminationReason::None:
            break;
        }
    }

    Rect AppRenderer::getMenuButtonRectangle() const
    {
        constexpr float buttonSize =
            58.0F;

        return {
            screenWidth / 2.0F -
            buttonSize / 2.0F,

            screenHeight / 2.0F -
            buttonSize / 2.0F,

            buttonSize,
            buttonSize
        };
    }

    void AppRenderer::drawMenuButton()
    {
        const Rect button =
            getMenuButtonRectangle();

        const Point center = {
            button.x +
            button.width / 2.0F,

            button.y +
            button.height / 2.0F
        };

        /*
         * Hintergrund des Buttons.
         */
        canvas_.fillCircle(
            center,
            button.width / 2.0F,
            Ink::White
        );

        /*
         * Kreisrand.
         */
        canvas_.drawCircle(
            center,
            button.width / 2.0F,
            2.0F,
            Ink::Black
        );

        /*
         * Lotus-Icon mittig im Button platzieren.
         */
        const Rect iconArea = {
            center.x -
            static_cast<float>(LOTUS_ICON_WIDTH) / 2.0F,

            center.y -
            static_cast<float>(LOTUS_ICON_HEIGHT) / 2.0F - 2.0F,

            static_cast<float>(LOTUS_ICON_WIDTH),
            static_cast<float>(LOTUS_ICON_HEIGHT)
        };

        canvas_.drawMonochromeBitmap(
            lotus_icon,
            LOTUS_ICON_WIDTH,
            LOTUS_ICON_HEIGHT,
            LOTUS_ICON_BYTES_PER_ROW,
            iconArea,
            0.0F
        );
    }
    void AppRenderer::drawTwoPlayerLifeSettingsButtons(
        const GameState& game
    )
    {
        drawSettingsButton(
            getTwoPlayer40Rectangle(),
            "40",
            game.getSettings().twoPlayerStartingLife == 40
        );

        drawSettingsButton(
            getTwoPlayer30Rectangle(),
            "30",
            game.getSettings().twoPlayerStartingLife == 30
        );

        drawSettingsButton(
            getTwoPlayer25Rectangle(),
            "25",
            game.getSettings().twoPlayerStartingLife == 25
        );

        drawSettingsButton(
            getTwoPlayer20Rectangle(),
            "20",
            game.getSettings().twoPlayerStartingLife == 20
        );

        canvas_.flush();
    }
    void AppRenderer::drawPlayerModeSettingsButtons(
        const GameState& game
    )
    {
        drawSettingsButton(
            getTwoPlayersButtonRectangle(),
            "2",
            game.getSettings().playerMode ==
                PlayerMode::TwoPlayers
        );

        drawSettingsButton(
            getFourPlayersButtonRectangle(),
            "4",
            game.getSettings().playerMode ==
                PlayerMode::FourPlayers
        );

        canvas_.flush();
    }

    void AppRenderer::drawMultiplayerLifeSettingsButtons(
        const GameState& game
    )
    {
        drawSettingsButton(
            getMultiplayer40Rectangle(),
            "40",
            game.getSettings().multiplayerStartingLife ==
                40
        );

        drawSettingsButton(
            getMultiplayer30Rectangle(),
            "30",
            game.getSettings().multiplayerStartingLife ==
                30
        );

        drawSettingsButton(
            getMultiplayer20Rectangle(),
            "20",
            game.getSettings().multiplayerStartingLife ==
                20
        );

        canvas_.flush();
    }

    void AppRenderer::drawSettingsButton(
        const Rect& area,
        const char* label,
        const bool selected
    )
    {
        constexpr float radius =
            14.0F;

        constexpr float borderThickness =
            2.0F;

        if (selected)
        {
            canvas_.fillRoundedRect(
                area,
                radius,
                Ink::Black
            );

            canvas_.drawText(
                label,
                area,
                24,
                Ink::White,
                TextAlignment::Center,
                0.0F
            );

            return;
        }

        canvas_.fillRoundedRect(
            area,
            radius,
            Ink::White
        );

        canvas_.drawRoundedRect(
            area,
            radius,
            borderThickness,
            Ink::Black
        );

        canvas_.drawText(
            label,
            area,
            24,
            Ink::Black,
            TextAlignment::Center,
            0.0F
        );
    }

    void AppRenderer::drawSettingsPanel(
        const Rect& area
    )
    {
        canvas_.fillRoundedRect(
            area,
            20.0F,
            Ink::White
        );

        canvas_.drawRoundedRect(
            area,
            20.0F,
            2.0F,
            Ink::Black
        );
    }

    void AppRenderer::drawSettings(
        const GameState& game
    )
    {
        canvas_.clear(
            Ink::White
        );

        const Rect settingsPanel = {
            35.0F,
            25.0F,
            screenWidth - 70.0F,
            screenHeight - 50.0F
        };

        drawSettingsPanel(
            settingsPanel
        );

        /*
         * Titel
         */
        canvas_.drawText(
            "Settings",
            {
                settingsPanel.x + 20.0F,
                settingsPanel.y + 15.0F,
                settingsPanel.width - 40.0F,
                42.0F
            },
            30,
            Ink::Black,
            TextAlignment::Center,
            0.0F
        );
        /*
         * Schließen oben rechts
         */
        const Rect closeButton =
            getCloseButtonRectangle();
        drawSettingsButton(
            closeButton,
            "X",
            false
        );
        /*
         * Vorhandene Button-Rechtecke holen.
         *
         * Die Überschriften werden anhand dieser Rechtecke
         * ausgerichtet. Dadurch stimmen Layout und Touchflächen
         * automatisch miteinander überein.
         */
        const Rect twoPlayersButton =
            getTwoPlayersButtonRectangle();

        const Rect fourPlayersButton =
            getFourPlayersButtonRectangle();

        const Rect multiplayer40Button =
            getMultiplayer40Rectangle();

        const Rect multiplayer30Button =
            getMultiplayer30Rectangle();

        const Rect multiplayer20Button =
            getMultiplayer20Rectangle();

        const Rect twoPlayer40Button =
            getTwoPlayer40Rectangle();

        const Rect twoPlayer30Button =
            getTwoPlayer30Rectangle();

        const Rect twoPlayer25Button =
            getTwoPlayer25Rectangle();
        const Rect twoPlayer20Button =
            getTwoPlayer20Rectangle();

        /*
         * Players
         */
        canvas_.drawText(
            "Players",
            {
                twoPlayersButton.x,
                twoPlayersButton.y - 38.0F,
                260.0F,
                32.0F
            },
            26,
            Ink::Black,
            TextAlignment::Left,
            0.0F
        );

        drawSettingsButton(
            twoPlayersButton,
            "2",
            game.getSettings().playerMode ==
            PlayerMode::TwoPlayers
        );

        drawSettingsButton(
            fourPlayersButton,
            "4",
            game.getSettings().playerMode ==
            PlayerMode::FourPlayers
        );

        /*
         * Multiplayer starting life
         */
        canvas_.drawText(
            "Multiplayer Life",
            {
                multiplayer40Button.x,
                multiplayer40Button.y - 38.0F,
                300.0F,
                32.0F
            },
            26,
            Ink::Black,
            TextAlignment::Left,
            0.0F
        );

        drawSettingsButton(
            multiplayer40Button,
            "40",
            game.getSettings()
                .multiplayerStartingLife ==
            40
        );

        drawSettingsButton(
            multiplayer30Button,
            "30",
            game.getSettings()
                .multiplayerStartingLife ==
            30
        );

        drawSettingsButton(
            multiplayer20Button,
            "20",
            game.getSettings()
                .multiplayerStartingLife ==
            20
        );

        /*
         * Two-player starting life
         */
        canvas_.drawText(
            "Two Player Life",
            {
                twoPlayer40Button.x,
                twoPlayer40Button.y - 38.0F,
                300.0F,
                32.0F
            },
            26,
            Ink::Black,
            TextAlignment::Left,
            0.0F
        );

        drawSettingsButton(
            twoPlayer40Button,
            "40",
            game.getSettings()
                .twoPlayerStartingLife ==
            40
        );

        drawSettingsButton(
            twoPlayer30Button,
            "30",
            game.getSettings()
                .twoPlayerStartingLife ==
            30
        );

        drawSettingsButton(
            twoPlayer25Button,
            "25",
            game.getSettings()
                .twoPlayerStartingLife ==
            25
        );

        drawSettingsButton(
            twoPlayer20Button,
            "20",
            game.getSettings()
                .twoPlayerStartingLife ==
            20
        );
        /*
         * Footer
         */
        drawSettingsButton(
            getResetButtonRectangle(),
            "Reset",
            false
        );

        drawSettingsButton(
            getDoneButtonRectangle(),
            "Done",
            true
        );

        /*
         * Branding / About rechts
         */
        constexpr float brandingX = 515.0F;
        constexpr float brandingWidth = 240.0F;

        /*
         * URL
         */
        canvas_.drawText(
            "lotus.cooppunks.social",
            {
                brandingX,
                185.0F,
                brandingWidth,
                30.0F
            },
            18,
            Ink::Black,
            TextAlignment::Center,
            0.0F
        );

        /*
         * Cooppunks Logo
         */
        constexpr float logoWidth =
            static_cast<float>(
                COOPPUNKS_SOCIAL_WIDTH
            );

        constexpr float logoHeight =
            static_cast<float>(
                COOPPUNKS_SOCIAL_HEIGHT
            );

        const Rect cooppunksLogoArea = {
            brandingX + 20.0F,
            235.0F,
            logoWidth,
            logoHeight
        };

        canvas_.drawMonochromeBitmap(
            cooppunks_social,
            COOPPUNKS_SOCIAL_WIDTH,
            COOPPUNKS_SOCIAL_HEIGHT,
            COOPPUNKS_SOCIAL_BYTES_PER_ROW,
            cooppunksLogoArea,
            0.0F
        );

        /*
         * by ink 2026
         */
        canvas_.drawText(
            "by ink 2026",
            {
                cooppunksLogoArea.x +
                cooppunksLogoArea.width +
                12.0F,

                cooppunksLogoArea.y,

                brandingWidth -
                cooppunksLogoArea.width -
                32.0F,

                cooppunksLogoArea.height
            },
            18,
            Ink::Black,
            TextAlignment::Left,
            0.0F
        );

        /*
         * Version
         */
        canvas_.drawText(
            "v1.0",
            {
                brandingX,
                cooppunksLogoArea.y +
                cooppunksLogoArea.height +
                12.0F,

                brandingWidth,
                24.0F
            },
            16,
            Ink::DarkGray,
            TextAlignment::Center,
            0.0F
        );



        canvas_.invalidate({
            0.0F,
            0.0F,
            static_cast<float>(screenWidth),
            static_cast<float>(screenHeight)
        });


        canvas_.flush();
    }

    Rect AppRenderer::getCloseButtonRectangle() const
    {
        return {
            700.0F,
            35.0F,
            45.0F,
            45.0F
        };
    }

    Rect AppRenderer::getTwoPlayersButtonRectangle() const
    {
        return {
            70.0F,
            125.0F,
            125.0F,
            44.0F
        };
    }

    Rect AppRenderer::getFourPlayersButtonRectangle() const
    {
        return {
            210.0F,
            125.0F,
            125.0F,
            44.0F
        };
    }

    Rect AppRenderer::getMultiplayer40Rectangle() const
    {
        return {
            70.0F,
            220.0F,
            75.0F,
            42.0F
        };
    }

    Rect AppRenderer::getMultiplayer30Rectangle() const
    {
        return {
            155.0F,
            220.0F,
            75.0F,
            42.0F
        };
    }

    Rect AppRenderer::getMultiplayer20Rectangle() const
    {
        return {
            240.0F,
            220.0F,
            75.0F,
            42.0F
        };
    }

    Rect AppRenderer::getMultiplayerEditRectangle() const
    {
        return {
            325.0F,
            220.0F,
            95.0F,
            42.0F
        };
    }

    Rect AppRenderer::getTwoPlayer40Rectangle() const
    {
        return {
            70.0F,
            315.0F,
            75.0F,
            42.0F
        };
    }

    Rect AppRenderer::getTwoPlayer30Rectangle() const
    {
        return {
            155.0F,
            315.0F,
            75.0F,
            42.0F
        };
    }

    Rect AppRenderer::getTwoPlayer25Rectangle() const
    {
        return {
            240.0F,
            315.0F,
            75.0F,
            42.0F
        };
    }

    Rect AppRenderer::getTwoPlayer20Rectangle() const
    {
        return {
            325.0F,
            315.0F,
            75.0F,
            42.0F
        };
    }

    Rect AppRenderer::getTwoPlayerEditRectangle() const
    {
        return {
            325.0F,
            315.0F,
            95.0F,
            42.0F
        };
    }

    Rect AppRenderer::getResetButtonRectangle() const
    {
        return {
            70.0F,
            390.0F,
            170.0F,
            42.0F
        };
    }

    Rect AppRenderer::getDoneButtonRectangle() const
    {
        return {
            560.0F,
            390.0F,
            170.0F,
            42.0F
        };
    }
    std::size_t AppRenderer::getCommanderDamageRowIndex(
        const std::size_t sourcePlayer,
        const std::size_t targetPlayer,
        const std::size_t playerCount
    )
    {
        std::size_t visibleRow = 0;

        for (
            std::size_t player = 0;
            player < playerCount;
            ++player
        )
        {
            if (
                player ==
                sourcePlayer
            )
            {
                continue;
            }

            if (
                player ==
                targetPlayer
            )
            {
                return visibleRow;
            }

            ++visibleRow;
        }

        return visibleRow;
    }

    Rect AppRenderer::getCommanderDamageMinusRectangle(
        const std::size_t attackingPlayer,
        const std::size_t playerCount
    ) const
    {
        return tableLayout_.minusArea(
            attackingPlayer,
            playerCount
        );
    }

    Rect AppRenderer::getCommanderDamagePlusRectangle(
        const std::size_t attackingPlayer,
        const std::size_t playerCount
    ) const
    {
        return tableLayout_.plusArea(
            attackingPlayer,
            playerCount
        );
    }

    Rect AppRenderer::getPoisonPlusRectangle(
        const std::size_t playerIndex,
        const std::size_t playerCount
    ) const
    {
        const Rect playerArea =
            tableLayout_.playerArea(
                playerIndex,
                playerCount
            );

        constexpr float width =
            120.0F;

        constexpr float height =
            55.0F;

        return {
            playerArea.x +
            playerArea.width / 2.0F -
            width / 2.0F,

            playerArea.y + 22.0F,

            width,
            height
        };
    }

    Rect AppRenderer::getPoisonMinusRectangle(
        const std::size_t playerIndex,
        const std::size_t playerCount
    ) const
    {
        const Rect playerArea =
            tableLayout_.playerArea(
                playerIndex,
                playerCount
            );

        constexpr float width =
            120.0F;

        constexpr float height =
            55.0F;

        return {
            playerArea.x +
            playerArea.width / 2.0F -
            width / 2.0F,

            playerArea.y +
            playerArea.height -
            height -
            22.0F,

            width,
            height
        };
    }

    Rect AppRenderer::getPlayerRectangle(
        const std::size_t playerIndex,
        const std::size_t playerCount
    ) const
    {
        return tableLayout_.playerArea(
            playerIndex,
            playerCount
        );
    }

    void AppRenderer::flush()
    {
        canvas_.flush();
    }

    void AppRenderer::drawCommanderDamage(
        const GameState& game,
        const commander::CommanderDamageDraft& draft
    )
    {
        canvas_.clear(
            Ink::White
        );

        const Rect fullScreen = {
            0.0F,
            0.0F,
            static_cast<float>(screenWidth),
            static_cast<float>(screenHeight)
        };

        if (!draft.isActive())
        {
            canvas_.drawText(
                "No Commander Damage session",
                fullScreen,
                24,
                Ink::Black
            );
            redrawStatusOverlay(
                fullScreen
            );
            return;
        }

        const std::size_t playerCount =
            draft.playerCount();

        for (
            std::size_t playerIndex = 0;
            playerIndex < playerCount;
            ++playerIndex
        )
        {
            drawCommanderDamageRegion(
                game,
                draft,
                playerIndex
            );
        }
        redrawStatusOverlay(
            fullScreen
        );
    }

    void AppRenderer::drawUiButton(
        const Rect& area,
        const char* label,
        const bool selected,
        const float rotationDegrees
    )
    {
        constexpr float cornerRadius =
            14.0F;

        constexpr float borderThickness =
            2.0F;

        if (selected)
        {
            canvas_.fillRoundedRect(
                area,
                cornerRadius,
                Ink::Black
            );

            canvas_.drawText(
                label,
                area,
                24,
                Ink::White,
                TextAlignment::Center,
                rotationDegrees
            );

            return;
        }

        canvas_.fillRoundedRect(
            area,
            cornerRadius,
            Ink::White
        );

        canvas_.drawRoundedRect(
            area,
            cornerRadius,
            borderThickness,
            Ink::Black
        );

        canvas_.drawText(
            label,
            area,
            24,
            Ink::Black,
            TextAlignment::Center,
            rotationDegrees
        );
    }

    void AppRenderer::drawPoison(
        const GameState& game,
        const std::size_t selectedPlayer
    )
    {
        canvas_.clear(
            Ink::White
        );

        const std::size_t playerCount =
            game.getPlayerCount();

        if (selectedPlayer >= playerCount)
        {
            return;
        }

        /*
         * Beim Öffnen des Poison-Screens einmal alles aufbauen.
         */
        drawPlayers(
            game
        );

        drawMenuButton();

        /*
         * Das ausgewählte Feld mit dem Poison-Screen übermalen.
         */
        drawPoisonRegion(
            game,
            selectedPlayer
        );

        canvas_.invalidate({
            0.0F,
            0.0F,
            static_cast<float>(screenWidth),
            static_cast<float>(screenHeight)
        });
    }

    void AppRenderer::drawLifeCounterRegion(
        const GameState& game,
        const std::size_t playerIndex
    )
    {
        const std::size_t playerCount =
            game.getPlayerCount();

        if (playerIndex >= playerCount)
        {
            return;
        }

        const Rect lifeArea =
            tableLayout_.counterArea(
                playerIndex,
                playerCount
            );

        const bool upsideDown =
            tableLayout_.isUpsideDown(
                playerIndex,
                playerCount
            );

        const float rotation =
            upsideDown ? 180.0F : 0.0F;

        /*
         * Nur einen etwas kleineren inneren Bereich löschen.
         * Dadurch bleiben die Trennlinien der Spielerfelder erhalten.
         */
        constexpr float horizontalInset = 10.0F;
        constexpr float verticalInset = 8.0F;

        const Rect lifeRedrawArea = {
            lifeArea.x + horizontalInset,
            lifeArea.y + verticalInset,
            lifeArea.width -
            horizontalInset * 2.0F,
            lifeArea.height -
            verticalInset * 2.0F
        };

        /*
         * Alte Zahl nur im inneren Bereich entfernen.
         */
        canvas_.fillRect(
            lifeRedrawArea,
            Ink::White
        );

        /*
         * Neue Zahl weiterhin im ursprünglichen lifeArea zeichnen,
         * damit Position und Zentrierung identisch bleiben.
         */
        canvas_.drawText(
            std::to_string(
                game.getPlayer(playerIndex).life
            ),
            lifeArea,
            90,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        /*
         * Nur den tatsächlich veränderten Bereich markieren.
         */
        canvas_.invalidate(
            lifeRedrawArea
        );

        canvas_.flush();
    }

    void AppRenderer::drawCommanderDamageRegion(
        const GameState& game,
        const commander::CommanderDamageDraft& draft,
        const std::size_t playerIndex
    )
    {
        if (!draft.isActive())
        {
            return;
        }

        const std::size_t playerCount =
            draft.playerCount();

        if (playerIndex >= playerCount)
        {
            return;
        }

        const std::size_t receivingPlayer =
            draft.receivingPlayer();

        const Rect playerArea =
            tableLayout_.playerArea(
                playerIndex,
                playerCount
            );

        /*
         * Orientierung des tatsächlich gezeichneten Spielerfeldes.
         */
        const bool upsideDown =
            tableLayout_.isUpsideDown(
                playerIndex,
                playerCount
            );

        const float rotation =
            upsideDown
                ? 180.0F
                : 0.0F;

        /*
         * Nur dieses Spielerfeld im Framebuffer löschen.
         */
        canvas_.fillRect(
            playerArea,
            Ink::White
        );

        canvas_.drawRect(
            playerArea,
            2.0F,
            Ink::Black
        );

        /*
         * Empfänger des Commander Damage.
         */
        if (playerIndex == receivingPlayer)
        {
            if (draft.isLethal())
            {
                const Rect skullIconArea = {
                    playerArea.x +
                    playerArea.width / 2.0F -
                    static_cast<float>(SKULL_ICON_WIDTH) / 2.0F,

                    playerArea.y +
                    playerArea.height / 2.0F -
                    static_cast<float>(SKULL_ICON_HEIGHT) / 2.0F,

                    static_cast<float>(SKULL_ICON_WIDTH),
                    static_cast<float>(SKULL_ICON_HEIGHT)
                };

                canvas_.drawMonochromeBitmap(
                    skull_icon,
                    SKULL_ICON_WIDTH,
                    SKULL_ICON_HEIGHT,
                    SKULL_ICON_BYTES_PER_ROW,
                    skullIconArea,
                    rotation
                );

                canvas_.drawText(
                    "LETHAL",
                    {
                        playerArea.x + 20.0F,
                        playerArea.y +
                        playerArea.height -
                        55.0F,
                        playerArea.width - 40.0F,
                        35.0F
                    },
                    18,
                    Ink::Black,
                    TextAlignment::Center,
                    rotation
                );
            }
            else
            {
                canvas_.drawText(
                    "",
                    {
                        playerArea.x + 20.0F,
                        playerArea.y + 35.0F,
                        playerArea.width - 40.0F,
                        35.0F
                    },
                    24,
                    Ink::Black,
                    TextAlignment::Center,
                    rotation
                );

                canvas_.drawText(
                    "COMMANDER DAMAGE",
                    {
                        playerArea.x + 20.0F,
                        playerArea.y + 72.0F,
                        playerArea.width - 40.0F,
                        35.0F
                    },
                    24,
                    Ink::Black,
                    TextAlignment::Center,
                    rotation
                );

                canvas_.drawText(
                    "Swipe left or right to save",
                    {
                        playerArea.x + 20.0F,
                        playerArea.y +
                        playerArea.height -
                        70.0F,
                        playerArea.width - 40.0F,
                        35.0F
                    },
                    20,
                    Ink::DarkGray,
                    TextAlignment::Center,
                    rotation
                );
            }

            /*
             * Wichtig:
             * Nicht nur invalidate(), sondern den Refresh
             * auch wirklich vormerken.
             *
             * Gleichzeitig werden Batterie/Menu-Overlay
             * wieder über das Spielerfeld gezeichnet.
             */
            redrawStatusOverlay(
                playerArea
            );

            return;
        }

        /*
         * Damage-Quellspieler.
         */
        const Rect physicalMinusArea =
            tableLayout_.minusArea(
                playerIndex,
                playerCount
            );

        const Rect physicalPlusArea =
            tableLayout_.plusArea(
                playerIndex,
                playerCount
            );

        /*
         * Bei gedrehten Spielern 3/4 werden die sichtbaren
         * Positionen von Plus und Minus vertauscht.
         */
        const Rect minusArea =
            upsideDown
                ? physicalPlusArea
                : physicalMinusArea;

        const Rect plusArea =
            upsideDown
                ? physicalMinusArea
                : physicalPlusArea;

        const Rect damageArea =
            tableLayout_.counterArea(
                playerIndex,
                playerCount
            );

        /*
         * Ganz wichtig:
         * Hier wird der aktuelle DRAFT-Wert angezeigt,
         * nicht der bereits gespeicherte GameState-Wert.
         *
         * Dadurch sieht man die Änderung schon vor dem
         * Verlassen des Commander-Damage-Screens.
         */
        canvas_.drawText(
            std::to_string(
                draft.damageFrom(
                    playerIndex
                )
            ),
            damageArea,
            76,
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

        canvas_.drawText(
            std::to_string(
                playerIndex + 1
            ),
            {
                playerArea.x + 12.0F,
                playerArea.y + 10.0F,
                40.0F,
                28.0F
            },
            16,
            Ink::DarkGray,
            TextAlignment::Center,
            rotation
        );

        /*
         * Auch hier Refresh wirklich vormerken.
         */
        redrawStatusOverlay(
            playerArea
        );

        (void)game;
    }

    bool AppRenderer::isPlayerUpsideDown(
        const std::size_t playerIndex,
        const std::size_t playerCount
    ) const
    {
        return tableLayout_.isUpsideDown(
            playerIndex,
            playerCount
        );
    }

    void AppRenderer::setBatteryPercent(
        const int batteryPercent
    )
    {
        batteryPercent_ =
            std::clamp(
                batteryPercent,
                -1,
                100
            );
    }

    void AppRenderer::setBatteryStatus(
        const int batteryPercent,
        const bool charging
    )
    {
        batteryPercent_ =
            std::clamp(
                batteryPercent,
                -1,
                100
            );

        batteryCharging_ =
            charging;
    }

    void AppRenderer::drawBatteryPercent()
    {
        if (batteryPercent_ < 0)
        {
            return;
        }

        const std::string batteryText =
            std::to_string(
                batteryPercent_
            ) +
            "%";

        const Rect textArea = {
            static_cast<float>(
                screenWidth
            ) - 62.0F,

            static_cast<float>(
                screenHeight
            ) - 28.0F,

            52.0F,
            18.0F
        };

        if (batteryCharging_)
        {
            drawChargingBolt({
                static_cast<float>(
                    screenWidth
                ) - 65.0F, //76.0F,

                static_cast<float>(
                    screenHeight
                ) - 27.0F
            });
        }

        canvas_.drawText(
            batteryText,
            textArea,
            18,
            Ink::Black,
            TextAlignment::Right
        );
    }

    void AppRenderer::drawChargingBolt(
        const Point origin
    )
    {
        canvas_.drawLine(
            {
                origin.x + 6.0F,
                origin.y
            },
            {
                origin.x,
                origin.y + 10.0F
            },
            2.0F,
            Ink::Black
        );

        canvas_.drawLine(
            {
                origin.x,
                origin.y + 10.0F
            },
            {
                origin.x + 5.0F,
                origin.y + 10.0F
            },
            2.0F,
            Ink::Black
        );

        canvas_.drawLine(
            {
                origin.x + 5.0F,
                origin.y + 10.0F
            },
            {
                origin.x + 1.0F,
                origin.y + 20.0F
            },
            2.0F,
            Ink::Black
        );

        canvas_.drawLine(
            {
                origin.x + 1.0F,
                origin.y + 20.0F
            },
            {
                origin.x + 10.0F,
                origin.y + 8.0F
            },
            2.0F,
            Ink::Black
        );

        canvas_.drawLine(
            {
                origin.x + 10.0F,
                origin.y + 8.0F
            },
            {
                origin.x + 5.0F,
                origin.y + 8.0F
            },
            2.0F,
            Ink::Black
        );

        canvas_.drawLine(
            {
                origin.x + 5.0F,
                origin.y + 8.0F
            },
            {
                origin.x + 6.0F,
                origin.y
            },
            2.0F,
            Ink::Black
        );
    }

    void AppRenderer::drawSleepScreen()
    {
        canvas_.clear(
            Ink::White
        );

        const Rect fullScreen = {
            0.0F,
            0.0F,
            static_cast<float>(screenWidth),
            static_cast<float>(screenHeight)
        };

        canvas_.drawLine(
            {
                100.0F,
                70.0F
            },
            {
                static_cast<float>(
                    screenWidth
                ) - 100.0F,
                70.0F
            },
            2.0F,
            Ink::Black
        );

        canvas_.drawText(
            "Sticky Lotus",
            {
                100.0F,
                100.0F,
                static_cast<float>(
                    screenWidth
                ) - 200.0F,
                60.0F
            },
            38,
            Ink::Black,
            TextAlignment::Center
        );

        canvas_.drawText(
            "THE STACK IS EMPTY",
            {
                100.0F,
                180.0F,
                static_cast<float>(
                    screenWidth
                ) - 200.0F,
                45.0F
            },
            25,
            Ink::DarkGray,
            TextAlignment::Center
        );

        canvas_.drawText(
            "Press Power",
            {
                100.0F,
                265.0F,
                static_cast<float>(
                    screenWidth
                ) - 200.0F,
                40.0F
            },
            24,
            Ink::Black,
            TextAlignment::Center
        );

        canvas_.drawText(
            "to continue",
            {
                100.0F,
                305.0F,
                static_cast<float>(
                    screenWidth
                ) - 200.0F,
                40.0F
            },
            22,
            Ink::Black,
            TextAlignment::Center
        );

        canvas_.drawLine(
            {
                100.0F,
                static_cast<float>(
                    screenHeight
                ) - 70.0F
            },
            {
                static_cast<float>(
                    screenWidth
                ) - 100.0F,
                static_cast<float>(
                    screenHeight
                ) - 70.0F
            },
            2.0F,
            Ink::Black
        );

        drawBatteryPercent();

        canvas_.invalidate(
            fullScreen
        );
    }
} // namespace sticky_lotus::ui
