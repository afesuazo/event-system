//
// Created by Andres Suazo
//

#pragma once

#include <iostream>
#include "base_event.h"
#include "event_handler.h"

namespace event_manager {

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
    class EventListener {
        static_assert(std::is_base_of<BaseEvent, TEvent>::value, "TEvent must be be derived from BaseEvent");
    public:
        virtual ~EventListener() = default;

        void RegisterHandler(std::type_index event_type, std::unique_ptr<EventHandler> handler) {
            handlers[event_type] = std::move(handler);
        }

        /**
         * @brief Called when an event this listener subscribed to is triggered
         *
         * @param event Reference to the event (of type TEvent) that was triggered.
         */
        void OnEvent(const TEvent& event) {
            std::type_index event_type = event.GetType();
            if (handlers.contains(event_type)) {
                handlers[event_type]->HandleEvent(event);
            }
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<EventHandler>> handlers;
    };


}