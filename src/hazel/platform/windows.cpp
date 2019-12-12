#include "hazel/core.hpp"

#include "hazel/platform/windows.hpp"

namespace hazel
{
static bool is_glfw_initialized = false;

Window* Window::create(const WindowProps& props) { return new WindowsWindow(props); }

WindowsWindow::WindowsWindow(const WindowProps& props) { init(props); }
WindowsWindow::~WindowsWindow() { shutdown(); }

void WindowsWindow::init(const WindowProps& props) {
  data_.title = props.title;
  data_.width = props.width;
  data_.height = props.height;

  HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

  if (!is_glfw_initialized) {
    // TODO: glfwTerminate on system shutdown
    int success = glfwInit();
    HZ_CORE_ASSERT(success, "Could not initialize GLFW!");

    is_glfw_initialized = true;
  }

  window_ = glfwCreateWindow((int) props.width, (int) props.height, data_.title.c_str(), nullptr,
                             nullptr);
  glfwMakeContextCurrent(window_);
  glfwSetWindowUserPointer(window_, &data_);
  set_vsync(true);
}

void WindowsWindow::shutdown() { glfwDestroyWindow(window_); }
void WindowsWindow::on_update() {
  glfwPollEvents();
  glfwSwapBuffers(window_);
}

void WindowsWindow::set_vsync(bool enabled) {
  if (enabled) {
    glfwSwapInterval(1);
  } else {
    glfwSwapInterval(0);
  }

  data_.vsync = enabled;
}

bool WindowsWindow::is_vsync() const { return data_.vsync; }

} // namespace hazel
