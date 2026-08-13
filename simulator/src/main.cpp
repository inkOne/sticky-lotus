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

    while (!canvas.shouldClose()) {
        application.tick();
    }

    return 0;
}