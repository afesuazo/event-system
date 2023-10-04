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

class SpecificEventLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    template<typename TEvent>
    void PublicAddHandler(const std::shared_ptr<IEventHandler<TEvent>>& event_handler) {
        AddEventHandler(event_handler);
    }

    template <typename TEvent>
    void PublicTriggerEvent(const TEvent& event) {
        TriggerEvent(event);
    }

    bool IsAllowedEvent(const BaseEvent& event) override {
        // By default, all event types are allowed
        return typeid(event) != typeid(GeneralEvent);
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

    std::shared_ptr<IEventHandler<SpecificEvent>>
            specific_event_handler = std::make_shared<TestEventHandler<SpecificEvent>>();

    event_layer.PublicAddHandler(specific_event_handler);

    SpecificEvent specific_event{};
    event_layer.PublicTriggerEvent(specific_event);

    auto casted_event_handler = std::static_pointer_cast<TestEventHandler<SpecificEvent>>(specific_event_handler);
    EXPECT_TRUE(casted_event_handler->event_triggered);
}

TEST(EventLayerTest, InvalidEventEmissionTest) {
    SpecificEventLayer event_layer{};

    std::shared_ptr<IEventHandler<GeneralEvent>>
            general_event_handler = std::make_shared<TestEventHandler<GeneralEvent>>();

    event_layer.PublicAddHandler(general_event_handler);

    GeneralEvent general_event{};
    event_layer.PublicTriggerEvent(general_event);

    auto casted_event_handler = std::static_pointer_cast<TestEventHandler<GeneralEvent>>(general_event_handler);
    EXPECT_FALSE(casted_event_handler->event_triggered);
}