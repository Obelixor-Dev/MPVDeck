add_subdirectory(models)
add_subdirectory(viewmodels)
add_subdirectory(views)
add_subdirectory(utils)

add_executable(MPVDeck
    main.cpp
    MPVDeckApplication.cpp
    ../resources/resources.qrc
)

target_link_libraries(MPVDeck PRIVATE Qt6::Widgets MPVDeckViewModels MPVDeckViews MPVDeckUtils)
target_compile_features(MPVDeck PRIVATE cxx_std_20)

install(TARGETS MPVDeck DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/org.mpvdeck.MPVDeck.desktop DESTINATION share/applications)
install(FILES ${CMAKE_SOURCE_DIR}/resources/mcog.png DESTINATION share/icons/hicolor/512x512/apps RENAME org.mpvdeck.MPVDeck.png)
install(FILES ${CMAKE_SOURCE_DIR}/org.mpvdeck.MPVDeck.metainfo.xml DESTINATION share/metainfo)