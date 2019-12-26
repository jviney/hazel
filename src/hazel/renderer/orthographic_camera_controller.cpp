#include "hazel/core/core.hpp"
#include "hazel/core/input.hpp"
#include "hazel/events/key_codes.hpp"
#include "hazel/renderer/orthographic_camera_controller.hpp"

namespace hazel
{

OrthographicCameraController::OrthographicCameraController(float aspect_ratio, bool rotation)
    : aspect_ratio_(aspect_ratio),
      camera_(-aspect_ratio * zoom_level_, aspect_ratio * zoom_level_, -zoom_level_, zoom_level_),
      rotation_(rotation) {}

void OrthographicCameraController::on_update(Timestep ts) {
  if (Input::is_key_pressed(HZ_KEY_A)) {
    camera_position_.x -= camera_translation_speed_ * ts;
  } else if (Input::is_key_pressed(HZ_KEY_D)) {
    camera_position_.x += camera_translation_speed_ * ts;
  }

  if (Input::is_key_pressed(HZ_KEY_W)) {
    camera_position_.y += camera_translation_speed_ * ts;
  } else if (Input::is_key_pressed(HZ_KEY_S)) {
    camera_position_.y -= camera_translation_speed_ * ts;
  }

  if (Input::is_key_pressed(HZ_KEY_Q)) {
    camera_rotation_ += camera_rotation_speed_ * ts;
  } else if (Input::is_key_pressed(HZ_KEY_E)) {
    camera_rotation_ -= camera_rotation_speed_ * ts;
  }

  camera_.set_position(camera_position_);
  camera_.set_rotation(camera_rotation_);

  camera_translation_speed_ = zoom_level_;
}

void OrthographicCameraController::on_event(Event& e) {
  auto dispatcher = EventDispatcher(e);
  dispatcher.dispatch<MouseScrolledEvent>(
      HZ_BIND_EVENT_FN(OrthographicCameraController::on_mouse_scrolled));
  dispatcher.dispatch<WindowResizeEvent>(
      HZ_BIND_EVENT_FN(OrthographicCameraController::on_window_resized));
}

bool OrthographicCameraController::on_mouse_scrolled(MouseScrolledEvent& e) {
  zoom_level_ -= e.y_offset() * 0.25f;
  zoom_level_ = std::max(zoom_level_, 0.25f);
  camera_.set_projection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_,
                         zoom_level_);

  return false;
}

bool OrthographicCameraController::on_window_resized(WindowResizeEvent& e) {
  aspect_ratio_ = float(e.width()) / float(e.height());
  camera_.set_projection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_,
                         zoom_level_);

  return false;
}

} // namespace hazel
