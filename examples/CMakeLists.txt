set(EXAMPLES_SOURCES logging.cpp)

add_executable(examples ${EXAMPLES_SOURCES})

target_link_libraries(examples PRIVATE event_system)
