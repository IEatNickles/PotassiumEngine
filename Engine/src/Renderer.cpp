#include "Renderer.hpp"
#include "glm/gtc/quaternion.hpp"

#include "glad/glad.h"

Mesh::Mesh() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
}
Mesh::Mesh(std::vector<glm::vec3> const &positions,
           std::vector<int> const &indices)
    : Mesh() {
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3),
               positions.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
               indices.data(), GL_STATIC_DRAW);
  index_count = indices.size();
}

Renderer::Renderer() {}

void Renderer::init() {
  m_shader = Shader("Engine/assets/shaders/default.vert",
                    "Engine/assets/shaders/default.frag");

  glGenBuffers(1, &m_vp_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, m_vp_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_vp_ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
}

void Renderer::set_viewport(Viewport const &view) {
  glm::mat4 v = glm::inverse(
      glm::translate(glm::mat4_cast(view.rotation), view.position));
  glm::mat4 p;
  switch (view.type) {
  case PROJ_ORTHOGRAPHIC:
    p = glm::ortho(-view.size, view.size, -view.size * view.aspect,
                   view.size * view.aspect, view.z_near, view.z_far);
    break;
  case PROJ_PERSPECTIVE:
    p = glm::perspective(view.size, view.aspect, view.z_near, view.z_far);
    break;
  }

  glBindBuffer(GL_UNIFORM_BUFFER, m_vp_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_vp_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &(p * v)[0][0]);
}

void Renderer::draw_mesh(Mesh const &mesh, glm::mat4 const &transform) {
  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);

  glBindBuffer(GL_UNIFORM_BUFFER, m_vp_ubo);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_vp_ubo);

  m_shader.bind();
  m_shader.set_mat4("u_world", transform);

  glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr);
}
