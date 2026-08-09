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

#include "nvs.h"
#include "nvs_flash.h"

#include <cstdint>

namespace sticky_lotus_firmware
{
    namespace
    {
        constexpr const char* logTag =
            "sticky_lotus_app";

        /*
         * Persistenter Speicherbereich für den Spielstand.
         */
        constexpr const char* gameStateNamespace =
            "sticky_lotus";

        constexpr const char* gameStateKey =
            "game_state";

        /*
         * Batteriestand alle fünf Minuten aktualisieren.
         */
        constexpr std::int64_t batteryUpdateIntervalUs =
            5LL * 60LL * 1'000'000LL;

        /*
         * Vorwärtsdeklaration, weil enterDeepSleep()
         * saveGameState() bereits verwendet.
         */
        esp_err_t saveGameState(
            const sticky_lotus::app::Application& application
        );

        /**
         * Erstellt die Hardwarekonfiguration
         * für das E-Paper-Panel.
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

            config.external_spi_bus =
                true;

            config.buffer_len =
                STICKY_EPD_BUFFER_LEN;

            config.busy_timeout_ms =
                10000;

            config.reset_low_ms =
                2;

            config.reset_high_ms =
                50;

            config.busy_level =
                1;

            return config;
        }

        /**
         * Konfiguriert die Haupttaste als digitalen Eingang.
         *
         * GPIO 4:
         *
         * HIGH = nicht gedrückt
         * LOW  = gedrückt
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
         * Prüft, ob die Haupttaste aktuell
         * gedrückt ist.
         */
        bool isPowerButtonPressed()
        {
            return gpio_get_level(
                STICKY_POWER_BUTTON_PIN
            ) == 0;
        }

        /**
         * Spielstand in NVS speichern.
         */
        esp_err_t saveGameState(
            const sticky_lotus::app::Application& application
        )
        {
            nvs_handle_t handle{};

            esp_err_t result =
                nvs_open(
                    gameStateNamespace,
                    NVS_READWRITE,
                    &handle
                );

            if (result != ESP_OK)
            {
                return result;
            }

            const sticky_lotus::GameSnapshot snapshot =
                application.createGameSnapshot();

            result =
                nvs_set_blob(
                    handle,
                    gameStateKey,
                    &snapshot,
                    sizeof(snapshot)
                );

            if (result == ESP_OK)
            {
                result =
                    nvs_commit(
                        handle
                    );
            }

            nvs_close(
                handle
            );

            return result;
        }

        /**
         * Gespeicherten Spielstand aus NVS laden.
         */
        esp_err_t loadGameState(
            sticky_lotus::app::Application& application
        )
        {
            nvs_handle_t handle{};

            esp_err_t result =
                nvs_open(
                    gameStateNamespace,
                    NVS_READONLY,
                    &handle
                );

            if (result != ESP_OK)
            {
                return result;
            }

            sticky_lotus::GameSnapshot snapshot{};

            std::size_t size =
                sizeof(snapshot);

            result =
                nvs_get_blob(
                    handle,
                    gameStateKey,
                    &snapshot,
                    &size
                );

            nvs_close(
                handle
            );

            if (result != ESP_OK)
            {
                return result;
            }

            if (size != sizeof(snapshot))
            {
                return ESP_ERR_INVALID_SIZE;
            }

            if (
                !application.restoreGameSnapshot(
                    snapshot
                )
            )
            {
                return ESP_ERR_INVALID_STATE;
            }

            return ESP_OK;
        }

        /**
         * Spielstand speichern, Sleep-Screen anzeigen,
         * E-Paper schlafen legen und anschließend
         * den ESP32-S3 in Deep Sleep versetzen.
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
             * Aktuellen Spielstand persistent sichern,
             * bevor Display und ESP32 schlafen gehen.
             */
            const esp_err_t saveResult =
                saveGameState(
                    application
                );

            if (saveResult == ESP_OK)
            {
                ESP_LOGI(
                    logTag,
                    "Game state saved"
                );
            }
            else
            {
                ESP_LOGE(
                    logTag,
                    "Could not save game state: %s",
                    esp_err_to_name(
                        saveResult
                    )
                );
            }

            /*
             * Standby-/Sleep-Screen zeichnen.
             */
            application.showSleepScreen();

