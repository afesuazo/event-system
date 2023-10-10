//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include "testing_utils.h"
#include <memory>

using namespace event_system;

class EventHandlerTest : public ::testing::Test {
protected:
    std::shared_ptr<EventHandler<TestGeneralEvent>> general_event_handler;
    bool event_triggered = false;
    void SetUp() override {
        general_event_handler = std::make_shared<EventHandler<TestGeneralEvent>>();
    }
};

TEST_F(EventHandlerTest, DerivedOnEventTriggeredTest) {
    TestGeneralEvent general_event{};
    general_event_handler->OnEvent(general_event);
    EXPECT_FALSE(event_triggered);
    auto callback = [this](const TestGeneralEvent& event){event_triggered = true;};
    general_event_handler->Subscribe(callback);
    general_event_handler->OnEvent(general_event);
    EXPECT_TRUE(event_triggered);
}

TEST_F(EventHandlerTest, DISABLED_ProperEventTypeTest) {
//    TestSpecificEvent specificEvent{};
//    general_event_handler->OnEvent(specificEvent);
//    auto callback = [this](const TestSpecificEvent& event){event_triggered = true;};
//    general_event_handler->Subscribe(callback);
//    general_event_handler->OnEvent(TestGeneralEvent{});
//    EXPECT_FALSE(event_triggered);
}