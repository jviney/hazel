#pragma once

#include "hazel/core.hpp"
#include "hazel/events/event.hpp"

namespace hazel
{
class HZ_API Application
{
public:
  Application();
  virtual ~Application();

  virtual void run();
};

extern std::unique_ptr<Application> create_application();

} // namespace hazel
