set(EXAMPLES_SOURCES example.cpp)

add_executable(examples ${EXAMPLES_SOURCES})

target_link_libraries(examples PRIVATE event_system)
