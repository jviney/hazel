#include <fstream>

#include "hazel/core.hpp"
#include "hazel/log.hpp"
#include "hazel/platform/opengl/opengl_shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace hazel
{

static GLenum shader_type_from_string(const std::string& type) {
  if (type == "vertex") {
    return GL_VERTEX_SHADER;
  } else if (type == "fragment" || type == "pixel") {
    return GL_FRAGMENT_SHADER;
  }

  HZ_CORE_ASSERT(false, "unknown shader type");
  return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath) {
  auto source = read_file(filepath);
  auto shader_sources = pre_process(source);

  compile(shader_sources);
}

OpenGLShader::OpenGLShader(const std::string& vertex_source, const std::string& fragment_source) {
  auto sources = std::unordered_map<GLenum, std::string>();

  sources[GL_VERTEX_SHADER] = vertex_source;
  sources[GL_FRAGMENT_SHADER] = fragment_source;

  compile(sources);
}

void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& sources) {
  GLuint program = glCreateProgram();

  auto shader_ids = std::vector<GLenum>(sources.size());

  for (auto const& [shader_type, source] : sources) {
    GLuint shader = glCreateShader(shader_type);

    const GLchar* source_cstr = source.c_str();
    glShaderSource(shader, 1, &source_cstr, 0);
    glCompileShader(shader);

    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
      GLint max_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

      std::vector<GLchar> info_log(max_length);
      glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

      glDeleteShader(shader);

      HZ_CORE_ERROR("{}", info_log.data());
      HZ_CORE_ASSERT(false, "shader compilation failure");

      return;
    }

    glAttachShader(program, shader);

    shader_ids.push_back(shader);
  }

  glLinkProgram(program);

  GLint is_linked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int*) &is_linked);
  if (is_linked == GL_FALSE) {
    GLint max_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

    std::vector<GLchar> infoLog(max_length);
    glGetProgramInfoLog(program, max_length, &max_length, &infoLog[0]);

    glDeleteProgram(program);

    for (auto shader_id : shader_ids) {
      glDeleteShader(shader_id);
    }

    HZ_CORE_ERROR("{}", infoLog.data());
    HZ_CORE_ASSERT(false, "shader link failure");

    return;
  }

  for (auto shader_id : shader_ids) {
    glDetachShader(program, shader_id);
  }

  renderer_id_ = program;
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(renderer_id_); }

std::string OpenGLShader::read_file(const std::string& filepath) {
  auto result = std::string();
  auto in = std::ifstream(filepath, std::ios::in | std::ios::binary);

  if (!in) {
    HZ_CORE_ERROR("could not open file '{}'", filepath);
    return result;
  }

  in.seekg(0, std::ios::end);
  result.resize(in.tellg());
  in.seekg(0, std::ios::beg);

  in.read(&result[0], result.size());

  return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::pre_process(const std::string& source) {
  auto shader_sources = std::unordered_map<GLenum, std::string>();

  auto* type_token = "#type";
  auto type_token_length = strlen(type_token);
  auto pos = source.find(type_token, 0);

  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\n", pos);
    HZ_CORE_ASSERT(eol != std::string::npos, "syntax error");
    size_t begin = pos + type_token_length + 1;
    std::string type = source.substr(begin, eol - begin);
    HZ_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel",
                   "invalid shader type specification");

    size_t next_line_pos = source.find_first_not_of("\n", eol);
    pos = source.find(type_token, next_line_pos);

    shader_sources[shader_type_from_string(type)] =
        source.substr(next_line_pos, pos - (next_line_pos == std::string::npos ? source.size() - 1
                                                                               : next_line_pos));
  }

  return shader_sources;
}

void OpenGLShader::bind() const { glUseProgram(renderer_id_); }

void OpenGLShader::unbind() const { glUseProgram(0); }

void OpenGLShader::upload_uniform_int(const std::string& name, int value) const {
  const GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform1i(location, value);
}

void OpenGLShader::upload_uniform_float(const std::string& name, float value) const {
  const GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform1f(location, value);
}

void OpenGLShader::upload_uniform_float2(const std::string& name, const glm::vec2& value) const {
  const GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform2f(location, value.x, value.y);
}

void OpenGLShader::upload_uniform_float3(const std::string& name, const glm::vec3& value) const {
  const GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::upload_uniform_float4(const std::string& name, const glm::vec4& value) const {
  const GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::upload_uniform_mat3(const std::string& name, const glm::mat3& matrix) const {
  const GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::upload_uniform_mat4(const std::string& name, const glm::mat4& matrix) const {
  const GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

} // namespace hazel
