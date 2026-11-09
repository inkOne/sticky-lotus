#include "sticky_lotus/ui/AppRenderer.h"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
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

    void AppRenderer::drawDice(
        const bool selector,
        const int sides,
        const int result
    )
    {
        const Rect panel{70.0F, 70.0F, 660.0F, 340.0F};

        canvas_.fillRoundedRect(panel, 18.0F, Ink::White);
        canvas_.drawRoundedRect(panel, 18.0F, 3.0F, Ink::Black);

        char title[16]{};

        if (selector) {
            std::strcpy(title, "Choose a die");
        } else if (sides == 2) {
            std::strcpy(title, "COIN");
        } else {
            std::snprintf(title, sizeof(title), "D%d", sides);
        }

        canvas_.drawText(
            title,
            {90.0F, 88.0F, 620.0F, 40.0F},
            28,
            Ink::Black,
            TextAlignment::Center,
            0.0F
        );

        if (selector) {
            for (int i = 0; i < diceOptionCount; ++i) {
                char label[8]{};

                if (diceOptionSides(i) == 2) {
                    std::strcpy(label, "Coin");
                } else {
                    std::snprintf(
                        label,
                        sizeof(label),
                        "D%d",
                        diceOptionSides(i)
                    );
                }

                drawSettingsButton(
                    diceOptionArea(i),
                    label,
                    sides == diceOptionSides(i)
                );
            }

            canvas_.drawText(
                "Tap to select",
                {90.0F, 340.0F, 620.0F, 30.0F},
                22,
                Ink::Black,
                TextAlignment::Center,
                0.0F
            );
        } else {
            char value[16]{};

            if (sides == 2) {
                std::strcpy(value, result == 1 ? "HEADS" : "TAILS");
            } else {
                std::snprintf(value, sizeof(value), "%d", result);
            }

            canvas_.drawText(
                value,
                {90.0F, 150.0F, 620.0F, 170.0F},
                90,
                Ink::Black,
                TextAlignment::Center,
                0.0F
            );
        }

        canvas_.invalidate(panel);
        canvas_.flush();
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
        drawButtonHints(
            false
        );

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

        /*
         * Und für die Tasten-Hinweise am unteren Rand, falls das
         * neu gezeichnete Feld sie überdeckt.
         */
        redrawButtonHintsIfCovered(
            playerArea
        );

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
         *
         * Groß genug für die Zahl in derselben Schriftgröße (90)
         * wie im Lebenscounter auf dem Game-Screen.
         */
        constexpr float poisonNumberWidth =
            130.0F;

        constexpr float poisonNumberHeight =
            100.0F;
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
            90,
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
        {
            const float swipeHintCenterX =
                playerArea.x +
                playerArea.width / 2.0F;

            const float swipeHintY =
                outwardOffsetFromEdge(
                    playerArea,
                    upsideDown,
                    50.0F
                );

            drawTriangleDown(
                {
                    swipeHintCenterX - 125.0F,
                    swipeHintY + 10.5F,
                    14.0F,
                    14.0F
                },
                Ink::DarkGray,
                upsideDown
            );

            canvas_.drawText(
                "Swipe down to save",
                {
                    swipeHintCenterX - 95.0F,
                    swipeHintY,
                    230.0F,
                    35.0F
                },
                20,
                Ink::DarkGray,
                TextAlignment::Center,
                rotation
            );
        }

        drawPoisonDeltaBadge(
            game,
            selectedPlayer,
            playerArea,
            upsideDown
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

    Rect AppRenderer::getButtonHintsRectangle() const
    {
        return {
            38.0F,
            2.0F,
            314.0F,
            22.0F
        };
    }

    void AppRenderer::drawButtonHints(
        const bool large
    )
    {
        /*
         * Beschriftung der drei Gehäusetasten oben links, um 180 Grad
         * gedreht - lesbar aus Sicht der Seite, an der die Tasten
         * sitzen. Von links nach rechts im Display-Koordinatensystem:
         * IO (Power), Reset, Roll. Die x-Positionen liegen mittig
         * über der jeweiligen Taste.
         *
         * Große Variante (Settings): doppelte Schriftgröße. Das
         * Rechteck beginnt knapp über dem Bildschirmrand, damit die
         * Buchstaben nach der 180-Grad-Spiegelung im schmalen Rand
         * über dem Settings-Panel liegen und dessen Linie nicht
         * berühren.
         */
        const int fontSize =
            large ? 28 : 14;

        const float hintWidth =
            large ? 120.0F : 60.0F;

        const float hintHeight =
            large ? 28.0F : 18.0F;

        const float hintY =
            large ? -4.0F : 4.0F;

        const auto drawHint =
            [&](
                const char* label,
                const float centerX
            )
        {
            canvas_.drawText(
                label,
                {
                    centerX - hintWidth / 2.0F,
                    hintY,
                    hintWidth,
                    hintHeight
                },
                fontSize,
                Ink::DarkGray,
                TextAlignment::Center,
                180.0F
            );
        };

        drawHint("IO", 70.0F);
        drawHint("DICE", 207.0F);
        drawHint("ROLL", 320.0F);
    }

    void AppRenderer::redrawButtonHintsIfCovered(
        const Rect& dirtyArea
    )
    {
        const Rect hints =
            getButtonHintsRectangle();

        const bool overlaps =
            dirtyArea.x < hints.x + hints.width &&
            dirtyArea.x + dirtyArea.width > hints.x &&
            dirtyArea.y < hints.y + hints.height &&
            dirtyArea.y + dirtyArea.height > hints.y;

        if (!overlaps)
        {
            return;
        }

        drawButtonHints(
            false
        );

        canvas_.invalidate(
            hints
        );
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

    void AppRenderer::drawTriangleUp(
        const Rect& area,
        const Ink ink,
        const bool upsideDown
    )
    {
        const int height =
            std::max(
                1,
                static_cast<int>(area.height)
            );

        for (int row = 0; row < height; ++row)
        {
            const float t =
                height == 1
                    ? 1.0F
                    : static_cast<float>(row) /
                      static_cast<float>(height - 1);

            const float progress =
                upsideDown
                    ? 1.0F - t
                    : t;

            const float rowWidth =
                area.width * progress;

            const float rowX =
                area.x +
                (area.width - rowWidth) / 2.0F;

            canvas_.fillRect(
                {
                    rowX,
                    area.y + static_cast<float>(row),
                    rowWidth,
                    1.0F
                },
                ink
            );
        }
    }

    void AppRenderer::drawTriangleDown(
        const Rect& area,
        const Ink ink,
        const bool upsideDown
    )
    {
        /*
         * Spiegelbild von drawTriangleUp(): Spitze normalerweise
         * unten, bei umgedrehten Spielern oben - daher dieselbe
         * Logik mit invertiertem Flag.
         */
        drawTriangleUp(
            area,
            ink,
            !upsideDown
        );
    }

    void AppRenderer::drawTriangleLeft(
        const Rect& area,
        const Ink ink
    )
    {
        const int width =
            std::max(
                1,
                static_cast<int>(area.width)
            );

        for (int column = 0; column < width; ++column)
        {
            const float t =
                width == 1
                    ? 1.0F
                    : static_cast<float>(column) /
                      static_cast<float>(width - 1);

            const float columnHeight =
                area.height * t;

            const float columnY =
                area.y +
                (area.height - columnHeight) / 2.0F;

            canvas_.fillRect(
                {
                    area.x + static_cast<float>(column),
                    columnY,
                    1.0F,
                    columnHeight
                },
                ink
            );
        }
    }

    void AppRenderer::drawTriangleRight(
        const Rect& area,
        const Ink ink
    )
    {
        const int width =
            std::max(
                1,
                static_cast<int>(area.width)
            );

        for (int column = 0; column < width; ++column)
        {
            const float t =
                width == 1
                    ? 1.0F
                    : static_cast<float>(column) /
                      static_cast<float>(width - 1);

            const float columnHeight =
                area.height * (1.0F - t);

            const float columnY =
                area.y +
                (area.height - columnHeight) / 2.0F;

            canvas_.fillRect(
                {
                    area.x + static_cast<float>(column),
                    columnY,
                    1.0F,
                    columnHeight
                },
                ink
            );
        }
    }

    float AppRenderer::outwardOffsetFromEdge(
        const Rect& playerArea,
        const bool upsideDown,
        const float distanceFromOuterEdge
    ) const
    {
        return upsideDown
            ? playerArea.y + distanceFromOuterEdge
            : playerArea.y +
              playerArea.height -
              distanceFromOuterEdge;
    }

    void AppRenderer::drawPlayerName(
        const GameState& game,
        const std::size_t playerIndex,
        const std::size_t playerCount,
        const float rotation
    )
    {
        const std::string name =
            game.profiles.seatName(
                playerIndex
            );

        canvas_.drawText(
            name,
            playerNameArea(
                playerIndex,
                playerCount
            ),
            name.size() > 12
                ? 18
                : 22,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );
    }

    Ink AppRenderer::playerBackgroundInk(
        const GameState& game,
        const std::size_t playerIndex
    ) const
    {
        /*
         * Ausgeschiedene Spieler und der per "Roll Start"
         * ausgewürfelte Startspieler werden grau hinterlegt.
         */
        const bool highlighted =
            game.hasStartingPlayerHighlight() &&
            game.getStartingPlayerHighlight() ==
            playerIndex;

        return
            game.isPlayerEliminated(playerIndex) ||
            highlighted
                ? Ink::LightGray
                : Ink::White;
    }

    Rect AppRenderer::getCornerBadgeRectangle(
        const Rect& playerArea,
        const bool leftSide,
        const bool upsideDown
    ) const
    {
        constexpr float badgeWidth = 110.0F;
        constexpr float badgeHeight = 40.0F;
        constexpr float margin = 6.0F;

        /*
         * "leftSide" ist aus Sicht des Spielers gemeint. Bei
         * umgedrehten Spielern liegt deren "links" und "oben"
         * physisch auf der rechten/unteren Bildschirmseite.
         */
        const bool physicalLeft =
            leftSide != upsideDown;

        const float x =
            physicalLeft
                ? playerArea.x + margin
                : playerArea.x +
                  playerArea.width -
                  margin -
                  badgeWidth;

        const float y =
            upsideDown
                ? playerArea.y +
                  playerArea.height -
                  margin -
                  badgeHeight
                : playerArea.y + margin;

        return {
            x,
            y,
            badgeWidth,
            badgeHeight
        };
    }

    Rect AppRenderer::getCommanderDamageBadgeRectangle(
        const Rect& playerArea,
        const bool leftSide,
        const bool upsideDown
    ) const
    {
        constexpr float badgeWidth = 110.0F;
        constexpr float badgeHeight = 40.0F;
        constexpr float margin = 6.0F;

        const bool physicalLeft =
            leftSide != upsideDown;

        const float x =
            physicalLeft
                ? playerArea.x + margin
                : playerArea.x +
                  playerArea.width -
                  margin -
                  badgeWidth;

        /*
         * Anders als getCornerBadgeRectangle(): hier liegt die
         * Ecke "unten" statt "oben" aus Lesesicht des Spielers,
         * weil oben bereits die Spielernummer bzw. im Empfänger-
         * feld die Überschrift und der Swipe-Hinweis liegen.
         */
        const float y =
            upsideDown
                ? playerArea.y + margin
                : playerArea.y +
                  playerArea.height -
                  margin -
                  badgeHeight;

        return {
            x,
            y,
            badgeWidth,
            badgeHeight
        };
    }

    void AppRenderer::drawLifeDeltaBadge(
        const GameState& game,
        const std::size_t playerIndex,
        const Rect& playerArea,
        const bool upsideDown
    )
    {
        const Rect minusBadgeArea =
            getCornerBadgeRectangle(
                playerArea,
                true,
                upsideDown
            );

        const Rect plusBadgeArea =
            getCornerBadgeRectangle(
                playerArea,
                false,
                upsideDown
            );

        /*
         * Immer beide Ecken löschen, unabhängig vom aktuellen
         * Vorzeichen - sonst bliebe bei einem Vorzeichenwechsel
         * oder nach Ablauf des Timeouts ein Bildrest stehen.
         */
        /*
         * Hintergrund des Feldes statt festem Weiß, damit die
         * Ecken bei der grauen Startspieler-Hervorhebung nicht als
         * weiße Flecken stehen bleiben.
         */
        const Ink background =
            playerBackgroundInk(
                game,
                playerIndex
            );

        canvas_.fillRect(
            minusBadgeArea,
            background
        );

        canvas_.fillRect(
            plusBadgeArea,
            background
        );

        if (!game.hasLifeDeltaHighlight(playerIndex))
        {
            return;
        }

        const int delta =
            game.getLifeDelta(playerIndex);

        if (delta == 0)
        {
            return;
        }

        const float rotation =
            upsideDown ? 180.0F : 0.0F;

        const std::string text =
            delta > 0
                ? "+" + std::to_string(delta)
                : std::to_string(delta);

        canvas_.drawText(
            text,
            delta < 0 ? minusBadgeArea : plusBadgeArea,
            28,
            Ink::DarkGray,
            TextAlignment::Center,
            rotation
        );
    }

    void AppRenderer::drawPoisonDeltaBadge(
        const GameState& game,
        const std::size_t playerIndex,
        const Rect& playerArea,
        const bool upsideDown
    )
    {
        const Rect minusBadgeArea =
            getCornerBadgeRectangle(
                playerArea,
                true,
                upsideDown
            );

        const Rect plusBadgeArea =
            getCornerBadgeRectangle(
                playerArea,
                false,
                upsideDown
            );

        canvas_.fillRect(
            minusBadgeArea,
            Ink::White
        );

        canvas_.fillRect(
            plusBadgeArea,
            Ink::White
        );

        if (!game.hasPoisonDeltaHighlight(playerIndex))
        {
            return;
        }

        const int delta =
            game.getPoisonDelta(playerIndex);

        if (delta == 0)
        {
            return;
        }

        const float rotation =
            upsideDown ? 180.0F : 0.0F;

        const std::string text =
            delta > 0
                ? "+" + std::to_string(delta)
                : std::to_string(delta);

        canvas_.drawText(
            text,
            delta < 0 ? minusBadgeArea : plusBadgeArea,
            28,
            Ink::DarkGray,
            TextAlignment::Center,
            rotation
        );
    }

    void AppRenderer::drawCommanderDamageDeltaBadge(
        const commander::CommanderDamageDraft& draft,
        const std::size_t attackingPlayer,
        const Rect& playerArea,
        const bool upsideDown
    )
    {
        const Rect minusBadgeArea =
            getCommanderDamageBadgeRectangle(
                playerArea,
                true,
                upsideDown
            );

        const Rect plusBadgeArea =
            getCommanderDamageBadgeRectangle(
                playerArea,
                false,
                upsideDown
            );

        canvas_.fillRect(
            minusBadgeArea,
            Ink::White
        );

        canvas_.fillRect(
            plusBadgeArea,
            Ink::White
        );

        /*
         * Die Änderungsanzeige gehört zum gerade angezeigten Commander.
         */
        const int commander =
            draft.selectedCommander(attackingPlayer);

        if (!draft.hasDamageDeltaHighlight(attackingPlayer, commander))
        {
            return;
        }

        const int delta =
            draft.getDamageDelta(attackingPlayer, commander);

        if (delta == 0)
        {
            return;
        }

        const float rotation =
            upsideDown ? 180.0F : 0.0F;

        const std::string text =
            delta > 0
                ? "+" + std::to_string(delta)
                : std::to_string(delta);

        canvas_.drawText(
            text,
            delta < 0 ? minusBadgeArea : plusBadgeArea,
            28,
            Ink::DarkGray,
            TextAlignment::Center,
            rotation
        );
    }

    void AppRenderer::drawGestureHints(
        const Rect& playerArea,
        const float rotation
    )
    {
        const bool upsideDown =
            rotation == 180.0F;

        const float centerX =
            playerArea.x +
            playerArea.width / 2.0F;

        const float centerY =
            playerArea.y +
            playerArea.height / 2.0F;

        /*
         * Die Hinweiszeilen sollen immer auf der Außenseite der
         * Zahl liegen (Richtung Bildschirmrand), nicht Richtung
         * Bildschirmmitte/Menü-Button.
         *
         * Bei umgedrehten Spielern (obere Feldreihe) liegt die
         * Außenseite auf der entgegengesetzten Seite der Feldmitte.
         */
        const float outwardSign =
            upsideDown
                ? -1.0F
                : 1.0F;

        /*
         * Zeile 1: Poison-Hinweis (nach oben wischen).
         *
         * Ein einzelnes Symbol - muss wie Zahl und Text
         * für umgedrehte Spieler gespiegelt werden.
         */
        const float poisonRowY =
            centerY + outwardSign * 58.0F;

        drawTriangleUp(
            {
                centerX - 37.5F,
                poisonRowY - 5.5F,
                11.0F,
                11.0F
            },
            Ink::DarkGray,
            upsideDown
        );

        /*
         * Center statt Left-Alignment: die Textbreite von
         * "POISON" wird hier nicht exakt gemessen (kein
         * Zugriff auf die Font-Metriken von AppRenderer aus).
         *
         * Bei Center-Alignment bleibt der Text unabhängig von
         * der tatsächlichen Breite symmetrisch um seine eigene
         * Boxmitte - und spiegelt bei 180 Grad exakt um dieselbe
         * Mitte zurück. Kein Risiko, dass er über den Bildschirm-
         * rand hinaus gespiegelt wird, selbst wenn die geschätzte
         * Box-Breite nicht exakt stimmt.
         *
         * Text liegt jetzt exakt unter der Lebenszahl (Center bei
         * centerX), das Dreieck sitzt links davon.
         */
        canvas_.drawText(
            "POISON",
            {
                centerX - 50.0F,
                poisonRowY - 9.0F,
                100.0F,
                18.0F
            },
            14,
            Ink::DarkGray,
            TextAlignment::Center,
            rotation
        );

        /*
         * Zeile 2: Commander-Damage-Hinweis (seitlich wischen).
         *
         * Die beiden Dreiecke bleiben unabhängig von der
         * Spielerausrichtung immer gleich: Ein links-rechts-
         * symmetrisches Paar sieht nach einer 180-Grad-Drehung
         * wieder identisch aus - anders als das einzelne
         * Poison-Symbol oben.
         */
        const float damageRowY =
            centerY + outwardSign * 83.0F;

        drawTriangleLeft(
            {
                centerX - 93.5F,
                damageRowY - 5.5F,
                11.0F,
                11.0F
            },
            Ink::DarkGray
        );

        /*
         * Großzügiger Breitenpuffer statt knapper Schätzung -
         * siehe Kommentar bei "POISON" oben. "COMMANDERDAMAGE"
         * lief vorher rechts über seine zu schmale Box hinaus,
         * was sich bei den umgedrehten Feldern als Textmüll
         * links über den Bildschirmrand hinaus gespiegelt hat.
         *
         * Ebenfalls exakt unter der Lebenszahl zentriert.
         */
        canvas_.drawText(
            "COMMANDERDAMAGE",
            {
                centerX - 90.0F,
                damageRowY - 9.0F,
                180.0F,
                18.0F
            },
            14,
            Ink::DarkGray,
            TextAlignment::Center,
            rotation
        );

        drawTriangleRight(
            {
                centerX + 82.5F,
                damageRowY - 5.5F,
                11.0F,
                11.0F
            },
            Ink::DarkGray
        );
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
            playerBackgroundInk(
                game,
                playerIndex
            )
        );

        canvas_.drawRect(
            playerArea,
            2.0F,
            Ink::Black
        );

        drawPlayerName(
            game,
            playerIndex,
            playerCount,
            rotation
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

            drawGestureHints(
                playerArea,
                rotation
            );

            drawLifeDeltaBadge(
                game,
                playerIndex,
                playerArea,
                upsideDown
            );

            return;
        }

        /*
         * "Game Over" bleibt immer inward (Richtung Mitte/Menü-
         * Button), die Ursache darunter immer outward (Richtung
         * Bildschirmrand) - unabhängig von der Ausrichtung.
         *
         * Ohne dieses Vorzeichen tauschen Überschrift und Ursache
         * bei umgedrehten Spielern relativ die Seite.
         */
        const float eliminationOutwardSign =
            upsideDown
                ? -1.0F
                : 1.0F;

        canvas_.drawText(
            player.deathMessage.empty()
                ? "Game Over"
                : player.deathMessage.c_str(),
            {
                playerArea.x + 20.0F,
                playerArea.y +
                playerArea.height / 2.0F -
                eliminationOutwardSign * 20.0F,
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
                    eliminationOutwardSign * 12.0F,
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
                    eliminationOutwardSign * 20.0F,
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
                    eliminationOutwardSign * 25.0F,
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
            getThreePlayersButtonRectangle(),
            "3",
            game.getSettings().playerMode ==
            PlayerMode::ThreePlayers
        );

        drawSettingsButton(
            getFourPlayersButtonRectangle(),
            "4",
            game.getSettings().playerMode ==
                PlayerMode::FourPlayers
        );

        canvas_.flush();
    }

    void AppRenderer::drawSoundSettingsButtons(
        const GameState& game
    )
    {
        drawSettingsButton(
            getSoundOnButtonRectangle(),
            "On",
            game.getSettings().soundEnabled
        );

        drawSettingsButton(
            getSoundOffButtonRectangle(),
            "Off",
            !game.getSettings().soundEnabled
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
            getThreePlayersButtonRectangle(),
            "3",
            game.getSettings().playerMode ==
            PlayerMode::ThreePlayers
        );

        drawSettingsButton(
            fourPlayersButton,
            "4",
            game.getSettings().playerMode ==
            PlayerMode::FourPlayers
        );

        /*
         * Sound (Klickfeedback)
         */
        const Rect soundOnButton =
            getSoundOnButtonRectangle();

        const Rect soundOffButton =
            getSoundOffButtonRectangle();

        canvas_.drawText(
            "Sound",
            {
                soundOnButton.x,
                soundOnButton.y - 38.0F,
                260.0F,
                32.0F
            },
            26,
            Ink::Black,
            TextAlignment::Left,
            0.0F
        );

        drawSettingsButton(
            soundOnButton,
            "On",
            game.getSettings().soundEnabled
        );

        drawSettingsButton(
            soundOffButton,
            "Off",
            !game.getSettings().soundEnabled
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
            getRollStartButtonRectangle(),
            "Roll for first",
            false
        );

        drawSettingsButton(
            getDoneButtonRectangle(),
            "Done",
            true
        );

        /*
         * Spielerprofile und Punktestand
         */
        drawSettingsButton(
            profilesButtonArea(),
            "Players & scores",
            false
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
                245.0F,
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
            280.0F,
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
            "v1.1.5",
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

        /*
         * Tasten-Beschriftung, in den Settings doppelt so groß.
         */
        drawButtonHints(
            true
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
            80.0F,
            44.0F
        };
    }

    Rect AppRenderer::getThreePlayersButtonRectangle() const
    {
        return {
            162.5F,
            125.0F,
            80.0F,
            44.0F
        };
    }

    Rect AppRenderer::getFourPlayersButtonRectangle() const
    {
        return {
            255.0F,
            125.0F,
            80.0F,
            44.0F
        };
    }

    Rect AppRenderer::getSoundOnButtonRectangle() const
    {
        return {
            515.0F,
            125.0F,
            110.0F,
            44.0F
        };
    }

    Rect AppRenderer::getSoundOffButtonRectangle() const
    {
        return {
            645.0F,
            125.0F,
            110.0F,
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

    Rect AppRenderer::getRollStartButtonRectangle() const
    {
        /*
         * Zentriert in der Lücke zwischen Reset (endet bei 240)
         * und Done (beginnt bei 560) - breiter als die übrigen
         * Footer-Buttons, damit "Roll Start" bei Schriftgröße 24
         * bequem passt.
         */
        return {
            265.0F,
            390.0F,
            270.0F,
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

        const Rect playerArea =
            tableLayout_.playerArea(
                playerIndex,
                playerCount
            );

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
         *
         * lifeArea deckt die komplette Feldhöhe ab (nicht nur die
         * sichtbare Zahl), also auch den Bereich der Gesten-Hinweise
         * darunter/darüber - die werden hier also mitgelöscht und
         * müssen deshalb unten explizit neu gezeichnet werden.
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
        const Ink background =
            playerBackgroundInk(
                game,
                playerIndex
            );

        canvas_.fillRect(
            lifeRedrawArea,
            background
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
         * Wurden vom fillRect() oben mitgelöscht - müssen bei
         * jedem Life-Update neu gezeichnet werden.
         */
        drawGestureHints(
            playerArea,
            rotation
        );

        /*
         * Der Name liegt im selben Streifen wie die Lebenszahl und
         * wurde vom Löschen oben mit entfernt.
         */
        drawPlayerName(
            game,
            playerIndex,
            playerCount,
            rotation
        );

        drawLifeDeltaBadge(
            game,
            playerIndex,
            playerArea,
            upsideDown
        );

        /*
         * Die Änderungsanzeigen in den Ecken reichen bis an den
         * mittigen Menübutton heran und löschen ihn teilweise.
         * Overlays (Menübutton, Batterie, Tasten-Hinweise) deshalb
         * wie in drawPlayerRegion() zuletzt neu zeichnen.
         *
         * Invalidiert dabei das komplette Spielerfeld, nicht nur
         * lifeRedrawArea - sonst wird der Refresh für die
         * Gesten-Hinweise auf echter Hardware nicht ausgelöst,
         * auch wenn der Framebuffer bereits korrekt ist.
         */
        redrawButtonHintsIfCovered(
            playerArea
        );

        redrawStatusOverlay(
            playerArea
        );
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
         * Orientierung richtet sich nach dem Empfänger, nicht nach dem
         * jeweils angezeigten Angreifer.
         *
         * Der Empfänger bedient diesen Screen von seinem eigenen Platz aus
         * und muss Zahl, Plus und Minus in jedem Feld ohne Drehung des
         * Geräts lesen und treffen können.
         */
        const bool upsideDown =
            tableLayout_.isUpsideDown(
                receivingPlayer,
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
                        outwardOffsetFromEdge(
                            playerArea,
                            upsideDown,
                            55.0F
                        ),
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
                        /*
                         * Bewusst "inward" (Richtung Mitte/
                         * Menü-Button) - die Überschrift bleibt
                         * damit für beide Ausrichtungen auf
                         * derselben relativen Seite wie der
                         * Hinweistext unten (outward).
                         */
                        outwardOffsetFromEdge(
                            playerArea,
                            !upsideDown,
                            72.0F
                        ),
                        playerArea.width - 40.0F,
                        35.0F
                    },
                    24,
                    Ink::Black,
                    TextAlignment::Center,
                    rotation
                );

                {
                    const float swipeHintY =
                        outwardOffsetFromEdge(
                            playerArea,
                            upsideDown,
                            70.0F
                        );

                    drawTriangleLeft(
                        {
                            playerArea.x + 20.0F,
                            swipeHintY + 10.5F,
                            14.0F,
                            14.0F
                        },
                        Ink::DarkGray
                    );

                    canvas_.drawText(
                        "Swipe left or right to save",
                        {
                            playerArea.x + 40.0F,
                            swipeHintY,
                            playerArea.width - 80.0F,
                            35.0F
                        },
                        20,
                        Ink::DarkGray,
                        TextAlignment::Center,
                        rotation
                    );

                    drawTriangleRight(
                        {
                            playerArea.x +
                            playerArea.width -
                            34.0F,

                            swipeHintY + 10.5F,
                            14.0F,
                            14.0F
                        },
                        Ink::DarkGray
                    );
                }
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
            90,
            Ink::Black,
            TextAlignment::Center,
            rotation
        );

        /*
         * Welcher Commander gerade gezeigt wird (A oder Partner B). Hat
         * der andere schon Schaden, steht dessen Wert in Klammern dabei.
         * Ein Tap auf die Zahl wechselt (siehe commanderToggleArea()).
         */
        {
            const int shown =
                draft.selectedCommander(playerIndex);

            const int other =
                draft.damageFrom(playerIndex, 1 - shown);

            char marker[24]{};

            if (other > 0)
            {
                std::snprintf(
                    marker,
                    sizeof(marker),
                    "%c  (%c %d)",
                    'A' + shown,
                    'A' + 1 - shown,
                    other
                );
            }
            else
            {
                std::snprintf(
                    marker,
                    sizeof(marker),
                    "%c",
                    'A' + shown
                );
            }

            /*
             * Unter der Zahl aus Sicht des Empfängers; bei gedrehter
             * Darstellung liegt "unten" auf der anderen Feldseite.
             */
            canvas_.drawText(
                marker,
                commanderMarkerArea(
                    playerIndex,
                    playerCount,
                    upsideDown
                ),
                32,
                Ink::Black,
                TextAlignment::Center,
                rotation
            );
        }

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
                /*
                 * Inward wie die COMMANDER-DAMAGE-Überschrift
                 * oben - bleibt dadurch für beide Ausrichtungen
                 * auf derselben relativen Seite des Feldes.
                 */
                outwardOffsetFromEdge(
                    playerArea,
                    !upsideDown,
                    10.0F
                ),
                40.0F,
                28.0F
            },
            16,
            Ink::DarkGray,
            TextAlignment::Center,
            rotation
        );

        drawCommanderDamageDeltaBadge(
            draft,
            playerIndex,
            playerArea,
            upsideDown
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
