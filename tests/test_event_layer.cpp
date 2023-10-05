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

class TestExternalManager {
public:
    bool event_triggered = false;

    void OnEvent(const event_system::BaseEvent& event) {
        event_triggered = true;
    }

};

class SpecificEventLayer : public EventLayer, public IEventHandler<TestSpecificEvent> {
public:

    explicit SpecificEventLayer(std::string layer_name = "") : EventLayer(std::move(layer_name)) {
        set_allowed_events(SpecificEvent);
    }

    void HandleEvent(const TestSpecificEvent& event) override {
        std::cout << "Specific event handled in layer: " << event.get_sender_id() << "\n";
    }

    template <typename TEvent>
    void PublicTriggerEvent(const TEvent& event) {
        EmitEvent(event);
    }

    void Run() override {}
};


class EventLayerTest : public ::testing::Test {
protected:
    TestExternalManager test_external_manager;
    SpecificEventLayer event_layer;

    void SetUp() override {
        event_layer.set_layer_manager_callback([this](const BaseEvent& event){
            test_external_manager.OnEvent(event);
        });
    }
};


TEST_F(EventLayerTest, ValidEventEmissionTest) {
    TestSpecificEvent specific_event{};
    event_layer.PublicTriggerEvent(specific_event);

    EXPECT_TRUE(test_external_manager.event_triggered);
}

TEST_F(EventLayerTest, InvalidEventEmissionTest) {
    TestGeneralEvent general_event{};
    event_layer.PublicTriggerEvent(general_event);

    EXPECT_FALSE(test_external_manager.event_triggered);
}