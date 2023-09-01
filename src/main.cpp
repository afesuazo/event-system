//
// Created by Andres Suazo
//

#include "event_manager.h"
#include "event_listener.h"
#include "i_event.h"
#include <iostream>
#include <memory>

class GeneralEvent : public event_manager::IEvent {
public:
    [[nodiscard]] int GetSubType() const override {
        return 5;
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
    auto eventListener1 = std::make_shared<GeneralEventListener>();
    auto eventListener2 = std::make_shared<GeneralEventListener>();

    /*
     * The compiler doesn't turn std::shared_ptr<Derived> to std::shared_ptr<Base> so we
     * need a static cast. This is done at compile time so not real performance hit
     */
    eventManager.AddSubscriber(std::static_pointer_cast<event_manager::IEventListener<GeneralEvent>>(eventListener1));
    eventManager.AddSubscriber(std::static_pointer_cast<event_manager::IEventListener<GeneralEvent>>(eventListener2));

    GeneralEvent sampleEvent;
    eventManager.TriggerEvent(sampleEvent);

    return 0;
}