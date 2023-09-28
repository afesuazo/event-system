//
// Created by Andres Suazo
//

#pragma once
#include "base_event.h"
#include "event_listener.h"
#include "event_manager.h"
#include "event_emitter.h"

namespace event_manager {

    class GeneralEvent : public BaseEvent {
    public:
        enum class SubType {
            GeneralSubType0,
            GeneralSubType1,
        };

        explicit GeneralEvent(SubType subType) : subType_(subType) {
            eventName = "GeneralEvent";
        }

        [[nodiscard]] int GetSubType() const override {
            return static_cast<int>(subType_);
        }


    private:
        SubType subType_;
    };

    class SpecificEvent : public BaseEvent {
    public:
        enum class SubType {
            SpecificSubType0,
            SpecificSubType1,
        };

        explicit SpecificEvent(SubType subType) : subType_(subType) {
            eventName = "SpecificEvent";
        }

        [[nodiscard]] int GetSubType() const override {
            return static_cast<int>(subType_);
        }

    private:
        SubType subType_;
    };

    template<typename TEvent>
    class TestEventListener : public EventListener<TEvent> {
    public:
        bool eventTriggered = false;

        void OnEvent(const TEvent& event, bool is_test) {
            eventTriggered = true;
            std::cout << "Received event: " << event.GetSubType() << std::endl;
        }
    };


    class TestEventManager :  public EventManager {
    public:
        template<typename TEvent>
        void EmitEvent(const TEvent &event, bool is_test) {

            // Without this check, an empty vector would be created, and we would loop over an empty container
            auto it = GetEventMapIterator(typeid(event));
            if (it == subscribers.end()) { return; }

            // Iterate through the collection of weak_pointers
            // If we can lock the pointer, call the OnEvent method on the listener
            // If we can't lock the pointer,

            auto& listeners = it->second;
            for (auto weakPtrIt = listeners.begin(); weakPtrIt != listeners.end();) {
                // Check if pointer is valid
                if (auto listener = weakPtrIt->lock()) {
                    auto casted_listener = std::dynamic_pointer_cast<TestEventListener<TEvent>>(listener);
                    casted_listener->OnEvent(event, true);
                    ++weakPtrIt;
                } else {
                    // Object no longer exists and should be removed from map
                    weakPtrIt = listeners.erase(weakPtrIt); // Return an iterator to the next weak ptr if any
                }
            }

        }
    };

    class TestEventEmitter : public EventEmitter {
    public:
        TestEventEmitter() = default;
        TestEventEmitter(const std::shared_ptr<EventManager>& eventManager) : EventEmitter(eventManager) {}
        ~TestEventEmitter() = default;

        template <typename TEvent>
        void Emit(const TEvent& event, bool is_test) {
            if (!IsAllowedEvent(event)) {
                std::cerr << "Event type not allowed.\n";
                return;
            }

            if (auto sharedManager = get_event_manager()) {
                // cast manager to test manager
                auto casted_manager = std::static_pointer_cast<TestEventManager>(sharedManager);
                casted_manager->EmitEvent(event, true);
            }
        }
    };


}
