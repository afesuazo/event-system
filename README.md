# EventSystem 

[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)]()

Light and flexible messaging system to facilitate event-driven communication between different parts of an application.

## Introduction

This library was created as a lightweight alternative to Boost.Signals2 and other complex event systems. It is designed to be simple, easy to use, and flexible. In order to achieve this,
the library imposes minimal restrictions on event types and handling. 

## Features

* Simple
* Type-safe
* Loosely-coupled
* Less than 3KB in size

## Performance

### Handler

Metrics generated using Google Benchmark

```text
Event type : int
----------------------------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
----------------------------------------------------------------------------------------
Call1EventsWith1Callbacks          1.99 ns         1.97 ns    356913413
Call1EventsWith10Callbacks         11.6 ns         11.4 ns     60631783
Call1EventsWith100Callbacks         109 ns          108 ns      6540528
Call1EventsWith1KCallbacks          983 ns          969 ns       717941
```

* Performance metrics are still being measured and evaluated at the moment. The numbers above give a rough idea, however 
more accurate and extensive metrics will be added soon.

## Getting Started

### Clone and Setup

```bash
# Clone the repository
$ git clone https://github.com/afesuazo/event-system.git
$ cd event-system

# Run the build script 
// TODO
```

### Basic Usage

#### (optional) Define an event 

```cpp
struct PriceUpdateEvent {
    std::string symbol;
    double price;
};
```

#### Create event handlers 

```cpp
// Basic types 
EventHandler<int> int_handler;

// Reference types
EventHandler<const std::string&> string_event_handler;

// Custom types
EventHandler<const PriceUpdateEvent&> price_update_handler;
```

#### Add callback functions to event handlers

```cpp
void OnIntEvent(int value) {
  std::cout << "Int Event - Value: " << value << "\n";
}

void OnPriceUpdateEvent(const PriceUpdateEvent& price_update) {
  std::cout << "Price Update - Symbol: " << update.symbol << ", New Price: $" << update.price << "\n";
}

// Add callback functions to event handlers
int_handler.AddCallback(OnIntEvent);
price_update_handler.AddCallback(OnPriceUpdateEvent);

// Add lambda function as callback
string_event_handler.AddCallback([](const std::string& message) {
  std::cout << "String Event - Value: " << message << "\n";
});
```

#### Process events

```cpp 
// Int event
int_handler.OnEvent(50);
// String event
string_event_handler.OnEvent("test message");
// Custom event
price_update_handler.OnEvent({ "AAPL", 150.00 });


// Output:
// Int Event - Value: 50
// String Event - Value: test message
// Price Update - Symbol: AAPL, New Price: $150.00
```

### Using an Event Dispatcher

The event dispatcher class provides a way to manage and dispatch events of different types.

#### Create event dispatcher

```cpp
EventDispatcher event_dispatcher;
```

#### Add callback functions to event dispatcher

```cpp
// You need to specify the event type when adding a callback
event_dispatcher.AddCallback<int>(OnIntEvent);

event_dispatcher.AddCallback<const std::string&>([](const std::string& message) {
  std::cout << "String Event - Value: " << message << "\n";
});

// A unique callback id is returned when adding a callback that can be used to remove the callback later
size_t callback_id = event_dispatcher.AddCallback<PriceUpdateEvent>(OnPriceUpdateEvent);
```

#### Emit events

```cpp 
// Int event
event_dispatcher.Dispatch<int>(50);
// String event
event_dispatcher.Dispatch<const std::string&>("test message");
// Custom event
event_dispatcher.Dispatch<PriceUpdateEvent>({ "AAPL", 150.00 });


// Output:
// Int Event - Value: 50
// String Event - Value: test message
// Price Update - Symbol: AAPL, New Price: $150.00
```

<br>

## Examples 

You can take a look at example use cases in the [examples](examples) directory.

More elaborate examples will be added soon!

## To-Do List

- [ ] Add performance metrics
- [ ] Add example demo video
- [ ] Add documentation 
- [ ] Thread-safe event handling
- [ ] Add build scripts

## Contributing

Questions or comments? Feel free to add an issue!

If you come across bugs or have any ideas for improvements, 
please submit a pull request or add an issue, and I'll take a look at it as soon as I can. 

## License

This project is licensed under the terms of the MIT license. See the LICENSE file for details.
