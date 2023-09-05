//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include "testing_utils.h"
#include "event_listener.h"
#include <memory>

using namespace event_manager;

class EventListenerTest : public ::testing::Test {
protected:
    std::shared_ptr<IEventListener<GeneralEvent>> generalEventListener;

    void SetUp() override {
        generalEventListener = std::make_shared<GeneralEventListener>();
    }
};

TEST_F(EventListenerTest, DerivedOnEventTriggeredTest) {
    GeneralEvent generalEvent{GeneralEvent::SubType::GeneralSubType0};
    generalEventListener->OnEvent(generalEvent);
    auto generalEventListenerCast = std::dynamic_pointer_cast<GeneralEventListener>(generalEventListener);

    EXPECT_TRUE(generalEventListenerCast->eventTriggered);
}

TEST_F(EventListenerTest, ProperEventTypeTest) {
    SpecificEvent specificEvent{SpecificEvent::SubType::SpecificSubType0};
    generalEventListener->OnEvent(specificEvent);
    auto generalEventListenerCast = std::dynamic_pointer_cast<GeneralEventListener>(generalEventListener);

    EXPECT_FALSE(generalEventListenerCast->eventTriggered);
}