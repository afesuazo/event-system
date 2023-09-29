//
// Created by Andres Suazo
//

#include "gtest/gtest.h"
#include "event_manager.h"
#include "base_event.h"
#include "testing_utils.h"
#include <memory>

using namespace event_system;

class EventManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<EventManager> local_event_manager;
    std::shared_ptr<EventListener<GeneralEvent>> general_event_listener;

    void SetUp() override {
        local_event_manager = std::make_shared<TestEventManager>();
        general_event_listener = std::make_shared<TestEventListener<GeneralEvent>>();
    }
};

TEST_F(EventManagerTest, AddSubscriberTest) {
    local_event_manager->AddSubscriber(general_event_listener);
    ASSERT_TRUE(local_event_manager->SubscriptionExists(general_event_listener));
}

TEST_F(EventManagerTest, AddDuplicateSubscriberTest) {
    ASSERT_EQ(local_event_manager->get_subscriber_count(), 0);
    local_event_manager->AddSubscriber(general_event_listener);
    ASSERT_EQ(local_event_manager->get_subscriber_count(), 1);
}

TEST_F(EventManagerTest, MultipleSubscriberTest) {
    local_event_manager->AddSubscriber(general_event_listener);
    std::shared_ptr<EventListener<GeneralEvent>> generalListener2 = std::make_shared<TestEventListener<GeneralEvent>>();
    local_event_manager->AddSubscriber(generalListener2);

    ASSERT_EQ(local_event_manager->get_subscriber_count(), 2);
    ASSERT_EQ(local_event_manager->get_subscriber_count<BaseEvent>(), 0);
}

TEST_F(EventManagerTest, RemoveSubscriberTest) {
    local_event_manager->AddSubscriber(general_event_listener);
    ASSERT_EQ(local_event_manager->get_subscriber_count(), 1);
    local_event_manager->RemoveSubscriber<GeneralEvent>(general_event_listener);
    ASSERT_EQ(local_event_manager->get_subscriber_count(), 0);
}

TEST_F(EventManagerTest, RemoveSubscriberFromEmptyMapTest) {
    ASSERT_NO_THROW(local_event_manager->RemoveSubscriber<GeneralEvent>(general_event_listener));
}

TEST_F(EventManagerTest, TriggerEventTest) {
    local_event_manager->AddSubscriber(general_event_listener);
    GeneralEvent general_event{GeneralEvents::GeneralSubType0};

    auto casted_listener = std::static_pointer_cast<TestEventListener<GeneralEvent>>(general_event_listener);
    auto casted_manager = std::static_pointer_cast<TestEventManager>(local_event_manager);

    EXPECT_FALSE(casted_listener->event_triggered);
    casted_manager->EmitEvent<GeneralEvent>(general_event, true);
    EXPECT_TRUE(casted_listener->event_triggered);
}

TEST_F(EventManagerTest, TriggerEventMultipleSubscriberTest) {
    local_event_manager->AddSubscriber(general_event_listener);
    std::shared_ptr<EventListener<SpecificEvent>> specificListener = std::make_shared<TestEventListener<SpecificEvent>>();
    local_event_manager->AddSubscriber(specificListener);

    auto castedListener = std::static_pointer_cast<TestEventListener<GeneralEvent>>(general_event_listener);
    auto castedListener2 = std::static_pointer_cast<TestEventListener<SpecificEvent>>(specificListener);

    EXPECT_FALSE(castedListener->event_triggered);
    EXPECT_FALSE(castedListener2->event_triggered);

    GeneralEvent generalEvent{GeneralEvents::GeneralSubType0};

    auto casted_manager = std::static_pointer_cast<TestEventManager>(local_event_manager);
    casted_manager->EmitEvent<GeneralEvent>(generalEvent, true);
    EXPECT_TRUE(castedListener->event_triggered);
    EXPECT_FALSE(castedListener2->event_triggered);
}

TEST_F(EventManagerTest, DanglingPointerTest) {
    local_event_manager->AddSubscriber(general_event_listener);
    general_event_listener.reset();

    GeneralEvent generalEvent{GeneralEvents::GeneralSubType0};
    auto casted_manager = std::static_pointer_cast<TestEventManager>(local_event_manager);
    casted_manager->EmitEvent<GeneralEvent>(generalEvent, true);

    ASSERT_EQ(local_event_manager->get_subscriber_count<GeneralEvent>(), 0);
}