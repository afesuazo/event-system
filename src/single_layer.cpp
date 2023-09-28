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

    explicit GeneralEvent(SubType subType) : subType_(subType) {
        event_name = "GeneralEvent";
    }

    [[nodiscard]] int get_sub_type() const override {
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
        event_name = "SpecificEvent";
    }

    [[nodiscard]] int get_sub_type() const override {
        return static_cast<int>(subType_);
    }

private:
    SubType subType_;
};



class GeneralEventListener : public IEventListener<GeneralEvent> {
public:
    void OnEvent(const GeneralEvent& event) override {
        std::cout << "Received general event: " << event.get_sub_type() << std::endl;
    }

    void RegisterCallback(const std::function<void()>& callback) override {}

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