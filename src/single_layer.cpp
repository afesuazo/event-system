//
// Created by Andres Suazo
//

#include "event_layer.h"
#include "event_listener.h"
#include "base_event.h"
#include "event_emitter.h"
#include <string>

using namespace event_system;

class GeneralEvent : public BaseEvent {
public:
    enum class SubType {
        GeneralSubType0,
        GeneralSubType1,
    };

    explicit GeneralEvent(SubType sub_type) : sub_type_(sub_type) {
        event_name = "GeneralEvent";
    }

    [[nodiscard]] int get_sub_type() const override {
        return static_cast<int>(sub_type_);
    }


private:
    SubType sub_type_;
};

class SpecificEvent : public BaseEvent {
public:
    enum class SubType {
        SpecificSubType0,
        SpecificSubType1,
    };

    explicit SpecificEvent(SubType sub_type) : sub_type_(sub_type) {
        event_name = "SpecificEvent";
    }

    [[nodiscard]] int get_sub_type() const override {
        return static_cast<int>(sub_type_);
    }

private:
    SubType sub_type_;
};

class SampleLayer1 : public EventLayer {
public:

    void Run() override {
        std::shared_ptr<EventListener<GeneralEvent>>
                general_listener_1 = std::make_shared<EventListener<GeneralEvent>>();

        AddListener(general_listener_1);

        GeneralEvent general_event_1{GeneralEvent::SubType::GeneralSubType1};
        SpecificEvent specific_event_0{SpecificEvent::SubType::SpecificSubType0};

        TriggerEvent(specific_event_0); // Should not emit an event
        TriggerEvent(general_event_1);
    }

};

int main() {

    std::cout << "** Single Layer Event System Example **\n\n";

    // Set application layers
    SampleLayer1 sample_layer_1{};

    // Add listeners to each layer
    sample_layer_1.Run();

    return 0;
}