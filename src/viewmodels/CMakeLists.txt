add_library(MPVDeckViewModels STATIC
    SettingsViewModel.cpp
    SettingsViewModel.h
    AudioViewModel.cpp
    AudioViewModel.h
    VideoViewModel.cpp
    VideoViewModel.h
    SubtitleViewModel.cpp
    SubtitleViewModel.h
    PlaybackBehaviorViewModel.cpp
    PlaybackBehaviorViewModel.h
    PerformanceCachingViewModel.cpp
    PerformanceCachingViewModel.h
    InterfaceOsdViewModel.cpp
    InterfaceOsdViewModel.h
)

target_include_directories(MPVDeckViewModels PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(MPVDeckViewModels PRIVATE Qt6::Widgets MPVDeckCore)
target_compile_features(MPVDeckViewModels PRIVATE cxx_std_20)
