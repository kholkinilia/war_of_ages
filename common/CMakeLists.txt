cmake_minimum_required(VERSION 3.10)

project(common_lib CXX)

add_library(common
        tournament/source/tournament.cpp
        game_logic/src/game_state.cpp
        game_logic/src/player.cpp
        game_logic/src/unit.cpp
        game_logic/src/bullet.cpp
        game_logic/src/cannon.cpp
        game_logic/src/game_commands.cpp
        game_logic/src/vec2f.cpp
        )

target_include_directories(common PUBLIC
        include
        game_logic/include
        network/include
        tournament/include
        )
