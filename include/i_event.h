//
// Created by Andres Suazo
//

#pragma once

namespace event_manager {

    /**
     * @class IEvent
     * @brief Interface for all events
     *
     * Provides a simple interface for all events to implement. Essentially and event requires a
     * method to relay any general details about the event type/category
     */
    class IEvent {
    public:
        virtual ~IEvent() = default;

        /**
         * @brief Get the sub-type of the event
         *
         * This is a virtual method that must be implemented by any derived class. It allows greater granularity
         * withing an event type by specify a sub-type
         *
         * @returns Int representing a specific event sub-type/identifier
         * @note Currently returns an int but this will be switched to an enum less error prone code.
         */
        [[nodiscard]] virtual int GetSubType() const = 0; // TODO: Switch to an enum
    };

}