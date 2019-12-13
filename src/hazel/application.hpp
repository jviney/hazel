#pragma once

#include "hazel/core.hpp"
#include "hazel/events/event.hpp"
#include "hazel/events/application_event.hpp"
#include "hazel/window.hpp"

namespace hazel
{
class HZ_API Application
{
public:
  Application();
  virtual ~Application();

  virtual void run();

  virtual void on_event(Event& e);

private:
  virtual bool on_window_close(WindowCloseEvent& e);
  std::unique_ptr<Window> window_;
  bool running_ = true;
};

extern std::unique_ptr<Application> create_application();

} // namespace hazel
