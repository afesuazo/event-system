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
    void AddSubscriber(const std::string& eventName, EventListener* listener);

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
    void RemoveSubscriber(const std::string& eventName, EventListener* listener);

    /**
     * @brief Trigger and event.
     *
     * @param eventName Name of the event to trigger.
     *
     * Iterates thought the collection of listeners subscribed to the event
     * and informs them the event has been triggered.
     *
     */
    void TriggerEvent(const std::string& eventName);

    /**
     * @brief Checks if the collection of subscribers contains a certain event-listener pair
     *
     * @param eventName Name of the event to check
     * @param listener Pointer to listener object in question
     * @return True if there is a listener subscribed to the event
     */
    bool SubscriptionExists(const std::string& eventName, EventListener* listener);

    private:
    //TODO: Compare performance to list
    std::unordered_map<std::string, std::vector<EventListener*>> subscribersList;

};

}