idf_component_register(
        SRCS
        "main.cpp"
        "sticky_lotus_firmware.cpp"

        INCLUDE_DIRS
        "."

        EMBED_FILES
        "../../assets/icons/poison.h"

        REQUIRES
            nvs_flash
            sticky_lotus_platform
            battery_service
            project_assets
)
target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_20)
