#ifndef MGE_TIMER_HH
#define MGE_TIMER_HH

#include "mgepch.hh"

namespace mge {
class Timer {
 public:
  Timer();
  ~Timer();

  void tick();
  void reset();
  inline const float get_dt() const { return m_delta_time; };

 private:
  std::chrono::high_resolution_clock::time_point m_reference_time;
  float m_delta_time;
};
}  // namespace mge

#endif  // MGE_TIMER_HH