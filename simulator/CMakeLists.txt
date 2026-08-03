add_executable(
        sticky-lotus-simulator
        src/main.cpp
)

target_include_directories(
        sticky-lotus-simulator
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
        sticky-lotus-simulator
        PRIVATE
        sticky-lotus-core
        sticky-lotus-engine
        sticky-lotus-raylib-platform
)

target_compile_features(
        sticky-lotus-simulator
        PRIVATE
        cxx_std_20
)

target_compile_options(
        sticky-lotus-simulator
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)