#include "sticky_lotus_sticky/StickyImageRenderer.h"

#include "sticky_lotus_sticky/StickyCanvas.h"
#include "sticky_lotus/ui/Canvas.h"

#include <algorithm>

namespace sticky_lotus_sticky {

using sticky_lotus::ui::ImageFit;
using sticky_lotus::ui::ImageId;
using sticky_lotus::ui::Ink;
using sticky_lotus::ui::Rect;

StickyImageRenderer::StickyImageRenderer(StickyCanvas& canvas)
    : canvas_(canvas)
{
}

void StickyImageRenderer::drawImage(
    const ImageId image,
    const Rect& targetArea,
    const ImageFit fit,
    const float rotationDegrees
)
{
    (void)fit;

    if (image != ImageId::Poison) {
        return;
    }

    static constexpr int poison[7][7] = {
        {0,0,0,1,0,0,0},
        {0,0,0,1,0,0,0},
        {0,1,0,1,0,1,0},
        {1,0,0,1,0,0,1},
        {0,1,0,1,0,1,0},
        {0,0,0,1,0,0,0},
        {0,0,0,1,0,0,0}
    };

    constexpr int rows = 7;
    constexpr int columns = 7;

    const float pixelSize = std::min(
        targetArea.width / static_cast<float>(columns),
        targetArea.height / static_cast<float>(rows)
    );

    const float drawingWidth = pixelSize * columns;
    const float drawingHeight = pixelSize * rows;
    const float startX = targetArea.x + (targetArea.width - drawingWidth) / 2.0F;
    const float startY = targetArea.y + (targetArea.height - drawingHeight) / 2.0F;
    const bool upsideDown = rotationDegrees == 180.0F;

    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            if (poison[row][column] == 0) {
                continue;
            }

            const int displayRow = upsideDown ? rows - 1 - row : row;
            const int displayColumn = upsideDown ? columns - 1 - column : column;

            canvas_.fillRect(
                {
                    startX + displayColumn * pixelSize,
                    startY + displayRow * pixelSize,
                    pixelSize,
                    pixelSize
                },
                Ink::Black
            );
        }
    }
}

} // namespace sticky_lotus_sticky
