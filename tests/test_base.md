//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include <memory>
#include "event_layer.h"
#include "event_manager.h"
#include "base_event.h"
#include "testing_utils.h"

using namespace event_manager;

class SampleLayer : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void EmitEvent(BaseEvent &event) override {
        if (auto sharedManager = eventManager.lock()) {
            std::cout << "Emitting Local Event\n";
            std::cout << "Subscribers: " << sharedManager->GetSubscriberCount() << "\n";
            auto generalEvent = dynamic_cast<GeneralEvent*>(&event);
            sharedManager->EmitEvent(*generalEvent);
        }
        std::cout << "Sample layer on event called: " << event.GetType().name() << "\n";
    }
};

class EventLayerTest : public ::testing::Test {
protected:
std::shared_ptr<EventManager> eventManager;
std::shared_ptr<SampleLayer> eventLayer;

    void SetUp() override {
        eventManager = std::make_shared<EventManager>();
        eventLayer = std::make_shared<SampleLayer>(eventManager);
    }
};


TEST_F(EventLayerTest, RegisterListenerTest) {
std::shared_ptr<IEventListener<GeneralEvent>> generalEventListener = std::make_shared<GeneralEventListener>();
eventLayer->AddListener(generalEventListener);

    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};
    eventManager->EmitEvent(generalEvent);

    auto castedListener = std::static_pointer_cast<GeneralEventListener>(generalEventListener);
    EXPECT_TRUE(castedListener->eventTriggered);
}

TEST_F(EventLayerTest, UnregisterListenerTest) {
std::shared_ptr<IEventListener<GeneralEvent>> generalEventListener = std::make_shared<GeneralEventListener>();
eventLayer->AddListener(generalEventListener);
eventLayer->RemoveListener(generalEventListener);

    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};
    eventManager->EmitEvent(generalEvent);

    auto castedListener = std::static_pointer_cast<GeneralEventListener>(generalEventListener);
    EXPECT_FALSE(castedListener->eventTriggered);
}

TEST_F(EventLayerTest, OnEventTriggerTest) {
std::shared_ptr<IEventListener<GeneralEvent>> generalEventListener = std::make_shared<GeneralEventListener>();
eventLayer->AddListener(generalEventListener);

    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};
    eventLayer->EmitEvent(generalEvent);

    auto castedListener = std::static_pointer_cast<GeneralEventListener>(generalEventListener);
    EXPECT_TRUE(castedListener->eventTriggered);
}
