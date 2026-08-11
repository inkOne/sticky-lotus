#include "sticky_lotus_sticky/StickyCanvas.h"

#include "epaper_panel.h"
#include "epaper_ui/bitmap_font.h"
#include "epaper_ui/generated_epaper_fonts.h"

#include "esp_log.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace sticky_lotus_sticky
{
    using sticky_lotus::ui::Ink;
    using sticky_lotus::ui::Point;
    using sticky_lotus::ui::Rect;
    using sticky_lotus::ui::TextAlignment;

    namespace
    {
        constexpr const char* logTag = "StickyCanvas";

        const epaper_font::BitmapFont& fontForSize(const int fontSize)
        {
            if (fontSize <= 22)
            {
                return epaper_fonts::kInter22SemiBold;
            }
            if (fontSize <= 26)
            {
                return epaper_fonts::kInter26SemiBold;
            }
            if (fontSize <= 32)
            {
                return epaper_fonts::kInter32SemiBold;
            }
            if (fontSize <= 38)
            {
                return epaper_fonts::kInter38SemiBold;
            }
            if (fontSize <= 46)
            {
                return epaper_fonts::kInter46Bold;
            }
            if (fontSize <= 70)
            {
                return epaper_fonts::kInter55Bold;
            }

            return epaper_fonts::kInter165Black;
        }

        int rounded(const float value)
        {
            return static_cast<int>(std::lround(value));
        }
    } // namespace

    StickyCanvas::StickyCanvas(
        EpaperPanel& panel,
        const int width,
        const int height
    )
        : panel_(panel),
          width_(width),
          height_(height)
    {
    }

    void StickyCanvas::beginFrame()
    {
    }

    void StickyCanvas::endFrame()
    {
    }

    void StickyCanvas::clear(const Ink ink)
    {
        panel_.Clear(ink == Ink::White || ink == Ink::LightGray);

        if (ink == Ink::DarkGray || ink == Ink::LightGray)
        {
            fillRect(
                {0.0F, 0.0F, static_cast<float>(width_), static_cast<float>(height_)},
                ink
            );
        }

        dirty_ = true;
    }

    bool StickyCanvas::shouldDrawBlack(
        const Ink ink,
        const int x,
        const int y
    ) const
    {
        switch (ink)
        {
        case Ink::White:
            return false;
        case Ink::Black:
            return true;
        case Ink::DarkGray:
            return ((x + y) & 1) == 0;
        case Ink::LightGray:
            return ((x & 1) == 0) && ((y & 1) == 0);
        }

        return true;
    }

    void StickyCanvas::setPixel(
        const int x,
        const int y,
        const Ink ink
    )
    {
        if (
            x < 0 || y < 0 ||
            x >= width_ || y >= height_ ||
            panel_.framebuffer() == nullptr
        )
        {
            return;
        }

        const std::size_t pixelIndex =
            static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) +
            static_cast<std::size_t>(x);

        const std::size_t byteIndex = pixelIndex / 8U;
        const std::uint8_t mask =
            static_cast<std::uint8_t>(0x80U >> (pixelIndex % 8U));

        if (shouldDrawBlack(ink, x, y))
        {
            panel_.framebuffer()[byteIndex] &=
                static_cast<std::uint8_t>(~mask);
        }
        else
        {
            panel_.framebuffer()[byteIndex] |= mask;
        }
    }

    void StickyCanvas::fillRect(
        const Rect& area,
        const Ink ink
    )
    {
        const int left = std::max(0, rounded(area.x));
        const int top = std::max(0, rounded(area.y));
        const int right = std::min(width_, rounded(area.x + area.width));
        const int bottom = std::min(height_, rounded(area.y + area.height));

        for (int y = top; y < bottom; ++y)
        {
            for (int x = left; x < right; ++x)
            {
                setPixel(x, y, ink);
            }
        }

        dirty_ = true;
    }

    void StickyCanvas::drawRect(
        const Rect& area,
        const float thickness,
        const Ink ink
    )
    {
        const float line = std::max(1.0F, thickness);

        fillRect({area.x, area.y, area.width, line}, ink);
        fillRect({area.x, area.y + area.height - line, area.width, line}, ink);
        fillRect({area.x, area.y, line, area.height}, ink);
        fillRect({area.x + area.width - line, area.y, line, area.height}, ink);
    }

    void StickyCanvas::drawLine(
        const Point start,
        const Point end,
        const float thickness,
        const Ink ink
    )
    {
        int x0 = rounded(start.x);
        int y0 = rounded(start.y);
        const int x1 = rounded(end.x);
        const int y1 = rounded(end.y);

        const int dx = std::abs(x1 - x0);
        const int sx = x0 < x1 ? 1 : -1;
        const int dy = -std::abs(y1 - y0);
        const int sy = y0 < y1 ? 1 : -1;
        int error = dx + dy;

        const int radius = std::max(0, rounded(thickness) / 2);

        while (true)
        {
            for (int offsetY = -radius; offsetY <= radius; ++offsetY)
            {
                for (int offsetX = -radius; offsetX <= radius; ++offsetX)
                {
                    setPixel(x0 + offsetX, y0 + offsetY, ink);
                }
            }

            if (x0 == x1 && y0 == y1)
            {
                break;
            }

            const int twiceError = 2 * error;
            if (twiceError >= dy)
            {
                error += dy;
                x0 += sx;
            }
            if (twiceError <= dx)
            {
                error += dx;
                y0 += sy;
            }
        }

        dirty_ = true;
    }

    void StickyCanvas::fillCircle(
        const Point center,
        const float radius,
        const Ink ink
    )
    {
        const int centerX = rounded(center.x);
        const int centerY = rounded(center.y);
        const int radiusInt = std::max(0, rounded(radius));
        const int radiusSquared = radiusInt * radiusInt;

        for (int y = -radiusInt; y <= radiusInt; ++y)
        {
            for (int x = -radiusInt; x <= radiusInt; ++x)
            {
                if (x * x + y * y <= radiusSquared)
                {
                    setPixel(centerX + x, centerY + y, ink);
                }
            }
        }

        dirty_ = true;
    }

    void StickyCanvas::drawCircle(
        const Point center,
        const float radius,
        const float thickness,
        const Ink ink
    )
    {
        const float outerRadius = std::max(0.0F, radius);
        const float innerRadius = std::max(0.0F, radius - std::max(1.0F, thickness));
        const float outerSquared = outerRadius * outerRadius;
        const float innerSquared = innerRadius * innerRadius;
        const int bound = rounded(outerRadius);

        for (int y = -bound; y <= bound; ++y)
        {
            for (int x = -bound; x <= bound; ++x)
            {
                const float distanceSquared =
                    static_cast<float>(x * x + y * y);

                if (distanceSquared <= outerSquared && distanceSquared >= innerSquared)
                {
                    setPixel(rounded(center.x) + x, rounded(center.y) + y, ink);
                }
            }
        }

        dirty_ = true;
    }

    void StickyCanvas::drawText(
        const std::string_view text,
        const Rect& area,
        const int fontSize,
        const Ink ink,
        const TextAlignment alignment,
        const float rotationDegrees
    )
    {
        const epaper_font::BitmapFont& font = fontForSize(fontSize);
        const float scale =
            static_cast<float>(fontSize) / static_cast<float>(font.line_height);

        const int sourceWidth = epaper_font::MeasureText(font, text);
        const int textWidth = rounded(static_cast<float>(sourceWidth) * scale);
        const int textHeight = rounded(static_cast<float>(font.line_height) * scale);

        int left = rounded(area.x);
        if (alignment == TextAlignment::Center)
        {
            left = rounded(area.x + (area.width - static_cast<float>(textWidth)) / 2.0F);
        }
        else if (alignment == TextAlignment::Right)
        {
            left = rounded(area.x + area.width - static_cast<float>(textWidth));
        }

        const int top = rounded(
            area.y + (area.height - static_cast<float>(textHeight)) / 2.0F
        );

        int cursorX = 0;
        for (const char character : text)
        {
            const epaper_font::GlyphBitmap* glyph =
                epaper_font::FindGlyph(font, character);

            if (glyph == nullptr)
            {
                continue;
            }

            const int glyphX = rounded(
                static_cast<float>(cursorX + glyph->bearing_x) * scale
            );
            const int glyphY = rounded(
                static_cast<float>(font.ascent - glyph->bearing_y) * scale
            );

            for (int row = 0; row < glyph->height; ++row)
            {
                for (int column = 0; column < glyph->width; ++column)
                {
                    const std::size_t bitIndex =
                        static_cast<std::size_t>(row) * glyph->width + column;
                    const std::size_t byteIndex =
                        glyph->bitmap_offset + bitIndex / 8U;
                    const std::uint8_t mask =
                        static_cast<std::uint8_t>(0x80U >> (bitIndex % 8U));

                    if ((font.bitmaps[byteIndex] & mask) == 0)
                    {
                        continue;
                    }

                    const int xStart = rounded(static_cast<float>(column) * scale);
                    const int xEnd = std::max(
                        xStart + 1,
                        rounded(static_cast<float>(column + 1) * scale)
                    );
                    const int yStart = rounded(static_cast<float>(row) * scale);
                    const int yEnd = std::max(
                        yStart + 1,
                        rounded(static_cast<float>(row + 1) * scale)
                    );

                    for (int scaledY = yStart; scaledY < yEnd; ++scaledY)
                    {
                        for (int scaledX = xStart; scaledX < xEnd; ++scaledX)
                        {
                            int targetX = left + glyphX + scaledX;
                            int targetY = top + glyphY + scaledY;

                            if (rotationDegrees == 180.0F)
                            {
                                targetX = rounded(area.x + area.width) - 1 -
                                    (targetX - rounded(area.x));
                                targetY = rounded(area.y + area.height) - 1 -
                                    (targetY - rounded(area.y));
                            }

                            setPixel(targetX, targetY, ink);
                        }
                    }
                }
            }

            cursorX += glyph->advance;
        }

        dirty_ = true;
    }

    void StickyCanvas::invalidate(const Rect& area)
    {
        (void)area;
        dirty_ = true;
    }

    void StickyCanvas::flush()
    {
        if (!dirty_)
        {
            return;
        }

        /*
         * Der Renderer hat einen neuen vollständigen Frame
         * erzeugt. Er wird aber noch nicht sofort ans Panel
         * übertragen.
         *
         * Jede weitere Eingabe verschiebt den Refresh erneut.
         */
        refreshPending_ = true;

        lastFrameChangeTimeUs_ =
            esp_timer_get_time();

        /*
         * Die Änderungen befinden sich bereits im Framebuffer.
         * dirty_ beschreibt hier nur, ob der aktuelle Renderdurchlauf
         * neue Inhalte erzeugt hat.
         */
        dirty_ = false;
    }

    void StickyCanvas::serviceRefresh()
    {
        if (!refreshPending_)
        {
            return;
        }

        if (
            esp_timer_get_time() -
            lastFrameChangeTimeUs_ <
            refreshDelayUs
        )
        {
            return;
        }

        ESP_LOGI(
            logTag,
            "Refreshing coalesced Sticky Lotus framebuffer"
        );

        const esp_err_t result =
            panel_.RefreshChangedRegion();

        if (result != ESP_OK)
        {
            ESP_LOGE(
                logTag,
                "Coalesced E-paper refresh failed: %s",
                esp_err_to_name(result)
            );

            return;
        }

        refreshPending_ = false;

        ESP_LOGI(
            logTag,
            "Coalesced framebuffer displayed"
        );
    }

    void StickyCanvas::flushImmediately()
    {
        if (
            !dirty_ &&
            !refreshPending_
        )
        {
            return;
        }

        ESP_LOGI(
            logTag,
            "Refreshing Sticky Lotus framebuffer immediately"
        );

        const esp_err_t result =
            panel_.RefreshFull();

        if (result != ESP_OK)
        {
            ESP_LOGE(
                logTag,
                "Immediate E-paper refresh failed: %s",
                esp_err_to_name(result)
            );

            return;
        }

        dirty_ = false;
        refreshPending_ = false;
    }

    void StickyCanvas::drawMonochromeBitmap(
    const std::uint8_t* bitmap,
    const int bitmapWidth,
    const int bitmapHeight,
    const int bytesPerRow,
    const Rect& targetArea,
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

            int displayX =
                sourceX;

            int displayY =
                sourceY;

            if (upsideDown)
            {
                displayX =
                    bitmapWidth - 1 - sourceX;

                displayY =
                    bitmapHeight - 1 - sourceY;
            }

            const int targetX =
                static_cast<int>(
                    targetArea.x
                ) +
                displayX;

            const int targetY =
                static_cast<int>(
                    targetArea.y
                ) +
                displayY;

            /*
             * Nur innerhalb des Displays zeichnen.
             */
            if (
                targetX < 0 ||
                targetY < 0 ||
                targetX >= width_ ||
                targetY >= height_
            )
            {
                continue;
            }

            setPixel(
                targetX,
                targetY,
                white
                    ? Ink::White
                    : Ink::Black
            );
        }
    }

    dirty_ = true;
}
} // namespace sticky_lotus_sticky
