//
// Created by Andres Suazo
//

#pragma once
#include "string"
#include "i_event.h"

namespace event_manager {

    class EventListenerBase {
    public:
        virtual ~EventListenerBase() = default;
        virtual void OnEvent(IEvent& event) = 0;
    };

    template<typename TEvent>
    class EventListener : public EventListenerBase {
        static_assert(std::is_base_of<IEvent, TEvent>::value, "TEvent must be be derived from IEvent");
    public:

        void OnEvent(IEvent& event) override {
            if (typeid(event) == typeid(TEvent)) {
                OnEvent(dynamic_cast<TEvent&>(event));
            }
        }

        /**
         * @brief Called when an event this listener is subscribed to gets triggered.
         *
         * @param eventName Name of the event that was triggered.
         */
        virtual void OnEvent(TEvent& event) = 0;
    };

}