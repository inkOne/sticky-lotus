#include "sticky_lotus_raylib/RaylibCanvas.h"

#include <string>

namespace sticky_lotus_raylib
{
    using sticky_lotus::ui::Ink;
    using sticky_lotus::ui::Point;
    using sticky_lotus::ui::Rect;
    using sticky_lotus::ui::TextAlignment;

    RaylibCanvas::RaylibCanvas(
        const int width,
        const int height,
        const char* windowTitle
    )
        : width_(width),
          height_(height)
    {
        InitWindow(
            width_,
            height_,
            windowTitle
        );

        SetTargetFPS(30);
    }

    RaylibCanvas::~RaylibCanvas()
    {
        if (IsWindowReady())
        {
            CloseWindow();
        }
    }

    bool RaylibCanvas::shouldClose() const
    {
        return WindowShouldClose();
    }

    void RaylibCanvas::beginFrame()
    {
        BeginDrawing();
    }

    void RaylibCanvas::endFrame()
    {
        EndDrawing();
    }

    void RaylibCanvas::clear(const Ink ink)
    {
        ClearBackground(
            toRaylibColor(ink)
        );
    }

    void RaylibCanvas::fillRect(
        const Rect& area,
        const Ink ink
    )
    {
        DrawRectangleRec(
            {
                area.x,
                area.y,
                area.width,
                area.height
            },
            toRaylibColor(ink)
        );
    }

    void RaylibCanvas::drawRect(
        const Rect& area,
        const float thickness,
        const Ink ink
    )
    {
        DrawRectangleLinesEx(
            {
                area.x,
                area.y,
                area.width,
                area.height
            },
            thickness,
            toRaylibColor(ink)
        );
    }

    void RaylibCanvas::drawLine(
        const Point start,
        const Point end,
        const float thickness,
        const Ink ink
    )
    {
        DrawLineEx(
            {
                start.x,
                start.y
            },
            {
                end.x,
                end.y
            },
            thickness,
            toRaylibColor(ink)
        );
    }

    void RaylibCanvas::fillCircle(
        const Point center,
        const float radius,
        const Ink ink
    )
    {
        DrawCircleV(
            {
                center.x,
                center.y
            },
            radius,
            toRaylibColor(ink)
        );
    }

    void RaylibCanvas::drawCircle(
        const Point center,
        const float radius,
        const float thickness,
        const Ink ink
    )
    {
        /*
         * Raylib bietet für Kreislinien keine direkte Dicke an.
         * Deshalb zeichnen wir mehrere konzentrische Kreislinien.
         */
        const int lineCount =
            thickness < 1.0F
                ? 1
                : static_cast<int>(thickness);

        for (int offset = 0; offset < lineCount; ++offset)
        {
            DrawCircleLines(
                static_cast<int>(center.x),
                static_cast<int>(center.y),
                radius - static_cast<float>(offset),
                toRaylibColor(ink)
            );
        }
    }

    void RaylibCanvas::drawText(
        const std::string_view text,
        const Rect& area,
        const int fontSize,
        const Ink ink,
        const TextAlignment alignment,
        const float rotationDegrees
    )
    {
        const Font font =
            GetFontDefault();

        /*
         * Raylib benötigt einen nullterminierten String.
         */
        const std::string ownedText{text};

        const Vector2 textSize =
            MeasureTextEx(
                font,
                ownedText.c_str(),
                static_cast<float>(fontSize),
                1.0F
            );

        Vector2 position = {
            area.x,
            area.y + area.height / 2.0F
        };

        Vector2 origin = {
            0.0F,
            textSize.y / 2.0F
        };

        switch (alignment)
        {
        case TextAlignment::Left:
            position.x = area.x;
            origin.x = 0.0F;
            break;

        case TextAlignment::Center:
            position.x =
                area.x + area.width / 2.0F;

            origin.x =
                textSize.x / 2.0F;
            break;

        case TextAlignment::Right:
            position.x =
                area.x + area.width;

            origin.x =
                textSize.x;
            break;
        }

        DrawTextPro(
            font,
            ownedText.c_str(),
            position,
            origin,
            rotationDegrees,
            static_cast<float>(fontSize),
            1.0F,
            toRaylibColor(ink)
        );
    }

    void RaylibCanvas::invalidate(
        const Rect& area
    )
    {
        /*
         * Der Desktop-Simulator zeichnet weiterhin komplette Frames.
         * Deshalb ist invalidate hier noch ein No-op.
         *
         * Der StickyCanvas wird diesen Bereich später für
         * Partial Refresh vormerken.
         */
        (void)area;
    }

    void RaylibCanvas::flush()
    {
        /*
         * Bei Raylib geschieht die Ausgabe bereits durch EndDrawing().
         * Auf dem Sticky wird flush() den E-Ink-Refresh auslösen.
         */
    }

    Color RaylibCanvas::toRaylibColor(
        const Ink ink
    )
    {
        switch (ink)
        {
        case Ink::White:
            return {
                255,
                255,
                255,
                255
            };

        case Ink::LightGray:
            return {
                190,
                190,
                190,
                255
            };

        case Ink::DarkGray:
            return {
                85,
                85,
                85,
                255
            };

        case Ink::Black:
            return {
                0,
                0,
                0,
                255
            };
        }

        return BLACK;
    }

    void RaylibCanvas::drawMonochromeBitmap(
        const std::uint8_t* bitmap,
        const int bitmapWidth,
        const int bitmapHeight,
        const int bytesPerRow,
        const sticky_lotus::ui::Rect& targetArea,
        const float rotationDegrees
    )
    {
        if (
            bitmap == nullptr ||
            bitmapWidth <= 0 ||
            bitmapHeight <= 0 ||
            bytesPerRow <= 0
        )
        {
            return;
        }

        const bool upsideDown =
            rotationDegrees == 180.0F;

        for (int sourceY = 0; sourceY < bitmapHeight; ++sourceY)
        {
            for (int sourceX = 0; sourceX < bitmapWidth; ++sourceX)
            {
                const int byteIndex =
                    sourceY * bytesPerRow +
                    sourceX / 8;

                const int bitIndex =
                    7 - (sourceX % 8);

                const bool white =
                (
                    bitmap[byteIndex] >>
                    bitIndex
                ) & 0x01;

                if (white)
                {
                    continue;
                }

                int displayX = sourceX;
                int displayY = sourceY;

                if (upsideDown)
                {
                    displayX =
                        bitmapWidth - 1 - sourceX;

                    displayY =
                        bitmapHeight - 1 - sourceY;
                }

                DrawPixel(
                    static_cast<int>(targetArea.x) + displayX,
                    static_cast<int>(targetArea.y) + displayY,
                    BLACK
                );
            }
        }
    }
} // namespace sticky_lotus_raylib
