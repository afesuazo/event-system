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

        /**
         * @brief Called when an event this listener is subscribed to gets triggered.
         *
         * @param eventName Name of the event that was triggered.
         *
         */
        void OnEvent(const std::string& eventName);
    };

}