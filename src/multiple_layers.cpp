//
// Created by Andres Suazo
//

#include <iostream>
#include "event_layer.h"
#include "event_handler.h"
#include "base_event.h"
#include <thread>
#include <utility>

using namespace event_system;

enum class MGeneralEvents {
    GeneralSubType0,
    GeneralSubType1,
};

class MGeneralEvent : public BaseEvent {
public:
    explicit MGeneralEvent(LayerId layer_id, MGeneralEvents sub_type) : BaseEvent(std::move(layer_id)), sub_type_(sub_type) {
        event_name_ = "GeneralEvent";
    }

    [[nodiscard]] MGeneralEvents get_sub_type() const {
        return sub_type_;
    }

private:
    MGeneralEvents sub_type_;
};

class MGeneralEventHandler : public IEventHandler<MGeneralEvent> {
    void HandleEvent(const MGeneralEvent& event) override {
        std::cout << "General event handled in layer: " << event.get_sender_id() << "\n";
    }
};

class MSampleLayer : public EventLayer {
public:

    using EventLayer::EventLayer;

    void Run() override {
        std::shared_ptr<IEventHandler<MGeneralEvent>>
                general_handler_1 = std::make_shared<MGeneralEventHandler>();

        AddEventHandler(general_handler_1);

        MGeneralEvent general_event_1{get_layer_name(), MGeneralEvents::GeneralSubType1};
        TriggerEvent(general_event_1);
    }

};


int main() {

    std::cout << "** Multiple Layer Event System Example **\n\n";

    // Set application layers
    MSampleLayer layer_1{"layer_1"};
    MSampleLayer layer_2{"layer_2"};

    // Run each layer
    std::thread run_layer_1(&EventLayer::Run, &layer_1);
    std::thread run_layer_2(&EventLayer::Run, &layer_2);

    // Join all layer threads
    layer_1.Stop();
    layer_2.Stop();
    run_layer_1.join();
    run_layer_2.join();
}