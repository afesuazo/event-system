//
// Created by Andres Suazo
//

#include "event_layer.h"
#include "event_listener.h"
#include "base_event.h"
#include <string>

using namespace event_system;

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

class SampleLayer1 : public EventLayer {
public:

    void Run() override {
        std::shared_ptr<EventListener<GeneralEvent>>
                general_listener_1 = std::make_shared<EventListener<GeneralEvent>>();

        AddEventListener(general_listener_1);

        GeneralEvent general_event_1{GeneralEvents::GeneralSubType1};
        SpecificEvent specific_event_0{SpecificEvents::SpecificSubType0};

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