add_subdirectory(models)
add_subdirectory(viewmodels)
add_subdirectory(views)

add_executable(MPVDeck
    main.cpp
    MPVDeckApplication.cpp
    ../resources/resources.qrc
)

target_link_libraries(MPVDeck PRIVATE Qt6::Widgets MPVDeckViewModels MPVDeckViews)
target_compile_features(MPVDeck PRIVATE cxx_std_20)

# Add a custom target for running clang-tidy checks
find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    add_custom_target(clang-tidy-check
        COMMAND ${CLANG_TIDY}
                -p=${CMAKE_BINARY_DIR} # Path to compile_commands.json
                -checks="*,-clang-analyzer-alpha*"
                ${CMAKE_SOURCE_DIR}/src/models/ConfigManager.cpp # Example: specify files to check
                ${CMAKE_SOURCE_DIR}/src/viewmodels/SettingsViewModel.cpp
                # Add other source files or use a glob pattern for all .cpp files in src/
                # For example: file(GLOB_RECURSE ALL_CPP_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
                # And then pass ${ALL_CPP_FILES} to clang-tidy
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy checks"
        VERBATIM
    )
    # Make sure this target is run when building 'all' or 'check'
    # add_dependencies(clang-tidy-check MPVDeck) # This would run it with every build, which might be slow.
                                                # Better to run it manually or as a separate CI step.
else()
    message(WARNING "clang-tidy not found, skipping static analysis target.")
endif()