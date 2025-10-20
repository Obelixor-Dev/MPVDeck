add_library(MPVDeckCore STATIC
    ConfigManager.cpp
    ConfigManager.h
)

target_include_directories(MPVDeckCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MPVDeckCore PRIVATE Qt6::Widgets)
target_compile_features(MPVDeckCore PRIVATE cxx_std_20)
