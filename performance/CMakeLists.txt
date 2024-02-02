set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable Benchmark's tests" FORCE)

add_subdirectory(benchmark)
include_directories(benchmark/include)

add_executable(event_system_performance event_system_performance.cpp)
target_link_libraries(event_system_performance event_system benchmark::benchmark)