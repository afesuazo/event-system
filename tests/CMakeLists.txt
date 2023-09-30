find_package(GTest REQUIRED)

set(TEST_SOURCES
        test_event_manager.cpp
        test_event_emitter.cpp
        test_event_handler.cpp
        test_event_layer.cpp
)

add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests GTest::GTest GTest::Main)