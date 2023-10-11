//
// Created by Andres Suazo
//

#pragma once
#include <utility>

#include "base_event.h"
#include "event_dispatcher.h"

namespace event_system {

    class TestGeneralEvent : public BaseEvent {
    public:
        EVENT_CLASS_TYPE(General)

        explicit TestGeneralEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

        [[nodiscard]] std::string GetName() const override {
            return "General Event 1";
        }

        [[nodiscard]] std::string GetSenderID() const override {
            return sender_id_;
        }

    private:
        std::string sender_id_;
    };

    class TestSpecificEvent : public BaseEvent {
    public:
        EVENT_CLASS_TYPE(Specific)

        explicit TestSpecificEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

        [[nodiscard]] std::string GetName() const override {
            return "Specific Event 1";
        }

        [[nodiscard]] std::string GetSenderID() const override {
            return sender_id_;
        }

    private:
        std::string sender_id_;
    };

}
