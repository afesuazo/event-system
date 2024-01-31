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

        /**
         * @brief Adds a callback for the specified event type.
         * @tparam TEvent The event type the callback will be registered for.
         * @param callback Function to be called when an event is received.
         * @returns int Identifier of the callback. Ids are unique within each event type.
         *
         * @note If there is no EventHandler for the specified event type, one will be created.
         */
        template<typename TEvent>
        int AddCallback(const Callback<TEvent>& callback) {
            auto&  handler = GetOrCreateHandler<TEvent>();
            return handler.Register(callback);
        }

        /**
         * @brief Removes a callback for the specified event type.
         * @tparam TEvent The event type the callback is registered for.
         * @param callback_id Identifier of the callback to remove.
         */
        template<typename TEvent>
        void RemoveCallback(int callback_id) {
            auto handler = GetHandler<TEvent>();
            if (!handler) { return; }
            // TODO: Delete event handler if its last callback has been removed
            handler->Deregister(callback_id);
        }

        /**
         * @brief Removes all callbacks for the specified event type.
         * @tparam TEvent The event type for which callbacks will be removed.
         */
        template<typename TEvent>
        void ClearHandlerCallbacks() {
            handlers_.erase(std::type_index(typeid(TEvent)));
        }

        /**
         * @brief Dispatches an event to the appropriate EventHandler.
         * @tparam TEvent The type of the event to be dispatched.
         * @param event The event to be dispatched.
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
         * @brief Returns the amount of active event handlers.
         * @return size_t The amount of active handlers
         */
        [[nodiscard]] size_t GetHandlerCount() const;

    private:
        /**
         * @brief Returns a reference to the EventHandler for the specified event type. Creates a new handler
         * if one does not exist.
         * @tparam TEvent The event type of the handler.
         * @returns EventHandler<TEvent>& Reference to the EventHandler object.
         */
        template<typename TEvent>
        [[nodiscard]] EventHandler<TEvent>& GetOrCreateHandler() {
            auto it = handlers_.find(std::type_index(typeid(TEvent)));
            if (it == handlers_.end()) {
                auto handler = std::make_unique<EventHandler<TEvent>>();
                it = handlers_.emplace(std::type_index(typeid(TEvent)), std::move(handler)).first;
            }
            // Return a reference instead of a pointer since the handler is guaranteed to exist
            return static_cast<EventHandler<TEvent>&>(*(it->second));
        }

        /**
         * @brief Returns a pointer to the EventHandler for the specified event type.
         * @tparam TEvent The event type of the handler.
         * @returns EventHandler<TEvent>* Pointer to the EventHandler object.
         */
        template<typename TEvent>
        [[nodiscard]] EventHandler<TEvent>* GetHandler() {
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