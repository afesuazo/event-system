//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"

namespace event_system {

    template<typename TEvent>
    using Callback = std::function<void(const TEvent& event)>;

    /**
     * @class IEventHandler
     * @brief Interface for all event handlers, intended for polymorphism.
     *
     * Provides a base interface for all event handlers. This allows us to easily create containers for
     * EventHandlers and handle multiple types of events in a uniform manner.
     *
     * @note Should not be inherited from directly, instead use IEventHandler
     */
    class IEventHandler {
    public:
        virtual ~IEventHandler() = default;

        [[nodiscard]] virtual EventType GetHandledEventType() const = 0;

    };

    /**
     * @class EventHandler
     * @brief Templated class for creating a handler of a specific event type
     *
     * @tparam TEvent The event type this handler is intended to listen for
     */
    template<typename TEvent>
    class EventHandler : public IEventHandler {
        static_assert(std::is_base_of<BaseEvent, TEvent>::value, "TEvent must be derive from BaseEvent");
    public:
        /**
         * @brief Iterates through all registered callbacks, passing the event as an argument to each one.
         *
         * @param event The event that was triggered.
         */
        void OnEvent(const TEvent& event) {
            for (auto const& callback: callbacks_) {
                callback(event);
            }
        }

        /**
         * @brief Register a callback.
         *
         * @param callback Function to be called when an event is received.
         */
        int Register(const Callback<TEvent> callback) {
            callbacks_.push_back(callback);
            // TODO: Generate a unique id for each callback
            return callbacks_.size() - 1;
        }

        /**
         * @brief Deregister a callback.
         *
         * @param callback_id Index of the callback to deregister.
         */
        void Deregister(int callback_id) {
            // TODO: Replace with a map and generate unique ids for each callback
            // Replaces callback with an empty version in order to keep all IDs valid
            if (callback_id >= callbacks_.size()) { return; }
            callbacks_[callback_id] = [](const TEvent&) {};
        }

        [[nodiscard]] EventType GetHandledEventType() const override {
            return TEvent::GetStaticType();
        }

    private:
        std::vector<Callback<TEvent>> callbacks_;
    };

}