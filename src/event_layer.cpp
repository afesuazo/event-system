//
// Created by Andres Suazo
//

#include "event_layer.h"

namespace event_system {

    EventLayer::EventLayer(std::string layer_name) : layer_name_(std::move(layer_name)) {}

    void EventLayer::Stop() {
        should_stop_ = true;
    }

    void EventLayer::OnExternalEvent(const BaseEvent& event) {
        event_manager_.EmitEvent(event);
    }

    void EventLayer::set_layer_manager_callback(const EventCallback& callback) {
        layer_manager_callback_ = callback;
    }

    std::string EventLayer::get_layer_name() const {
        return layer_name_;
    }

    size_t EventLayer::get_handler_count() const {
        return event_manager_.get_handler_count();
    }

    void EventLayer::AddEventHandler(const std::shared_ptr<IEventHandlerBase>& handler) {
        event_manager_.AddHandler(handler);
    }

    void EventLayer::RemoveEventHandler(const std::shared_ptr<IEventHandlerBase>& handler) {
        event_manager_.RemoveHandler(handler);
    }

    bool EventLayer::ShouldStop() {
        return should_stop_;
    }

    bool EventLayer::IsAllowedEvent(const BaseEvent& event) {
        // By default, all event types are allowed
        return true;
    }

    void EventLayer::TriggerEvent(const BaseEvent& event) {
        if (!IsAllowedEvent(event)) {
            std::cerr << "Event type not allowed.\n";
            return;
        }

        // Emit even locally and check if there is an external callback registered
        event_manager_.EmitEvent(event);
        if (layer_manager_callback_) {
            layer_manager_callback_(event, layer_name_);

        }
    }

}