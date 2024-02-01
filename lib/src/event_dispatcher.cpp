
#include "event_dispatcher.h"

namespace event_system {

[[nodiscard]] std::size_t EventDispatcher::GetHandlerCount() const {
  return handlers_.size();
}

} // namespace event_system
