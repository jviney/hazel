#include "hazel/core.hpp"
#include "hazel/log.hpp"
#include "hazel/events/event.hpp"
#include "hazel/events/key_event.hpp"
#include "hazel/events/application_event.hpp"
#include "hazel/events/mouse_event.hpp"
#include "hazel/platform/opengl/opengl_context.hpp"
#include "hazel/platform/windows/windows.hpp"

#include <glad/glad.h>

static void glfw_error_callback(int error, const char* description) {
  HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

namespace hazel
{

static bool is_glfw_initialized = false;

Scope<Window> Window::create(const WindowProps& props) {
  return std::make_unique<WindowsWindow>(props);
}

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

    // Gets OpenGL 4.1 instead of 2.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwSetErrorCallback(glfw_error_callback);
    is_glfw_initialized = true;
  }

  window_ = glfwCreateWindow((int) props.width, (int) props.height, data_.title.c_str(), nullptr,
                             nullptr);

  context_ = std::make_unique<OpenGLContext>(window_);
  context_->init();

  glfwSetWindowUserPointer(window_, &data_);
  set_vsync(true);

  // Set GLFW callbacks
  glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
    auto& data = *(WindowData*) glfwGetWindowUserPointer(window);

    data.width = width;
    data.height = height;

    auto event = WindowResizeEvent(width, height);
    data.event_callback(event);
  });

  glfwSetWindowContentScaleCallback(window_, [](GLFWwindow* window, float x_scale, float y_scale) {
    auto& data = *(WindowData*) glfwGetWindowUserPointer(window);
    data.content_scale_x = x_scale;
    data.content_scale_y = y_scale;

    auto event = WindowResizeEvent(data.width, data.height);
    data.event_callback(event);
  });

  glfwGetWindowContentScale(window_, &data_.content_scale_x, &data_.content_scale_y);

  glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
    auto& data = *(WindowData*) glfwGetWindowUserPointer(window);

    auto event = WindowCloseEvent();
    data.event_callback(event);
  });

  glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int, int action, int) {
    auto& data = *(WindowData*) glfwGetWindowUserPointer(window);

    switch (action) {
    case GLFW_PRESS: {
      auto event = KeyPressedEvent(key, 0);
      data.event_callback(event);
      break;
    }

    case GLFW_RELEASE: {
      auto event = KeyReleasedEvent(key);
      data.event_callback(event);
      break;
    }

    case GLFW_REPEAT: {
      auto event = KeyPressedEvent(key, 1);
      data.event_callback(event);
      break;
    }
    }
  });

  glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int key_code) {
    auto& data = *(WindowData*) glfwGetWindowUserPointer(window);

    auto event = KeyTypedEvent(key_code);
    data.event_callback(event);
  });

  glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int) {
    auto& data = *(WindowData*) glfwGetWindowUserPointer(window);

    switch (action) {
    case GLFW_PRESS: {
      auto event = MouseButtonPressedEvent(button);
      data.event_callback(event);
      break;
    }

    case GLFW_RELEASE: {
      auto event = MouseButtonReleasedEvent(button);
      data.event_callback(event);
      break;
    }
    }
  });

  glfwSetScrollCallback(window_, [](GLFWwindow* window, double x_offset, double y_offset) {
    auto& data = *(WindowData*) glfwGetWindowUserPointer(window);

    auto event = MouseScrolledEvent(float(x_offset), float(y_offset));
    data.event_callback(event);
  });

  glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double x_position, double y_position) {
    auto& data = *(WindowData*) glfwGetWindowUserPointer(window);

    auto event = MouseMovedEvent(float(x_position), float(y_position));
    data.event_callback(event);
  });
}

void WindowsWindow::shutdown() { glfwDestroyWindow(window_); }

void WindowsWindow::on_update() {
  glfwPollEvents();
  context_->swap_buffers();
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
