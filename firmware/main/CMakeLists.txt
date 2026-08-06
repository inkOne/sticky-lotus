idf_component_register(
    SRCS
        "main.cpp"
        "sticky_lotus_firmware.cpp"
    INCLUDE_DIRS
        "."
    REQUIRES
        nvs_flash
        board
        epaper_panel
        sticky_lotus_platform
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_20)
