//
// Created by Andres Suazo
//

#include "gtest/gtest.h"
#include "event_manager.h"
#include "base_event.h"
#include "event_listener.h"
#include "testing_utils.h"
#include <memory>

using namespace event_manager;

class EventManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<EventManager> eventManager;
    std::shared_ptr<IEventListener<GeneralEvent>> generalEventListener;

    void SetUp() override {
        eventManager = std::make_shared<EventManager>();
        generalEventListener = std::make_shared<GeneralEventListener>();
    }
};

TEST_F(EventManagerTest, AddSubscriberTest) {
    eventManager->AddSubscriber(generalEventListener);
    ASSERT_TRUE(eventManager->SubscriptionExists(generalEventListener));
}

TEST_F(EventManagerTest, AddDuplicateSubscriberTest) {
    ASSERT_EQ(eventManager->GetSubscriberCount(), 0);
    eventManager->AddSubscriber(generalEventListener);
    ASSERT_EQ(eventManager->GetSubscriberCount(), 1);
}

TEST_F(EventManagerTest, MultipleSubscriberTest) {
    eventManager->AddSubscriber(generalEventListener);
    std::shared_ptr<IEventListener<GeneralEvent>> generalListener2 = std::make_shared<GeneralEventListener>();
    eventManager->AddSubscriber(generalListener2);

    ASSERT_EQ(eventManager->GetSubscriberCount(), 2);
}

TEST_F(EventManagerTest, RemoveSubscriberTest) {
    eventManager->AddSubscriber(generalEventListener);
    ASSERT_EQ(eventManager->GetSubscriberCount(), 1);
    eventManager->RemoveSubscriber<GeneralEvent>(generalEventListener);
    ASSERT_EQ(eventManager->GetSubscriberCount(), 0);
}

TEST_F(EventManagerTest, RemoveSubscriberFromEmptyMapTest) {
    ASSERT_NO_THROW(eventManager->RemoveSubscriber<GeneralEvent>(generalEventListener));
}

TEST_F(EventManagerTest, TriggerEventTest) {
    eventManager->AddSubscriber(generalEventListener);
    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};

    auto castedListener = std::static_pointer_cast<GeneralEventListener>(generalEventListener);

    EXPECT_FALSE(castedListener->eventTriggered);
    eventManager->EmitEvent(generalEvent);
    EXPECT_TRUE(castedListener->eventTriggered);
}

TEST_F(EventManagerTest, TriggerEventMultipleSubscriberTest) {
    eventManager->AddSubscriber(generalEventListener);
    std::shared_ptr<IEventListener<SpecificEvent>> specificListener = std::make_shared<SpecificEventListener>();
    eventManager->AddSubscriber(specificListener);

    auto castedListener = std::static_pointer_cast<GeneralEventListener>(generalEventListener);
    auto castedListener2 = std::static_pointer_cast<SpecificEventListener>(specificListener);

    EXPECT_FALSE(castedListener->eventTriggered);
    EXPECT_FALSE(castedListener2->eventTriggered);

    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};
    eventManager->EmitEvent(generalEvent);
    EXPECT_TRUE(castedListener->eventTriggered);
    EXPECT_FALSE(castedListener2->eventTriggered);
}

TEST_F(EventManagerTest, DanglingPointerTest) {
    eventManager->AddSubscriber(generalEventListener);
    generalEventListener.reset();

    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};
    eventManager->EmitEvent(generalEvent);

    ASSERT_EQ(eventManager->GetSubscriberCount<GeneralEvent>(), 0);
}