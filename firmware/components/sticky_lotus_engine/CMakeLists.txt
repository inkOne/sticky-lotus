idf_component_register(
    SRCS
        "../../../engine/src/AppRenderer.cpp"
        "../../../engine/src/Application.cpp"
        "../../../engine/src/TableLayout.cpp"
        "../../../engine/src/screens/GameScreen.cpp"
        "../../../engine/src/screens/SettingsScreen.cpp"
        "../../../engine/src/screens/CommanderDamageScreen.cpp"
        "../../../engine/src/screens/ScreenManager.cpp"
        "../../../engine/src/screens/PoisonScreen.cpp"
    INCLUDE_DIRS
        "../../../engine/include"
    PRIV_INCLUDE_DIRS
        "../../../assets"
    REQUIRES
        sticky_lotus_core
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_20)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Wpedantic)
