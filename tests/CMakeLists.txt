find_package(GTest REQUIRED)

set(TEST_SOURCES
        # test source files
)

set(TEST_SOURCES ${TEST_SOURCES})

add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests event_manager GTest::GTest GTest::Main)