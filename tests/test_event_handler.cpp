//
// Created by Andres Suazo
//

#include <gtest/gtest.h>
#include "testing_utils.h"
#include <memory>
#include <vector>

using namespace event_system;

class EventHandlerTest : public ::testing::Test {
protected:
    event_system::EventHandler<int> handler;
};

TEST_F(EventHandlerTest, RegisterAddsCallbackToHandler) {
    // Ensures callbacks are being added to the handler
    ASSERT_EQ(handler.GetCallbackCount(), 0);
    handler.Register([](const int& event){});
    ASSERT_EQ(handler.GetCallbackCount(), 1);
}

TEST_F(EventHandlerTest, RegisterAddsCallbackToSingleHandler) {
    // Ensures that other handlers are not affected
    EventHandler<int> handler2{};

    ASSERT_EQ(handler.GetCallbackCount(), 0);
    ASSERT_EQ(handler2.GetCallbackCount(), 0);

    handler.Register([](const int& event){});

    ASSERT_EQ(handler.GetCallbackCount(), 1);
    ASSERT_EQ(handler2.GetCallbackCount(), 0);
}

TEST_F(EventHandlerTest, RegisterAppendsToCallbacks) {
    // Ensures that callbacks are not overwritten
    ASSERT_EQ(handler.GetCallbackCount(), 0);
    handler.Register([](const int& event){});
    handler.Register([](const int& event){});

    ASSERT_EQ(handler.GetCallbackCount(), 2);
}

TEST_F(EventHandlerTest, OnEventCallsRegisteredCallbacks_SingleCallback) {
    int event_value = 10;
    bool callback_called = false;
    handler.Register([&](const int& event){
        callback_called = true;
        ASSERT_EQ(event, event_value);
    });
    handler.OnEvent(event_value);
    ASSERT_TRUE(callback_called);
}

TEST_F(EventHandlerTest, OnEventCallsRegisteredCallbacks_MultipleCallbacks) {
    int event_value = 5;
    bool callback_1_called = false;
    bool callback_2_called = false;

    handler.Register([&](const int& event){
        callback_1_called = true;
        ASSERT_EQ(event, event_value);
    });

    handler.Register([&](const int& event){
        callback_2_called = true;
        ASSERT_EQ(event, event_value);
    });

    handler.OnEvent(event_value);
    ASSERT_TRUE(callback_1_called);
    ASSERT_TRUE(callback_2_called);
}

TEST_F(EventHandlerTest, DeregisterRemovesCallbackFromHandler) {
    bool callback_called = false;
    int callback_id = handler.Register([&](const int& event){
        callback_called = true;
    });
    handler.Deregister(callback_id);
    handler.OnEvent(5);

    ASSERT_FALSE(callback_called);
}

TEST_F(EventHandlerTest, DeregisterDoesNotAffectOtherCallbacks) {
    int callback_id = handler.Register([](const int& event){});
    bool callback_called = false;
    handler.Register([&](const int& event){
        callback_called = true;
    });
    handler.Deregister(callback_id);
    handler.OnEvent(5);
    ASSERT_TRUE(callback_called);
}