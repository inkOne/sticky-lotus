#pragma once

#include "sticky_lotus/ui/Canvas.h"

#include <cstdint>
#include <string_view>
#include "esp_timer.h"

class EpaperPanel;

namespace sticky_lotus_sticky
{
    /**
     * Canvas-Implementierung für das monochrome E-Paper des Sticky.
     *
     * Die komplette plattformunabhängige Sticky-Lotus-Oberfläche verwendet
     * weiterhin sticky_lotus::ui::Canvas. Diese Klasse übersetzt die bereits
     * vorhandenen Zeichenbefehle in den 1-Bit-Framebuffer des EpaperPanel.
     */
    class StickyCanvas final : public sticky_lotus::ui::Canvas
    {
    public:
        StickyCanvas(
            EpaperPanel& panel,
            int width,
            int height
        );

        void beginFrame() override;
        void endFrame() override;

        void clear(sticky_lotus::ui::Ink ink) override;

        void fillRect(
            const sticky_lotus::ui::Rect& area,
            sticky_lotus::ui::Ink ink
        ) override;

        void drawRect(
            const sticky_lotus::ui::Rect& area,
            float thickness,
            sticky_lotus::ui::Ink ink
        ) override;

        void drawLine(
            sticky_lotus::ui::Point start,
            sticky_lotus::ui::Point end,
            float thickness,
            sticky_lotus::ui::Ink ink
        ) override;

        void fillCircle(
            sticky_lotus::ui::Point center,
            float radius,
            sticky_lotus::ui::Ink ink
        ) override;

        void drawCircle(
            sticky_lotus::ui::Point center,
            float radius,
            float thickness,
            sticky_lotus::ui::Ink ink
        ) override;

        void drawText(
            std::string_view text,
            const sticky_lotus::ui::Rect& area,
            int fontSize,
            sticky_lotus::ui::Ink ink,
            sticky_lotus::ui::TextAlignment alignment,
            float rotationDegrees
        ) override;

        void invalidate(
            const sticky_lotus::ui::Rect& area
        ) override;


        void drawMonochromeBitmap(
            const std::uint8_t* bitmap,
            int bitmapWidth,
            int bitmapHeight,
            int bytesPerRow
        );

        void flush() override;
        /**
         * Führt einen vorgemerkten Display-Refresh aus,
         * sobald seit der letzten Änderung genügend Ruhezeit
         * vergangen ist.
         */
        void serviceRefresh();

        /**
         * Erzwingt den Refresh sofort.
         *
         * Sinnvoll für die erste Darstellung beim Start.
         */
        void flushImmediately();

    private:
        void setPixel(
            int x,
            int y,
            sticky_lotus::ui::Ink ink
        );

        [[nodiscard]]
        bool shouldDrawBlack(
            sticky_lotus::ui::Ink ink,
            int x,
            int y
        ) const;

        EpaperPanel& panel_;
        int width_;
        int height_;
        bool dirty_ = false;

        bool refreshPending_ = false;

        std::int64_t lastFrameChangeTimeUs_ = 0;
        /** Zeit die gewartet wird bis die Eingabe ausgeführt wird.
         * **/
        static constexpr std::int64_t refreshDelayUs = 0;
    };
} // namespace sticky_lotus_sticky
