#pragma once

#include "sticky_lotus/ui/ImageRenderer.h"

#include <map>

#include <raylib.h>

namespace sticky_lotus_raylib {

    /**
     * Bildrenderer für den Desktop-Simulator.
     *
     * Grafiken werden beim ersten Gebrauch geladen und anschließend
     * als Raylib-Texturen zwischengespeichert.
     */
    class RaylibImageRenderer final
        : public sticky_lotus::ui::ImageRenderer
    {
    public:
        RaylibImageRenderer() = default;

        ~RaylibImageRenderer() override;

        RaylibImageRenderer(
            const RaylibImageRenderer&
        ) = delete;

        RaylibImageRenderer& operator=(
            const RaylibImageRenderer&
        ) = delete;

        void drawImage(
            sticky_lotus::ui::ImageId image,
            const sticky_lotus::ui::Rect& targetArea,
            sticky_lotus::ui::ImageFit fit,
            float rotationDegrees
        ) override;

    private:
        std::map<
            sticky_lotus::ui::ImageId,
            Texture2D
        > textures_;

        [[nodiscard]]
        Texture2D& getTexture(
            sticky_lotus::ui::ImageId image
        );

        [[nodiscard]]
        static const char* getImagePath(
            sticky_lotus::ui::ImageId image
        );
    };

} // namespace sticky_lotus_raylib