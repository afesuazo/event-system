//
// Created by Andres Suazo
//

#include "base_event.h"
#include "event_layer.h"
#include "event_handler.h"
#include "event_manager.h"
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

class SGeneralEventHandler : public IEventHandler<SGeneralEvent> {
    void HandleEvent(const SGeneralEvent& event) override {
        std::cout << "General event handled\n";
    }
};

class SGeneralEventHandler2 : public IEventHandler<SGeneralEvent> {
    void HandleEvent(const SGeneralEvent& event) override {
        std::cout << "General event handled from second handler\n";
    }
};


int main_disabled() {

    std::cout << "** No Layer Event System Example **\n\n";

    EventManager event_manager{};

    std::shared_ptr<SGeneralEventHandler> handler_1 = std::make_shared<SGeneralEventHandler>();
    std::shared_ptr<SGeneralEventHandler2> handler_2 = std::make_shared<SGeneralEventHandler2>();

    event_manager.AddHandler(handler_1);
    event_manager.AddHandler(handler_2);

    SGeneralEvent general_event_1{};
    event_manager.OnEvent(general_event_1);

    return 0;
}