idf_component_register(
    SRCS "sticky_board.cpp"
    INCLUDE_DIRS "include"
    REQUIRES esp_adc esp_driver_gpio esp_driver_i2c esp_driver_spi
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)
