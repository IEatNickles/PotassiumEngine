#pragma once

#include "Viewport.hpp"
#include "glm/ext/vector_float3.hpp"

struct CameraComponent {
  CameraComponent() {}
  CameraComponent(ProjectionType projection, float size, float aspect,
                  float z_near, float z_far, glm::vec3 const &clear_color)
      : projection(projection), size(size), aspect(aspect), z_near(z_near),
        z_far(z_far), clear_color(clear_color) {}

  ProjectionType projection = PROJ_ORTHOGRAPHIC;

  float size = 1.0f;
  float aspect = 16.0f / 9.0f;
  float z_near = 0.1f, z_far = 100.0f;

  glm::vec3 clear_color = glm::vec3(1.0f, 0.0f, 1.0f);
};
