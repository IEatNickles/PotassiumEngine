#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/quaternion.hpp"

struct TransformComponent {
public:
  TransformComponent() : TransformComponent(glm::vec3()) {}
  TransformComponent(glm::vec3 position)
      : TransformComponent(position, glm::identity<glm::quat>(), glm::vec3(1)) {
  }
  TransformComponent(glm::vec3 const &position, glm::quat const &rotation,
                     glm::vec3 const &scale)
      : position(position), rotation(rotation), scale(scale) {}

  glm::vec3 position = glm::vec3(0.0f);
  glm::quat rotation = glm::identity<glm::quat>();
  glm::vec3 scale = glm::vec3(1.0f);

  operator glm::mat4() {
    return glm::translate(
        glm::mat4_cast(rotation) * glm::scale(glm::mat4(1), scale), position);
  }
};
