#pragma once

#include <GLFW/glfw3.h>

#include "hazel/window.hpp"

namespace hazel
{

class WindowsWindow : public Window
{
public:
  WindowsWindow(const WindowProps& props);
  virtual ~WindowsWindow();

  void on_update() override;

  unsigned int width() const override { return data_.width; }
  unsigned int height() const override { return data_.height; }

  // Window attributes
  void set_event_callback(const EventCallbackFn& callback) override {
    data_.event_callback = callback;
  };
  void set_vsync(bool enabled) override;
  bool is_vsync() const override;

private:
  virtual void init(const WindowProps& props);
  virtual void shutdown();

  GLFWwindow* window_;

  struct WindowData {
    std::string title;
    unsigned int width, height;
    bool vsync;
    EventCallbackFn event_callback;
  };

  WindowData data_;
};

} // namespace hazel
