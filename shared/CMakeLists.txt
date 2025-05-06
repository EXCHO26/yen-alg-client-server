# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Shared library (Graph.cpp/hpp)
add_library(shared_lib
    src/Graph.cpp
    src/Graph.hpp
    src/ThreadPool.cpp
    src/ThreadPool.hpp
)

# Make headers available to parent directory
target_include_directories(shared_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)