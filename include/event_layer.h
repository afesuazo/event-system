//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"
#include "event_manager.h"
#include "event_emitter.h"

namespace event_manager {

    class EventLayer {
    public:
        explicit EventLayer() : eventEmitter(eventManager) {
            eventManager = std::make_shared<EventManager>();
        }

        virtual ~EventLayer() = default;

        virtual void Run() = 0;

        // For events sourced from external layers
        void OnEvent(BaseEvent& event) {
            eventEmitter.Emit(event);
        };


    protected:

        template<typename TEvent>
        void AddListener(const std::shared_ptr<IEventListener<TEvent>>& listener) {
            eventManager->AddSubscriber(listener);
        }

        template<typename TEvent>
        void RemoveListener(const std::shared_ptr<IEventListener<TEvent>>& listener) {
            eventManager->RemoveSubscriber(listener);
        }

        // TODO: Default to all or none?
        void SetAllowedEvents() {

        };

        // Even triggered within the layer
        void TriggerEvent(BaseEvent& event) {
            eventEmitter.Emit(event);
        };

        std::string name;

    private:
        // Points to the shared event manager
        std::shared_ptr<EventManager> eventManager;
        // Used to emit local and global events
        EventEmitter eventEmitter;
    };

}