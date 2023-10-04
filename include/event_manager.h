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
#include <string>

namespace event_system {

    /**
    * @brief Provides a method of communication between independent components
    *
    * Provides an object to which EventHandlers can subscribe to. When needed, the EventManager
    * can trigger an event which will get relayed to all objects subscribed to said event. This class
    * allows easy communication between independent components in a program
    */
    class EventManager {
        // Using weak_ptr to be able to confirm the existence of the object before use
        using HandlerSharedPtr = std::shared_ptr<IEventHandlerBase>;
        using HandlerWeakPtr = std::weak_ptr<IEventHandlerBase>;
        using EventHandlerMap = std::unordered_map<EventType, std::vector<HandlerWeakPtr>>;

    public:
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
        void AddHandler(const HandlerSharedPtr& event_handler) {
#ifdef ENABLE_SAFETY_CHECKS
            if (!HandlerExists(event_handler)) {
#endif
                // Adding a new key creates a default initialized vector
                subscribers_[event_handler->GetHandledEventType()].push_back(event_handler);
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
        void RemoveHandler(const HandlerSharedPtr& event_handler) {
            // Check if key exists to avoid creating an empty vector
            auto it = subscribers_.find(event_handler->GetHandledEventType());
            // No key found, nothing to remove
            if (it == subscribers_.end()) { return; }

            auto &handlers = it->second;
            it->second.erase(std::remove_if(handlers.begin(), handlers.end(), [&event_handler](
                    const HandlerWeakPtr &handler_weak_ptr) {
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
        void EmitEvent(const BaseEvent& event) {

            // Without this check, an empty vector would be created, and we would loop over an empty container
            auto it = subscribers_.find(event.get_event_type());
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
        [[nodiscard]] bool HandlerExists(const HandlerSharedPtr& handler) const {

            // get_event_map_iterator() not used to keep this as const method
            auto it = subscribers_.find(handler->GetHandledEventType());
            if (it == subscribers_.end()) { return false; }

            auto &handlers = it->second;
            // Using lambda for comparison between weak_ptr and shared_ptr
            return std::any_of(
                    handlers.begin(),
                    handlers.end(),
                    [&handler](const HandlerWeakPtr &handler_weak_ptr) {
                        return handler_weak_ptr.lock() == handler;
                    });
        }

        [[nodiscard]] size_t get_handler_count() const {
            size_t count{0};
            for (const auto& event_type : subscribers_) {
                count += event_type.second.size();
            }
            return count;
        }

        [[nodiscard]] size_t get_handler_count(EventType event_type) const {
            auto it = subscribers_.find(event_type);
            if (it == subscribers_.end()) { return 0; }

            return it->second.size();
        }

    protected:
        //TODO: Compare performance to list
        EventHandlerMap subscribers_;
    };

}