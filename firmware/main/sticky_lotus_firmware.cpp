#include "sticky_lotus_firmware.h"

#include "sticky_lotus/app/Application.h"
#include "sticky_lotus_sticky/StickyCanvas.h"
#include "sticky_lotus_sticky/StickyInputProvider.h"

#include "battery_service.h"
#include "buzzer_service.h"
#include "epaper_panel.h"
#include "sticky_board.h"
#include "sticky_board_config.h"

#include "sticky_lotus_1bit.h"

#include "driver/gpio.h"

#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_system.h"
#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "nvs.h"
#include "nvs_flash.h"

#include <cstdint>
#include <cstdio>


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

        constexpr const char* diceSidesKey =
            "dice_sides";

        constexpr const char* profilesKey =
            "profiles_v1";

        /*
         * Batteriestand nur bei Bedienung auslesen, und auch dann
         * höchstens alle 30 Sekunden - bei entladenem Akku kann der
         * I2C-Zugriff auf den Fuel Gauge lange blockieren und würde
         * sonst jeden Tap verzögern.
         */
        constexpr std::int64_t batteryReadMinIntervalUs =
            30LL * 1'000'000LL;

        /*
         * Nach dieser Zeit ohne Bedienung geht das Gerät in Deep Sleep.
         */
        constexpr std::int64_t idleSleepTimeoutUs =
            10LL * 60LL * 1'000'000LL;

        /*
         * Temporärer Diagnose-Build: zeigt Boot-Zähler, Reset-Grund
         * und Akkuspannung auf dem Display und gibt beim Start drei
         * Klicks sowie danach alle 5 s einen Klick als Herzschlag aus.
         * Dient dazu, Probleme im Akkubetrieb (ohne USB-Monitor)
         * sichtbar zu machen. Für Releases auf false setzen.
         */
        constexpr bool diagnosticBuild =
            false;

        constexpr std::int64_t heartbeatIntervalUs =
            5LL * 1'000'000LL;

        const char* resetReasonName(
            const esp_reset_reason_t reason
        )
        {
            switch (reason)
            {
            case ESP_RST_POWERON:
                return "POWERON";
            case ESP_RST_EXT:
                return "EXT";
            case ESP_RST_SW:
                return "SW";
            case ESP_RST_PANIC:
                return "PANIC";
            case ESP_RST_INT_WDT:
                return "INT_WDT";
            case ESP_RST_TASK_WDT:
                return "TASK_WDT";
            case ESP_RST_WDT:
                return "WDT";
            case ESP_RST_DEEPSLEEP:
                return "DEEPSLEEP";
            case ESP_RST_BROWNOUT:
                return "BROWNOUT";
            case ESP_RST_USB:
                return "USB";
            case ESP_RST_PWR_GLITCH:
                return "PWR_GLITCH";
            default:
                return "OTHER";
            }
        }

        std::uint32_t incrementBootCount()
        {
            nvs_handle_t handle{};

            if (
                nvs_open(
                    "sticky_lotus",
                    NVS_READWRITE,
                    &handle
                ) != ESP_OK
            )
            {
                return 0;
            }

            std::uint32_t count =
                0;

            nvs_get_u32(
                handle,
                "boot_count",
                &count
            );

            ++count;

            nvs_set_u32(
                handle,
                "boot_count",
                count
            );

            nvs_commit(
                handle
            );

            nvs_close(
                handle
            );

            return count;
        }

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
         * Konfiguriert die untere Taste (Down) als
         * digitalen Eingang.
         *
         * Löst "Roll Start" aus - siehe Hauptschleife.
         */
        esp_err_t configureDownButton()
        {
            gpio_config_t config{};

            config.pin_bit_mask =
                1ULL << STICKY_BUTTON_DOWN_PIN;

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
         * Prüft, ob die untere Taste aktuell
         * gedrückt ist.
         */
        bool isDownButtonPressed()
        {
            return gpio_get_level(
                STICKY_BUTTON_DOWN_PIN
            ) == 0;
        }

        /**
         * Konfiguriert die mittlere Taste (Up) als
         * digitalen Eingang.
         *
         * Setzt bei 3s Halten das Spiel zurück -
         * siehe Hauptschleife.
         */
        esp_err_t configureUpButton()
        {
            gpio_config_t config{};

            config.pin_bit_mask =
                1ULL << STICKY_BUTTON_UP_PIN;

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
         * Prüft, ob die mittlere Taste aktuell
         * gedrückt ist.
         */
        bool isUpButtonPressed()
        {
            return gpio_get_level(
                STICKY_BUTTON_UP_PIN
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
                    nvs_set_i32(
                        handle,
                        diceSidesKey,
                        application.diceSides()
                    );
            }

            /*
             * Profile (Namen, Siege, Sitzplätze) liegen in einem eigenen
             * Blob. Statischer Puffer, damit die ~800 Byte nicht auf dem
             * kleinen Main-Task-Stack landen.
             */
            static sticky_lotus::ProfileSnapshot profileBuffer;

            if (result == ESP_OK)
            {
                profileBuffer =
                    application.profileSnapshot();

                result =
                    nvs_set_blob(
                        handle,
                        profilesKey,
                        &profileBuffer,
                        sizeof(profileBuffer)
                    );
            }

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
         * Gespeicherte Spielerprofile aus NVS laden.
         *
         * Unabhängig vom Spielstand: auch ohne gespeichertes Spiel
         * bleiben Namen und Siege erhalten.
         */
        esp_err_t loadProfiles(
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

            static sticky_lotus::ProfileSnapshot profileBuffer;

            std::size_t size =
                sizeof(profileBuffer);

            result =
                nvs_get_blob(
                    handle,
                    profilesKey,
                    &profileBuffer,
                    &size
                );

            nvs_close(
                handle
            );

            if (result != ESP_OK)
            {
                return result;
            }

            if (size != sizeof(profileBuffer))
            {
                return ESP_ERR_INVALID_SIZE;
            }

            if (!application.restoreProfiles(profileBuffer))
            {
                return ESP_ERR_INVALID_STATE;
            }

            return ESP_OK;
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

            std::int32_t diceSides = 6;

            if (
                nvs_get_i32(
                    handle,
                    diceSidesKey,
                    &diceSides
                ) == ESP_OK
            )
            {
                application.setDiceSides(
                    diceSides
                );
            }

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
                "Preparing deep sleep"
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
             * Fertigen 1-Bit-Sleep-Screen direkt in den
             * Sticky-Framebuffer übernehmen.
             */
            canvas.drawMonochromeBitmap(
                sticky_lotus_fb,
                STICKY_LOTUS_FB_WIDTH,
                STICKY_LOTUS_FB_HEIGHT,
                STICKY_LOTUS_FB_BYTES_PER_ROW,
                sticky_lotus::ui::Rect{
                    0.0F,
                    0.0F,
                    static_cast<float>(STICKY_LOTUS_FB_WIDTH),
                    static_cast<float>(STICKY_LOTUS_FB_HEIGHT)
                },
                0.0F
            );

            /*
             * Das Bild sofort vollständig auf das E-Paper übertragen.
             * Danach darf der Displaycontroller schlafen.
             */
            canvas.flushImmediately();

            /*
             * Zweiter, echter Full-Refresh direkt über das Panel.
             *
             * canvas.flushImmediately() ein zweites Mal aufzurufen
             * würde nichts bewirken: der Canvas ist nach dem ersten
             * Aufruf bereits "nicht mehr dirty" und überspringt den
             * Refresh dann intern. Das Sleep-Bild bleibt aber unter
             * Umständen sehr lange sichtbar - ein erneuter kompletter
             * Full-Refresh-Zyklus (nochmaliges Schwarz/Weiß-Spülen
             * des Panels) entfernt zuverlässiger jegliches
             * Restghosting aus den vorherigen Partial-Refreshes,
             * bevor das Bild dauerhaft stehen bleibt.
             */
            const esp_err_t secondRefreshResult =
                panel.RefreshFull();

            if (secondRefreshResult != ESP_OK)
            {
                ESP_LOGW(
                    logTag,
                    "Second (ghosting-clearing) full refresh failed: %s",
                    esp_err_to_name(secondRefreshResult)
                );
            }

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
        /*
         * Strom-Halteschaltung (Power Latch) aktivieren - als Erstes.
         *
         * Im Akkubetrieb bleibt die Systemspannung nur erhalten, solange
         * dieser Latch gesetzt ist. Ohne ihn läuft das Gerät lediglich
         * am USB-Kabel oder solange die Power-Taste gehalten wird und
         * geht beim Abziehen bzw. Loslassen sofort aus. Der Latch behält
         * seinen Zustand nur, solange der Akku nicht komplett leer war -
         * die Firmware muss ihn daher bei jedem Start selbst setzen.
         *
         * Nicht kritisch für den USB-Betrieb - Fehler nur loggen.
         */
        {
            const esp_err_t holdResult =
                sticky_board::EnablePowerHold();

            if (holdResult != ESP_OK)
            {
                ESP_LOGW(
                    logTag,
                    "Could not enable power hold: %s",
                    esp_err_to_name(holdResult)
                );
            }
        }

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
         * Untere Taste (Roll Start) konfigurieren.
         */
        ESP_RETURN_ON_ERROR(
            configureDownButton(),
            logTag,
            "Could not configure down button"
        );

        /*
         * Mittlere Taste (Würfel: kurz = würfeln, halten = Auswahl) konfigurieren.
         */
        ESP_RETURN_ON_ERROR(
            configureUpButton(),
            logTag,
            "Could not configure up button"
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

        /*
         * Buzzer für kurzes Klick-Feedback initialisieren.
         *
         * Nicht kritisch für den Betrieb - Fehler nur loggen,
         * die App läuft auch ohne Klickfeedback weiter.
         */
        {
            const esp_err_t buzzerResult =
                buzzer_service::Init();

            if (buzzerResult != ESP_OK)
            {
                ESP_LOGW(
                    logTag,
                    "Could not initialize buzzer service: %s",
                    esp_err_to_name(buzzerResult)
                );
            }
        }

        const std::uint32_t bootCount =
            diagnosticBuild
                ? incrementBootCount()
                : 0;

        if constexpr (diagnosticBuild)
        {
            for (int i = 0; i < 3; ++i)
            {
                buzzer_service::Click();

                vTaskDelay(
                    pdMS_TO_TICKS(200)
                );
            }
        }

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


        static sticky_lotus_sticky::StickyInputProvider
            inputProvider;

        static sticky_lotus::app::Application application(
            canvas,
            inputProvider
        );

        /*
         * Profile zuerst laden, unabhängig davon, ob ein Spielstand
         * existiert.
         */
        {
            const esp_err_t profileResult =
                loadProfiles(
                    application
                );

            if (profileResult == ESP_OK)
            {
                ESP_LOGI(
                    logTag,
                    "Player profiles restored"
                );
            }
            else if (profileResult != ESP_ERR_NVS_NOT_FOUND)
            {
                ESP_LOGW(
                    logTag,
                    "Could not restore player profiles: %s",
                    esp_err_to_name(
                        profileResult
                    )
                );
            }
        }

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

        if constexpr (diagnosticBuild)
        {
            char line[96];

            std::snprintf(
                line,
                sizeof(line),
                "boot #%lu  rst=%s  bat=%d mV",
                static_cast<unsigned long>(bootCount),
                resetReasonName(
                    esp_reset_reason()
                ),
                initialBatteryResult == ESP_OK
                    ? battery.voltageMv
                    : -1
            );

            canvas.drawText(
                line,
                sticky_lotus::ui::Rect{
                    8.0F,
                    452.0F,
                    480.0F,
                    20.0F
                },
                14,
                sticky_lotus::ui::Ink::Black,
                sticky_lotus::ui::TextAlignment::Left,
                0.0F
            );
        }

        /*
         * Beim Start sofort anzeigen.
         */
        canvas.flushImmediately();

        ESP_LOGI(
            logTag,
            "Initial Sticky Lotus screen rendered"
        );

        std::int64_t lastBatteryReadUs =
            esp_timer_get_time();

        std::int64_t lastActivityUs =
            lastBatteryReadUs;

        /*
         * Nächster erlaubter Speicherversuch für geänderte Profile.
         */
        std::int64_t nextProfileSaveUs =
            0;

        std::int64_t lastHeartbeatUs =
            lastBatteryReadUs;

        /*
         * Wird bei jeder Bedienung (Touch oder Taste) aufgerufen,
         * bevor deren Redraw erfolgt: setzt den Leerlauf-Timer
         * zurück und holt - gedrosselt - einen frischen Batteriestand,
         * damit er im folgenden Redraw angezeigt wird. Es gibt keinen
         * eigenständigen Batterie-Refresh im Leerlauf mehr.
         */
        const auto onOperation =
            [&]()
        {
            const std::int64_t now =
                esp_timer_get_time();

            lastActivityUs =
                now;

            if (
                now - lastBatteryReadUs <
                batteryReadMinIntervalUs
            )
            {
                return;
            }

            lastBatteryReadUs =
                now;

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

                ESP_LOGI(
                    logTag,
                    "Battery read: %d%%, %d mV, charging=%s",
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
                    "Battery read failed: %s",
                    esp_err_to_name(
                        batteryResult
                    )
                );
            }
        };

        /*
         * Aktuellen Zustand übernehmen, damit eine beim Boot
         * noch gehaltene Taste nicht direkt als neue Flanke gilt.
         */
        bool previousPowerButtonPressed =
            isPowerButtonPressed();

        /*
         * Zeitpunkt, seit dem die Power-Taste ununterbrochen
         * gedrückt gehalten wird. 0 bedeutet: nicht gedrückt.
         */
        std::int64_t powerButtonPressStartUs =
            0;

        bool previousDownButtonPressed =
            isDownButtonPressed();

        bool previousUpButtonPressed =
            isUpButtonPressed();

        /*
         * Zeitpunkt, seit dem die mittlere Taste ununterbrochen
         * gedrückt gehalten wird. 0 bedeutet: nicht gedrückt.
         */
        std::int64_t upButtonPressStartUs =
            0;

        /*
         * Öffnet die Würfelauswahl nur einmal pro Halten und
         * unterdrückt das Würfeln beim anschließenden Loslassen.
         */
        bool upButtonHoldTriggered =
            false;

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

            if (inputProvider.hasPendingInput())
            {
                onOperation();
            }

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
             *
             * Erst nach kontinuiertem powerButtonHoldUs-Halten
             * auslösen, um versehentliches Auslösen beim
             * Greifen/Bedienen des Geräts zu vermeiden.
             */
            constexpr std::int64_t powerButtonHoldUs =
                1LL * 1'000'000LL;

            const bool powerButtonPressed =
                isPowerButtonPressed();

            if (
                powerButtonPressed &&
                !previousPowerButtonPressed
            )
            {
                powerButtonPressStartUs =
                    esp_timer_get_time();

                onOperation();
            }

            if (
                powerButtonPressed &&
                powerButtonPressStartUs != 0 &&
                esp_timer_get_time() - powerButtonPressStartUs >=
                powerButtonHoldUs
            )
            {
                enterDeepSleep(
                    panel,
                    application,
                    canvas
                );
            }

            if (!powerButtonPressed)
            {
                powerButtonPressStartUs =
                    0;
            }

            previousPowerButtonPressed =
                powerButtonPressed;

            /*
             * Untere Taste: "Roll Start" - reagiert bewusst nur
             * auf die Druckflanke, unabhängig vom aktuellen Screen.
             */
            const bool downButtonPressed =
                isDownButtonPressed();

            if (
                downButtonPressed &&
                !previousDownButtonPressed
            )
            {
                onOperation();

                application.rollStartingPlayer();
            }

            previousDownButtonPressed =
                downButtonPressed;

            /*
             * Mittlere Taste (Würfel): kurz drücken und loslassen
             * würfelt, etwa 0,8 s halten öffnet die Auswahl von Münze
             * und Würfeln. Nach einem langen Halten würfelt das
             * Loslassen nicht zusätzlich. Zurücksetzen geht über die
             * Settings.
             */
            constexpr std::int64_t upButtonHoldUs =
                800'000LL;

            const bool upButtonPressed =
                isUpButtonPressed();

            if (
                upButtonPressed &&
                !previousUpButtonPressed
            )
            {
                upButtonPressStartUs =
                    esp_timer_get_time();

                upButtonHoldTriggered =
                    false;

                onOperation();
            }

            if (
                upButtonPressed &&
                !upButtonHoldTriggered &&
                upButtonPressStartUs != 0 &&
                esp_timer_get_time() - upButtonPressStartUs >=
                upButtonHoldUs
            )
            {
                application.showDiceSelector();

                upButtonHoldTriggered =
                    true;
            }

            if (!upButtonPressed)
            {
                if (
                    previousUpButtonPressed &&
                    !upButtonHoldTriggered &&
                    upButtonPressStartUs != 0 &&
                    esp_timer_get_time() - upButtonPressStartUs >=
                    50'000LL
                )
                {
                    application.rollDice();
                }

                upButtonPressStartUs =
                    0;

                upButtonHoldTriggered =
                    false;
            }

            previousUpButtonPressed =
                upButtonPressed;

            /*
             * Hebt z.B. die "Roll Start"-Hervorhebung auf, sobald
             * lange genug keine Eingabe mehr kam.
             */
            application.checkTimeouts();

            /*
             * Geänderte Profile (neuer Name, Sieg, Reset) gleich
             * zusammen mit dem Spiel speichern, damit sie einen
             * Stromverlust überstehen. Bei einem Fehler nach fünf
             * Sekunden erneut versuchen.
             */
            if (
                application.profilesDirty() &&
                esp_timer_get_time() >= nextProfileSaveUs
            )
            {
                const esp_err_t saveResult =
                    saveGameState(
                        application
                    );

                if (saveResult == ESP_OK)
                {
                    application.profilesSaved();

                    ESP_LOGI(
                        logTag,
                        "Player profiles and game saved"
                    );
                }
                else
                {
                    ESP_LOGE(
                        logTag,
                        "Saving player profiles failed: %s; retrying",
                        esp_err_to_name(
                            saveResult
                        )
                    );

                    nextProfileSaveUs =
                        esp_timer_get_time() + 5'000'000LL;
                }
            }

            /*
             * Vorgemerkten E-Paper-Refresh durchführen,
             * sobald die konfigurierte Ruhezeit abgelaufen ist.
             */
            canvas.serviceRefresh();

            if constexpr (diagnosticBuild)
            {
                const std::int64_t heartbeatNow =
                    esp_timer_get_time();

                if (
                    heartbeatNow - lastHeartbeatUs >=
                    heartbeatIntervalUs
                )
                {
                    lastHeartbeatUs =
                        heartbeatNow;

                    buzzer_service::Click();
                }
            }

            /*
             * Lange keine Bedienung: Spielstand sichern und in
             * Deep Sleep gehen. Kehrt enterDeepSleep() zurück
             * (Wakeup-Konfiguration fehlgeschlagen), Timer neu
             * starten statt jeden Durchlauf erneut zu versuchen.
             */
            if (
                esp_timer_get_time() - lastActivityUs >=
                idleSleepTimeoutUs
            )
            {
                ESP_LOGI(
                    logTag,
                    "No input for %lld s; entering deep sleep",
                    idleSleepTimeoutUs / 1'000'000LL
                );

                enterDeepSleep(
                    panel,
                    application,
                    canvas
                );

                lastActivityUs =
                    esp_timer_get_time();
            }

            vTaskDelay(
                pdMS_TO_TICKS(10)
            );
        }
    }
} // namespace sticky_lotus_firmware
