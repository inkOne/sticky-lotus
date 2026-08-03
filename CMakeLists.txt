cmake_minimum_required(VERSION 3.24)

project(
        sticky_lotus
        VERSION 0.3.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Spiellogik.
add_subdirectory(core)

# Plattformunabhängige UI-Engine.
add_subdirectory(engine)

# Raylib-Implementierung der Plattform-Schnittstellen.
add_subdirectory(platform/raylib)

# Desktop-Simulator.
add_subdirectory(simulator)