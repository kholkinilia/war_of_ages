cmake_minimum_required(VERSION 3.10)
project(war_of_ages CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(TGUI 0.10 COMPONENTS tgui)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
add_definitions(-DSFML_STATIC)

set(NETWORKING_LIBS)
if (WIN32 AND NOT MSVC AND NOT BORLAND)
    # Boost::Asio automatically links with necessary
    # libraries when using Visual C++ or Borland C++,
    # see BOOST_ASIO_NO_DEFAULT_LINKED_LIBS.
    set(NETWORKING_LIBS "ws2_32")
endif ()

set(BUILD_CLIENT True)
set(BUILD_SERVER False)

set(COMMON_PROJECT_SOURCES
        game_logic/src/game_state.cpp
        game_logic/src/player.cpp
        game_logic/src/unit.cpp
        game_logic/src/bullet.cpp
        game_logic/src/cannon.cpp
        game_logic/src/game_commands.cpp
        game_logic/src/vec2f.cpp)

if (BUILD_CLIENT)
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
    add_executable(war_of_ages ${CLIENT_PROJECT_SOURCES} ${COMMON_PROJECT_SOURCES})
    target_link_libraries(war_of_ages sfml-system sfml-window sfml-graphics sfml-audio)
    target_link_libraries(war_of_ages tgui)
endif ()

if (BUILD_SERVER)
    set(SERVER_PROJECT_SOURCES
            server/src/main.cpp
            server/src/tournament.cpp)
    add_executable(war_of_ages_server ${SERVER_PROJECT_SOURCES} ${COMMON_PROJECT_SOURCES})
    target_link_libraries(war_of_ages_server ${NETWORKING_LIBS})
endif ()
