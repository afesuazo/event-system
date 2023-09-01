//
// Created by Andres Suazo
//

#include "event_manager.h"
#include "event_listener.h"
#include "i_event.h"
#include <iostream>

class GeneralEvent : public event_manager::IEvent {
public:
    int GetSubType() const override {
        return 5;
    }
};

class GeneralEventListener : public event_manager::IEventListener<GeneralEvent> {
public:
    void OnEvent(GeneralEvent& event) override {
        std::cout << "Received string event: " << event.GetSubType() << std::endl;
    }
};


int main() {

    std::cout << "Event Manager!\n\n";

    event_manager::EventManager eventManager;
    GeneralEventListener eventListener1, eventListener2;

    eventManager.AddSubscriber(&eventListener1);
    eventManager.AddSubscriber(&eventListener2);

    GeneralEvent sampleEvent;
    eventManager.TriggerEvent(sampleEvent);

    return 0;
}