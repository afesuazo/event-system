//
// Created by Andres Suazo
//

#pragma once

#include <string>
#include <cstddef>

namespace event_system {

    // Generic example events
    enum class EventType {
        None = 0,
        General =  1 << 0,     // 1
        Specific = 1 << 1,     // 2
    };

#define EVENT_CLASS_TYPE(enum_name, type) \
        static EventType GetStaticType() { return static_cast<event_system::EventType>(enum_name::type); } \
        virtual EventType GetEventType() const override { return GetStaticType(); }

#define EVENT_SUBTYPE(subtype_enum_name) \
    private: subtype_enum_name event_subtype_; \
    public: virtual subtype_enum_name GetEventSubtype() const { return event_subtype_; } \


    /**
     * @struct BaseEvent
     * @brief Base class for all events
     *
     * Provides a simple base class for all events to extend
     */
    struct BaseEvent {
        virtual ~BaseEvent() = default;

        [[nodiscard]] virtual EventType GetEventType() const = 0;

        [[nodiscard]] virtual std::string GetName() const = 0;

        [[nodiscard]] virtual std::size_t GetSenderID() const = 0;
    };

}