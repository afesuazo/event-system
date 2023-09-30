//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include <memory>
#include "event_manager.h"
#include "event_emitter.h"
#include "testing_utils.h"
#include "event_handler.h"

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
    EventEmitter event_emitter;

    void SetUp() override {
        event_manager = std::make_shared<EventManager>();
        event_emitter = EventEmitter{event_manager};
    }
};

TEST_F(EventEmitterTest, ValidEventEmissionTest) {
    std::shared_ptr<IEventHandler<GeneralEvent>> general_event_handler = std::make_shared<TestEventHandler<GeneralEvent>>();
    event_manager->AddSubscriber(general_event_handler);

    GeneralEvent general_event{GeneralEvents::GeneralSubType0};
    event_emitter.Emit(general_event);

    auto casted_handler = std::static_pointer_cast<TestEventHandler<GeneralEvent>>(general_event_handler);
    EXPECT_TRUE(casted_handler->event_triggered);
}

TEST_F(EventEmitterTest, InvalidEventEmissionTest) {
    std::shared_ptr<IEventHandler<GeneralEvent>> general_event_handler = std::make_shared<TestEventHandler<GeneralEvent>>();
    event_manager->AddSubscriber(general_event_handler);

    GeneralEvent general_event{GeneralEvents::GeneralSubType0};
    SpecificOnlyEventEmitter specific_event_emitter{event_manager};

    specific_event_emitter.Emit(general_event);

    auto casted_handler = std::static_pointer_cast<TestEventHandler<GeneralEvent>>(general_event_handler);
    EXPECT_FALSE(casted_handler->event_triggered);
}