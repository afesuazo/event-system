//
// Created by Andres Suazo
//

#pragma once

#include <iostream>
#include "base_event.h"
#include "event_handler.h"
#include <unordered_set>

namespace event_system {

    /**
     * @class IEventListener
     * @brief Interface for all event listeners, intended for polymorphism.
     *
     * Provides a base interface for all event listeners. This allows us to easily create containers for
     * EventListeners and handle multiple types of events in a uniform manner.
     *
     * @note Should not be inherited from directly, instead use EventListener
     */
    class IEventListener {
    public:
        virtual ~IEventListener() = default;

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
    class EventListener : public IEventListener {
        static_assert(std::is_base_of<BaseEvent, TEvent>::value, "TEvent must be be derived from BaseEvent");
    public:
         ~EventListener() override = default;

        void RegisterHandler(std::type_index event_type, std::unique_ptr<IEventHandler> handler) {
            // TODO: Verify event_type is derived from BaseEvent and event handler is made for said event
            handlers_.insert(std::move(handler));
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
                HandleEvent(static_cast<const TEvent&>(event));
            }
        }

        /**
         * @brief Called when an event this listener subscribed to is triggered
         *
         * @param event Reference to the event (of type TEvent) that was triggered.
         */
        void HandleEvent(const TEvent& event) {
            for(auto& handler : handlers_) {
               handler->HandleEvent(event);
            }
        }

    private:
        std::unordered_set<std::unique_ptr<IEventHandler>> handlers_;
    };


}