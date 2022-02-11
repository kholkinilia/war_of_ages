cmake_minimum_required(VERSION 3.20)
project(war_of_ages CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
add_definitions(-DSFML_STATIC)

include_directories(client/ext/include)
link_directories(client/ext/lib)

set(PROJECT_SOURCES
        client/src/main.cpp
        client/src/tournament_screens.cpp
        client/src/ui_functions.cpp client/include/screens.h)

add_executable(war_of_ages ${PROJECT_SOURCES})

# for boost::asio
if(MINGW)
    target_link_libraries(war_of_ages ws2_32)
endif()

target_link_libraries(war_of_ages sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(war_of_ages tgui-d)