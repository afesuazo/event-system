find_package(GTest REQUIRED)

set(HANDLER_TEST_NAME "event_handler.test")
set(HANDLER_TEST_SOURCES
        test_event_handler.cpp
)

add_executable(${HANDLER_TEST_NAME} ${HANDLER_TEST_SOURCES})

set(DISPATCHER_TEST_NAME "event_dispatcher.test")
set(DISPATCHER_TEST_SOURCES
        test_event_dispatcher.cpp
)

add_executable(${DISPATCHER_TEST_NAME} ${DISPATCHER_TEST_SOURCES})

target_link_libraries(${HANDLER_TEST_NAME}
        PRIVATE
        event_system
        GTest::GTest
        GTest::Main
)

target_link_libraries(${DISPATCHER_TEST_NAME}
        PRIVATE
        event_system
        GTest::GTest
        GTest::Main
)

add_test(NAME ${HANDLER_TEST_NAME} COMMAND ${HANDLER_TEST_NAME})
add_test(NAME ${DISPATCHER_TEST_NAME} COMMAND ${DISPATCHER_TEST_NAME})