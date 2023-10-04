//
// Created by Andres Suazo
//

#include "event_layer_manager.h"

namespace event_system {

    void EventLayerManager::RegisterLayer(std::shared_ptr<EventLayer>& layer) {
        auto callback = [this](const BaseEvent& event, const std::string& sender_id) {
            this->OnEvent(event, sender_id);
        };
        layer->set_layer_manager_callback(callback);
        layers_.push_back(layer);
    }

    void EventLayerManager::RemoveLayer(std::shared_ptr<EventLayer> layer) {
        layers_.erase(std::remove_if(layers_.begin(), layers_.end(),
                                     [&layer](const std::weak_ptr<EventLayer>& layer_reference) {
                                         auto shared_layer_reference = layer_reference.lock();
                                         return shared_layer_reference && shared_layer_reference == layer;
                                     }), layers_.end());
    }

    void EventLayerManager::OnEvent(const event_system::BaseEvent& event, const std::string& sender_id) {
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

}