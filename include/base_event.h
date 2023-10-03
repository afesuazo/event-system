//
// Created by Andres Suazo
//

#pragma once
#include <typeindex>
#include <string>
#include <utility>

namespace event_system {

    using LayerId = std::string;

    /**
     * @class BaseEvent
     * @brief Base class for all events
     *
     * Provides a simple base class for all events to extend. Essentially and event requires a
     * method to relay any general details about the event type/category and its name
     */
    class BaseEvent {
    public:

        explicit BaseEvent(LayerId layer_id = "")
                : layer_id_(std::move(layer_id)) {}

        virtual ~BaseEvent() = default;

        [[nodiscard]] std::type_index get_type() const {
            return typeid(*this); // Class Type
        };

        [[nodiscard]] std::string get_name() const {
            return event_name_;
        };

        [[nodiscard]] LayerId get_sender_id() const {
            return layer_id_;
        }

    protected:
        std::string event_name_ = get_type().name();
        LayerId layer_id_;  // ID of the layer that generated the event

    };


}