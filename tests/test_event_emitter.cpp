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
    std::shared_ptr<EventManager> eventManager;
    TestEventEmitter eventEmitter;

    void SetUp() override {
        eventManager = std::make_shared<TestEventManager>();
        eventEmitter = TestEventEmitter{eventManager};
    }
};

TEST_F(EventEmitterTest, ValidEventEmissionTest) {
    std::shared_ptr<EventListener<GeneralEvent>> generalEventListener = std::make_shared<TestEventListener<GeneralEvent>>();
    eventManager->AddSubscriber(generalEventListener);

    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};
    eventEmitter.Emit(generalEvent, true);

    auto castedListener = std::static_pointer_cast<TestEventListener<GeneralEvent>>(generalEventListener);
    EXPECT_TRUE(castedListener->eventTriggered);
}

TEST_F(EventEmitterTest, InvalidEventEmissionTest) {
    std::shared_ptr<EventListener<GeneralEvent>> generalEventListener = std::make_shared<TestEventListener<GeneralEvent>>();
    eventManager->AddSubscriber(generalEventListener);

    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};
    SpecificOnlyEventEmitter specificEventEmitter{eventManager};

    specificEventEmitter.Emit(generalEvent);

    auto castedListener = std::static_pointer_cast<TestEventListener<GeneralEvent>>(generalEventListener);
    EXPECT_FALSE(castedListener->eventTriggered);
}