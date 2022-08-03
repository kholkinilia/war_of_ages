project(war_of_ages_server VERSION 1.0 LANGUAGES CXX)

# ==== configuring version

configure_file(configs/woa_server_version.h.in woa_server_version.h)

# ==== setting sources

set(SERVER_PROJECT_SOURCES
        src/main.cpp
        src/server.cpp
        src/random_matchmaker.cpp
        src/room_matchmaker.cpp
        src/game.cpp
        src/game_handler.cpp
        src/tournament_handler.cpp
        src/server_tournament.cpp
        src/database_handler.cpp
        src/chat_handler.cpp
        src/chat.cpp
        src/server_unit.cpp
        src/server_bullet.cpp
        src/server_cannon.cpp
        )

# adding executable and targeting everything

add_executable(war_of_ages_server ${SERVER_PROJECT_SOURCES})
target_link_libraries(war_of_ages_server common ${NETWORKING_LIBS} pthread sqlite3)
target_include_directories(war_of_ages_server PUBLIC "${PROJECT_BINARY_DIR}" "include")
