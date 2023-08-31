//
// Created by Andres Suazo
//

#pragma once
#include "string"
#include "i_event.h"

namespace event_manager {

    /**
     * @class EventListenerBase
     *
     * @brief Interface for all event listeners, intended for polymorphism.
     *
     * Provides a base interface for all event listeners. This allows us to easily create containers for
     * EventListeners and handle multiple types of events in a uniform manner.
     *
     * @note Should not be inherited from directly, instead use EventListener
     *
     */
    class EventListenerBase {
    public:
        virtual ~EventListenerBase() = default;

        /**
         * @brief Called when an event this listener subscribed to is triggered
         *
         * @param event Reference to the event that was triggered.
         */
        virtual void OnEvent(IEvent& event) = 0;
    };

    /**
     * @class EventListener
     * @brief Templated interface for creating a listener of a specific event type
     *
     * @tparam TEvent The event type this listener is intended to subscribe to
     * @note The `TEvent` type must be derived from `IEvent`
     */
    template<typename TEvent>
    class EventListener : public EventListenerBase {
        static_assert(std::is_base_of<IEvent, TEvent>::value, "TEvent must be be derived from IEvent");
    public:

        /**
         * @brief Overridden method to handle type-erasure. Will immediately call the expected OnEvent method.
         *
         * Dynamically casts the event reference to the expected event type (TEvent). If types match,
         * an virtual OnEvent function with the same signature gets called.
         *
         * @param event The event that was triggered.
         */
        void OnEvent(IEvent& event) override {
            if (typeid(event) == typeid(TEvent)) {
                OnEvent(dynamic_cast<TEvent&>(event));
            }
        }

        /**
         * @brief Called when an event this listener subscribed to is triggered
         *
         * @param event Reference to the event (of type TEvent) that was triggered.
         */
        virtual void OnEvent(TEvent& event) = 0;
    };

}