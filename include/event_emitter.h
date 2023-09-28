//
// Created by Andres Suazo
//

#pragma once
#include <iostream>
#include "event_manager.h"
#include "base_event.h"

namespace event_system {

    /**
     * @class EventEmitter
     * @brief Helper class used to emit events in an event manager object.
     *
     * The EventEmitter serves as a middleman class when emitting events. It add a mechanism for checking and
     * filtering event types that should not or are not allowed to be emitted.
     */
    class EventEmitter {
    public:

        EventEmitter() = default;

        explicit EventEmitter(const std::shared_ptr<EventManager>& manager) :
                event_manager(manager) {}

        virtual ~EventEmitter() = default;

        /**
         * @brief Emit an event on the registered event manager.
         *
         * This method first checks if the event is allowed by calling the IsAllowedEvent method before
         * relaying it to the event manager.
         *
         * @param event The event to emit.
         */
        template <typename TEvent>
        void Emit(const TEvent& event) {
            if (!IsAllowedEvent(event)) {
                std::cerr << "Event type not allowed.\n";
                return;
            }

            if (auto shared_manager = event_manager.lock()) {
                shared_manager->EmitEvent(event);
            }
        }

        std::shared_ptr<EventManager> get_event_manager() {
            return event_manager.lock();
        }

    protected:
        /**
         * @brief Checks if this object is allowed to emit the given event
         *
         * @param event The event to check.
         * @returns True if the event type is allowed.
         */
        virtual bool IsAllowedEvent(const BaseEvent& event) {
            // By default, all event types are allowed
            return true;
        }

    private:
        std::weak_ptr<EventManager> event_manager;
    };

}