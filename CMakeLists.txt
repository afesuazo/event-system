cmake_minimum_required(VERSION 3.26)
project(event_system
        VERSION 0.1
        DESCRIPTION "A header-only event system for simple communication between objects"
        HOMEPAGE_URL "https://github.com/afesuazo/event_system")

set(CMAKE_CXX_STANDARD 20)

option(ENABLE_TESTS "Enable tests" ON)
if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

add_subdirectory(lib)
add_subdirectory(examples)