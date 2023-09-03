//
// Created by Andres Suazo
//

#pragma once

#include "base_event.h"

namespace event_manager {

    class EventLayer {
    public:
        explicit EventLayer(std::shared_ptr<EventManager>& eventManager) : eventManager(eventManager) {}

        virtual ~EventLayer() = default;

        template<typename TEvent>
        void AddListener(const std::shared_ptr<IEventListener<TEvent>>& listener) {
            if (auto sharedManager = eventManager.lock()) {
                // Register listener
                sharedManager->AddSubscriber(listener);
            }
        }

        template<typename TEvent>
        void RemoveListener(const std::shared_ptr<IEventListener<TEvent>>& listener) {
            if (auto sharedManager = eventManager.lock()) {
                // Register listener
                sharedManager->RemoveSubscriber(listener);
            }
        }

        // Triggers an event on the local event manager
        virtual void OnEvent(BaseEvent& event) = 0;

    protected:
        // Points to the shared event manager
        std::weak_ptr<EventManager> eventManager;
        std::string name;
    };

}