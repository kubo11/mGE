#ifndef MGE_LOGGER_HH
#define MGE_LOGGER_HH

#include "mgepch.hh"

namespace mge {
class Logger {
  using LogLevel = spdlog::level::level_enum;

 public:
  ~Logger();
  static std::shared_ptr<Logger> create(const std::string& name);
  inline static Logger& get_instance() { return *s_instance; }
  inline static std::shared_ptr<spdlog::logger> get_mge_logger() { return s_instance->m_mge_logger; }

  void terminate();

  static void set_level(LogLevel level);

 private:
  static std::shared_ptr<Logger> s_instance;
  std::shared_ptr<spdlog::logger> m_mge_logger;

  Logger(const std::string& name);
};
}  // namespace mge

#ifdef NDEBUG

#define MGE_TRACE(...)
#define MGE_DEBUG(...)
#define MGE_INFO(...)
#define MGE_WARN(...)
#define MGE_ERROR(...)
#define MGE_CRITICAL(...)

#else

#define MGE_TRACE(...) mge::Logger::get_mge_logger()->trace(__VA_ARGS__)
#define MGE_DEBUG(...) mge::Logger::get_mge_logger()->debug(__VA_ARGS__)
#define MGE_INFO(...) mge::Logger::get_mge_logger()->info(__VA_ARGS__)
#define MGE_WARN(...) mge::Logger::get_mge_logger()->warn(__VA_ARGS__)
#define MGE_ERROR(...) mge::Logger::get_mge_logger()->error(__VA_ARGS__)
#define MGE_CRITICAL(...) mge::Logger::get_mge_logger()->critical(__VA_ARGS__)

#endif  // NDEBUG

#endif  // MGE_LOGGER_HH