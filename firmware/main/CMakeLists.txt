idf_component_register(
        SRCS
        "main.cpp"
        "sticky_lotus_firmware.cpp"

        INCLUDE_DIRS
        "."

        EMBED_FILES
        "../../assets/poison.bmp"

        REQUIRES
        nvs_flash
        sticky_lotus_platform
)
target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_20)
