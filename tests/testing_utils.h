//
// Created by Andres Suazo
//

#pragma once
#include <utility>

#include "base_event.h"
#include "event_manager.h"

namespace event_system {

    class TestGeneralEvent : public BaseEvent {
    public:
        EVENT_CLASS_TYPE(GeneralEvent)

        explicit TestGeneralEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

        [[nodiscard]] std::string get_name() const override {
            return "General Event 1";
        }

        [[nodiscard]] std::string get_sender_id() const override {
            return sender_id_;
        }

    private:
        std::string sender_id_;
    };

    class TestSpecificEvent : public BaseEvent {
    public:
        EVENT_CLASS_TYPE(SpecificEvent)

        explicit TestSpecificEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

        [[nodiscard]] std::string get_name() const override {
            return "Specific Event 1";
        }

        [[nodiscard]] std::string get_sender_id() const override {
            return sender_id_;
        }

    private:
        std::string sender_id_;
    };

    template <typename TEvent>
    class TestEventHandler : public IEventHandler<TEvent> {
    public:
        bool event_triggered = false;

    private:
        void HandleEvent(const TEvent& event) override {
            event_triggered = true;
        }
    };

}
