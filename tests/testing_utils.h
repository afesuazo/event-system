//
// Created by Andres Suazo
//

#pragma once
#include "base_event.h"

namespace event_manager {

    class GeneralEvent : public BaseEvent {
    public:
        enum class SubType {
            GeneralSubType0,
            GeneralSubType1,
            GeneralSubType2
        };

        explicit GeneralEvent(SubType subType) : subType_(subType) {}

        [[nodiscard]] std::type_index GetType() const override {
            return typeid(this);
        }

        [[nodiscard]] int GetSubType() const override {
            return static_cast<int>(subType_);
        }

        [[nodiscard]] std::string GetName() const override {
            return "GeneralEvent";
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

        explicit SpecificEvent(SubType subType) : subType_(subType) {}

        [[nodiscard]] std::type_index GetType() const override {
            return typeid(this);
        }

        [[nodiscard]] int GetSubType() const override {
            return static_cast<int>(subType_);
        }

        [[nodiscard]] std::string GetName() const override {
            return "SpecificEvent";
        }

    private:
        SubType subType_;
    };


    class GeneralEventListener : public IEventListener<GeneralEvent> {
    public:
        bool eventTriggered = false;

        void OnEvent(GeneralEvent& event) override {
            eventTriggered = true;
            std::cout << "Received general event: " << event.GetSubType() << std::endl;
        }
    };

    class SpecificEventListener : public IEventListener<SpecificEvent> {
    public:
        bool eventTriggered = false;

        void OnEvent(SpecificEvent& event) override {
            eventTriggered = true;
            std::cout << "Received specific event: " << event.GetSubType() << std::endl;
        }
    };


}
