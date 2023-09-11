//
// Created by Andres Suazo
//

#define ENABLE_SAFETY_CHECKS

#pragma once
#include "event_listener.h"
#include <unordered_map>
#include "algorithm"
#include <typeindex>
#include <memory>
#include <iostream>

namespace event_manager {

    /**
    * @brief Provides a method of communication between independent components
    *
    * Provides an object to which EventListeners can subscribe to. When needed, the EventManager
    * can trigger an event which will get relayed to all objects subscribed to said event. This class
    * allows easy communication between independent components in a program
    */
    class EventManager {
    public:
        // Using weak_ptr to be able to confirm the existence of the object before use
        using EventSubscriberMap = std::unordered_map<std::type_index, std::vector<std::weak_ptr<IEventListenerBase>>>;

        EventManager() = default;
        ~EventManager() = default;

        /**
         * @brief Add a listener to the event subscriber list.
         *
         * @tparam TEvent The type of the event to subscribe to.
         * @param listener Pointer to the listener object subscribing to the event.
         *
         * Adds a listener to the subscriber list tied to the event type. If the event type has not been
         * previously registered, it will be registered with the given listener as its only subscriber.
         */
        template<typename TEvent>
        void AddSubscriber(const std::shared_ptr<IEventListener<TEvent>>& listener) {

#ifdef ENABLE_SAFETY_CHECKS
            if (!SubscriptionExists(listener)) {
#endif
                // Adding a new key creates a default initialized vector
                subscribers[typeid(TEvent)].push_back(listener);
#ifdef ENABLE_SAFETY_CHECKS
            }
#endif
        }

        /**
         * @brief Removes a listener from an events subscriber list.
         *
         * @tparam TEvent The type of the event to unsubscribe from.
         * @param listener Pointer to the listener object unsubscribing from the event.
         *
         * Removes a listener from the subscriber list tied to the event type. If there are no subscribers left
         * after removing the listener, the event will be removed from the map
         */
        template<typename TEvent>
        void RemoveSubscriber(const std::shared_ptr<IEventListener<TEvent>>& listener) {

            // Check if key exists to avoid creating an empty vector
            auto it = GetEventMapIterator(typeid(TEvent));
            // No key found, nothing to remove
            if (it == subscribers.end()) { return; }

            auto &listeners = it->second;
            it->second.erase(std::remove_if(listeners.begin(), listeners.end(), [&listener](
                    const std::weak_ptr<IEventListenerBase> &listenerWeakPtr) {
                auto sharedPtr = listenerWeakPtr.lock();
                // Also check if pointer is still valid
                return !sharedPtr || sharedPtr == listener;
            }), listeners.end());

#ifdef ENABLE_SAFETY_CHECKS
            // Make sure we don't leave unused keys and empty vectors
            if (listeners.empty()) {
                subscribers.erase(it);
            }
#endif
        }

        /**
         * @brief Triggers and event.
         *
         * @tparam TEvent The type of the event to trigger.
         * @param event Reference to the event object to trigger.
         *
         * Informs all subscribers for the given event type that the event has been triggered.
         */
        template<typename TEvent>
        void EmitEvent(const TEvent &event) {

            // Without this check, an empty vector would be created, and we would loop over an empty container
            auto it = GetEventMapIterator(typeid(event));
            if (it == subscribers.end()) { return; }

            // Iterate through the collection of weak_pointers
            // If we can lock the pointer, call the OnEvent method on the listener
            // If we can't lock the pointer,

            auto& listeners = it->second;
            for (auto weakPtrIt = listeners.begin(); weakPtrIt != listeners.end();) {
                // Check if pointer is valid
                if (auto listener = weakPtrIt->lock()) {
                    listener->OnEvent(static_cast<const BaseEvent &>(event));
                    ++weakPtrIt;
                } else {
                    // Object no longer exists and should be removed from map
                    weakPtrIt = listeners.erase(weakPtrIt); // Return an iterator to the next weak ptr if any
                }
            }

        }

        /**
         * @brief Checks if an event-listener subscription exists.
         *
         * @tparam TEvent The type of the event to check.
         * @param listener Pointer to listener object in question.
         * @return True if there is a listener subscribed to the event type
         */
        template<typename TEvent>
        bool SubscriptionExists(const std::shared_ptr<IEventListener<TEvent>>& listener) const {

            // GetEventMapIterator() not used to keep this as const method
            auto it = subscribers.find(typeid(TEvent));
            if (it == subscribers.end()) { return false; }

            auto &listeners = it->second;

            // Using lambda for comparison between weak_ptr and shared_ptr
            return std::any_of(
                    listeners.begin(),
                    listeners.end(),
                    [&listener](const std::weak_ptr<IEventListenerBase> &listenerWeakPtr) {
                        return listenerWeakPtr.lock() == listener;
                    });
        }

        [[nodiscard]] size_t GetSubscriberCount() const {
            int count{0};
            for (const auto& eventType : subscribers) {
                count += eventType.second.size();
            }
            return count;
        }

        template<typename TEvent>
        [[nodiscard]] size_t GetSubscriberCount() const {
            auto it = subscribers.find(typeid(TEvent));
            if (it == subscribers.end()) { return 0; }

            return it->second.size();
        }

    private:
        /**
         * @brief Checks if an event type is part of the internal map
         *
         * @param type The type index of the event type.
         * @return An iterator to the event type or end() if none is found.
         */
        // Marked as nodiscard as return value should be used immediately
        [[nodiscard]]
        EventSubscriberMap::iterator GetEventMapIterator (const std::type_index& type) {
            auto it = subscribers.find(type);
            if (it != subscribers.end()) {
                return it;
            }
            return subscribers.end();
        }

        //TODO: Compare performance to list
        EventSubscriberMap subscribers;

    };

}