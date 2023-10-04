//
// Created by Andres Suazo
//

#include <iostream>
#include "event_layer.h"
#include "event_handler.h"
#include "event_layer_manager.h"
#include "base_event.h"
#include <thread>
#include <utility>

using namespace event_system;

class MGeneralEvent : public BaseEvent {
public:
    EVENT_CLASS_TYPE(GeneralEvent)

    explicit MGeneralEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

    [[nodiscard]] std::string get_name() const override {
        return "General Event 1";
    }

    [[nodiscard]] std::string get_sender_id() const override {
        return sender_id_;
    }

private:
    std::string sender_id_;
};

class MGeneralEventHandler : public IEventHandler<MGeneralEvent> {
    void HandleEvent(const MGeneralEvent& event) override {
        std::cout << "General event handled from layer: " << event.get_sender_id() << "\n";
    }
};

class MSampleLayer : public EventLayer {
public:

    using EventLayer::EventLayer;

    void Run() override {
        std::shared_ptr<IEventHandler<MGeneralEvent>>
                general_handler_1 = std::make_shared<MGeneralEventHandler>();

        AddEventHandler(general_handler_1);

        MGeneralEvent general_event_1{get_layer_name()};
        TriggerEvent(general_event_1);

        while (!ShouldStop()){

        }
    }

};

class MSampleLayer2 : public EventLayer {
public:

    using EventLayer::EventLayer;

    void Run() override {
        std::shared_ptr<IEventHandler<MGeneralEvent>>
                general_handler_1 = std::make_shared<MGeneralEventHandler>();

        AddEventHandler(general_handler_1);

        while (!ShouldStop()){

        }
    }

};

int main() {

    std::cout << "** Multiple Layer Event System Example **\n\n";

    EventLayerManager layer_manager{};

    // Set application layers
    std::shared_ptr<EventLayer> layer_1 = std::make_shared<MSampleLayer>("layer_1");
    std::shared_ptr<EventLayer> layer_2 = std::make_shared<MSampleLayer2>("layer_2");

    layer_manager.RegisterLayer(layer_1);
    layer_manager.RegisterLayer(layer_2);

    // Run each layer
    std::thread run_layer_1(&EventLayer::Run, layer_1);
    std::thread run_layer_2(&EventLayer::Run, layer_2);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Join all layer threads
    layer_1->Stop();
    layer_2->Stop();
    run_layer_1.join();
    run_layer_2.join();
}