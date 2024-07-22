#ifndef MGE_EVENTS_INPUTS
#define MGE_EVENTS_INPUTS

#include "../mgepch.hh"

namespace mge {
enum class InputState { Invalid, Release, Press, Repeat };

enum class KeyboardKey {
  Unknown,
  Space,
  Apostrophe,
  Comma,
  Minus,
  Period,
  Slash,
  K0,
  K1,
  K2,
  K3,
  K4,
  K5,
  K6,
  K7,
  K8,
  K9,
  Semicolon,
  Equal,
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  LeftBracket,
  Backslash,
  RightBracket,
  GraveAccent,
  World1,
  World2,
  Escape,
  Enter,
  Tab,
  Backspace,
  Insert,
  Delete,
  Right,
  Left,
  Down,
  Up,
  PageUp,
  PageDown,
  Home,
  End,
  CapsLock,
  ScrollLock,
  NumLock,
  PrintScreen,
  Pause,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  F16,
  F17,
  F18,
  F19,
  F20,
  F21,
  F22,
  F23,
  F24,
  F25,
  KP0,
  KP1,
  KP2,
  KP3,
  KP4,
  KP5,
  KP6,
  KP7,
  KP8,
  KP9,
  KPDecimal,
  KPDivide,
  KPMultiply,
  KPSubtract,
  KPAdd,
  KPEnter,
  KPEqual,
  LeftShift,
  LeftControl,
  LeftAlt,
  LeftSuper,
  RightShift,
  RightControl,
  RIghtAlt,
  RightSuper,
  Menu
};

enum class MouseButton { Unknown, Left, Right, Middle };

using InputModifierKeyFlags = uint8_t;

enum class InputModifierKey : uint8_t {
  Unknown = 1 << 0,
  Shift = 1 << 1,
  Control = 1 << 2,
  Alt = 1 << 3,
  Super = 1 << 4,
  CapsLock = 1 << 5,
  NumLock = 1 << 6
};

inline InputState input_state_from_glfw(int state) {
  switch (state) {
    case GLFW_RELEASE:
      return InputState::Release;
    case GLFW_PRESS:
      return InputState::Press;
    case GLFW_REPEAT:
      return InputState::Repeat;

    default:
      return InputState::Invalid;
  }
}

inline KeyboardKey keyboard_key_from_glfw(int key) {
  switch (key) {
    case GLFW_KEY_SPACE:
      return KeyboardKey::Space;
    case GLFW_KEY_APOSTROPHE:
      return KeyboardKey::Apostrophe;
    case GLFW_KEY_COMMA:
      return KeyboardKey::Comma;
    case GLFW_KEY_MINUS:
      return KeyboardKey::Minus;
    case GLFW_KEY_PERIOD:
      return KeyboardKey::Period;
    case GLFW_KEY_SLASH:
      return KeyboardKey::Slash;
    case GLFW_KEY_0:
      return KeyboardKey::K0;
    case GLFW_KEY_1:
      return KeyboardKey::K1;
    case GLFW_KEY_2:
      return KeyboardKey::K2;
    case GLFW_KEY_3:
      return KeyboardKey::K3;
    case GLFW_KEY_4:
      return KeyboardKey::K4;
    case GLFW_KEY_5:
      return KeyboardKey::K5;
    case GLFW_KEY_6:
      return KeyboardKey::K6;
    case GLFW_KEY_7:
      return KeyboardKey::K7;
    case GLFW_KEY_8:
      return KeyboardKey::K8;
    case GLFW_KEY_9:
      return KeyboardKey::K9;
    case GLFW_KEY_SEMICOLON:
      return KeyboardKey::Semicolon;
    case GLFW_KEY_EQUAL:
      return KeyboardKey::Equal;
    case GLFW_KEY_A:
      return KeyboardKey::A;
    case GLFW_KEY_B:
      return KeyboardKey::B;
    case GLFW_KEY_C:
      return KeyboardKey::C;
    case GLFW_KEY_D:
      return KeyboardKey::D;
    case GLFW_KEY_E:
      return KeyboardKey::E;
    case GLFW_KEY_F:
      return KeyboardKey::F;
    case GLFW_KEY_G:
      return KeyboardKey::G;
    case GLFW_KEY_H:
      return KeyboardKey::H;
    case GLFW_KEY_I:
      return KeyboardKey::I;
    case GLFW_KEY_J:
      return KeyboardKey::J;
    case GLFW_KEY_K:
      return KeyboardKey::K;
    case GLFW_KEY_L:
      return KeyboardKey::L;
    case GLFW_KEY_M:
      return KeyboardKey::M;
    case GLFW_KEY_N:
      return KeyboardKey::N;
    case GLFW_KEY_O:
      return KeyboardKey::O;
    case GLFW_KEY_P:
      return KeyboardKey::P;
    case GLFW_KEY_Q:
      return KeyboardKey::Q;
    case GLFW_KEY_R:
      return KeyboardKey::R;
    case GLFW_KEY_S:
      return KeyboardKey::S;
    case GLFW_KEY_T:
      return KeyboardKey::T;
    case GLFW_KEY_U:
      return KeyboardKey::U;
    case GLFW_KEY_V:
      return KeyboardKey::V;
    case GLFW_KEY_W:
      return KeyboardKey::W;
    case GLFW_KEY_X:
      return KeyboardKey::X;
    case GLFW_KEY_Y:
      return KeyboardKey::Y;
    case GLFW_KEY_Z:
      return KeyboardKey::Z;
    case GLFW_KEY_LEFT_BRACKET:
      return KeyboardKey::LeftBracket;
    case GLFW_KEY_BACKSLASH:
      return KeyboardKey::Backslash;
    case GLFW_KEY_RIGHT_BRACKET:
      return KeyboardKey::RightBracket;
    case GLFW_KEY_GRAVE_ACCENT:
      return KeyboardKey::GraveAccent;
    case GLFW_KEY_WORLD_1:
      return KeyboardKey::World1;
    case GLFW_KEY_WORLD_2:
      return KeyboardKey::World2;
    case GLFW_KEY_ESCAPE:
      return KeyboardKey::Escape;
    case GLFW_KEY_ENTER:
      return KeyboardKey::Enter;
    case GLFW_KEY_TAB:
      return KeyboardKey::Tab;
    case GLFW_KEY_BACKSPACE:
      return KeyboardKey::Backspace;
    case GLFW_KEY_INSERT:
      return KeyboardKey::Insert;
    case GLFW_KEY_DELETE:
      return KeyboardKey::Delete;
    case GLFW_KEY_RIGHT:
      return KeyboardKey::Right;
    case GLFW_KEY_LEFT:
      return KeyboardKey::Left;
    case GLFW_KEY_DOWN:
      return KeyboardKey::Down;
    case GLFW_KEY_UP:
      return KeyboardKey::Up;
    case GLFW_KEY_PAGE_UP:
      return KeyboardKey::PageUp;
    case GLFW_KEY_PAGE_DOWN:
      return KeyboardKey::PageDown;
    case GLFW_KEY_HOME:
      return KeyboardKey::Home;
    case GLFW_KEY_END:
      return KeyboardKey::End;
    case GLFW_KEY_CAPS_LOCK:
      return KeyboardKey::CapsLock;
    case GLFW_KEY_SCROLL_LOCK:
      return KeyboardKey::ScrollLock;
    case GLFW_KEY_NUM_LOCK:
      return KeyboardKey::NumLock;
    case GLFW_KEY_PRINT_SCREEN:
      return KeyboardKey::PrintScreen;
    case GLFW_KEY_PAUSE:
      return KeyboardKey::Pause;
    case GLFW_KEY_F1:
      return KeyboardKey::F1;
    case GLFW_KEY_F2:
      return KeyboardKey::F2;
    case GLFW_KEY_F3:
      return KeyboardKey::F3;
    case GLFW_KEY_F4:
      return KeyboardKey::F4;
    case GLFW_KEY_F5:
      return KeyboardKey::F5;
    case GLFW_KEY_F6:
      return KeyboardKey::F6;
    case GLFW_KEY_F7:
      return KeyboardKey::F7;
    case GLFW_KEY_F8:
      return KeyboardKey::F8;
    case GLFW_KEY_F9:
      return KeyboardKey::F9;
    case GLFW_KEY_F10:
      return KeyboardKey::F10;
    case GLFW_KEY_F11:
      return KeyboardKey::F11;
    case GLFW_KEY_F12:
      return KeyboardKey::F12;
    case GLFW_KEY_F13:
      return KeyboardKey::F13;
    case GLFW_KEY_F14:
      return KeyboardKey::F14;
    case GLFW_KEY_F15:
      return KeyboardKey::F15;
    case GLFW_KEY_F16:
      return KeyboardKey::F16;
    case GLFW_KEY_F17:
      return KeyboardKey::F17;
    case GLFW_KEY_F18:
      return KeyboardKey::F18;
    case GLFW_KEY_F19:
      return KeyboardKey::F19;
    case GLFW_KEY_F20:
      return KeyboardKey::F20;
    case GLFW_KEY_F21:
      return KeyboardKey::F21;
    case GLFW_KEY_F22:
      return KeyboardKey::F22;
    case GLFW_KEY_F23:
      return KeyboardKey::F23;
    case GLFW_KEY_F24:
      return KeyboardKey::F24;
    case GLFW_KEY_F25:
      return KeyboardKey::F25;
    case GLFW_KEY_KP_0:
      return KeyboardKey::KP0;
    case GLFW_KEY_KP_1:
      return KeyboardKey::KP1;
    case GLFW_KEY_KP_2:
      return KeyboardKey::KP2;
    case GLFW_KEY_KP_3:
      return KeyboardKey::KP3;
    case GLFW_KEY_KP_4:
      return KeyboardKey::KP4;
    case GLFW_KEY_KP_5:
      return KeyboardKey::KP5;
    case GLFW_KEY_KP_6:
      return KeyboardKey::KP6;
    case GLFW_KEY_KP_7:
      return KeyboardKey::KP7;
    case GLFW_KEY_KP_8:
      return KeyboardKey::KP8;
    case GLFW_KEY_KP_9:
      return KeyboardKey::KP9;
    case GLFW_KEY_KP_DECIMAL:
      return KeyboardKey::KPDecimal;
    case GLFW_KEY_KP_DIVIDE:
      return KeyboardKey::KPDivide;
    case GLFW_KEY_KP_MULTIPLY:
      return KeyboardKey::KPMultiply;
    case GLFW_KEY_KP_SUBTRACT:
      return KeyboardKey::KPSubtract;
    case GLFW_KEY_KP_ADD:
      return KeyboardKey::KPAdd;
    case GLFW_KEY_KP_ENTER:
      return KeyboardKey::KPEnter;
    case GLFW_KEY_KP_EQUAL:
      return KeyboardKey::KPEqual;
    case GLFW_KEY_LEFT_SHIFT:
      return KeyboardKey::LeftShift;
    case GLFW_KEY_LEFT_CONTROL:
      return KeyboardKey::LeftControl;
    case GLFW_KEY_LEFT_ALT:
      return KeyboardKey::LeftAlt;
    case GLFW_KEY_LEFT_SUPER:
      return KeyboardKey::LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT:
      return KeyboardKey::RightShift;
    case GLFW_KEY_RIGHT_CONTROL:
      return KeyboardKey::RightControl;
    case GLFW_KEY_RIGHT_ALT:
      return KeyboardKey::RIghtAlt;
    case GLFW_KEY_RIGHT_SUPER:
      return KeyboardKey::RightSuper;
    case GLFW_KEY_MENU:
      return KeyboardKey::Menu;

    default:
      return KeyboardKey::Unknown;
  }
}

inline MouseButton mouse_button_from_glfw(int button) {
  switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      return MouseButton::Left;
    case GLFW_MOUSE_BUTTON_RIGHT:
      return MouseButton::Right;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      return MouseButton::Middle;

    default:
      return MouseButton::Unknown;
  }
}

