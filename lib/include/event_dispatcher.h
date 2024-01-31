//
// Created by Andres Suazo
//

#pragma once

#include "event_handler.h"
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <typeindex>
#include <iostream>

namespace event_system {

    /**
     * @brief Provides a method of communication between independent application components through events
     *
     * This is the central piece of the event system. The event dispatcher allows for callbacks to be registered
     * for specific events and calls them as events of that type are being triggered through the application.
     * It instantiates an new EventHandler for each event type that needs to be listened to and passes along
     * the registered callbacks to said handler.
     */
    class EventDispatcher {
        using EventDispatcherMap = std::unordered_map<std::type_index, std::unique_ptr<IEventHandler>>;

    public:
        EventDispatcher() = default;
        ~EventDispatcher() = default;

        template<typename TEvent>
        int AddCallback(const Callback<TEvent>& callback) {
            auto&  handler = GetOrCreateHandler<TEvent>();
            return handler.Register(callback);
        }

        /**
         * @brief Removes a IEventHandler from an events subscriber list.
         *
         * @tparam TEvent The event type the callback is registered for.
         * @param callback_id Identifier of the callback to deregister.
         *
         * Removes a callback from the EventHandler for the specified event type.
         */
        template<typename TEvent>
        void RemoveCallback(int callback_id) {
            auto handler = GetHandler<TEvent>();
            if (!handler) { return; }
            // TODO: Delete event handler if its last callback has been removed
            handler->Deregister(callback_id);
        }

        /**
         * @brief Removes all callbacks registered for a specific event type.
         * @tparam TEvent The event type for which callbacks will be removed.
         */
        template<typename TEvent>
        void ClearHandlerCallbacks() {
            handlers_.erase(std::type_index(typeid(TEvent)));
        }

        /**
         * @brief Dispatches an event to the appropriate EventHandler.
         *
         * @tparam TEvent The type of the event to be dispatched.
         * @param event Reference to the event object to dispatch.
         */
        template <typename TEvent>
        void Dispatch(const TEvent& event) {
            auto handler = GetHandler<TEvent>();
            if (!handler) {
                std::clog << "No handler for event type: " << typeid(TEvent).name() << std::endl;
                return;
            }

            handler->OnEvent(event);
        }

        /**
          * @brief Returns the total amount of active EventHandlers
          * @return size_t representing the number of handlers
          */
        [[nodiscard]] size_t GetHandlerCount() const;

    private:
        // Returning a reference since the method ensures that the handler exists after the call
        template<typename TEvent>
        EventHandler<TEvent>& GetOrCreateHandler() {
            auto it = handlers_.find(std::type_index(typeid(TEvent)));
            if (it == handlers_.end()) {
                auto handler = std::make_unique<EventHandler<TEvent>>();
                it = handlers_.emplace(std::type_index(typeid(TEvent)), std::move(handler)).first;
            }
            return static_cast<EventHandler<TEvent>&>(*(it->second));
        }

        /**
         * @brief Returns a pointer to the EventHandler for the specified event type.
         * If there is no handler for the event type, one will be created.
         *
         * @tparam TEvent The event type of the handler.
         * @returns EventHandler<TEvent>* Pointer to the EventHandler object.
         */
        template<typename TEvent>
        EventHandler<TEvent>* GetHandler() {
            auto it = handlers_.find(std::type_index(typeid(TEvent)));
            if (it != handlers_.end()) {
                return static_cast<EventHandler<TEvent>*>(it->second.get());
            }
            return nullptr;
        }

    private:
        EventDispatcherMap handlers_;
    };

}