#pragma once

#include <glm/glm.hpp>

namespace hazel
{

class OrthographicCamera
{
public:
  OrthographicCamera(float left, float right, float bottom, float top);

  void set_projection(float left, float right, float bottom, float top);

  const auto& position() const { return position_; }

  void set_position(const glm::vec3& position) {
    position_ = position;
    recalculate_view_matrix();
  }

  float rotation() const { return rotation_; }

  void set_rotation(float rotation) {
    rotation_ = rotation;
    recalculate_view_matrix();
  }

  const auto& projection_matrix() const { return projection_matrix_; }
  const auto& view_matrix() const { return view_matrix_; }
  const auto& view_projection_matrix() const { return view_projection_matrix_; }

private:
  void recalculate_view_matrix();

  glm::mat4 projection_matrix_{};
  glm::mat4 view_matrix_{1.0f};
  glm::mat4 view_projection_matrix_{};

  glm::vec3 position_{0.0f, 0.0f, 0.0f};
  float rotation_ = 0.0f;
};

} // namespace hazel