inline InputModifierKeyFlags operator|(const InputModifierKey a, const InputModifierKey b) {
  return static_cast<InputModifierKeyFlags>(a) | static_cast<InputModifierKeyFlags>(b);
}

inline InputModifierKeyFlags operator|(const InputModifierKeyFlags a, const InputModifierKey b) {
  return a | static_cast<InputModifierKeyFlags>(b);
}

inline InputModifierKeyFlags operator&(const InputModifierKeyFlags a, const InputModifierKey b) {
  return a & static_cast<InputModifierKeyFlags>(b);
}

inline InputModifierKey modifier_key_from_glfw(int mod) {
  switch (mod) {
    case GLFW_MOD_SHIFT:
      return InputModifierKey::Shift;
    case GLFW_MOD_CONTROL:
      return InputModifierKey::Control;
    case GLFW_MOD_ALT:
      return InputModifierKey::Alt;
    case GLFW_MOD_SUPER:
      return InputModifierKey::Super;
    case GLFW_MOD_CAPS_LOCK:
      return InputModifierKey::CapsLock;
    case GLFW_MOD_NUM_LOCK:
      return InputModifierKey::Shift;

    default:
      return InputModifierKey::NumLock;
  }
}

inline InputModifierKeyFlags modifier_flags_from_glfw(int glfw_mods) {
  InputModifierKeyFlags mods = 0;
  if (glfw_mods | GLFW_MOD_SHIFT) {
    mods = mods | InputModifierKey::Shift;
  }
  if (glfw_mods | GLFW_MOD_CONTROL) {
    mods = mods | InputModifierKey::Control;
  }
  if (glfw_mods | GLFW_MOD_ALT) {
    mods = mods | InputModifierKey::Alt;
  }
  if (glfw_mods | GLFW_MOD_SUPER) {
    mods = mods | InputModifierKey::Super;
  }
  if (glfw_mods | GLFW_MOD_CAPS_LOCK) {
    mods = mods | InputModifierKey::CapsLock;
  }
  if (glfw_mods | GLFW_MOD_NUM_LOCK) {
    mods = mods | InputModifierKey::NumLock;
  }

  return mods;
}

}  // namespace mge

#endif  // MGE_EVENTS_KEYBOARD