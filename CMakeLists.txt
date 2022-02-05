cmake_minimum_required(VERSION 3.21)
project(war_of_ages CXX)

set(CMAKE_CXX_STANDARD 17)

# TGUI trying to find without exact paths:

# set(CMAKE_PREFIX_PATH ./cmake_modules)
set(TGUI_DIR D:/Soft/Lib/TGUI-0.10) # this is not good 100%, needs to be replaced with a decent alternative
find_package(TGUI COMPONENTS tgui)

# include_directories(D:/Soft/Lib/TGUI-0.10/include)
# link_directories(D:/Soft/Lib/TGUI-0.10/lib)

# end TGUI

# SFML
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
add_definitions(-DSFML_STATIC)
# end SFML

set(PROJECT_SOURCES
        client/src/main.cpp client/src/tournament_page.cpp)

add_executable(war_of_ages ${PROJECT_SOURCES})

# for boost::asio
if(MINGW)
    target_link_libraries(war_of_ages ws2_32)
endif()

target_link_libraries(war_of_ages sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(war_of_ages tgui)
