#pragma once

namespace sticky_lotus::ui {

    /**
     * Zweidimensionaler Punkt.
     *
     * Diese Struktur ist unabhängig von Raylib, LVGL oder ESP-IDF.
     */
    struct Point
    {
        float x = 0.0F;
        float y = 0.0F;
    };

    /**
     * Rechteckiger Bildschirmbereich.
     */
    struct Rect
    {
        float x = 0.0F;
        float y = 0.0F;
        float width = 0.0F;
        float height = 0.0F;

        /**
         * Prüft, ob ein Punkt innerhalb des Rechtecks liegt.
         */
        [[nodiscard]]
        bool contains(const Point point) const
        {
            return
                point.x >= x &&
                point.x <= x + width &&
                point.y >= y &&
                point.y <= y + height;
        }
    };

} // namespace sticky_lotus::ui