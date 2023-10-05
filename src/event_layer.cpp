//
// Created by Andres Suazo
//

#include "event_layer.h"

namespace event_system {

    EventLayer::EventLayer(std::string layer_name, const EventCallback& callback) : layer_name_(std::move(layer_name)) {
        allowed_events_ = ~None;
        manager_callback_ = callback;
    }

    void EventLayer::Stop() {
        should_stop_ = true;
    }

    void EventLayer::set_layer_manager_callback(const EventCallback& callback) {
        manager_callback_ = callback;
    }

    std::string EventLayer::get_layer_name() const {
        return layer_name_;
    }

    bool EventLayer::ShouldStop() {
        return should_stop_;
    }

    bool EventLayer::IsAllowedEvent(const BaseEvent& event) const {
        return (allowed_events_ & event.get_event_type()) != 0;
    }

    void EventLayer::EmitEvent(const BaseEvent& event) {
        if (!IsAllowedEvent(event)) {
            std::cerr << "Event type not allowed.\n";
            return;
        }

        // We broadcast the event directly and don't handle it until we hear back from the event manager
        // This might introduce some latency but improves consistency and traceability
        if (manager_callback_) {
            manager_callback_(event);

        }
    }

}