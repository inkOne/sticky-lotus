cmake_minimum_required(VERSION 3.24)

project(
        sticky_lotus
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

add_executable(
        sticky-lotus
        src/main.cpp
        src/GameState.cpp
        include/GameSettings.h
)

target_include_directories(
        sticky-lotus
        PRIVATE
        include
        ${RAYLIB_INCLUDE_DIRS}
)

target_link_directories(
        sticky-lotus
        PRIVATE
        ${RAYLIB_LIBRARY_DIRS}
)

target_link_libraries(
        sticky-lotus
        PRIVATE
        ${RAYLIB_LIBRARIES}
)

target_compile_options(
        sticky-lotus
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        ${RAYLIB_CFLAGS_OTHER}
)