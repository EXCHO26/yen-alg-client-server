# Shared library (Graph.cpp/hpp)
add_library(shared_lib
    src/Graph.cpp
    src/Graph.hpp
)

# Make headers available to parent directory
target_include_directories(shared_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)