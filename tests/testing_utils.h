//
// Created by Andres Suazo
//

#pragma once
#include "base_event.h"
#include "event_listener.h"
#include "event_manager.h"
#include "event_emitter.h"

namespace event_system {

    class GeneralEvent : public BaseEvent {
    public:
        enum class SubType {
            GeneralSubType0,
            GeneralSubType1,
        };

        explicit GeneralEvent(SubType sub_type) : sub_type_(sub_type) {
            event_name = "GeneralEvent";
        }

        [[nodiscard]] int get_sub_type() const override {
            return static_cast<int>(sub_type_);
        }


    private:
        SubType sub_type_;
    };

    class SpecificEvent : public BaseEvent {
    public:
        enum class SubType {
            SpecificSubType0,
            SpecificSubType1,
        };

        explicit SpecificEvent(SubType sub_type) : sub_type_(sub_type) {
            event_name = "SpecificEvent";
        }

        [[nodiscard]] int get_sub_type() const override {
            return static_cast<int>(sub_type_);
        }

    private:
        SubType sub_type_;
    };

    template<typename TEvent>
    class TestEventListener : public EventListener<TEvent> {
    public:
        bool event_triggered = false;

        void OnEvent(const TEvent& event, bool is_test) {
            event_triggered = true;
            std::cout << "Received event: " << event.get_sub_type() << std::endl;
        }
    };


    class TestEventManager :  public EventManager {
    public:
        template<typename TEvent>
        void EmitEvent(const TEvent &event, bool is_test) {

            // Without this check, an empty vector would be created, and we would loop over an empty container
            auto it = get_event_map_iterator(typeid(event));
            if (it == subscribers.end()) { return; }

            // Iterate through the collection of weak_pointers
            // If we can lock the pointer, call the OnEvent method on the listener
            // If we can't lock the pointer,

            auto& listeners = it->second;
            for (auto weak_ptr_it = listeners.begin(); weak_ptr_it != listeners.end();) {
                // Check if pointer is valid
                if (auto listener = weak_ptr_it->lock()) {
                    auto casted_listener = std::dynamic_pointer_cast<TestEventListener<TEvent>>(listener);
                    casted_listener->OnEvent(event, true);
                    ++weak_ptr_it;
                } else {
                    // Object no longer exists and should be removed from map
                    weak_ptr_it = listeners.erase(weak_ptr_it); // Return an iterator to the next weak ptr if any
                }
            }

        }
    };

    class TestEventEmitter : public EventEmitter {
    public:
        TestEventEmitter() = default;
        TestEventEmitter(const std::shared_ptr<EventManager>& event_manager) : EventEmitter(event_manager) {}
        ~TestEventEmitter() = default;

        template <typename TEvent>
        void Emit(const TEvent& event, bool is_test) {
            if (!IsAllowedEvent(event)) {
                std::cerr << "Event type not allowed.\n";
                return;
            }

            if (auto shared_manager = get_event_manager()) {
                // cast manager to test manager
                auto casted_manager = std::static_pointer_cast<TestEventManager>(shared_manager);
                casted_manager->EmitEvent(event, true);
            }
        }
    };

}
