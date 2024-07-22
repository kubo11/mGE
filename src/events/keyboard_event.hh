#ifndef MGE_EVENTS_KEYBOARD_EVENT
#define MGE_EVENTS_KEYBOARD_EVENT

#include "../mgepch.hh"

#include "../window.hh"
#include "event.hh"
#include "inputs.hh"

namespace mge {
enum class KeyboardEvents { KeyboardKeyPressed };

class KeyboardKeyPressedEvent : public Event<KeyboardEvents> {
 public:
  KeyboardKeyPressedEvent(const Window& source_window, KeyboardKey key, InputModifierKeyFlags mods = 0)
      : Event<KeyboardEvents>(KeyboardEvents::KeyboardKeyPressed, "KeyboardKeyPressedEvent"),
        source_window(source_window),
        key(key),
        mods(mods) {}
  virtual ~KeyboardKeyPressedEvent() = default;
  const Window& source_window;
  KeyboardKey key;
  InputModifierKeyFlags mods;
};
}  // namespace mge

#endif  // MGE_EVENTS_KEYBOARD_EVENT