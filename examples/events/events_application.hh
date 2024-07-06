#ifndef MGE_EVENTS_APPLICATION
#define MGE_EVENTS_APPLICATION

#include "mge.hh"

#include "events.hh"

class EventsApplication : public mge::Application {
 public:
  EventsApplication();

 protected:
  std::shared_ptr<EventManager> m_event_manager = nullptr;
};

#endif  // MGE_EVENTS_APPLICATION