//
// Created by Andres Suazo
//

#pragma once
#include "string"

namespace event_manager {

    class EventListener {
    public:
        EventListener() = default;
        ~EventListener() = default;
        void OnEvent(const std::string& eventName);
    };

}