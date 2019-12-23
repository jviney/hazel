#pragma once

#include "hazel/renderer/orthographic_camera.hpp"
#include "hazel/renderer/render_command.hpp"
#include "hazel/renderer/renderer_api.hpp"
#include "hazel/renderer/shader.hpp"
#include "hazel/renderer/vertex_array.hpp"

namespace hazel
{

class Renderer
{
public:
  static void begin_scene(OrthographicCamera&);
  static void end_scene();

  static void submit(const Shader*, const VertexArray*,
                     const glm::mat4& transform = glm::mat4(1.0f));

  static inline RendererAPI::API api() { return RendererAPI::api(); }

private:
  struct SceneData {
    glm::mat4 view_projection_matrix;
  };

  static SceneData* scene_data_;
};

} // namespace hazel
