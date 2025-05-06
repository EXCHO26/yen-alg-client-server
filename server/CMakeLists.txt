# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Server executable
add_executable(server
    src/server.cpp
)

# Link shared library + WSA (Windows only)
target_link_libraries(server PRIVATE shared_lib)

if(WIN32)
    target_link_libraries(server PRIVATE ws2_32)
endif()