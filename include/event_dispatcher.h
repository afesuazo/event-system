//
// Created by Andres Suazo
//

#pragma once

#include "event_handler.h"
#include <unordered_map>
#include <algorithm>
#include <memory>

namespace event_system {

    using ref = std::reference_wrapper<const std::type_info>;

    struct hash {
        auto operator()(ref code) const
        { return code.get().hash_code(); }
    };

    struct equality {
        auto operator()(ref lhs, ref rhs) const
        { return lhs.get() == rhs.get(); }
    };

    /**
     * @brief Provides a method of communication between independent application components through events
     *
     * This is the central piece of the event system. The event dispatcher allows for callbacks to be registered
     * for specific events and calls them as events of that type are being triggered through the application.
     * It instantiates an new EventHandler for each event type that needs to be listened to and passes along
     * the registered callbacks to said handler.
     */
    class EventDispatcher {
        using HandlerSharedPtr = std::shared_ptr<IEventHandler>;
        using EventDispatcherMap = std::unordered_map<std::reference_wrapper<const std::type_info>, HandlerSharedPtr, hash, equality>;

    public:
        EventDispatcher() = default;

        ~EventDispatcher() = default;

        /**
         * @brief Register a callback for a specific event type.
         *
         * @tparam TEvent The event type the callback will be registered for.
         * @param callback Function to be called when an event is received.
         *
         * @returns int Identifier representing the callback in the EventHandler
         *
         * Adds a callback to the EventHandler for the specified event type. If there is no handler for the event type,
         * one will be created.
         */
        template<typename TEvent>
        int Register(Callback<TEvent> callback) {
            int callback_id = GetDispatcher<TEvent>()->Register(callback);
            return callback_id;
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
        void Deregister(int callback_id) {
            // TODO: Delete event handler if its last callback has been removed
            GetDispatcher<TEvent>()->Deregister(callback_id);
        }

        /**
         * @brief Removes all callbacks registered for a specific event type.
         *
         * @tparam TEvent The event type for which callbacks will be removed.
         */
        template<typename TEvent>
        void DeregisterAll() {
            handlers_.erase(typeid(TEvent));
        }

        /**
         * @brief Dispatches an event to the appropriate EventHandler.
         *
         * @tparam TEvent The type of the event to be dispatched. Must derive from BaseEvent
         * @param event Reference to the event object to dispatch.
         */
        template <typename TEvent>
        void Dispatch(const TEvent& event) {
            static_assert(std::is_base_of<BaseEvent, TEvent>::value, "TEvent must be derive from BaseEvent");
            GetDispatcher<TEvent>()->OnEvent(event);
        }

        /**
          * @brief Returns the total amount of active EventHandlers
          *
          * @return size_t representing the number of handlers
          */
        [[nodiscard]] size_t GetHandlerCount() const {
            return handlers_.size();
        }

    private:
        /**
         * @brief Returns a pointer to the EventHandler for the specified event type.
         * If there is no handler for the event type, one will be created.
         *
         * @tparam TEvent The event type of the handler.
         *
         * @return size_t Pointer to the EventHandler object.
         */
        template<typename TEvent>
        std::shared_ptr<EventHandler<TEvent>> GetDispatcher() {
            auto& specific_handler = handlers_[typeid(TEvent)];
            if (!specific_handler) {
                specific_handler.reset(new EventHandler<TEvent>());
            }
            return std::static_pointer_cast<EventHandler<TEvent>>(specific_handler);
        }
    private:
        EventDispatcherMap handlers_;
    };

}