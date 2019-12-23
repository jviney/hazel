#pragma once

namespace hazel
{

enum class ShaderDataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

static uint32_t shader_data_type_size(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::None:
    HZ_CORE_ASSERT(false, "unknown ShaderDataType");
    return 0;

  case ShaderDataType::Float:
    return sizeof(float);
  case ShaderDataType::Float2:
    return sizeof(float) * 2;
  case ShaderDataType::Float3:
    return sizeof(float) * 3;
  case ShaderDataType::Float4:
    return sizeof(float) * 4;

  case ShaderDataType::Mat3:
    return sizeof(float) * 3 * 3;
  case ShaderDataType::Mat4:
    return sizeof(float) * 4 * 4;

  case ShaderDataType::Int:
    return sizeof(int);
  case ShaderDataType::Int2:
    return sizeof(int) * 2;
  case ShaderDataType::Int3:
    return sizeof(int) * 3;
  case ShaderDataType::Int4:
    return sizeof(int) * 4;

  case ShaderDataType::Bool:
    return sizeof(bool);
  }
}

struct BufferElement {
  ShaderDataType type;
  std::string name;
  uint32_t size;
  uint32_t offset = 0;
  bool normalized = false;

  BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
      : type(type), name(name), size(shader_data_type_size(type)), normalized(normalized) {}

  uint32_t component_count() const {
    switch (type) {
    case ShaderDataType::None:
      HZ_CORE_ASSERT(false, "unknown ShaderDataType");
      return 0;

    case ShaderDataType::Int:
    case ShaderDataType::Float:
    case ShaderDataType::Bool:
      return 1;

    case ShaderDataType::Int2:
    case ShaderDataType::Float2:
      return 2;

    case ShaderDataType::Int3:
    case ShaderDataType::Float3:
      return 3;

    case ShaderDataType::Int4:
    case ShaderDataType::Float4:
      return 4;

    case ShaderDataType::Mat3:
      return 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4;
    }
  }
};

class BufferLayout
{
public:
  BufferLayout(){};
  BufferLayout(const std::initializer_list<BufferElement>& elements) : elements_(elements) {
    calculate_offsets_and_stride();
  }

  inline const auto& elements() const { return elements_; }

  auto begin() { return elements_.begin(); }
  auto end() { return elements_.end(); }

  auto begin() const { return elements_.begin(); }
  auto end() const { return elements_.end(); }

  uint32_t stride() const { return stride_; }

private:
  void calculate_offsets_and_stride() {
    uint32_t offset = 0;
    stride_ = 0;

    for (auto& element : elements_) {
      element.offset = offset;
      offset += element.size;
      stride_ += element.size;
    }
  };

  std::vector<BufferElement> elements_;
  uint32_t stride_ = 0;
};

class VertexBuffer
{
public:
  virtual ~VertexBuffer() {}

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual const BufferLayout& layout() const = 0;
  virtual void set_layout(const BufferLayout& layout) = 0;

  static Scope<VertexBuffer> create(float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
  virtual ~IndexBuffer() {}

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual uint32_t count() const = 0;

  static Scope<IndexBuffer> create(uint32_t* vertices, uint32_t size);
};

} // namespace hazel
