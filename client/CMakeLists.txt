project(war_of_ages_client VERSION 1.0 LANGUAGES CXX)

# ==== version configuration

configure_file(configs/woa_client_version.h.in woa_client_version.h)

# ==== searching for packages

find_package(TGUI 0.10 COMPONENTS tgui)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
add_definitions(-DSFML_STATIC)

# ==== setting sources and libs

set(CLIENT_PROJECT_SOURCES
        src/main.cpp
        src/screens/tournament_screens.cpp
        src/screens/room_screen.cpp
        src/client.cpp
        src/screens/start_screen.cpp
        src/screens/settings_screen.cpp
        src/screens/multiplayer_screen.cpp
        src/screens/game_screen.cpp
        src/screens/end_game_screen.cpp
        src/screens/wait_screen.cpp
        src/screens/statistics_screen.cpp
        src/screens/login_screen.cpp
        src/sprite_supplier.cpp
        src/sfml_printer.cpp
        src/bot_actions_supplier.cpp
        src/player_actions_supplier.cpp
        src/single_player_handler.cpp
        src/multiplayer_snapshots_handler.cpp
        src/sound_player.cpp
        src/animation_supplier.cpp
        src/screen_handler.cpp
        src/application.cpp
        src/tournament_handler.cpp
        src/room_handler.cpp
        src/chat_handler.cpp
        src/screens/wait_for_server_screen.cpp
        src/background_handler.cpp
        src/client_unit.cpp src/client_bullet.cpp src/client_cannon.cpp)

set(CLIENT_LIBS
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        tgui
        )

# adding executable and targeting everything

add_executable(war_of_ages_client ${CLIENT_PROJECT_SOURCES})
target_link_libraries(war_of_ages_client ${CLIENT_LIBS} ${NETWORKING_LIBS} common)
target_include_directories(war_of_ages_client PUBLIC "${PROJECT_BINARY_DIR}" "include")
