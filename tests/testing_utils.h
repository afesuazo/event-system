//
// Created by Andres Suazo
//

#pragma once
#include "base_event.h"
#include "event_manager.h"
#include "event_emitter.h"

namespace event_system {

    enum class GeneralEvents {
        GeneralSubType0,
        GeneralSubType1,
    };

    enum class SpecificEvents {
        SpecificSubType0,
        SpecificSubType1,
    };

    class GeneralEvent : public BaseEvent {
    public:
        explicit GeneralEvent(GeneralEvents sub_type) : sub_type_(sub_type) {
            event_name_ = "GeneralEvent";
        }

        [[nodiscard]] GeneralEvents get_sub_type() const {
            return sub_type_;
        }

    private:
        GeneralEvents sub_type_;
    };

    class SpecificEvent : public BaseEvent {
    public:
        explicit SpecificEvent(SpecificEvents sub_type) : sub_type_(sub_type) {
            event_name_ = "SpecificEvent";
        }

        [[nodiscard]] SpecificEvents get_sub_type() const {
            return sub_type_;
        }

    private:
        SpecificEvents sub_type_;
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
