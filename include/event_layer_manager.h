//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"
#include "event_layer.h"

namespace event_system {

    /**
     * @brief Provides a method of communication between independent EventLayers in an application
     *
     * Provides an object to which EventLayers can be registered with. This allows information to be shared
     * between layers through events. When an event is raised inside a layer, the EventLayerManager object is informed
     * and the event is broadcast to all other application layers. This helps keep proper encapsulation.
     */
    class EventLayerManager {
    public:
        EventLayerManager() = default;

        ~EventLayerManager() = default;

        /**
         * @brief Adds a reference to the EventLayer to the collection
         *
         * @param layer Pointer to the EventLayer object.
         *
         * Adds a reference to the EventLayer to the collection and registers a callback that enables the layer to
         * relay triggered events to the EventLayerManager object
         */
        void RegisterLayer(std::shared_ptr<EventLayer>& layer);

        /**
         * @brief Removes the reference to the EventLayer stored in the collection
         *
         * @param layer Pointer to the EventLayer object.
         */
        void RemoveLayer(std::shared_ptr<EventLayer> layer);

        /**
         * @brief Relays an incoming event to all layers except the sender
         *
         * @param event Event that was triggered by a layer.
         * @param sender_id String representing the name of the layer that emitted the event.
         *
         *  Called by a layer when an event has to be broadcast to the entire application. This method relays said event
         *  to all registered layers except for the sending layer.
         */
        void OnEvent(const BaseEvent& event, const std::string& sender_id);

    private:
        std::vector<std::weak_ptr<EventLayer>> layers_;
    };


}