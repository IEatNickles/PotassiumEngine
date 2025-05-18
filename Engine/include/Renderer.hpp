#pragma once

#include "Shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include <cstdint>
#include <vector>

#include "Viewport.hpp"

struct Mesh {
  Mesh();
  Mesh(std::vector<glm::vec3> const &positions,
       std::vector<int> const &indices);

  uint32_t vbo, ebo, vao;
  uint32_t index_count;
};

class Renderer {
public:
  Renderer();

  void init();

  void draw_mesh(Mesh const &mesh, glm::mat4 const &transform);

  void set_viewport(Viewport const &view);

private:
  uint32_t m_vp_ubo;

  Shader m_shader;
};
