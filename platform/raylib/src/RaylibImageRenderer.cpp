#include "sticky_lotus_raylib/RaylibImageRenderer.h"

#include <algorithm>

namespace sticky_lotus_raylib {

using sticky_lotus::ui::ImageFit;
using sticky_lotus::ui::ImageId;
using sticky_lotus::ui::Rect;

namespace {

/**
 * Berechnet die tatsächliche Zeichenfläche des Bildes.
 *
 * targetArea ist dabei die maximal erlaubte Fläche.
 */
Rectangle calculateDestination(
    const Texture2D& texture,
    const Rect& targetArea,
    const ImageFit fit
)
{
    const float sourceWidth =
        static_cast<float>(texture.width);

    const float sourceHeight =
        static_cast<float>(texture.height);

    if (
        sourceWidth <= 0.0F ||
        sourceHeight <= 0.0F ||
        targetArea.width <= 0.0F ||
        targetArea.height <= 0.0F
    ) {
        return {};
    }

    float drawWidth =
        targetArea.width;

    float drawHeight =
        targetArea.height;

    /*
     * Stretch verwendet den Zielbereich unverändert.
     *
     * Contain und Cover behalten das Seitenverhältnis bei.
     */
    if (fit != ImageFit::Stretch) {
        const float horizontalScale =
            targetArea.width / sourceWidth;

        const float verticalScale =
            targetArea.height / sourceHeight;

        const float scale =
            fit == ImageFit::Contain
                ? std::min(
                    horizontalScale,
                    verticalScale
                )
                : std::max(
                    horizontalScale,
                    verticalScale
                );

        drawWidth =
            sourceWidth * scale;

        drawHeight =
            sourceHeight * scale;
    }

    /*
     * Das skalierte Bild wird im maximalen Zielbereich zentriert.
     */
    return {
        targetArea.x +
            (targetArea.width - drawWidth) / 2.0F,

        targetArea.y +
            (targetArea.height - drawHeight) / 2.0F,

        drawWidth,
        drawHeight
    };
}

} // namespace

RaylibImageRenderer::~RaylibImageRenderer()
{
    /*
     * Alle geladenen GPU-Texturen wieder freigeben.
     *
     * Der Destruktor muss ausgeführt werden, solange das
     * Raylib-Fenster beziehungsweise der Grafik-Kontext
     * noch existiert.
     */
    for (auto& [imageId, texture] : textures_) {
        (void)imageId;

        if (texture.id != 0) {
            UnloadTexture(texture);
        }
    }

    textures_.clear();
}

const char* RaylibImageRenderer::getImagePath(
    const ImageId image
)
{
    switch (image) {
        case ImageId::Skull:
            return "assets/icons/skull.png";

        case ImageId::Poison:
            return "assets/icons/poison.png";
    }

    return "";
}

Texture2D& RaylibImageRenderer::getTexture(
    const ImageId image
)
{
    /*
     * Bereits geladene Textur aus dem Cache zurückgeben.
     */
    const auto existing =
        textures_.find(image);

    if (existing != textures_.end()) {
        return existing->second;
    }

    const char* path =
        getImagePath(image);

    Image loadedImage =
        LoadImage(path);

    if (loadedImage.data == nullptr) {
        TraceLog(
            LOG_ERROR,
            "Could not load image: %s",
            path
        );

        static Texture2D invalidTexture{};
        return invalidTexture;
    }

    Texture2D texture =
        LoadTextureFromImage(loadedImage);

    UnloadImage(loadedImage);

    const auto [iterator, inserted] =
        textures_.emplace(
            image,
            texture
        );

    (void)inserted;

    return iterator->second;
}

void RaylibImageRenderer::drawImage(
    const ImageId image,
    const Rect& targetArea,
    const ImageFit fit,
    const float rotationDegrees
)
{
    Texture2D& texture =
        getTexture(image);

    if (texture.id == 0) {
        return;
    }

    const Rectangle source = {
        0.0F,
        0.0F,
        static_cast<float>(texture.width),
        static_cast<float>(texture.height)
    };

    const Rectangle destination =
        calculateDestination(
            texture,
            targetArea,
            fit
        );

    if (
        destination.width <= 0.0F ||
        destination.height <= 0.0F
    ) {
        return;
    }

    /*
     * DrawTexturePro dreht um den angegebenen Origin.
     *
     * Deshalb setzen wir die Position des Zielrechtecks auf
     * dessen Mittelpunkt und den Origin ebenfalls auf die Mitte.
     */
    const Rectangle centeredDestination = {
        destination.x +
            destination.width / 2.0F,

        destination.y +
            destination.height / 2.0F,

        destination.width,
        destination.height
    };

    const Vector2 origin = {
        destination.width / 2.0F,
        destination.height / 2.0F
    };

    DrawTexturePro(
        texture,
        source,
        centeredDestination,
        origin,
        rotationDegrees,
        WHITE
    );
}

} // namespace sticky_lotus_raylib