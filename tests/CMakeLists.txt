enable_testing()

add_executable(TestConfigManager
    TestConfigManager.cpp
)

target_link_libraries(TestConfigManager PRIVATE Qt6::Widgets Qt6::Test MPVDeckCore)
target_compile_features(TestConfigManager PRIVATE cxx_std_20)
add_test(NAME TestConfigManager COMMAND TestConfigManager)

add_executable(TestSettingsViewModel
    TestSettingsViewModel.cpp
    ../resources/resources.qrc
)

target_link_libraries(TestSettingsViewModel PRIVATE Qt6::Widgets Qt6::Test MPVDeckViewModels MPVDeckCore)
target_compile_features(TestSettingsViewModel PRIVATE cxx_std_20)
add_test(NAME TestSettingsViewModel COMMAND TestSettingsViewModel)
