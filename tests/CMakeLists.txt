add_executable(TestConfigManager
    TestConfigManager.cpp
)

target_link_libraries(TestConfigManager PRIVATE Qt6::Widgets Qt6::Test MPVDeckCore)
target_compile_features(TestConfigManager PRIVATE cxx_std_17)
add_test(NAME TestConfigManager COMMAND TestConfigManager)
