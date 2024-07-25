#include "events_application.hh"
#include "events_layer.hh"

EventsApplication::EventsApplication() {
  m_event_manager = EventManager::create(*m_timer);
  push_layer(std::move(std::make_unique<EventsLayer>()));
}