#pragma once

#include "sticky_lotus/ui/ImageRenderer.h"

namespace sticky_lotus_sticky {

class StickyCanvas;

/**
 * Kleine eingebettete Icon-Implementierung für die Firmware.
 *
 * Der Simulator darf PNG-Dateien laden. Auf dem Device werden statische
 * Bitmaps verwendet, damit keine Dateisystem- oder PNG-Abhängigkeit nötig ist.
 */
class StickyImageRenderer final : public sticky_lotus::ui::ImageRenderer
{
public:
    explicit StickyImageRenderer(StickyCanvas& canvas);

    void drawImage(
        sticky_lotus::ui::ImageId image,
        const sticky_lotus::ui::Rect& targetArea,
        sticky_lotus::ui::ImageFit fit,
        float rotationDegrees
    ) override;

private:
    StickyCanvas& canvas_;
};

} // namespace sticky_lotus_sticky
