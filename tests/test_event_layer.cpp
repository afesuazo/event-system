//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include <memory>
#include "event_layer.h"
#include "event_handler.h"
#include "testing_utils.h"
#include <thread>
#include <chrono>

using namespace event_system;

class AddEventLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void Run() override {
        std::cout << "Sample layer running\n";

        std::shared_ptr<IEventHandler<GeneralEvent>>
                general_handler = std::make_shared<TestEventHandler<GeneralEvent>>();

        AddEventHandler(general_handler);
    }
};

class RemoveEventLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void Run() override {
        std::cout << "Sample layer running\n";

        std::shared_ptr<IEventHandler<GeneralEvent>>
                general_handler = std::make_shared<TestEventHandler<GeneralEvent>>();

        AddEventHandler(general_handler);
        RemoveEventHandler(general_handler);
    }
};

TEST(EventLayerTest, AddedHandlerTest) {
    AddEventLayer event_layer{};

    // Start Run in a separate thread
    std::thread run_thread(&EventLayer::Run, &event_layer);
    // Give time to set handlers
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(event_layer.get_handler_count(), 1);

    event_layer.Stop();
    run_thread.join();
}

TEST(EventLayerTest, RemovedHandlerTest) {
    RemoveEventLayer event_layer{};

    // Start Run in a separate thread
    std::thread run_thread(&EventLayer::Run, &event_layer);
    // Give time to set handlers
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(event_layer.get_handler_count(), 0);

    event_layer.Stop();
    run_thread.join();
}