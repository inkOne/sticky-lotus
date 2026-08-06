#include "sticky_lotus_firmware.h"

#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"

namespace {

    constexpr const char* logTag =
        "sticky_lotus";

    void initializeNvs()
    {
        esp_err_t result =
            nvs_flash_init();

        /*
         * NVS kann nach einer Änderung der Partitionierung oder
         * nach einem Versionswechsel nicht mehr verwendbar sein.
         *
         * In diesem Fall wird der NVS-Bereich gelöscht und
         * anschließend neu initialisiert.
         */
        if (
            result == ESP_ERR_NVS_NO_FREE_PAGES ||
            result == ESP_ERR_NVS_NEW_VERSION_FOUND
        ) {
            ESP_LOGW(
                logTag,
                "Resetting NVS partition"
            );

            ESP_ERROR_CHECK(
                nvs_flash_erase()
            );

            result =
                nvs_flash_init();
        }

        ESP_ERROR_CHECK(result);
    }

} // namespace

extern "C" void app_main()
{
    ESP_LOGI(
        logTag,
        "Sticky Lotus firmware starting"
    );

    initializeNvs();

    /*
     * Ab hier startet unsere eigentliche Anwendung.
     *
     * Display, Touch und Tasten werden nicht direkt in
     * app_main() implementiert. Dadurch bleibt der
     * Programmeinstieg klein und übersichtlich.
     */
    const esp_err_t result =
        sticky_lotus_firmware::run();

    if (result != ESP_OK) {
        ESP_LOGE(
            logTag,
            "Sticky Lotus failed: %s",
            esp_err_to_name(result)
        );

        ESP_ERROR_CHECK(result);
    }

    ESP_LOGI(
        logTag,
        "Sticky Lotus application stopped"
    );
}