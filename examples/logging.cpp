//
// Created by Andres Suazo
//

#include "event_dispatcher.h"
#include <iostream>
#include <string>

struct LogEvent {

  enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

  LogLevel level;
  std::string message;
};

void OnLogEvent(const LogEvent &event) {
  switch (event.level) {
  case LogEvent::LogLevel::DEBUG:
    std::cout << "[DEBUG] " << event.message << std::endl;
    break;
  case LogEvent::LogLevel::INFO:
    std::cout << "[INFO] " << event.message << std::endl;
    break;
  case LogEvent::LogLevel::WARNING:
    std::cout << "[WARNING] " << event.message << std::endl;
    break;
  case LogEvent::LogLevel::ERROR:
    std::cout << "[ERROR] " << event.message << std::endl;
    break;
  }
}

auto main(int argc, char *argv[]) -> int {

  using namespace event_system;

  std::cout << "** Log Events Example **\n\n";

  EventDispatcher event_dispatcher;

  size_t callback_id = event_dispatcher.AddCallback<LogEvent>(OnLogEvent);

  event_dispatcher.Dispatch<LogEvent>(
      {LogEvent::LogLevel::DEBUG,
       "Registered handlers: " +
           std::to_string(event_dispatcher.GetHandlerCount())});

  event_dispatcher.Dispatch<LogEvent>(
      {LogEvent::LogLevel::INFO, "Dispatched info message"});

  event_dispatcher.Dispatch<LogEvent>(
      {LogEvent::LogLevel::WARNING, "Dispatched warning message"});

  event_dispatcher.Dispatch<LogEvent>(
      {LogEvent::LogLevel::ERROR, "Dispatched error message"});

  event_dispatcher.RemoveCallback<LogEvent>(callback_id);

  return 0;
}
