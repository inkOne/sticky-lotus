find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

add_executable(
        sticky-lotus-simulator
        src/main.cpp
        src/RaylibRenderer.cpp
        src/RaylibInput.cpp
)

target_include_directories(
        sticky-lotus-simulator
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${RAYLIB_INCLUDE_DIRS}
)

target_link_directories(
        sticky-lotus-simulator
        PRIVATE
        ${RAYLIB_LIBRARY_DIRS}
)

target_link_libraries(
        sticky-lotus-simulator
        PRIVATE
        sticky-lotus-core
        ${RAYLIB_LIBRARIES}
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
        ${RAYLIB_CFLAGS_OTHER}
)