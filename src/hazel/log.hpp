#pragma once

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

#include "hazel/core.hpp"

namespace hazel
{

class HZ_API Log
{
public:
  static void init();

  inline static std::shared_ptr<spdlog::logger>& core_logger() { return core_logger_; }
  inline static std::shared_ptr<spdlog::logger>& client_logger() { return client_logger_; }

  Log();
  ~Log();

private:
  static std::shared_ptr<spdlog::logger> core_logger_;
  static std::shared_ptr<spdlog::logger> client_logger_;
};

} // namespace hazel

#define HZ_CORE_TRACE(...) ::hazel::Log::core_logger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...) ::hazel::Log::core_logger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...) ::hazel::Log::core_logger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...) ::hazel::Log::core_logger()->error(__VA_ARGS__)
#define HZ_CORE_FATAL(...) ::hazel::Log::core_logger()->fatal(__VA_ARGS__)

#define HZ_TRACE(...) ::hazel::Log::client_logger()->trace(__VA_ARGS__)
#define HZ_INFO(...) ::hazel::Log::client_logger()->info(__VA_ARGS__)
#define HZ_WARN(...) ::hazel::Log::client_logger()->warn(__VA_ARGS__)
#define HZ_ERROR(...) ::hazel::Log::client_logger()->error(__VA_ARGS__)
#define HZ_FATAL(...) ::hazel::Log::client_logger()->fatal(__VA_ARGS__)
