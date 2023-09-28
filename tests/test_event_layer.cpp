//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include <memory>
#include "event_layer.h"
#include "event_manager.h"
#include "testing_utils.h"
#include <thread>
#include <chrono>

using namespace event_system;

class AddListenerLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void Run() override {
        std::cout << "Sample layer running\n";

        std::shared_ptr<EventListener<GeneralEvent>>
                general_listener = std::make_shared<EventListener<GeneralEvent>>();

        AddListener(general_listener);
    }
};

class RemoveListenerLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void Run() override {
        std::cout << "Sample layer running\n";

        std::shared_ptr<EventListener<GeneralEvent>>
                general_listener = std::make_shared<EventListener<GeneralEvent>>();

        AddListener(general_listener);
        RemoveListener(general_listener);
    }
};

TEST(EventLayerTest, AddedListenerTest) {
    AddListenerLayer event_layer{};

    // Start Run in a separate thread
    std::thread run_thread(&EventLayer::Run, &event_layer);
    // Give time to set listeners
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(event_layer.get_listener_count(), 1);

    event_layer.Stop();
    run_thread.join();
}

TEST(EventLayerTest, RemovedListenerTest) {
    RemoveListenerLayer event_layer{};

    // Start Run in a separate thread
    std::thread run_thread(&EventLayer::Run, &event_layer);
    // Give time to set listeners
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(event_layer.get_listener_count(), 0);

    event_layer.Stop();
    run_thread.join();
}