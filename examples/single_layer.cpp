//
// Created by Andres Suazo
//

#include "event_layer.h"
#include "event_handler.h"
#include "base_event.h"
#include <string>
#include <iostream>

using namespace event_system;

class GeneralEvent : public BaseEvent {
public:
    EVENT_CLASS_TYPE(GeneralEvent)

    explicit GeneralEvent(std::string sender_id = "") : sender_id_(sender_id) {}

    [[nodiscard]] std::string get_name() const override {
        return "General Event 1";
    }

    [[nodiscard]] std::string get_sender_id() const override {
        return sender_id_;
    }

private:
    std::string sender_id_;
};

class SpecificEvent : public BaseEvent {
public:
    EVENT_CLASS_TYPE(SpecificEvent)

    explicit SpecificEvent(std::string sender_id = "") : sender_id_(sender_id) {}

    [[nodiscard]] std::string get_name() const override {
        return "Specific Event 1";
    }

    [[nodiscard]] std::string get_sender_id() const override {
        return sender_id_;
    }

private:
    std::string sender_id_;
};

class GeneralEventHandler : public IEventHandler<GeneralEvent> {
    void HandleEvent(const GeneralEvent& event) override {
        std::cout << "General event handled\n";
    }
};

class SampleLayer : public EventLayer {
public:

    void Run() override {
        std::shared_ptr<IEventHandler<GeneralEvent>>
                general_handler_1 = std::make_shared<GeneralEventHandler>();

        AddEventHandler(general_handler_1);

        GeneralEvent general_event_1{get_layer_name()};
        SpecificEvent specific_event_0{get_layer_name()};

        TriggerEvent(specific_event_0); // Should not emit an event
        TriggerEvent(general_event_1);
    }

};

int single_layer_driver() {

    std::cout << "** Single Layer Event System Example **\n\n";

    // Set application layers
    SampleLayer sample_layer{};
    sample_layer.Run();

    return 0;
}