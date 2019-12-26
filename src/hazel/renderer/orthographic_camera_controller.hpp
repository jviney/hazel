#pragma once

#include "hazel/core/timestep.hpp"
#include "hazel/events/application_event.hpp"
#include "hazel/events/mouse_event.hpp"
#include "hazel/renderer/orthographic_camera.hpp"

namespace hazel
{

class OrthographicCameraController
{
public:
  OrthographicCameraController(float aspect_ratio, bool rotation = false);

  void on_update(Timestep);
  void on_event(Event&);

  const auto& camera() const { return camera_; }
  auto& camera() { return camera_; }

  float zoom_level() const { return zoom_level_; }
  void set_zoom_level(float level) { zoom_level_ = level; }

private:
  bool on_mouse_scrolled(MouseScrolledEvent&);
  bool on_window_resized(WindowResizeEvent&);

  float aspect_ratio_{};
  float zoom_level_{1};
  OrthographicCamera camera_;

  glm::vec3 camera_position_{0.0f};
  float camera_rotation_{};
  float camera_translation_speed_{5.0f};
  float camera_rotation_speed_{180.0f};

  bool rotation_{};
};

} // namespace hazel
