//
// Created by Andres Suazo
//

#pragma once

#include "event_handler.h"
#include <unordered_map>
#include <algorithm>
#include <memory>

namespace event_system {

    using ref = std::reference_wrapper<const std::type_info>;


    struct hash {
        auto operator()(ref code) const
        { return code.get().hash_code(); }
    };

    struct equality {
        auto operator()(ref lhs, ref rhs) const
        { return lhs.get() == rhs.get(); }
    };

    /**
    * @brief Provides a method of communication between independent application segments through events
    *
    * Provides an object to which EventHandlers can subscribe to. When needed, the EventDispatcher
    * can trigger an event which will get relayed to all objects subscribed to said event. This class
    * allows easy communication between independent components in a program
    */
    class EventDispatcher {
        // Using weak_ptr to be able to confirm the existence of the object before use
        using HandlerUniquerPtr = std::shared_ptr<IEventHandler>;
        using EventDispatcherMap = std::unordered_map<std::reference_wrapper<const std::type_info>, HandlerUniquerPtr, hash, equality>;

    public:
        EventDispatcher() = default;

        ~EventDispatcher() = default;

        /**
         * @brief Add a IEventHandler to the event subscriber list.
         *
         * @param event_handler Pointer to the IEventHandler object subscribing to the event.
         *
         * Adds a IEventHandler to the subscriber collection tied to the event type. If the event type has not been
         * previously registered, it will be registered with the given event_handler as its only subscriber.
         */
        template<typename TEvent>
        int Subscribe(Callback<TEvent> callback) {
            int callback_id = GetDispatcher<TEvent>()->Subscribe(callback);
            return callback_id;
        }

        /**
         * @brief Removes a IEventHandler from an events subscriber list.
         *
         * @param event_handler Pointer to the IEventHandler object unsubscribing from the event.
         *
         * Removes a IEventHandler from the subscriber collection tied to the event type. If there are no handlers left
         * after removing the event_handler, the event type will be removed from the map
         */
        template<typename TEvent>
        void Unsubscribe(int callback_id) {
            GetDispatcher<TEvent>()->Unsubscribe(callback_id);
        }

        template<typename TEvent>
        void UnsubscribeAll() {
            auto eventType = TEvent::get_static_type();
            handlers_.erase(eventType);
        }

        /**
         * @brief Triggers an event.
         *
         * @param event Reference to the event object to trigger.
         *
         * Informs all registered handlers for the given event type that the event has been triggered.
         */
        template <typename TEvent>
        void Dispatch(const TEvent& event) {
            static_assert(std::is_base_of<BaseEvent, TEvent>::value, "TEvent must be derive from BaseEvent");
            GetDispatcher<TEvent>()->OnEvent(event);
        }

        /**
          * @brief Returns the total amount of event handlers registered
          *
          * @return size_t representing the number of handlers registered
          */
        [[nodiscard]] size_t GetHandlerCount() const {
            return handlers_.size();
        }

    private:
        /**
         * Returns a pointer to the specific dispatcher for the event type.
         * A specific dispatcher is first created if one does not exist.
         */
        template<typename TEvent>
        std::shared_ptr<EventHandler<TEvent>> GetDispatcher() {
            auto& specific_handler = handlers_[typeid(TEvent)];
            if (!specific_handler) {
                specific_handler.reset(new EventHandler<TEvent>());
            }
            return std::static_pointer_cast<EventHandler<TEvent>>(specific_handler);
        }
    private:
        EventDispatcherMap handlers_;
    };

}