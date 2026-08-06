#include "sticky_lotus/ui/AppRenderer.h"

#include <algorithm>
#include <cstddef>
#include <string>


class GameState;

namespace sticky_lotus::ui
{
    AppRenderer::AppRenderer(
        Canvas& canvas,
        ImageRenderer& imageRenderer
    )
        : canvas_(canvas),
          imageRenderer_(imageRenderer)
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

    void AppRenderer::drawPixelSkull(
        const Rect& area,
        const float rotationDegrees
    )
    {
        static constexpr int skull[9][9] = {
            {0, 0, 1, 1, 1, 1, 1, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 0},
            {1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 0, 0, 1, 0, 0, 1, 1},
            {1, 1, 0, 0, 1, 0, 0, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1},
            {0, 1, 1, 0, 1, 0, 1, 1, 0},
            {0, 0, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 0, 1, 0, 1, 0, 0}
        };

        drawPixelIcon(
            &skull[0][0],
            9,
            area,
            rotationDegrees,
            Ink::Black
        );
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
            static_cast<float>(gridSize);

        /*
         * Die Pixel werden so groß gewählt, dass das komplette
         * quadratische Raster in den Zielbereich passt.
         */
        const float pixelSize =
            std::min(
                area.width / gridSizeAsFloat,
                area.height / gridSizeAsFloat
            );

        /*
         * Tatsächliche Größe des gezeichneten Symbols.
         *
         * Falls area breiter als hoch ist oder umgekehrt,
         * wird das Symbol trotzdem quadratisch dargestellt.
         */
        const float drawingSize =
            pixelSize * gridSizeAsFloat;

        /*
         * Symbol horizontal und vertikal im Zielbereich zentrieren.
         */
        const float startX =
            area.x +
            (area.width - drawingSize) / 2.0F;

        const float startY =
            area.y +
            (area.height - drawingSize) / 2.0F;

        const bool upsideDown =
            rotationDegrees == 180.0F;

        for (int row = 0; row < gridSize; ++row)
        {
            for (
                int column = 0;
                column < gridSize;
                ++column
            )
            {
                /*
                 * Eine zweidimensionale Matrix liegt im Speicher
                 * zeilenweise hintereinander.
                 *
                 * Index:
                 * row * gridSize + column
                 */
                const int pixel =
                    pixels[
                        row * gridSize + column
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
            game.getPlayer(playerIndex);

        const std::size_t playerCount =
            game.getPlayerCount();

        const Rect minusArea =
            tableLayout_.minusArea(
                playerIndex,
                playerCount
            );

        const Rect plusArea =
            tableLayout_.plusArea(
                playerIndex,
                playerCount
            );

        const Rect lifeArea =
            tableLayout_.counterArea(
                playerIndex,
                playerCount
            );


        const float rotation =
            upsideDown ? 180.0F : 0.0F;

        const EliminationReason eliminationReason =
            game.getEliminationReason(playerIndex);

        const bool eliminated =
            eliminationReason !=
            EliminationReason::None;

        /*
         * Grundfläche und Umrandung des Spielerfeldes.
         */
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

        /*
         * Normale Anzeige eines aktiven Spielers.
         */
        if (!eliminated)
        {
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
            22,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        /*
         * Niederlage durch Commander Damage:
         * Die verbleibenden Lebenspunkte werden bewusst
         * nicht angezeigt.
         */
        switch (eliminationReason)
        {
        case EliminationReason::CommanderDamage:
            /*
             * Die verbleibenden Lebenspunkte werden bei einer
             * Niederlage durch Commander Damage nicht angezeigt.
             */
            canvas_.drawText(
                "COMMANDER",
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
                "DAMAGE",
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
                22,
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

        for (int line = -1; line <= 1; ++line)
        {
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

        /*drawSettingsButton(
            getMultiplayerEditRectangle(),
            "Edit",
            multiplayerLife != 40 &&
            multiplayerLife != 30 &&
            multiplayerLife != 20
        );*/

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

        /*drawSettingsButton(
            getTwoPlayerEditRectangle(),
            "Edit",
            twoPlayerLife != 20 &&
            twoPlayerLife != 30 &&
            twoPlayerLife != 40
        );*/

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
            if (player == sourcePlayer)
            {
                continue;
            }

            if (player == targetPlayer)
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

        constexpr float width = 120.0F;
        constexpr float height = 55.0F;

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

        constexpr float width = 120.0F;
        constexpr float height = 55.0F;

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


    void AppRenderer::drawCommanderDamage(
        const GameState& game,
        const commander::CommanderDamageDraft& draft
    )
    {
        canvas_.clear(Ink::White);

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

            canvas_.invalidate(fullScreen);
            return;
        }

        const std::size_t receivingPlayer =
            draft.receivingPlayer();

        const std::size_t playerCount =
            draft.playerCount();

        for (
            std::size_t playerIndex = 0;
            playerIndex < playerCount;
            ++playerIndex
        )
        {
            const Rect playerArea =
                tableLayout_.playerArea(
                    playerIndex,
                    playerCount
                );

            const bool upsideDown =
                tableLayout_.isUpsideDown(
                    receivingPlayer,
                    playerCount
                );

            const float rotation =
                upsideDown ? 180.0F : 0.0F;

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
             * Das Feld des Empfängers enthält nur Informationen
             * zur aktuellen Bearbeitung.
             */
            if (playerIndex == receivingPlayer)
            {
                if (draft.isLethal())
                {
                    drawPixelSkull(
                        {
                            playerArea.x +
                            playerArea.width / 2.0F -
                            50.0F,

                            playerArea.y +
                            playerArea.height / 2.0F -
                            50.0F,

                            100.0F,
                            100.0F
                        },
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
                        "COMMANDER",
                        {
                            playerArea.x + 20.0F,
                            playerArea.y + 35.0F,
                            playerArea.width - 40.0F,
                            35.0F
                        },
                        22,
                        Ink::Black,
                        TextAlignment::Center,
                        rotation
                    );

                    canvas_.drawText(
                        "DAMAGE",
                        {
                            playerArea.x + 20.0F,
                            playerArea.y + 72.0F,
                            playerArea.width - 40.0F,
                            35.0F
                        },
                        22,
                        Ink::Black,
                        TextAlignment::Center,
                        rotation
                    );

                    canvas_.drawText(
                        "Swipe to save",
                        {
                            playerArea.x + 20.0F,
                            playerArea.y +
                            playerArea.height -
                            70.0F,
                            playerArea.width - 40.0F,
                            35.0F
                        },
                        16,
                        Ink::DarkGray,
                        TextAlignment::Center,
                        rotation
                    );
                }

                continue;
            }

            /*
             * Die drei anderen Felder repräsentieren die möglichen
             * Commander-Damage-Quellen.
             */
            const Rect minusArea =
                tableLayout_.minusArea(
                    playerIndex,
                    playerCount
                );

            const Rect plusArea =
                tableLayout_.plusArea(
                    playerIndex,
                    playerCount
                );

            const Rect damageArea =
                tableLayout_.counterArea(
                    playerIndex,
                    playerCount
                );

            canvas_.drawText(
                std::to_string(
                    draft.damageFrom(playerIndex)
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

            /*
             * Kleine Kennzeichnung der Schadensquelle.
             *
             * Spielernamen werden weiterhin nicht verwendet.
             */
            canvas_.drawText(
                std::to_string(playerIndex + 1),
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
        }

        canvas_.invalidate(fullScreen);

        (void)game;
    }

    void AppRenderer::drawPoison(
        const GameState& game,
        const std::size_t selectedPlayer
    )
    {
        canvas_.clear(Ink::White);

        const std::size_t playerCount =
            game.getPlayerCount();

        if (selectedPlayer >= playerCount)
        {
            return;
        }

        /*
         * Zunächst wird der normale Spieltisch gezeichnet.
         * Anschließend übermalen wir nur das ausgewählte Feld
         * mit dem Poison-Counter.
         */
        drawPlayers(game);
        drawMenuButton();

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
            upsideDown ? 180.0F : 0.0F;

        /*
         * Das ausgewählte Spielerfeld vollständig leeren,
         * damit die Lebensanzeige darunter nicht sichtbar bleibt.
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
         * Die obere und untere Fläche werden abhängig von der
         * Orientierung vertauscht.
         *
         * Dadurch sieht jeder Spieler aus seiner Blickrichtung:
         *
         *     +
         *   Symbol
         *     Zahl
         *     -
         */
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

        const Rect plusArea =
            upsideDown
                ? physicalBottomArea
                : physicalTopArea;

        const Rect minusArea =
            upsideDown
                ? physicalTopArea
                : physicalBottomArea;

        /*
         * Plus-Button.
         */
        canvas_.drawText(
            "+",
            plusArea,
            42,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        /*
         * Poison-Symbol mittig oberhalb der Zahl.
         */
        const Rect poisonIconArea = {
            playerArea.x +
            playerArea.width / 2.0F -
            20.0F,

            playerArea.y +
            playerArea.height / 2.0F -
            42.0F,

            40.0F,
            40.0F
        };

        imageRenderer_.drawImage(
            ImageId::Poison,
            poisonIconArea,
            ImageFit::Contain,
            rotation
        );

        /*
         * Aktueller Poison-Wert unterhalb des Symbols.
         */
        const Rect poisonNumberArea = {
            playerArea.x + 20.0F,

            playerArea.y +
            playerArea.height / 2.0F +
            0.0F,

            playerArea.width - 40.0F,
            48.0F
        };

        canvas_.drawText(
            std::to_string(
                game.getPoison(selectedPlayer)
            ),
            poisonNumberArea,
            36,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        /*
         * Minus bleibt sichtbar, wird bei 0 aber heller dargestellt.
         * GameState verhindert zusätzlich Werte unter 0.
         */
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

        canvas_.invalidate(playerArea);
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


} // namespace sticky_lotus::ui
