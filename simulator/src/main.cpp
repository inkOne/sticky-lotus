#include "sticky_lotus/app/Application.h"
#include "sticky_lotus/ui/AppRenderer.h"
#include "sticky_lotus_raylib/RaylibCanvas.h"
#include "sticky_lotus_raylib/RaylibInputProvider.h"

using sticky_lotus::app::Application;
using sticky_lotus::ui::AppRenderer;

using sticky_lotus_raylib::RaylibCanvas;
using sticky_lotus_raylib::RaylibInputProvider;

int main()
{
    RaylibCanvas canvas(
        AppRenderer::screenWidth,
        AppRenderer::screenHeight,
        "Sticky Lotus Simulator"
    );

    RaylibInputProvider input;

    Application application(
        canvas,
        input
    );

    /*
     * Raylib benötigt im Gegensatz zum E-Ink-Display einen
     * durchgehenden Frame-Loop: BeginDrawing()/EndDrawing() müssen
     * jeden Durchlauf aufgerufen werden, sonst tauscht raylib nie
     * den Framebuffer und pollt keine Fensterereignisse, wodurch
     * macOS das Fenster als "reagiert nicht" meldet.
     */
    while (!canvas.shouldClose()) {
        application.tick();

        canvas.beginFrame();
        application.draw();
        canvas.endFrame();
    }

    return 0;
}