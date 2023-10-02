//
// Created by Andres Suazo
//

#define ENABLE_SAFETY_CHECKS

#pragma once
#include <unordered_map>
#include "algorithm"
#include <typeindex>
#include <memory>
#include <iostream>
#include "event_handler.h"

namespace event_system {

    /**
    * @brief Provides a method of communication between independent components
    *
    * Provides an object to which EventHandlers can subscribe to. When needed, the EventManager
    * can trigger an event which will get relayed to all objects subscribed to said event. This class
    * allows easy communication between independent components in a program
    */
    class EventManager {
    public:
        // Using weak_ptr to be able to confirm the existence of the object before use
        using EventSubscriberMap = std::unordered_map<std::type_index, std::vector<std::weak_ptr<IEventHandlerBase>>>;

        EventManager() = default;
        ~EventManager() = default;

        /**
         * @brief Add a IEventHandler to the event subscriber list.
         *
         * @tparam TEvent The type of the event to subscribe to.
         * @param event_handler Pointer to the IEventHandler object subscribing to the event.
         *
         * Adds a IEventHandler to the subscriber list tied to the event type. If the event type has not been
         * previously registered, it will be registered with the given event_handler as its only subscriber.
         */
        template<typename TEvent>
        void AddSubscriber(const std::shared_ptr<IEventHandler<TEvent>>& event_handler) {

#ifdef ENABLE_SAFETY_CHECKS
            if (!SubscriptionExists(event_handler)) {
#endif
                // Adding a new key creates a default initialized vector
                subscribers_[typeid(TEvent)].push_back(event_handler);
#ifdef ENABLE_SAFETY_CHECKS
            }
#endif
        }

        /**
         * @brief Removes a IEventHandler from an events subscriber list.
         *
         * @tparam TEvent The type of the event to unsubscribe from.
         * @param event_handler Pointer to the IEventHandler object unsubscribing from the event.
         *
         * Removes a IEventHandler from the subscriber list tied to the event type. If there are no subscribers_ left
         * after removing the event_handler, the event will be removed from the map
         */
        template<typename TEvent>
        void RemoveSubscriber(const std::shared_ptr<IEventHandler<TEvent>>& event_handler) {

            // Check if key exists to avoid creating an empty vector
            auto it = subscribers_.find(typeid(TEvent));
            // No key found, nothing to remove
            if (it == subscribers_.end()) { return; }

            auto &handlers = it->second;
            it->second.erase(std::remove_if(handlers.begin(), handlers.end(), [&event_handler](
                    const std::weak_ptr<IEventHandlerBase> &handler_weak_ptr) {
                auto shared_ptr = handler_weak_ptr.lock();
                // Also check if pointer is still valid
                return !shared_ptr || shared_ptr == event_handler;
            }), handlers.end());

#ifdef ENABLE_SAFETY_CHECKS
            // Make sure we don't leave unused keys and empty vectors
            if (handlers.empty()) {
                subscribers_.erase(it);
            }
#endif
        }

        /**
         * @brief Triggers and event.
         *
         * @tparam TEvent The type of the event to trigger.
         * @param event Reference to the event object to trigger.
         *
         * Informs all subscribers_ for the given event type that the event has been triggered.
         */
        template<typename TEvent>
        void EmitEvent(const TEvent &event) {

            // Without this check, an empty vector would be created, and we would loop over an empty container
            auto it = subscribers_.find(typeid(TEvent));
            if (it == subscribers_.end()) { return; }

            // Iterate through the collection of weak_pointers
            // If we can lock the pointer, call the OnEvent method on the handler
            auto& handlers = it->second;
            for (auto weak_ptr_it = handlers.begin(); weak_ptr_it != handlers.end();) {
                // Check if pointer is valid
                if (auto handler = weak_ptr_it->lock()) {
                    handler->OnEvent(event);
                    ++weak_ptr_it;
                } else {
                    // Object no longer exists and should be removed from map
                    weak_ptr_it = handlers.erase(weak_ptr_it); // Return an iterator to the next weak ptr if any
                }
            }

        }

        /**
         * @brief Checks if an IEventHandler subscription exists.
         *
         * @tparam TEvent The type of the event to check.
         * @param handler Pointer to handler object in question.
         * @return True if there is a handler subscribed to the event type
         */
        template<typename TEvent>
        bool SubscriptionExists(const std::shared_ptr<IEventHandler<TEvent>>& handler) const {

            // get_event_map_iterator() not used to keep this as const method
            auto it = subscribers_.find(typeid(TEvent));
            if (it == subscribers_.end()) { return false; }

            auto &handlers = it->second;
            // Using lambda for comparison between weak_ptr and shared_ptr
            return std::any_of(
                    handlers.begin(),
                    handlers.end(),
                    [&handler](const std::weak_ptr<IEventHandlerBase> &handler_weak_ptr) {
                        return handler_weak_ptr.lock() == handler;
                    });
        }

        [[nodiscard]] size_t get_subscriber_count() const {
            size_t count{0};
            for (const auto& event_type : subscribers_) {
                count += event_type.second.size();
            }
            return count;
        }

        template<typename TEvent>
        [[nodiscard]] size_t get_subscriber_count() const {
            auto it = subscribers_.find(typeid(TEvent));
            if (it == subscribers_.end()) { return 0; }

            return it->second.size();
        }

    protected:
        //TODO: Compare performance to list
        EventSubscriberMap subscribers_;

    };

}