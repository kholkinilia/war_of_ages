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
    set(NETWORKING_LIBS "ws2_32" "wsock32")
endif ()

set(COMMON_PROJECT_SOURCES
        common/tournament/source/tournament.cpp
        common/game_logic/src/game_state.cpp
        common/game_logic/src/player.cpp
        common/game_logic/src/unit.cpp
        common/game_logic/src/bullet.cpp
        common/game_logic/src/cannon.cpp
        common/game_logic/src/game_commands.cpp
        common/game_logic/src/vec2f.cpp
        )

set(CLIENT_PROJECT_SOURCES
        client/src/main.cpp
        client/src/tournament_screens.cpp
        client/src/ui_functions.cpp
        client/src/client.cpp
        client/src/screens.cpp
        client/src/start_screen.cpp
        client/src/settings_screen.cpp
        client/src/multiplayer_screen.cpp
        client/src/game_screen.cpp
        client/src/end_game_screen.cpp
        client/src/wait_screen.cpp
        client/src/sprite_supplier.cpp
        client/src/sprite_printer.cpp
        client/src/bot_actions_receiver.cpp
        client/src/player_actions_receiver.cpp
        client/src/game_handler.cpp
        client/src/sound_player.cpp
        client/src/animation_handler.cpp
        client/src/tournament_handler.cpp
        )
add_executable(war_of_ages ${CLIENT_PROJECT_SOURCES} ${COMMON_PROJECT_SOURCES})
target_link_libraries(war_of_ages sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(war_of_ages tgui)

set(SERVER_PROJECT_SOURCES
        server/src/main.cpp
        server/src/server.cpp
        server/src/random_matchmaker.cpp
        server/src/room_matchmaker.cpp
        server/src/game.cpp
        server/src/game_handler.cpp
        server/src/tournament_handler.cpp
        server/src/server_tournament.cpp
        )
add_executable(war_of_ages_server ${SERVER_PROJECT_SOURCES} ${COMMON_PROJECT_SOURCES})
target_link_libraries(war_of_ages_server ${NETWORKING_LIBS})
