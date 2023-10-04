//
// Created by Andres Suazo
//

#pragma once
#include "base_event.h"

namespace event_system {

    /**
     * @class IEventHandlerBase
     * @brief Interface for all event handlers, intended for polymorphism.
     *
     * Provides a base interface for all event handlers. This allows us to easily create containers for
     * EventHandlers and handle multiple types of events in a uniform manner.
     *
     * @note Should not be inherited from directly, instead use IEventHandler
     */
    class IEventHandlerBase {
    public:
        virtual ~IEventHandlerBase() = default;
        virtual void OnEvent(const BaseEvent& event) = 0;
        [[nodiscard]] virtual EventType GetHandledEventType() const = 0;

    };

    /**
     * @class IEventHandler
     * @brief Templated class for creating a handler of a specific event type
     *
     * @tparam TEvent The event type this handler is intended to subscribe to
     */
    template <typename TEvent>
    class IEventHandler : public IEventHandlerBase {
    public:

        /**
         * @brief Overridden method to handle type-erasure. Will immediately try to call the HandleEvent method.
         *
         * Statically casts the event reference to the expected event type (TEvent). If types match,
         * a virtual HandleEvent function for that event type is called.
         *
         * @param event The event that was triggered.
         */
        void OnEvent(const BaseEvent& event) override {
            // This function acts as the controlled, type-safe wrapper
            // Will cause undefined behavior if event is not of type TEvent
            HandleEvent(static_cast<const TEvent&>(event));
        }

        [[nodiscard]] EventType GetHandledEventType() const override {
            return TEvent::get_static_type();
        }

    protected:
        /**
         * @brief Called when an event this handler subscribed to is triggered
         *
         * @param event Reference to the event (of type TEvent) that was triggered.
         */
        virtual void HandleEvent(const TEvent& event) = 0;
    };

}