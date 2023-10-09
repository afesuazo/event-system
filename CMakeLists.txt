cmake_minimum_required(VERSION 3.26)
project(event_system)

set(CMAKE_CXX_STANDARD 20)

set(EVENT_SYSTEM_SOURCES
        src/event_manager.cpp
        src/event_layer.cpp
)

add_library(event_system ${EVENT_SYSTEM_SOURCES})

target_include_directories(event_system
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Separate executables for the examples
add_executable(no_layer_example examples/no_layer.cpp)
target_link_libraries(no_layer_example PRIVATE event_system)

add_executable(multiple_layers_example examples/multiple_layers.cpp)
target_link_libraries(multiple_layers_example PRIVATE event_system)

enable_testing()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)