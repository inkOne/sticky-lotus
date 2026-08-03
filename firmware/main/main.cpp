#include "esp_log.h"

namespace {

    constexpr const char* logTag = "sticky_lotus";

}

/**
 * Einstiegspunkt der späteren Sticky-Firmware.
 *
 * ESP-IDF verwendet app_main() statt der normalen C++-Funktion main().
 *
 * In späteren Schritten werden hier initialisiert:
 *
 * 1. Board und Stromversorgung
 * 2. E-Paper-Display
 * 3. Touchcontroller
 * 4. Hardwaretasten
 * 5. Persistenter Speicher
 * 6. Sticky-Lotus-Anwendung
 */
extern "C" void app_main()
{
    ESP_LOGI(
        logTag,
        "Sticky Lotus firmware starting"
    );
}