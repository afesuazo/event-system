//
// Created by Andres Suazo
//

#include "event_manager.h"

namespace event_system {

    void EventManager::AddHandler(const event_system::EventManager::HandlerSharedPtr& event_handler) {
#ifdef ENABLE_SAFETY_CHECKS
        if (!HandlerExists(event_handler)) {
#endif
            // Adding a new key creates a default initialized vector
            subscribers_[event_handler->GetHandledEventType()].push_back(event_handler);
#ifdef ENABLE_SAFETY_CHECKS
        }
#endif
    }

    void EventManager::RemoveHandler(const event_system::EventManager::HandlerSharedPtr& event_handler) {
        // Check if key exists to avoid creating an empty vector
        auto it = subscribers_.find(event_handler->GetHandledEventType());
        // No key found, nothing to remove
        if (it == subscribers_.end()) { return; }

        auto& handlers = it->second;
        it->second.erase(std::remove_if(handlers.begin(), handlers.end(), [&event_handler](
                const HandlerWeakPtr& handler_weak_ptr) {
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

    void EventManager::OnEvent(const event_system::BaseEvent& event) {
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

    bool EventManager::HandlerExists(const event_system::EventManager::HandlerSharedPtr& handler) const {
        // get_event_map_iterator() not used to keep this as const method
        auto it = subscribers_.find(handler->GetHandledEventType());
        if (it == subscribers_.end()) { return false; }

        auto& handlers = it->second;
        // Using lambda for comparison between weak_ptr and shared_ptr
        return std::any_of(
                handlers.begin(),
                handlers.end(),
                [&handler](const HandlerWeakPtr& handler_weak_ptr) {
                    return handler_weak_ptr.lock() == handler;
                });
    }

    size_t EventManager::get_handler_count() const {
        size_t count{0};
        for (const auto& event_type: subscribers_) {
            count += event_type.second.size();
        }
        return count;
    }

    size_t EventManager::get_handler_count(event_system::EventType event_type) const {
        auto it = subscribers_.find(event_type);
        if (it == subscribers_.end()) { return 0; }

        return it->second.size();
    }

}