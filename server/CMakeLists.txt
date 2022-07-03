project(war_of_ages_server CXX)

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
        )
add_executable(war_of_ages_server ${SERVER_PROJECT_SOURCES})
target_link_libraries(war_of_ages_server common ${NETWORKING_LIBS} pthread sqlite3)
