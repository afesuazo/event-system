//
// Created by Andres Suazo
//

#pragma once
#include "base_event.h"

namespace event_system {

    class IEventHandler {
    public:
        virtual ~IEventHandler() = default;
        virtual void HandleEvent(const BaseEvent& event) = 0;
    };

}