#include "timer.hh"

namespace mge {
Timer::Timer() {
  reset();
  MGE_INFO("Timer created and initialized");
}

Timer::~Timer() { MGE_INFO("Timer terminated"); }

void Timer::tick() {
  auto now = std::chrono::high_resolution_clock::now();
  m_delta_time = std::chrono::duration<float, std::chrono::seconds::period>(now - m_reference_time).count();
  m_reference_time = now;
}

void Timer::reset() { m_reference_time = std::chrono::high_resolution_clock::now(); }
}  // namespace mge
