#pragma once

#include "hazel/renderer/orthographic_camera.hpp"
#include "hazel/renderer/texture.hpp"

namespace hazel
{

class Renderer2D
{
public:
  static void init();
  static void shutdown();

  static void begin_scene(const OrthographicCamera&);
  static void end_scene();

  // Primitives
  static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
  static void draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
  static void draw_quad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>&);
  static void draw_quad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>&);

private:
};

} // namespace hazel
