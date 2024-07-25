#ifndef MGE_EVENTS_KEYBOARD_EVENT
#define MGE_EVENTS_KEYBOARD_EVENT

#include "../mgepch.hh"

#include "../window.hh"
#include "event.hh"
#include "inputs.hh"

namespace mge {
enum class KeyboardEvents { KeyboardKeyUpdated };

class KeyboardKeyUpdatedEvent : public Event<KeyboardEvents> {
 public:
  KeyboardKeyUpdatedEvent(const Window& source_window, KeyboardKey key, InputState state,
                          InputModifierKeyFlags mods = 0)
      : Event<KeyboardEvents>(KeyboardEvents::KeyboardKeyUpdated, "KeyboardKeyUpdatedEvent"),
        source_window(source_window),
        key(key),
        mods(mods),
        state(state) {}
  virtual ~KeyboardKeyUpdatedEvent() = default;
  const Window& source_window;
  KeyboardKey key;
  InputModifierKeyFlags mods;
  InputState state;
};
}  // namespace mge

#endif  // MGE_EVENTS_KEYBOARD_EVENT