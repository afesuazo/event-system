//
// Created by Andres Suazo
//

#pragma once

#include <string>

namespace event_system {

    enum EventType {
        None                  = 0,
        GeneralEvent          = 1 << 0,   // 1
        SpecificEvent         = 1 << 1,   // 2
    };

#define EVENT_CLASS_TYPE(type) \
    static EventType get_static_type() { return EventType::type; } \
    virtual EventType get_event_type() const override { return get_static_type(); }


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

        [[nodiscard]] virtual EventType get_event_type() const = 0;

        [[nodiscard]] virtual std::string get_name() const = 0;

        [[nodiscard]] virtual std::string get_sender_id() const = 0;
    };


}