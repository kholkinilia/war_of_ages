cmake_minimum_required(VERSION 3.10)
project(war_of_ages CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(TGUI 0.10 COMPONENTS tgui)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
add_definitions(-DSFML_STATIC)

set(CLIENT_PROJECT_SOURCES
        client/src/main.cpp
        client/src/tournament_screens.cpp
        client/src/ui_functions.cpp
        client/src/tournament.cpp
        client/src/client.cpp
        client/src/screens.cpp
        client/src/start_screen.cpp
        client/src/settings_screen.cpp
        client/src/multiplayer_screen.cpp)

set(SERVER_PROJECT_SOURCES
        server/src/main.cpp
        server/src/tournament.cpp
        server/src/game_state.cpp
        server/src/player.cpp
        server/src/unit.cpp
        server/src/bullet.cpp server/src/cannon.cpp)

add_executable(war_of_ages ${CLIENT_PROJECT_SOURCES})
add_executable(war_of_ages_server ${SERVER_PROJECT_SOURCES})

set(NETWORKING_LIBS)
if (WIN32 AND NOT MSVC AND NOT BORLAND)
    # Boost::Asio automatically links with necessary
    # libraries when using Visual C++ or Borland C++,
    # see BOOST_ASIO_NO_DEFAULT_LINKED_LIBS.
    set(NETWORKING_LIBS "ws2_32")
endif ()

target_link_libraries(war_of_ages sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(war_of_ages tgui)
target_link_libraries(war_of_ages ${NETWORKING_LIBS})
