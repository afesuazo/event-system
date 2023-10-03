cmake_minimum_required(VERSION 3.26)
project(event_system)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(EVENT_MANAGER_SOURCES
        src/single_layer.cpp
        src/multiple_layers.cpp
)

add_executable(event_system ${EVENT_MANAGER_SOURCES})

enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)