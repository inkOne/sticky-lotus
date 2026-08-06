#pragma once

#include "esp_err.h"

namespace battery_service {

    struct BatteryState
    {
        int percent = -1;
        int voltageMv = -1;
        bool charging = false;
    };

    /**
     * Initialisiert den Sensor-I²C-Bus und legt das
     * BQ27220-Gerät an.
     */
    esp_err_t Init();

    /**
     * Liest den aktuellen Batteriezustand.
     */
    esp_err_t Read(
        BatteryState* outState
    );

} // namespace battery_service