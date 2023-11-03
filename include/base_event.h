//
// Created by Andres Suazo
//

#pragma once

#include <string>

namespace event_system {

    enum EventType {
        None                  = 0,
        General               = 1 << 0,   // 1
        Specific              = 1 << 1,   // 2
        DataEvent             = 1 << 2,   // 4
        UIEvent               = 1 << 3,   // 4
    };

#define EVENT_CLASS_TYPE(type) \
    static EventType GetStaticType() { return EventType::type; } \
    virtual EventType GetEventType() const override { return GetStaticType(); }


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

        [[nodiscard]] virtual EventType GetEventType() const = 0;

        [[nodiscard]] virtual std::string GetName() const = 0;

        [[nodiscard]] virtual std::string GetSenderID() const = 0;
    };


}