            /*
             * Sleep-Screen sofort vollständig anzeigen.
             */
            canvas.flushImmediately();

            ESP_LOGI(
                logTag,
                "Sleep screen displayed"
            );

            /*
             * E-Paper-Controller schlafen legen.
             * Das sichtbare Bild bleibt erhalten.
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
             * Taste zunächst loslassen lassen.
             *
             * Sonst könnte der noch aktive LOW-Pegel
             * direkt wieder einen Wake-up verursachen.
             */
            while (isPowerButtonPressed())
            {
                vTaskDelay(
                    pdMS_TO_TICKS(20)
                );
            }

            /*
             * Erneutes Drücken der Active-Low-Taste
             * weckt das Gerät wieder auf.
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

            /*
             * Kurz warten, damit die letzte Logmeldung
             * noch über die serielle Schnittstelle ausgegeben wird.
             */
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
         * NVS initialisieren.
         *
         * Falls die NVS-Partition nicht mehr zur aktuellen
         * Struktur passt oder voll ist, einmal löschen
         * und neu initialisieren.
         */
        esp_err_t nvsResult =
            nvs_flash_init();

        if (
            nvsResult == ESP_ERR_NVS_NO_FREE_PAGES ||
            nvsResult == ESP_ERR_NVS_NEW_VERSION_FOUND
        )
        {
            ESP_RETURN_ON_ERROR(
                nvs_flash_erase(),
                logTag,
                "Could not erase NVS"
            );

            nvsResult =
                nvs_flash_init();
        }

        ESP_RETURN_ON_ERROR(
            nvsResult,
            logTag,
            "Could not initialize NVS"
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
         * Anzeigen, wodurch das Gerät gestartet wurde.
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
         * Gespeicherten Spielstand laden,
         * bevor der erste Screen gezeichnet wird.
         */
        const esp_err_t restoreResult =
            loadGameState(
                application
            );

        if (restoreResult == ESP_OK)
        {
            ESP_LOGI(
                logTag,
                "Saved game state restored"
            );
        }
        else if (restoreResult == ESP_ERR_NVS_NOT_FOUND)
        {
            ESP_LOGI(
                logTag,
                "No saved game state found"
            );
        }
        else
        {
            ESP_LOGW(
                logTag,
                "Could not restore saved game state: %s",
                esp_err_to_name(
                    restoreResult
                )
            );
        }

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
         * Ersten Screen auf Basis des jetzt gegebenenfalls
         * wiederhergestellten GameState zeichnen.
         */
        application.draw();

        /*
         * Beim Start sofort anzeigen.
         */
        canvas.flushImmediately();

        ESP_LOGI(
            logTag,
            "Initial Sticky Lotus screen rendered"
        );

        std::int64_t lastBatteryUpdate =
            esp_timer_get_time();

        /*
         * Aktuellen Zustand übernehmen, damit eine beim Boot
         * noch gehaltene Taste nicht direkt als neue Flanke gilt.
         */
        bool previousPowerButtonPressed =
            isPowerButtonPressed();

        while (true)
        {
            /*
             * Nicht unbegrenzt viele Touch-Events pro Zyklus
             * verarbeiten, damit Idle-Task und Watchdog
             * ausreichend CPU-Zeit bekommen.
             */
            constexpr std::size_t maximumInputsPerCycle =
                4;

            std::size_t processedInputs =
                0;

            while (
                inputProvider.hasPendingInput() &&
                processedInputs < maximumInputsPerCycle
            )
            {
                application.tick();

                ++processedInputs;

                /*
                 * Scheduler kurz zum Zug kommen lassen.
                 */
                taskYIELD();
            }

            /*
             * Power-Taste in jedem Schleifendurchlauf prüfen.
             */
            const bool powerButtonPressed =
                isPowerButtonPressed();

            /*
             * Nur auf die neue Druckflanke reagieren.
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
             * Batteriestand alle fünf Minuten aktualisieren.
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
                     * Batteriestatus in den aktuellen Framebuffer
                     * übernehmen.
                     */
                    application.draw();

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
             * Vorgemerkten E-Paper-Refresh durchführen,
             * sobald die konfigurierte Ruhezeit abgelaufen ist.
             */
            canvas.serviceRefresh();

            vTaskDelay(
                pdMS_TO_TICKS(10)
            );
        }
    }
} // namespace sticky_lotus_firmware