//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include "testing_utils.h"
#include "event_listener.h"
#include <memory>

using namespace event_system;

class EventListenerTest : public ::testing::Test {
protected:
    std::shared_ptr<EventListener<GeneralEvent>> general_event_listener;

    void SetUp() override {
        general_event_listener = std::make_shared<TestEventListener<GeneralEvent>>();
    }
};

TEST_F(EventListenerTest, DerivedOnEventTriggeredTest) {
    GeneralEvent general_event{GeneralEvent::SubType::GeneralSubType0};
    auto casted_general_event_listener = std::dynamic_pointer_cast<TestEventListener<GeneralEvent>>(general_event_listener);
    casted_general_event_listener->OnEvent(general_event, true);

    EXPECT_TRUE(casted_general_event_listener->event_triggered);
}