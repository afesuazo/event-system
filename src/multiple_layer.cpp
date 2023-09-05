//
// Created by Andres Suazo
//

#include "event_manager.h"
#include "event_layer.h"
#include "event_listener.h"
#include "base_event.h"
#include "event_emitter.h"
#include <string>

using namespace event_manager;

class GeneralEvent : public BaseEvent {
public:
    enum class SubType {
        GeneralSubType0,
        GeneralSubType1,
        GeneralSubType2
    };

    explicit GeneralEvent(SubType subType) : subType_(subType) {}

    [[nodiscard]] std::type_index GetType() const override {
        return typeid(this);
    }

    [[nodiscard]] int GetSubType() const override {
        return static_cast<int>(subType_);
    }

    [[nodiscard]] std::string GetName() const override {
        return "GeneralEvent";
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

    explicit SpecificEvent(SubType subType) : subType_(subType) {}

    [[nodiscard]] std::type_index GetType() const override {
        return typeid(this);
    }

    [[nodiscard]] int GetSubType() const override {
        return static_cast<int>(subType_);
    }

    [[nodiscard]] std::string GetName() const override {
        return "SpecificEvent";
    }

private:
    SubType subType_;
};

class GeneralEventListener : public IEventListener<GeneralEvent> {
public:
    void OnEvent(GeneralEvent& event) override {
        std::cout << "Received general event: " << event.GetSubType() << std::endl;
    }
};

class SampleLayer1 : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void EmitEvent(BaseEvent &event) override {
        // eventManager.EmitEvent(event);
        std::cout << "Sample layer 1 on event called: " << event.GetType().name() << "\n";
    }
};

class SampleLayer2 : public EventLayer {
public:

    using EventLayer :: EventLayer;

    void EmitEvent(BaseEvent &event) override {
        // eventManager.EmitEvent(event);
        std::cout << "Sample layer 1 on event called: " << event.GetType().name() << "\n";
    }
};

class GeneralEventEmitter : public EventEmitter {
public:
    using EventEmitter::EventEmitter;

    bool IsAllowedEvent(const BaseEvent& event) override {
        // Only allow KeyEvent types for this specific emitter
        return dynamic_cast<const GeneralEvent*>(&event) != nullptr;
    }
};

int main() {

    std::cout << "** Multiple Layer Event System Example **\n\n";

    // Create the event manager
    std::shared_ptr<EventManager> eventManager = std::make_shared<EventManager>();

    // Set application layers
    SampleLayer1 sampleLayer1{eventManager};
    SampleLayer2 sampleLayer2{eventManager};

    // Add listeners to each layer
    std::shared_ptr<IEventListener<GeneralEvent>>
            generalListener = std::make_shared<GeneralEventListener>();

    std::shared_ptr<IEventListener<GeneralEvent>>
            generalListener2 = std::make_shared<GeneralEventListener>();

    sampleLayer1.AddListener(generalListener);
    sampleLayer2.AddListener(generalListener2);

    // Create an emitter
    GeneralEventEmitter generalEventEmitter{eventManager};

    // Emit events
    // Used by any function in a layer
    GeneralEvent generalEvent0{GeneralEvent::SubType::GeneralSubType0};
    GeneralEvent generalEvent1{GeneralEvent::SubType::GeneralSubType1};
    SpecificEvent specificEvent0{SpecificEvent::SubType::SpecificSubType0};

    generalEventEmitter.Emit(generalEvent1);
    generalEventEmitter.Emit(specificEvent0); // Should not emit an event
    generalEventEmitter.Emit(generalEvent0);


    return 0;
}