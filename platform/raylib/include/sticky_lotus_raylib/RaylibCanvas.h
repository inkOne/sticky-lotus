#pragma once

#include "sticky_lotus/ui/Canvas.h"

#include <raylib.h>

namespace sticky_lotus_raylib
{
    /**
     * Implementiert Canvas mit Raylib.
     *
     * Diese Klasse ist ausschließlich für den Desktop-Simulator zuständig.
     * Keine UI-Klasse außerhalb der Raylib-Plattform sollte Raylib-Zeichenbefehle
     * direkt verwenden.
     */
    class RaylibCanvas final : public sticky_lotus::ui::Canvas
    {
    public:
        RaylibCanvas(
            int width,
            int height,
            const char* windowTitle
        );

        ~RaylibCanvas() override;

        RaylibCanvas(const RaylibCanvas&) = delete;
        RaylibCanvas& operator=(const RaylibCanvas&) = delete;

        /**
         * Gibt an, ob das Simulatorfenster geschlossen werden soll.
         */
        [[nodiscard]]
        bool shouldClose() const;

        void beginFrame() override;
        void endFrame() override;

        void clear(
            sticky_lotus::ui::Ink ink
        ) override;

        void fillRect(
            const sticky_lotus::ui::Rect& area,
            sticky_lotus::ui::Ink ink
        ) override;

        void drawRect(
            const sticky_lotus::ui::Rect& area,
            float thickness,
            sticky_lotus::ui::Ink ink
        ) override;
        void fillRoundedRect(
            const sticky_lotus::ui::Rect& rect,
            float radius,
            sticky_lotus::ui::Ink ink
        ) override;

        void drawRoundedRect(
            const sticky_lotus::ui::Rect& rect,
            float radius,
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

        void flush() override;
        void drawMonochromeBitmap(
            const std::uint8_t* bitmap,
            int bitmapWidth,
            int bitmapHeight,
            int bytesPerRow,
            const sticky_lotus::ui::Rect& targetArea,
            float rotationDegrees
        ) override;

    private:
        [[nodiscard]]
        static Color toRaylibColor(
            sticky_lotus::ui::Ink ink
        );

        int width_;
        int height_;
    };
} // namespace sticky_lotus_raylib
