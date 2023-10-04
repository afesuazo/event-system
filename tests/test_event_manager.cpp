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
    std::shared_ptr<IEventHandler<GeneralEvent>> general_event_handler;

    void SetUp() override {
        local_event_manager = std::make_shared<EventManager>();
        general_event_handler = std::make_shared<TestEventHandler<GeneralEvent>>();
    }
};

TEST_F(EventManagerTest, AddSubscriberTest) {
    local_event_manager->AddHandler(general_event_handler);
    ASSERT_TRUE(local_event_manager->HandlerExists(general_event_handler));
}

TEST_F(EventManagerTest, AddDuplicateSubscriberTest) {
    ASSERT_EQ(local_event_manager->get_handler_count(), 0);
    local_event_manager->AddHandler(general_event_handler);
    ASSERT_EQ(local_event_manager->get_handler_count(), 1);
}

TEST_F(EventManagerTest, MultipleSubscriberTest) {
    local_event_manager->AddHandler(general_event_handler);
    std::shared_ptr<IEventHandler<GeneralEvent>> general_handler_2 = std::make_shared<TestEventHandler<GeneralEvent>>();
    local_event_manager->AddHandler(general_handler_2);

    ASSERT_EQ(local_event_manager->get_handler_count(), 2);
    ASSERT_EQ(local_event_manager->get_handler_count(EventType::SpecificEvent), 0);
}

TEST_F(EventManagerTest, RemoveSubscriberTest) {
    local_event_manager->AddHandler(general_event_handler);
    ASSERT_EQ(local_event_manager->get_handler_count(), 1);
    local_event_manager->RemoveHandler(general_event_handler);
    ASSERT_EQ(local_event_manager->get_handler_count(), 0);
}

TEST_F(EventManagerTest, RemoveSubscriberFromEmptyMapTest) {
    ASSERT_NO_THROW(local_event_manager->RemoveHandler(general_event_handler));
}

TEST_F(EventManagerTest, TriggerEventTest) {
    local_event_manager->AddHandler(general_event_handler);
    GeneralEvent general_event{};

    auto casted_handler = std::static_pointer_cast<TestEventHandler<GeneralEvent>>(general_event_handler);

    EXPECT_FALSE(casted_handler->event_triggered);
    local_event_manager->EmitEvent(general_event);
    EXPECT_TRUE(casted_handler->event_triggered);
}

TEST_F(EventManagerTest, TriggerEventMultipleSubscriberTest) {
    local_event_manager->AddHandler(general_event_handler);
    std::shared_ptr<IEventHandler<SpecificEvent>> specific_event_handler = std::make_shared<TestEventHandler<SpecificEvent>>();
    local_event_manager->AddHandler(specific_event_handler);

    auto casted_handler = std::static_pointer_cast<TestEventHandler<GeneralEvent>>(general_event_handler);
    auto casted_handler_2 = std::static_pointer_cast<TestEventHandler<SpecificEvent>>(specific_event_handler);

    EXPECT_FALSE(casted_handler->event_triggered);
    EXPECT_FALSE(casted_handler_2->event_triggered);

    GeneralEvent generalEvent{};

    local_event_manager->EmitEvent(generalEvent);
    EXPECT_TRUE(casted_handler->event_triggered);
    EXPECT_FALSE(casted_handler_2->event_triggered);
}

TEST_F(EventManagerTest, DanglingPointerTest) {
    local_event_manager->AddHandler(general_event_handler);
    general_event_handler.reset();

    GeneralEvent generalEvent{};
    local_event_manager->EmitEvent(generalEvent);

    ASSERT_EQ(local_event_manager->get_handler_count(EventType::GeneralEvent), 0);
}