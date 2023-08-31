//
// Created by Andres Suazo
//

#pragma once
#include "string"

namespace event_manager {

    class EventListener {
    public:
        void OnEvent(const std::string& eventName);
    };

}