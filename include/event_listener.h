//
// Created by Andres Suazo
//

#pragma once

#include <iostream>
#include "base_event.h"
#include "event_handler.h"

namespace event_system {

    /**
     * @class IEventListenerBase
     * @brief Interface for all event listeners, intended for polymorphism.
     *
     * Provides a base interface for all event listeners. This allows us to easily create containers for
     * EventListeners and handle multiple types of events in a uniform manner.
     *
     * @note Should not be inherited from directly, instead use EventListener
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
     * @class EventListener
     * @brief Templated class for creating a listener of a specific event type
     *
     * @tparam TEvent The event type this listener is intended to subscribe to
     */
    template<typename TEvent>
    class EventListener : public IEventListenerBase {
        static_assert(std::is_base_of<BaseEvent, TEvent>::value, "TEvent must be be derived from BaseEvent");
    public:
         ~EventListener() override = default;

        void RegisterHandler(std::type_index event_type, std::unique_ptr<IEventHandler> handler) {
            handlers[event_type] = std::move(handler);
        }

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
        void OnEvent(const TEvent& event) {
            std::type_index event_type = event.get_type();
            if (handlers.contains(event_type)) {
                handlers[event_type]->HandleEvent(event);
            }
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<IEventHandler>> handlers;
    };


}