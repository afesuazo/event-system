cmake_minimum_required(VERSION 3.26)
project(event_system
        VERSION 1.0.0
        DESCRIPTION "A simple and fast event system for internal communication"
        HOMEPAGE_URL "https://github.com/afesuazo/event_system")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_TESTS "Enable tests" ON)
if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

option(ENABLE_PERFORMANCE_TESTS "Enable performance metrics" OFF)
if (ENABLE_PERFORMANCE_TESTS)
    add_subdirectory(performance)
endif ()

add_subdirectory(lib)
add_subdirectory(examples)