#include "events_application.hh"
#include "events_layer.hh"

EventsApplication::EventsApplication() {
  m_event_manager = EventManager::create();
  push_layer(std::move(std::make_unique<EventsLayer>()));
}