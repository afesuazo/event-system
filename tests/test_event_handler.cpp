//
// Created by Andres Suazo
//

#include "event_handler.h"
#include <gtest/gtest.h>

using namespace event_system;

void OnIntEvent(int a)
{
  std::cout << "Event received: " << a << std::endl;
}

class EventHandlerTest : public ::testing::Test {
protected:
  EventHandler<int> handler;
};

// Ensures callbacks are being added to the handler
TEST_F(EventHandlerTest, AddCallback_SingleCallbackIncrementsCount) {
  ASSERT_EQ(handler.GetCallbackCount(), 0);
  handler.AddCallback([](const int &event) {});
  ASSERT_EQ(handler.GetCallbackCount(), 1);
}

TEST_F(EventHandlerTest, AddCallback_MultipleCallbackIncrementsCount) {
  // Ensures that callbacks are not overwritten
  ASSERT_EQ(handler.GetCallbackCount(), 0);
  handler.AddCallback([](const int &event) {});
  handler.AddCallback([](const int &event) {});

  ASSERT_EQ(handler.GetCallbackCount(), 2);
}

// Ensures that other handlers are not affected
TEST_F(EventHandlerTest, AddCallbacks_AddsCallbackToSingleHandler) {
  EventHandler<int> handler2{};

  ASSERT_EQ(handler.GetCallbackCount(), 0);
  ASSERT_EQ(handler2.GetCallbackCount(), 0);

  handler.AddCallback([](const int &event) {});

  ASSERT_EQ(handler.GetCallbackCount(), 1);
  ASSERT_EQ(handler2.GetCallbackCount(), 0);
}

TEST(DISABLED_EventHandlerTest, AddCallbacks_IgnoreDuplicate)
{
  EventHandler<int> handler;
  handler.AddCallback(OnIntEvent);
  handler.AddCallback(OnIntEvent);
  ASSERT_TRUE(handler.GetCallbackCount() == 1);
}

TEST_F(EventHandlerTest, OnEventCallsRegisteredCallbacks_SingleCallback) {
  int event_value = 10;
  bool callback_invoked = false;
  handler.AddCallback([&](const int &event) {
    callback_invoked = true;
    ASSERT_EQ(event, event_value);
  });
  handler.OnEvent(event_value);
  ASSERT_TRUE(callback_invoked);
}

TEST_F(EventHandlerTest, OnEventCallsRegisteredCallbacks_MultipleCallbacks) {
  int event_value = 5;
  bool callback_1_invoked = false;
  bool callback_2_invoked = false;

  handler.AddCallback([&](const int &event) {
    callback_1_invoked = true;
    ASSERT_EQ(event, event_value);
  });

  handler.AddCallback([&](const int &event) {
    callback_2_invoked = true;
    ASSERT_EQ(event, event_value);
  });

  handler.OnEvent(event_value);
  ASSERT_TRUE(callback_1_invoked);
  ASSERT_TRUE(callback_2_invoked);
}

// Test constants and non-const references

TEST(DISABLED_EventHandlerTest, OnEventCallsRegisteredCallbacks_Const) {
  int callback_invoke_count = 0;
  int test_input_int_1 = 0;

  EventHandler<const int> reference_handler;
  EventHandler<int> handler;

  // Should not be called as it is not a const int type
  handler.AddCallback([&callback_invoke_count] (const int a) {
      callback_invoke_count++;
  });

  reference_handler.AddCallback([&callback_invoke_count] (int a) {
      callback_invoke_count++;
      // Value can be modified as it is a copy
      a++;
  });

  reference_handler.AddCallback([&callback_invoke_count] (const int a) {
      callback_invoke_count++;
  });

  // TODO: Since the event type is not a reference, the actual callback signature becomes (const int a) instead of (const int& a)
  reference_handler.AddCallback([&callback_invoke_count, &test_input_int_1] (const int& a) {
      callback_invoke_count++;
      ASSERT_EQ(&a, &test_input_int_1);
  });

  ASSERT_EQ(callback_invoke_count, 0);
  reference_handler.OnEvent(test_input_int_1);
  ASSERT_EQ(callback_invoke_count, 3);
}

TEST_F(EventHandlerTest, OnEventCallsRegisteredCallbacks_NonConstReference) {
  int callback_invoke_count = 0;
  int test_input_int_1 = 0;

  EventHandler<int&> reference_handler;

  reference_handler.AddCallback([&callback_invoke_count, &test_input_int_1] (int a) {
      callback_invoke_count++;
      // Value is copied so input is not modified
      a++;
      ASSERT_NE(&a, &test_input_int_1);
  });

  reference_handler.AddCallback([&callback_invoke_count] (const int a) {
      callback_invoke_count++;
  });

  reference_handler.AddCallback([&callback_invoke_count, &test_input_int_1] (const int& a) {
      callback_invoke_count++;
      ASSERT_EQ(&a, &test_input_int_1);
  });

  reference_handler.AddCallback([&callback_invoke_count] (int& a) {
      callback_invoke_count++;
      a++;
  });

  ASSERT_EQ(callback_invoke_count, 0);
  reference_handler.OnEvent(test_input_int_1);
  ASSERT_EQ(callback_invoke_count, 4);
  ASSERT_EQ(test_input_int_1, 1);
}

// Test removing callbacks

TEST_F(EventHandlerTest, DeregisterRemovesCallbackFromHandler) {
  bool callback_invoked = false;
  size_t callback_id =
      handler.AddCallback([&](const int &event) { callback_invoked = true; });
  handler.RemoveCallback(callback_id);
  handler.OnEvent(5);

  ASSERT_FALSE(callback_invoked);
}

TEST_F(EventHandlerTest, DeregisterDoesNotAffectOtherCallbacks) {
  size_t callback_id = handler.AddCallback([](const int &event) {});
  bool callback_invoked = false;
  handler.AddCallback([&](const int &event) { callback_invoked = true; });
  handler.RemoveCallback(callback_id);
  handler.OnEvent(5);
  ASSERT_TRUE(callback_invoked);
}


// TODO: Test overflow of callback id