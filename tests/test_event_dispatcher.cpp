//
// Created by Andres Suazo
//

#include "gtest/gtest.h"
#include "event_dispatcher.h"
#include <string>

using namespace event_system;

void OnDoubleStringEvent (double a, const std::string& b)
{
  std::cout << "Event received: " << a << ", " << b << std::endl;
}

struct SimpleStruct {
    int a;
    float b;
};

struct ComplexStruct {
    int &a;
    SimpleStruct& b;
};

void OnSharedPointerEvent (const std::shared_ptr<int>& a, const std::shared_ptr<float> b)
{
  std::cout << "Event received: " << *a << ", " << *b << std::endl;
}

void OnRawPointerEvent (const int *a, const float *b)
{
  std::cout << "Event received: " << *a << ", " << *b << std::endl;
}

void OnSimpleStruct (SimpleStruct simple_struct)
{
  std::cout << "Event received: " << simple_struct.a << ", " << simple_struct.b << std::endl;
}

void OnComplexStruct (ComplexStruct complex_struct)
{
  std::cout << "Event received: " << complex_struct.a << ", " << complex_struct.b.a << std::endl;
}

class EventDispatcherTest : public ::testing::Test {
 protected:
  EventDispatcher event_dispatcher;
};

// Adding and removing callbacks

TEST_F(EventDispatcherTest, AddCallbacksCreatesCorrectNumberOfHandlers)
{
  // With lambda
  event_dispatcher.AddCallback<int> ([] (int event_int){});
  event_dispatcher.AddCallback<int> ([] (const int &event_int){});
  event_dispatcher.AddCallback<int> ([] (const int event_int){});
  // Function
  event_dispatcher.AddCallback<int, std::string> (OnDoubleStringEvent);
  event_dispatcher.AddCallback<std::shared_ptr<int>, std::shared_ptr<float>> (OnSharedPointerEvent);
  event_dispatcher.AddCallback<int *, float *> (OnRawPointerEvent);
  // Custom event types
  event_dispatcher.AddCallback<SimpleStruct> (OnSimpleStruct);
  event_dispatcher.AddCallback<ComplexStruct> (OnComplexStruct);

  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 6);
}

TEST_F(EventDispatcherTest, RemoveCallbacksRemovesCorrectNumberOfHandlers)
{
  size_t callback_id = event_dispatcher.AddCallback<int> ([] (int event_int){});
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 1);
  event_dispatcher.RemoveCallback<int>(callback_id + 1);
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 1);
  event_dispatcher.RemoveCallback<int>(callback_id);
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 0);
}

TEST_F(EventDispatcherTest, RemoveCallbackFromNonExistentHandler)
{
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 0);
  ASSERT_NO_THROW(event_dispatcher.RemoveCallback<int>(0));
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 0);
}

TEST_F(EventDispatcherTest, RemoveEntireHandler) {
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 0);
  event_dispatcher.AddCallback<int> ([] (int event_int){});
  event_dispatcher.AddCallback<int> ([] (const int event_int){});
  event_dispatcher.AddCallback<int> ([] (const int& event_int){});
  event_dispatcher.AddCallback<float> ([] (const float& event_float){});
  event_dispatcher.AddCallback<std::vector<double>> ([] (const std::vector<double>& event_vector){});
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 3);

  event_dispatcher.ClearHandlerCallbacks<int>();
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 2);

}

TEST_F(EventDispatcherTest, RemoveEntireHandler_NonExistent) {
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 0);
  event_dispatcher.AddCallback<int> ([] (int event_int){});
  event_dispatcher.AddCallback<int> ([] (const int event_int){});
  ASSERT_NO_THROW(event_dispatcher.ClearHandlerCallbacks<float>());
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 1);

}

// Basic callbacks

TEST_F(EventDispatcherTest, CallbackInvokedProperly_NoParameter)
{
  bool callback_invoked = false;
  event_dispatcher.AddCallback<>([&callback_invoked] (){ callback_invoked = true;});
  ASSERT_FALSE(callback_invoked);
  event_dispatcher.Dispatch<>();
  ASSERT_TRUE(callback_invoked);
}

TEST_F(EventDispatcherTest, CallbackInvokedProperly_SingleParameter)
{
  bool callback_invoked = false;
  event_dispatcher.AddCallback<int> ([&callback_invoked] (int event){ callback_invoked = true;});
  ASSERT_FALSE(callback_invoked);
  event_dispatcher.Dispatch<int>(1);
  ASSERT_TRUE(callback_invoked);
}

TEST_F(EventDispatcherTest, CallbackInvokedProperly_MultipleParameters)
{
  bool callback_invoked = false;
  int test_input_int_1 = 1;
  int test_input_int_2 = 12;

  event_dispatcher.AddCallback<int, int>([&] (int a, int b) {
    callback_invoked = true;
    ASSERT_EQ(a, test_input_int_1);
    ASSERT_EQ(b, test_input_int_2);
  });

  ASSERT_FALSE(callback_invoked);
  event_dispatcher.Dispatch<int, int>(test_input_int_1, test_input_int_2);
  ASSERT_TRUE(callback_invoked);
}

