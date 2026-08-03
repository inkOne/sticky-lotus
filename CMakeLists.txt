cmake_minimum_required(VERSION 3.24)

project(
        sticky_lotus
        VERSION 0.2.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gemeinsame, hardwareunabhängige Spiellogik.
add_subdirectory(core)

# macOS-Simulator mit Raylib.
add_subdirectory(simulator)