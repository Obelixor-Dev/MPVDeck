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

add_executable(TestDefaultsLoader
    TestDefaultsLoader.cpp
    ../resources/resources.qrc
)

target_link_libraries(TestDefaultsLoader PRIVATE Qt6::Widgets Qt6::Test MPVDeckViewModels MPVDeckCore)
target_compile_features(TestDefaultsLoader PRIVATE cxx_std_20)
add_test(NAME TestDefaultsLoader COMMAND TestDefaultsLoader)

add_executable(TestSettingsView
    TestSettingsView.cpp
    ../resources/resources.qrc
)

target_link_libraries(TestSettingsView PRIVATE Qt6::Widgets Qt6::Test MPVDeckViews MPVDeckViewModels MPVDeckCore MPVDeckUtils)
target_compile_features(TestSettingsView PRIVATE cxx_std_20)
add_test(NAME TestSettingsView COMMAND TestSettingsView)

add_executable(TestOptionBindings
    TestOptionBindings.cpp
)

target_link_libraries(TestOptionBindings PRIVATE Qt6::Widgets Qt6::Test MPVDeckViewModels MPVDeckCore)
target_compile_features(TestOptionBindings PRIVATE cxx_std_20)
add_test(NAME TestOptionBindings COMMAND TestOptionBindings)
