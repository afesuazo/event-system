//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"

namespace event_system {

    template<typename... Args>
    using Callback = std::function<void(const Args&...)>;

    /**
     * @class IEventHandler
     * @brief Interface for all event handlers
     *
     * Provides a base interface for all event handlers. This allows EventHandlers for
     * different event types to be stored in a single container.
     *
     * @note Should not be inherited from directly, instead use EventHandler
     */
    class IEventHandler {
    public:
        virtual ~IEventHandler() = default;
    };

    /**
     * @class EventHandler
     * @brief Templated class for creating a handler of a specific event type.
     * @tparam Args The types of the arguments that will be passed to the callbacks.
     *
     * @example EventHandler<int> handler{};
     * @example EventHandler<std::string, float> handler{};
     */
    template<typename... Args>
    class EventHandler: public IEventHandler {
    public:
        /**
         * @brief Iterates through all registered callbacks, calling them with the provided arguments.
         * @param args Arguments to be passed to the callbacks.
         */
        void OnEvent(const Args&... args) {
            auto callbacks_copy = callbacks_;  // Make a copy in case vector gets modified during iteration
            for (const auto& callback : callbacks_copy) {
                callback(args...);
            }
        }

        /**
         * @brief Register a callback.
         * @param callback Function to be called when an event is received.
         */
        int Register(const Callback<Args...>& callback) {
            callbacks_.push_back(callback);
            return callbacks_.size();
        }

        /**
         * @brief Deregister a callback.
         * @param callback_id Index of the callback to deregister.
         */
        void Deregister(int callback_id) {
            // TODO: Replace with a map and generate unique ids for each callback
            // Replaces callback with an empty version in order to keep all IDs valid
            if (callback_id > callbacks_.size()) { return; }
            callbacks_[callback_id-1] = [](const Args&...) {};
        }

        [[nodiscard]] std::size_t GetCallbackCount() const {
            return callbacks_.size();
        }

    private:
        std::vector<Callback<Args...>> callbacks_;
    };

}