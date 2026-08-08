#include "sticky_lotus_firmware.h"

#include "sticky_lotus/app/Application.h"
#include "sticky_lotus_sticky/StickyCanvas.h"
#include "sticky_lotus_sticky/StickyImageRenderer.h"
#include "sticky_lotus_sticky/StickyInputProvider.h"

#include "battery_service.h"
#include "epaper_panel.h"
#include "sticky_board.h"
#include "sticky_board_config.h"

#include "driver/gpio.h"

#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <cstdint>

namespace sticky_lotus_firmware
{
    namespace
    {
        constexpr const char* logTag =
            "sticky_lotus_app";

        /*
         * Batteriestand einmal pro Minute aktualisieren.
         */
        constexpr std::int64_t batteryUpdateIntervalUs =
            5LL * 60LL * 1'000'000LL; //5LL =5min

        /**
         * Erstellt die Hardwarekonfiguration für das E-Paper-Panel.
         */
        EpaperPanelConfig buildPanelConfig()
        {
            EpaperPanelConfig config{};

            config.spi_host =
                STICKY_SHARED_SPI_HOST;

            config.cs =
                STICKY_EPD_CS_PIN;

            config.dc =
                STICKY_EPD_DC_PIN;

            config.rst =
                STICKY_EPD_RST_PIN;

            config.busy =
                STICKY_EPD_BUSY_PIN;

            config.mosi =
                STICKY_SHARED_SPI_MOSI_PIN;

            config.miso =
                STICKY_SHARED_SPI_MISO_PIN;

            config.sck =
                STICKY_SHARED_SPI_CLK_PIN;

            config.external_spi_bus = true;
            config.buffer_len = STICKY_EPD_BUFFER_LEN;
            config.busy_timeout_ms = 10000;
            config.reset_low_ms = 2;
            config.reset_high_ms = 50;
            config.busy_level = 1;

            return config;
        }

        /**
         * Konfiguriert die Haupttaste als digitalen Eingang.
         *
         * GPIO 4 wird als Active-Low-Taste angenommen:
         *
         * nicht gedrückt = HIGH
         * gedrückt       = LOW
         */
        esp_err_t configurePowerButton()
        {
            gpio_config_t config{};

            config.pin_bit_mask =
                1ULL << STICKY_POWER_BUTTON_PIN;

            config.mode =
                GPIO_MODE_INPUT;

            config.pull_up_en =
                GPIO_PULLUP_ENABLE;

            config.pull_down_en =
                GPIO_PULLDOWN_DISABLE;

            config.intr_type =
                GPIO_INTR_DISABLE;

            return gpio_config(
                &config
            );
        }

        /**
         * Prüft, ob die Haupttaste aktuell gedrückt ist.
         */
        bool isPowerButtonPressed()
        {
            return gpio_get_level(
                STICKY_POWER_BUTTON_PIN
            ) == 0;
        }

        /**
         * Versetzt zuerst das E-Paper und anschließend
         * den ESP32-S3 in den Deep Sleep.
         *
         * Das vorhandene E-Paper-Bild bleibt dabei sichtbar.
         */
        void enterDeepSleep(
            EpaperPanel& panel,
            sticky_lotus::app::Application& application,
            sticky_lotus_sticky::StickyCanvas& canvas
        )
        {
            ESP_LOGI(
                logTag,
                "Power button pressed; preparing deep sleep"
            );

            /*
             * Speziellen Standby-Bildschirm in den Framebuffer zeichnen.
             */
            application.showSleepScreen();

            /*
             * Den Sleep-Screen sofort vollständig auf das E-Paper
             * übertragen. Erst danach darf der Panelcontroller schlafen.
             */
            canvas.flushImmediately();

            ESP_LOGI(
                logTag,
                "Sleep screen displayed"
            );

            /*
             * Displaycontroller schlafen legen.
             * Das zuletzt dargestellte Bild bleibt auf dem E-Paper sichtbar.
             */
            const esp_err_t panelSleepResult =
                panel.Sleep();

            if (panelSleepResult != ESP_OK)
            {
                ESP_LOGW(
                    logTag,
                    "Could not put E-paper panel to sleep: %s",
                    esp_err_to_name(
                        panelSleepResult
                    )
                );
            }

            /*
             * Warten, bis die Taste losgelassen wurde.
             * Sonst könnte derselbe LOW-Pegel sofort wieder aufwecken.
             */
            while (isPowerButtonPressed())
            {
                vTaskDelay(
                    pdMS_TO_TICKS(20)
                );
            }

            /*
             * Ein neuer LOW-Pegel auf GPIO 4 weckt den ESP32-S3 auf.
             */
            const esp_err_t wakeupResult =
                esp_sleep_enable_ext0_wakeup(
                    STICKY_POWER_BUTTON_PIN,
                    0
                );

            if (wakeupResult != ESP_OK)
            {
                ESP_LOGE(
                    logTag,
                    "Could not configure deep-sleep wakeup: %s",
                    esp_err_to_name(
                        wakeupResult
                    )
                );

                return;
            }

            ESP_LOGI(
                logTag,
                "Deep sleep starting"
            );

            vTaskDelay(
                pdMS_TO_TICKS(100)
            );

            esp_deep_sleep_start();
        }
    } // namespace

