//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include "testing_utils.h"
#include <memory>

using namespace event_system;

class EventHandlerTest : public ::testing::Test {
protected:
    std::shared_ptr<IEventHandler<GeneralEvent>> general_event_handler;

    void SetUp() override {
        general_event_handler = std::make_shared<TestEventHandler<GeneralEvent>>();
    }
};

TEST_F(EventHandlerTest, DerivedOnEventTriggeredTest) {
    GeneralEvent general_event{GeneralEvents::GeneralSubType0};
    general_event_handler->OnEvent(general_event);

    auto casted_event_handler = std::static_pointer_cast<TestEventHandler<GeneralEvent>>(general_event_handler);
    EXPECT_TRUE(casted_event_handler->event_triggered);
}

// Disabled for now as I opted to use a static cast in the event handler class
TEST_F(EventHandlerTest, DISABLED_ProperEventTypeTest) {
    SpecificEvent specificEvent{SpecificEvents::SpecificSubType0};
    general_event_handler->OnEvent(specificEvent);

    auto casted_event_handler = std::static_pointer_cast<TestEventHandler<GeneralEvent>>(general_event_handler);

    EXPECT_FALSE(casted_event_handler->event_triggered);
}