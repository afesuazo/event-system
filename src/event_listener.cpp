//
// Created by Andres Suazo
//


#include "event_listener.h"
#include <iostream>

namespace event_manager {

    void EventListener::OnEvent(const std::string &eventName) {
        std::cout << "Received event: " << eventName << "\n";
    }

}