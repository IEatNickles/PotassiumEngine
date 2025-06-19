#include "Shader.hpp"

#include "Log.hpp"

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fstream>

#include <iostream>
#include <sstream>

#include "glad/glad.h"
#include "glm/ext/matrix_float4x4.hpp"

uint32_t gen_shader(uint32_t type, char const *source) {
  uint32_t shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    char *log = new char[len];
    glGetShaderInfoLog(shader, len, &len, log);
    DEBUG_ERROR("Failed to compile shader: {}", log);
  }

  return shader;
}

std::string read_all(std::filesystem::path const &path) {
  std::ifstream file(path);
  if (file.fail()) {
    std::cout << "File Error: " << strerror(errno) << "\n";
  }
  std::stringstream ss;
  ss << file.rdbuf();
  if (ss.fail()) {
    std::cout << "Stream Error: " << strerror(errno) << "\n";
  }
  return ss.str();
}

Shader::Shader() {}
Shader::Shader(std::filesystem::path const &vs_path,
               std::filesystem::path const &fs_path) {
  std::string vs_cont = read_all(vs_path);
  std::string fs_cont = read_all(fs_path);
  uint32_t vs = gen_shader(GL_VERTEX_SHADER, vs_cont.c_str());
  uint32_t fs = gen_shader(GL_FRAGMENT_SHADER, fs_cont.c_str());

  m_program = glCreateProgram();
  glAttachShader(m_program, vs);
  glAttachShader(m_program, fs);

  glLinkProgram(m_program);

  int status = 0;
  glGetProgramiv(m_program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    int len = 0;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
    char *log = new char[len];
    glGetProgramInfoLog(m_program, len, &len, log);
    DEBUG_ERROR("Failed to link shader program: {}", log);
  }

  glValidateProgram(m_program);
  glGetProgramiv(m_program, GL_VALIDATE_STATUS, &status);
  if (status == GL_FALSE) {
    int len = 0;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
    char *log = new char[len];
    glGetProgramInfoLog(m_program, len, &len, log);
    DEBUG_ERROR("Failed to validate shader program: {}", log);
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
}

void Shader::bind() const { glUseProgram(m_program); }

void Shader::set_mat4(std::string const &name, glm::mat4 const &value) {
  int id = glGetUniformLocation(m_program, name.c_str());
  if (id < 0)
    DEBUG_ERROR("Could not find uniform matrix4x4 '{}'", name);
  glUniformMatrix4fv(id, 1, GL_FALSE, &value[0][0]);
}
