cmake_minimum_required(VERSION 3.26)
project(event_system)

set(CMAKE_CXX_STANDARD 20)

add_library(event_system INTERFACE)

target_include_directories(event_system
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(event_system_example examples/example.cpp)
target_link_libraries(event_system_example PRIVATE event_system)

enable_testing()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)