#include "logger.hh"

namespace mge {
std::unique_ptr<Logger> Logger::s_instance = nullptr;

Logger::Logger() {
  spdlog::set_pattern("%^[%T][%n][%l]%$: %v");

  m_mge_logger = spdlog::stdout_color_mt("Espert");
  m_mge_logger->set_level(spdlog::level::trace);
}

Logger::~Logger() { Logger::s_instance = nullptr; }

Logger& Logger::create() {
  if (Logger::s_instance != nullptr) {
    throw std::runtime_error("The logger instance already exists!");
  }

  Logger::s_instance = std::unique_ptr<Logger>(new Logger());
  MGE_INFO("Logger created");

  return *s_instance;
}

void Logger::set_level(LogLevel level) {
  s_instance->m_mge_logger->set_level(level);
}
}  // namespace mge
