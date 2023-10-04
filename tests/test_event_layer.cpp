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
#include <utility>

using namespace event_system;

class AddEventLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void Run() override {
        std::cout << "Sample layer running\n";

        std::shared_ptr<IEventHandler<TestGeneralEvent>>
                general_handler = std::make_shared<TestEventHandler<TestGeneralEvent>>();

        AddEventHandler(general_handler);
    }
};

class RemoveEventLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void Run() override {
        std::cout << "Sample layer running\n";

        std::shared_ptr<IEventHandler<TestGeneralEvent>>
                general_handler = std::make_shared<TestEventHandler<TestGeneralEvent>>();

        AddEventHandler(general_handler);
        RemoveEventHandler(general_handler);
    }
};

class SpecificEventLayer : public EventLayer {
public:

    explicit SpecificEventLayer(std::string layer_name = "") : EventLayer(std::move(layer_name)) {
        set_allowed_events(SpecificEvent);
    }

    template<typename TEvent>
    void PublicAddHandler(const std::shared_ptr<IEventHandler<TEvent>>& event_handler) {
        AddEventHandler(event_handler);
    }

    template <typename TEvent>
    void PublicTriggerEvent(const TEvent& event) {
        TriggerEvent(event);
    }

    void Run() override {}
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

TEST(EventLayerTest, ValidEventEmissionTest) {
    SpecificEventLayer event_layer{};

    std::shared_ptr<IEventHandler<TestSpecificEvent>>
            specific_event_handler = std::make_shared<TestEventHandler<TestSpecificEvent>>();

    event_layer.PublicAddHandler(specific_event_handler);

    TestSpecificEvent specific_event{};
    event_layer.PublicTriggerEvent(specific_event);

    auto casted_event_handler = std::static_pointer_cast<TestEventHandler<TestSpecificEvent>>(specific_event_handler);
    EXPECT_TRUE(casted_event_handler->event_triggered);
}

TEST(EventLayerTest, InvalidEventEmissionTest) {
    SpecificEventLayer event_layer{};

    std::shared_ptr<IEventHandler<TestGeneralEvent>>
            general_event_handler = std::make_shared<TestEventHandler<TestGeneralEvent>>();

    event_layer.PublicAddHandler(general_event_handler);

    TestGeneralEvent general_event{};
    event_layer.PublicTriggerEvent(general_event);

    auto casted_event_handler = std::static_pointer_cast<TestEventHandler<TestGeneralEvent>>(general_event_handler);
    EXPECT_FALSE(casted_event_handler->event_triggered);
}