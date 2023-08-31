//
// Created by Andres Suazo
//

#include "event_manager.h"
#include <iostream>

int main() {

    std::cout << "Event Manager!\n\n";

    event_manager::EventManager eventManager;
    event_manager::EventListener eventListener1, eventListener2;

    eventManager.AddSubscriber("Event1", &eventListener1);
    eventManager.AddSubscriber("Event1", &eventListener2);
    eventManager.AddSubscriber("Event2", &eventListener1);

    eventManager.TriggerEvent("Event1");
    eventManager.TriggerEvent("Event2");

    return 0;
}