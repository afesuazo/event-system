//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"
#include "event_manager.h"
#include <memory>
#include <string>
#include <mutex>
#include <string>
#include <atomic>
#include "event_handler.h"

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
        explicit EventLayer(std::string layer_name = "")
                : layer_name_(std::move(layer_name)) {}
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
         * is thread safe, usable when calling Run() in a separate thread.
         */
        void Stop() {
            should_stop_ = true;
        }

        /**
         * @brief Propagate and event to this layer
         *
         * This method can be called by external sources to emit an event inside this layer. The local manager is
         * given the event in order to trigger all appropriate local handlers.
         *
         * @param event The event to emit.
         */
        // For events sourced from external layers
        void OnExternalEvent(const BaseEvent& event) {
            event_manager_.EmitEvent(event);
        }

        void set_layer_manager_callback(const EventCallback& callback) {
            layer_manager_callback_ = callback;
        }

        /**
         * @brief Returns the name of the layer
         */
        std::string get_layer_name() const {
            return layer_name_;
        }

        /**
         * @brief Get the total amount of active handlers in the layer
         *
         * @returns size_t representing the total active handlers
         */
        size_t get_handler_count() const {
            return event_manager_.get_handler_count();
        }

    protected:

        /**
         * @brief Adds a handler to the local event manager
         *
         * @param handler A reference to the handler object to add.
         */
        void AddEventHandler(const std::shared_ptr<IEventHandlerBase>& handler) {
            event_manager_.AddHandler(handler);
        }

        /**
         * @brief Removes a handler from the local event manager
         *
         * @param handler A reference to the handler object to remove.
         */
        void RemoveEventHandler(const std::shared_ptr<IEventHandlerBase>& handler) {
            event_manager_.RemoveHandler(handler);
        }

        // TODO: Default to all or none?
//        void SetAllowedEvents() {
//
//        }

        /**
         * @brief Checks if the layer has should stop.
         *
         * @returns True if layer should stop.
         */
        bool ShouldStop() {
            return should_stop_;
        }

        /**
         * @brief Checks if this object is allowed to emit the given event
         *
         * @param event The event to check.
         * @returns True if the event type is allowed.
         */
        virtual bool IsAllowedEvent(const BaseEvent& event) {
            // By default, all event types are allowed
            return true;
        }

        /**
         * @brief Emit an event through the local emitter.
         *
         * The emitter will run appropriate checks and decide weather or not to emit the event and
         * if the event should reach beyond this layer.
         *
         * @param event The event to emit.
         */
        void TriggerEvent(const BaseEvent& event) {
            if (!IsAllowedEvent(event)) {
                std::cerr << "Event type not allowed.\n";
                return;
            }

            // Emit even locally and check if there is an external callback registered
            event_manager_.EmitEvent(event);
            if(layer_manager_callback_){
                layer_manager_callback_(event, layer_name_);
            }
        }

    private:
        // Points to the shared event manager
        EventManager event_manager_;
        EventCallback layer_manager_callback_;  // Callback to the LayerEventManager::OnEvent method
        std::string layer_name_;
        std::atomic<bool> should_stop_;
    };

}