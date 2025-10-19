cmake_minimum_required(VERSION 3.16)
project(MPVDeck VERSION 1.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(MPVDeck
    src/main.cpp
    src/models/ConfigManager.cpp
    src/viewmodels/SettingsViewModel.cpp
    src/views/SettingsView.cpp
)

target_link_libraries(MPVDeck PRIVATE Qt6::Widgets)
