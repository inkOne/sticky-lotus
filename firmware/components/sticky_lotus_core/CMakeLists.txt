idf_component_register(
    SRCS
        "../../../core/src/GameState.cpp"
        "../../../core/src/DeathMessageGenerator.cpp"
    INCLUDE_DIRS
        "../../../core/include"
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_20)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Wpedantic)
