#ifndef MGE_EVENTS_EVENT_MANAGER
#define MGE_EVENTS_EVENT_MANAGER

#include "../mgepch.hh"

#include "event.hh"

namespace {
template <class A, class B>
std::function<bool(A&)> cast_to_base_function(const std::function<bool(B&)>& func_b) {
  return [func_b](A& a) -> bool {
    B& b = dynamic_cast<B&>(a);
    return func_b(b);
  };
}
}  // namespace

namespace mge {

// type map implementation from
// https://en.cppreference.com/w/cpp/types/type_info/hash_code
using TypeInfoRef = std::reference_wrapper<const std::type_info>;

struct Hasher {
  std::size_t operator()(TypeInfoRef code) const { return code.get().hash_code(); }
};

struct EqualTo {
  bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const { return lhs.get() == rhs.get(); }
};

template <class... T>
using DispatcherMap =
    std::unordered_map<TypeInfoRef, std::unique_ptr<std::variant<EventDispatcher<T>...>>, Hasher, EqualTo>;

template <class... T>
class _EventManager {
 public:
  virtual ~_EventManager() {
    if (s_instance) terminate();
  }

  inline static std::shared_ptr<_EventManager> create() {
    s_instance = std::shared_ptr<_EventManager<T...>>(new _EventManager());
    return s_instance;
  }

  inline void terminate() { s_instance = nullptr; }

  inline static _EventManager& get_instance() { return *s_instance; }

  template <class E>
  inline EventDispatcher<E>& get_dispatcher(E event_type) {
    return std::get<EventDispatcher<E>>(*m_event_dispatchers.at(typeid(E)));
  }

  template <class E, class F, class G>
  inline void add_listener(E event_type, bool (G::*func_ptr)(F&), G* arg) {
    std::function<bool(G*, F&)> func = func_ptr;
    std::function<bool(Event<E>&)> cast_func = cast_to_base_function<Event<E>, F>(
        static_cast<std::function<bool(F&)>>(std::bind(func, arg, std::placeholders::_1)));
    get_dispatcher(event_type).add_listener(event_type, cast_func);
  }

 private:
  inline static std::shared_ptr<_EventManager> s_instance = nullptr;
  DispatcherMap<T...> m_event_dispatchers;

  _EventManager() { (register_dispatcher<T>(), ...); }

  template <class E>
  void register_dispatcher() {
    auto var_ptr = new std::variant<EventDispatcher<T>...>(EventDispatcher<E>());
    auto var_uptr = std::unique_ptr<std::variant<EventDispatcher<T>...>>(var_ptr);
    m_event_dispatchers.insert({typeid(E), std::move(var_uptr)});
  }
};

#define DeclareEventManager(...) using EventManager = _EventManager<__VA_ARGS__>

#define DeclareClientEventManager(...) using EventManager = mge::_EventManager<__VA_ARGS__>

#define AddEventListener(event_type, func, arg) EventManager::get_instance().add_listener(event_type, &func, arg)

#define RemoveEventListener(event_type, handle) \
  EventManager::get_instance().get_dispatcher(event_type).remove_listener(handle)

#define SendEvent(_event) EventManager::get_instance().get_dispatcher(_event.get_type()).send_event(_event)

}  // namespace mge

#endif  // MGE_EVENTS_EVENT_MANAGER