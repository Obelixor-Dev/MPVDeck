add_library(MPVDeckViews STATIC
    SettingsView.cpp
    SettingsView.h
)

target_include_directories(MPVDeckViews PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MPVDeckViews PRIVATE Qt6::Widgets MPVDeckViewModels MPVDeckCore)
target_compile_features(MPVDeckViews PRIVATE cxx_std_17)
