#include "events_application.hh"

int main() {
  auto app = EventsApplication();
  app.run();
  app.terminate();
  return 0;
}