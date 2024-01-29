//
// Created by Andres Suazo
//

#include "base_event.h"
#include "event_dispatcher.h"
#include <string>
#include <iostream>

using namespace event_system;

class GeneralEvent : public BaseEvent {
public:
    EVENT_CLASS_TYPE(EventType, General)

    explicit GeneralEvent(std::size_t sender_id = 0) : sender_id_{sender_id} {}

    [[nodiscard]] std::string GetName() const override {
        return "General Event 1";
    }

    [[nodiscard]] std::size_t GetSenderID() const override {
        return sender_id_;
    }

private:
    std::size_t sender_id_;
};

class SpecificEvent : public BaseEvent {
public:
    EVENT_CLASS_TYPE(EventType, Specific)

    explicit SpecificEvent(std::size_t sender_id = 0) : sender_id_{sender_id} {}

    [[nodiscard]] std::string GetName() const override {
        return "Specific Event 1";
    }

    [[nodiscard]] std::size_t GetSenderID() const override {
        return sender_id_;
    }

private:
    std::size_t sender_id_;
};

void GeneralEventHandler(const GeneralEvent& event) {
    std::cout << "General event handled\n";
    std::cout << "Sender: " << event.GetName() << "\n";
}

void SpecificEventHandler(const SpecificEvent& event) {
    std::cout << "Specific event handled\n";
    std::cout << "Sender: " << event.GetName() << "\n";
}


int main() {

    std::cout << "** Event System Example **\n\n";

    EventDispatcher event_dispatcher;

    event_dispatcher.Register<GeneralEvent>(GeneralEventHandler);
    event_dispatcher.Register<SpecificEvent>(SpecificEventHandler);

    event_dispatcher.Dispatch(GeneralEvent{1});
    event_dispatcher.Dispatch(SpecificEvent{1});

    return 0;
}