#pragma once

#include "sticky_lotus/ui/Geometry.h"
#include "sticky_lotus/ui/Image.h"

namespace sticky_lotus::ui {

    /**
     * Plattformunabhängige Schnittstelle zum Zeichnen von Bildern.
     *
     * Der AppRenderer bestimmt nur:
     *
     * - welches Bild,
     * - in welchem maximalen Bereich,
     * - mit welcher Skalierung,
     * - und mit welcher Drehung
     *
     * gezeichnet werden soll.
     *
     * Das eigentliche Laden und Rendern übernimmt die Plattform.
     */
    class ImageRenderer
    {
    public:
        virtual ~ImageRenderer() = default;

        virtual void drawImage(
            ImageId image,
            const Rect& targetArea,
            ImageFit fit = ImageFit::Contain,
            float rotationDegrees = 0.0F
        ) = 0;
    };

} // namespace sticky_lotus::ui