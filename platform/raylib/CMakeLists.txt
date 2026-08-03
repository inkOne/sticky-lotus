find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

add_library(
        sticky-lotus-raylib-platform
        STATIC
        src/RaylibCanvas.cpp
)

target_include_directories(
        sticky-lotus-raylib-platform
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${RAYLIB_INCLUDE_DIRS}
)

target_link_directories(
        sticky-lotus-raylib-platform
        PUBLIC
        ${RAYLIB_LIBRARY_DIRS}
)

target_link_libraries(
        sticky-lotus-raylib-platform
        PUBLIC
        sticky-lotus-engine
        ${RAYLIB_LIBRARIES}
)

target_compile_features(
        sticky-lotus-raylib-platform
        PUBLIC
        cxx_std_20
)

target_compile_options(
        sticky-lotus-raylib-platform
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        ${RAYLIB_CFLAGS_OTHER}
)