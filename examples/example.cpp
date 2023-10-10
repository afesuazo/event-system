//
// Created by Andres Suazo
//

#include "base_event.h"
#include "event_manager.h"
#include <string>
#include <iostream>

using namespace event_system;

class GeneralEvent : public BaseEvent {
public:
    EVENT_CLASS_TYPE(General)

    explicit GeneralEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

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
    EVENT_CLASS_TYPE(Specific)

    explicit SpecificEvent(std::string sender_id = "") : sender_id_(std::move(sender_id)) {}

    [[nodiscard]] std::string get_name() const override {
        return "Specific Event 1";
    }

    [[nodiscard]] std::string get_sender_id() const override {
        return sender_id_;
    }

private:
    std::string sender_id_;
};

void GeneralEventHandler(const GeneralEvent& event) {
    std::cout << "General event handled\n";
    std::cout << "Sender: " << event.get_name() << "\n";
}

void SpecificEventHandler(const SpecificEvent& event) {
    std::cout << "Specific event handled\n";
    std::cout << "Sender: " << event.get_name() << "\n";
}


int main() {

    std::cout << "** Event System Example **\n\n";

    EventDispatcher event_dispatcher;

    event_dispatcher.Subscribe<GeneralEvent>(GeneralEventHandler);
    event_dispatcher.Subscribe<SpecificEvent>(SpecificEventHandler);

    event_dispatcher.Dispatch(GeneralEvent{"sender_1"});
    event_dispatcher.Dispatch(SpecificEvent{"sender_1"});

    return 0;
}