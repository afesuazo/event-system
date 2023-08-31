//
// Created by Andres Suazo
//

#pragma once

namespace event_manager {

    class IEvent {
    public:
        virtual ~IEvent() = default;

        // TODO: Switch to an enum
        virtual int GetType() const = 0;
    };

}