#include "hazel/core/core.hpp"
#include "hazel/renderer/vertex_array.hpp"

namespace hazel
{

class OpenGLVertexArray : public VertexArray
{
public:
  OpenGLVertexArray();
  ~OpenGLVertexArray();

  void bind() const override;
  void unbind() const override;

  void add_vertex_buffer(Ref<VertexBuffer>) override;
  void set_index_buffer(Ref<IndexBuffer>) override;

  const std::vector<Ref<VertexBuffer>>& vertex_buffers() const override { return vertex_buffers_; }
  const Ref<IndexBuffer>& index_buffer() const override { return index_buffer_; }

private:
  uint32_t renderer_id_;

  std::vector<Ref<VertexBuffer>> vertex_buffers_;
  Ref<IndexBuffer> index_buffer_;
};

} // namespace hazel
