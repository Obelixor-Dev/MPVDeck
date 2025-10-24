cmake_minimum_required(VERSION 3.16)
project(MPVDeck VERSION 1.0.0 LANGUAGES CXX)

# Path to the wrapper script
set(CLANG_TIDY_WRAPPER "${CMAKE_SOURCE_DIR}/tools/clang_tidy_wrapper.py")

# Only enable if clang-tidy is available
find_program(CLANG_TIDY_BIN NAMES clang-tidy)

if(CLANG_TIDY_BIN)
    message(STATUS "Enabling clang-tidy with wrapper: ${CLANG_TIDY_WRAPPER}")
    set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_WRAPPER}"
        "--extra-arg=-Wno-unknown-warning-option"
    )
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Test REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

find_program(CLANG_FORMAT_BIN NAMES clang-format)
if(CLANG_FORMAT_BIN)
    file(GLOB FORMAT_FILES
        "src/*.cpp"
        "src/*.h"
        "tests/*.cpp"
        "tests/*.h"
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_BIN} -i ${FORMAT_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

if(CLANG_TIDY_BIN)
    add_custom_target(clang-tidy-check
        COMMAND ${CLANG_TIDY_WRAPPER} -p ${CMAKE_BINARY_DIR} ${FORMAT_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on source files"
    )
endif()
