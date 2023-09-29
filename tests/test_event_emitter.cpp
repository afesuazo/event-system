//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include <memory>
#include "event_manager.h"
#include "event_emitter.h"
#include "testing_utils.h"

using namespace event_system;

class SpecificOnlyEventEmitter : public EventEmitter {
public:
    using EventEmitter::EventEmitter;

protected:
    bool IsAllowedEvent(const BaseEvent& event) override {
        return typeid(event) != typeid(GeneralEvent);
    }
};

class EventEmitterTest : public ::testing::Test {
protected:
    std::shared_ptr<EventManager> event_manager;
    TestEventEmitter event_emitter;

    void SetUp() override {
        event_manager = std::make_shared<TestEventManager>();
        event_emitter = TestEventEmitter{event_manager};
    }
};

TEST_F(EventEmitterTest, ValidEventEmissionTest) {
    std::shared_ptr<EventListener<GeneralEvent>> general_event_listener = std::make_shared<TestEventListener<GeneralEvent>>();
    event_manager->AddSubscriber(general_event_listener);

    GeneralEvent general_event{GeneralEvents::GeneralSubType0};
    event_emitter.Emit(general_event, true);

    auto casted_listener = std::static_pointer_cast<TestEventListener<GeneralEvent>>(general_event_listener);
    EXPECT_TRUE(casted_listener->event_triggered);
}

TEST_F(EventEmitterTest, InvalidEventEmissionTest) {
    std::shared_ptr<EventListener<GeneralEvent>> general_event_listener = std::make_shared<TestEventListener<GeneralEvent>>();
    event_manager->AddSubscriber(general_event_listener);

    GeneralEvent general_event{GeneralEvents::GeneralSubType0};
    SpecificOnlyEventEmitter specific_event_emitter{event_manager};

    specific_event_emitter.Emit(general_event);

    auto casted_listener = std::static_pointer_cast<TestEventListener<GeneralEvent>>(general_event_listener);
    EXPECT_FALSE(casted_listener->event_triggered);
}