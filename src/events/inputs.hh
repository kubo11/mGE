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
  RightAlt,
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
      return KeyboardKey::RightAlt;
    case GLFW_KEY_RIGHT_SUPER:
      return KeyboardKey::RightSuper;
    case GLFW_KEY_MENU:
      return KeyboardKey::Menu;

    default:
      return KeyboardKey::Unknown;
  }
}

inline int keyboard_key_to_glfw(KeyboardKey key) {
  switch (key) {
    case KeyboardKey::Space:
      return GLFW_KEY_SPACE;
    case KeyboardKey::Apostrophe:
      return GLFW_KEY_APOSTROPHE;
    case KeyboardKey::Comma:
      return GLFW_KEY_COMMA;
    case KeyboardKey::Minus:
      return GLFW_KEY_MINUS;
    case KeyboardKey::Period:
      return GLFW_KEY_PERIOD;
    case KeyboardKey::Slash:
      return GLFW_KEY_SLASH;
    case KeyboardKey::K0:
      return GLFW_KEY_0;
    case KeyboardKey::K1:
      return GLFW_KEY_1;
    case KeyboardKey::K2:
      return GLFW_KEY_2;
    case KeyboardKey::K3:
      return GLFW_KEY_3;
    case KeyboardKey::K4:
      return GLFW_KEY_4;
    case KeyboardKey::K5:
      return GLFW_KEY_5;
    case KeyboardKey::K6:
      return GLFW_KEY_6;
    case KeyboardKey::K7:
      return GLFW_KEY_7;
    case KeyboardKey::K8:
      return GLFW_KEY_8;
    case KeyboardKey::K9:
      return GLFW_KEY_9;
    case KeyboardKey::Semicolon:
      return GLFW_KEY_SEMICOLON;
    case KeyboardKey::Equal:
      return GLFW_KEY_EQUAL;
    case KeyboardKey::A:
      return GLFW_KEY_A;
    case KeyboardKey::B:
      return GLFW_KEY_B;
    case KeyboardKey::C:
      return GLFW_KEY_C;
    case KeyboardKey::D:
      return GLFW_KEY_D;
    case KeyboardKey::E:
      return GLFW_KEY_E;
    case KeyboardKey::F:
      return GLFW_KEY_F;
    case KeyboardKey::G:
      return GLFW_KEY_G;
    case KeyboardKey::H:
      return GLFW_KEY_H;
    case KeyboardKey::I:
      return GLFW_KEY_I;
    case KeyboardKey::J:
      return GLFW_KEY_J;
    case KeyboardKey::K:
      return GLFW_KEY_K;
    case KeyboardKey::L:
      return GLFW_KEY_L;
    case KeyboardKey::M:
      return GLFW_KEY_M;
    case KeyboardKey::N:
      return GLFW_KEY_N;
    case KeyboardKey::O:
      return GLFW_KEY_O;
    case KeyboardKey::P:
      return GLFW_KEY_P;
    case KeyboardKey::Q:
      return GLFW_KEY_Q;
    case KeyboardKey::R:
      return GLFW_KEY_R;
    case KeyboardKey::S:
      return GLFW_KEY_S;
    case KeyboardKey::T:
      return GLFW_KEY_T;
    case KeyboardKey::U:
      return GLFW_KEY_U;
    case KeyboardKey::V:
      return GLFW_KEY_V;
    case KeyboardKey::W:
      return GLFW_KEY_W;
    case KeyboardKey::X:
      return GLFW_KEY_X;
    case KeyboardKey::Y:
      return GLFW_KEY_Y;
    case KeyboardKey::Z:
      return GLFW_KEY_Z;
    case KeyboardKey::LeftBracket:
      return GLFW_KEY_LEFT_BRACKET;
    case KeyboardKey::Backslash:
      return GLFW_KEY_BACKSLASH;
    case KeyboardKey::RightBracket:
      return GLFW_KEY_RIGHT_BRACKET;
    case KeyboardKey::GraveAccent:
      return GLFW_KEY_GRAVE_ACCENT;
    case KeyboardKey::World1:
      return GLFW_KEY_WORLD_1;
    case KeyboardKey::World2:
      return GLFW_KEY_WORLD_2;
    case KeyboardKey::Escape:
      return GLFW_KEY_ESCAPE;
    case KeyboardKey::Enter:
      return GLFW_KEY_ENTER;
    case KeyboardKey::Tab:
      return GLFW_KEY_TAB;
    case KeyboardKey::Backspace:
      return GLFW_KEY_BACKSPACE;
    case KeyboardKey::Insert:
      return GLFW_KEY_INSERT;
    case KeyboardKey::Delete:
      return GLFW_KEY_DELETE;
    case KeyboardKey::Right:
      return GLFW_KEY_RIGHT;
    case KeyboardKey::Left:
      return GLFW_KEY_LEFT;
    case KeyboardKey::Down:
      return GLFW_KEY_DOWN;
    case KeyboardKey::Up:
      return GLFW_KEY_UP;
    case KeyboardKey::PageUp:
      return GLFW_KEY_PAGE_UP;
    case KeyboardKey::PageDown:
      return GLFW_KEY_PAGE_DOWN;
    case KeyboardKey::Home:
      return GLFW_KEY_HOME;
    case KeyboardKey::End:
      return GLFW_KEY_END;
    case KeyboardKey::CapsLock:
      return GLFW_KEY_CAPS_LOCK;
    case KeyboardKey::ScrollLock:
      return GLFW_KEY_SCROLL_LOCK;
    case KeyboardKey::NumLock:
      return GLFW_KEY_NUM_LOCK;
    case KeyboardKey::PrintScreen:
      return GLFW_KEY_PRINT_SCREEN;
    case KeyboardKey::Pause:
      return GLFW_KEY_PAUSE;
    case KeyboardKey::F1:
      return GLFW_KEY_F1;
    case KeyboardKey::F2:
      return GLFW_KEY_F2;
    case KeyboardKey::F3:
      return GLFW_KEY_F3;
    case KeyboardKey::F4:
      return GLFW_KEY_F4;
    case KeyboardKey::F5:
      return GLFW_KEY_F5;
    case KeyboardKey::F6:
      return GLFW_KEY_F6;
    case KeyboardKey::F7:
      return GLFW_KEY_F7;
    case KeyboardKey::F8:
      return GLFW_KEY_F8;
    case KeyboardKey::F9:
      return GLFW_KEY_F9;
    case KeyboardKey::F10:
      return GLFW_KEY_F10;
    case KeyboardKey::F11:
      return GLFW_KEY_F11;
    case KeyboardKey::F12:
      return GLFW_KEY_F12;
    case KeyboardKey::F13:
      return GLFW_KEY_F13;
    case KeyboardKey::F14:
      return GLFW_KEY_F14;
    case KeyboardKey::F15:
      return GLFW_KEY_F15;
    case KeyboardKey::F16:
      return GLFW_KEY_F16;
    case KeyboardKey::F17:
      return GLFW_KEY_F17;
    case KeyboardKey::F18:
      return GLFW_KEY_F18;
    case KeyboardKey::F19:
      return GLFW_KEY_F19;
    case KeyboardKey::F20:
      return GLFW_KEY_F20;
    case KeyboardKey::F21:
      return GLFW_KEY_F21;
    case KeyboardKey::F22:
      return GLFW_KEY_F22;
    case KeyboardKey::F23:
      return GLFW_KEY_F23;
    case KeyboardKey::F24:
      return GLFW_KEY_F24;
    case KeyboardKey::F25:
      return GLFW_KEY_F25;
    case KeyboardKey::KP0:
      return GLFW_KEY_KP_0;
    case KeyboardKey::KP1:
      return GLFW_KEY_KP_1;
    case KeyboardKey::KP2:
      return GLFW_KEY_KP_2;
    case KeyboardKey::KP3:
      return GLFW_KEY_KP_3;
    case KeyboardKey::KP4:
      return GLFW_KEY_KP_4;
    case KeyboardKey::KP5:
      return GLFW_KEY_KP_5;
    case KeyboardKey::KP6:
      return GLFW_KEY_KP_6;
    case KeyboardKey::KP7:
      return GLFW_KEY_KP_7;
    case KeyboardKey::KP8:
      return GLFW_KEY_KP_8;
    case KeyboardKey::KP9:
      return GLFW_KEY_KP_9;
    case KeyboardKey::KPDecimal:
      return GLFW_KEY_KP_DECIMAL;
    case KeyboardKey::KPDivide:
      return GLFW_KEY_KP_DIVIDE;
    case KeyboardKey::KPMultiply:
      return GLFW_KEY_KP_MULTIPLY;
    case KeyboardKey::KPSubtract:
      return GLFW_KEY_KP_SUBTRACT;
    case KeyboardKey::KPAdd:
      return GLFW_KEY_KP_ADD;
    case KeyboardKey::KPEnter:
      return GLFW_KEY_KP_ENTER;
    case KeyboardKey::KPEqual:
      return GLFW_KEY_KP_EQUAL;
    case KeyboardKey::LeftShift:
      return GLFW_KEY_LEFT_SHIFT;
    case KeyboardKey::LeftControl:
      return GLFW_KEY_LEFT_CONTROL;
    case KeyboardKey::LeftAlt:
      return GLFW_KEY_LEFT_ALT;
    case KeyboardKey::LeftSuper:
      return GLFW_KEY_LEFT_SUPER;
    case KeyboardKey::RightShift:
      return GLFW_KEY_RIGHT_SHIFT;
    case KeyboardKey::RightControl:
      return GLFW_KEY_RIGHT_CONTROL;
    case KeyboardKey::RightAlt:
      return GLFW_KEY_RIGHT_ALT;
    case KeyboardKey::RightSuper:
      return GLFW_KEY_RIGHT_SUPER;
    case KeyboardKey::Menu:
      return GLFW_KEY_MENU;

    default:
      return GLFW_KEY_UNKNOWN;
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

inline int mouse_button_to_glfw(MouseButton button) {
  switch (button) {
    case MouseButton::Left:
      return GLFW_MOUSE_BUTTON_LEFT;
    case MouseButton::Right:
      return GLFW_MOUSE_BUTTON_RIGHT;
    case MouseButton::Middle:
      return GLFW_MOUSE_BUTTON_MIDDLE;

    default:
      return GLFW_KEY_UNKNOWN;
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
  if (glfw_mods & GLFW_MOD_SHIFT) {
    mods = mods | InputModifierKey::Shift;
  }
  if (glfw_mods & GLFW_MOD_CONTROL) {
    mods = mods | InputModifierKey::Control;
  }
  if (glfw_mods & GLFW_MOD_ALT) {
    mods = mods | InputModifierKey::Alt;
  }
  if (glfw_mods & GLFW_MOD_SUPER) {
    mods = mods | InputModifierKey::Super;
  }
  if (glfw_mods & GLFW_MOD_CAPS_LOCK) {
    mods = mods | InputModifierKey::CapsLock;
  }
  if (glfw_mods & GLFW_MOD_NUM_LOCK) {
    mods = mods | InputModifierKey::NumLock;
  }

  return mods;
}

}  // namespace mge

#endif  // MGE_EVENTS_KEYBOARD