//
// Created by Andres Suazo
//

#pragma once

#include <iostream>
#include "base_event.h"

namespace event_manager {

    /**
     * @class IEventListenerBase
     * @brief Interface for all event listeners, intended for polymorphism.
     *
     * Provides a base interface for all event listeners. This allows us to easily create containers for
     * EventListeners and handle multiple types of events in a uniform manner.
     *
     * @note Should not be inherited from directly, instead use IEventListener
     */
    class IEventListenerBase {
    public:
        virtual ~IEventListenerBase() = default;

        /**
         * @brief Called when an event this listener subscribed to is triggered
         *
         * @param event Reference to the event that was triggered.
         */
        virtual void OnEvent(const BaseEvent& event) = 0;
    };

    /**
     * @class IEventListener
     * @brief Templated interface for creating a listener of a specific event type
     *
     * @tparam TEvent The event type this listener is intended to subscribe to
     * @note The `TEvent` type must be derived from `IEvent`
     */
    template<typename TEvent>
    class IEventListener : public IEventListenerBase {
        static_assert(std::is_base_of<BaseEvent, TEvent>::value, "TEvent must be be derived from IEvent");
    public:

        /**
         * @brief Overridden method to handle type-erasure. Will immediately call the expected OnEvent method.
         *
         * Dynamically casts the event reference to the expected event type (TEvent). If types match,
         * a virtual OnEvent function with the same signature gets called.
         *
         * @param event The event that was triggered.
         */
        void OnEvent(const BaseEvent& event) override {
            if (typeid(event) == typeid(TEvent)) {
                OnEvent(static_cast<const TEvent&>(event));
            }
        }

        /**
         * @brief Called when an event this listener subscribed to is triggered
         *
         * @param event Reference to the event (of type TEvent) that was triggered.
         */
        virtual void OnEvent(const TEvent& event) = 0;
    };

}