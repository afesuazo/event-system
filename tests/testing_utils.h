//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"
#include "event_dispatcher.h"

namespace event_system {

    class TestGeneralEvent : public BaseEvent {
    public:
        EVENT_CLASS_TYPE(EventType, General)

        explicit TestGeneralEvent(std::size_t sender_id = 0) : sender_id_{sender_id} {}

        [[nodiscard]] std::string GetName() const override {
            return "General Event 1";
        }

        [[nodiscard]] std::size_t GetSenderID() const override {
            return sender_id_;
        }

    private:
        std::size_t sender_id_;
    };

    class TestSpecificEvent : public BaseEvent {
    public:
        EVENT_CLASS_TYPE(EventType, Specific)

        explicit TestSpecificEvent(std::size_t sender_id = 0) : sender_id_{sender_id} {}

        [[nodiscard]] std::string GetName() const override {
            return "Specific Event 1";
        }

        [[nodiscard]] std::size_t GetSenderID() const override {
            return sender_id_;
        }

    private:
        std::size_t sender_id_;
    };

}
