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
     * @class IEventHandler
     * @brief Templated class for creating a handler of a specific event type
     *
     * @tparam TEvent The event type this handler is intended to subscribe to
     */
    template<typename TEvent>
    class EventHandler : public IEventHandler {
        static_assert(std::is_base_of<BaseEvent, TEvent>::value, "TEvent must be derive from BaseEvent");
    public:
        /**
         * @brief Overridden method to handle type-erasure. Will immediately try to call the HandleEvent method.
         *
         * Statically casts the event reference to the expected event type (TEvent). If types match,
         * a virtual HandleEvent function for that event type is called.
         *
         * @param event The event that was triggered.
         */
        void OnEvent(const TEvent& event) {
            for (auto const& callback: callbacks) {
                callback(event);
            }
        }

        int Subscribe(const Callback<TEvent>& callback)
        {
            callbacks.push_back(callback);
            // TODO: Generate a unique id for each callback
            return callbacks.size() - 1;
        }

        void Unsubscribe(int id)
        {
            // TODO: Replace with a map and generate unique ids for each callback
            // Replaces callback with an empty version in order to keep all IDs valid
            if (id >= callbacks.size()) {return;}
            callbacks[id] = [](const TEvent&){};
        }

        [[nodiscard]] EventType GetHandledEventType() const override {
            return TEvent::get_static_type();
        }

    private:
        std::vector<Callback<TEvent>> callbacks;

    };

}