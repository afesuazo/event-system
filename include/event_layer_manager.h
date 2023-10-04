//
// Created by Andres Suazo
//

#pragma once
#include "base_event.h"
#include "event_layer.h"
#include <functional>

namespace event_system {

    class EventLayerManager {
    public:
        EventLayerManager() = default;
        ~EventLayerManager() = default;

        void RegisterLayer(std::shared_ptr<EventLayer>& layer) {
            auto callback = [this](const BaseEvent& event, const std::string& sender_id) {
                this->OnEvent(event, sender_id);
            };
            layer->set_layer_manager_callback(callback);
            layers_.push_back(layer);
        }

        void RemoveLayer(std::shared_ptr<EventLayer> layer) {
            layers_.erase(std::remove_if(layers_.begin(), layers_.end(),
                                         [&layer](const std::weak_ptr<EventLayer>& layer_reference) {
                                             auto shared_layer_reference = layer_reference.lock();
                                             return shared_layer_reference && shared_layer_reference == layer;
                                         }), layers_.end());
        }

        void OnEvent(const BaseEvent& event, const std::string& sender_id) {
            for (auto it = layers_.begin(); it != layers_.end();) {
                if (auto layer = it->lock()) {
                    // Avoid sending the event back to the sender layer
                    if (layer->get_layer_name() != sender_id) {
                        layer->OnExternalEvent(event);
                    }
                    ++it;
                } else {
                    // Remove the layer if it no longer exists
                    it = layers_.erase(it);
                }
            }
        }

    private:
        std::vector<std::weak_ptr<EventLayer>> layers_;
    };


}