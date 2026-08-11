idf_component_register(
        SRCS
        "../../../platform/sticky/src/StickyCanvas.cpp"
        "../../../platform/sticky/src/StickyInputProvider.cpp"

        INCLUDE_DIRS
        "../../../platform/sticky/include"

        REQUIRES
        sticky_lotus_engine
        epaper_panel
        epaper_ui
        board
        driver
        touch_service
)

target_compile_features(
        ${COMPONENT_LIB}
        PUBLIC
        cxx_std_20
)

target_compile_options(
        ${COMPONENT_LIB}
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)