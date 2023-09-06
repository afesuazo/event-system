//
// Created by Andres Suazo
//

#pragma once
#include <iostream>
#include "event_manager.h"
#include "base_event.h"

namespace event_manager {

    class EventEmitter {
    public:

        EventEmitter() = default;

        explicit EventEmitter(const std::shared_ptr<EventManager>& eventManager) :
        eventManager(eventManager) {}

        virtual ~EventEmitter() = default;

        template <typename TEvent>
        void Emit(TEvent& event) {
            if (!IsAllowedEvent(event)) {
                std::cerr << "Event type not allowed.\n";
                return;
            }

            if (auto sharedManager = eventManager.lock()) {
                sharedManager->EmitEvent(event);
            }
        }

    protected:
        virtual bool IsAllowedEvent(const BaseEvent& event) {
            // By default, all event types are allowed
            return true;
        }

    private:
        std::weak_ptr<EventManager> eventManager;
    };

}