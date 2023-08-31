//
// Created by Andres Suazo
//

#pragma once

// TODO: Typedef event map

#include "event_listener.h"
#include <unordered_map>
#include "string"
#include "algorithm"
#include <typeindex>

namespace event_manager {

    class EventManager {
    public:
        EventManager() = default;
        ~EventManager() = default;

    /**
     * @brief Add a listener to an events subscriber list.
     *
     * @param eventName  Name of the event to subscribe to.
     * @param listener Pointer to the listener object subscribing to the event.
     *
     * If the event has not been registered previously, it will eb registered and the listener would
     * be the only subscriber at the time
     *
     */
    template<typename TEvent>
    void AddSubscriber(EventListener<TEvent>* listener) {

        // Adding a new key creates a default initialized vector
        if (!SubscriptionExists(listener)) {
            std::type_index type = typeid(TEvent);
            subscribers[type].push_back(listener);
        }

    }

    /**
     * @brief Removes a listener from an events subscriber list.
     *
     * @param eventName Name of the event to unsubscribe from.
     * @param listener Pointer to the listener object unsubscribing from the event.
     *
     * If there are no subscribers left after removing the listener, the event will be
     * removed from the registry
     *
     */
    template<typename TEvent>
    void RemoveSubscriber(EventListener<TEvent>* listener) {

        // Check if key exists to avoid creating an empty vector
        std::type_index type = typeid(TEvent); // TODO: DRY - Multiple checks for existing keys
        auto it = subscribers.find(type);

        // No key found, nothing to remove
        if (it == subscribers.end()) { return; }

        auto& listeners = it->second;
        listeners.erase(std::remove(listener), listeners.end());

        // Make sure we don't leave unused keys and empty vectors
        if (listeners.empty()){
            subscribers.erase(it);
        }

    }

    /**
     * @brief Trigger and event.
     *
     * @param eventName Name of the event to trigger.
     *
     * Iterates thought the collection of listeners subscribed to the event
     * and informs them the event has been triggered.
     *
     */
    template<typename TEvent>
    void TriggerEvent(TEvent& event) {

        // Without this check, an empty vector would be created, and we would loop over an empty container
        std::type_index type = typeid(TEvent);
        auto it = subscribers.find(type);

        if (it == subscribers.end()) { return; }

        for (auto& listener : it->second) {
            listener->OnEvent(event);
        }

    }

    /**
     * @brief Checks if the collection of subscribers contains a certain event-listener pair
     *
     * @param eventName Name of the event to check
     * @param listener Pointer to listener object in question
     * @return True if there is a listener subscribed to the event
     */
    template<typename TEvent>
    bool SubscriptionExists(EventListener<TEvent>* listener) {

        std::type_index type = typeid(TEvent);
        auto it = subscribers.find(type);
        if (it == subscribers.end()) { return false; }

        auto& listeners = it->second;
        return std::find(listeners.begin(), listeners.end(), listener) != listeners.end();

    }

    private:
    //TODO: Compare performance to list
    std::unordered_map<std::type_index, std::vector<EventListenerBase*>> subscribers;

};

}