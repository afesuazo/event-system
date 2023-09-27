//
// Created by Andres Suazo
//

#pragma once
#include "base_event.h"
#include "event_listener.h"

namespace event_manager {

    class GeneralEvent : public BaseEvent {
    public:
        enum class SubType {
            GeneralSubType0,
            GeneralSubType1,
            GeneralSubType2
        };

        explicit GeneralEvent(SubType subType) : subType_(subType) {
            eventName = "GeneralEvent";
        }

        [[nodiscard]] int GetSubType() const override {
            return static_cast<int>(subType_);
        }


    private:
        SubType subType_;
    };

    class SpecificEvent : public BaseEvent {
    public:
        enum class SubType {
            SpecificSubType0,
            SpecificSubType1,
        };

        explicit SpecificEvent(SubType subType) : subType_(subType) {
            eventName = "SpecificEvent";
        }

        [[nodiscard]] int GetSubType() const override {
            return static_cast<int>(subType_);
        }

    private:
        SubType subType_;
    };

    class GeneralEventListener : public IEventListener<GeneralEvent> {
    public:
        bool eventTriggered = false;

        void OnEvent(const GeneralEvent& event) override {
            eventTriggered = true;
            std::cout << "Received general event: " << event.GetSubType() << std::endl;
        }

        void RegisterCallback(const std::function<void()>& callback) override {

        }

    };

    class SpecificEventListener : public IEventListener<SpecificEvent> {
    public:
        bool eventTriggered = false;

        void OnEvent(const SpecificEvent& event) override {
            eventTriggered = true;
            std::cout << "Received specific event: " << event.GetSubType() << std::endl;
        }

        void RegisterCallback(const std::function<void()>& callback) override {

        }

    };

}
