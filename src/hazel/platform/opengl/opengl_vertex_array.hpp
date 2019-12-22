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

  void add_vertex_buffer(std::shared_ptr<VertexBuffer>) override;
  void set_index_buffer(std::shared_ptr<IndexBuffer>) override;

  const std::vector<std::shared_ptr<VertexBuffer>>& vertex_buffers() const override {
    return vertex_buffers_;
  }
  const std::shared_ptr<IndexBuffer>& index_buffer() const override { return index_buffer_; }

private:
  uint32_t renderer_id_;

  std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers_;
  std::shared_ptr<IndexBuffer> index_buffer_;
};

} // namespace hazel
