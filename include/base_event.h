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
            return typeid(*this); // Class Type
        };

        [[nodiscard]] std::string get_name() const {
            return event_name_;
        };
    protected:
        std::string event_name_ = get_type().name();
    };


}