#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

#include "glm/fwd.hpp"

class Shader {
public:
  Shader();
  Shader(std::filesystem::path const &vs_path,
         std::filesystem::path const &fs_path);

  void bind() const;

  void set_mat4(std::string const &name, glm::mat4 const &value);

private:
  uint32_t m_program;
};
