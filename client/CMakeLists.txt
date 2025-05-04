# Server executable
add_executable(client
    src/client.cpp
)

# Link shared library + WSA (Windows only)
target_link_libraries(client PRIVATE shared_lib)

if(WIN32)
    target_link_libraries(client PRIVATE ws2_32)
endif()