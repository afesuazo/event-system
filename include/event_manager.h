//
// Created by Andres Suazo
//

#define ENABLE_SAFETY_CHECKS

#pragma once

#include "event_handler.h"
#include <unordered_map>
#include <algorithm>
#include <memory>

namespace event_system {

    /**
    * @brief Provides a method of communication between independent components through events
    *
    * Provides an object to which EventHandlers can subscribe to. When needed, the EventManager
    * can trigger an event which will get relayed to all objects subscribed to said event. This class
    * allows easy communication between independent components in a program
    */
    class EventManager {
        // Using weak_ptr to be able to confirm the existence of the object before use
        using HandlerSharedPtr = std::shared_ptr<IEventHandlerBase>;
        using HandlerWeakPtr = std::weak_ptr<IEventHandlerBase>;
        using EventHandlerMap = std::unordered_map<EventType, std::vector<HandlerWeakPtr>>;

    public:
        EventManager() = default;

        ~EventManager() = default;

        /**
         * @brief Add a IEventHandler to the event subscriber list.
         *
         * @param event_handler Pointer to the IEventHandler object subscribing to the event.
         *
         * Adds a IEventHandler to the subscriber collection tied to the event type. If the event type has not been
         * previously registered, it will be registered with the given event_handler as its only subscriber.
         */
        void AddHandler(const HandlerSharedPtr& event_handler);

        /**
         * @brief Removes a IEventHandler from an events subscriber list.
         *
         * @param event_handler Pointer to the IEventHandler object unsubscribing from the event.
         *
         * Removes a IEventHandler from the subscriber collection tied to the event type. If there are no handlers left
         * after removing the event_handler, the event type will be removed from the map
         */
        void RemoveHandler(const HandlerSharedPtr& event_handler);

        /**
         * @brief Triggers and event.
         *
         * @param event Reference to the event object to trigger.
         *
         * Informs all registered handlers for the given event type that the event has been triggered.
         */
        void EmitEvent(const BaseEvent& event);

        /**
         * @brief Checks if an IEventHandler subscription exists.
         *
         * @param handler Pointer to handler object in question.
         * @return True if the handler is registered for its event type
         */
        [[nodiscard]] bool HandlerExists(const HandlerSharedPtr& handler) const;

        /**
          * @brief Returns the total amount of event handlers registered
          *
          * @return size_t representing the number of handlers registered
          */
        [[nodiscard]] size_t get_handler_count() const;

        /**
          * @brief Returns the total amount of event handlers registered to a specific event
          *
          * @return size_t representing the number of handlers registered
          */
        [[nodiscard]] size_t get_handler_count(EventType event_type) const;

    protected:
        //TODO: Compare performance to list
        EventHandlerMap subscribers_;
    };

}