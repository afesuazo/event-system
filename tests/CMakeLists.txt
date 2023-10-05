find_package(GTest REQUIRED)

set(TEST_SOURCES
        test_event_manager.cpp
        test_event_handler.cpp
        test_event_layer.cpp
)

add_executable(event_system_tests ${TEST_SOURCES})

target_link_libraries(event_system_tests
        PRIVATE event_system_lib
        GTest::GTest
        GTest::Main
)
target_include_directories(event_system_tests
        PRIVATE ${CMAKE_SOURCE_DIR}/include
)