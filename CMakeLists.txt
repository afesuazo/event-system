cmake_minimum_required(VERSION 3.26)
project(event_system)

set(CMAKE_CXX_STANDARD 20)

set(EVENT_SYSTEM_SOURCES
        src/event_manager.cpp
        src/event_layer.cpp
)

add_library(event_system_lib ${EVENT_SYSTEM_SOURCES})

target_include_directories(event_system_lib
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(EVENT_SYSTEM_EXAMPLES
        examples/no_layer.cpp
        examples/multiple_layers.cpp
)

add_executable(event_system ${EVENT_SYSTEM_EXAMPLES})

target_include_directories(event_system_lib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(event_system
        PRIVATE event_system_lib
)

target_include_directories(event_system
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)