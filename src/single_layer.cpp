//
// Created by Andres Suazo
//

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

    SampleLayer1() : EventLayer() {}

    void Run() override {

        std::shared_ptr<IEventListener<GeneralEvent>>
                generalListener = std::make_shared<GeneralEventListener>();

        AddListener(generalListener);

        GeneralEvent generalEvent0{GeneralEvent::SubType::GeneralSubType0};
        TriggerEvent(generalEvent0);
    }

};

class SampleLayer2 : public EventLayer {
public:

    SampleLayer2() : EventLayer() {}

    void Run() override {
        std::shared_ptr<IEventListener<GeneralEvent>>
                generalListener2 = std::make_shared<GeneralEventListener>();

        AddListener(generalListener2);

        GeneralEvent generalEvent1{GeneralEvent::SubType::GeneralSubType1};
        SpecificEvent specificEvent0{SpecificEvent::SubType::SpecificSubType0};

        TriggerEvent(specificEvent0); // Should not emit an event
        TriggerEvent(generalEvent1);
    }

};

int main() {

    std::cout << "** Multiple Layer Event System Example **\n\n";

    // Set application layers
    SampleLayer1 sampleLayer1{};

    // Add listeners to each layer
    sampleLayer1.Run();

    return 0;
}