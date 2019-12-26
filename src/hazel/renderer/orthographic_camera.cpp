#include "hazel/core/core.hpp"
#include "hazel/renderer/orthographic_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace hazel
{

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) {
  set_projection(left, right, bottom, top);
}

void OrthographicCamera::set_projection(float left, float right, float bottom, float top) {
  projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

void OrthographicCamera::recalculate_view_matrix() {
  auto translation_transform = glm::translate(glm::mat4(1.0f), position_);
  auto rotation_transform =
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation_), glm::vec3(0, 0, 1));
  auto transform = translation_transform * rotation_transform;

  view_matrix_ = glm::inverse(transform);
  view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

} // namespace hazel
