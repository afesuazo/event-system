//
// Created by Andres Suazo
//

#pragma once

// TODO: Typedef event map

#include "event_listener.h"
#include <unordered_map>
#include "string"

namespace event_manager {

    class EventManager {
    public:
        EventManager() = default;
        ~EventManager() = default;

        void AddSubscriber(const std::string& eventName, EventListener* listener);
        void RemoveSubscriber(const std::string& eventName, EventListener* listener);
        void TriggerEvent(const std::string& eventName);
        bool SubscriptionExists(const std::string& eventName, EventListener* listener);
    private:
        //TODO: Compare performance to list
        std::unordered_map<std::string, std::vector<EventListener*>> subscribersList;

    };

}