cmake_minimum_required(VERSION 3.5)

project(path-tracer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    #src/sphere.cpp
)

# Add header files
set(HEADERS
    include/vec3.hpp
    include/color.hpp    
    include/ray.hpp
    include/utils.hpp
    include/hittable.hpp
    include/hittable-list.hpp
    include/sphere.hpp
)

# Add executable target
add_executable(path-tracer ${SOURCES} ${HEADERS})

# Set include directories
target_include_directories(path-tracer PRIVATE include)

# Add any necessary libraries
target_link_libraries(path-tracer PRIVATE)

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
