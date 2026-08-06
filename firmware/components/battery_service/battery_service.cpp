#include "battery_service.h"

#include "sticky_board.h"

#include "driver/i2c_master.h"
#include "esp_check.h"
#include "esp_log.h"

#include <algorithm>
#include <cstdint>

namespace battery_service {

namespace {

constexpr const char* logTag =
    "BatteryService";

/*
 * BQ27220-Standardbefehle.
 */
constexpr std::uint8_t voltageRegister =
    0x08;

constexpr std::uint8_t stateOfChargeRegister =
    0x2C;

i2c_master_bus_handle_t sensorBus =
    nullptr;

i2c_master_dev_handle_t batteryDevice =
    nullptr;

bool initialized = false;

esp_err_t readUnsigned16(
    const std::uint8_t command,
    std::uint16_t* outValue
)
{
    if (
        batteryDevice == nullptr ||
        outValue == nullptr
    ) {
        return ESP_ERR_INVALID_ARG;
    }

    std::uint8_t data[2] = {};

    ESP_RETURN_ON_ERROR(
        i2c_master_transmit_receive(
            batteryDevice,
            &command,
            1,
            data,
            sizeof(data),
            100
        ),
        logTag,
        "BQ27220 register read failed"
    );

    /*
     * Der BQ27220 liefert das niederwertige Byte zuerst.
     */
    *outValue =
        static_cast<std::uint16_t>(data[0]) |
        (
            static_cast<std::uint16_t>(data[1])
            << 8
        );

    return ESP_OK;
}

} // namespace

    esp_err_t Init()
{
    if (initialized) {
        return ESP_OK;
    }

    ESP_RETURN_ON_ERROR(
        sticky_board::EnsureSensorI2cBus(
            &sensorBus
        ),
        logTag,
        "Could not initialize sensor I2C bus"
    );

    ESP_RETURN_ON_ERROR(
        sticky_board::AddBq27220Device(
            sensorBus,
            &batteryDevice
        ),
        logTag,
        "Could not add BQ27220 device"
    );

    ESP_RETURN_ON_ERROR(
        sticky_board::ConfigureChargerPins(),
        logTag,
        "Could not configure charger pins"
    );

    initialized = true;

    return ESP_OK;
}

esp_err_t Read(
    BatteryState* outState
)
{
    if (outState == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!initialized) {
        ESP_RETURN_ON_ERROR(
            Init(),
            logTag,
            "Battery service initialization failed"
        );
    }

    std::uint16_t stateOfCharge = 0;
    std::uint16_t voltage = 0;

    ESP_RETURN_ON_ERROR(
        readUnsigned16(
            stateOfChargeRegister,
            &stateOfCharge
        ),
        logTag,
        "Could not read state of charge"
    );

    ESP_RETURN_ON_ERROR(
        readUnsigned16(
            voltageRegister,
            &voltage
        ),
        logTag,
        "Could not read battery voltage"
    );

    outState->percent =
        std::clamp(
            static_cast<int>(stateOfCharge),
            0,
            100
        );

    outState->voltageMv =
        static_cast<int>(voltage);

    /*
     * Charging wird später über AverageCurrent
     * oder den Ladecontroller ergänzt.
     */
    bool charging = false;

    const esp_err_t chargeResult =
        sticky_board::ReadChargeState(
            &charging
        );

    if (chargeResult == ESP_OK) {
        outState->charging =
            charging;
    } else {
        outState->charging =
            false;

        ESP_LOGW(
            logTag,
            "Could not read charging state: %s",
            esp_err_to_name(
                chargeResult
            )
        );
    }

    return ESP_OK;
}

} // namespace battery_service