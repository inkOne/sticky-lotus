idf_component_register(
        SRCS
        "battery_service.cpp"

        INCLUDE_DIRS
        "include"

        REQUIRES
        board
        esp_driver_i2c
)