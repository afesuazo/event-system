//
// Created by Andres Suazo
//

#pragma once

#include "event_handler.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <cstddef>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace event_system {

/**
 * @brief Provides a method of communication between independent application
 * components through events
 *
 * This is the central piece of the event system. The event dispatcher allows
 * for callbacks to be registered for specific events and calls them as events
 * of that type are being triggered through the application. It instantiates an
 * new EventHandler for each event type that needs to be listened to and passes
 * along the registered callbacks to said handler.
 */
class EventDispatcher {
public:
  EventDispatcher() = default;
  ~EventDispatcher() = default;

  /**
   * @brief Adds a callback for the specified event type.
   * @tparam Args The event type the callback will be registered for.
   * @param callback Function to be called when an event is received.
   * @returns int Identifier of the callback. Ids are unique within each event
   * type.
   *
   * @note If there is no EventHandler for the specified event type, one will
   * be created.
   * @note Non const references can't be used as they cannot be bound to
   * rvalues.
   * @todo Treat const int and int as the same type. Tre
   */
  template <typename... Args, typename Function>
  size_t AddCallback(Function &&callback) {
    return GetOrCreateHandler<Args...>().AddCallback(
        std::forward<Function>(callback));
  }

  /**
   * @brief Removes a callback for the specified event type.
   * @tparam Args The event type the callback is registered for.
   * @param callback_id Identifier of the callback to remove.
   */
  template <typename... Args> void RemoveCallback(size_t callback_id) {
    auto handler = GetHandler<Args...>();
    if (!handler) {
      return;
    }

    handler->RemoveCallback(callback_id);

    if (handler->GetCallbackCount() == 0) {
      ClearHandlerCallbacks<Args...>();
    }
  }

  /**
   * @brief Removes all callbacks for the specified event type.
   * @tparam Args The event type for which callbacks will be removed.
   */
  template <typename... Args> void ClearHandlerCallbacks() {
    handlers_.erase(std::type_index(typeid(EventHandler<Args...>)));
  }

  /**
   * @brief Dispatches an event to the appropriate EventHandler.
   * @tparam Args The type of the event to be dispatched.
   * @param event The event to be dispatched.
   */
  template <typename... Args> void Dispatch(Args... args) {
    auto handler = GetHandler<Args...>();
    if (!handler) {
      std::clog << "No handler for event type: "
                << typeid(EventHandler<Args...>).name() << std::endl;
      return;
    }
    handler->OnEvent(args...);
  }

  /**
   * @brief Returns the amount of active event handlers.
   * @return size_t The amount of active handlers
   */
  [[nodiscard]] size_t GetHandlerCount() const;

private:
  /**
   * @brief Returns a reference to the EventHandler for the specified event
   * type. Creates a new handler if one does not exist.
   * @tparam Args The event type of the handler.
   * @returns EventHandler<TEvent>& Reference to the EventHandler object.
   */
  template <typename... Args>
  [[nodiscard]] EventHandler<Args...> &GetOrCreateHandler() {
    std::type_index type_index{typeid(EventHandler<Args...>)};
    auto it = handlers_.find(type_index);
    if (it == handlers_.end()) {
      auto handler = std::make_unique<EventHandler<Args...>>();
      it = handlers_.emplace(type_index, std::move(handler)).first;
    }
    // Return a reference instead of a pointer since the handler is guaranteed
    // to exist
    return static_cast<EventHandler<Args...>&>(*it->second);
  }

  /**
   * @brief Returns a pointer to the EventHandler for the specified event type.
   * @tparam Args The event type of the handler.
   * @returns EventHandler<TEvent>* Pointer to the EventHandler object.
   */
  template <typename... Args>
  [[nodiscard]] EventHandler<Args...>* GetHandler() {
    auto it = handlers_.find(std::type_index(typeid(EventHandler<Args...>)));
    if (it != handlers_.end()) {
      return static_cast<EventHandler<Args...> *>(it->second.get());
    }
    return nullptr;
  }

private:
  std::unordered_map<std::type_index, std::unique_ptr<IEventHandler>> handlers_;
};

} // namespace event_system
