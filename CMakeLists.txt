cmake_minimum_required(VERSION 3.26)
project(event_system)

set(CMAKE_CXX_STANDARD 20)

add_library(event_system_lib
        src/event_manager.cpp
        src/event_layer.cpp
        src/event_layer_manager.cpp
)

target_include_directories(event_system_lib
        PRIVATE ${CMAKE_SOURCE_DIR}/include
)

set(EVENT_SYSTEM_EXAMPLES
        examples/single_layer.cpp
        examples/multiple_layers.cpp
)

add_executable(event_system ${EVENT_SYSTEM_EXAMPLES})

target_link_libraries(event_system
        PRIVATE event_system_lib
)

target_include_directories(event_system
        PRIVATE ${CMAKE_SOURCE_DIR}/include
)

enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)