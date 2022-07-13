cmake_minimum_required(VERSION 3.10)
project(war_of_ages CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(NETWORKING_LIBS)
if (WIN32 AND NOT MSVC AND NOT BORLAND)
    # Boost::Asio automatically links with necessary
    # libraries when using Visual C++ or Borland C++,
    # see BOOST_ASIO_NO_DEFAULT_LINKED_LIBS.
    set(NETWORKING_LIBS "ws2_32" "wsock32")
endif ()

add_subdirectory(common)

option(BUILD_CLIENT "Build the client" ON)
option(BUILD_SERVER "Build the server" OFF)

if (BUILD_CLIENT)
add_subdirectory(client)
endif()
if (BUILD_SERVER)
add_subdirectory(server)
endif()
