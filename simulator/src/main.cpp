#include "sticky_lotus/app/Application.h"
#include "sticky_lotus/ui/AppRenderer.h"
#include "sticky_lotus_raylib/RaylibCanvas.h"
#include "sticky_lotus_raylib/RaylibInputProvider.h"
#include "sticky_lotus_raylib/RaylibImageRenderer.h"

using sticky_lotus::app::Application;
using sticky_lotus::ui::AppRenderer;

using sticky_lotus_raylib::RaylibCanvas;
using sticky_lotus_raylib::RaylibInputProvider;
using sticky_lotus_raylib::RaylibImageRenderer;

int main()
{
    RaylibCanvas canvas(
        AppRenderer::screenWidth,
        AppRenderer::screenHeight,
        "Sticky Lotus Simulator"
    );

    /*
     * Der ImageRenderer muss nach dem Canvas konstruiert und
     * vor dem Canvas zerstört werden.
     *
     * Wegen der umgekehrten Zerstörungsreihenfolge lokaler
     * Variablen ist diese Reihenfolge korrekt.
     */
    RaylibImageRenderer imageRenderer;
    RaylibInputProvider input;

    Application application(
        canvas,
        imageRenderer,
        input
    );

    while (!canvas.shouldClose()) {
        application.tick();
    }

    return 0;
}