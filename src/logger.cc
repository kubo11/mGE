#include "logger.hh"

namespace mge {
std::shared_ptr<Logger> Logger::s_instance = nullptr;

Logger::Logger() {
  spdlog::set_pattern("%^[%T][%n][%l]%$: %v");

  m_mge_logger = spdlog::stdout_color_mt("Espert");
  m_mge_logger->set_level(spdlog::level::trace);
}

Logger::~Logger() {
  if (Logger::s_instance) terminate();
}

std::shared_ptr<Logger> Logger::create() {
  if (Logger::s_instance != nullptr) {
    throw std::runtime_error("The logger instance already exists!");
  }

  Logger::s_instance = std::shared_ptr<Logger>(new Logger());
  MGE_INFO("Logger created");

  return s_instance;
}

void Logger::terminate() {
  s_instance = nullptr;
  m_mge_logger = nullptr;
}

void Logger::set_level(LogLevel level) {
  s_instance->m_mge_logger->set_level(level);
}
}  // namespace mge
