add_executable(MPVDeck
    main.cpp
    ../resources/resources.qrc
)

target_link_libraries(MPVDeck PRIVATE Qt6::Widgets MPVDeckViewModels MPVDeckViews)
target_compile_features(MPVDeck PRIVATE cxx_std_17)

add_subdirectory(models)
add_subdirectory(viewmodels)
add_subdirectory(views)
