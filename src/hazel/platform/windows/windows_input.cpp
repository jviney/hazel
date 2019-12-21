#include "hazel/core.hpp"
#include "hazel/application.hpp"
#include "hazel/platform/windows/windows_input.hpp"

#include <GLFW/glfw3.h>

namespace hazel
{

std::unique_ptr<Input> Input::instance_ = std::make_unique<WindowsInput>();

bool WindowsInput::is_key_pressed_impl(int key_code) const {
  auto* window = static_cast<GLFWwindow*>(Application::get().window().native_window());
  auto state = glfwGetKey(window, key_code);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::is_mouse_button_pressed_impl(int button) const {
  auto window = static_cast<GLFWwindow*>(Application::get().window().native_window());
  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::mouse_position_impl() const {
  auto window = static_cast<GLFWwindow*>(Application::get().window().native_window());
  double x_pos, y_pos;
  glfwGetCursorPos(window, &x_pos, &y_pos);
  return {float(x_pos), float(y_pos)};
}

float WindowsInput::mouse_x_impl() const { return mouse_position().first; }

float WindowsInput::mouse_y_impl() const { return mouse_position().second; }

} // namespace hazel
