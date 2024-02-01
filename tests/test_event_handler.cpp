//
// Created by Andres Suazo
//

#include "event_handler.h"
#include <gtest/gtest.h>

using namespace event_system;

class EventHandlerTest : public ::testing::Test {
protected:
  EventHandler<int> handler;
};

// Ensures callbacks are being added to the handler
TEST_F(EventHandlerTest, RegisterAddsCallbackToHandler) {
  ASSERT_EQ(handler.GetCallbackCount(), 0);
  handler.AddCallback([](const int &event) {});
  ASSERT_EQ(handler.GetCallbackCount(), 1);
}

// Ensures that other handlers are not affected
TEST_F(EventHandlerTest, RegisterAddsCallbackToSingleHandler) {
  EventHandler<int> handler2{};

  ASSERT_EQ(handler.GetCallbackCount(), 0);
  ASSERT_EQ(handler2.GetCallbackCount(), 0);

  handler.AddCallback([](const int &event) {});

  ASSERT_EQ(handler.GetCallbackCount(), 1);
  ASSERT_EQ(handler2.GetCallbackCount(), 0);
}

TEST_F(EventHandlerTest, RegisterAppendsToCallbacks) {
  // Ensures that callbacks are not overwritten
  ASSERT_EQ(handler.GetCallbackCount(), 0);
  handler.AddCallback([](const int &event) {});
  handler.AddCallback([](const int &event) {});

  ASSERT_EQ(handler.GetCallbackCount(), 2);
}

TEST_F(EventHandlerTest, OnEventCallsRegisteredCallbacks_SingleCallback) {
  int event_value = 10;
  bool callback_called = false;
  handler.AddCallback([&](const int &event) {
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

  handler.AddCallback([&](const int &event) {
    callback_1_called = true;
    ASSERT_EQ(event, event_value);
  });

  handler.AddCallback([&](const int &event) {
    callback_2_called = true;
    ASSERT_EQ(event, event_value);
  });

  handler.OnEvent(event_value);
  ASSERT_TRUE(callback_1_called);
  ASSERT_TRUE(callback_2_called);
}

TEST_F(EventHandlerTest, DeregisterRemovesCallbackFromHandler) {
  bool callback_called = false;
  size_t callback_id =
      handler.AddCallback([&](const int &event) { callback_called = true; });
  handler.RemoveCallback(callback_id);
  handler.OnEvent(5);

  ASSERT_FALSE(callback_called);
}

TEST_F(EventHandlerTest, DeregisterDoesNotAffectOtherCallbacks) {
  size_t callback_id = handler.AddCallback([](const int &event) {});
  bool callback_called = false;
  handler.AddCallback([&](const int &event) { callback_called = true; });
  handler.RemoveCallback(callback_id);
  handler.OnEvent(5);
  ASSERT_TRUE(callback_called);
}
