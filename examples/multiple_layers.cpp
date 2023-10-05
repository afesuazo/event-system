//
// Created by Andres Suazo
//

#include <iostream>
#include "event_layer.h"
#include "event_handler.h"
#include "event_manager.h"
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

class MSampleLayer : public EventLayer, public IEventHandler<MGeneralEvent> {
public:

    using EventLayer::EventLayer;

    void HandleEvent(const MGeneralEvent& event) override {
        std::cout << "General event handled from layer: " << event.get_sender_id() << "\n";
    }

    void Run() override {
        MGeneralEvent general_event_1{get_layer_name()};
        EmitEvent(general_event_1);

        while (!ShouldStop()) {

        }
    }

};

class MSampleLayer2 : public EventLayer, public IEventHandler<MGeneralEvent> {
public:

    using EventLayer::EventLayer;

    void HandleEvent(const MGeneralEvent& event) override {
        std::cout << "General event handled from layer: " << event.get_sender_id() << "\n";
    }

    void Run() override {
        while (!ShouldStop()) {

        }
    }

};

int main() {

    std::cout << "** Multiple Layer Event System Example **\n\n";

    EventManager event_manager{};

    auto callback = [&event_manager](const BaseEvent& event) {
        event_manager.OnEvent(event);
    };

    // Set application layers
    std::shared_ptr<MSampleLayer> layer_1 = std::make_shared<MSampleLayer>("layer_1", callback);
    std::shared_ptr<MSampleLayer2> layer_2 = std::make_shared<MSampleLayer2>("layer_2", callback);

    event_manager.AddHandler(layer_1);
    event_manager.AddHandler(layer_2);

    // Run each layer
    std::thread run_layer_1(&EventLayer::Run, layer_1);
    std::thread run_layer_2(&EventLayer::Run, layer_2);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Join all layer threads
    layer_1->Stop();
    layer_2->Stop();
    run_layer_1.join();
    run_layer_2.join();

    return 0;
}