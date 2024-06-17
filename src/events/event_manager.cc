#include "events/event_manager.hh"

namespace mge {
template <class... T>
std::shared_ptr<_EventManager<T...>> _EventManager<T...>::s_instance = nullptr;

template <class... T>
_EventManager<T...>::~_EventManager() {
  if (s_instance) terminate();
}

template <class... T>
std::shared_ptr<_EventManager<T...>> _EventManager<T...>::create() {
  s_instance = std::make_shared<_EventManager<T...>>();
}

template <class... T>
_EventManager<T...>& _EventManager<T...>::get_instance() {
  return *s_instance;
}

template <class... T>
void _EventManager<T...>::terminate() {
  s_instance = nullptr;
}

}  // namespace mge
