#ifndef MGE_EVENTS_EVENT_MANAGER
#define MGE_EVENTS_EVENT_MANAGER

#include "mgepch.hh"

#include "event.hh"

namespace mge {

// type map implementation from
// https://en.cppreference.com/w/cpp/types/type_info/hash_code
using TypeInfoRef = std::reference_wrapper<const std::type_info>;

struct Hasher {
  std::size_t operator()(TypeInfoRef code) const {
    return code.get().hash_code();
  }
};

struct EqualTo {
  bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const {
    return lhs.get() == rhs.get();
  }
};

template <class... T>
using DispatcherMap =
    std::unordered_map<TypeInfoRef,
                       std::unique_ptr<std::variant<EventDispatcher<T>...>>,
                       Hasher, EqualTo>;

template <class... T>
class _EventManager {
 public:
  virtual ~_EventManager();

  static std::shared_ptr<_EventManager> create();
  void terminate();
  static _EventManager& get_instance();
  template <class E>
  inline EventDispatcher<E>& get_dispatcher(E event_type) {
    return std::get<EventDispatcher<E>>(*m_event_dispatchers.at(typeid(E)));
  }

 private:
  _EventManager() = default;

  static std::shared_ptr<_EventManager> s_instance;
  DispatcherMap<T...> m_event_dispatchers;
};

#define DeclareEventManager(...) using EventManager = _EventManager<__VA_ARGS__>

#define AddEventListener(event_type, func, arg) \
  EventManager::get_instance()                  \
      .get_dispatcher(event_type)               \
      .add_listener(event_type, std::bind(&func, arg, std::placeholders::_1))

#define RemoveEventListener(event_type, handle) \
  EventManager::get_instance()                  \
      .get_dispatcher(event_type)               \
      .remove_listener(handle)

#define SendEvent(_event)                \
  EventManager::get_instance()           \
      .get_dispatcher(_event.get_type()) \
      .send_event(_event)

}  // namespace mge

#endif  // MGE_EVENTS_EVENT_MANAGER