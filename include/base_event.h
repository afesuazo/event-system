//
// Created by Andres Suazo
//

#pragma once
#include <typeindex>
#include <string>

namespace event_system {

    /**
     * @class BaseEvent
     * @brief Base class for all events
     *
     * Provides a simple base class for all events to extend. Essentially and event requires a
     * method to relay any general details about the event type/category and its name
     */
    class BaseEvent {
    public:

        virtual ~BaseEvent() = default;

        [[nodiscard]] std::type_index get_type() const {
            return typeid(this); // Class Type
        };

        [[nodiscard]] std::string get_name() const {
            return event_name;
        };

        /**
         * @brief Get the sub-type of the event
         *
         * This is a virtual method that must be implemented by any derived class. It allows greater granularity
         * withing an event type by specify a sub-type
         *
         * @returns Int representing a specific event sub-type/identifier
         * @note Currently returns an int but this will be switched to an enum less error prone code.
         */
        [[nodiscard]] virtual int get_sub_type() const = 0; // TODO: Switch to an enum
    protected:
        std::string event_name = get_type().name();
    };


}