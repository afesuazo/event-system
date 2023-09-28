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
                generalListener = std::make_shared<EventListener<GeneralEvent>>();

        AddListener(generalListener);
    }
};

class RemoveListenerLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void Run() override {
        std::cout << "Sample layer running\n";

        std::shared_ptr<EventListener<GeneralEvent>>
                generalListener = std::make_shared<EventListener<GeneralEvent>>();

        AddListener(generalListener);
        RemoveListener(generalListener);
    }
};

TEST(EventLayerTest, AddedListenerTest) {
    AddListenerLayer eventLayer{};

    // Start Run in a separate thread
    std::thread runThread(&EventLayer::Run, &eventLayer);
    // Give time to set listeners
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(eventLayer.get_listener_count(), 1);

    eventLayer.Stop();
    runThread.join();
}

TEST(EventLayerTest, RemovedListenerTest) {
    RemoveListenerLayer eventLayer{};

    // Start Run in a separate thread
    std::thread runThread(&EventLayer::Run, &eventLayer);
    // Give time to set listeners
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(eventLayer.get_listener_count(), 0);

    eventLayer.Stop();
    runThread.join();
}