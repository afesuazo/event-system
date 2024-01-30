set(EVENT_SYSTEM_SOURCES src/event_dispatcher.cpp)

add_library(event_system STATIC ${EVENT_SYSTEM_SOURCES})

target_include_directories(event_system PUBLIC include)