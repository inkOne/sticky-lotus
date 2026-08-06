#pragma once

#include "esp_err.h"

namespace sticky_lotus_firmware {

    /**
     * Initialisiert die Sticky-Hardware und startet
     * die eigentliche Sticky-Lotus-Anwendung.
     */
    esp_err_t run();

} // namespace sticky_lotus_firmware