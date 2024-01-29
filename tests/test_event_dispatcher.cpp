//
// Created by Andres Suazo
//

#include "gtest/gtest.h"
#include "event_dispatcher.h"
#include "base_event.h"
#include "testing_utils.h"
#include <memory>

using namespace event_system;

class EventDispatcherTest : public ::testing::Test {
protected:
    std::shared_ptr<EventDispatcher> local_event_dispatcher;
    bool general_event_triggered = false;
    bool specific_event_triggered = false;

    void SetUp() override {
        local_event_dispatcher = std::make_shared<EventDispatcher>();
    }
};

TEST_F(EventDispatcherTest, SubscribeTest) {
    auto callback = [](const TestGeneralEvent& event){};
    local_event_dispatcher->Register<TestGeneralEvent>(callback);
    ASSERT_TRUE(local_event_dispatcher->GetHandlerCount() == 1);
}

TEST_F(EventDispatcherTest, AddDuplicateSubscriberTest) {
    ASSERT_EQ(local_event_dispatcher->GetHandlerCount(), 0);
    auto callback = [](const TestGeneralEvent& event){};
    local_event_dispatcher->Register<TestGeneralEvent>(callback);
    local_event_dispatcher->Register<TestGeneralEvent>(callback);
    ASSERT_EQ(local_event_dispatcher->GetHandlerCount(), 1);
}

TEST_F(EventDispatcherTest, MultipleSubscriberTest) {
    auto general_callback = [](const TestGeneralEvent& event){};
    auto specific_callback = [](const TestSpecificEvent& event){};

    local_event_dispatcher->Register<TestGeneralEvent>(general_callback);
    local_event_dispatcher->Register<TestSpecificEvent>(specific_callback);

    ASSERT_EQ(local_event_dispatcher->GetHandlerCount(), 2);
}

TEST_F(EventDispatcherTest, RemoveSubscriberTest) {
    auto general_callback = [this](const TestGeneralEvent& event){general_event_triggered = true;};
    int callback_id = local_event_dispatcher->Register<TestGeneralEvent>(general_callback);
    ASSERT_EQ(local_event_dispatcher->GetHandlerCount(), 1);
    local_event_dispatcher->Dispatch(TestGeneralEvent{});
    EXPECT_TRUE(general_event_triggered);
    general_event_triggered = false;
    local_event_dispatcher->Deregister<TestGeneralEvent>(callback_id);
    local_event_dispatcher->Dispatch(TestGeneralEvent{});
    EXPECT_FALSE(general_event_triggered);
}

TEST_F(EventDispatcherTest, RemoveSubscriberFromEmptyMapTest) {
    ASSERT_NO_THROW(local_event_dispatcher->Deregister<TestGeneralEvent>(4));
}

TEST_F(EventDispatcherTest, TriggerEventTest) {
    auto callback = [this](const TestGeneralEvent& event){general_event_triggered = true;};
    local_event_dispatcher->Register<TestGeneralEvent>(callback);
    EXPECT_FALSE(general_event_triggered);
    TestGeneralEvent general_event{};
    local_event_dispatcher->Dispatch(general_event);
    EXPECT_TRUE(general_event_triggered);
}

TEST_F(EventDispatcherTest, TriggerEventMultipleSubscriberTest) {
    auto general_callback = [this](const TestGeneralEvent& event){general_event_triggered = true;};
    auto specific_callback = [this](const TestSpecificEvent& event){specific_event_triggered = true;};

    local_event_dispatcher->Register<TestGeneralEvent>(general_callback);
    local_event_dispatcher->Register<TestSpecificEvent>(specific_callback);

    EXPECT_FALSE(general_event_triggered);
    EXPECT_FALSE(specific_event_triggered);

    TestGeneralEvent generalEvent{};
    local_event_dispatcher->Dispatch(generalEvent);

    EXPECT_TRUE(general_event_triggered);
    EXPECT_FALSE(specific_event_triggered);
}

TEST_F(EventDispatcherTest, UnsubscribeAllCallbacksTest) {
    auto general_callback = [](const TestGeneralEvent& event){};
    auto specific_callback = [](const TestSpecificEvent& event){};

    local_event_dispatcher->Register<TestGeneralEvent>(general_callback);
    local_event_dispatcher->Register<TestSpecificEvent>(specific_callback);

    ASSERT_EQ(local_event_dispatcher->GetHandlerCount(), 2);
    local_event_dispatcher->DeregisterAll<TestGeneralEvent>();
    ASSERT_EQ(local_event_dispatcher->GetHandlerCount(), 1);

    // Test removing an event that does not exist
    ASSERT_NO_THROW(local_event_dispatcher->DeregisterAll<TestGeneralEvent>());

}