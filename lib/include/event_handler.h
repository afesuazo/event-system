//
// Created by Andres Suazo
//

#pragma once

#include <functional>
#include <vector>

namespace event_system {

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
     * @brief Templated class that holds a collection of callbacks for a specific event type and calls them when an event is received.
     * @tparam Args The types of the arguments that will be passed to all callbacks.
     *
     * @example EventHandler<int> handler{};
     * @example EventHandler<std::string, float> handler{};
     */
    template<typename... Args>
    class EventHandler : public IEventHandler {
        using Callback = std::function<void(const Args&...)>;


    public:
        /**
         * @brief Adds a callback.
         * @param callback Function to be called when an event is received.
         */
        size_t AddCallback(const Callback& callback) {
            static size_t id = 0;
            callbacks_[id] = callback;
            return id++;
        }

        /**
         * @brief Removes a callback.
         * @param callback_id Index of the callback to remove.
         */
        void RemoveCallback(size_t callback_id) {
            callbacks_.erase(callback_id);
        }

        /**
 * @brief Iterates through all registered callbacks, calling them with the provided arguments.
 * @param args Arguments to be passed to the callbacks.
 */
        void OnEvent(Args... args) {
            // Make a copy in case vector gets modified during iteration
            auto callbacks_copy = callbacks_;
            for (auto& [_, callback]: callbacks_copy) {
                callback(args...);
            }
        }

        [[nodiscard]] size_t GetCallbackCount() const {
            return callbacks_.size();
        }

    private:
        std::unordered_map<size_t, Callback> callbacks_;
    };

}