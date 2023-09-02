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

class GeneralEventListener : public event_manager::IEventListener<GeneralEvent> {
public:
    void OnEvent(GeneralEvent& event) override {
        std::cout << "Received string event: " << event.GetSubType() << std::endl;
    }
};


int main() {

    std::cout << "Event Manager!\n\n";

    event_manager::EventManager eventManager;

    // Create shared pointers to event listeners
    std::shared_ptr<event_manager::IEventListener<GeneralEvent>> eventListener1 = std::make_shared<GeneralEventListener>();
    auto eventListener2 = std::make_shared<GeneralEventListener>();

    /*
     * The compiler doesn't turn std::shared_ptr<Derived> to std::shared_ptr<Base> so we
     * need a static cast. This is done at compile time so not real performance hit
     */
    eventManager.AddSubscriber(eventListener1);

    GeneralEvent sampleEvent;
    eventManager.EmitEvent(sampleEvent);

    eventManager.RemoveSubscriber(eventListener1);

    eventManager.EmitEvent(sampleEvent);

    return 0;
}