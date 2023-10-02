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
     * filtering event types that should not or are not allowed to be emitted. It also separates the logic of
     * emitting event locally within a layer vs letting the event reach other layers
     */
    class EventEmitter {
    public:

        EventEmitter() = default;

        EventEmitter(const std::shared_ptr<EventManager>& manager,
                     const std::shared_ptr<IEventHandler<BaseEvent>>& central_handler)
                : event_manager_(manager), parent_application_event_handler_(central_handler) {}

        explicit EventEmitter(const std::shared_ptr<EventManager>& manager) :
                event_manager_(manager) {}

        ~EventEmitter() = default;

        /**
         * @brief Emit an event on the registered event manager and parent application handler.
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

            if (auto shared_manager = event_manager_.lock()) {
                shared_manager->EmitEvent(event);
            }

            // Send event to parent application
            if (auto shared_central_handler = parent_application_event_handler_.lock()) {
                shared_central_handler->OnEvent(event);
            }
        }

        std::shared_ptr<EventManager> get_event_manager() {
            return event_manager_.lock();
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
        std::weak_ptr<EventManager> event_manager_;
        std::weak_ptr<IEventHandler<BaseEvent>> parent_application_event_handler_;  // Central event handler
    };

}