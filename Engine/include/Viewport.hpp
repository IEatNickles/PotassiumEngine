#pragma once

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>

enum ProjectionType {
  PROJ_ORTHOGRAPHIC,
  PROJ_PERSPECTIVE,
};

struct Viewport {
  ProjectionType type;

  glm::vec3 position;
  glm::quat rotation;

  float size;
  float aspect;
  float z_near, z_far;
};
