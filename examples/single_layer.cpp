//
// Created by Andres Suazo
//

#include "base_event.h"
#include "event_layer.h"
#include "event_handler.h"
#include <string>
#include <iostream>
#include <utility>

using namespace event_system;

class SGeneralEvent : public BaseEvent {
public:
    EVENT_CLASS_TYPE(GeneralEvent)

    explicit SGeneralEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

    [[nodiscard]] std::string get_name() const override {
        return "General Event 1";
    }

    [[nodiscard]] std::string get_sender_id() const override {
        return sender_id_;
    }

private:
    std::string sender_id_;
};

class SSpecificEvent : public BaseEvent {
public:
    EVENT_CLASS_TYPE(SpecificEvent)

    explicit SSpecificEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

    [[nodiscard]] std::string get_name() const override {
        return "Specific Event 1";
    }

    [[nodiscard]] std::string get_sender_id() const override {
        return sender_id_;
    }

private:
    std::string sender_id_;
};

class GeneralEventHandler : public IEventHandler<SGeneralEvent> {
    void HandleEvent(const SGeneralEvent& event) override {
        std::cout << "General event handled\n";
    }
};

class SampleLayer : public EventLayer {
public:

    void Run() override {
        std::shared_ptr<IEventHandler<SGeneralEvent>>
                general_handler_1 = std::make_shared<GeneralEventHandler>();

        AddEventHandler(general_handler_1);

        SGeneralEvent general_event_1{get_layer_name()};
        SSpecificEvent specific_event_0{get_layer_name()};

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