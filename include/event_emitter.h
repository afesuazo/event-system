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
                     const std::shared_ptr<EventManager>& external_manager_)
                : event_manager_(manager), external_event_manager_(external_manager_) {}

        ~EventEmitter() = default;

        /**
         * @brief Emit an event on the registered event manager and parent application manager.
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

            auto shared_manager = event_manager_.lock();
            auto shared_external_manager = external_event_manager_.lock();

            // Layer with no event manager can still emit
            // Layer with no external event manager can be used if events don't need to propagate
            // If both are missing, emitter does nothing
            if (!shared_manager && !shared_external_manager) {
                std::cerr << "Both EventManager and External Event Handler are not available or unset.\n";
                return;
            }

            if (shared_manager) {
                try {
                    shared_manager->EmitEvent(event);
                } catch (const std::exception& e) {
                    std::cerr << "Exception caught when emitting event to EventManager: " << e.what() << '\n';
                }
            }

            // Send event to parent application
            if (shared_external_manager) {
                try {
                    shared_external_manager->EmitEvent(event);
                } catch (const std::exception& e) {
                    std::cerr << "Exception caught when emitting event to External Event Handler: " << e.what() << '\n';
                }
            }
        }

        void set_event_manager(const std::shared_ptr<EventManager>& manager) {
            event_manager_ = manager;
        }

        void set_external_manager(const std::shared_ptr<EventManager>& external_manager_) {
            external_event_manager_ = external_manager_;
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
        std::weak_ptr<EventManager> external_event_manager_;  // Parent application manager
    };

}