    esp_err_t run()
    {
        ESP_LOGI(
            logTag,
            "Initializing Sticky hardware"
        );

        /*
         * Haupttaste konfigurieren.
         */
        ESP_RETURN_ON_ERROR(
            configurePowerButton(),
            logTag,
            "Could not configure power button"
        );

        /*
         * Optional im Log anzeigen, wodurch das Gerät gestartet wurde.
         */
        const esp_sleep_wakeup_cause_t wakeupCause =
            esp_sleep_get_wakeup_cause();

        ESP_LOGI(
            logTag,
            "Wakeup cause: %d",
            static_cast<int>(
                wakeupCause
            )
        );

        /*
         * Gemeinsamen SPI-Bus initialisieren.
         */
        ESP_RETURN_ON_ERROR(
            sticky_board::EnsureSharedSpiBus(),
            logTag,
            "Could not initialize shared SPI bus"
        );

        /*
         * Stromversorgung für das E-Paper aktivieren.
         */
        ESP_RETURN_ON_ERROR(
            sticky_board::EnableEpaperPower(),
            logTag,
            "Could not enable E-paper power"
        );

        /*
         * Batteriemessung initialisieren.
         */
        ESP_RETURN_ON_ERROR(
            battery_service::Init(),
            logTag,
            "Could not initialize battery service"
        );

        battery_service::BatteryState battery{};

        const esp_err_t initialBatteryResult =
            battery_service::Read(
                &battery
            );

        if (initialBatteryResult != ESP_OK)
        {
            ESP_LOGW(
                logTag,
                "Initial battery read failed: %s",
                esp_err_to_name(
                    initialBatteryResult
                )
            );
        }

        /*
         * E-Paper-Panel erzeugen.
         */
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
         * Diese Objekte leben für die gesamte Laufzeit
         * im statischen Speicher.
         *
         * Dadurch wird der Stack des ESP-IDF-Main-Tasks
         * nicht unnötig belastet.
         */
        static sticky_lotus_sticky::StickyCanvas canvas(
            panel,
            STICKY_EPD_WIDTH,
            STICKY_EPD_HEIGHT
        );

        static sticky_lotus_sticky::StickyImageRenderer
            imageRenderer(
                canvas
            );

        static sticky_lotus_sticky::StickyInputProvider
            inputProvider;

        static sticky_lotus::app::Application application(
            canvas,
            imageRenderer,
            inputProvider
        );

        /*
         * Initialen Batteriestand an die Anwendung übergeben.
         */
        if (initialBatteryResult == ESP_OK)
        {
            application.setBatteryStatus(
                battery.percent,
                battery.charging
            );

            ESP_LOGI(
                logTag,
                "Initial battery: %d%%, %d mV, charging=%s",
                battery.percent,
                battery.voltageMv,
                battery.charging
                    ? "true"
                    : "false"
            );
        }

        /*
         * Ersten Bildschirm zeichnen.
         */
        application.draw();

        /*
         * Beim Start direkt aktualisieren und nicht auf die
         * verzögerte Refresh-Logik warten.
         */
        canvas.flushImmediately();

        ESP_LOGI(
            logTag,
            "Initial Sticky Lotus screen rendered"
        );

        std::int64_t lastBatteryUpdate =
            esp_timer_get_time();

        /*
         * Aktuellen Zustand übernehmen, damit ein bereits
         * gehaltener Button beim Start nicht als neue Flanke gilt.
         */
        bool previousPowerButtonPressed =
            isPowerButtonPressed();

        while (true)
        {
            /*
             * Alle gepufferten Touch-Eingaben verarbeiten.
             */
            constexpr std::size_t maximumInputsPerCycle = 4;

            std::size_t processedInputs = 0;

            while (
                inputProvider.hasPendingInput() &&
                processedInputs < maximumInputsPerCycle
            )
            {
                application.tick();
                ++processedInputs;

                /*,
                 * Scheduler und Idle-Task kurz zum Zug kommen lassen.
                 */
                taskYIELD();
            }

            /*
             * Power-Taste in jedem Schleifendurchlauf prüfen.
             */
            const bool powerButtonPressed =
                isPowerButtonPressed();

            /*
             * Nur auf die neue Druckflanke reagieren:
             *
             * vorher nicht gedrückt
             * jetzt gedrückt
             */
            if (
                powerButtonPressed &&
                !previousPowerButtonPressed
            )
            {
                enterDeepSleep(
                    panel,
                    application,
                    canvas
                );
            }

            previousPowerButtonPressed =
                powerButtonPressed;

            const std::int64_t currentTime =
                esp_timer_get_time();

            /*
             * Batteriestand regelmäßig aktualisieren.
             */
            if (
                currentTime - lastBatteryUpdate >=
                batteryUpdateIntervalUs
            )
            {
                const esp_err_t batteryResult =
                    battery_service::Read(
                        &battery
                    );

                if (batteryResult == ESP_OK)
                {
                    application.setBatteryStatus(
                        battery.percent,
                        battery.charging
                    );

                    /*
                     * Batteriewert erneut in den Framebuffer zeichnen.
                     * Der tatsächliche Panel-Refresh erfolgt später
                     * über serviceRefresh().
                     */
                    application.tick();

                    ESP_LOGI(
                        logTag,
                        "Battery updated: %d%%, %d mV, charging=%s",
                        battery.percent,
                        battery.voltageMv,
                        battery.charging
                            ? "true"
                            : "false"
                    );
                }
                else
                {
                    ESP_LOGW(
                        logTag,
                        "Battery update failed: %s",
                        esp_err_to_name(
                            batteryResult
                        )
                    );
                }

                lastBatteryUpdate =
                    currentTime;
            }

            /*
             * Vorgemerkten E-Paper-Refresh ausführen,
             * sobald die konfigurierte Ruhezeit abgelaufen ist.
             */
            canvas.serviceRefresh();

            vTaskDelay(
                pdMS_TO_TICKS(10)
            );
        }
    }
} // namespace sticky_lotus_firmware
