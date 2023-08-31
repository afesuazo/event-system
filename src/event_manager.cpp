//
// Created by Andres Suazo
//

#include "event_manager.h"
#include <algorithm>

namespace event_manager {

    void EventManager::AddSubscriber(const std::string& eventName, EventListener* listener) {

        // Adding a new key creates a default initialized vector
        if (!SubscriptionExists(eventName, listener)) {
            subscribersList[eventName].push_back(listener);
        }

    }

    void EventManager::RemoveSubscriber(const std::string& eventName, EventListener* listener) {

        // Check if key exists to avoid creating an empty vector
        auto it = subscribersList.find(eventName); // TODO: DRY - Multiple checks for existing keys
        // No key found, nothing to remove
        if (it == subscribersList.end()) { return; }

        auto& listeners = it->second;
        listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());

        // Make sure we don't leave unused keys and empty vectors
        if (listeners.empty()){
            subscribersList.erase(it);
        }

    }

    void EventManager::TriggerEvent(const std::string& eventName) {

        // Without this check, an empty vector would be created and we would loop over an empty container
        auto it = subscribersList.find(eventName);
        if (it == subscribersList.end()) { return; }

        for (EventListener* listener : it->second) {
            listener->OnEvent(eventName);
        }

    }

    bool EventManager::SubscriptionExists(const std::string& eventName, EventListener* listener) {

        auto it = subscribersList.find(eventName);
        if (it == subscribersList.end()) { return false; }

        auto& listeners = it->second;
        return std::find(listeners.begin(), listeners.end(), listener) != listeners.end();

    }

}