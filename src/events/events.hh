#ifndef MGE_EVENTS_EVENTS_HH
#define MGE_EVENTS_EVENTS_HH

#include "event_manager.hh"

#include "camera_event.hh"
#include "render_mode_event.hh"
#include "window_event.hh"

namespace mge {
#define MGE_EVENTS CameraEvents, RenderModeEvents, WindowEvents

DECLARE_EVENT_MANAGER(MGE_EVENTS);
}  // namespace mge

#endif  // MGE_EVENTS_EVENTS_HH