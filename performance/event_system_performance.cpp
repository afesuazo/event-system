//
// Created by Andres Suazo
//

#include <benchmark/benchmark.h>

#include "event_handler.h"

using namespace event_system;

static void EventHandler_DispatchEventsToCallbacks(benchmark::State &state) {
  EventHandler<int> handler;

  const size_t callback_amount = state.range(0);
  const size_t event_amount = state.range(1);

  // Add basic callback/s
  for (int i = 0; i < callback_amount; ++i) {
    handler.AddCallback([](int value) {});
  }

  // Dispatch event/s
  for (auto _ : state) {
    for (size_t j = 0; j < event_amount; ++j) {
      handler.OnEvent(1);
    }
  }
}

// Register the benchmark for various numbers of listeners and event emissions
BENCHMARK(EventHandler_DispatchEventsToCallbacks)
    ->Args({1, 1})
    ->Args({1, 10})
    ->Args({1, 100})
    ->Args({1, 1000});

BENCHMARK_MAIN();
