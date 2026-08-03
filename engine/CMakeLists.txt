add_library(
        sticky-lotus-engine
        STATIC
        src/AppRenderer.cpp
        src/Application.cpp
        src/TableLayout.cpp
        src/screens/GameScreen.cpp
        src/screens/SettingsScreen.cpp
        src/screens/CommanderDamageScreen.cpp
        src/screens/ScreenManager.cpp
)

target_include_directories(
        sticky-lotus-engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
        sticky-lotus-engine
        PUBLIC
        sticky-lotus-core
)

target_compile_features(
        sticky-lotus-engine
        PUBLIC
        cxx_std_20
)

target_compile_options(
        sticky-lotus-engine
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)