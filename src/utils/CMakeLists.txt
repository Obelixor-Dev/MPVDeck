add_library(MPVDeckUtils STATIC
    MpvConfHighlighter.cpp
    MpvConfHighlighter.h
    ColorUtils.cpp
    ColorUtils.h
)

target_link_libraries(MPVDeckUtils PRIVATE Qt6::Widgets Qt6::Core)
target_compile_features(MPVDeckUtils PRIVATE cxx_std_20)
