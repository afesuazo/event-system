cmake_minimum_required(VERSION 3.26)
project(event_manager)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(EVENT_MANAGER_SOURCES
        src/main.cpp
)

add_executable(event_manager ${EVENT_MANAGER_SOURCES})

#enable_testing()
#add_subdirectory(${CMAKE_SOURCE_DIR}/tests)