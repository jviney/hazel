#include "hazel/core.hpp"

#include "hazel/log.hpp"
#include "hazel/application.hpp"
#include "hazel/events/event.hpp"
#include "hazel/imgui/imgui_layer.hpp"

#include <imgui.h>
#include "hazel/platform/opengl/imgui_opengl_renderer.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace hazel
{

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}
ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::on_attach() {
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGuiIO& io = ImGui::GetIO();

  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

  // TEMPORARY: should eventually use hazel key codes
  io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
  io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

  ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::on_detach() {}

void ImGuiLayer::on_update() {
  ImGuiIO& io = ImGui::GetIO();
  auto& app = Application::get();

  auto const& window = app.window();
  io.DisplaySize = ImVec2(window.width(), window.height());
  io.DisplayFramebufferScale = ImVec2(window.content_scale_x(), window.content_scale_y());

  float time = (float) glfwGetTime();
  io.DeltaTime = time_ > 0.0 ? time - time_ : 1.0f / 60.0f;
  time_ = time;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  static bool show = true;
  ImGui::ShowDemoWindow(&show);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::on_event(Event& event) {
  auto dispatcher = EventDispatcher(event);
  dispatcher.dispatch<MouseButtonPressedEvent>(
      HZ_BIND_EVENT_FN(ImGuiLayer::on_mouse_button_pressed_event));
  dispatcher.dispatch<MouseButtonReleasedEvent>(
      HZ_BIND_EVENT_FN(ImGuiLayer::on_mouse_button_released_event));
  dispatcher.dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::on_mouse_moved_event));
  dispatcher.dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::on_mouse_scrolled_event));
  dispatcher.dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::on_key_pressed_event));
  dispatcher.dispatch<KeyTypedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::on_key_typed_event));
  dispatcher.dispatch<KeyReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::on_key_released_event));
  dispatcher.dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::on_window_resize_event));
}

bool ImGuiLayer::on_mouse_button_pressed_event(MouseButtonPressedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.mouse_button()] = true;

  return false;
}

bool ImGuiLayer::on_mouse_button_released_event(MouseButtonReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.mouse_button()] = false;

  return false;
}

bool ImGuiLayer::on_mouse_moved_event(MouseMovedEvent& e) {
  HZ_CORE_TRACE("ImGui::on_mouse_moved_event");
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2(e.x(), e.y());

  return false;
}

bool ImGuiLayer::on_mouse_scrolled_event(MouseScrolledEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheelH += e.x_offset();
  io.MouseWheel += e.y_offset();

  return false;
}

bool ImGuiLayer::on_key_pressed_event(KeyPressedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();

  io.KeysDown[e.key_code()] = true;

  io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
  io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

  return false;
}

bool ImGuiLayer::on_key_released_event(KeyReleasedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[e.key_code()] = false;

  return false;
}

bool ImGuiLayer::on_key_typed_event(KeyTypedEvent& e) {
  ImGuiIO& io = ImGui::GetIO();

  int key_code = e.key_code();
  if (key_code > 0 && key_code < 0x10000) {
    io.AddInputCharacter((unsigned short) key_code);
  }

  return false;
}

bool ImGuiLayer::on_window_resize_event(WindowResizeEvent& e) {
  // These io changes are probably not necessary because on_update sets them anyway.
  auto const& window = Application::get().window();
  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize = ImVec2(e.width(), e.height());
  io.DisplayFramebufferScale = ImVec2(window.content_scale_x(), window.content_scale_y());

  glViewport(0, 0, e.width(), e.height());

  return false;
}

} // namespace hazel
