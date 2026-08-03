# Hardwareunabhängige Spiellogik.
#
# Diese Bibliothek enthält absichtlich weder Raylib noch ESP-IDF-
# oder Sticky-Abhängigkeiten. Dadurch kann sie auf macOS sowie
# später auf dem ESP32-S3 kompiliert werden.

add_library(
        sticky-lotus-core
        STATIC
        src/GameState.cpp
)

target_include_directories(
        sticky-lotus-core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
        sticky-lotus-core
        PUBLIC
        cxx_std_20
)

target_compile_options(
        sticky-lotus-core
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)