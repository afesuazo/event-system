//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include "testing_utils.h"
#include <memory>

using namespace event_system;

class EventHandlerTest : public ::testing::Test {
protected:
    std::shared_ptr<IEventHandler<TestGeneralEvent>> general_event_handler;

    void SetUp() override {
        general_event_handler = std::make_shared<TestEventHandler<TestGeneralEvent>>();
    }
};

TEST_F(EventHandlerTest, DerivedOnEventTriggeredTest) {
    TestGeneralEvent general_event{};
    general_event_handler->OnEvent(general_event);

    auto casted_event_handler = std::static_pointer_cast<TestEventHandler<TestGeneralEvent>>(general_event_handler);
    EXPECT_TRUE(casted_event_handler->event_triggered);
}

TEST_F(EventHandlerTest, ProperEventTypeTest) {
    TestSpecificEvent specificEvent{};
    general_event_handler->OnEvent(specificEvent);

    auto casted_event_handler = std::static_pointer_cast<TestEventHandler<TestGeneralEvent>>(general_event_handler);

    EXPECT_FALSE(casted_event_handler->event_triggered);
}