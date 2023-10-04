//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"
#include "event_manager.h"
#include "event_handler.h"
#include <memory>
#include <string>
#include <atomic>
#include <iostream>

namespace event_system {

    using EventCallback = std::function<void(const BaseEvent& event, std::string sender_id)>;

    /**
     * @class EventLayer
     * @brief Encapsulates a section of an application and its events
     *
     * The EventLayer class serves as a organizational tool. It represents a specific process or section of an application
     * that handles its own internal events and logic. This is meant to serve as a base class for an entire application section.
     */
    class EventLayer {
    public:
        explicit EventLayer(std::string layer_name = "");

        virtual ~EventLayer() = default;

        /**
         * @brief The main logic for a specific application layer
         *
         * This method is the equivalent of the main entry function for a program. It has to be
         * overridden by any derived layer created.
         */
        virtual void Run() = 0;

        /**
         * @brief Set the stop flag to true
         *
         * The stop flag can be used to terminate the Run method if running in a loop. This method
         * is thread safe, helpful when calling Run() in a separate thread.
         */
        void Stop();

        /**
         * @brief Propagate and event to this layer
         *
         * @param event The event to emit.
         *
         * This method can be called by external sources to emit an event inside this layer. The event manager is
         * given the event in order to trigger all appropriate local handlers.
         */
        void OnExternalEvent(const BaseEvent& event);

        /**
         * @brief set a callback to communicate local events to ane external source
         */
        void set_layer_manager_callback(const EventCallback& callback);

        /**
         * @brief Returns the name of the layer
         */
        std::string get_layer_name() const;

        /**
         * @brief Get the total amount of active handlers in the layer
         *
         * @returns size_t representing the total active handlers
         */
        size_t get_handler_count() const;

        /**
         * @brief Set which events are allowed to be emitted within this layer
         *
         * @param event_mask Integer representing the event or events that should be allowed
         */
        void set_allowed_events(int event_mask) {
            allowed_events_ = event_mask;
        }

        /**
         * @brief Checks if this object is allowed to emit the given event type
         *
         * @param event The event to check.
         * @returns True if the event type is allowed.
         */
         bool IsAllowedEvent(const BaseEvent& event) const;

    protected:

        /**
         * @brief Adds a handler to the local event manager
         *
         * @param handler A reference to the handler object to add.
         */
        void AddEventHandler(const std::shared_ptr<IEventHandlerBase>& handler);

        /**
         * @brief Removes a handler from the local event manager
         *
         * @param handler A reference to the handler object to remove.
         */
        void RemoveEventHandler(const std::shared_ptr<IEventHandlerBase>& handler);

        /**
         * @brief Checks if the layer has should stop.
         *
         * @returns True if layer should stop.
         */
        bool ShouldStop();

        /**
         * @brief Emit an event within the layer
         *
         * @param event The event to emit.
         *
         * The layer will run appropriate checks and decide weather or not to emit the event and
         * if the event should reach beyond this layer by using the external source callback if set.
         */
        void TriggerEvent(const BaseEvent& event);

    private:
        EventManager event_manager_;
        EventCallback layer_manager_callback_;  // Callback to the LayerEventManager::OnEvent method
        std::string layer_name_;
        std::atomic<bool> should_stop_;
        int allowed_events_;
    };

}