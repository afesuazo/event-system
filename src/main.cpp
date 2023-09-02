//
// Created by Andres Suazo
//

#include "event_manager.h"
#include "event_listener.h"
#include "base_event.h"
#include <iostream>
#include <memory>
#include <string>

class GeneralEvent : public event_manager::BaseEvent {
public:

    [[nodiscard]] std::type_index GetType() const override {
        return typeid(this);
    }

    [[nodiscard]] int GetSubType() const override {
        return 5;
    }

    [[nodiscard]] std::string GetName() const override {
        return {"General"};
    }
};

class SpecificEvent : public event_manager::BaseEvent {
public:

    [[nodiscard]] std::type_index GetType() const override {
        return typeid(this);
    }

    [[nodiscard]] int GetSubType() const override {
        return 5;
    }

    [[nodiscard]] std::string GetName() const override {
        return {"Specific"};
    }
};

class GeneralEventListener : public event_manager::IEventListener<GeneralEvent> {
public:
    void OnEvent(GeneralEvent& event) override {
        std::cout << "Received general event: " << event.GetSubType() << std::endl;
    }
};

class SpecificEventListener : public event_manager::IEventListener<SpecificEvent> {
public:
    void OnEvent(SpecificEvent& event) override {
        std::cout << "Received specific event: " << event.GetSubType() << std::endl;
    }
};


int main() {

    std::cout << "Event Manager!\n\n";

    event_manager::EventManager eventManager;

    // Create shared pointers to event listeners
    std::shared_ptr<event_manager::IEventListener<GeneralEvent>>
            generalListener = std::make_shared<GeneralEventListener>();
    std::shared_ptr<event_manager::IEventListener<SpecificEvent>>
            specificListener = std::make_shared<SpecificEventListener>();

    /*
     * The compiler doesn't turn std::shared_ptr<Derived> to std::shared_ptr<Base> so we
     * need a static cast. This is done at compile time so not real performance hit
     */
    eventManager.AddSubscriber(generalListener);
    eventManager.AddSubscriber(specificListener);

    GeneralEvent sampleGeneralEvent;
    SpecificEvent sampleSpecificEvent;

    eventManager.EmitEvent(sampleGeneralEvent);
    eventManager.EmitEvent(sampleSpecificEvent);

    eventManager.RemoveSubscriber(generalListener);

    eventManager.EmitEvent(sampleGeneralEvent); // Nothing should be received by the general listener
    eventManager.EmitEvent(sampleSpecificEvent);

    return 0;
}