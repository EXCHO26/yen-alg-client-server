cmake_minimum_required(VERSION 3.15)
project(NetworkApp VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific: Link against Winsock2 (WSA)
if(WIN32)
    set(WSA_LIBRARIES ws2_32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

# Add subdirectories
add_subdirectory(shared)
add_subdirectory(server)
add_subdirectory(client)