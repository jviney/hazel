#include "hazel/core/core.hpp"
#include "hazel/renderer/render_command.hpp"
#include "hazel/platform/opengl/opengl_renderer_api.hpp"

namespace hazel
{

RendererAPI* RenderCommand::renderer_api_ = new OpenGLRendererAPI();

}
