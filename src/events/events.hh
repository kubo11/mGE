#ifndef MGE_EVENTS_EVENTS_HH
#define MGE_EVENTS_EVENTS_HH

#include "event_manager.hh"

#include "camera_event.hh"
#include "entity_event.hh"
#include "keyboard_event.hh"
#include "mouse_event.hh"
#include "render_mode_event.hh"
#include "tag_event.hh"
#include "window_event.hh"

namespace mge {
DeclareEventManager(CameraEvents, RenderModeEvents, WindowEvents, MouseEvents, KeyboardEvents, EntityEvents, TagEvents);
}  // namespace mge

#endif  // MGE_EVENTS_EVENTS_HH