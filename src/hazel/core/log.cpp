#include "hazel/core/log.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace hazel
{
std::shared_ptr<spdlog::logger> Log::core_logger_;
std::shared_ptr<spdlog::logger> Log::client_logger_;

void Log::init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");

  core_logger_ = spdlog::stdout_color_mt("HAZEL");
  core_logger_->set_level(spdlog::level::trace);

  client_logger_ = spdlog::stdout_color_mt("APP");
  client_logger_->set_level(spdlog::level::trace);
}

} // namespace hazel
