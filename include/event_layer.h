//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"
#include "event_manager.h"
#include "event_emitter.h"
#include <memory>
#include <string>
#include <mutex>
#include <atomic>

namespace event_manager {

    /**
     * @class EventLayer
     * @brief Encapsulates a section of an application and its events
     *
     * The EventLayer class serves as a organizational tool. It represents a specific process or section of an application
     * that handles its own internal events and logic. This is meant to serve as a base class for an entire application section.
     */
    class EventLayer {
    public:
        EventLayer() {
            eventManager = std::make_shared<EventManager>();
            eventEmitter = EventEmitter{eventManager};
        }

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
            std::lock_guard<std::mutex> lock(mutex);
            shouldStop = true;
        }

        /**
         * @brief Propagate and event to this layer
         *
         * This method can be called by external sources to emit an event inside this layer. The local manager is
         * given the event in order to trigger all appropriate local listeners.
         *
         * @param event The event to emit.
         */
        // For events sourced from external layers
        void OnEvent(const BaseEvent& event) {
            std::cout << "here";
            eventManager->EmitEvent(event);
        }

        /**
         * @brief Returns the name of the layer
         */
        std::string GetLayerName() {
            return layerName;
        }

        /**
         * @brief Get the total amount of active listeners in the layer
         *
         * @returns size_t representing the total active listeners
         */
        size_t GetListenerCount() {
            return eventManager->GetSubscriberCount();
        }

    protected:

        /**
         * @brief Adds a listener to the local event manager
         *
         * @param listener A reference to the listener object to add.
         */
        template<typename TEvent>
        void AddListener(const std::shared_ptr<IEventListener<TEvent>>& listener) {
            eventManager->AddSubscriber(listener);
        }

        /**
         * @brief Removes a listener from the local event manager
         *
         * @param listener A reference to the listener object to remove.
         */
        template<typename TEvent>
        void RemoveListener(const std::shared_ptr<IEventListener<TEvent>>& listener) {
            eventManager->RemoveSubscriber(listener);
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
            std::lock_guard<std::mutex> lock(mutex);
            return shouldStop;
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
            eventEmitter.Emit(event);
        }

        /**
         * @brief Set the layers name.
         *
         * @param name The name of the layer.
         */
        void SetLayerName(const std::string& name) {
            layerName = name;
        }

    private:
        // Points to the shared event manager
        std::shared_ptr<EventManager> eventManager;
        // Used to emit local and global events
        EventEmitter eventEmitter;
        std::string layerName;

        std::mutex mutex;
        std::atomic<bool> shouldStop;
    };

}