#include "sticky_lotus_firmware.h"

#include "sticky_lotus/app/Application.h"
#include "sticky_lotus_sticky/StickyCanvas.h"
#include "sticky_lotus_sticky/StickyImageRenderer.h"
#include "sticky_lotus_sticky/StickyInputProvider.h"

#include "epaper_panel.h"
#include "sticky_board.h"
#include "sticky_board_config.h"

#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace sticky_lotus_firmware {

namespace {

constexpr const char* logTag = "sticky_lotus_app";

EpaperPanelConfig buildPanelConfig()
{
    EpaperPanelConfig config{};
    config.spi_host = STICKY_SHARED_SPI_HOST;
    config.cs = STICKY_EPD_CS_PIN;
    config.dc = STICKY_EPD_DC_PIN;
    config.rst = STICKY_EPD_RST_PIN;
    config.busy = STICKY_EPD_BUSY_PIN;
    config.mosi = STICKY_SHARED_SPI_MOSI_PIN;
    config.miso = STICKY_SHARED_SPI_MISO_PIN;
    config.sck = STICKY_SHARED_SPI_CLK_PIN;
    config.external_spi_bus = true;
    config.buffer_len = STICKY_EPD_BUFFER_LEN;
    config.busy_timeout_ms = 10000;
    config.reset_low_ms = 2;
    config.reset_high_ms = 50;
    config.busy_level = 1;
    return config;
}

} // namespace

esp_err_t run()
{
    ESP_LOGI(logTag, "Initializing Sticky hardware");

    ESP_RETURN_ON_ERROR(
        sticky_board::EnsureSharedSpiBus(),
        logTag,
        "Could not initialize shared SPI bus"
    );

    ESP_RETURN_ON_ERROR(
        sticky_board::EnableEpaperPower(),
        logTag,
        "Could not enable E-paper power"
    );

    static EpaperPanel panel(
        STICKY_EPD_WIDTH,
        STICKY_EPD_HEIGHT,
        buildPanelConfig()
    );

    ESP_RETURN_ON_ERROR(
        panel.Initialize(),
        logTag,
        "Could not initialize E-paper panel"
    );

    ESP_LOGI(
        logTag,
        "Starting shared Sticky Lotus application at %d x %d",
        STICKY_EPD_WIDTH,
        STICKY_EPD_HEIGHT
    );

    /*
     * Diese Objekte leben während der gesamten Laufzeit der Firmware.
     *
     * Als lokale Variablen würden insbesondere Input-Queue,
     * Renderer und Application den kleinen Stack des main-Tasks
     * belasten. Durch static liegen ihre Daten stattdessen im
     * statischen Speicher.
     */
    static sticky_lotus_sticky::StickyCanvas canvas(
        panel,
        STICKY_EPD_WIDTH,
        STICKY_EPD_HEIGHT
    );

    static sticky_lotus_sticky::StickyImageRenderer imageRenderer(
        canvas
    );

    static sticky_lotus_sticky::StickyInputProvider inputProvider;

    static sticky_lotus::app::Application application(
        canvas,
        imageRenderer,
        inputProvider
    );

    /*
     * Der erste Tick zeichnet die Startansicht.
     */
    /*
 * Erster Frame wird normal erzeugt.
 */
    application.tick();

    /*
     * Beim Start wollen wir nicht erst 500 ms warten.
     */
    canvas.flushImmediately();

    ESP_LOGI(
        logTag,
        "Initial Sticky Lotus screen rendered"
    );

    while (true) {
        /*
         * Alle verfügbaren Eingaben möglichst schnell
         * nacheinander verarbeiten.
         */
        while (inputProvider.hasPendingInput()) {
            application.tick();
        }

        /*
         * Der tatsächliche E-Paper-Refresh erfolgt erst,
         * wenn seit 500 ms keine neue Darstellung mehr
         * erzeugt wurde.
         */
        canvas.serviceRefresh();

        vTaskDelay(
            pdMS_TO_TICKS(10)
        );
    }


}
} // namespace sticky_lotus_firmware