TEST_F(EventDispatcherTest, CallbackInvokedProperly_MultipleDifferentParameters)
{
  bool callback_invoked = false;
  int test_input_int_1 = 1;
  std::string test_input_string_1 = "test_string";

  event_dispatcher.AddCallback<int, std::string>([&] (int a, const std::string& b) {
      callback_invoked = true;
      ASSERT_EQ(a, test_input_int_1);
      ASSERT_EQ(b, test_input_string_1);
  });

  ASSERT_FALSE(callback_invoked);
  event_dispatcher.Dispatch<int, std::string>(test_input_int_1, test_input_string_1);
  ASSERT_TRUE(callback_invoked);
}

// Const and Reference Parameters

TEST_F(EventDispatcherTest, CallbackInvokedProperly_ConstParameter)
{
  int callback_invoke_count = 0;
  int test_input_int_1 = 1;

  // Callbacks can be values, const values, or const references. They cannot be non-const references unless specified in the template
  event_dispatcher.AddCallback<int>([&] (int a) {
      callback_invoke_count++;
      ASSERT_EQ(a, test_input_int_1);
  });
  event_dispatcher.AddCallback<int>([&] (const int a) {
      callback_invoke_count++;
      ASSERT_EQ(a, test_input_int_1);
  });
  event_dispatcher.AddCallback<int>([&] (const int& a) {
      callback_invoke_count++;
      ASSERT_EQ(a, test_input_int_1);
  });

  ASSERT_EQ(callback_invoke_count, 0);
  event_dispatcher.Dispatch<int>(test_input_int_1);
  // No callbacks should be invoked
  event_dispatcher.Dispatch<int&>(test_input_int_1);
  ASSERT_EQ(callback_invoke_count, 3);
}

TEST_F(EventDispatcherTest, CallbackInvokedProperly_NonConstReferenceParameter)
{
  int callback_invoke_count = 0;
  int test_input_int_1 = 0;

  // Callbacks can be values, const values, or const references. They cannot be non-const references unless specified in the template
  event_dispatcher.AddCallback<int&>([&callback_invoke_count] (int a) {
      callback_invoke_count++;
      a++;
  });
  event_dispatcher.AddCallback<int&>([&callback_invoke_count] (const int a) {
      callback_invoke_count++;
  });
  event_dispatcher.AddCallback<int&>([&callback_invoke_count] (const int& a) {
      callback_invoke_count++;
  });
  event_dispatcher.AddCallback<int&>([&callback_invoke_count] (int& a) {
      callback_invoke_count++;
      a++;
  });

  ASSERT_EQ(callback_invoke_count, 0);
  event_dispatcher.Dispatch<int&>(test_input_int_1);
  // No callbacks should be invoked
  event_dispatcher.Dispatch<int>(test_input_int_1);
  ASSERT_EQ(callback_invoke_count, 4);
  ASSERT_EQ(test_input_int_1, 1);
}

// Dispatching events when no callbacks are registered

TEST_F(EventDispatcherTest, DispatchingWithNoCallbacksDoesNotCreateHandler) {
  EXPECT_NO_THROW(event_dispatcher.Dispatch<int>(100));
  ASSERT_TRUE(event_dispatcher.GetHandlerCount () == 0);
}

// Multiple callbacks for the same event

TEST_F(EventDispatcherTest, DispatchToMultipleCallbacks_SameEventType) {
  int callback_count = 0;

  event_dispatcher.AddCallback<>([&]() { callback_count++; });
  event_dispatcher.AddCallback<>([&]() { callback_count++; });
  event_dispatcher.Dispatch<>();
  EXPECT_EQ(callback_count, 2);
}

// Multiple callbacks for different events

TEST_F(EventDispatcherTest, DispatchToMultipleCallbacks_DifferentEventTypes) {
  int callback_count = 0;
  bool string_callback_invoked = false;
  bool double_callback_invoked = false;

  event_dispatcher.AddCallback<>([&callback_count]() { callback_count++; });
  event_dispatcher.AddCallback<int>([&callback_count](int) { callback_count++; });
  event_dispatcher.AddCallback<double>([&callback_count, &double_callback_invoked](double ) { double_callback_invoked = true, callback_count++; });
  event_dispatcher.AddCallback<std::string>([&callback_count, &string_callback_invoked]( const std::string& ) { string_callback_invoked = true, callback_count++; });

  event_dispatcher.Dispatch<int>(10);
  event_dispatcher.Dispatch<int>(5.4);
  event_dispatcher.Dispatch<>();

  EXPECT_EQ(callback_count, 3);
  EXPECT_FALSE(double_callback_invoked);
  EXPECT_FALSE(string_callback_invoked);
}