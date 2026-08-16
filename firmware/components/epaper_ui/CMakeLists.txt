idf_component_register(
    SRCS "generated_epaper_fonts.cpp" "bitmap_font.cpp"
    INCLUDE_DIRS "include"
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)
